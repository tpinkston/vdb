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
#include "vdb_data_buffer.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"

// ----------------------------------------------------------------------------
vdb::data_buffer_t::data_buffer_t(void) : data_length(0), data_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdb::data_buffer_t::data_buffer_t(uint16_t length) :
    data_length(length),
    data_ptr(0)
{
    if (data_length > 0)
    {
        data_ptr = new uint8_t[data_length];

        std::memset(data_ptr, 0, data_length);
    }
}

// ----------------------------------------------------------------------------
vdb::data_buffer_t::data_buffer_t(uint16_t length, const uint8_t *data) :
    data_length(0),
    data_ptr(0)
{
    if (data and data_length > 0)
    {
        data_length = length;
        data_ptr = new uint8_t[data_length];

        std::memcpy(data_ptr, data, data_length);
    }
}

// ----------------------------------------------------------------------------
vdb::data_buffer_t::data_buffer_t(const data_buffer_t &copy) :
    data_length(0),
    data_ptr(0)
{
    if (copy.data_ptr and (copy.data_length > 0))
    {
        data_length = copy.data_length;
        data_ptr = new uint8_t[data_length];

        std::memcpy(data_ptr, copy.data_ptr, data_length);
    }
}

// ----------------------------------------------------------------------------
vdb::data_buffer_t::~data_buffer_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
uint8_t vdb::data_buffer_t::operator[](uint32_t index) const
{
    if (index < data_length)
    {
        return data_ptr[index];
    }
    else
    {
        // Do not abort with index out of bounds, log an error and return
        // zero.
        //
        LOG_ERROR(
            "Indexing past end of of buffer at %d, buffer length %d",
            index,
            data_length);

        return 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::data_buffer_t::clear(void)
{
    data_length = 0;

    if (data_ptr)
    {
        delete data_ptr;
        data_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::data_buffer_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "length " << data_length << std::endl;

    if (data_ptr and (data_length > 0))
    {
        hexadecimal::stream(
            (prefix + "data "),
            data_ptr,
            data_length,
            8,
            stream);
    }
}

// ----------------------------------------------------------------------------
// Assumes the 'data_length' member has been set to the length of data to be
// read from byte stream.  The 'data_ptr' does not need to be allocated.
//
void vdb::data_buffer_t::read(byte_stream &stream)
{
    // Free up the old buffer if already allocated.
    //
    if (data_ptr)
    {
        delete[] data_ptr;
        data_ptr = 0;
    }

    if (data_length > stream.get_length_remaining())
    {
        LOG_ERROR(
            "Data buffer length (%d) exceeds bytes remaining (%d)!",
            data_length,
            stream.get_length_remaining());

        data_length = 0;
    }
    else if (data_length == 0)
    {
        LOG_WARNING("Reading zero bytes into data buffer!");
    }
    else
    {
        data_ptr = new uint8_t[data_length];

        stream.read(data_ptr, data_length);
    }
}

// ----------------------------------------------------------------------------
void vdb::data_buffer_t::write(byte_stream &stream) const
{
    if (data_ptr and (data_length > 0))
    {
        stream.write(data_ptr, data_length);
    }
}
