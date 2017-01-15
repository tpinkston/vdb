#ifndef VDB_SCAN_H
#define VDB_SCAN_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct entity_state_pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    // ------------------------------------------------------------------------
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

        static void print_heading(const pdu_data_t &data);

        static void process_entity_state(
            const pdu_data_t &data,
            const vdis::entity_state_pdu_t &pdu
        );

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
