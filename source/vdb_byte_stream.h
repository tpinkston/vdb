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

#ifndef VDB_BYTE_STREAM_H
#define VDB_BYTE_STREAM_H

#include "vdb_common.h"

// Values for vdb::byte_stream::operation_t
//
#define BYTE_STREAM_READ     "read"
#define BYTE_STREAM_WRITE    "write"

namespace vdb
{
    // ------------------------------------------------------------------------
    class byte_stream
    {
      public:

        byte_stream(void);
        byte_stream(uint32_t length);
        byte_stream(const uint8_t *buffer, uint32_t length);
        byte_stream(const byte_stream &copy);
        virtual ~byte_stream(void);

        typedef enum
        {
            EMPTY,                // Buffer is null, next operation will fail
            SUCCESS_READY,        // Last operation successful, ready for next
            SUCCESS_END,          // Last operation successful, at EOB
            ERROR_READ_PAST_END,  // Error read past end of buffer
            ERROR_WRITE_PAST_END, // Error write past end of buffer
            ERROR_INDEX_PAST_END  // Index set past end of buffer
        } status_e;

        status_e status(void) const { return stream_status; }

        // Returns true if status is SUCCESS_READY.
        //
        bool empty(void) const { return (stream_status == EMPTY); }

        // Returns true if status is any 'SUCCESS' value.
        //
        bool success(void) const;

        // Returns true if status is any 'ERROR' value.
        //
        bool error(void) const;

        // Returns true if status is SUCCESS_READY.
        //
        bool operator()(void) const;

        const uint8_t *get_buffer(void) const;
        uint32_t get_index(void) const;
        uint32_t get_length(void) const;
        uint32_t get_length_remaining(void) const;

        // --------------------------------------------------------------------
        // Object result, empties buffer (no arguments) or resets buffer with
        // provided buffer.
        //
        void reset(void);
        void reset(const uint8_t *buffer, uint32_t length);

        // --------------------------------------------------------------------
        // Index manipulation
        //
        void reset_index(uint32_t value = 0);
        void skip(uint32_t count);

        // --------------------------------------------------------------------
        // Adjusts buffer size, truncating if needed.
        //
        void set_length(uint32_t length);

        // --------------------------------------------------------------------
        // Index operator on buffer
        //
        uint8_t operator[](uint32_t index) const;

        // --------------------------------------------------------------------
        // Template read and write operations
        //
        template<typename T>
        void read(T &value);
        template<typename T>
        void write(const T &value);

        // --------------------------------------------------------------------
        // String read and write operations - reads and writes four bytes for
        // length and the string itself in 4-byte blocks (possibility for up
        // to 3 zero-bytes of padding at the end).
        //
        void read(std::string &value);
        void write(const std::string &value);

        // --------------------------------------------------------------------
        // Block read and write operations
        //
        void read(void *value_ptr, uint32_t length);
        void write(const void *value_ptr, uint32_t length);

        // --------------------------------------------------------------------
        // Numeric stream read operations
        //
        void read(int8_t &value);
        void read(uint8_t &value);
        void read(int16_t &value);
        void read(uint16_t &value);
        void read(int32_t &value);
        void read(uint32_t &value);
        void read(int64_t &value);
        void read(uint64_t &value);
        void read(float32_t &value);
        void read(float64_t &value);

        // --------------------------------------------------------------------
        // Numeric stream write operations
        //
        void write(int8_t value);
        void write(uint8_t value);
        void write(int16_t value);
        void write(uint16_t value);
        void write(int32_t value);
        void write(uint32_t value);
        void write(int64_t value);
        void write(uint64_t value);
        void write(float32_t value);
        void write(float64_t value);

      protected:

        typedef std::string operation_t;

        bool read_ready(uint32_t length);
        bool write_ready(uint32_t length);
        bool operation_ready(uint32_t length, operation_t operation);

        uint8_t
            *buffer;
        uint32_t
            buffer_length,
            buffer_index;
        status_e
            stream_status;
    };

    // ------------------------------------------------------------------------
    inline bool byte_stream::operator()(void) const
    {
        return (stream_status == SUCCESS_READY);
    }

    // ------------------------------------------------------------------------
    inline const uint8_t *byte_stream::get_buffer(void) const
    {
        return buffer;
    }

    // ------------------------------------------------------------------------
    inline uint32_t byte_stream::get_index(void) const
    {
        return buffer_index;
    }

    // ------------------------------------------------------------------------
    inline uint32_t byte_stream::get_length(void) const
    {
        return buffer_length;
    }

    // ------------------------------------------------------------------------
    inline uint32_t byte_stream::get_length_remaining(void) const
    {
        return (buffer_length - buffer_index);
    }

    // ------------------------------------------------------------------------
    template<typename T>
    void byte_stream::read(T &value)
    {
        value.read(*this);
    }

    // ------------------------------------------------------------------------
    template<typename T>
    void byte_stream::write(const T &value)
    {
        value.write(*this);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(int8_t &value)
    {
        read(*reinterpret_cast<uint8_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(uint8_t &value)
    {
        if (read_ready(1))
        {
            value = buffer[buffer_index++];
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(int16_t &value)
    {
        read(*reinterpret_cast<uint16_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(uint16_t &value)
    {
        if (read_ready(2))
        {
            value = 0;

            value |= buffer[buffer_index++] << 8;
            value |= buffer[buffer_index++];

            byteswap(value);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(int32_t &value)
    {
        read(*reinterpret_cast<uint32_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(uint32_t &value)
    {
        if (read_ready(4))
        {
            value = 0;

            value |= buffer[buffer_index++] << 24;
            value |= buffer[buffer_index++] << 16;
            value |= buffer[buffer_index++] << 8;
            value |= buffer[buffer_index++];

            byteswap(value);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(int64_t &value)
    {
        read(*reinterpret_cast<uint64_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(uint64_t &value)
    {
        if (read_ready(8))
        {
            value = 0;

            value |= (uint64_t)buffer[buffer_index++] << 56;
            value |= (uint64_t)buffer[buffer_index++] << 48;
            value |= (uint64_t)buffer[buffer_index++] << 40;
            value |= (uint64_t)buffer[buffer_index++] << 32;
            value |= (uint64_t)buffer[buffer_index++] << 24;
            value |= (uint64_t)buffer[buffer_index++] << 16;
            value |= (uint64_t)buffer[buffer_index++] << 8;
            value |= (uint64_t)buffer[buffer_index++];

            byteswap(value);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(float32_t &value)
    {
        if (read_ready(4))
        {
            uint32_t
                temp = 0;

            temp |= buffer[buffer_index++] << 24;
            temp |= buffer[buffer_index++] << 16;
            temp |= buffer[buffer_index++] << 8;
            temp |= buffer[buffer_index++];

            value = get_float32(temp);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::read(float64_t &value)
    {
        if (read_ready(8))
        {
            uint64_t
                temp = 0;

            temp |= (uint64_t)buffer[buffer_index++] << 56;
            temp |= (uint64_t)buffer[buffer_index++] << 48;
            temp |= (uint64_t)buffer[buffer_index++] << 40;
            temp |= (uint64_t)buffer[buffer_index++] << 32;
            temp |= (uint64_t)buffer[buffer_index++] << 24;
            temp |= (uint64_t)buffer[buffer_index++] << 16;
            temp |= (uint64_t)buffer[buffer_index++] << 8;
            temp |= (uint64_t)buffer[buffer_index++];

            value = get_float64(temp);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(int8_t value)
    {
        write((uint8_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(uint8_t value)
    {
        if (write_ready(1))
        {
            buffer[buffer_index++] = value;
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(int16_t value)
    {
        write((uint16_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(uint16_t value)
    {
        if (write_ready(2))
        {
            byteswap(value);
            buffer[buffer_index++] = (uint8_t)((value >> 8) & 0xFF);
            buffer[buffer_index++] = (uint8_t)(value & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(int32_t value)
    {
        write((uint32_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(uint32_t value)
    {
        if (write_ready(4))
        {
            byteswap(value);
            buffer[buffer_index++] = (uint8_t)((value >> 24) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 16) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 8) & 0xFF);
            buffer[buffer_index++] = (uint8_t)(value & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(int64_t value)
    {
        write((uint64_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(uint64_t value)
    {
        if (write_ready(8))
        {
            byteswap(value);
            buffer[buffer_index++] = (uint8_t)((value >> 56) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 48) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 40) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 32) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 24) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 16) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((value >> 8) & 0xFF);
            buffer[buffer_index++] = (uint8_t)(value & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(float32_t value)
    {
        if (write_ready(4))
        {
            const uint32_t
                integer = get_int32(value);

            buffer[buffer_index++] = (uint8_t)((integer >> 24) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 16) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 8) & 0xFF);
            buffer[buffer_index++] = (uint8_t)(integer & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream::write(float64_t value)
    {
        if (write_ready(8))
        {
            const uint64_t
                integer = get_int64(value);

            buffer[buffer_index++] = (uint8_t)((integer >> 56) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 48) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 40) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 32) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 24) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 16) & 0xFF);
            buffer[buffer_index++] = (uint8_t)((integer >> 8) & 0xFF);
            buffer[buffer_index++] = (uint8_t)(integer & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline bool byte_stream::read_ready(uint32_t length)
    {
        return operation_ready(length, BYTE_STREAM_READ);
    }

    // ------------------------------------------------------------------------
    inline bool byte_stream::write_ready(uint32_t length)
    {
        return operation_ready(length, BYTE_STREAM_WRITE);
    }
}


// ----------------------------------------------------------------------------
inline std::ostream &operator<<(
    std::ostream &stream,
    vdb::byte_stream::status_e value)
{
    switch(value)
    {
        case vdb::byte_stream::EMPTY:
            stream << "EMPTY";
            break;
        case vdb::byte_stream::SUCCESS_READY:
            stream << "SUCCESS_READY";
            break;
        case vdb::byte_stream::SUCCESS_END:
            stream << "SUCCESS_END";
            break;
        case vdb::byte_stream::ERROR_READ_PAST_END:
            stream << "ERROR_READ_PAST_END";
            break;
        case vdb::byte_stream::ERROR_WRITE_PAST_END:
            stream << "ERROR_WRITE_PAST_END";
            break;
        case vdb::byte_stream::ERROR_INDEX_PAST_END:
            stream << "ERROR_INDEX_PAST_END";
            break;
        default:
            stream << "UNKNOWN_" << (int)value;
    }

    return stream;
}

#endif
