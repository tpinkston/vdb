#include "vdis_byte_stream.h"
#include "vdis_logger.h"
#include "vdis_datum_records.h"
#include "vdis_services.h"
#include "vdis_string.h"

// ----------------------------------------------------------------------------
// Returns array with length equal to 'fixed_count'
//
vdis::fixed_datum_record_t **vdis::read_fixed_datum_records(
    byte_stream_t &stream,
    uint32_t fixed_count)
{
    fixed_datum_record_t **records = 0;

    if (fixed_count > 0)
    {
        records = new fixed_datum_record_t*[fixed_count];

        LOG_EXTRA_VERBOSE(
            "Allocated fixed datum record array: %p...",
            records);
    }

    for(uint32_t i = 0; i < fixed_count; ++i)
    {
        records[i] = new fixed_datum_record_t({ 0, 0 });

        LOG_EXTRA_VERBOSE(
            "Reading fixed datum %d/%d@%p with stream at index %d/%d...",
            (i + 1),
            fixed_count,
            records[i],
            stream.index(),
            stream.length());

        records[i]->read(stream);
    }

    return records;
}

// ----------------------------------------------------------------------------
// Returns array with length equal to 'variable_count'
//
vdis::variable_datum_record_t **vdis::read_variable_datum_records(
    byte_stream_t &stream,
    uint32_t variable_count)
{
    variable_datum_record_t **records = 0;

    if (variable_count > 0)
    {
        records = new variable_datum_record_t*[variable_count];

        LOG_EXTRA_VERBOSE(
            "Allocated variable datum record array: %p...",
            records);
    }

    for(uint32_t i = 0; i < variable_count; ++i)
    {
        records[i] = new variable_datum_record_t;

        LOG_EXTRA_VERBOSE(
            "Reading variable datum %d/%d@%p with stream at index %d/%d...",
            (i + 1),
            variable_count,
            records[i],
            stream.index(),
            stream.length());

        records[i]->read(stream);
    }

    return records;
}

// ----------------------------------------------------------------------------
// Returns true if no inconsistencies are found (errors will be logged)
//
bool vdis::write_fixed_datum_records(
    byte_stream_t &stream,
    fixed_datum_record_t **fixed_records,
    uint32_t fixed_count)
{
    bool success = false;

    if ((fixed_count > 0) and not fixed_records)
    {
        LOG_ERROR("Null fixed datum array with length %d!", fixed_count);
    }
    else if ((fixed_count == 0) and fixed_records)
    {
        LOG_ERROR("Non-null fixed record array with length 0!");
    }
    else
    {
        success = true;

        for(uint8_t i = 0; fixed_records and (i < fixed_count); ++i)
        {
            if (fixed_records[i])
            {
                fixed_records[i]->write(stream);

                if (stream.error())
                {
                    success = false;
                }
            }
            else
            {
                LOG_ERROR("Null fixed record at index %d!", i);
                success = false;
            }
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdis::write_variable_datum_records(
    byte_stream_t &stream,
    variable_datum_record_t **variable_records,
    uint32_t variable_count)
{
    bool success = false;

    if ((variable_count > 0) and not variable_records)
    {
        LOG_ERROR("Null fixed record array with length %d!", variable_count);
    }
    else if ((variable_count == 0) and variable_records)
    {
        LOG_ERROR("Non-null fixed record array with length 0!");
    }
    else
    {
        success = true;

        for(uint8_t i = 0; variable_records and (i < variable_count); ++i)
        {
            if (variable_records[i])
            {
                variable_records[i]->write(stream);

                if (stream.error())
                {
                    success = false;
                }
            }
            else
            {
                LOG_ERROR("Null variable record at index %d!", i);
                success = false;
            }
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
void vdis::fixed_datum_record_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "datum_id " << datum_id_enum() << std::endl
        << prefix << "datum_value "
        << value << " [0x" << to_hex_string(value) << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::fixed_datum_record_t::read(byte_stream_t &stream)
{
    stream.read(id);
    stream.read(value);
}

// ----------------------------------------------------------------------------
void vdis::fixed_datum_record_t::write(byte_stream_t &stream)
{
    stream.write(id);
    stream.write(value);
}

// ----------------------------------------------------------------------------
// Reads 4 bytes for datum length and checks the byte stream for the needed
// bytes.  Returns datum length in bytes.
//
uint32_t vdis::variable_datum_content_t::read_length(byte_stream_t &stream)
{
    uint32_t length_bits = 0;
    uint32_t length_bytes = 0;

    // Value should be in bits not bytes
    //
    stream.read(length_bits);

    LOG_EXTRA_VERBOSE(
        "Variable datum record length in bits is %d...",
        length_bits);

    if ((length_bits % 8) > 0)
    {
        LOG_WARNING(
            "Reading variable datum content with invalid length: %d",
            length_bits);
    }
    else
    {
        // Convert value in bits to bytes
        //
        length_bytes = (length_bits / 8);

        LOG_EXTRA_VERBOSE(
            "Variable datum record length in bytes is %d...",
            length_bytes);

        if (length_bytes > 0)
        {
            if (stream.remaining_length() >= length_bytes)
            {
                LOG_EXTRA_VERBOSE(
                    "Reading variable datum content %d bytes...",
                    length_bytes);
            }
            else
            {
                LOG_ERROR(
                    "Cannot create variable datum content with "
                    " %d/%d bytes remaining",
                    stream.remaining_length(),
                    length_bytes);
            }
        }
    }

    return length_bytes;
}

// ----------------------------------------------------------------------------
vdis::variable_datum_record_t::variable_datum_record_t(void) :
    id(0),
    content_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdis::variable_datum_record_t::~variable_datum_record_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::variable_datum_record_t::clear(void)
{
    id = 0;

    if (content_ptr)
    {
        LOG_EXTRA_VERBOSE(
            "Clearing variable datum record with content: %p",
            content_ptr);

        delete content_ptr;
        content_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdis::variable_datum_record_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "datum_id " << datum_id_enum() << std::endl;

    if (content_ptr)
    {
        out << prefix << "datum_length "
            << content_ptr->length() << " bits" << std::endl;

        content_ptr->print(prefix, out);
    }
}

// ----------------------------------------------------------------------------
void vdis::variable_datum_record_t::read(byte_stream_t &stream)
{
    clear();

    stream.read(id);

    switch(id)
    {
        case DATUM_IDS_DID_STATUS_DAMAGE_AGGR:
            content_ptr = new damage_status_t;
            break;
        case DATUM_IDS_DID_CARRIER_MOUNT_ID:
            content_ptr = new carrier_mount_id_t;
            break;
        case DATUM_IDS_DID_MOUNT_DATA_ID:
            content_ptr = new mount_dismount_t;
            break;
        case DATUM_IDS_DID_SLING_LOAD_CAPABILITY:
            content_ptr = new sling_load_capability_t;
            break;
        case DATUM_IDS_DID_TASK_ORG_FORCE_ID_AFFILIATION:
            content_ptr = new force_id_affiliation_t;
            break;
        case DATUM_IDS_DID_AVCATT_TO_NCM3_01:
            content_ptr = new avcatt_to_ncm3_01_t;
            break;
        case DATUM_IDS_DID_NCM3_TO_AVCATT_01:
            content_ptr = new ncm3_to_avcatt_01_t;
            break;
        case DATUM_IDS_DID_AVCATT_TO_NCM3_02:
            content_ptr = new avcatt_to_ncm3_02_t;
            break;
        case DATUM_IDS_DID_NCM3_TO_AVCATT_02:
            content_ptr = new ncm3_to_avcatt_02_t;
            break;
        case DATUM_IDS_DID_AVCATT_TO_NCM3_03:
            content_ptr = new avcatt_to_ncm3_03_t;
            break;
        case DATUM_IDS_DID_NCM3_TO_AVCATT_03:
            content_ptr = new ncm3_to_avcatt_03_t;
            break;
        default:
            content_ptr = new default_variable_datum_content_t;
            break;
    }

    LOG_EXTRA_VERBOSE("Reading variable datum content: %p", content_ptr);

    content_ptr->read(stream);
}

// ----------------------------------------------------------------------------
void vdis::variable_datum_record_t::write(byte_stream_t &stream)
{
    stream.write(id);

    if (content_ptr)
    {
        content_ptr->write(stream);
    }
    else
    {
        // Write zero length...
        //
        stream.write((uint32_t)0);
    }
}

// ----------------------------------------------------------------------------
void vdis::default_variable_datum_content_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    if (buffer.length() > 0)
    {
        buffer.print((prefix + "data."), out);
    }
    else
    {
        out << prefix << "data (NULL)" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdis::default_variable_datum_content_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    // Convert length in bits to length in bytes
    //
    buffer.read(stream, length);

    uint32_t padding = padding_length(length, 8);

    if (padding > 0)
    {
        stream.skip(padding);
    }
}

// ----------------------------------------------------------------------------
void vdis::default_variable_datum_content_t::write(byte_stream_t &stream)
{
    const uint32_t length = (buffer.length() * 8);

    stream.write(length);
    stream.write(buffer);
}

// ----------------------------------------------------------------------------
void vdis::damage_status_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "damage_status.casualties "
        << (int)casualties << std::endl
        << prefix << "damage_status.cause "
        << cause_enum() << std::endl
        << prefix << "damage_status.padding(8 bits) "
        << to_bin_string(padding1) << std::endl
        << prefix << "damage_status.event_id "
        << event_id << std::endl
        << prefix << "damage_status.extent "
        << extent_enum() << std::endl
        << prefix << "damage_status.padding(32 bits) "
        << to_hex_string(padding2, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::damage_status_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for damage_status_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    stream.read(casualties);
    stream.read(cause);
    stream.read(padding1);
    event_id.read(stream);
    stream.read(extent);
    stream.read(padding2);
}

// ----------------------------------------------------------------------------
void vdis::damage_status_t::write(byte_stream_t &stream)
{
    stream.write(length());
    stream.write(casualties);
    stream.write(cause);
    stream.write(padding1);
    event_id.write(stream);
    stream.write(extent);
    stream.write(padding2);
}

// ----------------------------------------------------------------------------
void vdis::carrier_mount_id_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "carrier_mount_id.carrier "
        << entity_marking(carrier) << std::endl
        << prefix << "damage_status.padding(16 bits) "
        << to_bin_string(padding, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::carrier_mount_id_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for carrier_mount_id_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    carrier.read(stream);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::carrier_mount_id_t::write(byte_stream_t &stream)
{
    stream.write(length());
    carrier.write(stream);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::mount_dismount_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "mount_dismount.entity_id "
        << entity_marking(entity_id) << std::endl
        << prefix << "mount_dismount.station_id "
        << (station_name_e)station_id << std::endl
        << prefix << "mount_dismount.mass(kg) "
        << to_string(mass, 1, 3) << std::endl
        << prefix << "mount_dismount.volume(meters cubed) "
        << to_string(volume, 1, 3) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::mount_dismount_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for mount_dismount_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    entity_id.read(stream);
    stream.read(station_id);
    stream.read(mass);
    stream.read(volume);
}

// ----------------------------------------------------------------------------
void vdis::mount_dismount_t::write(byte_stream_t &stream)
{
    stream.write(length());
    entity_id.write(stream);
    stream.write(station_id);
    stream.write(mass);
    stream.write(volume);
}

// ----------------------------------------------------------------------------
vdis::sling_load_capability_t::sling_load_capability_t(void) :
    drag_coeffficient(0),
    current_mass(0),
    padding(0),
    hook_type(0),
    lines_needed(0),
    lines(0)
{

}

// ----------------------------------------------------------------------------
vdis::sling_load_capability_t::~sling_load_capability_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
uint32_t vdis::sling_load_capability_t::length(void) const
{
    // 8 bytes (64 bits) for every sling line
    //
    return (BASE_LENGTH_BITS + (lines_needed * 64));
}

// ----------------------------------------------------------------------------
void vdis::sling_load_capability_t::clear(void)
{
    for(uint8_t i = 0; ((i < lines_needed) and lines); ++i)
    {
        if (lines[i])
        {
            delete lines[i];
            lines[i] = 0;
        }
    }

    if (lines)
    {
        delete[] lines;
        lines = 0;
    }

    payload.clear();
    carrier.clear();
    drag_coeffficient = 0;
    current_mass = 0;
    padding = 0;
    hook_type = 0;
    lines_needed = 0;
}

// ----------------------------------------------------------------------------
void vdis::sling_load_capability_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "sling_load_capability.payload "
        << entity_marking(payload) << std::endl
        << prefix << "sling_load_capability.carrier "
        << entity_marking(carrier) << std::endl
        << prefix << "sling_load_capability.drag_coeffficient "
        << to_string(drag_coeffficient, 1, 4) << std::endl
        << prefix << "sling_load_capability.current_mass(kg) "
        << to_string(current_mass, 1, 4) << std::endl
        << prefix << "sling_load_capability.padding(16 bits) "
        << to_bin_string(padding, true) << std::endl
        << prefix << "sling_load_capability.hook_type "
        << hook_type_enum() << std::endl
        << prefix << "sling_load_capability.lines_needed "
        << (int)lines_needed << std::endl;

    for(uint8_t i = 0; ((i < lines_needed) and lines); ++i)
    {
        lines[i]->print(
            (prefix + "line[" + to_string((int)i) + "]."),
            out);
    }
}

// ----------------------------------------------------------------------------
void vdis::sling_load_capability_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length < (BASE_LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for sling_load_capability_t: %d/%d",
            length,
            (BASE_LENGTH_BITS / 8));
    }

    payload.read(stream);
    carrier.read(stream);
    stream.read(drag_coeffficient);
    stream.read(current_mass);
    stream.read(padding);
    stream.read(hook_type);
    stream.read(lines_needed);

    if (lines_needed > 0)
    {
        lines = new sling_line_t*[lines_needed];

        for(uint8_t i = 0; (i < lines_needed); ++i)
        {
            lines[i] = new sling_line_t;
            lines[i]->read(stream);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::sling_load_capability_t::write(byte_stream_t &stream)
{
    stream.write(length());
    payload.write(stream);
    carrier.write(stream);
    stream.write(drag_coeffficient);
    stream.write(current_mass);
    stream.write(padding);
    stream.write(hook_type);
    stream.write(lines_needed);

    for(uint8_t i = 0; ((i < lines_needed) and lines); ++i)
    {
        lines[i]->write(stream);
    }
}

// ----------------------------------------------------------------------------
std::string vdis::force_id_affiliation_t::force_name(void) const
{
    if (name.length() > 0)
    {
        return name.str();
    }
    else
    {
        return "NULL";
    }
}

// ----------------------------------------------------------------------------
uint32_t vdis::force_id_affiliation_t::length(void) const
{
    // The 'name' buffer's length includes trailing zero bytes for padding
    //
    return (BASE_LENGTH_BITS + (name.length() * 8));
}

// ----------------------------------------------------------------------------
void vdis::force_id_affiliation_t::clear(void)
{
    force = 0;
    padding = 0;
    name.clear();
}

// ----------------------------------------------------------------------------
void vdis::force_id_affiliation_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "force_id_affiliation.force "
        << force_enum() << std::endl
        << prefix << "force_id_affiliation.name '"
        << force_name() << "'" << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::force_id_affiliation_t::read(byte_stream_t &stream)
{
    const uint32_t
        length = read_length(stream);
    uint32_t
        name_length = 0;

    clear();

    stream.read(force); // 2 bytes
    stream.read(padding); // 2 bytes

    name_length = (length - (BASE_LENGTH_BITS / 8));

    if (name_length > 0)
    {
        name.read(stream, name_length);
    }
}

// ----------------------------------------------------------------------------
void vdis::force_id_affiliation_t::write(byte_stream_t &stream)
{
    stream.write(length());
    stream.write(force);
    stream.write(padding);
    stream.write(name);
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_01_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    const std::string ncm3_prefix = (prefix + "avcatt_to_ncm3_01.");

    out << ncm3_prefix << "ownship_gcc "
        << ownship_gcc << std::endl
        << ncm3_prefix << "ownship_gdc "
        << ownship_gdc << std::endl
        << ncm3_prefix << "ownship_orientation "
        << ownship_orientation << std::endl
        << ncm3_prefix << "ownship_velocity "
        << ownship_velocity << std::endl
        << ncm3_prefix << "load_id "
        << entity_marking(load_id) << std::endl
        << ncm3_prefix << "frozen "
        << (yes_no_e)frozen << std::endl
        << ncm3_prefix << "load_gcc "
        << load_gcc << std::endl
        << ncm3_prefix << "load_attached "
        << (yes_no_e)load_attached << std::endl
        << ncm3_prefix << "load_orientation "
        << load_orientation << std::endl
        << ncm3_prefix << "search_light "
        << (on_off_e)search_light << std::endl
        << ncm3_prefix << "search_light_azimuth "
        << search_light_azimuth << std::endl
        << ncm3_prefix << "search_light_elevation "
        << search_light_elevation << std::endl
        << ncm3_prefix << "water_bucket_full "
        << (yes_no_e)water_bucket_full << std::endl
        << ncm3_prefix << "place_survivor_on_hoist "
        << place_survivor_on_hoist << std::endl
        << ncm3_prefix << "survivor_id "
        << entity_marking(survivor_id) << std::endl
        << ncm3_prefix << "hook_type "
        << (hook_type_e)hook_type << std::endl
        << ncm3_prefix << "malfunction "
        << (yes_no_e)malfunction << std::endl
        << ncm3_prefix << "collision "
        << (yes_no_e)collision << std::endl
        << ncm3_prefix << "ownship_appearance "
        << to_bin_string(ownship_appearance, true) << std::endl
        << ncm3_prefix << "ownship_update "
        << ownship_update << std::endl
        << ncm3_prefix << "sling_tightness_center(%) "
        << to_string(sling_tightness_center) << std::endl
        << ncm3_prefix << "sling_tightness_forward(%) "
        << to_string(sling_tightness_forward) << std::endl
        << ncm3_prefix << "sling_tightness_aft(%) "
        << to_string(sling_tightness_aft) << std::endl
        << ncm3_prefix << "rotor_roll "
        << to_string(rotor_roll) << std::endl
        << ncm3_prefix << "rotor_pitch "
        << to_string(rotor_pitch) << std::endl
        << ncm3_prefix << "rotor_coning_forward(%) "
        << to_string(rotor_coning_forward) << std::endl
        << ncm3_prefix << "rotor_coning_aft(%) "
        << to_string(rotor_coning_aft) << std::endl
        << ncm3_prefix << "rotor_speed(%) "
        << to_string(rotor_speed) << std::endl
        << ncm3_prefix << "lift_vibration_frequency(%) "
        << to_string(lift_vibration_frequency) << std::endl
        << ncm3_prefix << "lift_vibration_amplitude(%) "
        << to_string(lift_vibration_amplitude) << std::endl;

    engine_rates1.print((ncm3_prefix + "engine_rates1."), out);
    engine_rates2.print((ncm3_prefix + "engine_rates2."), out);

    out << ncm3_prefix << "total_airspeed(ft/s) "
        << to_string(total_airspeed) << std::endl
        << ncm3_prefix << "weight_on_wheel_left "
        << (yes_no_e)weight_on_wheel_left << std::endl
        << ncm3_prefix << "weight_on_wheel_right "
        << (yes_no_e)weight_on_wheel_right << std::endl
        << ncm3_prefix << "weight_on_wheel_aft "
        << (yes_no_e)weight_on_wheel_aft << std::endl
        << ncm3_prefix << "groundspeed_x(ft/s) "
        << groundspeed_x << std::endl
        << ncm3_prefix << "groundspeed_y(ft/s) "
        << groundspeed_y << std::endl
        << ncm3_prefix << "gear_squeal_left "
        << (yes_no_e)gear_squeal_left << std::endl
        << ncm3_prefix << "gear_squeal_right "
        << (yes_no_e)gear_squeal_right << std::endl
        << ncm3_prefix << "gear_squeal_aft "
        << (yes_no_e)gear_squeal_aft << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_01_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for avcatt_to_ncm3_01_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    ownship_gcc.read(stream);
    ownship_gdc.read(stream);
    ownship_orientation.read(stream);
    ownship_velocity.read(stream);
    load_id.read(stream);
    stream.read(frozen);
    load_gcc.read(stream);
    stream.read(load_attached);
    load_orientation.read(stream);
    stream.read(search_light);
    stream.read(search_light_azimuth);
    stream.read(search_light_elevation);
    stream.read(water_bucket_full);
    stream.read(place_survivor_on_hoist);
    survivor_id.read(stream);
    stream.read(hook_type);
    stream.read(malfunction);
    stream.read(collision);
    stream.read(ownship_appearance);
    stream.read(ownship_update);
    stream.read(sling_tightness_center);
    stream.read(sling_tightness_forward);
    stream.read(sling_tightness_aft);
    stream.read(rotor_roll);
    stream.read(rotor_pitch);
    stream.read(rotor_coning_forward);
    stream.read(rotor_coning_aft);
    stream.read(rotor_speed);
    stream.read(lift_vibration_frequency);
    stream.read(lift_vibration_amplitude);
    engine_rates1.read(stream);
    engine_rates2.read(stream);
    stream.read(total_airspeed);
    stream.read(weight_on_wheel_left);
    stream.read(weight_on_wheel_right);
    stream.read(weight_on_wheel_aft);
    stream.read(groundspeed_x);
    stream.read(groundspeed_y);
    stream.read(gear_squeal_left);
    stream.read(gear_squeal_right);
    stream.read(gear_squeal_aft);
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_01_t::write(byte_stream_t &stream)
{
    stream.write(length());
    ownship_gcc.write(stream);
    ownship_gdc.write(stream);
    ownship_orientation.write(stream);
    ownship_velocity.write(stream);
    load_id.write(stream);
    stream.write(frozen);
    load_gcc.write(stream);
    stream.write(load_attached);
    load_orientation.write(stream);
    stream.write(search_light);
    stream.write(search_light_azimuth);
    stream.write(search_light_elevation);
    stream.write(water_bucket_full);
    stream.write(place_survivor_on_hoist);
    survivor_id.write(stream);
    stream.write(hook_type);
    stream.write(malfunction);
    stream.write(collision);
    stream.write(ownship_appearance);
    stream.write(ownship_update);
    stream.write(sling_tightness_center);
    stream.write(sling_tightness_forward);
    stream.write(sling_tightness_aft);
    stream.write(rotor_roll);
    stream.write(rotor_pitch);
    stream.write(rotor_coning_forward);
    stream.write(rotor_coning_aft);
    stream.write(rotor_speed);
    stream.write(lift_vibration_frequency);
    stream.write(lift_vibration_amplitude);
    engine_rates1.write(stream);
    engine_rates2.write(stream);
    stream.write(total_airspeed);
    stream.write(weight_on_wheel_left);
    stream.write(weight_on_wheel_right);
    stream.write(weight_on_wheel_aft);
    stream.write(groundspeed_x);
    stream.write(groundspeed_y);
    stream.write(gear_squeal_left);
    stream.write(gear_squeal_right);
    stream.write(gear_squeal_aft);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_01_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    const std::string ncm3_prefix = (prefix + "ncm3_to_avcatt_01.");

    out << ncm3_prefix << "configuration "
        << (ncm3_configuration_e)configuration << std::endl
        << ncm3_prefix << "gun_trigger_left "
        << (ncm3_gun_trigger_e)gun_trigger_left << std::endl
        << ncm3_prefix << "gun_trigger_right "
        << (ncm3_gun_trigger_e)gun_trigger_right << std::endl
        << ncm3_prefix << "gun_trigger_aft "
        << (ncm3_gun_trigger_e)gun_trigger_aft << std::endl
        << ncm3_prefix << "hook_animation_state "
        << (ncm3_hook_animation_e)hook_animation_state << std::endl
        << ncm3_prefix << "released_water "
        << (yes_no_e)released_water << std::endl
        << ncm3_prefix << "released_cargo "
        << (yes_no_e)released_cargo << std::endl
        << ncm3_prefix << "released_flare "
        << (yes_no_e)released_flare << std::endl
        << ncm3_prefix << "hoist_medic_state "
        << (ncm3_attach_state_e)hoist_medic_state << std::endl
        << ncm3_prefix << "hoist_survivor_state "
        << (ncm3_attach_state_e)hoist_survivor_state << std::endl
        << ncm3_prefix << "hoist_available "
        << (yes_no_e)hoist_available << std::endl
        << ncm3_prefix << "medic_orientation "
        << medic_orientation << std::endl
        << ncm3_prefix << "medic_location "
        << medic_location << std::endl
        << ncm3_prefix << "survivor_location "
        << survivor_location << std::endl
        << ncm3_prefix << "survivor_orientation "
        << survivor_orientation << std::endl
        << ncm3_prefix << "sling_load_offset "
        << sling_load_offset << std::endl
        << ncm3_prefix << "sling_load_orientation "
        << sling_load_orientation << std::endl
        << ncm3_prefix << "padding(32 bits) "
        << to_bin_string(padding, true) << std::endl;
 }

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_01_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for ncm3_to_avcatt_01_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    stream.read(configuration);
    stream.read(gun_trigger_left);
    stream.read(gun_trigger_right);
    stream.read(gun_trigger_aft);
    stream.read(hook_animation_state);
    stream.read(released_water);
    stream.read(released_cargo);
    stream.read(released_flare);
    stream.read(hoist_medic_state);
    stream.read(hoist_survivor_state);
    stream.read(hoist_available);
    medic_orientation.read(stream);
    medic_location.read(stream);
    survivor_location.read(stream);
    survivor_orientation.read(stream);
    sling_load_offset.read(stream);
    sling_load_orientation.read(stream);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_01_t::write(byte_stream_t &stream)
{
    stream.write(length());
    stream.write(configuration);
    stream.write(gun_trigger_left);
    stream.write(gun_trigger_right);
    stream.write(gun_trigger_aft);
    stream.write(hook_animation_state);
    stream.write(released_water);
    stream.write(released_cargo);
    stream.write(released_flare);
    stream.write(hoist_medic_state);
    stream.write(hoist_survivor_state);
    stream.write(hoist_available);
    medic_orientation.write(stream);
    medic_location.write(stream);
    survivor_location.write(stream);
    survivor_orientation.write(stream);
    sling_load_offset.write(stream);
    sling_load_orientation.write(stream);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_02_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    const std::string ncm3_prefix = (prefix + "avcatt_to_ncm3_02.");

    maintenance_cautions.print((ncm3_prefix + "maintenance_cautions."), out);
    hydraulic_cautions.print((ncm3_prefix + "hydraulic_cautions."), out);
    hydraulic_gauges.print((ncm3_prefix + "hydraulic_gauges."), out);
    hook_safety.print((ncm3_prefix + "hook_safety."), out);
    uh72_controls.print((ncm3_prefix + "uh72_controls."), out);

    out << ncm3_prefix << "padding(32 bits) "
        << to_bin_string(padding, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_02_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for avcatt_to_ncm3_02_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    maintenance_cautions.read(stream);
    hydraulic_cautions.read(stream);
    hydraulic_gauges.read(stream);
    hook_safety.read(stream);
    uh72_controls.read(stream);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_02_t::write(byte_stream_t &stream)
{
    stream.write(length());
    maintenance_cautions.write(stream);
    hydraulic_cautions.write(stream);
    hydraulic_gauges.write(stream);
    hook_safety.write(stream);
    uh72_controls.write(stream);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_02_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    const std::string ncm3_prefix = (prefix + "ncm3_to_avcatt_02.");

    flare_state.print((ncm3_prefix + "flare_state."), out);
    cargo_hook_state.print((ncm3_prefix + "cargo_hook_state."), out);

    out << ncm3_prefix << "padding(32 bits) "
        << to_bin_string(padding, true) << std::endl;

    water_bucket_state.print((ncm3_prefix + "water_bucket_state."), out);
    hoist_data.print((ncm3_prefix + "hoist_data."), out);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_02_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for ncm3_to_avcatt_02_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    flare_state.read(stream);
    cargo_hook_state.read(stream);
    stream.read(padding);
    water_bucket_state.read(stream);
    hoist_data.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_02_t::write(byte_stream_t &stream)
{
    stream.write(length());
    flare_state.write(stream);
    cargo_hook_state.write(stream);
    stream.write(padding);
    water_bucket_state.write(stream);
    hoist_data.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_03_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    const std::string ncm3_prefix = (prefix + "avcatt_to_ncm3_03.");

    out << ncm3_prefix << "audio_message_queue(16 bytes) ";

    for(uint32_t i = 0; i < 16; ++i) {

        out << to_hex_string(audio_message_queue[i]);

        if (i < 15) {

            out << "-";
        }
    }

    out << std::endl
        << ncm3_prefix << "flare_count " << (int)flare_count << std::endl
        << ncm3_prefix << "padding(2 bytes) "
        << to_bin_string(padding[0]) << "-"
        << to_bin_string(padding[1]) << std::endl
        << ncm3_prefix << "normal_hook_power "
        << (int)normal_hook_power << std::endl
        << ncm3_prefix << "emergency_hook_power "
        << (int)emergency_hook_power << std::endl
        << ncm3_prefix << "audio_message_system "
        << (int)audio_message_system << std::endl
        << ncm3_prefix << "audio_message_active "
        << (yes_no_e)audio_message_active << std::endl
        << to_hex_string(padding[0]) << "-"
        << to_hex_string(padding[1]) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_03_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for avcatt_to_ncm3_03_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    stream.read(audio_message_queue, 16);
    stream.read(flare_count);
    stream.read(normal_hook_power);
    stream.read(emergency_hook_power);
    stream.read(audio_message_system);
    stream.read(padding, 2);
    stream.read(audio_message_active);
}

// ----------------------------------------------------------------------------
void vdis::avcatt_to_ncm3_03_t::write(byte_stream_t &stream)
{
    stream.write(length());
    stream.write(audio_message_queue, 16);
    stream.write(flare_count);
    stream.write(normal_hook_power);
    stream.write(emergency_hook_power);
    stream.write(audio_message_system);
    stream.write(padding, 2);
    stream.write(audio_message_active);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_03_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    const std::string ncm3_prefix = (prefix + "ncm3_to_avcatt_03.");

    out << ncm3_prefix << "padding(8 bytes) "
        << to_hex_string(padding, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_03_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    clear();

    if (length != (LENGTH_BITS / 8))
    {
        LOG_ERROR(
            "Inconsistent datum length for ncm3_to_avcatt_03_t: %d/%d",
            length,
            (LENGTH_BITS / 8));
    }

    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::ncm3_to_avcatt_03_t::write(byte_stream_t &stream)
{
    stream.write(length());
    stream.write(padding);
}
