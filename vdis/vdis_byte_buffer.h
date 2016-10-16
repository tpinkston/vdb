#ifndef VDIS_BYTE_BUFFER_H
#define VDIS_BYTE_BUFFER_H

#include "vdis_data_types.h"

namespace vdis
{
    class byte_stream_t;

    class byte_buffer_t
    {
      public:

        byte_buffer_t(void);
        byte_buffer_t(uint32_t size);
        byte_buffer_t(const uint8_t *buffer, uint32_t size, bool allocate = true);
        byte_buffer_t(const byte_buffer_t &copy);
        virtual ~byte_buffer_t(void);

        void clear(void);

        inline const uint8_t *buffer(void) const { return data_buffer; }
        inline uint8_t *update_buffer(void) { return data_buffer; }

        inline uint32_t length(void) const { return data_length; }

        uint8_t operator[](uint32_t index) const;

        std::string str(void) const;

        void print(const std::string &, std::ostream &) const;

        void read(byte_stream_t &);
        void read(byte_stream_t &, uint32_t size);
        void write(byte_stream_t &);
        void write(byte_stream_t &, uint32_t size);

        virtual void reset(uint32_t size = 0);
        virtual void reset(const uint8_t *buffer, uint32_t size, bool allocate = true);
        virtual void reset(const byte_buffer_t &buffer);

      protected:

        uint32_t    data_length;
        uint8_t    *data_buffer;

      private:

        // When true the 'data_buffer' will be deallocated in deconstructor,
        // false implies that this buffer is just a reference to another
        // buffer.
        //
        bool        data_allocated;
    };
}

std::ostream &operator<<(std::ostream &, const vdis::byte_buffer_t &);

#endif
