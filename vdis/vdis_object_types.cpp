#include "vdis_data_types.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const std::string
        description_undefined = "Undefined";
}

// ----------------------------------------------------------------------------
void vdis::object_types::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;

        LOG_VERBOSE("Loaded %d object types...", descriptions.size());
    }
}

// ----------------------------------------------------------------------------
void vdis::object_types::print(std::ostream &stream)
{
    std::map<uint32_t, std::string>::const_iterator
        description = descriptions.begin();
    std::map<uint32_t, object_geometry_e>::const_iterator
        geometry;
    object_type_t
        object_type;

    while(description != descriptions.end())
    {
        geometry = geometries.find(description->first);

        convert(description->first, object_type);

        stream << object_type << ",";

        if (geometry != geometries.end())
        {
            stream << (object_geometry_e)geometry->second;
        }
        else
        {
            stream << "unknown geometry";
        }

        stream << ",\"" << description->second << "\"" << std::endl;

        ++description;
    }
}

// ----------------------------------------------------------------------------
void vdis::object_types::convert(const object_type_t &type, uint32_t &value)
{
    value = 0;
    value = (uint8_t)(type.domain & 0xFF);
    value <<= 8;
    value |= (uint8_t)(type.kind & 0xFF);
    value <<= 8;
    value |= (uint8_t)(type.category & 0xFF);
    value <<= 8;
    value |= (uint8_t)(type.subcategory & 0xFF);
}

// ----------------------------------------------------------------------------
void vdis::object_types::convert(const uint32_t value, object_type_t &type)
{
    uint32_t bits = value;

    type.subcategory = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    type.category = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    type.kind = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    type.domain = (uint8_t)(bits & 0xFF);
}

// ----------------------------------------------------------------------------
const std::string &vdis::object_types::get_description(uint32_t value)
{
    std::map<uint32_t, std::string>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        return search_itor->second;
    }

    return description_undefined;
}

// ----------------------------------------------------------------------------
vdis::object_geometry_e vdis::object_types::get_geometry(uint32_t value)
{
    std::map<uint32_t, object_geometry_e>::const_iterator
        search_itor = geometries.find(value);

    if (search_itor != geometries.end())
    {
        return search_itor->second;
    }

    return OBJECT_GEOMETRY_UNKNOWN;
}

// ----------------------------------------------------------------------------
void vdis::object_types::add(
    const int domain,
    const int kind,
    const int category,
    const int subcategory,
    const char *description_ptr,
    const char *geometry_ptr)
{
    uint32_t
        value;
    object_type_t
        type;

    type.domain = (uint8_t)domain;
    type.kind = (uint8_t)kind;
    type.category = (uint8_t)category;
    type.subcategory = (uint8_t)subcategory;

    convert(value, type);

    if (description_ptr)
    {
        descriptions[value] = description_ptr;
    }

    if (geometry_ptr)
    {
        std::string
            geometry = to_lowercase(geometry_ptr);

        if (geometry == "point")
        {
            geometries[value] = OBJECT_GEOMETRY_POINT;
        }
        else if (geometry == "linear")
        {
            geometries[value] = OBJECT_GEOMETRY_LINEAR;
        }
        else if (geometry == "areal")
        {
            geometries[value] = OBJECT_GEOMETRY_AREAL;
        }
        else
        {
            geometries[value] = OBJECT_GEOMETRY_UNKNOWN;
        }
    }
}
