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

#ifndef VDB_EXPERIMENTAL_PDUS_H
#define VDB_EXPERIMENTAL_PDUS_H

#include "vdb_ids.h"
#include "vdb_pdu.h"
#include "vdb_standard_variable_record.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Application Control PDU
    //
    class application_control_pdu_t : public pdu_content_t
    {
      public:

        application_control_pdu_t(void);
        application_control_pdu_t(const application_control_pdu_t &copy);
        virtual ~application_control_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // Abstract values specified by child classes.
        //
        id_t
            originator,
            recipient;
        uint8_t
            reliability_service,
            time_interval,
            control_type,
            padding;
        uint16_t
            originating_application_type,
            receiving_application_type;
        uint32_t
            request_id;
        uint8_t
            total_parts,
            current_part;
        standard_variable_records_t
            records;

        // 40 bytes minus the the header and the 16-bit "number of records"
        // field contained within the 'standard_variable_records_t' class.
        //
        static const uint32_t
            BASE_LENGTH = 26;
    };

    // ------------------------------------------------------------------------
    inline uint32_t application_control_pdu_t::length(void) const
    {
        return (BASE_LENGTH + records.length());
    }
}

#endif
