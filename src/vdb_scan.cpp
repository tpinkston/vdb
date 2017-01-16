#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_scan.h"

#include "vdis_appearance.h"
#include "vdis_pdus.h"
#include "vdis_string.h"
#include "vdis_variable_parameter_records.h"

bool vdb::scan::scanning = false;
bool vdb::scan::associations = false;
bool vdb::scan::lasers = false;
bool vdb::scan::fires = false;
bool vdb::scan::collisions = false;
bool vdb::scan::entities = false;
bool vdb::scan::objects = false;

vdb::scan::entities_t           vdb::scan::current_entities;
vdb::scan::lasers_t             vdb::scan::current_lasers;
vdb::scan::carriers_t           vdb::scan::current_carriers;
vdb::scan::targets_t            vdb::scan::current_targets;
vdb::scan::entity_requests_t    vdb::scan::current_requests;
std::ostream                   &vdb::scan::err = std::cerr;
std::ostream                   &vdb::scan::out = std::cout;

// ----------------------------------------------------------------------------
void vdb::scan::scan_all(void)
{
    scanning = true;
    associations = true;
    lasers = true;
    fires = true;
    collisions = true;
    entities = true;
    objects = true;
}

// ----------------------------------------------------------------------------
bool vdb::scan::parse(const std::string &value)
{
    std::vector<std::string>
        values;
    bool
        success = false;

    if (vdis::tokenize_csv(vdis::trim(value), values) > 0)
    {
        success = true;

        for(uint32_t i = 0; success and (i < values.size()); ++i)
        {
            std::string item = vdis::trim(values[i]);

            if (item == "associations")
            {
                scanning = true;
                associations = true;
            }
            else if (item == "lasers")
            {
                scanning = true;
                lasers = true;
            }
            else if (item == "fires")
            {
                scanning = true;
                fires = true;
            }
            else if (item == "collisions")
            {
                scanning = true;
                collisions = true;
            }
            else if (item == "entities")
            {
                scanning = true;
                entities = true;
            }
            else if (item == "objects")
            {
                scanning = true;
                objects = true;
            }
            else
            {
                success = false;
            }
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
void vdb::scan::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
{
    if (pdu.base())
    {
        switch(pdu.type())
        {
            case vdis::PDU_TYPE_ENTITY_STATE:
            {
                const vdis::entity_state_pdu_t *base = SCAST(
                    vdis::entity_state_pdu_t,
                    pdu.base());

                process_entity_state(data, *base);
                process_entity_associations(data, *base);
                break;
            }
            case vdis::PDU_TYPE_FIRE:
            {
                const vdis::fire_pdu_t *base = SCAST(
                    vdis::fire_pdu_t,
                    pdu.base());

                process_fire(data, *base);
                break;
            }
            case vdis::PDU_TYPE_DETONATION:
            {
                const vdis::detonation_pdu_t *base = SCAST(
                    vdis::detonation_pdu_t,
                    pdu.base());

                process_detonation(data, *base);
                break;
            }
            case vdis::PDU_TYPE_ACTION_REQUEST:
            {
                const vdis::action_request_pdu_t *base = SCAST(
                    vdis::action_request_pdu_t,
                    pdu.base());

                process_request(data, *base);
                break;
            }
            case vdis::PDU_TYPE_ACTION_RESPONSE:
            {
                const vdis::action_response_pdu_t *base = SCAST(
                    vdis::action_response_pdu_t,
                    pdu.base());

                process_response(data, *base);
                break;
            }
            case vdis::PDU_TYPE_DESIGNATOR:
            {
                const vdis::designator_pdu_t *base = SCAST(
                    vdis::designator_pdu_t,
                    pdu.base());

                process_designator(data, *base);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_entity_state(
    const pdu_data_t &data,
    const vdis::entity_state_pdu_t &pdu)
{
    entities_t::iterator
        itor = current_entities.find(pdu.id);
    bool
        deactivated = ((pdu.appearance & 0x800000) > 0);

    if ((itor == current_entities.end()) and not deactivated)
    {
        entity_t
            entity;

        entity.capabilities = pdu.capabilities;
        entity.appearance = pdu.appearance;

        print_heading(data);

        out << color::bold_yellow
            << "Entity found " << vdis::entity_marking(pdu.id)
            << " " << pdu.type << " '" << pdu.type.description() << "'"
            << color::none << std::endl;

        if (options::extra)
        {
            pdu.capabilities.print("  ", out);

            vdis::print_appearance("  ", pdu.type, pdu.appearance, out);
        }

        current_entities[pdu.id] = entity;
    }
    else if (deactivated)
    {
        if (itor != current_entities.end())
        {
            print_heading(data);

            out << color::bold_yellow
                << "Entity deactivated " << vdis::entity_marking(pdu.id)
                << " " << pdu.type << " '" << pdu.type.description() << "'"
                << color::none << std::endl;

            current_entities.erase(pdu.id);
        }
    }
    else if (options::extra)
    {
        if (itor->second.capabilities.value != pdu.capabilities.value)
        {
            print_heading(data);

            out << color::bold_yellow
                << "Entity modified capabilities "
                << vdis::entity_marking(pdu.id)
                << " " << pdu.type << " '" << pdu.type.description() << "'"
                << color::none << std::endl;

            if (options::extra)
            {
                pdu.capabilities.print("  ", out);
            }

            itor->second.capabilities.value = pdu.capabilities.value;
        }

        if (itor->second.appearance != pdu.appearance)
        {
            print_heading(data);

            out << color::bold_yellow
                << "Entity modified appearance "
                << vdis::entity_marking(pdu.id)
                << " " << pdu.type << " '" << pdu.type.description() << "'"
                << color::none << std::endl;

            if (options::extra)
            {
                vdis::print_appearance("  ", pdu.type, pdu.appearance, out);
            }

            itor->second.appearance = pdu.appearance;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_fire(
    const pdu_data_t &data,
    const vdis::fire_pdu_t &pdu)
{
    print_heading(data);

    out << color::bold_yellow
        << "Entity " << vdis::entity_marking(pdu.shooter)
        << " fired on target " << vdis::entity_marking(pdu.target)
        << color::none << std::endl;

    if (options::extra)
    {
        out << "  event " << pdu.event << std::endl
            << "  velocity " << pdu.velocity << std::endl
            << "  range " << vdis::to_string(pdu.range, 1, 3) << std::endl;

        pdu.burst_descriptor.print("  ", out);
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_detonation(
    const pdu_data_t &data,
    const vdis::detonation_pdu_t &pdu)
{
    print_heading(data);

    out << color::bold_yellow
        << "Fire from entity " << vdis::entity_marking(pdu.shooter)
        << " detonated on target " << vdis::entity_marking(pdu.target)
        << color::none << std::endl;

    if (options::extra)
    {
        out << "  event " << pdu.event << std::endl
            << "  result " << (vdis::detonation_result_e)pdu.result
            << std::endl;

        pdu.burst_descriptor.print("  ", out);
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_designator(
    const pdu_data_t &data,
    const vdis::designator_pdu_t &pdu)
{
    laser_systems_t
        &systems = current_lasers[pdu.designating_id];
    laser_systems_t::iterator
        itor = systems.find(pdu.system_number);
    laser_t
        laser;

    laser.target = pdu.designated_id;
    laser.code = pdu.code;
    laser.power = pdu.power;
    laser.spot_type = (vdis::desig_spot_type_e)pdu.spot_type;
    laser.system_name = (vdis::desig_system_name_e)pdu.system_name;
    laser.function = (vdis::laser_function_e)pdu.function;

    if ((itor == systems.end()) and (pdu.power > 0.0f))
    {
        print_heading(data);

        out << color::bold_yellow
            << "Entity " << vdis::entity_marking(pdu.designating_id)
            << " designating on system " << (int)pdu.system_number;

        if (not pdu.designated_id.is_none())
        {
            out << ", target " << vdis::entity_marking(pdu.designated_id);
        }

        out << color::none << std::endl;

        if (options::extra)
        {
            out << "  power " << vdis::to_string(pdu.power, 1, 2) << std::endl
                << "  code " << (int)pdu.code << std::endl
                << "  spot_type " << pdu.spot_type_enum() << std::endl
                << "  system_name " << pdu.system_name_enum() << std::endl
                << "  function " << pdu.function_enum() << std::endl;
        }

        systems[pdu.system_number] = laser;
    }
    else if (itor != systems.end())
    {
        if (pdu.power < 0.00001)
        {
            print_heading(data);

            out << color::bold_yellow
                << "Entity " << vdis::entity_marking(pdu.designating_id)
                << " stopped designating on system "
                << (int)pdu.system_number << color::none << std::endl;

            systems.erase(pdu.system_number);
        }
        else if (itor->second != laser)
        {
            print_heading(data);

            out << color::bold_yellow
                << "Entity " << vdis::entity_marking(pdu.designating_id)
                << " modified designator on system " << (int)pdu.system_number;

            if (not pdu.designated_id.is_none())
            {
                out << ", target " << vdis::entity_marking(pdu.designated_id);
            }

            out << color::none << std::endl;

            if (options::extra)
            {
                out << "  power " << vdis::to_string(pdu.power, 1, 2)
                    << std::endl
                    << "  code " << (int)pdu.code << std::endl
                    << "  spot_type " << pdu.spot_type_enum() << std::endl
                    << "  system_name " << pdu.system_name_enum() << std::endl
                    << "  function " << pdu.function_enum() << std::endl;
            }

            itor->second = laser;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_entity_associations(
    const pdu_data_t &data,
    const vdis::entity_state_pdu_t &pdu)
{
    std::set<vdis::id_t>
        known_targets;
    bool
        is_target = false;

    get_targets(pdu.id, known_targets);

    for(uint8_t i = 0; i < pdu.record_count; ++i)
    {
        const vdis::variable_parameter_record_t *record_ptr = pdu.record(i);

        if (record_ptr)
        {
            if (record_ptr->type() == vdis::VP_RECORD_TYPE_ENTITY_ASSOC)
            {
                const vdis::entity_association_t *association_ptr = SCAST(
                    vdis::entity_association_t,
                    record_ptr->content_ptr);

                switch(association_ptr->association_status)
                {
                    case 1: // PHYS_ASSOC_TARGET
                        process_target(data, pdu, *association_ptr);
                        is_target = true;
                        break;
                    case 4: // PHYS_ASSOC_CARRIER
                        process_carrier(data, pdu, *association_ptr);
                        known_targets.erase(association_ptr->entity_id);
                        break;
                }
            }
        }
    }

    if (not is_target)
    {
        targets_t::iterator
            target_itor = current_targets.find(pdu.id);

        if (target_itor != current_targets.end())
        {
            print_heading(data);

            out << color::bold_yellow
                << "Target " << vdis::entity_marking(pdu.id)
                << " detached from carrier "
                << vdis::entity_marking(target_itor->second.entity_id)
                << color::none << std::endl;

            current_targets.erase(pdu.id);
        }
    }

    if (not known_targets.empty())
    {
        std::set<vdis::id_t>::const_iterator
            target_itor = known_targets.begin();

        while(target_itor != known_targets.end())
        {
            print_heading(data);

            out << color::bold_yellow
                << "Carrier " << vdis::entity_marking(pdu.id)
                << " detached from target "
                << vdis::entity_marking(*target_itor) << color::none
                << std::endl;

            current_carriers[pdu.id].erase(*target_itor);

            ++target_itor;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_request(
    const pdu_data_t &data,
    const vdis::action_request_pdu_t &pdu)
{
    uint32_t
        action_id = 0;

    switch(pdu.action_id)
    {
        case 15: // INIT_HITCH_LEAD
        case 16: // INIT_HITCH_FOLLOW
        case 17: // UNHITCH
        case 18: // MOUNT
        case 19: // DISMOUNT
        case 4300: // SLING_CAPABILITY_REQ
        case 4301: // SLING_ATTACH_REQ
        case 4302: // SLING_RELEASE_REQ
        case 4303: // AIRMT_MOUNT_REQUEST
        case 4304: // AIRMT_DISMOUNT_REQUEST
        case 4305: // AIRMT_INFO_REQUEST
        case 4306: // HOIST_REQ
            action_id = pdu.action_id;
    }

    if (action_id > 0)
    {
        current_requests[pdu.originator].insert(pdu.request_id);

        print_heading(data);

        out << color::bold_yellow
            << "Request [" << pdu.request_id << "] from "
            << vdis::entity_marking(pdu.originator) << " to "
            << vdis::entity_marking(pdu.recipient) << color::none << std::endl
            << "  " << pdu.action_id_enum() << std::endl;

        if (options::extra)
        {
            pdu.print_records("  ", out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_response(
    const pdu_data_t &data,
    const vdis::action_response_pdu_t &pdu)
{
    entity_requests_t::iterator
        request_itor = current_requests.find(pdu.recipient);

    if (request_itor != current_requests.end())
    {
        if (request_itor->second.find(pdu.request_id) !=
            request_itor->second.end())
        {
            switch(pdu.request_status)
            {
                case 4:     // ACTRES_REQ_COMPLETE
                case 5:     // ACTRES_REQ_REQUEST_REJECTED
                case 10:    // ACTRES_REQ_REQUEST_DONE
                {
                    request_itor->second.erase(pdu.request_id);

                    if (request_itor->second.empty())
                    {
                        current_requests.erase(pdu.recipient);
                    }

                    break;
                }
            }

            print_heading(data);

            out << color::bold_yellow
                << "Response [" << pdu.request_id << "] from "
                << vdis::entity_marking(pdu.originator) << " to "
                << vdis::entity_marking(pdu.recipient) << color::none
                << std::endl
                << "  " << pdu.request_status_enum() << std::endl;

            if (options::extra)
            {
                pdu.print_records("  ", out);
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_carrier(
    const pdu_data_t &data,
    const vdis::entity_state_pdu_t &pdu,
    const vdis::entity_association_t &record)
{
    targets_t
        &targets = current_carriers[pdu.id];
    targets_t::iterator
        target_itor = targets.find(record.entity_id);

    if (target_itor == targets.end())
    {
        print_heading(data);

        out << color::bold_yellow
            << "Carrier " << vdis::entity_marking(pdu.id)
            << " attached to target "
            << vdis::entity_marking(record.entity_id) << std::endl
            << color::none;

        if (options::extra)
        {
            record.print("  ", out);
        }

        targets[record.entity_id] = record;
    }
    else if (record != target_itor->second)
    {
        out << color::bold_yellow
            << "Carrier " << vdis::entity_marking(pdu.id)
            << " modified association to target "
            << vdis::entity_marking(record.entity_id) << color::none
            << std::endl;

        if (options::extra)
        {
            record.print("  ", out);
        }

        target_itor->second = record;
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::process_target(
    const pdu_data_t &data,
    const vdis::entity_state_pdu_t &pdu,
    const vdis::entity_association_t &record)
{
    targets_t::iterator
        target_itor = current_targets.find(pdu.id);

    if (target_itor == current_targets.end())
    {
        print_heading(data);

        out << color::bold_yellow
            << "Target " << vdis::entity_marking(pdu.id)
            << " attached to carrier "
            << vdis::entity_marking(record.entity_id)
            << color::none << std::endl;

        if (options::extra)
        {
            record.print("  ", out);
        }

        current_targets[pdu.id] = record;
    }
    else
    {
        if (record.entity_id != target_itor->second.entity_id)
        {
            print_heading(data);

            out << color::bold_yellow
                << "Target " << vdis::entity_marking(pdu.id)
                << " changed carrier to "
                << vdis::entity_marking(record.entity_id) << color::none
                << std::endl;

            if (options::extra)
            {
                record.print("  ", out);
            }

            target_itor->second = record;
        }

        if (record != target_itor->second)
        {
            print_heading(data);

            out << color::bold_yellow
                << "Target " <<  vdis::entity_marking(pdu.id)
                << " modified association to carrier "
                << record.entity_id << color::none
                << std::endl;

            if (options::extra)
            {
                record.print("  ", out);
            }

            target_itor->second = record;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::get_targets(
    const vdis::id_t &carrier_id,
    std::set<vdis::id_t> &target_ids)
{
    carriers_t::const_iterator carrier = current_carriers.find(carrier_id);

    if (carrier != current_carriers.end())
    {
        targets_t::const_iterator target = carrier->second.begin();

        while(target != carrier->second.end())
        {
            target_ids.insert(target->first);
            ++target;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::scan::print_heading(const pdu_data_t &data)
{
    out << color::bold_cyan
        << vdis::time_to_string(data.get_time())
        << color::none << " [#" << data.get_index() << "]: ";
}
