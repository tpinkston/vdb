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

#ifndef VDB_PDU_H
#define VDB_PDU_H

#include "vdb_data_buffer.h"
#include "vdb_object.h"
#include "vdb_time.h"

namespace vdb
{
    class id_t;

    // ------------------------------------------------------------------------
    class pdu_content_t : public record_t
    {
      public:

        pdu_content_t(void) { }
        virtual ~pdu_content_t(void) { }

        // Returns the request ID if available (SIMAN).
        //
        virtual const uint32_t *get_request_id(void) const { return 0; }

        // Returns the initiating entity if available.
        //
        virtual const id_t *get_initator(void) const { return 0; }

        // Returns true if PDU makes any reference to the given ID.
        //
        virtual bool contains(const id_t *id_ptr) const { return false; }

        virtual void print_summary(std::ostream &stream) const = 0;
    };

    // ------------------------------------------------------------------------
    class pdu_t : public record_t
    {
      public:

        pdu_t(void);
        virtual ~pdu_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // PDU header attributes (1st twelve bytes of all PDUs):
        //
        uint8_t
            header_protocol,
            header_exercise,
            header_type,
            header_family;
        timestamp_t
            header_timestamp;
        mutable uint16_t
            header_length;
        uint8_t
            header_status,
            header_padding;

        // PDU content (all bytes after the header bytes):
        //
        pdu_content_t
            *content_ptr;

        static const uint32_t
            HEADER_LENGTH = 0x000C; // 12 bytes
    };

    // ------------------------------------------------------------------------
    // Used when no specific class extension is available.
    //
    class default_pdu_t : public pdu_content_t
    {
      public:

        default_pdu_t(uint32_t length);
        virtual ~default_pdu_t(void);

        virtual void clear(void);

        virtual void print_summary(std::ostream &stream) const;

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return data.length(); }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        data_buffer_t
           data;
    };
}

#endif
