#ifndef VDB_ASSOCIATIONS_H
#define VDB_ASSOCIATIONS_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct entity_state_pdu_t;
    struct action_request_pdu_t;
    struct action_response_pdu_t;
    struct entity_association_t;
}

namespace vdb
{
    class pdu_data_t;

    // ------------------------------------------------------------------------
    class associations
    {
      public:

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &);

      private:

        typedef std::map<vdis::id_t, vdis::entity_association_t>
            targets_t;
        typedef std::map<vdis::id_t, targets_t>
            carriers_t;
        typedef std::set<uint32_t>
            request_ids_t;
        typedef std::map<vdis::id_t, request_ids_t>
            entity_requests_t;

        static void process_entity_state(
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

        static void print_data(const pdu_data_t &data);

        // Maps carrier ID to associated targets.
        //
        static carriers_t           current_carriers;

        // Maps target ID to association node.
        //
        static targets_t            current_targets;

        // Maps originator ID to set of request IDs.
        //
        static entity_requests_t    current_requests;

        static std::ostream        &err;
        static std::ostream        &out;
    };
}

#endif
