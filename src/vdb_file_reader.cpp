#include "vdb_file_reader.h"
#include "vdb_options.h"

#include "vdis_logger.h"
#include "vdis_network.h"

// ----------------------------------------------------------------------------
vdb::file_reader_t::file_reader_t(const string_t &filename) :
    filename(filename),
    error_condition(false)
{

}

// ----------------------------------------------------------------------------
vdb::file_reader_t::~file_reader_t(void)
{

}

// ----------------------------------------------------------------------------
// Starts the parsing of the file.  Input is a function pointer for
// callback handler for PDUs read from the file.  Callback function
// should return false when file parsing need not continue (true
// to keep parsing).
//
// This method returns true if no errors occurred during parsing.
//
bool vdb::file_reader_t::parse(bool (*callback)(const pdu_data_t &))
{
    if (callback)
    {
        const uint64_t
            start_time = vdis::get_system_time();
        pdu_data_t
            data;
        bool
            parsing = true;

        LOG_VERBOSE("Parsing file content...");

        while(parsing and good() and next_entry(data))
        {
            if (data.has_pdu())
            {
                parsing = callback(data);
            }
        }

        LOG_VERBOSE(
            "Parsing completed in %d milliseconds...",
            (vdis::get_system_time() - start_time));
    }
    else
    {
        LOG_ERROR("Parsing callback function is null!");
        error_condition = true;
    }

    return good();
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::standard_reader_t(const string_t &filename) :
    file_reader_t(filename)
{
    if (not stream.read_file(filename))
    {
        error_condition = true;
    }
    else
    {
        header.read(stream);

        if (header.invalid_title() or stream.error())
        {
            std::cerr << "vdb: invalid capture file: " << filename
                      << std::endl;

            error_condition = true;
        }
    }
}

// ----------------------------------------------------------------------------
vdb::standard_reader_t::~standard_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::standard_reader_t::next_entry(pdu_data_t &data)
{
    bool
        valid_entry = false;

    if (good() and (stream.remaining_length() > 0))
    {
        LOG_EXTRA_VERBOSE(
            "Reading PDU data at index %d of %d...",
            stream.index(),
            stream.length());

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
vdb::pdu_reader_t::pdu_reader_t(const string_t &filename, uint32_t interval) :
    file_reader_t(filename),
    interval_milliseconds(interval),
    index(0)
{
    if (options::ipv6)
    {
        vdis::set_address("::1", 0, address_ipv6);
    }
    else
    {
        bool broadcast = false;

        vdis::set_address("127.0.0.1", 0, address_ipv4, broadcast);
    }
}

// ----------------------------------------------------------------------------
vdb::pdu_reader_t::~pdu_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::pdu_reader_t::next_entry(pdu_data_t &data)
{
    file_stream
        stream;
    bool
        success = false;

    data.clear();

    if (stream.read_file(filename))
    {
        if (index == 0)
        {
            data.set_time(vdis::get_system_time());
        }
        else
        {
            data.set_time(vdis::get_system_time() + interval_milliseconds);
        }

        data.set_index(index++);
        data.set_pdu_buffer(stream.buffer(), stream.length());

        if (options::ipv6)
        {
            data.set_source(address_ipv6, 0);
        }
        else
        {
            data.set_source(address_ipv4, 0);
        }

        success = true;
    }

    return success;
}
