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

#include "vdb_binary.h"
#include "vdb_hexadecimal.h"
#include "vdb_string.h"
#include "vdb_time.h"
#include "vdb_byte_stream.h"

namespace
{
    const float64_t
        TIME_UNIT_TO_SECONDS = (3600.0 / 2147483647.0),
        SECONDS_TO_TIME_UNITS = (1.0 / TIME_UNIT_TO_SECONDS);
}

const uint64_t
    vdb::time::MILLIS_PER_SECOND = 1000,
    vdb::time::MILLIS_PER_MINUTE = (MILLIS_PER_SECOND * 60),
    vdb::time::MILLIS_PER_HOUR = (MILLIS_PER_MINUTE * 60);
const uint32_t
    vdb::timestamp_t::LENGTH = 4,
    vdb::clocktime_t::LENGTH = 8;

// ----------------------------------------------------------------------------
time_struct_t *vdb::time::get_local()
{
    time_t
        seconds = std::time(0);
    time_struct_t
        *local_ptr = std::localtime(&seconds);

    local_ptr->tm_year += 1900;

    return local_ptr;
}

// ----------------------------------------------------------------------------
std::string vdb::time::to_string(const time_struct_t *ts_ptr)
{
    std::ostringstream
        stream;
    const time_struct_t
        *local_ptr = ts_ptr;

    if (not local_ptr)
    {
        local_ptr = get_local();
    }

    stream << std::setfill('0')
           << std::setw(2) << local_ptr->tm_mon << "-"
           << std::setw(2) << local_ptr->tm_mday << "-"
           << std::setw(4) << local_ptr->tm_year << " "
           << std::setw(2) << local_ptr->tm_hour << ":"
           << std::setw(2) << local_ptr->tm_min << ":"
           << std::setw(2) << local_ptr->tm_sec;

    return stream.str();
}

// ----------------------------------------------------------------------------
uint64_t vdb::time::get_system(void)
{
    struct timeval
        time;
    struct timezone
        zone;
    uint64_t
        milliseconds = 0;

    if (gettimeofday(&time, &zone) == 0)
    {
        milliseconds = (uint64_t)(time.tv_sec * 1000);
        milliseconds += ((uint64_t)(time.tv_usec) / 1000);
    }

    return milliseconds;
}

// ----------------------------------------------------------------------------
std::string vdb::time::to_string(uint64_t time)
{
    std::string
        time_string;
    time_t
        seconds = (time / 1000);

    time_string = std::string(std::ctime(&seconds));

    // Remove trailing new line character.
    //
    time_string = time_string.substr(0, (time_string.length() - 1));

    return time_string;
}

// ----------------------------------------------------------------------------
void vdb::time::parse(
    uint64_t system_time,
    uint32_t &hours,
    uint32_t &minutes,
    uint32_t &seconds,
    uint32_t &milliseconds)
{
    hours = (system_time / MILLIS_PER_HOUR);
    system_time -= (uint64_t)hours * MILLIS_PER_HOUR;

    minutes = (system_time / MILLIS_PER_MINUTE);
    system_time -= (uint64_t)minutes * MILLIS_PER_MINUTE;

    seconds = (system_time / MILLIS_PER_SECOND);
    system_time -= (uint64_t)seconds * MILLIS_PER_SECOND;

    milliseconds = system_time;
}

// ----------------------------------------------------------------------------
vdb::timestamp_t::timestamp_t(void) : value(0)
{

}

// ----------------------------------------------------------------------------
vdb::timestamp_t::timestamp_t(const timestamp_t &copy) : value(copy.value)
{

}

// ----------------------------------------------------------------------------
vdb::timestamp_t::~timestamp_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::get(
    uint32_t &value_minutes,
    float32_t &value_seconds,
    bool &value_absolute) const
{
    parse(value, value_minutes, value_seconds, value_absolute);
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::set(
    uint32_t value_minutes,
    float32_t value_seconds,
    bool value_absolute)
{
    parse(value_minutes, value_seconds, value_absolute, value);
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::clear(void)
{
    value = 0;
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    uint32_t
        value_minutes;
    float32_t
        value_seconds;
    bool
        value_absolute;

    parse(value, value_minutes, value_seconds, value_absolute);

    stream << value_minutes << "m "
           << float_to_string(value_seconds, 1, 3) << "s "
           << (value_absolute ? "(ABS)" : "(REL)");
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::read(byte_stream &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::write(byte_stream &stream) const
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::parse(
    uint32_t minutes,
    float32_t seconds,
    bool absolute,
    uint32_t &value)
{
    float64_t
        time = (seconds + ((float64_t)(minutes) * 60.0f));

    value = (uint32_t)(time * SECONDS_TO_TIME_UNITS);
    value = (value << 1);

    // Set the absolute bit.
    //
    if (absolute)
    {
        value |= 0x01;
    }
}

// ----------------------------------------------------------------------------
void vdb::timestamp_t::parse(
    uint32_t value,
    uint32_t &minutes,
    float32_t &seconds,
    bool &absolute)
{
    float64_t
        time = 0.0;

    absolute = ((value & 0x01) > 0);
    value = ((value >> 1) & 0x7FFFFFFF);

    time = (TIME_UNIT_TO_SECONDS * float64_t(value));
    minutes = (uint32_t)(time / 60.0);
    time -= ((float32_t)(minutes) * 60.0);
    seconds = time;
}

// ----------------------------------------------------------------------------
vdb::clocktime_t::clocktime_t() : hours(0)
{
}

// ----------------------------------------------------------------------------
vdb::clocktime_t::clocktime_t(float64_t time) : hours(0)
{
    set_time(time);
}

// ----------------------------------------------------------------------------
vdb::clocktime_t::clocktime_t(const clocktime_t &copy) :
    hours(copy.hours),
    offset(copy.offset)
{

}

// ----------------------------------------------------------------------------
vdb::clocktime_t::~clocktime_t()
{
}

// ----------------------------------------------------------------------------
void vdb::clocktime_t::set_epoch()
{
    struct timeval
        time;
    struct timezone
        zone;
    float64_t
        seconds = 0.0;

    if (gettimeofday(&time, &zone) == 0)
    {
        seconds = (float64_t)(time.tv_sec);
        seconds += ((float64_t)(time.tv_usec) / 1000000.0);
    }

    set_time(seconds);
}

// ----------------------------------------------------------------------------
void vdb::clocktime_t::set_time(float64_t seconds)
{
    hours = (uint32_t)(seconds / 3600.0f);

    offset.set(
        0,
        (seconds - (float64_t)(hours * 3600)),
        false);
}

// ----------------------------------------------------------------------------
void vdb::clocktime_t::clear(void)
{
    hours = 0;
    offset.clear();

}

// ----------------------------------------------------------------------------
void vdb::clocktime_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << hours << "h " << offset;
}

// ----------------------------------------------------------------------------
void vdb::clocktime_t::read(byte_stream &stream)
{
    stream.read(hours);
    stream.read(offset);
}

// ----------------------------------------------------------------------------
void vdb::clocktime_t::write(byte_stream &stream) const
{
    stream.write(hours);
    stream.write(offset);
}
