#ifndef VDB_ENTITIES_H
#define VDB_ENTITIES_H

#include "vdb_command.h"

namespace vdis
{
    struct entity_type_t;
}

namespace vdb
{
    // ------------------------------------------------------------------------
    class entities_t : public command_t
    {
      public:

        entities_t(
            const std::string &name,
            const std::vector<std::string> &arguments
        );

        virtual ~entities_t(void);

        virtual int run(void);

        virtual bool option_callback(
            const option_t &option,
            const std::string &value,
            bool &success
        );

      protected:

        bool list_type(const vdis::entity_type_t &type);

        std::set<uint32_t>
            kinds,
            domains,
            countries;
        bool
            print_countries;
    };
}

#endif
