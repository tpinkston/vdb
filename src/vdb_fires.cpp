#include "vdb_fires.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"

std::ostream
    &vdb::fires::err = std::cerr,
    &vdb::fires::out = std::cout;

// ----------------------------------------------------------------------------
void vdb::fires::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
{
    if (pdu.base())
    {
        switch(pdu.type())
        {
            case vdis::PDU_TYPE_FIRE:
            {
                const vdis::fire_pdu_t &fire = *SCAST(
                    vdis::fire_pdu_t,
                    pdu.base());

                process_fire(data, fire);
                break;
            }
            case vdis::PDU_TYPE_DETONATION:
            {
                const vdis::detonation_pdu_t &detonation = *SCAST(
                    vdis::detonation_pdu_t,
                    pdu.base());

                process_detonation(data, detonation);
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
void vdb::fires::process_fire(
    const pdu_data_t &data,
    const vdis::fire_pdu_t &pdu)
{
    print_data(data);

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
void vdb::fires::process_detonation(
    const pdu_data_t &data,
    const vdis::detonation_pdu_t &pdu)
{
    print_data(data);

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
void vdb::fires::print_data(const pdu_data_t &data)
{
    out << color::bold_cyan
        << vdis::time_to_string(data.get_time())
        << color::none << " [#" << data.get_index() << "]: ";
}
