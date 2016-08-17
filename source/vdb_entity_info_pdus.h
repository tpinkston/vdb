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

#ifndef VDB_ENTITY_INFO_PDUS_H
#define VDB_ENTITY_INFO_PDUS_H

#include "vdb_entity_type.h"
#include "vdb_marking.h"
#include "vdb_pdu.h"
#include "vdb_spatial.h"
#include "vdb_variable_parameters.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Entity State PDU.
    //
    class entity_state_pdu_t : public pdu_content_t
    {
      public:

        entity_state_pdu_t(void);
        virtual ~entity_state_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity_id;
        uint8_t
            force_id;
        entity_type_t
            entity_type,
            alternative_entity_type;
        velocity_t
            velocity;
        location24_t
            location;
        orientation_t
            orientation;
        uint32_t
            appearance;
        dead_reckoning_t
            dead_reckoning;
        entity_marking_t
            marking;
        uint32_t
            capabilities;
        variable_parameter_records_t
            variable_parameters;

        static const uint32_t
            BASE_LENGTH = 132; // bytes, minus header

      private:

        void print_capabilities(
            const std::string &prefix,
            std::ostream &stream
        ) const;
    };

    // ------------------------------------------------------------------------
    // Entity State Update PDU.
    //
    class entity_state_update_pdu_t : public pdu_content_t
    {
      public:

        entity_state_update_pdu_t(void);
        virtual ~entity_state_update_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity_id;
        uint8_t
            padding;
        velocity_t
            velocity;
        location24_t
            location;
        orientation_t
            orientation;
        uint32_t
            appearance;
        variable_parameter_records_t
            variable_parameters;

        static const uint32_t
            BASE_LENGTH = 60; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Collision PDU.
    //
    class collision_pdu_t : public pdu_content_t
    {
      public:

        collision_pdu_t(void);
        virtual ~collision_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            issuing_entity_id,
            colliding_entity_id,
            event_id;
        uint8_t
            collision_type,
            padding;
        velocity_t
            velocity;
        float32_t
            mass;
        location12_t
            location;

        static const uint32_t
            LENGTH = 48; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Utility class for printing out the values in the 32-bit appearance
    // field on entity state PDUs.
    //
    class entity_state_appearance
    {
      public:

        static void print(
            const entity_state_pdu_t &pdu,
            const std::string &prefix,
            std::ostream &stream
        );

      private:

        static void print_lifeform(
            const entity_state_pdu_t &pdu,
            const std::string &prefix,
            std::ostream &stream
        );

        static void print_air_platform(
            const entity_state_pdu_t &pdu,
            const std::string &prefix,
            std::ostream &stream
        );

        static void print_land_platform(
            const entity_state_pdu_t &pdu,
            const std::string &prefix,
            std::ostream &stream
        );
    };
}

#endif
