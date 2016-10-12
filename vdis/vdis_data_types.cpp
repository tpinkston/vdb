#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_entity_types.h"
#include "vdis_object_types.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const float32_t
        TIME_UNIT_TO_SECONDS = (3600.0 / 2147483647.0),
        SECONDS_TO_TIME_UNITS = (1.0 / TIME_UNIT_TO_SECONDS);
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
    bool match = true;

    if ((site != ALL) and (other.site != ALL))
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
string_t vdis::entity_type_t::description(void) const
{
    string_t
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
string_t vdis::object_type_t::description(void) const
{
    std::ostringstream
        stream;
    string_t
        description = object_types::get_description(get());

    if (description.empty())
    {
        object_type_t
            parent;

        if (object_types::get_valid_parent(*this, parent))
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
vdis::object_geometry_e vdis::object_type_t::geometry(void) const
{
    return object_types::get_geometry(get());
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
void vdis::marking_t::str(const string_t &name)
{
    character_set = ENTITY_MARKING_ASCII;

    for(unsigned i = 0; i < VDIS_MARKING_CHARACTERS; ++i)
    {
        characters[i] = ((i < name.length()) ? (uint8_t)name[i] : 0);
    }
}

// ----------------------------------------------------------------------------
string_t vdis::marking_t::str(void) const
{
    const bool
        ASCII = is_printable_character(character_set);
    string_t
        string;
    uint32_t
        sum = 0;

    for(unsigned i = 0; i < VDIS_MARKING_CHARACTERS; ++i)
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

        for(unsigned i = 0; (i < VDIS_MARKING_CHARACTERS) and not done; ++i)
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

        string = ("'" + stream.str() + "'");
    }
    else
    {
        std::ostringstream
            stream;

        for(unsigned i = 0; i < VDIS_MARKING_CHARACTERS; ++i)
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
    clear_memory(characters, VDIS_MARKING_CHARACTERS);
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
    stream.read(characters, VDIS_MARKING_CHARACTERS);
}

// ----------------------------------------------------------------------------
void vdis::marking_t::write(byte_stream_t &stream)
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
    const string_t &prefix,
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
    const string_t &prefix,
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
    const string_t &prefix,
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
    const string_t &prefix,
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
    const string_t &prefix,
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
    const string_t &prefix,
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
void vdis::modulation_type_t::print(
    const string_t &prefix,
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
    const string_t &prefix,
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
    const string_t &prefix,
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
    const string_t &prefix,
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
void vdis::exercise_state_t::print(
    const string_t &prefix,
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
