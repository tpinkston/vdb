#include "vdis_appearance.h"
#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const float32_t
        TIME_UNIT_TO_SECONDS = (3600.0 / 2147483647.0),
        SECONDS_TO_TIME_UNITS = (1.0 / TIME_UNIT_TO_SECONDS);
    const vdis::object_type_t
        *object_type_ptr = 0;
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
bool vdis::id_t::is_none(void) const
{
    return (site == NONE) and (application == NONE) and (identity == NONE);
}

// ----------------------------------------------------------------------------
bool vdis::id_t::is_all(void) const
{
    return (site == ALL) and (application == ALL) and (identity == ALL);
}

// ----------------------------------------------------------------------------
void vdis::id_t::set(uint64_t value)
{
    uint64_t bits = value;

    identity = (uint16_t)(bits & 0xFFFF);
    bits >>= 16;
    application = (uint16_t)(bits & 0xFFFF);
    bits >>= 16;
    site = (uint16_t)(bits & 0xFFFF);
}

// ----------------------------------------------------------------------------
uint64_t vdis::id_t::get(void) const
{
    uint64_t value = 0;

    value = (uint16_t)(site & 0xFFFF);
    value <<= 16;
    value |= (uint16_t)(application & 0xFFFF);
    value <<= 16;
    value |= (uint16_t)(identity & 0xFFFF);

    return value;
}

// ----------------------------------------------------------------------------
bool vdis::id_t::matches(const id_t &other) const
{
    bool match = not is_all();

    if (match and (site != ALL) and (other.site != ALL))
    {
        match = (site == other.site);
    }

    if (match and (application != ALL) and (other.application != ALL))
    {
        match = (application == other.application);
    }

    if (match and (identity != ALL) and (other.identity != ALL))
    {
        match = (identity == other.identity);
    }

    return match;
}

// ----------------------------------------------------------------------------
bool vdis::id_t::operator==(const id_t &other) const
{
    return (site == other.site) and
           (application == other.application) and
           (identity == other.identity);
}

// ----------------------------------------------------------------------------
bool vdis::id_t::operator!=(const id_t &other) const
{
    return not (*this == other);
}

// ----------------------------------------------------------------------------
bool vdis::id_t::operator<(const id_t &other) const
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
    else if (identity > other.identity)
    {
        return false;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
bool vdis::id_t::operator>(const id_t &other) const
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
    else if (identity < other.identity)
    {
        return false;
    }
    else
    {
        return false;
    }
}

// ----------------------------------------------------------------------------
void vdis::id_t::read(byte_stream_t &stream)
{
    stream.read(site);
    stream.read(application);
    stream.read(identity);
}

// ----------------------------------------------------------------------------
void vdis::id_t::write(byte_stream_t &stream)
{
    stream.write(site);
    stream.write(application);
    stream.write(identity);
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
    std::string
        description;;

    if (not entity_types::get_description(get(), description))
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

    return description;
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
std::string vdis::object_type_t::description(object_geometry_e geometry) const
{
    std::string
        description = object_types::get_description(geometry, get());

    if (description.empty())
    {
        object_type_t
            parent;

        if (object_types::get_valid_parent(geometry, *this, parent))
        {
            description = object_types::get_description(geometry, parent.get());
            description += " (Parent)";
        }
        else
        {
            description = "(Undefined)";
        }
    }

    return description;
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
void vdis::marking_t::str(const std::string &name)
{
    character_set = ENTITY_MARKING_ASCII;

    for(unsigned i = 0; i < 11; ++i)
    {
        characters[i] = ((i < name.length()) ? (uint8_t)name[i] : 0);
    }
}

// ----------------------------------------------------------------------------
std::string vdis::marking_t::str(void) const
{
    const bool
        ASCII = is_printable_character(character_set);
    std::string
        string;
    uint32_t
        sum = 0;

    for(unsigned i = 0; i < 11; ++i)
    {
        sum += (uint32_t)characters[i];
    }

    if (sum == 0)
    {
        string = "(NULL)";
    }
    else if (ASCII or (character_set == ENTITY_MARKING_ASCII))
    {
        std::ostringstream
            stream;
        bool
            done = false;

        for(unsigned i = 0; (i < 11) and not done; ++i)
        {
            uint8_t value = characters[i];

            // Some systems will ignore specifications and use all 12 bytes
            // as ASCII characters.
            //
            if ((i == 0) and ASCII)
            {
                stream << (char)character_set;
            }

            if (value == 0)
            {
                done = true;
            }
            else if (is_printable_character(value))
            {
                stream << (char)value;
            }
            else
            {
                stream << "x" << to_hex_string(value);
            }
        }

        string = ("'" + trim(stream.str()) + "'");
    }
    else
    {
        std::ostringstream
            stream;

        for(unsigned i = 0; i < 11; ++i)
        {
            stream << to_hex_string((uint8_t)characters[i]);
        }

        string = stream.str();
    }

    return string;
}

// ----------------------------------------------------------------------------
void vdis::marking_t::clear(void)
{
    character_set = 0;
    clear_memory(characters, 11);
}

// ----------------------------------------------------------------------------
bool vdis::marking_t::operator==(const marking_t &other) const
{
    return (std::memcmp(this, &other, 12) == 0);
}

// ----------------------------------------------------------------------------
bool vdis::marking_t::operator!=(const marking_t &other) const
{
    return (std::memcmp(this, &other, 12) != 0);
}

// ----------------------------------------------------------------------------
bool vdis::marking_t::operator<(const marking_t &other) const
{
    return (std::memcmp(this, &other, 12) < 0);
}

// ----------------------------------------------------------------------------
bool vdis::marking_t::operator>(const marking_t &other) const
{
    return (std::memcmp(this, &other, 12) > 0);
}

// ----------------------------------------------------------------------------
void vdis::marking_t::read(byte_stream_t &stream)
{
    stream.read(character_set);
    stream.read(characters, 11);
}

// ----------------------------------------------------------------------------
void vdis::marking_t::write(byte_stream_t &stream)
{
    stream.write(character_set);
    stream.write(characters, 11);
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
void vdis::offset12_t::read(byte_stream_t &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdis::offset12_t::write(byte_stream_t &stream)
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
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
void vdis::geodetic_location_t::read(byte_stream_t &stream)
{
    stream.read(latitude);
    stream.read(longitude);
    stream.read(elevation);
}

// ----------------------------------------------------------------------------
void vdis::geodetic_location_t::write(byte_stream_t &stream)
{
    stream.write(latitude);
    stream.write(longitude);
    stream.write(elevation);
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
    clear_memory(parameters, 15);
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
    stream.read(parameters, 15);
    linear_acceleration.read(stream);
    angular_velocity.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::dead_reckoning_t::write(byte_stream_t &stream)
{
    stream.write(algorithm);
    stream.write(parameters, 15);
    linear_acceleration.write(stream);
    angular_velocity.write(stream);
}

// ----------------------------------------------------------------------------
uint64_t vdis::timestamp_t::get_milliseconds(void) const
{
    float64_t time;

    time = (TIME_UNIT_TO_SECONDS * (float64_t)((value >> 1) & 0x7FFFFFFF));
    time = (time * 1000.0);

    return (uint64_t)time;
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::set_milliseconds(uint64_t millis)
{
    const bool
        absolute = is_absolute();
    float32_t
        time = (float64_t)((float32_t)millis / 1000.0);

    LOG_VERBOSE("set_milliseconds() millis = %d", millis);
    LOG_VERBOSE("set_milliseconds() time = %f", time);

    value = (uint32_t)(time * SECONDS_TO_TIME_UNITS);
    value = (value << 1);

    LOG_VERBOSE("set_milliseconds() value = %d", value);

    set_absolute(absolute);
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::add_milliseconds(uint64_t millis)
{
    set_milliseconds(get_milliseconds() + millis);
}

// ----------------------------------------------------------------------------
bool vdis::timestamp_t::is_absolute(void) const
{
    return ((value & 0x01) > 0);
}

// ----------------------------------------------------------------------------
void vdis::timestamp_t::set_absolute(bool absolute)
{
    LOG_VERBOSE("set_absolute() value before = %d", value);

    if (absolute)
    {
        value = (value | 0x01);
    }
    else
    {
        value = ((uint64_t)value & 0xFFFFFFFELU);
    }

    LOG_VERBOSE("set_absolute() value after = %d", value);
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
bool vdis::pdu_header_t::is_valid(void) const
{
	return (get_family((pdu_type_e)type) == (pdu_family_e)family);
}

// ----------------------------------------------------------------------------
void vdis::pdu_header_t::reset(pdu_type_e pdu_type)
{
	clear();

	protocol = (uint8_t)PROTOCOL_VERSION_PTCL_VER_IEEE_1278_1_2012;
	type = (uint8_t)pdu_type;
	family = (uint8_t)get_family(pdu_type);
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
    out << prefix << "munition " << munition
        << " '" << munition.description() << "'" << std::endl
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
void vdis::spread_spectrum_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "frequency_hopping "
        << (on_off_e)bits.frequency_hopping << std::endl
        << prefix << "pseudo_noise "
        << (on_off_e)bits.pseudo_noise << std::endl
        << prefix << "time_hopping "
        << (on_off_e)bits.time_hopping << std::endl
        << prefix << "padding(13 bits) "
        << to_bin_string((uint16_t)bits.padding) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::spread_spectrum_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::spread_spectrum_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::encoding_scheme_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "encoding_class "
        << (encoding_class_e)bits.encoding_class << std::endl;

    if (bits.encoding_class == ENCODING_CLASS_ENCODED_AUDIO)
    {
        out << prefix << "encoding_type "
            << (encoding_type_e)bits.encoding_type << std::endl;
    }
    else
    {
        out << prefix << "encoding_type(TDL) "
            << (int)bits.encoding_type << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::encoding_scheme_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::encoding_scheme_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::modulation_type_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    major_modulation_e
        mm_value = (major_modulation_e)major_modulation;

    spread_spectrum.print((prefix + "spread_spectrum."), out);

    out << prefix << "major_modulation " << mm_value << std::endl
        << prefix << "modulation_detail ";

    switch(mm_value)
    {
        case MAJOR_MODULATION_MAJ_MOD_AMPLITUDE:
            out << (amplitude_e)modulation_detail << std::endl;
            break;
        case MAJOR_MODULATION_MAJ_MOD_AMPLITUDE_AND_ANGLE:
            out << (amplitude_and_angle_e)modulation_detail << std::endl;
            break;
        case MAJOR_MODULATION_MAJ_MOD_ANGLE:
            out << (angle_e)modulation_detail << std::endl;
            break;
        default:
            out << (int)modulation_detail << std::endl;
            break;
    }

    out << prefix << "radio_system "
        << (radio_system_e)radio_system << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::modulation_type_t::read(byte_stream_t &stream)
{
    spread_spectrum.read(stream);
    stream.read(major_modulation);
    stream.read(modulation_detail);
    stream.read(radio_system);
}

// ----------------------------------------------------------------------------
void vdis::modulation_type_t::write(byte_stream_t &stream)
{
    spread_spectrum.write(stream);
    stream.write(major_modulation);
    stream.write(modulation_detail);
    stream.write(radio_system);
}

// ----------------------------------------------------------------------------
void vdis::emitter_target_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "entity " << entity_marking(entity) << std::endl
        << prefix << "emitter " << (int)emitter << std::endl
        << prefix << "beam " << (int)beam << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::emitter_target_t::read(byte_stream_t &stream)
{
    entity.read(stream);
    stream.read(emitter);
    stream.read(beam);
}

// ----------------------------------------------------------------------------
void vdis::emitter_target_t::write(byte_stream_t &stream)
{
    entity.write(stream);
    stream.write(emitter);
    stream.write(beam);
}

// ----------------------------------------------------------------------------
vdis::emitter_beam_t::emitter_beam_t(void) :
    data_length(0),
    id_number(0),
    parameter_index(0),
    frequency(0),
    frequency_range(0),
    effective_radiated_power(0),
    pulse_repetition_frequency(0),
    pulse_width(0),
    azimuth_center(0),
    azimuth_sweep(0),
    elevation_center(0),
    elevation_sweep(0),
    sweep_sync(0),
    function(0),
    target_count(0),
    high_density_track_jam(0),
    status(0),
    jamming_technique(0),
    targets(0)
{
}

// ----------------------------------------------------------------------------
vdis::emitter_beam_t::~emitter_beam_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::emitter_beam_t::clear(void)
{
    if (targets and (target_count > 0))
    {
        for(uint8_t i = 0; i < target_count; ++i)
        {
            if (targets[i])
            {
                delete targets[i];
                targets[i] = 0;
            }
        }
    }

    if (targets)
    {
        delete[] targets;
    }

    data_length = 0;
    id_number = 0;
    parameter_index = 0;
    frequency = 0;
    frequency_range = 0;
    effective_radiated_power = 0;
    pulse_repetition_frequency = 0;
    pulse_width = 0;
    azimuth_center = 0;
    azimuth_sweep = 0;
    elevation_center = 0;
    elevation_sweep = 0;
    sweep_sync = 0;
    function = 0;
    target_count = 0;
    high_density_track_jam = 0;
    status = 0;
    jamming_technique = 0;
    targets = 0;
}

// ----------------------------------------------------------------------------
void vdis::emitter_beam_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "data_length " << (int)data_length << std::endl
        << prefix << "id_number " << (int)id_number << std::endl
        << prefix << "parameter_index " << (int)parameter_index << std::endl
        << prefix << "frequency "
        << to_string(frequency) << std::endl
        << prefix << "frequency_range "
        << to_string(frequency_range) << std::endl
        << prefix << "effective_radiated_power "
        << to_string(effective_radiated_power) << std::endl
        << prefix << "pulse_repetition_frequency "
        << to_string(pulse_repetition_frequency) << std::endl
        << prefix << "pulse_width "
        << to_string(pulse_width) << std::endl
        << prefix << "azimuth_center "
        << to_string(azimuth_center) << std::endl
        << prefix << "azimuth_sweep "
        << to_string(azimuth_sweep) << std::endl
        << prefix << "elevation_center "
        << to_string(elevation_center) << std::endl
        << prefix << "elevation_sweep "
        << to_string(elevation_sweep) << std::endl
        << prefix << "sweep_sync "
        << to_string(sweep_sync) << std::endl
        << prefix << "function " << (beam_function_e)function << std::endl
        << prefix << "high_density_track_jam "
        << (int)high_density_track_jam << std::endl
        << prefix << "status " << (int)status << std::endl
        << prefix << "jamming_technique " << (int)jamming_technique << std::endl
        << prefix << "targets.count " << (int)target_count << std::endl;

    for(uint8_t i = 0; i < target_count; ++i)
    {
        if (targets[i])
        {
            targets[i]->print(
                (prefix + "targets[" + to_string((int)i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::emitter_beam_t::read(byte_stream_t &stream)
{
    clear();

    stream.read(data_length);
    stream.read(id_number);
    stream.read(parameter_index);
    stream.read(frequency);
    stream.read(frequency_range);
    stream.read(effective_radiated_power);
    stream.read(pulse_repetition_frequency);
    stream.read(pulse_width);
    stream.read(azimuth_center);
    stream.read(azimuth_sweep);
    stream.read(elevation_center);
    stream.read(elevation_sweep);
    stream.read(sweep_sync);
    stream.read(function);
    stream.read(target_count);
    stream.read(high_density_track_jam);
    stream.read(status);
    stream.read(jamming_technique);

    if (target_count > 0)
    {
        targets = new emitter_target_t*[target_count];
    }

    for(uint8_t i = 0; (i < target_count) and not stream.error(); ++i)
    {
        targets[i] = new emitter_target_t;
        targets[i]->read(stream);
    }
}

// ----------------------------------------------------------------------------
void vdis::emitter_beam_t::write(byte_stream_t &stream)
{
    stream.write(data_length);
    stream.write(id_number);
    stream.write(parameter_index);
    stream.write(frequency);
    stream.write(frequency_range);
    stream.write(effective_radiated_power);
    stream.write(pulse_repetition_frequency);
    stream.write(pulse_width);
    stream.write(azimuth_center);
    stream.write(azimuth_sweep);
    stream.write(elevation_center);
    stream.write(elevation_sweep);
    stream.write(sweep_sync);
    stream.write(function);
    stream.write(target_count);
    stream.write(high_density_track_jam);
    stream.write(status);
    stream.write(jamming_technique);

    for(uint8_t i = 0; targets and (i < target_count); ++i)
    {
        if (not stream.error())
        {
            targets[i]->write(stream);
        }
    }
}

// ----------------------------------------------------------------------------
vdis::emitter_system_t::emitter_system_t(void) :
    data_length(0),
    beam_count(0),
    padding(0),
    name(0),
    function(0),
    number(0),
    beams(0)
{
}

// ----------------------------------------------------------------------------
vdis::emitter_system_t::~emitter_system_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::emitter_system_t::clear(void)
{
    if (beams and (beam_count > 0))
    {
        for(uint8_t i = 0; i < beam_count; ++i)
        {
            if (beams[i])
            {
                delete beams[i];
                beams[i] = 0;
            }
        }
    }

    if (beams)
    {
        delete[] beams;
    }

    data_length = 0;
    beam_count = 0;
    padding = 0;
    name = 0;
    function = 0;
    number = 0;
    location.clear();
    beams = 0;
}

// ----------------------------------------------------------------------------
void vdis::emitter_system_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "data_length " << (int)data_length << std::endl
        << prefix << "padding(16 bits) "
        << to_bin_string(padding, true) << std::endl
        << prefix << "name " << (emitter_name_e)name << std::endl
        << prefix << "function "
        << (emitter_function_e)function << std::endl
        << prefix << "number " << (int)number << std::endl
        << prefix << "location " << location << std::endl
        << prefix << "beams.count " << (int)beam_count << std::endl;

    for(uint8_t i = 0; i < beam_count; ++i)
    {
        if (beams[i])
        {
            beams[i]->print(
                (prefix + "beams[" + to_string((int)i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::emitter_system_t::read(byte_stream_t &stream)
{
    clear();

    stream.read(data_length);
    stream.read(beam_count);
    stream.read(padding);
    stream.read(name);
    stream.read(function);
    stream.read(number);
    location.read(stream);

    if (beam_count > 0)
    {
        beams = new emitter_beam_t*[beam_count];
    }

    for(uint8_t i = 0; (i < beam_count) and not stream.error(); ++i)
    {
        beams[i] = new emitter_beam_t;
        beams[i]->read(stream);
    }
}

// ----------------------------------------------------------------------------
void vdis::emitter_system_t::write(byte_stream_t &stream)
{
    stream.write(data_length);
    stream.write(beam_count);
    stream.write(padding);
    stream.write(name);
    stream.write(function);
    stream.write(number);
    location.write(stream);

    for(uint8_t i = 0; beams and (i < beam_count); ++i)
    {
        if (not stream.error())
        {
            beams[i]->write(stream);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::iff_change_options_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "change_options.bits "
        << to_bin_string(value) << std::endl;

    if (bits.specific1)
    {
        out << prefix << "change_options.system_specific1 yes" << std::endl;
    }

    if (bits.specific2)
    {
        out << prefix << "change_options.system_specific2 yes" << std::endl;
    }

    if (bits.heartbeat)
    {
        out << prefix << "change_options.heartbeat yes" << std::endl;
    }

    out << prefix << "change_options.iff_mode "
        << (bits.iff_mode ? "interrogator" : "transponder") << std::endl
        << prefix << "change_options.simulation_mode "
        << (bits.simulation_mode ? "interactive" : "regeneration") << std::endl;

    if (bits.interactive)
    {
        out << prefix << "change_options.interactive capable" << std::endl;
    }

    if (bits.test_mode)
    {
        out << prefix << "change_options.test_mode on" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::iff_change_options_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::iff_change_options_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::iff_system_id_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "type " << (iff_system_type_e)type << std::endl
        << prefix << "name " << (iff_system_name_e)name << std::endl
        << prefix << "mode " << (iff_system_mode_e)mode << std::endl;

    options.print(prefix, out);
}

// ----------------------------------------------------------------------------
void vdis::iff_system_id_t::read(byte_stream_t &stream)
{
    stream.read(type);
    stream.read(name);
    stream.read(mode);
    stream.read(options);
}

// ----------------------------------------------------------------------------
void vdis::iff_system_id_t::write(byte_stream_t &stream)
{
    stream.write(type);
    stream.write(name);
    stream.write(mode);
    stream.write(options);
}

// ----------------------------------------------------------------------------
void vdis::iff_operational_data_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "status " << (int)status << std::endl
        << prefix << "data_field_1 " << (int)data_field_1 << std::endl
        << prefix << "information_layers " << (int)information_layers << std::endl
        << prefix << "data_field_2 " << (int)data_field_2 << std::endl
        << prefix << "parameter_1 " << (int)parameter_1 << std::endl
        << prefix << "parameter_2 " << (int)parameter_2 << std::endl
        << prefix << "parameter_3 " << (int)parameter_3 << std::endl
        << prefix << "parameter_4 " << (int)parameter_4 << std::endl
        << prefix << "parameter_5 " << (int)parameter_5 << std::endl
        << prefix << "parameter_6 " << (int)parameter_6 << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::iff_operational_data_t::read(byte_stream_t &stream)
{
    stream.read(status);
    stream.read(data_field_1);
    stream.read(information_layers);
    stream.read(data_field_2);
    stream.read(parameter_1);
    stream.read(parameter_2);
    stream.read(parameter_3);
    stream.read(parameter_4);
    stream.read(parameter_5);
    stream.read(parameter_6);
}

// ----------------------------------------------------------------------------
void vdis::iff_operational_data_t::write(byte_stream_t &stream)
{
    stream.write(status);
    stream.write(data_field_1);
    stream.write(information_layers);
    stream.write(data_field_2);
    stream.write(parameter_1);
    stream.write(parameter_2);
    stream.write(parameter_3);
    stream.write(parameter_4);
    stream.write(parameter_5);
    stream.write(parameter_6);
}

// ----------------------------------------------------------------------------
void vdis::iff_parameter_data_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "radiated_power(dBm) "
        << to_string(radiated_power) << std::endl
        << prefix << "frequency(Hz) "
        << to_string(frequency) << std::endl
        << prefix << "interrogation_frequency(Hz) "
        << to_string(interrogation_frequency) << std::endl
        << prefix << "pulse_width "
        << to_string(pulse_width) << std::endl
        << prefix << "burst_length "
        << to_string(burst_length) << std::endl
        << prefix << "applicable_modes "
        << to_bin_string(applicable_modes) << std::endl
        << prefix << "system_specific_data "
        << to_hex_string(system_specific_data[0]) << "-"
        << to_hex_string(system_specific_data[1]) << "-"
        << to_hex_string(system_specific_data[2]) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::iff_parameter_data_t::read(byte_stream_t &stream)
{
    stream.read(radiated_power);
    stream.read(frequency);
    stream.read(interrogation_frequency);
    stream.read(pulse_width);
    stream.read(burst_length);
    stream.read(applicable_modes);
    stream.read(system_specific_data[0]);
    stream.read(system_specific_data[1]);
    stream.read(system_specific_data[2]);
}

// ----------------------------------------------------------------------------
void vdis::iff_parameter_data_t::write(byte_stream_t &stream)
{
    stream.write(radiated_power);
    stream.write(frequency);
    stream.write(interrogation_frequency);
    stream.write(pulse_width);
    stream.write(burst_length);
    stream.write(applicable_modes);
    stream.write(system_specific_data[0]);
    stream.write(system_specific_data[1]);
    stream.write(system_specific_data[2]);
}

// ----------------------------------------------------------------------------
void vdis::iff_layer1_data_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "emitter " << entity_marking(emitter) << std::endl
        << prefix << "event " << event << std::endl
        << prefix << "antenna_location " << antenna_location << std::endl;

    system_id.print((prefix + "system_id."), out);

    out << prefix << "system_designator "
        << (int)system_designator << std::endl
        << prefix << "system_specific "
        << (int)system_specific << std::endl;

    operational_data.print((prefix + "operational_data."), out);
}

// ----------------------------------------------------------------------------
void vdis::iff_layer1_data_t::read(byte_stream_t &stream)
{
    emitter.read(stream);
    event.read(stream);
    antenna_location.read(stream);
    system_id.read(stream);
    stream.read(system_designator);
    stream.read(system_specific);
    operational_data.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::iff_layer1_data_t::write(byte_stream_t &stream)
{
    emitter.write(stream);
    event.write(stream);
    antenna_location.write(stream);
    system_id.write(stream);
    stream.write(system_designator);
    stream.write(system_specific);
    operational_data.write(stream);
}

// ----------------------------------------------------------------------------
vdis::iff_layer2_data_t::iff_layer2_data_t(void) :
    number(0),
    specific_info(0),
    layer_length(0),
    azimuth_center(0),
    azimuth_sweep(0),
    elevation_center(0),
    elevation_sweep(0),
    sweep_sync(0),
    parameter_1(0),
    parameter_2(0),
    parameter_data_count(0),
    parameter_data(0)
{

}

// ----------------------------------------------------------------------------
vdis::iff_layer2_data_t::~iff_layer2_data_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::iff_layer2_data_t::clear(void)
{
    for(uint16_t i = 0; parameter_data and (i < parameter_data_count); ++i)
    {
        if (parameter_data[i])
        {
            delete parameter_data[i];
            parameter_data[i] = 0;
        }
    }

    if (parameter_data)
    {
        delete[] parameter_data;
    }

    number = 0;
    specific_info = 0;
    layer_length = 0;
    azimuth_center = 0;
    azimuth_sweep = 0;
    elevation_center = 0;
    elevation_sweep = 0;
    sweep_sync = 0;
    parameter_1 = 0;
    parameter_2 = 0;
    parameter_data_count = 0;
    parameter_data = 0;
}

// ----------------------------------------------------------------------------
void vdis::iff_layer2_data_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "number " << (int)number << std::endl
        << prefix << "specific_info " << (int)specific_info << std::endl
        << prefix << "layer_length " << (int)layer_length << std::endl
        << prefix << "azimuth_center "
        << to_string(azimuth_center) << std::endl
        << prefix << "azimuth_sweep "
        << to_string(azimuth_sweep) << std::endl
        << prefix << "elevation_center "
        << to_string(elevation_center) << std::endl
        << prefix << "elevation_sweep "
        << to_string(elevation_sweep) << std::endl
        << prefix << "sweep_sync " << to_string(sweep_sync) << std::endl
        << prefix << "parameter_1 " << (int)parameter_1 << std::endl
        << prefix << "parameter_2 " << (int)parameter_2 << std::endl
        << prefix << "parameter_data.count "
        << (int)parameter_data_count << std::endl;

    for(uint16_t i = 0; parameter_data and (i < parameter_data_count); ++i)
    {
        if (parameter_data[i])
        {
            parameter_data[i]->print(
                (prefix + "parameter_data[" + to_string((int)i) + "]."),
                out);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::iff_layer2_data_t::read(byte_stream_t &stream)
{
    clear();

    stream.read(number);
    stream.read(specific_info);
    stream.read(layer_length);
    stream.read(azimuth_center);
    stream.read(azimuth_sweep);
    stream.read(elevation_center);
    stream.read(elevation_sweep);
    stream.read(sweep_sync);
    stream.read(parameter_1);
    stream.read(parameter_2);
    stream.read(parameter_data_count);

    if (parameter_data_count > 0)
    {
        parameter_data = new iff_parameter_data_t*[parameter_data_count];
    }

    for(uint16_t i = 0; (i < parameter_data_count) and not stream.error(); ++i)
    {
        parameter_data[i] = new iff_parameter_data_t;
        parameter_data[i]->read(stream);
    }
}

// ----------------------------------------------------------------------------
void vdis::iff_layer2_data_t::write(byte_stream_t &stream)
{
    stream.write(number);
    stream.write(specific_info);
    stream.write(layer_length);
    stream.write(azimuth_center);
    stream.write(azimuth_sweep);
    stream.write(elevation_center);
    stream.write(elevation_sweep);
    stream.write(sweep_sync);
    stream.write(parameter_1);
    stream.write(parameter_2);
    stream.write(parameter_data_count);

    for(uint16_t i = 0; (i < parameter_data_count) and parameter_data; ++i)
    {
        parameter_data[i]->write(stream);
    }
}

// ----------------------------------------------------------------------------
vdis::environment_record_t::environment_record_t(void) :
    type(0),
    data_length(0),
    index(0),
    padding(0),
    data(0)
{

}

// ----------------------------------------------------------------------------
vdis::environment_record_t::~environment_record_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::environment_record_t::clear(void)
{
    type = 0;
    data_length = 0;
    index = 0;
    padding = 0;

    if (data)
    {
        delete[] data;
        data = 0;
    }
}

// ----------------------------------------------------------------------------
void vdis::environment_record_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "type " << (env_geometry_rec_type_e)type << std::endl
        << prefix << "index " << (int)index << std::endl
        << prefix << "padding " << (int)padding << std::endl
        << prefix << "data.length " << (int)data_length << std::endl;

    if (data)
    {
        byte_buffer_t data_buffer(data, data_length, false);

        data_buffer.print((prefix + "data."), out);
    }
}

// ----------------------------------------------------------------------------
void vdis::environment_record_t::read(byte_stream_t &stream)
{
    clear();

    stream.read(type);
    stream.read(data_length);
    stream.read(index);
    stream.read(padding);

    if (data_length > 0)
    {
        data_length = (data_length / 8);

        if (data_length < 8)
        {
            LOG_ERROR("Invalid length in environment record: %d", data_length);
            data_length = 0;
        }
        else
        {
            data_length -= 8;

            if (data_length > 0)
            {
                data = new uint8_t[data_length];
                stream.read(data, data_length);
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::environment_record_t::write(byte_stream_t &stream)
{
    stream.write(type);
    stream.write((data_length * 8) + 64);
    stream.write(index);
    stream.write(padding);

    if (data and (data_length > 0))
    {
        stream.write(data, data_length);
    }
}

// ----------------------------------------------------------------------------
void vdis::exercise_state_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "id " << (int)id << std::endl
        << prefix << "transition " << transition_enum() << std::endl
        << prefix << "current_state " << current_state_enum() << std::endl
        << prefix << "requested_state " << requested_state_enum() << std::endl;
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
void vdis::object_modification_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    if (bits.modified_location)
    {
        out << prefix << "modifications.location "
            << (yes_no_e)bits.modified_location << std::endl;
    }

    if (bits.modified_orientation)
    {
        out << prefix << "modification.orientation "
            << (yes_no_e)bits.modified_orientation << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "modifications.padding(6 bits) "
            << (int)bits.padding << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::object_modification_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::object_modification_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::generic_object_appearance_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "generic_appearance.value "
        << to_bin_string(value, true) << std::endl
        << prefix << "generic_appearance.percent_complete "
        << (int)bits.percent_complete << std::endl
        << prefix << "generic_appearance.damage "
        << (object_damage_e)bits.damage << std::endl
        << prefix << "generic_appearance.predistributed "
        << (yes_no_e)bits.predistributed << std::endl
        << prefix << "generic_appearance.deactivated "
        << (yes_no_e)bits.deactivated << std::endl
        << prefix << "generic_appearance.smoking "
        << (yes_no_e)bits.smoking << std::endl
        << prefix << "generic_appearance.flaming "
        << (yes_no_e)bits.flaming << std::endl
        << prefix << "generic_appearance.padding(2 bits) "
        << (int)bits.padding << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::generic_object_appearance_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::generic_object_appearance_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::linear_segment_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    geodetic_location_t
        geodetic_location;
    uint32_t
        object_type = 0;

    if (object_type_ptr)
    {
        object_type = object_type_ptr->get();
    }

    convert(location, geodetic_location);

    out << prefix << "number " << (int)number << std::endl
        << prefix << "modifications "
        << to_bin_string(modifications.value) << std::endl;

    modifications.print(prefix, out);
    generic_appearance.print(prefix, out);

    out << prefix << "specific_appearance "
        << to_bin_string(specific_appearance, true) << std::endl;

    switch(object_type & 0xFFFFFF00U)
    {
        case 0x01010100U: // Tank Ditches (1.1.1.*)
        case 0x01010200U: // Concertina Wires (1.1.2.*)
        {
            linear_appearance_v1_t object_appearance;
            object_appearance.value = specific_appearance;
            object_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
        case 0x00050100U: // Exhaust Smoke (0.5.1.*)
        {
            linear_appearance_v2_t object_appearance;
            object_appearance.value = specific_appearance;
            object_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
        case 0x01060100U: // Minefield Lane Markers (1.6.1.*)
        {
            linear_appearance_v3_t object_appearance;
            object_appearance.value = specific_appearance;
            object_appearance.print(prefix + "specific_appearance.", out);
            break;
        }
    }

    out << prefix << "location.gcc " << location << std::endl
        << prefix << "location.gdc " << geodetic_location << std::endl
        << prefix << "orientation " << orientation << std::endl;

#ifdef LINEAR_SEGMENTS_USE_32BIT_FLOAT
    out << prefix << "segment_length " << to_string(segment_length) << std::endl
        << prefix << "segment_width " << to_string(segment_width) << std::endl
        << prefix << "segment_height " << to_string(segment_height) << std::endl
        << prefix << "segment_depth " << to_string(segment_depth) << std::endl;
#else
    out << prefix << "segment_length " << (int)segment_length << std::endl
        << prefix << "segment_width " << (int)segment_width << std::endl
        << prefix << "segment_height " << (int)segment_height << std::endl
        << prefix << "segment_depth " << (int)segment_depth << std::endl;
#endif

    out << prefix << "padding " << to_hex_string(padding, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::linear_segment_t::read(byte_stream_t &stream)
{
    stream.read(number);
    modifications.read(stream);
    generic_appearance.read(stream);
    stream.read(specific_appearance);
    location.read(stream);
    orientation.read(stream);
    stream.read(segment_length);
    stream.read(segment_width);
    stream.read(segment_height);
    stream.read(segment_depth);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::linear_segment_t::write(byte_stream_t &stream)
{
    stream.write(number);
    modifications.write(stream);
    generic_appearance.write(stream);
    stream.write(specific_appearance);
    location.write(stream);
    orientation.write(stream);
    stream.write(segment_length);
    stream.write(segment_width);
    stream.write(segment_height);
    stream.write(segment_depth);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::linear_segment_t::using_type(const object_type_t *type_ptr)
{
    object_type_ptr = type_ptr;
}

// ----------------------------------------------------------------------------
void vdis::hifi_light_flags_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.powered_on)
    {
        out << prefix << "powered_on yes" << std::endl;
    }

    if (bits.attached)
    {
        out << prefix << "attached yes" << std::endl;
    }

    if (bits.directional)
    {
        out << prefix << "directional yes" << std::endl;
    }

    if (bits.feather_enabled)
    {
        out << prefix << "feather_enabled yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(4 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::hifi_light_flags_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::hifi_light_flags_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_engine_rates_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "generator(%) "
        << to_string(generator, 1, 2) << std::endl
        << prefix << "turbine(%) "
        << to_string(turbine, 1, 2) << std::endl
        << prefix << "fuel_flow(lbs/hr) "
        << to_string(fuel_flow, 1, 2) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::ncm3_engine_rates_t::read(byte_stream_t &stream)
{
    stream.read(generator);
    stream.read(turbine);
    stream.read(fuel_flow);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_engine_rates_t::write(byte_stream_t &stream)
{
    stream.write(generator);
    stream.write(turbine);
    stream.write(fuel_flow);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_engine_transmission_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.high_temperature)
    {
        out << prefix << "high_temperature yes" << std::endl;
    }

    if (bits.low_pressure)
    {
        out << prefix << "low_pressure yes" << std::endl;
    }

    if (bits.debris)
    {
        out << prefix << "debris yes" << std::endl;
    }

    if (bits.chips)
    {
        out << prefix << "chips yes" << std::endl;
    }

    if (bits.engine_chips)
    {
        out << prefix << "padding yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(3 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_engine_transmission_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_engine_transmission_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_transmission_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.high_temperature)
    {
        out << prefix << "high_temperature yes" << std::endl;
    }

    if (bits.low_pressure_auxiliary)
    {
        out << prefix << "low_pressure_auxiliary yes" << std::endl;
    }

    if (bits.low_pressure_main)
    {
        out << prefix << "low_pressure_main yes" << std::endl;
    }

    if (bits.debris)
    {
        out << prefix << "debris yes" << std::endl;
    }

    if (bits.chips)
    {
        out << prefix << "chips yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(3 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_transmission_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_transmission_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_comb_transmission_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.high_temperature)
    {
        out << prefix << "high_temperature yes" << std::endl;
    }

    if (bits.low_pressure_auxiliary)
    {
        out << prefix << "low_pressure_auxiliary yes" << std::endl;
    }

    if (bits.low_pressure_main)
    {
        out << prefix << "low_pressure_main yes" << std::endl;
    }

    if (bits.debris_right)
    {
        out << prefix << "debris_right yes" << std::endl;
    }

    if (bits.debris_left)
    {
        out << prefix << "debris_left yes" << std::endl;
    }

    if (bits.chips)
    {
        out << prefix << "chips yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(2 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_comb_transmission_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_comb_transmission_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_shaft_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.low_pressure)
    {
        out << prefix << "low_pressure yes" << std::endl;
    }

    if (bits.chips)
    {
        out << prefix << "chips yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(6 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_shaft_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_shaft_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_ground_contact_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.right_switch)
    {
        out << prefix << "right_switch yes" << std::endl;
    }

    if (bits.left_switch)
    {
        out << prefix << "left_switch yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(6 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_ground_contact_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_ground_contact_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_control_malfunctions_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.pump_fail)
    {
        out << prefix << "pump_fail yes" << std::endl;
    }

    if (bits.return_line_filter)
    {
        out << prefix << "return_line_filter yes" << std::endl;
    }

    if (bits.pressure_line_filter)
    {
        out << prefix << "pressure_line_filter yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(5 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_control_malfunctions_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_control_malfunctions_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_utility_malfunctions_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.utility_pump_fail)
    {
        out << prefix << "utility_pump_fail yes" << std::endl;
    }

    if (bits.apu_pump_fail)
    {
        out << prefix << "apu_pump_fail yes" << std::endl;
    }

    if (bits.return_line_filter)
    {
        out << prefix << "return_line_filter yes" << std::endl;
    }

    if (bits.pressure_line_filter)
    {
        out << prefix << "pressure_line_filter yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(4 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_utility_malfunctions_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_utility_malfunctions_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_uh72_controls_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.hoist_cable_cut)
    {
        out << prefix << "hoist_cable_cut yes" << std::endl;
    }

    if (bits.water_bucket_valve_open)
    {
        out << prefix << "water_bucket_valve_open yes" << std::endl;
    }

    if (bits.over_torque_warning)
    {
        out << prefix << "over_torque_warning yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(5 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_uh72_controls_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_uh72_controls_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hook_safety_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.uh60_emergency_release)
    {
        out << prefix << "uh60_emergency_release yes" << std::endl;
    }

    if (bits.hook_safe)
    {
        out << prefix << "hook_safe yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(6 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hook_safety_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hook_safety_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_flare_state_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value, true) << std::endl;

    if (bits.safe)
    {
        out << prefix << "safe yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(6 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_flare_state_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_flare_state_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_cargo_hook_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.locked_open)
    {
        out << prefix << "locked_open yes" << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(15 bits) "
            << to_bin_string((uint16_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_cargo_hook_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_cargo_hook_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_water_bucket_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "fill_level " << to_string(fill_level, 1, 2) << std::endl
        << prefix << "size " << (int)size << std::endl
        << prefix << "padding(8 bits) " << to_bin_string(padding) << std::endl
        << prefix << "releasing " << to_bin_string(releasing) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::ncm3_water_bucket_t::read(byte_stream_t &stream)
{
    stream.read(fill_level);
    stream.read(size);
    stream.read(padding);
    stream.read(releasing);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_water_bucket_t::write(byte_stream_t &stream)
{
    stream.write(fill_level);
    stream.write(size);
    stream.write(padding);
    stream.write(releasing);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hoist_status_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "value " << to_bin_string(value) << std::endl;

    if (bits.cable_reset)
    {
        out << prefix << "cable_reset " << (int)bits.cable_reset << std::endl;
    }

    if (bits.cable_cut)
    {
        out << prefix << "cable_cut " << (int)bits.cable_cut << std::endl;
    }

    if (bits.device_open)
    {
        out << prefix << "device_open " << (int)bits.device_open << std::endl;
    }

    if (bits.device_type)
    {
        out << prefix << "device_type "
            << (ncm3_hoist_device_e)bits.device_type << std::endl;
    }

    if (bits.padding)
    {
        out << prefix << "padding(2 bits) "
            << to_bin_string((uint8_t)bits.padding) << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hoist_status_t::read(byte_stream_t &stream)
{
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hoist_status_t::write(byte_stream_t &stream)
{
    stream.write(value);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hoist_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "rescue_device_location "
        << rescue_device_location << std::endl
        << prefix << "rescue_device_orientation "
        << rescue_device_orientation << std::endl
        << prefix << "boom_angle "
        << to_string(boom_angle, 1, 2) << std::endl
        << prefix << "hook_location "
        << hook_location << std::endl
        << prefix << "hook_orientation "
        << hook_orientation << std::endl;

    status.print((prefix + "status."), out);

    out << prefix << "hoist_device_id " << (int)hoist_device_id << std::endl
        << prefix << "padding(2 bytes) "
        << to_hex_string(padding[0]) << "-"
        << to_hex_string(padding[1]) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hoist_t::read(byte_stream_t &stream)
{
    rescue_device_location.read(stream);
    rescue_device_orientation.read(stream);
    stream.read(boom_angle);
    hook_location.read(stream);
    hook_orientation.read(stream);
    status.read(stream);
    stream.read(hoist_device_id);
    stream.read(padding, 2);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hoist_t::write(byte_stream_t &stream)
{
    rescue_device_location.write(stream);
    rescue_device_orientation.write(stream);
    stream.write(boom_angle);
    hook_location.write(stream);
    hook_orientation.write(stream);
    status.write(stream);
    stream.write(hoist_device_id);
    stream.write(padding, 2);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_system_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "padding(16 bits) "
        << to_bin_string(padding, true) << std::endl
        << prefix << "temperature(C) "
        << (int)temperature << std::endl
        << prefix << "pressure(psi) "
        << (int)pressure << std::endl
        << prefix << "reservoir_level(%) "
        << (int)reservoir_level << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_system_t::read(byte_stream_t &stream)
{
    stream.read(padding);
    stream.read(temperature);
    stream.read(pressure);
    stream.read(reservoir_level);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_system_t::write(byte_stream_t &stream)
{
    stream.write(padding);
    stream.write(temperature);
    stream.write(pressure);
    stream.write(reservoir_level);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_maintenance_cautions_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "padding(8 bits) "
        << to_bin_string(padding) << std::endl;

    engine_transmission1.print((prefix + "engine_transmission1."), out);
    engine_transmission2.print((prefix + "engine_transmission2."), out);
    forward_transmission.print((prefix + "forward_transmission."), out);
    combined_transmission.print((prefix + "combined_transmission."), out);
    aft_transmission.print((prefix + "aft_transmission."), out);
    aft_shaft.print((prefix + "aft_shaft."), out);
    ground_contact.print((prefix + "ground_contact."), out);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_maintenance_cautions_t::read(byte_stream_t &stream)
{
    stream.read(padding);
    engine_transmission1.read(stream);
    engine_transmission2.read(stream);
    forward_transmission.read(stream);
    combined_transmission.read(stream);
    aft_transmission.read(stream);
    aft_shaft.read(stream);
    ground_contact.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_maintenance_cautions_t::write(byte_stream_t &stream)
{
    stream.write(padding);
    engine_transmission1.write(stream);
    engine_transmission2.write(stream);
    forward_transmission.write(stream);
    combined_transmission.write(stream);
    aft_transmission.write(stream);
    aft_shaft.write(stream);
    ground_contact.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_cautions_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "padding(8 bits) "
        << to_bin_string(padding) << std::endl;

    flight_control1.print((prefix + "flight_control1."), out);
    flight_control2.print((prefix + "flight_control2."), out);
    utility.print((prefix + "utility."), out);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_cautions_t::read(byte_stream_t &stream)
{
    stream.read(padding);
    flight_control1.read(stream);
    flight_control2.read(stream);
    utility.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_cautions_t::write(byte_stream_t &stream)
{
    stream.write(padding);
    flight_control1.write(stream);
    flight_control2.write(stream);
    utility.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_gauges_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    flight_control1.print((prefix + "flight_control1."), out);
    flight_control2.print((prefix + "flight_control2."), out);
    utility.print((prefix + "utility."), out);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_gauges_t::read(byte_stream_t &stream)
{
    flight_control1.read(stream);
    flight_control2.read(stream);
    utility.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_hydraulic_gauges_t::write(byte_stream_t &stream)
{
    flight_control1.write(stream);
    flight_control2.write(stream);
    utility.write(stream);
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::simulation_id_t &o)
{
    out << (int)o.site << "." << (int)o.application;

    return out;
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::id_t &o)
{
    out << (int)o.site << "."
        << (int)o.application << "."
        << (int)o.identity;

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
std::ostream &operator<<(std::ostream &out, const vdis::marking_t &o)
{
    return (out << o.str());
}

// ----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const vdis::offset12_t &o)
{
    out << vdis::to_string(o.x) << ", "
        << vdis::to_string(o.y) << ", "
        << vdis::to_string(o.z);

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
std::ostream &operator<<(std::ostream &out, const vdis::geodetic_location_t &o)
{
    out << vdis::to_string(o.latitude, 1, 5) << ", "
        << vdis::to_string(o.longitude, 1, 5) << ", "
        << vdis::to_string(o.elevation, 1, 1);

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
