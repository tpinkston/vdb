#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_entity_types.h"
#include "vdis_string.h"

namespace
{
    const float32_t
        TIME_UNIT_TO_SECONDS = (3600.0 / 2147483647.0),
        SECONDS_TO_TIME_UNITS = (1.0 / TIME_UNIT_TO_SECONDS);
}

// ----------------------------------------------------------------------------
uint64_t vdis::convert(uint16_t v1, uint16_t v2, uint16_t v3)
{
    uint64_t value = 0;

    value = (uint16_t)(v1 & 0xFFFF);
    value <<= 16;
    value |= (uint16_t)(v2 & 0xFFFF);
    value <<= 16;
    value |= (uint16_t)(v3 & 0xFFFF);

    return value;
}

// ----------------------------------------------------------------------------
void vdis::convert(uint64_t v, uint16_t &v1, uint16_t &v2, uint16_t &v3)
{
    uint64_t value = v;

    v3 = (uint16_t)(value & 0xFFFF);
    value >>= 16;
    v2 = (uint16_t)(value & 0xFFFF);
    value >>= 16;
    v1 = (uint16_t)(value & 0xFFFF);
}

// ----------------------------------------------------------------------------
void vdis::simulation_id_t::read(byte_stream_t &stream)
{
    stream.read(site);
    stream.read(application);
}

// ----------------------------------------------------------------------------
void vdis::simulation_id_t::write(byte_stream_t &stream)
{
    stream.write(site);
    stream.write(application);
}

// ----------------------------------------------------------------------------
void vdis::entity_id_t::read(byte_stream_t &stream)
{
    stream.read(site);
    stream.read(application);
    stream.read(entity);
}

// ----------------------------------------------------------------------------
void vdis::entity_id_t::write(byte_stream_t &stream)
{
    stream.write(site);
    stream.write(application);
    stream.write(entity);
}

// ----------------------------------------------------------------------------
void vdis::event_id_t::read(byte_stream_t &stream)
{
    stream.read(site);
    stream.read(application);
    stream.read(event);
}

// ----------------------------------------------------------------------------
void vdis::event_id_t::write(byte_stream_t &stream)
{
    stream.write(site);
    stream.write(application);
    stream.write(event);
}

// ----------------------------------------------------------------------------
void vdis::object_id_t::read(byte_stream_t &stream)
{
    stream.read(site);
    stream.read(application);
    stream.read(object);
}

// ----------------------------------------------------------------------------
void vdis::object_id_t::write(byte_stream_t &stream)
{
    stream.write(site);
    stream.write(application);
    stream.write(object);
}

// ----------------------------------------------------------------------------
void vdis::entity_type_t::set(uint64_t value)
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
uint64_t vdis::entity_type_t::get(void) const
{
    uint64_t value = 0;

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

    return value;
}

// ----------------------------------------------------------------------------
std::string vdis::entity_type_t::description(void) const
{
    std::ostringstream
        stream;
    std::string
        description = entity_types::get_description(get());

    if (description.empty())
    {
        entity_type_t
            parent;

        if (entity_types::get_valid_parent(*this, parent))
        {
            description = entity_types::get_description(parent.get());
            description += " (Parent)";
        }
        else
        {
            description = "(Undefined)";
        }
    }

    stream << *this << " '" << description << "'";

    return stream.str();
}

// ----------------------------------------------------------------------------
void vdis::entity_type_t::read(byte_stream_t &stream)
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
void vdis::entity_type_t::write(byte_stream_t &stream)
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
void vdis::object_type_t::set(uint32_t value)
{
    uint32_t bits = value;

    subcategory = (uint8_t)(bits & 0xFF);
    bits >>= 8;
    category = (uint8_t)(bits & 0xFF);
    bits >>= 8;
    kind = (uint8_t)(bits & 0xFF);
    bits >>= 8;

    domain = (uint8_t)(bits & 0xFF);
}

// ----------------------------------------------------------------------------
uint32_t vdis::object_type_t::get(void) const
{
    uint32_t value = 0;

    value = (uint8_t)(domain & 0xFF);
    value <<= 8;
    value |= (uint8_t)(kind & 0xFF);
    value <<= 8;
    value |= (uint8_t)(category & 0xFF);
    value <<= 8;
    value |= (uint8_t)(subcategory & 0xFF);

    return value;
}

// ----------------------------------------------------------------------------
void vdis::object_type_t::read(byte_stream_t &stream)
{
    stream.read(domain);
    stream.read(kind);
    stream.read(category);
    stream.read(subcategory);
}

// ----------------------------------------------------------------------------
void vdis::object_type_t::write(byte_stream_t &stream)
{
    stream.write(domain);
    stream.write(kind);
    stream.write(category);
    stream.write(subcategory);
}

// ----------------------------------------------------------------------------
std::string vdis::entity_marking_t::hex_characters(void) const
{
    std::ostringstream
        stream;

    for(unsigned i = 0; i < VDIS_MARKING_CHARACTERS; ++i)
    {
        stream << to_hex_string((uint8_t)characters[i]);
    }

    return stream.str();
}

// ----------------------------------------------------------------------------
std::string vdis::entity_marking_t::ascii_characters(void) const
{
    std::ostringstream
        stream;
    uint32_t
        sum = 0;
    bool
        done = false;

    for(unsigned i = 0; i < VDIS_MARKING_CHARACTERS; ++i)
    {
        sum += (uint32_t)characters[i];
    }

    if (sum == 0)
    {
        stream << "(NULL)";
    }
    else for(unsigned i = 0; (i < VDIS_MARKING_CHARACTERS) and not done; ++i)
    {
        uint8_t value = characters[i];

        // Some systems will ignore specifications and use all 12 bytes
        // as ASCII characters.
        //
        if ((i == 0) and (character_set > 31) and (character_set < 127))
        {
            stream << (char)character_set;
        }

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
            stream << "\\x" << to_hex_string(value);
        }
    }

    return trim(stream.str());
}

// ----------------------------------------------------------------------------
void vdis::entity_marking_t::clear(void)
{
    character_set = 0;
    clear_memory(characters, VDIS_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
void vdis::entity_marking_t::read(byte_stream_t &stream)
{
    stream.read(character_set);
    stream.read(characters, VDIS_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
void vdis::entity_marking_t::write(byte_stream_t &stream)
{
    stream.write(character_set);
    stream.write(characters, VDIS_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
void vdis::entity_capabilities_t::clear(void)
{
    value = 0;
}

// ----------------------------------------------------------------------------
void vdis::entity_capabilities_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "capabilities.value "
        << to_bin_string(value, true) << std::endl;

    if (bits.ammunition_supply)
    {
        out << prefix << "capabilities.ammunition_supply yes" << std::endl;
    }

    if (bits.fuel_supply)
    {
        out << prefix << "capabilities.fuel_supply yes" << std::endl;
    }

    if (bits.recovery)
    {
        out << prefix << "capabilities.recovery yes" << std::endl;
    }

    if (bits.repair)
    {
        out << prefix << "capabilities.repair yes" << std::endl;
    }

    if (bits.ads_broadcast)
    {
        out << prefix << "capabilities.ads_broadcast yes" << std::endl;
    }

    if (bits.sling_load_carrier)
    {
        out << prefix << "capabilities.sling_load_carrier yes" << std::endl;
    }

    if (bits.sling_loadable)
    {
        out << prefix << "capabilities.sling_loadable yes" << std::endl;
    }

    if (bits.ied_presence)
    {
        out << prefix << "capabilities.ied_presence yes" << std::endl;
    }

    if (bits.task_organizable)
    {
        out << prefix << "capabilities.task_organizable yes" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::entity_capabilities_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::entity_capabilities_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::location12_t::read(byte_stream_t &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdis::location12_t::write(byte_stream_t &stream)
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
}

// ----------------------------------------------------------------------------
void vdis::location24_t::read(byte_stream_t &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdis::location24_t::write(byte_stream_t &stream)
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
}

// ----------------------------------------------------------------------------
void vdis::orientation_t::read(byte_stream_t &stream)
{
    stream.read(psi);
    stream.read(theta);
    stream.read(phi);
}

// ----------------------------------------------------------------------------
void vdis::orientation_t::write(byte_stream_t &stream)
{
    stream.write(psi);
    stream.write(theta);
    stream.write(phi);
}

// ----------------------------------------------------------------------------
void vdis::velocity_t::read(byte_stream_t &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdis::velocity_t::write(byte_stream_t &stream)
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
}

// ----------------------------------------------------------------------------
void vdis::dead_reckoning_t::clear(void)
{
    algorithm = 0;
    clear_memory(parameters, VDIS_DR_PARAMETERS_SIZE);
    linear_acceleration.clear();
    angular_velocity.clear();
}

// ----------------------------------------------------------------------------
void vdis::dead_reckoning_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "dead_reckoning.algorithm "
        << algorithm_enum() << std::endl
        << prefix << "dead_reckoning.linear_acceleration "
        << linear_acceleration << std::endl
        << prefix << "dead_reckoning.angular_velocity "
        << angular_velocity << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::dead_reckoning_t::read(byte_stream_t &stream)
{
    stream.read(algorithm);
    stream.read(parameters, VDIS_DR_PARAMETERS_SIZE);
    linear_acceleration.read(stream);
    angular_velocity.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::dead_reckoning_t::write(byte_stream_t &stream)
{
    stream.write(algorithm);
    stream.write(parameters, VDIS_DR_PARAMETERS_SIZE);
    linear_acceleration.write(stream);
    angular_velocity.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::set(
    uint32_t minutes,
    float32_t seconds,
    bool absolute)
{
    float64_t
        time = (seconds + ((float64_t)(minutes) * 60.0f));

    value = (uint32_t)(time * SECONDS_TO_TIME_UNITS);
    value = (value << 1);

    // Set the absolute bit.
    //
    if (absolute)
    {
        value |= 0x01;
    }
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::get(
    uint32_t &minutes,
    float32_t &seconds,
    bool &absolute) const
{
    uint32_t
        temp_value = value;
    float64_t
        time = 0.0;

    absolute = ((value & 0x01) > 0);
    temp_value = ((value >> 1) & 0x7FFFFFFF);

    time = (TIME_UNIT_TO_SECONDS * (float64_t)temp_value);
    minutes = (uint32_t)(time / 60.0);
    time -= ((float32_t)(minutes) * 60.0);
    seconds = time;
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::clocktime_t::read(byte_stream_t &stream)
{
    stream.read(hours);
    offset.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::clocktime_t::write(byte_stream_t &stream)
{
    stream.write(hours);
    offset.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::pdu_header_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "protocol " << protocol_enum() << std::endl
        << prefix << "exercise " << (int)exercise << std::endl
        << prefix << "type " << type_enum() << std::endl
        << prefix << "family " << family_enum() << std::endl
        << prefix << "timestamp " << timestamp << std::endl
        << prefix << "length " << (int)length << std::endl
        << prefix << "status " << (int)status << std::endl
        << prefix << "padding " << (int)padding << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::pdu_header_t::read(byte_stream_t &stream)
{
    stream.read(protocol);
    stream.read(exercise);
    stream.read(type);
    stream.read(family);
    timestamp.read(stream);
    stream.read(length);
    stream.read(status);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::pdu_header_t::write(byte_stream_t &stream)
{
    stream.write(protocol);
    stream.write(exercise);
    stream.write(type);
    stream.write(family);
    timestamp.write(stream);
    stream.write(length);
    stream.write(status);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::sling_line_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << to_string(line_length, 1, 2) << std::endl
        << prefix << "offset " << to_string(line_offset, 1, 2) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::sling_line_t::read(byte_stream_t &stream)
{
    stream.read(line_length);
    stream.read(line_offset);
}

// ----------------------------------------------------------------------------
void vdis::sling_line_t::write(byte_stream_t &stream)
{
    stream.write(line_length);
    stream.write(line_offset);
}

// ----------------------------------------------------------------------------
void vdis::burst_descriptor_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "munition " << munition.description() << std::endl
        << prefix << "warhead " << warhead_enum() << std::endl
        << prefix << "fuse " << fuse_enum() << std::endl
        << prefix << "quantity " << (int)quantity << std::endl
        << prefix << "rate " << (int)rate << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::burst_descriptor_t::read(byte_stream_t &stream)
{
    munition.read(stream);
    stream.read(warhead);
    stream.read(fuse);
    stream.read(quantity);
    stream.read(rate);
}

// ----------------------------------------------------------------------------
void vdis::burst_descriptor_t::write(byte_stream_t &stream)
{
    munition.write(stream);
    stream.write(warhead);
    stream.write(fuse);
    stream.write(quantity);
    stream.write(rate);
}

// ----------------------------------------------------------------------------
void vdis::modulation_type_t::read(byte_stream_t &stream)
{
    stream.read(spread_spectrum);
    stream.read(major_modulation);
    stream.read(modulation_detail);
    stream.read(radio_system);
}

// ----------------------------------------------------------------------------
void vdis::modulation_type_t::write(byte_stream_t &stream)
{
    stream.write(spread_spectrum);
    stream.write(major_modulation);
    stream.write(modulation_detail);
    stream.write(radio_system);
}

// ----------------------------------------------------------------------------
void vdis::exercise_state_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "id " << (int)id << std::endl
        << prefix << "transition " << (int)transition << std::endl
        << prefix << "current_state " << (int)current_state << std::endl
        << prefix << "requested_state " << (int)requested_state << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::exercise_state_t::read(byte_stream_t &stream)
{
    stream.read(id);
    stream.read(transition);
    stream.read(current_state);
    stream.read(requested_state);
}

// ----------------------------------------------------------------------------
void vdis::exercise_state_t::write(byte_stream_t &stream)
{
    stream.write(id);
    stream.write(transition);
    stream.write(current_state);
    stream.write(requested_state);
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::simulation_id_t &o)
{
    out << (int)o.site << "." << (int)o.application;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::entity_id_t &o)
{
    out << (int)o.site << "."
        << (int)o.application << "."
        << (int)o.entity;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::event_id_t &o)
{
    out << (int)o.site << "."
        << (int)o.application << "."
        << (int)o.event;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::object_id_t &o)
{
    out << (int)o.site << "."
        << (int)o.application << "."
        << (int)o.object;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::entity_type_t &o)
{
    out << (int)o.kind << "."
        << (int)o.domain << "."
        << (int)o.country << "."
        << (int)o.category << "."
        << (int)o.subcategory << "."
        << (int)o.specific << "."
        << (int)o.extra;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::object_type_t &o)
{
    out << (int)o.domain << "."
        << (int)o.kind << "."
        << (int)o.category << "."
        << (int)o.subcategory;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::entity_marking_t &o)
{
    switch((vdis::entity_marking_e)o.character_set)
    {
        case vdis::ENTITY_MARKING_ASCII:
            out << "'" << o.ascii_characters() << "'";
            break;
        default:
            out << o.hex_characters()
                << " " << (vdis::entity_marking_e)o.character_set;
            break;
    }

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::location12_t &o)
{
    out << vdis::to_string(o.x) << ", "
        << vdis::to_string(o.y) << ", "
        << vdis::to_string(o.z);

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::location24_t &o)
{
    out << vdis::to_string(o.x) << ", "
        << vdis::to_string(o.y) << ", "
        << vdis::to_string(o.z);

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::orientation_t &o)
{
    out << vdis::to_string(o.psi) << " (psi), "
        << vdis::to_string(o.theta) << " (theta), "
        << vdis::to_string(o.phi) << " (phi)";

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::velocity_t &o)
{
    out << vdis::to_string(o.x) << ", "
        << vdis::to_string(o.y) << ", "
        << vdis::to_string(o.z);

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::timestamp_t &o)
{
    uint32_t
        minutes;
    float32_t
        seconds;
    bool
        absolute;

    o.get(minutes, seconds, absolute);

    out << minutes << "m " << seconds << "s " << (absolute ? "ABS" : "REL");

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::clocktime_t &o)
{
    out << o.hours << "h " << o.offset;

    return out;
}
