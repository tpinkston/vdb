#include "vdis_enumerations.h"
#include "vdis_integer.h"
#include "vdis_string.h"

namespace
{
    // ------------------------------------------------------------------------
    string_t parse_name(std::string enum_name, std::string value_name)
    {
        if (vdis::starts_with(value_name, (enum_name + "_")))
        {
            return value_name.substr(enum_name.length() + 1);
        }

        return value_name;
    }
}

namespace vdis
{
    enumerations::enum_definitions_t
        enumerations::definitions;
    bool
        enumerations::loaded = false;
}

// ----------------------------------------------------------------------------
void vdis::enumerations::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;
    }
}

// ----------------------------------------------------------------------------
void vdis::enumerations::print(std::ostream &stream)
{
    enum_definitions_t::const_iterator
        definitions_itor = definitions.begin();

    while(definitions_itor != definitions.end())
    {
        enum_entries_t::const_iterator
            entries_itor = definitions_itor->second.begin();

        while(entries_itor != definitions_itor->second.end())
        {
            stream << definitions_itor->first << ","
                   << entries_itor->first << ","
                   << entries_itor->second << std::endl;

            ++entries_itor;
        }

        ++definitions_itor;
    }
}

// ----------------------------------------------------------------------------
bool vdis::enumerations::valid(const string_t &enumeration, int value)
{
    enum_definitions_t::const_iterator
        definition_itor = definitions.find(enumeration);

    if (definition_itor != definitions.end())
    {
        if (definition_itor->second.find(value) !=
            definition_itor->second.end())
        {
            return true;
        }
    }

    return false;
}

// ----------------------------------------------------------------------------
std::ostream &vdis::enumerations::print(
    const string_t &enumeration,
    int value,
    std::ostream &out)
{
    std::ostringstream
        stream;

    stream << std::hex << value;

    out << get_name(enumeration, value)
        << "(" << value << "[0x" << vdis::to_uppercase(stream.str()) << "])";

    return out;
}

// ----------------------------------------------------------------------------
string_t vdis::enumerations::get_name(
    const string_t &enumeration,
    int value)
{
    enum_definitions_t::const_iterator
        definition_itor = definitions.find(enumeration);

    if (definition_itor != definitions.end())
    {
        enum_entries_t::const_iterator
            entry_itor = definition_itor->second.find(value);

        if (entry_itor != definition_itor->second.end())
        {
            return entry_itor->second;
        }
    }

    return (enumeration + "_" + to_string(value));
}

// ----------------------------------------------------------------------------
void vdis::enumerations::add(
    const char *enum_ptr,
    int value,
    const char *name_ptr)
{
    const string_t
        enumeration = enum_ptr;
    enum_entries_t
        &entries = definitions[enumeration];

    entries[value] = parse_name(enum_ptr, name_ptr);
}
