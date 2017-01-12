#ifndef VDB_FIRES_H
#define VDB_FIRES_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct fire_pdu_t;
    struct detonation_pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    // ------------------------------------------------------------------------
    class fires
    {
      public:

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &);

      private:

        static void process_fire(
            const pdu_data_t &data,
            const vdis::fire_pdu_t &pdu
        );

        static void process_detonation(
            const pdu_data_t &data,
            const vdis::detonation_pdu_t &pdu
        );

        static void print_data(const pdu_data_t &data);

        static std::ostream                &err;
        static std::ostream                &out;
    };
}

#endif
