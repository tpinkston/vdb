#ifndef VDB_ENUMS_H
#define VDB_ENUMS_H

#include "vdb_command.h"

namespace vdb
{
    class enums_t : public command_t
    {
      public:

        enums_t(
            const std::string &name,
            const std::vector<std::string> &arguments
        );

        virtual ~enums_t(void);

        virtual int run(void);
    };
}

#endif
