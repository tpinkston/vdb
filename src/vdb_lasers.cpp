#include "vdb_lasers.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"

#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"

vdb::lasers::entity_lasers_t
    vdb::lasers::current_lasers;
std::ostream
    &vdb::lasers::err = std::cerr,
    &vdb::lasers::out = std::cout;

// ----------------------------------------------------------------------------
void vdb::lasers::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
{
    if (pdu.base())
    {
        switch(pdu.type())
        {
            case vdis::PDU_TYPE_DESIGNATOR:
            {
                const vdis::designator_pdu_t &designator = *SCAST(
                    vdis::designator_pdu_t,
                    pdu.base());

                process_designator(data, designator);
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
void vdb::lasers::process_designator(
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
        print_data(data);

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
            print_data(data);

            out << color::bold_yellow
                << "Entity " << vdis::entity_marking(pdu.designating_id)
                << " stopped designating on system "
                << (int)pdu.system_number << color::none << std::endl;

            systems.erase(pdu.system_number);
        }
        else if (itor->second != laser)
        {
            print_data(data);

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
                out << "  power " << vdis::to_string(pdu.power, 1, 2) << std::endl
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
void vdb::lasers::print_data(const pdu_data_t &data)
{
    out << color::bold_cyan
        << vdis::time_to_string(data.get_time())
        << color::none << " [#" << data.get_index() << "]: ";
}
