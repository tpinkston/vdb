#ifndef VDIS_BYTE_STREAM_H
#define VDIS_BYTE_STREAM_H

#include "vdis_byte_buffer.h"
#include "vdis_integer.h"
#include "vdis_services.h"

namespace vdis
{
    class byte_stream_t : public byte_buffer_t
    {
      public:

        byte_stream_t(void);
        byte_stream_t(uint32_t size);
        byte_stream_t(const uint8_t *buffer, uint32_t size, bool copy = true);
        byte_stream_t(const byte_buffer_t &buffer);
        byte_stream_t(const byte_stream_t &copy);
        virtual ~byte_stream_t(void);

        // Returns point to the buffer plus offset (index).
        //
        const uint8_t *offset_buffer(void) const;

        bool operator()(void) const;

        uint32_t index(void) const;

        uint32_t remaining_length(void) const;

        virtual void reset(const uint8_t *buffer, uint32_t size);
        virtual void reset(const byte_buffer_t &buffer);

        void reset_index(uint32_t value = 0);

        void skip(uint32_t count);

        void resize(uint32_t size);

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

        void read(byte_buffer_t &buffer, uint32_t size);
        void write(const byte_buffer_t &buffer);
        void write(const byte_buffer_t &buffer, uint32_t size);

        void read(void *value_ptr, uint32_t size);
        void write(const void *value_ptr, uint32_t size);

      protected:

        typedef std::string operation_t;

        static const operation_t
            BYTE_STREAM_OP_READ,
            BYTE_STREAM_OP_WRITE;

        bool read_ready(uint32_t size);
        bool write_ready(uint32_t size);
        bool operation_ready(const operation_t &operation, uint32_t size);

        uint32_t
            buffer_index;
        bool
            buffer_status;
    };

    // ------------------------------------------------------------------------
    inline const uint8_t *vdis::byte_stream_t::offset_buffer(void) const
    {
        return (data_buffer ? (data_buffer + buffer_index) : 0);
    }

    // ------------------------------------------------------------------------
    // True if bytes available to read or write
    //
    inline bool byte_stream_t::operator()(void) const
    {
        return buffer_status;
    }

    // ------------------------------------------------------------------------
    inline uint32_t byte_stream_t::index(void) const
    {
        return buffer_index;
    }

    // ------------------------------------------------------------------------
    inline uint32_t byte_stream_t::remaining_length(void) const
    {
        return (data_length - buffer_index);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(int8_t &value)
    {
        read(*reinterpret_cast<uint8_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(uint8_t &value)
    {
        if (read_ready(1))
        {
            value = data_buffer[buffer_index++];
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(int16_t &value)
    {
        read(*reinterpret_cast<uint16_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(uint16_t &value)
    {
        if (read_ready(2))
        {
            value = 0;
            value |= data_buffer[buffer_index++] << 8;
            value |= data_buffer[buffer_index++];
            swap(value);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(int32_t &value)
    {
        read(*reinterpret_cast<uint32_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(uint32_t &value)
    {
        if (read_ready(4))
        {
            value = 0;
            value |= data_buffer[buffer_index++] << 24;
            value |= data_buffer[buffer_index++] << 16;
            value |= data_buffer[buffer_index++] << 8;
            value |= data_buffer[buffer_index++];
            swap(value);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(int64_t &value)
    {
        read(*reinterpret_cast<uint64_t*>(&value));
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(uint64_t &value)
    {
        if (read_ready(8))
        {
            value = 0;
            value |= (uint64_t)data_buffer[buffer_index++] << 56;
            value |= (uint64_t)data_buffer[buffer_index++] << 48;
            value |= (uint64_t)data_buffer[buffer_index++] << 40;
            value |= (uint64_t)data_buffer[buffer_index++] << 32;
            value |= (uint64_t)data_buffer[buffer_index++] << 24;
            value |= (uint64_t)data_buffer[buffer_index++] << 16;
            value |= (uint64_t)data_buffer[buffer_index++] << 8;
            value |= (uint64_t)data_buffer[buffer_index++];
            swap(value);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(float32_t &value)
    {
        if (read_ready(4))
        {
            uint32_t
                temp = 0;

            temp |= data_buffer[buffer_index++] << 24;
            temp |= data_buffer[buffer_index++] << 16;
            temp |= data_buffer[buffer_index++] << 8;
            temp |= data_buffer[buffer_index++];

            value = to_float32(temp);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::read(float64_t &value)
    {
        if (read_ready(8))
        {
            uint64_t
                temp = 0;

            temp |= (uint64_t)data_buffer[buffer_index++] << 56;
            temp |= (uint64_t)data_buffer[buffer_index++] << 48;
            temp |= (uint64_t)data_buffer[buffer_index++] << 40;
            temp |= (uint64_t)data_buffer[buffer_index++] << 32;
            temp |= (uint64_t)data_buffer[buffer_index++] << 24;
            temp |= (uint64_t)data_buffer[buffer_index++] << 16;
            temp |= (uint64_t)data_buffer[buffer_index++] << 8;
            temp |= (uint64_t)data_buffer[buffer_index++];

            value = to_float64(temp);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(int8_t value)
    {
        write((uint8_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(uint8_t value)
    {
        if (write_ready(1))
        {
            data_buffer[buffer_index++] = value;
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(int16_t value)
    {
        write((uint16_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(uint16_t value)
    {
        if (write_ready(2))
        {
            swap(value);
            data_buffer[buffer_index++] = (uint8_t)((value >> 8) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)(value & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(int32_t value)
    {
        write((uint32_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(uint32_t value)
    {
        if (write_ready(4))
        {
            swap(value);
            data_buffer[buffer_index++] = (uint8_t)((value >> 24) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 16) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 8) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)(value & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(int64_t value)
    {
        write((uint64_t)value);
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(uint64_t value)
    {
        if (write_ready(8))
        {
            swap(value);
            data_buffer[buffer_index++] = (uint8_t)((value >> 56) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 48) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 40) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 32) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 24) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 16) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((value >> 8) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)(value & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(float32_t value)
    {
        if (write_ready(4))
        {
            const uint32_t
                integer = to_int32(value);

            data_buffer[buffer_index++] = (uint8_t)((integer >> 24) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 16) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 8) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)(integer & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline void byte_stream_t::write(float64_t value)
    {
        if (write_ready(8))
        {
            const uint64_t
                integer = to_int64(value);

            data_buffer[buffer_index++] = (uint8_t)((integer >> 56) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 48) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 40) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 32) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 24) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 16) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)((integer >> 8) & 0xFF);
            data_buffer[buffer_index++] = (uint8_t)(integer & 0xFF);
        }
    }

    // ------------------------------------------------------------------------
    inline bool byte_stream_t::read_ready(uint32_t size)
    {
        return operation_ready(BYTE_STREAM_OP_READ, size);
    }

    // ------------------------------------------------------------------------
    inline bool byte_stream_t::write_ready(uint32_t size)
    {
        return operation_ready(BYTE_STREAM_OP_WRITE, size);
    }
}

#endif
