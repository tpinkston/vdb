#ifndef VDB_ENTITIES_H
#define VDB_ENTITIES_H

#include "vdb_system.h"

namespace vdis
{
    struct entity_type_t;
}

namespace vdb
{
    // ------------------------------------------------------------------------
    class entities_t
    {
      public:

        entities_t(void) : print_countries(false) { }
        ~entities_t(void) { }

        int run(void);

        bool
            print_countries;
        std::set<uint32_t>
            kinds,
            domains,
            countries;

      private:

        bool list_type(const vdis::entity_type_t &type);
    };
}

#endif
