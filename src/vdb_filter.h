#ifndef VDB_FILTER_H
#define VDB_FILTER_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    class filter
    {
      public:

        static bool filter_by_metadata(const pdu_data_t &data);
        static bool filter_by_header(const pdu_data_t &data);
        static bool filter_by_content(const vdis::pdu_t &pdu);
        static bool filter_by_range(uint32_t index, bool &past_end);

        static std::set<std::string>
            include_hostnames,
            exclude_hostnames;
        static std::set<vdis::id_t>
            include_entity_ids,
            exclude_entity_ids;
        static std::set<uint8_t>
            include_exercises,
            exclude_exercises,
            include_types,
            exclude_types,
            include_families,
            exclude_families;
        static std::set<uint32_t>
            pdu_index_range;

      private:

        static bool set_contains(const std::set<uint8_t> &set, uint8_t value);
    };
}

#endif
