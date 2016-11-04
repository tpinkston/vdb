#ifndef VDB_LIST_H
#define VDB_LIST_H

namespace vdb
{
    class output_file_t;
    class pdu_data_t;

    class list
    {
      public:

        static int list_pdus(void);

      protected:

        static bool process_pdu_data(const pdu_data_t &data);

        static output_file_t
            *file_ptr;
        static uint32_t
            pdus_listed,
            pdus_filtered_out;
    };
}

#endif
