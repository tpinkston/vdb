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

#ifndef VDB_MINEFIELD_PDUS_H
#define VDB_MINEFIELD_PDUS_H

#include "vdb_datum.h"
#include "vdb_ids.h"
#include "vdb_pdu.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Minefield State PDU
    //
    class minefield_state_pdu_t : public pdu_content_t
    {
      public:

        minefield_state_pdu_t(void);
        virtual ~minefield_state_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // TODO: minefield_state_pdu_t content

        static const uint32_t
            BASE_SIZE;
    };

    // ------------------------------------------------------------------------
    // Minefield Query PDU
    //
    class minefield_query_pdu_t : public pdu_content_t
    {
      public:

        minefield_query_pdu_t(void);
        virtual ~minefield_query_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // TODO: minefield_query_pdu_t content

        static const uint32_t
            BASE_SIZE;
    };

    // ------------------------------------------------------------------------
    // Minefield Data PDU
    //
    class minefield_data_pdu_t : public pdu_content_t
    {
      public:

        minefield_data_pdu_t(void);
        virtual ~minefield_data_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // TODO: minefield_data_pdu_t content

        static const uint32_t
            BASE_SIZE;
    };

    // ------------------------------------------------------------------------
    // Minefield Response NACK PDU
    //
    class minefield_response_nack_pdu_t : public pdu_content_t
    {
      public:

        minefield_response_nack_pdu_t(void);
        virtual ~minefield_response_nack_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // TODO: minefield_response_nack_pdu_t content

        static const uint32_t
            BASE_SIZE;
    };
}


#endif
