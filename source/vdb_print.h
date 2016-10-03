#ifndef VDB_PRINT_H
#define VDB_PRINT_H

namespace vdis
{
    class pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    class print
    {
      public:

        static void print_pdu(
            const pdu_data_t &data,
            const vdis::pdu_t &pdu,
            std::ostream &out
        );

      private:

        static void print_data(
            const pdu_data_t &data,
            std::ostream &out
        );

        static void print_pdu_extracted(
            const pdu_data_t &data,
            const vdis::pdu_t &pdu,
            std::ostream &out
        );

        static void print_pdu_hex_dump(
            const pdu_data_t &data,
            std::ostream &out
        );

        static void print_pdu_normal(
            const pdu_data_t &data,
            const vdis::pdu_t &pdu,
            std::ostream &out
        );

        static void print_pdu_summary(
            const pdu_data_t &data,
            const vdis::pdu_t &pdu,
            std::ostream &out
        );
    };
}

#endif
