#ifndef VDB_LIST_H
#define VDB_LIST_H

namespace vdb
{
    class file_writer_t;
    class pdu_data_t;

    // Lists PDUs in a capture file and optionally outputs PDUs to a separate
    // capture file which is useful when filtering.
    //
    class list
    {
      public:

        // Returns 0 on success, 1 on error
        //
        static int list_pdus(void);

      protected:

        static bool process_pdu_data(const pdu_data_t &data);

        static file_writer_t
            *file_ptr;
        static uint32_t
            output_index,
            pdus_listed,
            pdus_filtered_out;
    };
}

#endif
