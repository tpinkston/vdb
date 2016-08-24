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

#include "vdb_file_readers.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_time.h"

// ----------------------------------------------------------------------------
vdb::file_reader_t::file_reader_t(const std::string &filename) :
    filename(filename),
    error_condition(false)
{

}

// ----------------------------------------------------------------------------
vdb::file_reader_t::~file_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::file_reader_t::parse(bool (*callback)(const pdu_data_t &))
{
    const uint64_t
        start_time = time::get_system();
    pdu_data_t
        data;
    bool
        parsing = true;

    LOG_VERBOSE("Parsing file content...");

    while(parsing and good() and next_entry(data))
    {
        parsing = callback(data);
    }

    const uint64_t
        parse_time = (time::get_system() - start_time);

    LOG_VERBOSE("Parsing completed in %d milliseconds...", parse_time);

    return good();
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::standard_reader_t(const std::string &filename) :
    file_reader_t(filename)
{
    if (not stream.read_file(filename))
    {
        error_condition = true;
    }
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::~standard_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::standard_reader_t::read_header(void)
{
    if (error_condition)
    {
        LOG_ERROR("Cannot read header with error condition!");
    }
    else
    {
        header.read(stream);

        if (stream.error() || header.invalid_title())
        {
            std::cerr << options::get_terminal_command()
                      << ": invalid capture file: " << filename << std::endl;

            error_condition = true;
        }
    }

    return good();
}

// ----------------------------------------------------------------------------
bool vdb::standard_reader_t::next_entry(pdu_data_t &data)
{
    bool
        valid_entry = false;

    if (good() and (stream.get_length_remaining() > 0))
    {
        LOG_EXTRA_VERBOSE(
            "Reading PDU data at index %d of %d...",
            stream.get_index(),
            stream.get_length());

        data.read(stream);

        if (stream.error())
        {
            error_condition = true;
        }
        else
        {
            valid_entry = true;
        }
    }

    return valid_entry;
}

// ----------------------------------------------------------------------------
vdb::pcap_reader_t::pcap_reader_t(const std::string &filename) :
    file_reader_t(filename)
{

}

// ----------------------------------------------------------------------------
vdb::pcap_reader_t::~pcap_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::pcap_reader_t::read_header(void)
{
    // TODO
    return false;
}

// ----------------------------------------------------------------------------
bool vdb::pcap_reader_t::next_entry(pdu_data_t &data)
{
    // TODO
    return false;
}
