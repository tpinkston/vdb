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

#ifndef VDB_ENVIRONMENT_PDUS_H
#define VDB_ENVIRONMENT_PDUS_H

#include "vdb_data_buffer.h"
#include "vdb_datum.h"
#include "vdb_entity_type.h"
#include "vdb_ids.h"
#include "vdb_object_type.h"
#include "vdb_pdu.h"
#include "vdb_spatial.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class environment_record_t : public record_t
    {
      public:

        environment_record_t(void);
        environment_record_t(const environment_record_t &copy);
        virtual ~environment_record_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;
        virtual uint32_t padding_length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint32_t
            type;
        uint8_t
            index,
            padding;
        data_buffer_t
            data;

        static const uint32_t
            BASE_LENGTH = 8; // bytes
    };

    // ------------------------------------------------------------------------
    class linear_segment_t : public record_t
    {
      public:

        linear_segment_t(void);
        linear_segment_t(const linear_segment_t &copy);
        virtual ~linear_segment_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            number,
            modifications;
        uint16_t
            generic_appearance;
        uint32_t
            specific_appearance;
        location24_t
            location;
        orientation_t
            orientation;

        // In the latest standard these are represented as 32-bit floating
        // point numbers, not 16-bit unsigned integers.
        //
        uint16_t
            segment_length,
            segment_width,
            segment_height,
            segment_depth;
        uint32_t
            padding;

        static const uint32_t
            LENGTH = 56; // bytes
    };

    // ------------------------------------------------------------------------
    // Environmental Process PDU
    //
    class environmental_process_pdu_t : public pdu_content_t
    {
      public:

        environmental_process_pdu_t(void);
        virtual ~environmental_process_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            process_id;
        entity_type_t
            environment_type;
        uint8_t
            model_type,
            status;
        uint16_t
            sequence_number;
        std::vector<environment_record_t>
            records;

        static const uint32_t
            BASE_LENGTH = 20; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Point Object State PDU
    //
    class point_object_state_pdu_t : public pdu_content_t
    {
      public:

        point_object_state_pdu_t(void);
        virtual ~point_object_state_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            object_id,
            referenced_object_id;
        uint16_t
            update;
        uint8_t
            force_id,
            modifications;
        object_type_t
            object_type;
        location24_t
            location;
        orientation_t
            orientation;
        uint32_t
            specific_appearance;
        uint16_t
            generic_appearance,
            padding16;
        simulation_id_t
            requestor_id,
            receiver_id;
        uint32_t
            padding32;

        static const uint32_t
            LENGTH = 76; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Linear Object State PDU
    //
    class linear_object_state_pdu_t : public pdu_content_t
    {
      public:

        linear_object_state_pdu_t(void);
        virtual ~linear_object_state_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            object_id,
            referenced_object_id;
        uint16_t
            update;
        uint8_t
            force_id;
        simulation_id_t
            requestor_id,
            receiver_id;
        object_type_t
            object_type;
        std::vector<linear_segment_t>
            segments;

        static const uint32_t
            BASE_LENGTH = 28; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Areal Object State PDU
    //
    class areal_object_state_pdu_t : public pdu_content_t
    {
      public:

        areal_object_state_pdu_t(void);
        virtual ~areal_object_state_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            object_id,
            referenced_object_id;
        uint16_t
            update;
        uint8_t
            force_id,
            modifications;
        object_type_t
            object_type;
        uint32_t
            specific_appearance;
        uint16_t
            generic_appearance;
        simulation_id_t
            requestor_id,
            receiver_id;
        std::vector<location24_t>
            points;

        static const uint32_t
            BASE_LENGTH = 36; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Utility class for printing out the values in the appearance fields on
    // object state PDUs.
    //
    class object_state_appearance
    {
      public:

        static void print_generic(
            const std::string &prefix,
            uint16_t value,
            std::ostream &stream);

        static void print_specific(
            const std::string &prefix,
            const object_type_t &type,
            uint32_t value,
            std::ostream &stream);

      private:

    };
}

#endif
