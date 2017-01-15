#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_scan.h"

#include "vdis_appearance.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

bool
    vdb::scan::scanning = false,
    vdb::scan::associations = false,
    vdb::scan::lasers = false,
    vdb::scan::fires = false,
    vdb::scan::collisions = false,
    vdb::scan::entities = false,
    vdb::scan::objects = false;
vdb::scan::entities_t
    vdb::scan::current_entities;
std::ostream
    &vdb::scan::err = std::cerr,
    &vdb::scan::out = std::cout;

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
void vdb::scan::print_heading(const pdu_data_t &data)
{
    out << color::bold_cyan
        << vdis::time_to_string(data.get_time())
        << color::none << " [#" << data.get_index() << "]: ";
}
