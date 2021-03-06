#include "vdis_byte_buffer.h"
#include "vdis_byte_stream.h"
#include "vdis_integer.h"
#include "vdis_string.h"

// ----------------------------------------------------------------------------
vdis::byte_buffer_t::byte_buffer_t(void) :
    data_length(0),
    data_buffer(0),
    data_allocated(false)
{

}

// ----------------------------------------------------------------------------
vdis::byte_buffer_t::byte_buffer_t(uint32_t size) :
    data_length(size),
    data_buffer(0),
    data_allocated(false)
{
    if (data_length > 0)
    {
        data_buffer = new uint8_t[data_length];
        data_allocated = true;

        std::memset(data_buffer, 0, data_length);
    }
}

// ----------------------------------------------------------------------------
vdis::byte_buffer_t::byte_buffer_t(
    const uint8_t *buffer,
    uint32_t size,
    bool allocate
) :
    data_length(0),
    data_buffer(0),
    data_allocated(false)
{
    if (buffer and (size > 0))
    {
        data_length = size;

        if (allocate)
        {
            data_buffer = new uint8_t[data_length];
            data_allocated = true;
            std::memcpy(data_buffer, buffer, data_length);
        }
        else
        {
            data_buffer = const_cast<uint8_t*>(buffer);
        }
    }
}

// ----------------------------------------------------------------------------
vdis::byte_buffer_t::byte_buffer_t(const byte_buffer_t &copy) :
    data_length(0),
    data_buffer(0),
    data_allocated(false)
{
    if (copy.data_buffer and (copy.data_length > 0))
    {
        data_length = copy.data_length;

        if (copy.data_allocated)
        {
            data_buffer = new uint8_t[data_length];
            data_allocated = true;

            std::memcpy(data_buffer, copy.data_buffer, data_length);
        }
        else
        {
            data_buffer = copy.data_buffer;
        }
    }
}

// ----------------------------------------------------------------------------
vdis::byte_buffer_t::~byte_buffer_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::byte_buffer_t::clear(void)
{
    data_length = 0;

    if (data_buffer and data_allocated)
    {
        delete[] data_buffer;
    }

    data_buffer = 0;
    data_allocated = false;
}

// ----------------------------------------------------------------------------
uint8_t vdis::byte_buffer_t::operator[](uint32_t index) const
{
    if (index < data_length)
    {
        return data_buffer[index];
    }
    else
    {
        return 0;
    }
}

// ----------------------------------------------------------------------------
std::string vdis::byte_buffer_t::str(void) const
{
    std::string
        string;

    if (data_buffer and (data_length > 0))
    {
        char *temp_buffer = new char[data_length + 1];

        // Copy buffer into temporary string buffer
        //
        std::memcpy(temp_buffer, data_buffer, data_length);

        temp_buffer[data_length] = 0;

        // Convert non-printable characters to space characters
        //
        for(uint32_t i = 0; i < data_length; ++i)
        {
            if (not is_printable_character(temp_buffer[i]))
            {
                temp_buffer[i] = ' ';
            }
        }

        string = temp_buffer;

        delete[] temp_buffer;
        temp_buffer = 0;
    }

    return string;
}

// ----------------------------------------------------------------------------
void vdis::byte_buffer_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    static const uint32_t
        byte_width = 8;
    uint32_t
        count,
        header_width = 3;

    if (data_length > 9999999)
    {
        header_width = 8;
    }
    else if (data_length > 999999)
    {
        header_width = 7;
    }
    else if (data_length > 99999)
    {
        header_width = 6;
    }
    else if (data_length > 9999)
    {
        header_width = 5;
    }
    else if (data_length > 999)
    {
        header_width = 4;
    }

    for(uint32_t i = 0; i < data_length;)
    {
        std::string
            header = vdis::to_string(i);
        std::ostringstream
            ascii,
            hexadecimal;

        while(header.length() < header_width)
        {
            header = ("0" + header);
        }

        for(uint32_t j = 0; (j < byte_width); ++i, ++j)
        {
            if (i < data_length)
            {
                hexadecimal << to_hex_string(data_buffer[i]);
            }
            else
            {
                hexadecimal << "  ";
            }

            if ((j % byte_width) != (byte_width - 1))
            {
                hexadecimal << ' ';
            }

            if (i >= data_length)
            {
                ascii << ' ';
            }
            else if ((data_buffer[i] > 31) && (data_buffer[i] < 127))
            {
                ascii << char(data_buffer[i]);
            }
            else
            {
                ascii << '.';
            }
        }

        count = ((byte_width * 3) - 1);

        while(hexadecimal.str().size() < count)
        {
            hexadecimal << ' ';
        }

        out << prefix << header << "  "
            << hexadecimal.str() << "  "
            << ascii.str() << std::endl;
    }
}

// ----------------------------------------------------------------------------
// Reads 32-bit length before reading the buffer.
//
void vdis::byte_buffer_t::read(byte_stream_t &stream)
{
    uint32_t
        size = 0;

    stream.read(size);
    read(stream, size);
}

// ----------------------------------------------------------------------------
// Reads just the buffer.
//
void vdis::byte_buffer_t::read(byte_stream_t &stream, uint32_t size)
{
    clear();

    if ((size > 0) and not stream.error())
    {
        data_length = size;
        data_buffer = new uint8_t[data_length];
        data_allocated = true;

        stream.read(data_buffer, data_length);
    }
}

// ----------------------------------------------------------------------------
// Writes 32-bit length before writing the buffer.
//
void vdis::byte_buffer_t::write(byte_stream_t &stream)
{
    stream.write(data_length);

    if (data_length and data_buffer)
    {
        stream.write(data_buffer, data_length);
    }
}

// ----------------------------------------------------------------------------
// Writes just the buffer.
//
void vdis::byte_buffer_t::write(byte_stream_t &stream, uint32_t size)
{
    if (data_buffer and (data_length > 0) and (size > 0))
    {
        if (data_length < size)
        {
            stream.write(data_buffer, data_length);
        }
        else
        {
            stream.write(data_buffer, size);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_buffer_t::reset(uint32_t size)
{
    if (size == 0)
    {
        clear();
    }
    else
    {
        if ((size != data_length) or not data_allocated)
        {
            clear();

            data_length = size;
            data_buffer = new uint8_t[data_length];
            data_allocated = true;
        }

        std::memset(data_buffer, 0, data_length);
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_buffer_t::reset(
    const uint8_t *buffer,
    uint32_t size,
    bool allocate)
{
    clear();

    if (buffer and (size > 0))
    {
        data_length = size;

        if (allocate)
        {
            data_buffer = new uint8_t[data_length];
            data_allocated = true;
            std::memcpy(data_buffer, buffer, data_length);
        }
        else
        {
            data_buffer = const_cast<uint8_t*>(buffer);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::byte_buffer_t::reset(const byte_buffer_t &buffer)
{
    clear();

    if (buffer.data_buffer and (buffer.data_length > 0))
    {
        data_length = buffer.data_length;
        data_buffer = new uint8_t[data_length];
        data_allocated = true;

        std::memcpy(data_buffer, buffer.data_buffer, data_length);
    }
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::byte_buffer_t &o)
{
    o.print(std::string(), out);

    return out;
}
