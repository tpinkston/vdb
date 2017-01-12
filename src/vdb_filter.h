#ifndef VDB_FILTER_H
#define VDB_FILTER_H

#include "vdb_system.h"

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

      private:

        static bool set_contains(const std::set<uint8_t> &set, uint8_t value);
    };
}

#endif
