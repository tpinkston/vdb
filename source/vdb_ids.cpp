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

#include "vdb_byte_stream.h"
#include "vdb_ids.h"
#include "vdb_logger.h"
#include "vdb_marking.h"
#include "vdb_options.h"

namespace vdb
{
    const uint32_t
        simulation_id_t::LENGTH = 4,
        id_t::LENGTH = 6;
    std::map<id_t, entity_marking_t>
        id_mappings::entity_markings;
}

// ----------------------------------------------------------------------------
vdb::simulation_id_t::simulation_id_t(void) : site(0), application(0)
{

}

// ----------------------------------------------------------------------------
vdb::simulation_id_t::simulation_id_t(uint16_t site, uint16_t application) :
    site(site),
    application(application)
{

}

// ----------------------------------------------------------------------------
vdb::simulation_id_t::simulation_id_t(const simulation_id_t &copy) :
    site(copy.site),
    application(copy.application)
{

}

// ----------------------------------------------------------------------------
vdb::simulation_id_t::~simulation_id_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::is_none(void) const
{
    return (site == NONE) and (application == NONE);
}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::is_all(void) const
{
    return (site == ALL) and (application == ALL);
}

// ----------------------------------------------------------------------------
void vdb::simulation_id_t::clear(void)
{
    site = 0;
    application = 0;
}

// ----------------------------------------------------------------------------
void vdb::simulation_id_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << (int)site << "." << (int)application;
}

// ----------------------------------------------------------------------------
void vdb::simulation_id_t::read(byte_stream &stream)
{
    stream.read(site);
    stream.read(application);
}

// ----------------------------------------------------------------------------
void vdb::simulation_id_t::write(byte_stream &stream) const
{
    stream.write(site);
    stream.write(application);
}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::operator==(const simulation_id_t &other) const
{
    return (site == other.site) and
           (application == other.application);
}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::operator!=(const simulation_id_t &other) const
{
    return not (*this == other);
}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::operator<(const simulation_id_t &other) const
{
    if (site < other.site)
    {
        return true;
    }
    else if (site > other.site)
    {
        return false;
    }
    else if (application < other.application)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::operator>(const simulation_id_t &other) const
{
    if (site > other.site)
    {
        return true;
    }
    else if (site < other.site)
    {
        return false;
    }
    else if (application > other.application)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
bool vdb::simulation_id_t::matches(const simulation_id_t &id) const
{
    bool match = true;

    if (id.site < ALL)
    {
        match = (site == id.site);
    }

    if (match and (id.application < ALL))
    {
        match = (application == id.application);
    }

    return match;
}

// ----------------------------------------------------------------------------
vdb::id_t::id_t(void) : simulation_id_t(0, 0), identity(0)
{

}

// ----------------------------------------------------------------------------
vdb::id_t::id_t(uint16_t site, uint16_t application, uint16_t identity) :
    simulation_id_t(site, application),
    identity(identity)
{

}

// ----------------------------------------------------------------------------
vdb::id_t::id_t(const id_t &copy) :
    simulation_id_t(copy),
    identity(copy.identity)
{

}

// ----------------------------------------------------------------------------
vdb::id_t::~id_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::id_t::is_none(void) const
{
    return (site == NONE) and (application == NONE) and (identity == NONE);
}

// ----------------------------------------------------------------------------
bool vdb::id_t::is_all(void) const
{
    return (site == ALL) and (application == ALL) and (identity == ALL);
}

// ----------------------------------------------------------------------------
void vdb::id_t::clear(void)
{
    site = 0;
    application = 0;
    identity = 0;
}

// ----------------------------------------------------------------------------
void vdb::id_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix
           << (int)site << "."
           << (int)application << "."
           << (int)identity;
}

// ----------------------------------------------------------------------------
void vdb::id_t::print_full(std::ostream &stream) const
{
    const entity_marking_t
        *marking_ptr = id_mappings::get_marking(*this);

    print(std::string(), stream);

    stream << " ";

    if (marking_ptr != 0)
    {
        stream << *marking_ptr;
    }
    else if (is_none())
    {
        stream << "[NO ENTITY]";
    }
    else if (is_all())
    {
        stream << "[ALL ENTITIES]";
    }
    else
    {
        stream << "[UNKNOWN]";
    }
}

// ----------------------------------------------------------------------------
void vdb::id_t::read(byte_stream &stream)
{
    stream.read(site);
    stream.read(application);
    stream.read(identity);
}

// ----------------------------------------------------------------------------
void vdb::id_t::write(byte_stream &stream) const
{
    stream.write(site);
    stream.write(application);
    stream.write(identity);
}

// ----------------------------------------------------------------------------
bool vdb::id_t::operator==(const id_t &other) const
{
    return (site == other.site) and
           (application == other.application) and
           (identity == other.identity);
}

// ----------------------------------------------------------------------------
bool vdb::id_t::operator!=(const id_t &other) const
{
    return not (*this == other);
}

// ----------------------------------------------------------------------------
bool vdb::id_t::operator<(const id_t &other) const
{
    if (site < other.site)
    {
        return true;
    }
    else if (site > other.site)
    {
        return false;
    }
    else if (application < other.application)
    {
        return true;
    }
    else if (application > other.application)
    {
        return false;
    }
    else if (identity < other.identity)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
bool vdb::id_t::operator>(const id_t &other) const
{
    if (site > other.site)
    {
        return true;
    }
    else if (site < other.site)
    {
        return false;
    }
    else if (application > other.application)
    {
        return true;
    }
    else if (application < other.application)
    {
        return false;
    }
    else if (identity > other.identity)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
bool vdb::id_t::matches(const id_t &id) const
{
    bool match = simulation_id_t::matches(id);

    if (match and (id.identity < ALL))
    {
        match = (identity == id.identity);
    }

    return match;
}

// ----------------------------------------------------------------------------
const vdb::entity_marking_t *vdb::id_mappings::get_marking(const id_t &id)
{
    std::map<id_t, entity_marking_t>::iterator
        search_itor = entity_markings.find(id);

    return (search_itor != entity_markings.end()) ? &search_itor->second : 0;
}

// ----------------------------------------------------------------------------
void vdb::id_mappings::set_marking(
    const id_t &id,
    const entity_marking_t &value)
{
    std::map<id_t, entity_marking_t>::iterator
        search_itor = entity_markings.find(id);

    if (search_itor == entity_markings.end())
    {
        if (options::flag(OPT_VERBOSE))
        {
            LOG_VERBOSE(
                "Adding marking %s for entity %s",
                value.c_str(),
                id.c_str());
        }

        entity_markings[id] = value;
    }
    else if (search_itor->second != value)
    {
        if (options::flag(OPT_VERBOSE))
        {
            LOG_VERBOSE(
                "Overwriting marking %s for entity %s",
                value.c_str(),
                id.c_str());
        }

        search_itor->second = value;
    }
}
