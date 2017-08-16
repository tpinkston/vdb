#include "vdb_command.h"
#include "vdb_common.h"
#include "vdb_file_reader.h"
#include "vdb_options.h"

#include "vdis_logger.h"
#include "vdis_string.h"

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
// Starts the parsing of the file.  Input is a pointer to a callback
// for PDUs read from the file.  Callback function should return false when
// file parsing need not continue (true to keep parsing).
//
// This method returns true if no errors occurred during parsing.
//
bool vdb::file_reader_t::parse(file_read_command_t *callback_ptr)
{
    if (callback_ptr)
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
                parsing = callback_ptr->process_pdu_data(data);
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
            LOG_ERROR("invalid capture file: %s", filename.c_str());
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
            LOG_EXTRA_VERBOSE("Valid entry at index %d...", data.get_index());
            valid_entry = true;
        }
    }

    return valid_entry;
}

// ----------------------------------------------------------------------------
vdb::pdu_reader_t::pdu_reader_t(const string_t &filename, uint64_t interval) :
    file_reader_t(filename),
    interval_milliseconds(interval),
    index(0)
{
    if (not stream.read_file(filename))
    {
        error_condition = true;
    }
    else
    {
        if (vdis::network_options::ipv6)
        {
            vdis::set_address("::1", 0, address_ipv6);
        }
        else
        {
            bool broadcast = false;

            vdis::set_address("127.0.0.1", 0, address_ipv4, broadcast);
        }
    }
}

// ----------------------------------------------------------------------------
vdb::pdu_reader_t::~pdu_reader_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::pdu_reader_t::next_entry(pdu_data_t &data)
{
    bool success = false;

    data.clear();

    if (debug_enabled(DEBUG_FILE_READER))
    {
        CONSOLE_OUT("pdu_reader_t::next_entry(index = %d)", index);
    }

    if (error_condition)
    {
        LOG_FATAL("Cannot read file with error condition!");
    }
    else
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

        if (vdis::network_options::ipv6)
        {
            data.set_source(address_ipv6, 0);
        }
        else
        {
            data.set_source(address_ipv4, 0);
        }

        if (index == 0)
        {
            // Set initial value for the timestamp.
            //
            vdis::byte_stream_t
                timestamp_stream(4);
            const uint8_t
                *pdu_buffer = data.get_pdu_buffer();
            uint8_t
                *timestamp_buffer = timestamp_stream.update_buffer();

            timestamp_buffer[0] = pdu_buffer[4];
            timestamp_buffer[1] = pdu_buffer[5];
            timestamp_buffer[2] = pdu_buffer[6];
            timestamp_buffer[3] = pdu_buffer[7];
            timestamp.read(timestamp_stream);

            LOG_EXTRA_VERBOSE(
                "Initial timestamp '%d' has value %d...",
                vdis::to_string(timestamp).c_str(),
                timestamp.value);
        }
        else
        {
            // Update the 4 bytes in the PDU for the timestamp (in PDU header)
            //
            vdis::byte_stream_t
                timestamp_stream(4);
            uint8_t
                *pdu_buffer = data.update_pdu_buffer();

            timestamp.add_milliseconds(interval_milliseconds);
            timestamp.write(timestamp_stream);

            // Set adjusted timestamp on PDU buffer to be sent.
            //
            pdu_buffer[4] = timestamp_stream[0];
            pdu_buffer[5] = timestamp_stream[1];
            pdu_buffer[6] = timestamp_stream[2];
            pdu_buffer[7] = timestamp_stream[3];

            LOG_EXTRA_VERBOSE(
                "Updated timestamp '%s' has value %d...",
                vdis::to_string(timestamp).c_str(),
                timestamp.value);
        }

        success = true;
    }

    return success;
}
