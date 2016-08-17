// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

#include "vdb_color.h"
#include "vdb_enums.h"
#include "vdb_hexadecimal.h"
#include "vdb_ids.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_pdu.h"
#include "vdb_print.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
void vdb::print::print_pdu(
    const pdu_data_t &data,
    const pdu_t &pdu,
    std::ostream &stream)
{
    if (options::flag(OPT_DUMP))
    {
        print_pdu_hex_dump(data, stream);
    }
    else if (options::flag(OPT_EXTRACT))
    {
        print_pdu_extracted(data, pdu, stream);
    }
    else
    {
        print_pdu_normal(data, pdu, stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::print::print_data(
    const pdu_data_t &data,
    std::ostream &stream)
{
    stream << color::bold_cyan
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
    const pdu_t &pdu,
    std::ostream &stream)
{
    std::string
        prefix = enumerations::get_name(ENUM_PDU_TYPE, data.get_pdu_type());

    prefix = to_lowercase(prefix + ".");

    print_data(data, stream);

    pdu.print(prefix, stream);
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_hex_dump(
    const pdu_data_t &data,
    std::ostream &stream)
{
    print_data(data, stream);

    if (data.get_pdu_length() > 0)
    {
        const std::string
            pdu_type = enumerations::get_name(
                ENUM_PDU_TYPE,
                data.get_pdu_type());

        hexadecimal::stream(
            (pdu_type + ": "),
            data.get_pdu_buffer(),
            data.get_pdu_length(),
            8,
            stream);
    }
    else
    {
        stream << "No PDU buffer available..." << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::print::print_pdu_normal(
    const pdu_data_t &data,
    const pdu_t &pdu,
    std::ostream &stream)
{
    const id_t
        *intiator_ptr = 0;
    const uint32_t
        *request_id_ptr = 0;
    uint64_t
        time = data.get_time();

    if (pdu.content_ptr)
    {
        intiator_ptr = pdu.content_ptr->get_initator();
        request_id_ptr = pdu.content_ptr->get_request_id();
    }

    stream << data.get_index() << ": ";

    // On playback don't print source and print the current system time.
    //
    if (options::get_command() == USER_COMMAND_PLAYBACK)
    {
        time = time::get_system();
    }
    else
    {
        stream << data.get_source() << "; ";
    }

    stream << color::green << (int)pdu.header_exercise
           << color::none << "; " << time::to_string(time) << "; "
           << color::yellow << (pdu_type_e)pdu.header_type;

    if (intiator_ptr)
    {
        stream << " " << color::bold_yellow << *intiator_ptr;
    }

    if (request_id_ptr)
    {
        stream << " " << color::bold_blue << "[" << *request_id_ptr << "]";
    }

    stream << color::none << std::endl;

    if (pdu.content_ptr and options::flag(OPT_SUMMARIZE))
    {
        stream << color::purple;

        pdu.content_ptr->print_summary(stream);

        stream << color::none;
    }
}
