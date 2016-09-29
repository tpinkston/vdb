#ifndef VDIS_SV_RECORDS_H
#define VDIS_SV_RECORDS_H

#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_enums.h"

namespace vdis
{
    // Standard Variable (SV) records are used by the following:
    //
    //    Entity Damage Status PDU
    //    Application Control PDU
    //
    struct sv_record_t;

    // Returns array with length equal to 'count'
    //
    sv_record_t **read_sv_records(byte_stream_t &stream, uint16_t count);

    // ------------------------------------------------------------------------
    struct sv_content_t
    {
        virtual ~sv_content_t(void) { }

        // Returns length in bytes
        //
        virtual uint16_t length(void) const = 0;

        virtual void clear(void) = 0;
        virtual void print(const std::string &, std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;

        uint16_t read_length(byte_stream_t &stream);
    };

    // ------------------------------------------------------------------------
    struct sv_record_t
    {
        uint32_t                     type;               // 4 bytes
        sv_content_t                *content_ptr;        // Variable length

        sv_record_t(void) : type(0), content_ptr(0) { }
        ~sv_record_t(void) { clear(); }

        inline datum_ids_e datum_id_enum(void) const
        {
            return (vdis::datum_ids_e)type;
        }

        // Returns datum length in bytes
        //
        inline uint16_t length(void) const
        {
            return (content_ptr ? content_ptr->length() : 0);
        }

        inline void clear(void)
        {
            if (content_ptr)
            {
                delete content_ptr;
            }

            type = 0;
            content_ptr = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct default_sv_content_t : sv_content_t
    {
        byte_buffer_t               buffer;

        ~default_sv_content_t(void) { clear(); }

        // Returns datum length in bytes
        //
        inline uint16_t length(void) const { return buffer.length(); }

        inline void clear(void)
        {
            buffer.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct application_state_t : public sv_content_t
    {
      public:

        uint8_t                     padding;                // 1 byte
        uint8_t                     transition;             // 1 byte
        uint8_t                     current_state;          // 1 byte
        uint8_t                     requested_state;        // 1 byte
        uint8_t                     current_configuration;  // 1 byte
        uint8_t                     exercise_count;         // 1 byte
        exercise_state_t          **exercises;              // 4 bytes each

        ~application_state_t(void) { clear(); }

        // TODO: enumeration methods

        uint16_t length(void) const;
        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            BASE_LENGTH_BITS = 6;
    };
}

#endif
