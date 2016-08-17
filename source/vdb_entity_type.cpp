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

#include "vdb_entity_type.h"

#include "vdb_byte_stream.h"
#include "vdb_logger.h"
#include "vdb_string.h"

namespace
{
    const std::string
        empty_string,
        undefined_name = "UNDEFINED",
        undefined_description = "Undefined";
}

namespace vdb
{
    const uint32_t
        entity_type_t::LENGTH = 8;
    std::map<uint64_t, std::string>
        entity_type_data::names,
        entity_type_data::descriptions;
    bool
        entity_type_data::loaded = false;
}

// ----------------------------------------------------------------------------
vdb::entity_type_t::entity_type_t(void) :
    kind(0),
    domain(0),
    country(0),
    category(0),
    subcategory(0),
    specific(0),
    extra(0)
{

}

// ----------------------------------------------------------------------------
vdb::entity_type_t::entity_type_t(uint64_t value) :
    kind(0),
    domain(0),
    country(0),
    category(0),
    subcategory(0),
    specific(0),
    extra(0)
{
    set_value(value);
}

// ----------------------------------------------------------------------------
vdb::entity_type_t::entity_type_t(const uint16_t values[7]) :
    kind(0),
    domain(0),
    country(0),
    category(0),
    subcategory(0),
    specific(0),
    extra(0)
{
    if (values[0] > 0x00FF)
    {
        LOG_ERROR("Value for 'kind' out of range: %d", (int)values[0]);
    }

    if (values[1] > 0x00FF)
    {
        LOG_ERROR("Value for 'domain' out of range: %d", (int)values[1]);
    }

    if (values[2] > 0xFFFF)
    {
        LOG_ERROR("Value for 'country' out of range: %d", (int)values[2]);
    }

    if (values[3] > 0x00FF)
    {
        LOG_ERROR("Value for 'category' out of range: %d", (int)values[3]);
    }

    if (values[4] > 0x00FF)
    {
        LOG_ERROR("Value for 'subcategory' out of range: %d", (int)values[4]);
    }

    if (values[5] > 0x00FF)
    {
        LOG_ERROR("Value for 'specific' out of range: %d", (int)values[5]);
    }

    if (values[6] > 0x00FF)
    {
        LOG_ERROR("Value for 'extra' out of range: %d", (int)values[6]);
    }

    kind = (uint8_t)values[0];
    domain = (uint8_t)values[1];
    country = (uint8_t)values[2];
    category = (uint8_t)values[3];
    subcategory = (uint8_t)values[4];
    specific = (uint8_t)values[5];
    extra = (uint8_t)values[6];
}

// ----------------------------------------------------------------------------
vdb::entity_type_t::entity_type_t(const entity_type_t &copy) :
    record_t(copy),
    kind(copy.kind),
    domain(copy.domain),
    country(copy.country),
    category(copy.category),
    subcategory(copy.subcategory),
    specific(copy.specific),
    extra(copy.extra)
{

}

// ----------------------------------------------------------------------------
vdb::entity_type_t::~entity_type_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::valid(void) const
{
    static std::string
        dummy_name;

    return entity_type_data::get_name(get_value(), dummy_name);
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::empty(void) const
{
    return (kind == 0) and
           (domain == 0) and
           (country == 0) and
           (category == 0) and
           (subcategory == 0) and
           (specific == 0) and
           (extra == 0);
}

// ----------------------------------------------------------------------------
const std::string &vdb::entity_type_t::get_name(void) const
{
    if (name.empty())
    {
        name = entity_type_data::get_name(get_value());

        if (name.empty())
        {
            entity_type_t
                parent;

            if (get_valid_parent(parent))
            {
                name = entity_type_data::get_name(parent.get_value());
                name += "_PARENT";
            }
        }

        if (name.empty())
        {
            name = undefined_name;
        }
    }

    return name;
}

// ----------------------------------------------------------------------------
const std::string &vdb::entity_type_t::get_description(void) const
{
    if (description.empty())
    {
        description = entity_type_data::get_description(get_value());

        if (description.empty())
        {
            entity_type_t
                parent;

            if (get_valid_parent(parent))
            {
                description = entity_type_data::get_description(
                    parent.get_value());

                description += " (Parent)";
            }
        }

        if (description.empty())
        {
            description = undefined_description;
        }
    }

    return description;
}

// ----------------------------------------------------------------------------
void vdb::entity_type_t::get_values(uint16_t values[7]) const
{
    values[0] = (uint16_t)kind;
    values[1] = (uint16_t)domain;
    values[2] = (uint16_t)country;
    values[3] = (uint16_t)category;
    values[4] = (uint16_t)subcategory;
    values[5] = (uint16_t)specific;
    values[6] = (uint16_t)extra;
}

// ----------------------------------------------------------------------------
void vdb::entity_type_t::get_parent(entity_type_t &parent) const
{
    parent = *this;

    if (parent.get_extra() > 0)
    {
        parent.set_extra(0);
    }
    else if (parent.get_specific() > 0)
    {
        parent.set_specific(0);
    }
    else if (parent.get_subcategory() > 0)
    {
        parent.set_subcategory(0);
    }
    else if (parent.get_category() > 0)
    {
        parent.set_category(0);
    }
    else if (parent.get_country() > 0)
    {
        parent.set_country(0);
    }
    else if (parent.get_domain() > 0)
    {
        parent.set_domain(0);
    }
    else if (parent.get_kind() > 0)
    {
        parent.set_kind(0);
    }
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::get_valid_parent(entity_type_t &parent) const
{
    uint16_t
        values[7];
    bool
        valid_parent = false;

    get_values(values);

    for(int32_t i = 6; (i >= 0) and not valid_parent; --i)
    {
        if (values[i] != 0)
        {
            values[i] = 0;

            entity_type_t
                temp_parent(values);

            if (temp_parent.valid())
            {
                parent = temp_parent;
                valid_parent = true;
            }
        }
    }

    return valid_parent;
}

// ----------------------------------------------------------------------------
void vdb::entity_type_t::clear(void)
{
    kind = 0;
    domain = 0;
    country = 0;
    category = 0;
    subcategory = 0;
    specific = 0;
    extra = 0;
    name.clear();
    description.clear();
}

// ----------------------------------------------------------------------------
void vdb::entity_type_t::print(
    const std::string &prefix,
    std::ostream &ost) const
{
    ost << prefix
        << (int)kind << "."
        << (int)domain << "."
        << (int)country << "."
        << (int)category << "."
        << (int)subcategory << "."
        << (int)specific << "."
        << (int)extra;
}

// ----------------------------------------------------------------------------
void vdb::entity_type_t::read(byte_stream &stream)
{
    stream.read(kind);
    stream.read(domain);
    stream.read(country);
    stream.read(category);
    stream.read(subcategory);
    stream.read(specific);
    stream.read(extra);
}

// ----------------------------------------------------------------------------
void vdb::entity_type_t::write(byte_stream &stream) const
{
    stream.write(kind);
    stream.write(domain);
    stream.write(country);
    stream.write(category);
    stream.write(subcategory);
    stream.write(specific);
    stream.write(extra);
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::operator==(const entity_type_t &rhs) const
{
    return (kind == rhs.kind) and
           (domain == rhs.domain) and
           (country == rhs.country) and
           (category == rhs.category) and
           (subcategory == rhs.subcategory) and
           (specific == rhs.specific) and
           (extra == rhs.extra);
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::operator!=(const entity_type_t &rhs) const
{
    return not (*this == rhs);
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::operator<(const entity_type_t &rhs) const
{
    if (kind < rhs.kind)
    {
        return true;
    }
    else if (kind > rhs.kind)
    {
         return false;
    }

    if (domain < rhs.domain)
    {
        return true;
    }
    else if (domain > rhs.domain)
    {
        return false;
    }

    if (country < rhs.country)
    {
        return true;
    }
    else if (country > rhs.country)
    {
        return false;
    }

    if (category < rhs.category)
    {
        return true;
    }
    else if (category > rhs.category)
    {
        return false;
    }

    if (subcategory < rhs.subcategory)
    {
        return true;
    }
    else if (subcategory > rhs.subcategory)
    {
        return false;
    }

    if (specific < rhs.specific)
    {
        return true;
    }
    else if (specific > rhs.specific)
    {
        return false;
    }

    if (extra < rhs.extra)
    {
        return true;
    }
    else
    {
        return false;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_t::operator>(const entity_type_t &rhs) const
{
    return (*this != rhs) and not (*this < rhs);
}

// ----------------------------------------------------------------------------
void vdb::entity_type_data::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;

        LOG_VERBOSE("Loaded %d entity types...", names.size());
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_type_data::print(std::ostream &stream)
{
    std::map<uint64_t, std::string>::const_iterator
        name = names.begin();
    entity_type_t
        entity_type;

    while(name != names.end())
    {
        entity_type.set_value(name->first);

        stream << entity_type << "," << name->second << std::endl;

        ++name;
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_type_data::convert_to_value(
    const uint8_t kind,
    const uint8_t domain,
    const uint16_t country,
    const uint8_t category,
    const uint8_t subcategory,
    const uint8_t specific,
    const uint8_t extra,
    uint64_t &value)
{
    value = 0;
    value = (uint8_t)(kind & 0xFF);
    value <<= 8;
    value |= (uint8_t)(domain & 0xFF);
    value <<= 16;
    value |= (uint16_t)(country & 0xFFFF);
    value <<= 8;
    value |= (uint8_t)(category & 0xFF);
    value <<= 8;
    value |= (uint8_t)(subcategory & 0xFF);
    value <<= 8;
    value |= (uint8_t)(specific & 0xFF);
    value <<= 8;
    value |= (uint8_t)(extra & 0xFF);
}

// ----------------------------------------------------------------------------
void vdb::entity_type_data::convert_from_value(
    const uint64_t value,
    uint8_t &kind,
    uint8_t &domain,
    uint16_t &country,
    uint8_t &category,
    uint8_t &subcategory,
    uint8_t &specific,
    uint8_t &extra)
{
    uint64_t bits = value;

    extra = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    specific = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    subcategory = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    category = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    country = (uint16_t)(bits & 0xFFFF);
    bits >>= 16;

    domain = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    kind = (uint8_t)(bits & 0xFF);
}

// ----------------------------------------------------------------------------
const std::string &vdb::entity_type_data::get_name(uint64_t value)
{
    std::map<uint64_t, std::string>::const_iterator
        search_itor = names.find(value);

    if (search_itor != names.end())
    {
        return search_itor->second;
    }

    return empty_string;
}

// ----------------------------------------------------------------------------
const std::string &vdb::entity_type_data::get_description(uint64_t value)
{
    std::map<uint64_t, std::string>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        return search_itor->second;
    }

    return empty_string;
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_data::get_name(
    uint64_t value,
    std::string &name)
{
    std::map<uint64_t, std::string>::const_iterator
        search_itor = names.find(value);

    if (search_itor != names.end())
    {
        name = search_itor->second;
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::entity_type_data::get_description(
    uint64_t value,
    std::string &description)
{
    std::map<uint64_t, std::string>::const_iterator
        search_itor = descriptions.find(value);

    if (search_itor != descriptions.end())
    {
        description = search_itor->second;
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
void vdb::entity_type_data::add(
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
    uint64_t
        value = 0;

    convert_to_value(
        kind,
        domain,
        country,
        category,
        subcategory,
        specific,
        extra,
        value);

    if (name_ptr)
    {
        names[value] = name_ptr;
    }

    if (description_ptr)
    {
        descriptions[value] = description_ptr;
    }
}
