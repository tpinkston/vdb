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

#ifndef VDB_DATUM_H
#define VDB_DATUM_H

#include "vdb_data_buffer.h"
#include "vdb_ids.h"
#include "vdb_object.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Encapsulates a single fixed datum record.
    //
    class fixed_datum_t : public record_t
    {
      public:

        fixed_datum_t(void);
        fixed_datum_t(const fixed_datum_t &copy);
        virtual ~fixed_datum_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint32_t
            datum_id,
            datum_value;

        static const uint32_t
            LENGTH = 8; // bytes
    };

    // ------------------------------------------------------------------------
    class variable_datum_content_t : public record_t
    {
      public:

        variable_datum_content_t(void) { }
        variable_datum_content_t(const variable_datum_content_t &copy) { }
        virtual ~variable_datum_content_t(void) { }

        virtual uint32_t padding_length(void) const;
        virtual uint32_t read_length(byte_stream &stream);
    };

    // ------------------------------------------------------------------------
    // Encapsulates a single variable datum record.
    //
    class variable_datum_t : public record_t
    {
      public:

        variable_datum_t(void);
        variable_datum_t(const variable_datum_t &copy);
        virtual ~variable_datum_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        // Returns total length of the entire variable datum record.
        //
        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint32_t
            datum_id;
        variable_datum_content_t
            *datum_content_ptr;

        static const uint32_t
            BASE_LENGTH = 8; // bytes
    };

    // ------------------------------------------------------------------------
    // Encapsulates a collection of fixed datum records.
    //
    class fixed_datum_records_t : public record_t
    {
      public:

        fixed_datum_records_t(void);
        fixed_datum_records_t(const fixed_datum_records_t &copy);
        ~fixed_datum_records_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void read(byte_stream &stream, uint32_t);
        virtual void write(byte_stream &stream) const;

        std::vector<fixed_datum_t>
            records;
    };

    // ------------------------------------------------------------------------
    // Encapsulates a collection of variable datum records.
    //
    class variable_datum_records_t : public record_t
    {
      public:

        variable_datum_records_t(void);
        variable_datum_records_t(const variable_datum_records_t &copy);
        ~variable_datum_records_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void read(byte_stream &stream, uint32_t);
        virtual void write(byte_stream &stream) const;

        std::vector<variable_datum_t>
            records;
    };

    // ------------------------------------------------------------------------
    class default_datum_content_t : public variable_datum_content_t
    {
      public:

        default_datum_content_t(void);
        default_datum_content_t(const default_datum_content_t &copy);
        virtual ~default_datum_content_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        data_buffer_t
            data;
    };

    // ------------------------------------------------------------------------
    class damage_status_t : public variable_datum_content_t
    {
      public:

        damage_status_t(void);
        damage_status_t(const damage_status_t &copy);
        virtual ~damage_status_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint16_t
            casualties;
        uint8_t
            cause,
            padding8;
        id_t
            event_id;
        uint16_t
            extent;
        uint32_t
            padding32;

        static const uint32_t
            LENGTH = 16;
    };

    // ------------------------------------------------------------------------
    class sling_load_capability_t : public variable_datum_content_t
    {
      public:

        sling_load_capability_t(void);
        sling_load_capability_t(const sling_load_capability_t &copy);
        virtual ~sling_load_capability_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // first: length
        // second: offset
        //
        typedef std::pair<float32_t, float32_t>
            sling_line_t;

        id_t
            payload,
            carrier;
        float32_t
            drag_coeffficient,
            current_mass;
        uint16_t
            padding;
        uint8_t
            hook_type;
        std::vector<sling_line_t>
            lines;

        static const uint32_t
            BASE_LENGTH = 24;
    };

    // ------------------------------------------------------------------------
    class force_id_affiliation_t : public variable_datum_content_t
    {
      public:

        force_id_affiliation_t(void);
        force_id_affiliation_t(const force_id_affiliation_t &copy);
        virtual ~force_id_affiliation_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint16_t
            force,
            padding;
        std::string
            name;

        static const uint32_t
            BASE_LENGTH = 4;
    };
}

#endif
