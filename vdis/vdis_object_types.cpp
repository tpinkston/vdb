#include "vdis_data_types.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const string_t
        empty_string;
}

std::map<uint32_t, string_t>
    vdis::object_types::descriptions;
std::map<uint32_t, vdis::object_geometry_e>
    vdis::object_types::geometries;
bool
    vdis::object_types::loaded = false;

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
    std::map<uint32_t, string_t>::const_iterator
        description = descriptions.begin();
    std::map<uint32_t, object_geometry_e>::const_iterator
        geometry;
    object_type_t
        object_type;

    while(description != descriptions.end())
    {
        geometry = geometries.find(description->first);

        object_type.set(description->first);

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
const string_t &vdis::object_types::get_description(uint32_t value)
{
    std::map<uint32_t, string_t>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        return search_itor->second;
    }

    return empty_string;
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
bool vdis::object_types::get_parent(
    const object_type_t &child,
    object_type_t &parent)
{
    bool success = false;

    parent = child;

    if (parent.subcategory > 0)
    {
        parent.subcategory = 0;
        success = true;
    }
    else if (parent.category > 0)
    {
        parent.category = 0;
        success = true;
    }
    else if (parent.kind > 0)
    {
        parent.kind = 0;
        success = true;
    }
    else if (parent.domain > 0)
    {
        parent.domain = 0;
        success = true;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdis::object_types::get_valid_parent(
    const object_type_t &child,
    object_type_t &parent)
{
    std::map<uint32_t, string_t>::const_iterator
        search_itor;
    object_type_t
        temp_child = child,
        temp_parent;
    bool
        success = false;

    while(get_parent(temp_child, temp_parent) and not success)
    {
        search_itor = descriptions.find(temp_parent.get());

        if (search_itor != descriptions.end())
        {
            parent = temp_parent;
            success = true;
        }
        else
        {
            temp_child = temp_parent;
        }
    }

    return success;
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

    value = type.get();

    if (description_ptr)
    {
        descriptions[value] = description_ptr;
    }

    if (geometry_ptr)
    {
        string_t
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
