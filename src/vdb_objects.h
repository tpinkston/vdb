#ifndef VDB_OBJECTS_H
#define VDB_OBJECTS_H

#include "vdb_command.h"

namespace vdis
{
    struct object_type_t;
}

namespace vdb
{
    // ------------------------------------------------------------------------
    class objects_t : public command_t
    {
      public:

        objects_t(
            const std::string &name,
            const std::vector<std::string> &arguments
        );

        virtual ~objects_t(void);

        virtual int run(void);

        virtual bool option_callback(
            const option_t &option,
            const std::string &value,
            bool &success
        );

      protected:

        virtual bool list_type(const vdis::object_type_t &type);

        std::set<uint32_t>
            geometries,
            domains,
            kinds;
    };
}

#endif
