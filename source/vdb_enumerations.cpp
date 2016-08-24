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

#include "vdb_enumerations.h"
#include "vdb_logger.h"
#include "vdb_string.h"

namespace
{
    // ------------------------------------------------------------------------
    std::string parse_name(std::string enum_name, std::string value_name)
    {
        if (vdb::starts_with(value_name, (enum_name + "_")))
        {
            return value_name.substr(enum_name.length() + 1);
        }

        return value_name;
    }
}

namespace vdb
{
    enumerations::enum_definitions_t
        enumerations::definitions;
    bool
        enumerations::loaded = false;
}

// ----------------------------------------------------------------------------
void vdb::enumerations::print(std::ostream &stream)
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
void vdb::enumerations::load(void)
{
    if (not loaded)
    {
        add_all();
        loaded = true;

        LOG_VERBOSE("Loaded %d enumerations...", definitions.size());
    }
}

// ----------------------------------------------------------------------------
bool vdb::enumerations::valid(const std::string &enumeration, int value)
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
std::string vdb::enumerations::print(
    const std::string &enumeration,
    int value)
{
    std::ostringstream
        stream;
    std::string
        string = get_name(enumeration, value);

    stream << std::hex << value;

    string += " [";
    string += generic_to_string(value);
    string += " (0x";
    string += to_uppercase(stream.str());
    string += ")]";

    return string;
}

// ----------------------------------------------------------------------------
std::string vdb::enumerations::get_name(
    const std::string &enumeration,
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

    return (enumeration + "_" + generic_to_string(value));
}

// ----------------------------------------------------------------------------
void vdb::enumerations::add(
    const char *enum_ptr,
    int value,
    const char *name_ptr)
{
    const std::string
        enumeration = enum_ptr;
    enum_entries_t
        &entries = definitions[enumeration];

    entries[value] = parse_name(enum_ptr, name_ptr);
}
