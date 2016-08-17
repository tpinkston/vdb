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

#ifndef VDB_STANDARD_VARIABLE_RECORD_H
#define VDB_STANDARD_VARIABLE_RECORD_H

#include "vdb_data_buffer.h"
#include "vdb_object.h"

namespace vdb
{
    class svr_content_t;

    // ------------------------------------------------------------------------
    class svr_t : public record_t
    {
      public:

        svr_t(void);
        svr_t(const svr_t &copy);
        virtual ~svr_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint32_t
            type;
        svr_content_t
            *content_ptr;

        static const uint32_t
            BASE_LENGTH = 6; // bytes, includes type (4) and length (2)
    };

    // ------------------------------------------------------------------------
    class svr_content_t : public record_t
    {
      public:

        svr_content_t(void) { }
        svr_content_t(const svr_content_t &copy) { }
        virtual ~svr_content_t(void) { }

        virtual uint32_t length(void) const = 0;

      protected:

        uint32_t read_length(byte_stream &stream);
    };

    // ------------------------------------------------------------------------
    class svr_default_content_t : public svr_content_t
    {
      public:

        svr_default_content_t(void);
        svr_default_content_t(const svr_default_content_t &copy);
        virtual ~svr_default_content_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        data_buffer_t
            data;
    };

    // ------------------------------------------------------------------------
    class exercise_state_t : public svr_content_t
    {
      public:

        exercise_state_t(void);
        exercise_state_t(const exercise_state_t &copy);
        virtual ~exercise_state_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            id,
            transition,
            current_state,
            requested_state;

        static const uint32_t
            LENGTH = 4;
    };

    // ------------------------------------------------------------------------
    class application_state_t : public svr_content_t
    {
      public:

        application_state_t(void);
        application_state_t(const application_state_t &copy);
        virtual ~application_state_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            padding,
            transition,
            current_state,
            requested_state,
            current_configuration;
        std::vector<exercise_state_t>
            exercise_states;

        static const uint32_t
            BASE_LENGTH = 6;
    };

    // ------------------------------------------------------------------------
    class standard_variable_records_t : public record_t
    {
      public:

        standard_variable_records_t(void);
        standard_variable_records_t(const standard_variable_records_t &copy);
        virtual ~standard_variable_records_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;
        virtual void print_summary(std::ostream &stream) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        std::vector<svr_t>
            records;

        // Two bytes for the 16-bit "number of records" field.
        //
        static const uint32_t
            BASE_LENGTH = 2;
    };
}

#endif
