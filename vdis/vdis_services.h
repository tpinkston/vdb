#ifndef VDIS_SERVICES_H
#define VDIS_SERVICES_H

#include "vdis_data_types.h"
#include "vdis_enums.h"

#define RCAST(T, t) *reinterpret_cast<const T *>(t)
#define SCAST(T, t) *static_cast<const T *>(t)

typedef struct timeval
    time_value_t;
typedef struct tm
    time_struct_t;
typedef struct timezone
    time_zone_t;

namespace color
{
    // ------------------------------------------------------------------------
    // Color definitions for TTY output

    enum color_e
    {
        none = 0,
        red = 31,
        green = 32,
        yellow = 33,
        blue = 34,
        purple = 35,
        cyan = 36,
        white = 37
    };

    enum bold_color_e
    {
        bold_red = 31,
        bold_green = 32,
        bold_yellow = 33,
        bold_blue = 34,
        bold_purple = 35,
        bold_cyan = 36,
        bold_white = 37
    };

    color_e get(vdis::force_id_e);

    void set_enabled(bool value);
}

std::ostream &operator<<(std::ostream &stream, color::color_e c);
std::ostream &operator<<(std::ostream &stream, color::bold_color_e c);

namespace vdis
{
    // ------------------------------------------------------------------------
    // Time functions

    time_struct_t *get_local_time(void);
    string_t time_to_string(const time_struct_t * = 0);

    uint64_t get_system_time(void);
    string_t time_to_string(uint64_t time);

    uint64_t get_system_time(const time_value_t &value);
    time_value_t get_time_value(uint64_t time);

    void parse_time(
        uint64_t system_time,
        uint32_t &hours,
        uint32_t &minutes,
        uint32_t &seconds,
        uint32_t &milliseconds
    );

    // ------------------------------------------------------------------------
    // Endian and byte swapping

    bool little_endian(void);
    bool big_endian(void);
    bool byteswapping(void);
    void set_byteswapping(void);
    void force_byteswapping(bool value);

    int16_t byteswap(int16_t value, bool force = false);
    int32_t byteswap(int32_t value, bool force = false);
    int64_t byteswap(int64_t value, bool force = false);
    uint16_t byteswap(uint16_t value, bool force = false);
    uint32_t byteswap(uint32_t value, bool force = false);
    uint64_t byteswap(uint64_t value, bool force = false);

    void swap(int16_t &value, bool force = false);
    void swap(int32_t &value, bool force = false);
    void swap(int64_t &value, bool force = false);
    void swap(uint16_t &value, bool force = false);
    void swap(uint32_t &value, bool force = false);
    void swap(uint64_t &value, bool force = false);

    // ------------------------------------------------------------------------
    // Miscellaneous

    uint32_t padding_length(uint32_t length, uint32_t boundary = 8);

    inline bool is_printable_character(uint8_t value)
    {
        return (value > 31) and (value < 127);
    }

    // ------------------------------------------------------------------------
    // Entity ID to marking
    //
    class entity_marking
    {
      public:

        entity_marking(const id_t &id) : id(id) { }
        ~entity_marking(void) { }

        static string_t get_marking(const id_t &);
        static const marking_t *get(const id_t &);
        static void set(const id_t &, const marking_t &);
        static void unset(const id_t &);

        const id_t &id;

      private:

        typedef std::map<id_t, marking_t>
            marking_map_t;

        static marking_map_t
            markings;
    };
}

// ----------------------------------------------------------------------------
inline bool vdis::little_endian(void)
{
    return (__BYTE_ORDER == __LITTLE_ENDIAN);
}

// ----------------------------------------------------------------------------
inline bool vdis::big_endian(void)
{
    return (__BYTE_ORDER == __BIG_ENDIAN);
}

// ----------------------------------------------------------------------------
inline void vdis::swap(int16_t &value, bool force)
{
    value = byteswap(value, force);
}

// ----------------------------------------------------------------------------
inline void vdis::swap(int32_t &value, bool force)
{
    value = byteswap(value, force);
}

// ----------------------------------------------------------------------------
inline void vdis::swap(int64_t &value, bool force)
{
    value = byteswap(value, force);
}

// ----------------------------------------------------------------------------
inline void vdis::swap(uint16_t &value, bool force)
{
    value = byteswap(value, force);
}

// ----------------------------------------------------------------------------
inline void vdis::swap(uint32_t &value, bool force)
{
    value = byteswap(value, force);
}

// ----------------------------------------------------------------------------
inline void vdis::swap(uint64_t &value, bool force)
{
    value = byteswap(value, force);
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(
    std::ostream &out,
    const vdis::entity_marking &value)
{
    out << value.id << " " << vdis::entity_marking::get_marking(value.id);

    return out;
}

#endif
