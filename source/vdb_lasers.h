#ifndef VDB_LASERS_H
#define VDB_LASERS_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct designator_pdu_t;
}

namespace vdb
{
    struct pdu_data_t;

    // ------------------------------------------------------------------------
    class lasers
    {
      public:

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &);

      private:

        static void process_designator(
            const pdu_data_t &data,
            const vdis::designator_pdu_t &pdu
        );
    };
}

#endif
