#ifndef VDB_SCAN_H
#define VDB_SCAN_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct action_request_pdu_t;
    struct action_response_pdu_t;
    struct designator_pdu_t;
    struct detonation_pdu_t;
    struct entity_association_t;
    struct entity_state_pdu_t;
    struct fire_pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    class scan
    {
      public:

        static void scan_all(void);

        static bool parse(const std::string &value);

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &);

        static bool
            scanning,
            associations,
            lasers,
            fires,
            collisions,
            entities,
            objects;

      private:

        static void process_entity_state(
            const pdu_data_t &data,
            const vdis::entity_state_pdu_t &pdu
        );

        static void process_fire(
            const pdu_data_t &data,
            const vdis::fire_pdu_t &pdu
        );

        static void process_detonation(
            const pdu_data_t &data,
            const vdis::detonation_pdu_t &pdu
        );

        static void process_designator(
            const pdu_data_t &data,
            const vdis::designator_pdu_t &pdu
        );

        static void process_entity_associations(
            const pdu_data_t &data,
            const vdis::entity_state_pdu_t &pdu
        );

        static void process_request(
            const pdu_data_t &data,
            const vdis::action_request_pdu_t &pdu
        );

        static void process_response(
            const pdu_data_t &data,
            const vdis::action_response_pdu_t &pdu
        );

        static void process_carrier(
            const pdu_data_t &data,
            const vdis::entity_state_pdu_t &pdu,
            const vdis::entity_association_t &record
        );

        static void process_target(
            const pdu_data_t &data,
            const vdis::entity_state_pdu_t &pdu,
            const vdis::entity_association_t &record
        );

        static void get_targets(
            const vdis::id_t &carrier_id,
            std::set<vdis::id_t> &target_ids
        );

        static void print_heading(const pdu_data_t &data);

        // --------------------------------------------------------------------
        struct entity_t
        {
            vdis::entity_capabilities_t     capabilities;
            uint32_t                        appearance;
        };

        // --------------------------------------------------------------------
        struct laser_t
        {
            vdis::id_t                      target;
            uint16_t                        code;
            float32_t                       power;
            vdis::desig_spot_type_e         spot_type;
            vdis::desig_system_name_e       system_name;
            vdis::laser_function_e          function;

            inline bool operator==(const laser_t &other) const
            {
                if ((target == other.target) and
                    (code == other.code) and
                    (spot_type == other.spot_type) and
                    (system_name == other.system_name) and
                    (function == other.function))
                {
                    return true;
                }

                return false;
            }

            inline bool operator!=(const laser_t &other) const
            {
                return not (*this == other);
            }
        };

        typedef std::map<vdis::id_t, entity_t>
            entities_t;
        typedef std::map<uint8_t, laser_t>
            laser_systems_t;
        typedef std::map<vdis::id_t, laser_systems_t>
            lasers_t;

        // Maps target ID to association node.
        //
        typedef std::map<vdis::id_t, vdis::entity_association_t>
            targets_t;

        // Maps carrier ID to associated targets.
        //
        typedef std::map<vdis::id_t, targets_t>
            carriers_t;

        typedef std::set<uint32_t>
            request_ids_t;

        // Maps originator ID to set of request IDs.
        //
        typedef std::map<vdis::id_t, request_ids_t>
            entity_requests_t;

        static entities_t                   current_entities;
        static lasers_t                     current_lasers;
        static carriers_t                   current_carriers;
        static targets_t                    current_targets;
        static entity_requests_t            current_requests;
        static std::ostream                &err;
        static std::ostream                &out;
    };
}

#endif
