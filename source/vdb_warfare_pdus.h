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

#ifndef VDB_WARFARE_PDUS_H
#define VDB_WARFARE_PDUS_H

#include "vdb_datum.h"
#include "vdb_pdu.h"
#include "vdb_spatial.h"
#include "vdb_entity_type.h"
#include "vdb_standard_variable_record.h"
#include "vdb_variable_parameters.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class burst_descriptor_t : public record_t
    {
      public:

        burst_descriptor_t(void);
        burst_descriptor_t(const burst_descriptor_t &copy);
        virtual ~burst_descriptor_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        entity_type_t
            munition;
        uint16_t
            warhead,
            fuse,
            quantity,
            rate;

        static const uint32_t
            LENGTH = 16; // bytes
    };

    // ------------------------------------------------------------------------
    // Fire PDU
    //
    class fire_pdu_t : public pdu_content_t
    {
      public:

        fire_pdu_t(void);
        virtual ~fire_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            shooter,
            target,
            munition,
            event;
        uint32_t
            fire_mission_index;
        location24_t
            world_location;
        burst_descriptor_t
            burst_descriptor;
        velocity_t
            velocity;
        float32_t
            range;

        static const uint32_t
            LENGTH = 84; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Detonation PDU
    //
    class detonation_pdu_t : public pdu_content_t
    {
      public:

        detonation_pdu_t(void);
        virtual ~detonation_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            shooter,
            target,
            munition,
            event;
        velocity_t
            velocity;
        location24_t
            world_location;
        burst_descriptor_t
            burst_descriptor;
        location12_t
            entity_location;
        uint8_t
            detonation_result;
        uint16_t
            padding;
        variable_parameter_records_t
            variable_parameters;

        static const uint32_t
            BASE_LENGTH = 92; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Entity Damage Status PDU.
    //
    class entity_damage_status_pdu_t : public pdu_content_t
    {
      public:

        entity_damage_status_pdu_t(void);
        virtual ~entity_damage_status_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity;
        uint32_t
            padding;
        standard_variable_records_t
            description_records;

        static const uint32_t
            BASE_LENGTH = 12; // bytes, minus header
    };
}

#endif
