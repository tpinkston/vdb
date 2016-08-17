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

#ifndef VDB_TIME_H
#define VDB_TIME_H

#include "vdb_object.h"

typedef struct tm
    time_struct_t;

namespace vdb
{
    // ------------------------------------------------------------------------
    class time
    {
      public:

        static time_struct_t *get_local(void);
        static std::string to_string(const time_struct_t * = 0);

        static uint64_t get_system(void);
        static std::string to_string(uint64_t time);

        static void parse(
            uint64_t system_time,
            uint32_t &hours,
            uint32_t &minutes,
            uint32_t &seconds,
            uint32_t &milliseconds
        );

        static const uint64_t
            MILLIS_PER_SECOND,
            MILLIS_PER_MINUTE,
            MILLIS_PER_HOUR;
    };

    // ------------------------------------------------------------------------
    class timestamp_t : public record_t
    {
      public:

        timestamp_t(void);
        timestamp_t(const timestamp_t &copy);
        virtual ~timestamp_t(void);

        void get(
            uint32_t &value_minutes,
            float32_t &value_seconds,
            bool &value_absolute
        ) const;

        void set(
            uint32_t value_minutes,
            float32_t value_seconds,
            bool value_absolute
        );

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // Converts minutes, seconds and absolute flag to 32-bit integer.
        //
        static void parse(
            uint32_t minutes,
            float32_t seconds,
            bool absolute,
            uint32_t &value
        );

        // Converts 32-bit integer to minutes, seconds and absolute flag.
        //
        static void parse(
            uint32_t value,
            uint32_t &minutes,
            float32_t &seconds,
            bool &absolute
        );

        uint32_t
            value;

      protected:

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class clocktime_t : public record_t
    {
      public:

        clocktime_t(void);
        clocktime_t(float64_t time);
        clocktime_t(const clocktime_t &copy);
        virtual ~clocktime_t(void);

        // Sets the clock time to be the system time (Epoch) at the the time
        // of the method call.
        //
        void set_epoch(void);

        // Sets the clock time according to the given time in seconds,
        // acceptable ranges are from zero to 'MAX DOUBLE'
        //
        void set_time(float64_t seconds);

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint32_t
            hours;     // 32-bit unsigned integer (number of hours)
        timestamp_t
            offset;    // 32-bit record (minutes and seconds past the hour)

      protected:

        static const uint32_t
            LENGTH;
    };
}

#endif
