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

#ifndef VDB_SIMAN_PDUS_H
#define VDB_SIMAN_PDUS_H

#include "vdb_datum.h"
#include "vdb_ids.h"
#include "vdb_pdu.h"
#include "vdb_time.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    // Acknowledge PDU
    //
    class acknowledge_pdu_t : public pdu_content_t
    {
      public:

        acknowledge_pdu_t(void);
        virtual ~acknowledge_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        uint16_t
            acknowledge_flag,
            response_flag;
        uint32_t
            request_id;

        static const uint32_t
            LENGTH = 28; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Create Entity PDU
    //
    class create_entity_pdu_t : public pdu_content_t
    {
      public:

        create_entity_pdu_t(void);
        virtual ~create_entity_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        uint32_t
            request_id;

        static const uint32_t
            LENGTH = 16; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Remove Entity PDU
    //
    class remove_entity_pdu_t : public pdu_content_t
    {
      public:

        remove_entity_pdu_t(void);
        virtual ~remove_entity_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        uint32_t
            request_id;

        static const uint32_t
            LENGTH = 16; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Start/Resume PDU
    //
    class start_resume_pdu_t : public pdu_content_t
    {
      public:

        start_resume_pdu_t(void);
        virtual ~start_resume_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        clocktime_t
            real_time,
            simulation_time;
        uint32_t
            request_id;

        static const uint32_t
            LENGTH = 32; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Stop/Freeze PDU
    //
    class stop_freeze_pdu_t : public pdu_content_t
    {
      public:

        stop_freeze_pdu_t(void);
        virtual ~stop_freeze_pdu_t(void);

        virtual const uint32_t *get_request_id(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        clocktime_t
            real_time;
        uint8_t
            reason,
            behavior;
        uint16_t
            padding;
        uint32_t
            request_id;

        static const uint32_t
            LENGTH = 28; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    class comment_pdu_t : public pdu_content_t
    {
      public:

        comment_pdu_t(void);
        virtual ~comment_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        fixed_datum_records_t
            fixed_records;
        variable_datum_records_t
            variable_records;

        static const uint32_t
            BASE_LENGTH = 20; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Abstract parent class for SIMAN PDUs that contain fixed and variable
    // records, PDU types are:
    //
    // PDU_TYPE_ACTION_REQUEST = 16
    // PDU_TYPE_ACTION_RESPONSE = 17
    // PDU_TYPE_DATA_QUERY = 18
    // PDU_TYPE_SET_DATA = 19
    // PDU_TYPE_DATA = 20
    // PDU_TYPE_EVENT_REPORT = 21
    //
    class abstract_siman_pdu_t : public pdu_content_t
    {
      public:

        abstract_siman_pdu_t(void);
        virtual ~abstract_siman_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        // Sets length in PDU header, datum record counts in
        // preparation for saving to serialize_t buffer.
        //
        virtual void set_lengths(void);

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const = 0;

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            originator,
            recipient;
        uint32_t
            value_1,
            value_2;
        fixed_datum_records_t
            fixed_records;
        variable_datum_records_t
            variable_records;

        static const uint32_t
            BASE_LENGTH = 28; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Action Request PDU
    //
    //  value_1 = request_id
    //  value_2 = action_id
    //
    class action_request_pdu_t : public abstract_siman_pdu_t
    {
      public:

        action_request_pdu_t(void) { }
        virtual ~action_request_pdu_t(void) { }

        virtual const uint32_t *get_request_id(void) const { return &value_1; }

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const;

        virtual void print_summary(std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    // Action Response PDU
    //
    //  value_1 = request_id
    //  value_2 = request_status
    //
    class action_response_pdu_t : public abstract_siman_pdu_t
    {
      public:

        action_response_pdu_t(void) { }
        virtual ~action_response_pdu_t(void) { }

        virtual const uint32_t *get_request_id(void) const { return &value_1; }

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const;

        virtual void print_summary(std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    // Data PDU
    //
    //  value_1 = request_id
    //  value_2 = padding
    //
    class data_pdu_t : public abstract_siman_pdu_t
    {
      public:

        data_pdu_t(void) { }
        virtual ~data_pdu_t(void) { }

        virtual const uint32_t *get_request_id(void) const { return &value_1; }

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const;

        virtual void print_summary(std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    // Data Query PDU
    //
    //  value_1 = request_id
    //  value_2 = time_interval
    //
    class data_query_pdu_t : public abstract_siman_pdu_t
    {
      public:

        data_query_pdu_t(void) { }
        virtual ~data_query_pdu_t(void) { }

        virtual const uint32_t *get_request_id(void) const { return &value_1; }

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const;

        virtual void print_summary(std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    // Set Data PDU
    //
    //  value_1 = request_id
    //  value_2 = padding
    //
    class set_data_pdu_t : public abstract_siman_pdu_t
    {
      public:

        set_data_pdu_t(void) { }
        virtual ~set_data_pdu_t(void) { }

        virtual const uint32_t *get_request_id(void) const { return &value_1; }

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const;

        virtual void print_summary(std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    // Event Report PDU
    //
    //  value_1 = event_type
    //  value_2 = padding
    //
    class event_report_pdu_t : public abstract_siman_pdu_t
    {
      public:

        event_report_pdu_t(void) { }
        virtual ~event_report_pdu_t(void) { }

        virtual void print_values(
            const std::string &prefix,
            std::ostream &
        ) const;

        virtual void print_summary(std::ostream &) const;
    };
}

#endif
