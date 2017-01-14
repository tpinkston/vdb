#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"

#include "vdis_datum_records.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"
#include "vdis_variable_parameter_records.h"

// ----------------------------------------------------------------------------
void vdb::print::print_pdu(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu,
    std::ostream &out)
{
    if (options::dump)
    {
        print_pdu_hex_dump(data, out);
    }
    else if (options::extracted)
    {
        print_pdu_extracted(data, pdu, out);
    }
    else
    {
        print_pdu_normal(data, pdu, out);
    }
}

// ----------------------------------------------------------------------------
void vdb::print::print_data(
    const pdu_data_t &data,
    std::ostream &out)
{
    out << color::bold_cyan
        << "======================================="
        << "=======================================" << std::endl
        << data
        << "======================================="
        << "======================================="
        << color::none << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_extracted(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu,
    std::ostream &out)
{
    string_t
        prefix = vdis::enumerations::get_name(
            ENUM_PDU_TYPE,
            data.get_pdu_type());

    prefix = vdis::to_lowercase(prefix + ".");

    print_data(data, out);

    out << pdu;
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_hex_dump(
    const pdu_data_t &data,
    std::ostream &out)
{
    print_data(data, out);

    if (data.get_pdu_length() > 0)
    {
        vdis::byte_buffer_t
            temp_buffer(data.get_pdu_buffer(), data.get_pdu_length(), false);
        string_t
            pdu_type = vdis::enumerations::get_name(
                ENUM_PDU_TYPE,
                data.get_pdu_type());

        temp_buffer.print((pdu_type + ":"), out);
    }
    else
    {
        out << "No PDU buffer available..." << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_normal(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu,
    std::ostream &out)
{
    const vdis::id_t
        *intiator_ptr = 0;
    const uint32_t
        *request_id_ptr = 0;
    uint64_t
        time = data.get_time();

    if (pdu.base())
    {
        intiator_ptr = pdu.base()->get_initator();
        request_id_ptr = pdu.base()->get_request_id();
    }

    out << data.get_index() << ": ";

    // On playback print the current system time instead of source.
    //
    if (true) // TODO options::command == USER_COMMAND_PLAYBACK)
    {
        time = vdis::get_system_time();
    }
    else
    {
        out << data.get_source() << "; ";
    }

    out << color::green << (int)data.get_pdu_exercise()
        << color::none << "; " << vdis::time_to_string(time) << "; "
        << color::yellow << vdis::enumerations::get_name(
            ENUM_PDU_TYPE,
            data.get_pdu_type())
        << "(" << (int)data.get_pdu_type() << ")";

    if (intiator_ptr)
    {
        out << " " << color::bold_yellow << *intiator_ptr;
    }

    if (request_id_ptr)
    {
        out << " " << color::bold_blue << "[" << *request_id_ptr << "]";
    }

    out << color::none << std::endl;

    if (pdu.base() and options::extra)
    {
        out << color::purple;

        print_pdu_summary(pdu, out);

        out << color::none;
    }
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_summary(const vdis::pdu_t &pdu, std::ostream &out)
{
    out << color::bold_purple;

    switch(pdu.type())
    {
        case vdis::PDU_TYPE_ENTITY_STATE:
        {
            const vdis::entity_state_pdu_t *p = SCAST(
                vdis::entity_state_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->id << "  " << p->marking
                    << "  " << vdis::enumerations::get_name(
                        ENUM_FORCE_ID,
                        p->force_enum());

                switch(p->type.kind_enum())
                {
                    case vdis::ENT_KIND_LIFEFORMS:
                        out << "  " << vdis::enumerations::get_name(
                            ENUM_LF_HEALTH,
                            p->damage());
                        break;
                    default:
                        out << "  " << vdis::enumerations::get_name(
                            ENUM_SEVERITY,
                            p->damage());
                        break;
                }

                out << "  " << p->type << " '" << p->type.description()
                    << "'  " << std::endl;

                for(uint8_t i = 0; i < p->record_count; ++i)
                {
                    const vdis::variable_parameter_record_t
                        *record_ptr = p->record(i);

                    if (record_ptr)
                    {
                        out << "  record[" << (int)i << "] = "
                            << record_ptr->type() << std::endl;
                    }
                }
            }

            break;
        }
        case vdis::PDU_TYPE_FIRE:
        {
            const vdis::fire_pdu_t *p = SCAST(
                vdis::fire_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->event << ": " << p->shooter
                    << "->" << p->target << std::endl
                    << "  " << p->burst_descriptor.munition
                    << " '" << p->burst_descriptor.munition.description() << "'"
                    << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_DETONATION:
        {
            const vdis::detonation_pdu_t *p = SCAST(
                vdis::detonation_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->event << ": " << p->shooter
                    << "->" << p->target << std::endl
                    << "  " << p->burst_descriptor.munition
                    << " '" << p->burst_descriptor.munition.description() << "'"
                    << std::endl
                    << "  " << p->result_enum() << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_COLLISION:
        {
            const vdis::collision_pdu_t *p = SCAST(
                vdis::collision_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->collision_type_enum() << std::endl
                    << "  " << p->issuing_entity
                    << "->" << p->colliding_entity << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_CREATE_ENTITY:
        {
            const vdis::create_entity_pdu_t *p = SCAST(
                vdis::create_entity_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_REMOVE_ENTITY:
        {
            const vdis::remove_entity_pdu_t *p = SCAST(
                vdis::remove_entity_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_START_RESUME:
        {
            const vdis::start_resume_pdu_t *p = SCAST(
                vdis::start_resume_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << "real time: " << p->real_time << std::endl
                    << "  " << "sim time: " << p->simulation_time << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_STOP_FREEZE:
        {
            const vdis::stop_freeze_pdu_t *p = SCAST(
                vdis::stop_freeze_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << "real time: " << p->real_time << std::endl
                    << "  " << p->reason_enum() << std::endl
                    << "  " << p->behavior_enum() << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_ACKNOWLEDGE:
        {
            const vdis::acknowledge_pdu_t *p = SCAST(
                vdis::acknowledge_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << p->acknowledge_flag_enum() << std::endl
                    << "  " << p->response_flag_enum() << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_ACTION_REQUEST:
        {
            const vdis::action_request_pdu_t *p = SCAST(
                vdis::action_request_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << p->request_id
                    << "->" << p->action_id_enum() << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_ACTION_RESPONSE:
        {
            const vdis::action_response_pdu_t *p = SCAST(
                vdis::action_response_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << p->request_id
                    << "->" << p->request_status_enum() << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_DATA_QUERY:
        {
            const vdis::data_query_pdu_t *p = SCAST(
                vdis::data_query_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << p->request_id << "->" << p->time_interval
                    << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_SET_DATA:
        {
            const vdis::set_data_pdu_t *p = SCAST(
                vdis::set_data_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_DATA:
        {
            const vdis::data_pdu_t *p = SCAST(
                vdis::data_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_EVENT_REPORT:
        {
            const vdis::event_report_pdu_t *p = SCAST(
                vdis::event_report_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl
                    << "  " << p->event_type_enum() << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_COMMENT:
        {
            const vdis::comment_pdu_t *p = SCAST(
                vdis::comment_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << p->originator << "->" << p->recipient
                    << std::endl;

                print_datum_record_summary(*p, out);
            }

            break;
        }
        case vdis::PDU_TYPE_EM_EMISSION:
        {
            const vdis::em_emission_pdu_t *p = SCAST(
                vdis::em_emission_pdu_t,
                pdu.base());

            for(uint8_t i = 0; p and (i < p->system_count); ++i)
            {
                const vdis::emitter_system_t
                    *system_ptr = p->system(i);

                if (system_ptr)
                {
                    out << "  system[" << (int)i << "] = "
                        << (vdis::emitter_function_e)system_ptr->function
                        << std::endl;

                    for(uint8_t j = 0; (j < system_ptr->beam_count); ++j)
                    {
                        const vdis::emitter_beam_t
                            *beam_ptr = system_ptr->beam(j);

                        if (beam_ptr)
                        {
                            out << "  system[" << (int)i
                                << "].beam[" << (int)j << "] = "
                                << (vdis::beam_function_e)beam_ptr->function
                                << std::endl;

                            for(uint8_t k = 0; (k < beam_ptr->target_count); ++k)
                            {
                                const vdis::emitter_target_t
                                    *target_ptr = beam_ptr->target(k);

                                if (target_ptr)
                                {
                                    out << "  system[" << (int)i
                                        << "].beam[" << (int)j
                                        << "].target[" << (int)k << "] = "
                                        << target_ptr->entity << std::endl;
                                }
                            }
                        }
                    }
                }
            }

            break;
        }
        case vdis::PDU_TYPE_DESIGNATOR:
        {
            const vdis::designator_pdu_t *p = SCAST(
                vdis::designator_pdu_t,
                pdu.base());

            if (p)
            {
                out << "  " << (int)p->system_number
                    << "->" << p->designated_id << "  " << (int)p->code
                    << "  " << vdis::to_string(p->power, 1, 3) << " watts"
                    << std::endl
                    << "  " << p->function_enum() << "  "
                    << p->spot_type_enum();

                if (p->system_name > 0)
                {
                    out << "  " << p->system_name_enum();
                }

                out << std::endl;
            }

            break;
        }
        case vdis::PDU_TYPE_TRANSMITTER:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_SIGNAL:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_RECEIVER:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_IFF:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_ENVIRONMENTAL_PROCESS:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_POINT_OBJECT_STATE:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_LINEAR_OBJECT_STATE:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_AREAL_OBJECT_STATE:
        {
            // TODO
            break;
        }
        case vdis::PDU_TYPE_APPLICATION_CTRL:
        {
            // TODO
            break;
        }
        default:
        {
            out << "  (no summary available)" << std::endl;
            break;
        }
    }

    out << color::none;
}

// ----------------------------------------------------------------------------
void vdb::print::print_datum_record_summary(
    const vdis::abstract_siman_pdu_t &pdu,
    std::ostream &out)
{
    for(uint32_t i = 0; i < pdu.fixed_count; ++i)
    {
        const vdis::fixed_datum_record_t
            *record_ptr = pdu.fixed_datum(i);

        if (record_ptr)
        {
            out << "  fixed_datum[" << i << "] = "
                << record_ptr->datum_id_enum() << std::endl;
        }
    }

    for(uint32_t i = 0; i < pdu.variable_count; ++i)
    {
        const vdis::variable_datum_record_t
            *record_ptr = pdu.variable_record(i);

        if (record_ptr)
        {
            out << "  variable_datum[" << i << "] = "
                << record_ptr->datum_id_enum() << std::endl;
        }
    }
}
