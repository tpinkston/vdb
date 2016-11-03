#ifndef VDB_ENTITIES_H
#define VDB_ENTITIES_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct entity_state_pdu_t;
}

namespace vdb
{
    struct pdu_data_t;

    // ------------------------------------------------------------------------
    class entities
    {
      public:

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &);

      private:

        static void process_entity_state(
            const pdu_data_t &data,
            const vdis::entity_state_pdu_t &pdu
        );

        static void print_data(const pdu_data_t &data);

        struct entity_t
        {
            vdis::entity_capabilities_t     capabilities;
            uint32_t                        appearance;
        };

        typedef std::map<vdis::id_t, entity_t>
            entities_t;

        static entities_t                   current_entities;

        static std::ostream                &err;
        static std::ostream                &out;
    };
}

#endif
