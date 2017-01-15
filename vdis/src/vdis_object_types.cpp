#include "vdis_data_types.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const std::string
        empty_string;
}

std::map<uint32_t, std::string>
    vdis::object_types::unknown_descriptions,
    vdis::object_types::point_descriptions,
    vdis::object_types::linear_descriptions,
    vdis::object_types::areal_descriptions;
bool
    vdis::object_types::loaded = false;

// ----------------------------------------------------------------------------
void vdis::object_types::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;

        LOG_VERBOSE(
            "Loaded %d point object types...",
            point_descriptions.size());
        LOG_VERBOSE(
            "Loaded %d linear object types...",
            linear_descriptions.size());
        LOG_VERBOSE(
            "Loaded %d areal object types...",
            areal_descriptions.size());
    }
}

// ----------------------------------------------------------------------------
void vdis::object_types::print(std::ostream &out)
{
    print(point_descriptions, OBJECT_GEOMETRY_POINT, out);
    print(linear_descriptions, OBJECT_GEOMETRY_LINEAR, out);
    print(areal_descriptions, OBJECT_GEOMETRY_AREAL, out);
}

// ----------------------------------------------------------------------------
const std::string &vdis::object_types::get_description(
    object_geometry_e geometry,
    uint32_t value)
{
    const std::map<uint32_t, std::string>
        *descriptions_ptr = 0;

    switch(geometry)
    {
        case OBJECT_GEOMETRY_POINT:
            descriptions_ptr = &point_descriptions;
            break;
        case OBJECT_GEOMETRY_LINEAR:
            descriptions_ptr = &linear_descriptions;
            break;
        case OBJECT_GEOMETRY_AREAL:
            descriptions_ptr = &areal_descriptions;
            break;
        default:
            LOG_WARNING(
                "Invalid object geometry: %s",
                enumerations::get_name(ENUM_OBJECT_GEOMETRY, geometry).c_str());
    }

    if (descriptions_ptr)
    {
        std::map<uint32_t, std::string>::const_iterator
            search_itor = descriptions_ptr->find(value);

        if (search_itor != descriptions_ptr->end())
        {
            return search_itor->second;
        }
    }

    return empty_string;
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
    const object_geometry_e geometry,
    const object_type_t &child,
    object_type_t &parent)
{
    std::map<uint32_t, std::string>::const_iterator
        search_itor;
    object_type_t
        temp_child = child,
        temp_parent;
    bool
        success = false;

    while(get_parent(temp_child, temp_parent) and not success)
    {
        std::string description = get_description(geometry, temp_parent.get());

        if (description.empty())
        {
            temp_child = temp_parent;
        }
        else
        {
            parent = temp_parent;
            success = true;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
void vdis::object_types::print(
    const std::map<uint32_t, std::string> &descriptions,
    object_geometry_e geometry,
    std::ostream &out)
{
    std::map<uint32_t, std::string>::const_iterator
        description = descriptions.begin();
    object_type_t
        object_type;

    while(description != descriptions.end())
    {
        object_type.set(description->first);

        out << object_type << "," << geometry
            << ",\"" << description->second << "\"" << std::endl;

        ++description;
    }
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

    if (not description_ptr)
    {
        LOG_ERROR(
            "No description for object type %d!",
            to_string(type).c_str());
    }
    else if (not geometry_ptr)
    {
        LOG_ERROR(
            "No geometry for object type %d!",
            to_string(type).c_str());
    }
    else
    {
        std::string
            geometry = to_lowercase(geometry_ptr);

        if (geometry == "point")
        {
            point_descriptions[value] = description_ptr;
        }
        else if (geometry == "linear")
        {
            linear_descriptions[value] = description_ptr;
        }
        else if (geometry == "areal")
        {
            areal_descriptions[value] = description_ptr;
        }
        else
        {
            LOG_ERROR(
                "Loading invalid object geometry '%s' for object type %d!",
                geometry.c_str(),
                to_string(type).c_str());
        }
    }
}
