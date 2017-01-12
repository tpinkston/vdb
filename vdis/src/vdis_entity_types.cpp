#include "vdis_data_types.h"
#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_string.h"

namespace
{
    const string_t
        empty_string;
}

std::map<uint64_t, string_t>
    vdis::entity_types::names,
    vdis::entity_types::descriptions;
bool
    vdis::entity_types::loaded = false;

// ----------------------------------------------------------------------------
void vdis::entity_types::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;

        LOG_VERBOSE("Loaded %d entity types...", names.size());
    }
}

// ----------------------------------------------------------------------------
void vdis::entity_types::print(std::ostream &stream)
{
    std::map<uint64_t, string_t>::const_iterator
        name = names.begin();
    entity_type_t
        entity_type;

    while(name != names.end())
    {
        entity_type.set(name->first);

        stream << entity_type << "," << name->second << std::endl;

        ++name;
    }
}

// ----------------------------------------------------------------------------
const string_t &vdis::entity_types::get_name(uint64_t value)
{
    std::map<uint64_t, string_t>::const_iterator
        search_itor = names.find(value);

    if (search_itor != names.end())
    {
        return search_itor->second;
    }

    return empty_string;
}

// ----------------------------------------------------------------------------
const string_t &vdis::entity_types::get_description(uint64_t value)
{
    std::map<uint64_t, string_t>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        return search_itor->second;
    }

    return empty_string;
}

// ----------------------------------------------------------------------------
bool vdis::entity_types::get_name(
    uint64_t value,
    string_t &name)
{
    std::map<uint64_t, string_t>::const_iterator
        search_itor = names.find(value);

    if (search_itor != names.end())
    {
        name = search_itor->second;
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::entity_types::get_description(
    uint64_t value,
    string_t &description)
{
    std::map<uint64_t, string_t>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        description = search_itor->second;
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdis::entity_types::get_parent(
    const entity_type_t &child,
    entity_type_t &parent)
{
    bool success = false;

    parent = child;

    if (parent.extra > 0)
    {
        parent.extra = 0;
        success = true;
    }
    else if (parent.specific > 0)
    {
        parent.specific = 0;
        success = true;
    }
    else if (parent.subcategory > 0)
    {
        parent.subcategory = 0;
        success = true;
    }
    else if (parent.category > 0)
    {
        parent.category = 0;
        success = true;
    }
    else if (parent.country > 0)
    {
        parent.country = 0;
        success = true;
    }
    else if (parent.domain > 0)
    {
        parent.domain = 0;
        success = true;
    }
    else if (parent.kind > 0)
    {
        parent.kind = 0;
        success = true;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdis::entity_types::get_valid_parent(
    const entity_type_t &child,
    entity_type_t &parent)
{
    std::map<uint64_t, string_t>::const_iterator
        search_itor;
    entity_type_t
        temp_child = child,
        temp_parent;
    bool
        success = false;

    while(get_parent(temp_child, temp_parent) and not success)
    {
        search_itor = names.find(temp_parent.get());

        if (search_itor != names.end())
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
void vdis::entity_types::get_values(
    const entity_type_t &type,
    uint16_t values[7])
{
    values[0] = (uint16_t)type.kind;
    values[1] = (uint16_t)type.domain;
    values[2] = (uint16_t)type.country;
    values[3] = (uint16_t)type.category;
    values[4] = (uint16_t)type.subcategory;
    values[5] = (uint16_t)type.specific;
    values[6] = (uint16_t)type.extra;
}

// ----------------------------------------------------------------------------
void vdis::entity_types::add(
    const int kind,
    const int domain,
    const int country,
    const int category,
    const int subcategory,
    const int specific,
    const int extra,
    const char *name_ptr,
    const char *description_ptr)
{
    std::map<uint64_t, string_t>::const_iterator
        search_itor;
    uint64_t
        value = 0;
    entity_type_t
        type;

    type.kind = kind;
    type.domain = domain;
    type.country = country;
    type.category = category;
    type.subcategory = subcategory;
    type.specific = specific;
    type.extra = extra;

    value = type.get();

    search_itor = names.find(value);

    if (search_itor != names.end())
    {
        LOG_WARNING(
            "Duplicate entity type: %llu = %s = %s",
            value,
            to_string(type).c_str(),
            (name_ptr ? name_ptr : "NULL"));
    }

    if (name_ptr)
    {
        names[value] = name_ptr;
    }

    if (description_ptr)
    {
        descriptions[value] = description_ptr;
    }
}
