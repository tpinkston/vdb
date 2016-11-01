#include "vdb_associations.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"
#include "vdis_variable_parameter_records.h"

vdb::associations::carriers_t
    vdb::associations::current_carriers;
vdb::associations::targets_t
    vdb::associations::current_targets;
vdb::associations::entity_requests_t
    vdb::associations::current_requests;
std::ostream
    &vdb::associations::err = std::cerr,
    &vdb::associations::out = std::cout;

// ----------------------------------------------------------------------------
void vdb::associations::process_pdu(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu)
{
    if (pdu.base())
    {
        switch(pdu.type())
        {
            case vdis::PDU_TYPE_ACTION_REQUEST:
            {
                const vdis::action_request_pdu_t &request = *SCAST(
                    vdis::action_request_pdu_t,
                    pdu.base());

                process_request(data, request);

                break;
            }
            case vdis::PDU_TYPE_ACTION_RESPONSE:
            {
                const vdis::action_response_pdu_t &response = *SCAST(
                    vdis::action_response_pdu_t,
                    pdu.base());

                process_response(data, response);

                break;
            }
            case vdis::PDU_TYPE_ENTITY_STATE:
            {
                const vdis::entity_state_pdu_t &entity_state = *SCAST(
                    vdis::entity_state_pdu_t,
                    pdu.base());

                process_entity_state(data, entity_state);

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
void vdb::associations::process_entity_state(
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
            print_data(data);

            out << color::bold_yellow
                << "Target " << vdis::entity_marking(pdu.id)
                << " detached from carrier "
                << vdis::entity_marking(target_itor->second.entity_id)
                << color::none << std::endl;

            current_targets.erase(pdu.id);
        }
    }

    if (known_targets.empty())
    {
        std::set<vdis::id_t>::const_iterator
            target_itor = known_targets.begin();

        while(target_itor != known_targets.end())
        {
            print_data(data);

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
void vdb::associations::process_request(
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

        print_data(data);

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
void vdb::associations::process_response(
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

            print_data(data);

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
void vdb::associations::process_carrier(
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
        print_data(data);

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
void vdb::associations::process_target(
    const pdu_data_t &data,
    const vdis::entity_state_pdu_t &pdu,
    const vdis::entity_association_t &record)
{
    targets_t::iterator
        target_itor = current_targets.find(pdu.id);

    if (target_itor == current_targets.end())
    {
        print_data(data);

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
            print_data(data);

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
            print_data(data);

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
void vdb::associations::get_targets(
    const vdis::id_t &carrier_id,
    std::set<vdis::id_t> &target_ids)
{
    carriers_t::const_iterator
        carrier = current_carriers.find(carrier_id);

    if (carrier != current_carriers.end())
    {
        targets_t::const_iterator
            target = carrier->second.begin();

        while(target != carrier->second.end())
        {
            target_ids.insert(target->first);

            ++target;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::associations::print_data(const pdu_data_t &data)
{
    out << color::bold_cyan
        << vdis::time_to_string(data.get_time())
        << color::none << " [#" << data.get_index() << "]: ";
}
