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

#include "vdb_byte_stream.h"
#include "vdb_color.h"
#include "vdb_enums.h"
#include "vdb_file_handler.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_string.h"
#include "vdb_time.h"
#include "vdb_version.h"

// ----------------------------------------------------------------------------
vdb::file_handler::file_handler(void) : file_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdb::file_handler::~file_handler(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::file_handler::open_output_file(void)
{
    file_header_t
        header;
    bool
        success = false;

    // First save header to the output capture file.
    //
    header.major_version = VDB_VERSION_MAJOR;
    header.minor_version = VDB_VERSION_MINOR;
    header.time_created = time::get_system();

    stream.set_length(header.length());
    stream.write(header);
    stream.write_file(filename);

    // Reopen the file for manually writing PDU data, appending to the header
    // just written ('a' for append, 'b' for binary).
    //
    file_ptr = fopen(filename.c_str(), "ab");

    if (file_ptr)
    {
        std::cout << "Opened output file: " << filename << std::endl;
        success = true;
    }
    else
    {
        perror("fopen");
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::file_handler::close_output_file(void)
{
    bool
        success = false;

    if (file_ptr)
    {
        if (fclose(file_ptr) == 0)
        {
            std::cout << "Closed output file: " << filename << std::endl;
            success = true;
        }
        else
        {
            perror("fclose");
        }

        file_ptr = 0;
    }

    return success;
}

// ----------------------------------------------------------------------------
void vdb::file_handler::write_pdu_data(const pdu_data_t &data)
{
    if (file_ptr)
    {
        const uint32_t
            size = data.length();

        stream.reset();
        stream.set_length(size);

        LOG_VERBOSE(
            "Writing %d bytes to %s...",
            size,
            filename.c_str());

        data.write(stream);

        if (stream.success())
        {
            uint32_t bytes_written = fwrite(
                stream.get_buffer(),
                1,
                stream.get_length(),
                file_ptr);

            if (bytes_written != size)
            {
                LOG_ERROR(
                    "File write failed (%d out of %d bytes)",
                    size,
                    stream.get_length());
            }
        }
    }
}

// ----------------------------------------------------------------------------
bool vdb::file_handler::read_content(void)
{
    bool
        success = true;

    // Reads the contents of the file in its entirety into the stream.
    // Then restore the header.
    //
    stream.read_file(filename);
    stream.read(header);

    if (stream.error() || header.invalid_title())
    {
        std::cerr << options::get_terminal_command()
                  << ": invalid capture file: " << filename << std::endl;

        success = false;
    }

    return success;
}

// ----------------------------------------------------------------------------
void vdb::file_handler::parse_content(void)
{
    const uint64_t
        start_time = time::get_system();
    pdu_data_t
        data;

    LOG_VERBOSE("Parsing file content...");

    while((stream.get_length_remaining() > 0) and not stream.error())
    {
        LOG_EXTRA_VERBOSE(
            "Restoring PDU data at index %d of %d...",
            stream.get_index(),
            stream.get_length());

        data.read(stream);

        if (not stream.error())
        {
            process_pdu_data(data);
        }
    }

    const uint64_t
        parse_time = (time::get_system() - start_time);

    LOG_VERBOSE("Parsing completed in %d milliseconds...", parse_time);
}

// ----------------------------------------------------------------------------
void vdb::file_handler::process_pdu_data(const pdu_data_t &data)
{
    LOG_ERROR("Doing nothing with PDU data...");
}

// ----------------------------------------------------------------------------
bool vdb::file_handler::passes_data_filters(const pdu_data_t &data)
{
    bool
        pass = true;

    if (pass and options::provided(OPT_EXERCISE))
    {
        pass = (data.get_pdu_exercise() != *options::integer(OPT_EXERCISE));

        if (not pass)
        {
            LOG_VERBOSE(
                "Filtered out PDU on exercise %d...",
                data.get_pdu_exercise());
        }
    }

    if (pass and options::provided(OPT_PDU_TYPE))
    {
        pass = options::integer_in_set(OPT_PDU_TYPE, data.get_pdu_type());

        if (not pass)
        {
            LOG_VERBOSE(
                "Filtered out PDU with type %s...",
                enumerations::get_name(
                    ENUM_PDU_TYPE,
                    data.get_pdu_type()).c_str());
        }
    }

    if (pass and  options::provided(OPT_PDU_FAMILY))
    {
        pass =  options::integer_in_set(OPT_PDU_FAMILY, data.get_pdu_family());

        if (not pass)
        {
            LOG_VERBOSE(
                "Filtered out PDU with family %s...",
                enumerations::get_name(
                    ENUM_PDU_FAMILY,
                    data.get_pdu_family()).c_str());
        }
    }

    return pass;

}

// ----------------------------------------------------------------------------
bool vdb::file_handler::passes_pdu_filters(const pdu_t &pdu)
{
    bool
        pass = true;

    if (pass and not options::entity_id_match(pdu))
    {
        pass = false;
    }

    return pass;
}
