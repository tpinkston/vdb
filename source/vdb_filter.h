#ifndef VDB_FILTER_H
#define VDB_FILTER_H

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
    };
}

#endif
