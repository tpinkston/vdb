#ifndef VDB_SUMMARY_H
#define VDB_SUMMARY_H

#include "vdb_options.h"
#include "vdb_system.h"

#include "vdis_pdus.h"

namespace vdb
{
    class file_reader_t;

    // ------------------------------------------------------------------------
    class collision_node_t
    {
      public:

        collision_node_t(void);
        collision_node_t(const collision_node_t &copy);
        ~collision_node_t(void);

        void print(std::ostream &out) const;

        vdis::id_t
            colliding_entity,
            event;
        vdis::collision_e
            type;
        vdis::velocity_t
            velocity;
        vdis::location12_t
            location;
    };

    // ------------------------------------------------------------------------
    class designator_node_t
    {
      public:

    	designator_node_t(void);
    	designator_node_t(const designator_node_t &copy);
        ~designator_node_t(void);

        bool matches(const vdis::designator_pdu_t &pdu) const;

        void print(std::ostream &out) const;

        vdis::id_t
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

        void print(std::ostream &out) const;

        vdis::id_t
            shooter,
            target,
            munition,
            event;
        vdis::burst_descriptor_t
            burst;
        vdis::detonation_result_e
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

        void print(std::ostream &out) const;

        vdis::id_t
            id;
        vdis::marking_t
            marking;
        vdis::force_id_e
            force_id;
        vdis::entity_type_t
            type;
        std::string
            source;
        std::vector<collision_node_t>
            collisions;
        std::vector<designator_node_t>
            designations;
        std::map<vdis::id_t, warfare_data_node_t>
            fires;
    };

    // ------------------------------------------------------------------------
    class object_data_node_t
    {
      public:

        object_data_node_t(void);
        object_data_node_t(const object_data_node_t &copy);
        ~object_data_node_t(void);

        void print(std::ostream &out) const;

        vdis::force_id_e
            force_id;
        vdis::object_geometry_e
            geometry;
        vdis::object_type_t
            type;
    };

    // ------------------------------------------------------------------------
    class source_data_node_t
    {
      public:

        source_data_node_t(void);
        source_data_node_t(const source_data_node_t &copy);
        ~source_data_node_t(void);

        void print(std::ostream &out) const;

        uint32_t
            total_pdus,
            total_bytes;
        std::map<vdis::pdu_type_e, uint32_t>
            pdu_types;
        std::set<uint8_t>
            exercises;
    };

    // ------------------------------------------------------------------------
    class summary
    {
      public:

        static int summarize_pdus(void);

      protected:

        static bool process_pdu_data(const pdu_data_t &);
        static bool process_pdu_data(const pdu_data_t &, const vdis::pdu_t &);

        static void process_pdu(
            const pdu_data_t &,
            const vdis::pdu_t &,
            source_data_node_t &
        );

        static void process(const vdis::entity_state_pdu_t &);
        static void process(const pdu_data_t &, const vdis::fire_pdu_t &);
        static void process(const pdu_data_t &, const vdis::detonation_pdu_t &);
        static void process(const vdis::collision_pdu_t &);
        static void process(const vdis::designator_pdu_t &);
        static void process(const vdis::point_object_state_pdu_t &);
        static void process(const vdis::linear_object_state_pdu_t &);
        static void process(const vdis::areal_object_state_pdu_t &);

        static void print_results(std::ostream &);

        static void print_results(
            const source_data_node_t &,
            std::ostream &
        );

        static file_reader_t
            *reader_ptr;
        static std::string
            filename,
            current_source;
        static uint64_t
            first_pdu_time,
            last_pdu_time;
        static source_data_node_t
            all_sources;
        static std::map<std::string, source_data_node_t>
            source_data;
        static std::map<vdis::id_t, entity_data_node_t>
            entity_data;
        static std::map<vdis::id_t, object_data_node_t>
            object_data;
    };
}

// ----------------------------------------------------------------------------
inline vdb::collision_node_t::collision_node_t(void) :
    type(vdis::COLLISION_INELASTIC)
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
    result(vdis::DETONATION_RESULT_DET_RESULT_OTHER),
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
    force_id(vdis::FORCE_ID_OTHER)
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
    force_id(vdis::FORCE_ID_OTHER),
    geometry(vdis::OBJECT_GEOMETRY_UNKNOWN)
{

}

// ----------------------------------------------------------------------------
inline vdb::object_data_node_t::object_data_node_t(
    const object_data_node_t &copy
) :
    force_id(copy.force_id),
    geometry(copy.geometry),
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
