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
#include "vdb_logger.h"

// ----------------------------------------------------------------------------
vdb::byte_stream::byte_stream(void) :
    buffer(0),
    buffer_length(0),
    buffer_index(0),
    stream_status(EMPTY)
{

}

// ----------------------------------------------------------------------------
vdb::byte_stream::byte_stream(uint32_t length) :
    buffer(0),
    buffer_length(0),
    buffer_index(0),
    stream_status(EMPTY)
{
    set_length(length);
}

// ----------------------------------------------------------------------------
vdb::byte_stream::byte_stream(const uint8_t *pointer, uint32_t length) :
    buffer(0),
    buffer_length(0),
    buffer_index(0),
    stream_status(EMPTY)
{
    reset(pointer, length);
}

// ----------------------------------------------------------------------------
// Deep copy of memory allocation, index and status does not get copied.
//
vdb::byte_stream::byte_stream(const byte_stream &copy) :
    buffer_length(0),
    buffer_index(0),
    stream_status(EMPTY)
{
    if (copy.buffer_length > 0)
    {
        reset(copy.buffer, copy.buffer_length);
    }
}

// ----------------------------------------------------------------------------
vdb::byte_stream::~byte_stream(void)
{
    reset();
}

// ----------------------------------------------------------------------------
bool vdb::byte_stream::success(void) const
{
    switch(stream_status)
    {
        case SUCCESS_READY:
        case SUCCESS_END:
            return true;
        default:
            return false;
    }
}

// ----------------------------------------------------------------------------
bool vdb::byte_stream::error(void) const
{
    switch(stream_status)
    {
        case ERROR_READ_PAST_END:
        case ERROR_WRITE_PAST_END:
        case ERROR_INDEX_PAST_END:
            return true;
        default:
            return false;
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::reset(void)
{
    if (buffer)
    {
        delete[] buffer;
    }

    buffer = 0;
    buffer_length = 0;
    buffer_index = 0;
    stream_status = EMPTY;
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::reset(const uint8_t *pointer, uint32_t length)
{
    reset();

    if (pointer and (length > 0))
    {
        buffer = new uint8_t[length];
        buffer_length = length;
        stream_status = SUCCESS_READY;

        std::memcpy(buffer, pointer, buffer_length);
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::reset_index(uint32_t value)
{
    if (buffer)
    {
        buffer_index = value;

        if (buffer_index > buffer_length)
        {
            LOG_ERROR(
                "Reset index past end of buffer (%d/%d)",
                buffer_index,
                buffer_length);

            stream_status = ERROR_INDEX_PAST_END;
        }
        else if (buffer_index == buffer_length)
        {
            stream_status = SUCCESS_END;
        }
        else
        {
            stream_status = SUCCESS_READY;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::skip(uint32_t count)
{
    if (success())
    {
        LOG_EXTRA_VERBOSE("Byte stream skip: %d...", count);

        buffer_index += count;

        if (buffer_index == buffer_length)
        {
            stream_status = SUCCESS_END;
        }
        else if (buffer_index > buffer_length)
        {
            LOG_ERROR(
                "Skipped past end of buffer (%d/%d)",
                buffer_index,
                buffer_length);

            stream_status = ERROR_INDEX_PAST_END;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::set_length(uint32_t length)
{
    if (length == 0)
    {
        reset();
    }
    else if (buffer_length == 0)
    {
        buffer = new uint8_t[length];
        buffer_length = length;
        buffer_index = 0;
        stream_status = SUCCESS_READY;

        std::memset(buffer, 0, buffer_length);
    }
    else
    {
        uint8_t
            *new_buffer = new uint8_t[length];
        size_t
            size = std::min(length, buffer_length);

        std::memset(new_buffer, 0, length);
        std::memcpy(new_buffer, buffer, size);

        buffer = new_buffer;
        buffer_length = length;

        if (buffer_index < buffer_length)
        {
            stream_status = SUCCESS_READY;
        }
        else
        {
            buffer_index = buffer_length;
            stream_status = SUCCESS_END;
        }

        delete[] new_buffer;
        new_buffer = 0;
    }
}

// ----------------------------------------------------------------------------
uint8_t vdb::byte_stream::operator[](uint32_t index) const
{
    if (index < buffer_length)
    {
        return buffer[index];
    }
    else
    {
        // Do not abort with index out of bounds, log an error and return
        // zero.
        //
        LOG_ERROR(
            "Indexing past end of of buffer at %d, buffer length %d",
            index,
            buffer_length);

        return 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::read(std::string &value)
{
    if (read_ready(4))
    {
        uint32_t
            length;

        read(length);

        if (length == 0)
        {
            value.clear();
        }
        else if (read_ready(length))
        {
            char
                string_buffer[length + 1];

            std::memset(string_buffer, 0, (length + 1));

            for(uint32_t i = 0; i < length; ++i, ++buffer_index)
            {
                string_buffer[i] = (char)buffer[buffer_index];
            }

            value = string_buffer;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::write(const std::string &value)
{
    uint32_t
        length = value.length();

    length += (length % 4);

    if (write_ready(length + 4))
    {
        write(length);

        for(uint32_t i = 0; i < length; ++i, ++buffer_index)
        {
            if (i < value.length())
            {
                buffer[buffer_index] = (uint8_t)value[i];
            }
            else
            {
                buffer[buffer_index] = 0;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::read(void *value_ptr, uint32_t length)
{
    if (read_ready(length))
    {
        std::memcpy(value_ptr, (buffer + buffer_index), length);
        buffer_index += length;
    }
}

// ----------------------------------------------------------------------------
void vdb::byte_stream::write(const void *value_ptr, uint32_t length)
{
    if (write_ready(length))
    {
        std::memcpy((buffer + buffer_index), value_ptr, length);
        buffer_index += length;
    }
}

// ----------------------------------------------------------------------------
bool vdb::byte_stream::operation_ready(uint32_t length, operation_t operation)
{
    bool
        result = true;

    LOG_EXTRA_VERBOSE("Byte stream check %s: %d...", operation.c_str(), length);

    if (empty())
    {
        result = false;

        LOG_ERROR(
            "Operation '%s' attempted on empty buffer!",
            operation.c_str());
    }
    else if (success() and ((buffer_index + length) == buffer_length))
    {
        stream_status = SUCCESS_END;
    }
    else if (success() and ((buffer_index + length) > buffer_length))
    {
        result = false;

        LOG_ERROR(
            "Operation '%s' attempted past end of of buffer at index %d "
            "(length %d), buffer length %d",
            operation.c_str(),
            buffer_index,
            length,
            buffer_length);

        if (operation == BYTE_STREAM_READ)
        {
            stream_status = ERROR_READ_PAST_END;
        }
        else if (operation == BYTE_STREAM_WRITE)
        {
            stream_status = ERROR_WRITE_PAST_END;
        }
        else
        {
            stream_status = ERROR_INDEX_PAST_END;
        }
    }

    return result;
}
