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

#ifndef VDB_QUERY_H
#define VDB_QUERY_H

#include "vdb_command.h"
#include "vdb_common.h"
#include "vdb_enums.h"
#include "vdb_file_handler.h"
#include "vdb_ids.h"
#include "vdb_marking.h"
#include "vdb_entity_type.h"
#include "vdb_warfare_pdus.h"

namespace vdb
{
    class entity_state_pdu_t;
    class collision_pdu_t;
    class designator_pdu_t;
    class em_emission_pdu_t;
    class transmitter_pdu_t;
    class signal_pdu_t;
    class receiver_pdu_t;
    class iff_pdu_t;
    class point_object_state_pdu_t;
    class linear_object_state_pdu_t;
    class areal_object_state_pdu_t;

    // ------------------------------------------------------------------------
    class collision_node_t
    {
      public:

        collision_node_t(void);
        collision_node_t(const collision_node_t &copy);
        ~collision_node_t(void);

        void print(std::ostream &stream) const;

        id_t
            colliding_entity,
            event;
        collision_e
            type;
        velocity_t
            velocity;
        location12_t
            location;
    };

    // ------------------------------------------------------------------------
    class designator_node_t
    {
      public:

    	designator_node_t(void);
    	designator_node_t(const designator_node_t &copy);
        ~designator_node_t(void);

        bool matches(const designator_pdu_t &pdu) const;

        void print(std::ostream &stream) const;

        id_t
            target;
        uint16_t
            code,
            total_count;
        float32_t
            minimum_power,
            maximum_power,
            average_power;
        std::map<uint8_t, uint32_t>
            functions,
            spot_types,
            system_names;
    };

    // ------------------------------------------------------------------------
    class warfare_data_node_t
    {
      public:

        warfare_data_node_t(void);
        warfare_data_node_t(const warfare_data_node_t &copy);
        ~warfare_data_node_t(void);

        void print(std::ostream &stream) const;

        id_t
            shooter,
            target,
            munition,
            event;
        burst_descriptor_t
            burst;
        detonation_result_e
            result;
        uint64_t
            fire_time,
            detonation_time;
    };

    // ------------------------------------------------------------------------
    class entity_data_node_t
    {
      public:

        entity_data_node_t(void);
        entity_data_node_t(const entity_data_node_t &copy);
        ~entity_data_node_t(void);

        void print(std::ostream &stream) const;

        id_t
            id;
        entity_marking_t
            marking;
        force_id_e
            force_id;
        entity_type_t
            type;
        std::string
            source;
        std::vector<collision_node_t>
            collisions;
        std::vector<designator_node_t>
            designations;
        std::map<id_t, warfare_data_node_t>
            fires;
    };

    // ------------------------------------------------------------------------
    class object_data_node_t
    {
      public:

        object_data_node_t(void);
        object_data_node_t(const object_data_node_t &copy);
        ~object_data_node_t(void);

        void print(std::ostream &stream) const;

        force_id_e
            force_id;
        object_type_t
            type;
    };

    // ------------------------------------------------------------------------
    class source_data_node_t
    {
      public:

        source_data_node_t(void);
        source_data_node_t(const source_data_node_t &copy);
        ~source_data_node_t(void);

        void print(std::ostream &stream) const;

        uint32_t
            total_pdus,
            total_bytes;
        std::map<pdu_type_e, uint32_t>
            pdu_types;
        std::set<uint8_t>
            exercises;
    };

    // ------------------------------------------------------------------------
    class query : public file_handler, public command
    {
      public:

        query(void);
        virtual ~query(void);

        virtual int run(void);

        static bool print(option_e);

      protected:

        virtual void process_pdu_data(const pdu_data_t &);

        virtual void process_pdu(
            const pdu_data_t &,
            const pdu_t &,
            source_data_node_t &
        );

        virtual void process(const entity_state_pdu_t &);
        virtual void process(const pdu_data_t &, const fire_pdu_t &);
        virtual void process(const pdu_data_t &, const detonation_pdu_t &);
        virtual void process(const collision_pdu_t &);
        virtual void process(const designator_pdu_t &);
        virtual void process(const point_object_state_pdu_t &);
        virtual void process(const linear_object_state_pdu_t &);
        virtual void process(const areal_object_state_pdu_t &);

        virtual void print_results(std::ostream &);

        virtual void print_results(
            const source_data_node_t &,
            std::ostream &
        );

        std::string
            current_source;
        uint64_t
            first_pdu_time,
            last_pdu_time;
        source_data_node_t
            all_sources;
        std::map<std::string, source_data_node_t>
            source_data;
        std::map<id_t, entity_data_node_t>
            entity_data;
        std::map<id_t, object_data_node_t>
            object_data;
    };
}

// ----------------------------------------------------------------------------
inline vdb::collision_node_t::collision_node_t(void) :
    type(COLLISION_INELASTIC)
{

}

// ----------------------------------------------------------------------------
inline vdb::collision_node_t::collision_node_t(
    const vdb::collision_node_t::collision_node_t &copy
) :
    colliding_entity(copy.colliding_entity),
    event(copy.event),
    type(copy.type),
    velocity(copy.velocity),
    location(copy.location)
{

}

// ----------------------------------------------------------------------------
inline vdb::collision_node_t::~collision_node_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::designator_node_t::designator_node_t(void) :
    code(0),
    total_count(0),
    minimum_power(999999.0),
    maximum_power(0),
    average_power(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::designator_node_t::designator_node_t(
    const vdb::designator_node_t::designator_node_t &copy
) :
    target(copy.target),
    code(copy.code),
    total_count(copy.total_count),
    minimum_power(copy.minimum_power),
    maximum_power(copy.maximum_power),
    average_power(copy.average_power),
    functions(copy.functions),
    spot_types(copy.spot_types),
    system_names(copy.system_names)
{

}

// ----------------------------------------------------------------------------
inline vdb::designator_node_t::~designator_node_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::warfare_data_node_t::warfare_data_node_t(void) :
    result(DETONATION_RESULT_DET_RESULT_OTHER),
    fire_time(0),
    detonation_time(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::warfare_data_node_t::warfare_data_node_t(
    const warfare_data_node_t &copy
) :
    shooter(copy.shooter),
    target(copy.target),
    munition(copy.munition),
    event(copy.event),
    burst(copy.burst),
    result(copy.result),
    fire_time(copy.fire_time),
    detonation_time(copy.detonation_time)
{

}

// ----------------------------------------------------------------------------
inline vdb::warfare_data_node_t::~warfare_data_node_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::entity_data_node_t::entity_data_node_t(void) :
    force_id(FORCE_ID_OTHER)
{

}

// ----------------------------------------------------------------------------
inline vdb::entity_data_node_t::entity_data_node_t(
    const entity_data_node_t &copy
) :
    id(copy.id),
    marking(copy.marking),
    force_id(copy.force_id),
    type(copy.type),
    source(copy.source),
    collisions(copy.collisions),
    designations(copy.designations),
    fires(copy.fires)
{

}

// ----------------------------------------------------------------------------
inline vdb::entity_data_node_t::~entity_data_node_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::object_data_node_t::object_data_node_t(void) :
    force_id(vdb::FORCE_ID_OTHER)
{

}

// ----------------------------------------------------------------------------
inline vdb::object_data_node_t::object_data_node_t(
    const object_data_node_t &copy
) :
    force_id(copy.force_id),
    type(copy.type)
{

}

// ----------------------------------------------------------------------------
inline vdb::object_data_node_t::~object_data_node_t(void)
{

}

// ----------------------------------------------------------------------------
inline vdb::source_data_node_t::source_data_node_t(void) :
    total_pdus(0),
    total_bytes(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::source_data_node_t::source_data_node_t(
    const source_data_node_t &copy
) :
    total_pdus(copy.total_pdus),
    total_bytes(copy.total_bytes),
    pdu_types(copy.pdu_types),
    exercises(copy.exercises)
{

}

// ----------------------------------------------------------------------------
inline vdb::source_data_node_t::~source_data_node_t(void)
{

}

#endif
