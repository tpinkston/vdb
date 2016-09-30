#include <cstdlib>
#include <unistd.h>

#include "vdis_data_types.h"
#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    bool
        swapping = false;
    bool
        color_enabled = true;
}

vdis::entity_marking::marking_map_t
    vdis::entity_marking::markings;

// ----------------------------------------------------------------------------
void color::set_enabled(bool value)
{
    color_enabled = value;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &stream, color::color_e c)
{
    if (isatty(STDOUT_FILENO) and color_enabled)
    {
        return (stream << "\e[" << (int)c << "m");
    }
    else
    {
        return stream;
    }
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &stream, color::bold_color_e c)
{
    if (isatty(STDOUT_FILENO) and color_enabled)
    {
        return (stream << "\e[;1;" << (int)c << "m");
    }
    else
    {
        return stream;
    }
}

// ----------------------------------------------------------------------------
bool vdis::byteswapping(void)
{
    return swapping;
}

// ----------------------------------------------------------------------------
void vdis::set_byteswapping(void)
{
    swapping = big_endian();
}

// ----------------------------------------------------------------------------
void vdis::force_byteswapping(bool value)
{
    swapping = value;
}

// ----------------------------------------------------------------------------
int16_t vdis::byteswap(int16_t value, bool force)
{
    uint16_t result = value;

    if (swapping or force)
    {
        result = (value << 8);
        result |= ((value >> 8) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
uint16_t vdis::byteswap(uint16_t value, bool force)
{
    uint16_t result = value;

    if (swapping or force)
    {
        result = (value << 8);
        result |= ((value >> 8) & 0xFF);
    }

    // LOG_EXTRA_VERBOSE("Byteswap uint16_t: %d -> %d", value, result);

    return result;
}

// ----------------------------------------------------------------------------
int32_t vdis::byteswap(int32_t value, bool force)
{
    int32_t result = value;

    if (swapping or force)
    {
        result = (value << 24);
        result |= ((value << 8) & 0xFF0000);
        result |= ((value >> 8) & 0xFF00);
        result |= ((value >> 24) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
uint32_t vdis::byteswap(uint32_t value, bool force)
{
    uint32_t result = value;

    if (swapping or force)
    {
        result = (value << 24);
        result |= ((value << 8) & 0xFF0000);
        result |= ((value >> 8) & 0xFF00);
        result |= ((value >> 24) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
int64_t vdis::byteswap(int64_t value, bool force)
{
    int64_t result = value;

    if (swapping or force)
    {
        result = (value << 56);
        result |= ((value << 40) & 0xFF000000000000);
        result |= ((value << 24) & 0xFF0000000000);
        result |= ((value << 8) & 0xFF00000000);
        result |= ((value >> 8) & 0xFF000000);
        result |= ((value >> 24) & 0xFF0000);
        result |= ((value >> 40) & 0xFF00);
        result |= ((value >> 56) & 0xFF);
    }

    return result;
}

// ----------------------------------------------------------------------------
uint64_t vdis::byteswap(uint64_t value, bool force)
{
    uint64_t result = value;

    if (swapping or force)
    {
        result = (value << 56);
        result |= ((value << 40) & 0xFF000000000000);
        result |= ((value << 24) & 0xFF0000000000);
        result |= ((value << 8) & 0xFF00000000);
        result |= ((value >> 8) & 0xFF000000);
        result |= ((value >> 24) & 0xFF0000);
        result |= ((value >> 40) & 0xFF00);
        result |= ((value >> 56) & 0xFF);
    }

    return result;
}


// ----------------------------------------------------------------------------
// Returns the number of bytes needed to pad data with the specified
// length to a certain word boundary (default 8 bytes).
//
uint32_t vdis::padding_length(uint32_t length, uint32_t boundary)
{
    uint32_t
        length_data = length,
        length_padding = 0;

    while(length_data % boundary)
    {
        length_data += 1;
        length_padding++;
    }

    return length_padding;
}

// ----------------------------------------------------------------------------
std::string vdis::entity_marking::get_marking(const entity_id_t &id)
{
    const marking_t
        *marking_ptr = get(id);

    if (marking_ptr)
    {
        return marking_ptr->str();
    }
    else if (id.is_none())
    {
        return "[NO ENTITY]";
    }
    else if (id.is_all())
    {
        return "[ALL ENTITIES]";
    }
    else
    {
        return "[UNKNOWN]";
    }
}

// ----------------------------------------------------------------------------
const vdis::marking_t *vdis::entity_marking::get(const entity_id_t &id)
{
    const marking_t
        *marking_ptr = 0;
    marking_map_t::const_iterator
        itor = markings.find(id);

    if (itor != markings.end())
    {
        marking_ptr = &itor->second;
    }

    return marking_ptr;
}

// ----------------------------------------------------------------------------
void vdis::entity_marking::set(const entity_id_t &id, const marking_t &marking)
{
    marking_map_t::const_iterator
        itor = markings.find(id);

    if (itor == markings.end())
    {
        LOG_VERBOSE("Setting marking for entity %s", to_string(id).c_str());
        markings[id] = marking;
    }
    else if (marking != itor->second)
    {
        LOG_VERBOSE("Resetting marking for entity %s", to_string(id).c_str());
        markings[id] = marking;
    }
}

// ----------------------------------------------------------------------------
void vdis::entity_marking::unset(const entity_id_t &id)
{
    marking_map_t::const_iterator
        itor = markings.find(id);

    if (itor != markings.end())
    {
        LOG_VERBOSE("Unsetting marking for entity %s", to_string(id).c_str());
        markings.erase(id);
    }
}
