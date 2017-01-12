#include "vdb_entities.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_appearance.h"
#include "vdis_pdus.h"
#include "vdis_services.h"

vdb::entities::entities_t
    vdb::entities::current_entities;
std::ostream
    &vdb::entities::err = std::cerr,
    &vdb::entities::out = std::cout;

// ----------------------------------------------------------------------------
void vdb::entities::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
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
void vdb::entities::process_entity_state(
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

        print_data(data);

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
        print_data(data);

        out << color::bold_yellow
            << "Entity deactivated " << vdis::entity_marking(pdu.id)
            << " " << pdu.type << " '" << pdu.type.description() << "'"
            << color::none << std::endl;

        current_entities.erase(pdu.id);
    }
    else
    {
        if (itor->second.capabilities.value != pdu.capabilities.value)
        {
            print_data(data);

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
            print_data(data);

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
void vdb::entities::print_data(const pdu_data_t &data)
{
    out << color::bold_cyan
        << vdis::time_to_string(data.get_time())
        << color::none << " [#" << data.get_index() << "]: ";
}
