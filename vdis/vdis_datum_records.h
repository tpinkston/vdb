#ifndef VDIS_DATUM_RECORDS_H
#define VDIS_DATUM_RECORDS_H

#include "vdis_byte_buffer.h"
#include "vdis_data_types.h"

namespace vdis
{
    class byte_stream_t;

    struct fixed_datum_record_t;
    struct variable_datum_record_t;

    // Returns array with length equal to 'count'
    //
    fixed_datum_record_t **read_fixed_datum_records(
        byte_stream_t &stream,
        uint32_t count
    );

    // Returns array with length equal to 'count'
    //
    variable_datum_record_t **read_variable_datum_records(
        byte_stream_t &stream,
        uint32_t count
    );

    // ------------------------------------------------------------------------
    struct fixed_datum_record_t
    {
        uint32_t                    id;                     // 4 bytes
        uint32_t                    value;                  // 4 bytes

        inline datum_ids_e datum_id_enum(void) const
        {
            return (datum_ids_e)id;
        }

        inline void clear(void)
        {
            id = 0;
            value = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct variable_datum_content_t
    {
        virtual ~variable_datum_content_t(void) { }

        // Returns datum length in bits
        //
        virtual uint32_t length(void) const = 0;

        virtual void clear(void) = 0;
        virtual void print(const string_t &, std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;

        uint32_t read_length(byte_stream_t &stream);
    };

    // ------------------------------------------------------------------------
    struct variable_datum_record_t
    {
        uint32_t                    id;                     // 4 bytes
        variable_datum_content_t   *content_ptr;            // Variable length

        ~variable_datum_record_t(void);

        inline datum_ids_e datum_id_enum(void) const
        {
            return (datum_ids_e)id;
        }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const
        {
            return (content_ptr ? content_ptr->length() : 0);
        }

        inline void clear(void)
        {
            if (content_ptr)
            {
                delete content_ptr;
            }

            id = 0;
            content_ptr = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct default_variable_datum_content_t : variable_datum_content_t
    {
        byte_buffer_t               buffer;

        // Returns datum length in bits
        //
        inline uint32_t length(void) const
        {
            return (buffer.length() * 8);
        }

        inline void clear(void)
        {
            buffer.clear();
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct damage_status_t : variable_datum_content_t
    {
        uint16_t                    casualties;             // 2 bytes
        uint8_t                     cause;                  // 1 byte
        uint8_t                     padding1;               // 1 byte
        id_t                        event_id;               // 6 bytes
        uint16_t                    extent;                 // 2 bytes
        uint32_t                    padding2;               // 4 bytes

        inline damage_cause_e cause_enum(void) const
        {
            return (damage_cause_e)cause;
        }

        inline damage_extent_e extent_enum(void) const
        {
            return (damage_extent_e)extent;
        }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            casualties = 0;
            cause = 0;
            padding1 = 0;
            event_id.clear();
            extent = 0;
            padding2 = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS;
    };

    // ------------------------------------------------------------------------
    struct sling_load_capability_t : variable_datum_content_t
    {
        id_t                        payload;                // 6 bytes
        id_t                        carrier;                // 6 bytes
        float32_t                   drag_coeffficient;      // 4 bytes
        float32_t                   current_mass;           // 4 bytes
        uint16_t                    padding;                // 2 bytes
        uint8_t                     hook_type;              // 1 byte
        uint8_t                     lines_needed;           // 1 byte
        sling_line_t              **lines;                  // N bytes (array)

        virtual ~sling_load_capability_t(void)
        {
            clear();
        }

        inline hook_type_e hook_type_enum(void) const
        {
            return (hook_type_e)hook_type;
        }

        // Returns datum length in bits
        //
        uint32_t length(void) const;

        void clear(void);
        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            BASE_LENGTH_BITS = 192; // Bits
    };

    // ------------------------------------------------------------------------
    struct force_id_affiliation_t : variable_datum_content_t
    {
        uint16_t                    force;                  // 2 bytes
        uint16_t                    padding;                // 2 bytes
        byte_buffer_t               name;                   // N bytes (array)

        virtual ~force_id_affiliation_t(void)
        {
            clear();
        }

        inline force_id_e force_enum(void) const
        {
            return (force_id_e)force;
        }

        string_t force_name(void) const;

        // Returns datum length in bits
        //
        uint32_t length(void) const;

        void clear(void);
        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            BASE_LENGTH_BITS = 32; // Bits
    };
}

#endif
