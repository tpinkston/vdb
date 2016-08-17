// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

#ifndef VDB_PRINT_H
#define VDB_PRINT_H

namespace vdb
{
    class pdu_t;
    class pdu_data_t;

    class print
    {
      public:

        static void print_pdu(
            const pdu_data_t &data,
            const pdu_t &pdu,
            std::ostream &stream
        );

      private:

        static void print_data(
            const pdu_data_t &data,
            std::ostream &stream
        );

        static void print_pdu_extracted(
            const pdu_data_t &data,
            const pdu_t &pdu,
            std::ostream &stream
        );

        static void print_pdu_hex_dump(
            const pdu_data_t &data,
            std::ostream &stream
        );

        static void print_pdu_normal(
            const pdu_data_t &data,
            const pdu_t &pdu,
            std::ostream &stream
        );
    };
}

#endif
