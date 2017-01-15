#ifndef VDB_OBJECTS_H
#define VDB_OBJECTS_H

#include "vdb_system.h"

namespace vdis
{
    struct object_type_t;
}

namespace vdb
{
    // ------------------------------------------------------------------------
    class objects_t
    {
      public:

        objects_t(void) { }
        ~objects_t(void) { }

        int run(void);

        std::set<uint32_t>
            geometries,
            domains,
            kinds;

      private:

        bool list_type(const vdis::object_type_t &type);
    };
}

#endif
