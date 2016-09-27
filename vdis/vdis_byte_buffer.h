#ifndef VDIS_BYTE_BUFFER_H
#define VDIS_BYTE_BUFFER_H

#include <cstdint>
#include <iostream>

namespace vdis
{
    class byte_buffer_t
    {
      public:

        byte_buffer_t(void);
        byte_buffer_t(uint32_t size);
        byte_buffer_t(const uint8_t *buffer, uint32_t size, bool copy = true);
        byte_buffer_t(const byte_buffer_t &copy);
        virtual ~byte_buffer_t(void);

        void clear(void);

        const uint8_t *buffer(void) const;

        uint8_t operator[](uint32_t index) const;

        uint32_t length(void) const;

        void print(const std::string &, std::ostream &) const;

        virtual void reset(const uint8_t *buffer, uint32_t size);
        virtual void reset(const byte_buffer_t &buffer);

      protected:

        uint32_t    data_length;
        uint8_t    *data_buffer;

      private:

        bool        data_allocated;
    };

    // ------------------------------------------------------------------------
    inline const uint8_t *vdis::byte_buffer_t::buffer(void) const
    {
        return data_buffer;
    }

    // ------------------------------------------------------------------------
    inline uint32_t vdis::byte_buffer_t::length(void) const
    {
        return data_length;
    }
}

std::ostream &operator<<(std::ostream &, const vdis::byte_buffer_t &);

#endif
