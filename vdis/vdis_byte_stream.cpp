#include <algorithm>
#include <cstring>
#include <limits>

#include "vdis_byte_stream.h"
#include "vdis_logger.h"

const vdis::byte_stream_t::operation_t
    vdis::byte_stream_t::BYTE_STREAM_OP_READ = "read",
    vdis::byte_stream_t::BYTE_STREAM_OP_WRITE = "write";

// ----------------------------------------------------------------------------
vdis::byte_stream_t::byte_stream_t(void) :
    buffer_index(0),
    buffer_status(false)
{

}

// ----------------------------------------------------------------------------
vdis::byte_stream_t::byte_stream_t(uint32_t size) :
    byte_buffer_t(size),
    buffer_index(0),
    buffer_status(size > 0)
{

}

// ----------------------------------------------------------------------------
vdis::byte_stream_t::byte_stream_t(
    const uint8_t *buffer,
    uint32_t size,
    bool copy
) :
    byte_buffer_t(buffer, size, copy),
    buffer_index(0),
    buffer_status(buffer and (size > 0))
{

}

// ----------------------------------------------------------------------------
// Deep copy of buffer.
//
vdis::byte_stream_t::byte_stream_t(const byte_buffer_t &buffer) :
    byte_buffer_t(buffer),
    buffer_index(0),
    buffer_status(buffer.buffer())
{

}

// ----------------------------------------------------------------------------
// Deep copy of buffer, index and status do not get copied.
//
vdis::byte_stream_t::byte_stream_t(const byte_stream_t &copy) :
    byte_buffer_t(copy),
    buffer_index(0),
    buffer_status(copy.buffer())
{

}

// ----------------------------------------------------------------------------
vdis::byte_stream_t::~byte_stream_t(void)
{

}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::reset(const uint8_t *buffer, uint32_t size)
{
    byte_buffer_t::reset(buffer, size);

    buffer_index = 0;
    buffer_status = (length() > 0);
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::reset(const byte_buffer_t &buffer)
{
    reset(buffer.buffer(), buffer.length());
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::reset_index(uint32_t value)
{
    if (buffer())
    {
        buffer_index = value;
        buffer_status = (buffer_index < length());

        if (buffer_index > length())
        {
            LOG_ERROR(
                "Index reset past end of buffer (%d/%d)",
                buffer_index,
                data_length);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::skip(uint32_t count)
{
    if ((*this)())
    {
        LOG_EXTRA_VERBOSE("Data stream skip: %d...", count);

        buffer_index += count;
        buffer_status = (buffer_index < length());

        if (buffer_index > length())
        {
            LOG_ERROR(
                "Skip past end of buffer (%d/%d)",
                buffer_index,
                data_length);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::resize(uint32_t size)
{
    if (size == 0)
    {
        clear();
    }
    else if (data_length == 0)
    {
        data_buffer = new uint8_t[size];
        data_length = size;
        buffer_index = 0;
        buffer_status = true;

        std::memset(data_buffer, 0, data_length);
    }
    else
    {
        uint8_t
            *new_buffer = new uint8_t[size];
        size_t
            copy_size = std::min(size, data_length);

        std::memset(new_buffer, 0, size);
        std::memcpy(new_buffer, data_buffer, copy_size);

        data_buffer = new_buffer;
        data_length = size;

        if (buffer_index < data_length)
        {
            buffer_status = true;
        }
        else
        {
            buffer_index = data_length;
            buffer_status = false;
        }

        delete[] new_buffer;
        new_buffer = 0;
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::read(byte_buffer_t &buffer, uint32_t size)
{
    if (read_ready(size))
    {
        uint8_t *temp_buffer = new uint8_t[size];

        read(temp_buffer, size);

        buffer.reset(temp_buffer, size, false);
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::write(const byte_buffer_t &buffer)
{
    if ((buffer.length() > 0) and write_ready(buffer.length()))
    {
        write(buffer.buffer(), buffer.length());
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::write(const byte_buffer_t &buffer, uint32_t size)
{
    if (size > buffer.length())
    {
        size = buffer.length();
    }

    if ((size > 0) and write_ready(size))
    {
        write(buffer.buffer(), size);
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::read(void *value_ptr, uint32_t size)
{
    if (read_ready(size))
    {
        std::memcpy(value_ptr, (data_buffer + buffer_index), size);
        buffer_index += size;
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_stream_t::write(const void *value_ptr, uint32_t size)
{
    if (write_ready(size))
    {
        std::memcpy((data_buffer + buffer_index), value_ptr, size);
        buffer_index += size;
    }
}

// ----------------------------------------------------------------------------
bool vdis::byte_stream_t::operation_ready(
    const operation_t &operation,
    uint32_t size)
{
    LOG_EXTRA_VERBOSE("Data stream check %s: %d...", operation.c_str(), size);

    if (not (*this)())
    {
        LOG_ERROR(
            "Operation '%s' attempted on null buffer!",
            operation.c_str());
    }
    else if ((index() + size) > length())
    {
        LOG_ERROR(
            "Operation '%s' attempted past end of of buffer at index %d "
            "(length %d), buffer length %d",
            operation.c_str(),
            buffer_index,
            size,
            data_length);

        buffer_status = false;
    }

    return (*this)();
}
