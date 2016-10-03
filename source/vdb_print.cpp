#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"

#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

// ----------------------------------------------------------------------------
void vdb::print::print_pdu(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu,
    std::ostream &out)
{
    if (options::flag(OPT_DUMP))
    {
        print_pdu_hex_dump(data, out);
    }
    else if (options::flag(OPT_EXTRACT))
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

        temp_buffer.print((pdu_type + ": "), out);
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
    if (options::get_command() == USER_COMMAND_PLAYBACK)
    {
        time = vdis::get_system_time();
    }
    else
    {
        out << data.get_source() << "; ";
    }

    out << color::green << (int)data.get_pdu_exercise()
        << color::none << "; " << vdis::time_to_string(time) << "; "
        << color::yellow << vdis::enumerations::get_name(ENUM_PDU_TYPE, data.get_pdu_type());

    if (intiator_ptr)
    {
        out << " " << color::bold_yellow << *intiator_ptr;
    }

    if (request_id_ptr)
    {
        out << " " << color::bold_blue << "[" << *request_id_ptr << "]";
    }

    out << color::none << std::endl;

    if (pdu.base() and options::flag(OPT_SUMMARIZE))
    {
        out << color::purple;

        print_pdu_summary(data, pdu, out);

        out << color::none;
    }
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_summary(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu,
    std::ostream &out)
{

}
