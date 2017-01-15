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

        static bool
            scanning,
            associations,
            lasers,
            fires,
            collisions,
            entities,
            objects;
        static std::ostream
            &err,
            &out;
    };
}

#endif
