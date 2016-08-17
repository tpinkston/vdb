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
#include "vdb_enums.h"
#include "vdb_hexadecimal.h"
#include "vdb_marking.h"
#include "vdb_string.h"

namespace vdb
{
    const uint32_t
        entity_marking_t::LENGTH = 12;
}

// ----------------------------------------------------------------------------
vdb::entity_marking_t::entity_marking_t(void) :
    character_set(0)
{
    std::memset(characters, 0, ENTITY_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
vdb::entity_marking_t::entity_marking_t(
    const entity_marking_t &copy
) :
    character_set(copy.character_set)
{
    std::memcpy(characters, copy.characters, ENTITY_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
vdb::entity_marking_t::~entity_marking_t(void)
{

}

// ----------------------------------------------------------------------------
std::string vdb::entity_marking_t::hex_characters(void) const
{
    std::ostringstream
        stream;

    for(unsigned i = 0; i < ENTITY_MARKING_CHARACTERS; ++i)
    {
        stream << hexadecimal::str((uint8_t)characters[i]);
    }

    return stream.str();
}

// ----------------------------------------------------------------------------
std::string vdb::entity_marking_t::ascii_characters(void) const
{
    std::ostringstream
        stream;
    bool
        done = false;

    for(unsigned i = 0; (i < ENTITY_MARKING_CHARACTERS) and not done; ++i)
    {
        uint8_t value = characters[i];

        if (value == 0)
        {
            done = true;
        }
        else if ((value > 31) and (value < 127))
        {
            stream << (char)value;
        }
        else
        {
            stream << "\\x" << hexadecimal::str(value);
        }
    }

    return trim(stream.str());
}

// ----------------------------------------------------------------------------
void vdb::entity_marking_t::clear(void)
{
    character_set = 0;

    std::memset(characters, 0, ENTITY_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
void vdb::entity_marking_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix;

    switch((entity_marking_e)character_set)
    {
        case ENTITY_MARKING_ASCII:
            stream << "'" << ascii_characters() << "'";
            break;
        default:
            stream << hex_characters()
                   << " [" << (entity_marking_e)character_set << "]";
            break;
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_marking_t::read(byte_stream &stream)
{
    clear();

    stream.read(character_set);

    for(unsigned i = 0; i < ENTITY_MARKING_CHARACTERS; ++i)
    {
        int8_t
            character = 0;

        stream.read(character);

        characters[i] = (char)character;
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_marking_t::write(byte_stream &stream) const
{
    stream.write(character_set);

    for(unsigned i = 0; i < ENTITY_MARKING_CHARACTERS; ++i)
    {
        stream.write((int8_t)characters[i]);
    }
}

// ----------------------------------------------------------------------------
vdb::entity_marking_t &vdb::entity_marking_t::operator=(
    const entity_marking_t &other)
{
    character_set = other.character_set;

    std::memcpy(
        characters,
        other.characters,
        ENTITY_MARKING_CHARACTERS);

    return *this;
}

// ----------------------------------------------------------------------------
bool vdb::entity_marking_t::operator==(const entity_marking_t &other) const
{
    if (character_set == other.character_set)
    {
        int comparison = std::memcmp(
            characters,
            other.characters,
            ENTITY_MARKING_CHARACTERS);

        return (comparison == 0);
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::entity_marking_t::operator!=(const entity_marking_t &other) const
{
    return not (*this == other);
}

// ----------------------------------------------------------------------------
bool vdb::entity_marking_t::operator<(const entity_marking_t &other) const
{
    if (character_set < other.character_set)
    {
        return true;
    }
    else
    {
        int comparison = std::memcmp(
            characters,
            other.characters,
            ENTITY_MARKING_CHARACTERS);

        return (comparison < 0);
    }
}

// ----------------------------------------------------------------------------
bool vdb::entity_marking_t::operator>(const entity_marking_t &other) const
{
    if (character_set < other.character_set)
    {
        return true;
    }
    else
    {
        int comparison = std::memcmp(
            characters,
            other.characters,
            ENTITY_MARKING_CHARACTERS);

        return (comparison > 0);
    }
}
