#ifndef VDIS_DATUM_RECORDS_H
#define VDIS_DATUM_RECORDS_H

#include <cstdint>

#include "vdis_byte_buffer.h"
#include "vdis_data_types.h"

namespace vdis
{
    class byte_stream_t;

    struct fixed_datum_t;
    struct variable_datum_t;

    // Returns array with length equal to 'count'
    //
    fixed_datum_t **get_fixed_datums(
        byte_stream_t &stream,
        uint32_t count
    );

    // Returns array with length equal to 'count'
    //
    variable_datum_t **get_variable_datums(
        byte_stream_t &stream,
        uint32_t count
    );

    // ------------------------------------------------------------------------
    struct fixed_datum_t
    {
        uint32_t                id;                     // 4 bytes
        uint32_t                value;                  // 4 bytes

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct datum_content_t
    {
        virtual ~datum_content_t(void) { }

        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;
    };

    // ------------------------------------------------------------------------
    struct variable_datum_t
    {
        uint32_t                id;                     // 4 bytes
        uint32_t                length;                 // 4 bytes
        datum_content_t        *content_ptr;            // N bytes (variable)

        ~variable_datum_t(void);

        void clear(void);

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    void swap(variable_datum_t &value, bool force = false);

    // ------------------------------------------------------------------------
    struct default_datum_content_t : datum_content_t
    {
        byte_buffer_t           buffer;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct damage_status_t : datum_content_t
    {
        uint16_t                casualties;             // 2 bytes
        uint8_t                 cause;                  // 1 byte
        uint8_t                 padding1;               // 1 byte
        event_id_t              event_id;               // 6 bytes
        uint16_t                extent;                 // 2 bytes
        uint32_t                padding2;               // 4 bytes

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct sling_line_t
    {
        float32_t               length;                 // 4 bytes
        float32_t               offset;                 // 4 bytes

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct sling_load_capability_t : datum_content_t
    {
        entity_id_t             payload;                // 6 bytes
        entity_id_t             carrier;                // 6 bytes
        float32_t               drag_coeffficient;      // 4 bytes
        float32_t               current_mass;           // 4 bytes
        uint16_t                padding;                // 2 bytes
        uint8_t                 hook_type;              // 1 byte
        uint8_t                 lines_needed;           // 1 byte
        sling_line_t           *lines;                  // N bytes (array)

        virtual ~sling_load_capability_t(void);

        void clear(void);

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct force_id_affiliation_t : datum_content_t
    {
        uint16_t                force;                  // 2 bytes
        uint16_t                padding;                // 2 bytes
        char                   *name;                   // N bytes (array)

        virtual ~force_id_affiliation_t(void);

        void clear(void);

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };
}

std::ostream &operator<<(std::ostream &, const vdis::fixed_datum_t &);
std::ostream &operator<<(std::ostream &, const vdis::variable_datum_t &);
std::ostream &operator<<(std::ostream &, const vdis::default_datum_content_t &);
std::ostream &operator<<(std::ostream &, const vdis::damage_status_t &);
std::ostream &operator<<(std::ostream &, const vdis::sling_load_capability_t &);
std::ostream &operator<<(std::ostream &, const vdis::force_id_affiliation_t &);

#endif
