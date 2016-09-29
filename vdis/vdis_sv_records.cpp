#include "vdis_byte_stream.h"
#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"
#include "vdis_sv_records.h"

// ----------------------------------------------------------------------------
vdis::sv_record_t **vdis::read_sv_records(
    byte_stream_t &stream,
    uint16_t count)
{
    sv_record_t
        **records = new sv_record_t*[count];

    for(uint32_t i = 0; i < count; ++i)
    {
        sv_record_t
            *record_ptr = new sv_record_t;

        LOG_EXTRA_VERBOSE(
            "Reading standard variable record %d/%d with stream at index %d/%d",
            (i + 1),
            count,
            stream.index(),
            stream.length());

        record_ptr->read(stream);

        records[i] = record_ptr;
    }

    return records;
}

// ----------------------------------------------------------------------------
// Reads 2 bytes for record length, converts value in bits to value
// in bytes, and checks the byte stream for the needed bytes.
// Returns datum length in bytes.
//
uint16_t vdis::sv_content_t::read_length(byte_stream_t &stream)
{
    uint16_t
        length = 0;

    // Value should be in bytes, and includes the 4-byte record type
    // and 2-byte length that being read meaning the minimum length is
    // 8 bytes.
    //
    stream.read(length);

    if ((length < 8) or (length % 8) > 0)
    {
        LOG_WARNING(
            "Reading standard variable record content with invalid length: %d",
            length);
    }
    else
    {
        // Subtract header bytes
        //
        length -= 6;

        if (stream.remaining_length() >= length)
        {
            LOG_EXTRA_VERBOSE(
                "Reading standard variable record content %d bytes...",
                length);
        }
        else
        {
            LOG_ERROR(
                "Cannot create standard variable record content with "
                " %d/%d bytes remaining",
                stream.remaining_length(),
                length);
        }
    }

    return length;
}

// ----------------------------------------------------------------------------
void vdis::sv_record_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "type " << datum_id_enum() << std::endl;

    if (content_ptr)
    {
        content_ptr->print(prefix, out);
    }
}

// ----------------------------------------------------------------------------
void vdis::sv_record_t::read(byte_stream_t &stream)
{
    clear();

    stream.read(type);

    LOG_EXTRA_VERBOSE("Reading standard variable record type %d...", type);

    switch(type)
    {
        case DATUM_IDS_DID_APPLICATION_STATE_RECORD:
            LOG_EXTRA_VERBOSE("Creating record: application_state_t");
            content_ptr = new application_state_t();
            break;
        case DATUM_IDS_DID_APPLICATION_HEALTH_STATUS_RECORD:
            LOG_EXTRA_VERBOSE("Creating record: application_health_status_t");
            content_ptr = new application_health_status_t();
            break;
        case DATUM_IDS_DID_OTHER_STEALTH_SPECTRUM:
            LOG_EXTRA_VERBOSE("Creating record: stealth_spectrum_t");
            content_ptr = new stealth_spectrum_t();
            break;
        case DATUM_IDS_DID_OTHER_STEALTH_LOCATION:
            LOG_EXTRA_VERBOSE("Creating record: stealth_location_t");
            content_ptr = new stealth_location_t();
            break;
        case DATUM_IDS_DID_OTHER_STEALTH_ATTACHMENT_DATA:
            LOG_EXTRA_VERBOSE("Creating record: stealth_attachment_t");
            content_ptr = new stealth_attachment_t();
            break;
        case DATUM_IDS_DID_OTHER_STEALTH_VEL:
            LOG_EXTRA_VERBOSE("Creating record: stealth_velocity_t");
            content_ptr = new stealth_velocity_t();
            break;
        case DATUM_IDS_DID_OTHER_STEALTH_FIELD_OF_VIEW:
            LOG_EXTRA_VERBOSE("Creating record: stealth_field_of_view_t");
            content_ptr = new stealth_field_of_view_t();
            break;
        case DATUM_IDS_DID_OTHER_STEALTH_MARKING_ID:
            LOG_EXTRA_VERBOSE("Creating record: stealth_marking_id_t");
            content_ptr = new stealth_marking_id_t();
            break;
        default:
            LOG_EXTRA_VERBOSE("Creating record: default_sv_content_t");
            content_ptr = new default_sv_content_t;
    }

    // Should read any padding in the record.
    //
    content_ptr->read(stream);
}

// ----------------------------------------------------------------------------
void vdis::sv_record_t::write(byte_stream_t &stream)
{
    stream.write(type);

    if (content_ptr)
    {
        // This write operation includes write the 2-byte record length.
        //
        content_ptr->write(stream);
    }
    else
    {
        // Write dummy data (length 8, 2 null bytes padding).
        //
        stream.write((uint32_t)0x00080000);
    }
}

// ----------------------------------------------------------------------------
void vdis::default_sv_content_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl;

    buffer.print((prefix + "data "), out);
}

// ----------------------------------------------------------------------------
void vdis::default_sv_content_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    stream.read(buffer, length);
}

// ----------------------------------------------------------------------------
void vdis::default_sv_content_t::write(byte_stream_t &stream)
{
    uint16_t record_length = (length() + 6);

    stream.write(record_length);
    stream.write(buffer);
}

// ----------------------------------------------------------------------------
uint16_t vdis::application_state_t::length(void) const
{
    uint16_t record_length = 0;

    record_length += 4; // Record type (4 bytes)
    record_length += 2; // Record length (2 bytes)
    record_length += BASE_LENGTH_BYTES;
    record_length += (exercise_count * 4);
    record_length += padding_length(record_length, 8);

    return record_length;
}

// ----------------------------------------------------------------------------
void vdis::application_state_t::clear(void)
{
    for(uint8_t i = 0; exercises and (i < exercise_count); ++i)
    {
        delete exercises[i];
        exercises[i] = 0;
    }

    if (exercises)
    {
        delete[] exercises;
    }

    padding = 0;
    transition = 0;
    current_state = 0;
    requested_state = 0;
    current_configuration = 0;
    exercise_count = 0;
    exercises = 0;
}

// ----------------------------------------------------------------------------
void vdis::application_state_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "application_state.padding(8 bits) "
        << to_bin_string(padding) << std::endl
        << prefix << "application_state.transition "
        << transition_enum() << std::endl
        << prefix << "application_state.current_state "
        << current_state_enum() << std::endl
        << prefix << "application_state.requested_state "
        << requested_state_enum() << std::endl
        << prefix << "application_state.current_configuration "
        << (int)current_configuration << std::endl
        << prefix << "application_state.exercises.count "
        << (int)exercise_count << std::endl;

    for(uint32_t i = 0; exercises and (i < exercise_count); ++i)
    {
        exercises[i]->print(
            (prefix + "application_state.exercises[" + to_string(i) + "]."),
            out);
    }
}

// ----------------------------------------------------------------------------
void vdis::application_state_t::read(byte_stream_t &stream)
{
    clear();

    read_length(stream);

    stream.read(padding);
    stream.read(transition);
    stream.read(current_state);
    stream.read(requested_state);
    stream.read(current_configuration);
    stream.read(exercise_count);

    if (exercise_count > 0)
    {
        exercises = new exercise_state_t*[exercise_count];

        for(uint8_t i = 0; i < exercise_count; ++i)
        {
            exercise_state_t *state_ptr = new exercise_state_t;

            state_ptr->read(stream);

            exercises[i] = state_ptr;
        }

        // Four bytes of padding if number of exercises is even...
        //
        if ((exercise_count % 2) == 0)
        {
            stream.skip(4);
        }
    }
}

// ----------------------------------------------------------------------------
void vdis::application_state_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(padding);
    stream.write(transition);
    stream.write(current_state);
    stream.write(requested_state);
    stream.write(current_configuration);
    stream.write(exercise_count);

    for(uint8_t i = 0; exercises and (i < exercise_count); ++i)
    {
        exercises[i]->write(stream);
    }

    // Four bytes of padding if number of exercises is even...
    //
    if ((exercise_count % 2) == 0)
    {
        stream.write((uint32_t)0);
    }
}

// ----------------------------------------------------------------------------
void vdis::application_health_status_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "application_health_status.padding(16 bits) "
        << to_bin_string(padding, true) << std::endl
        << prefix << "application_health_status.status_type "
        << status_type << std::endl
        << prefix << "application_health_status.general_status "
        << general_status_enum() << std::endl
        << prefix << "application_health_status.specific_status "
        << (int)specific_status << std::endl
        << prefix << "application_health_status.value_integer "
        << value_integer << std::endl
        << prefix << "application_health_status.value_float "
        << to_string(value_float, 1, 5) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::application_health_status_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for application_health_status_t")
    }

    stream.read(padding);
    stream.read(status_type);
    stream.read(general_status);
    stream.read(specific_status);
    stream.read(value_integer);
    stream.read(value_float);
}

// ----------------------------------------------------------------------------
void vdis::application_health_status_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(padding);
    stream.write(status_type);
    stream.write(general_status);
    stream.write(specific_status);
    stream.write(value_integer);
    stream.write(value_float);
}

// ----------------------------------------------------------------------------
void vdis::stealth_spectrum_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "stealth_spectrum.spectrum_type "
        << spectrum_type_enum() << std::endl
        << prefix << "stealth_spectrum.invert "
        << invert_enum() << std::endl
        << prefix << "stealth_spectrum.magnification "
        << to_string(magnification, 1, 5) << std::endl
        << prefix << "stealth_spectrum.sensor_mode "
        << sensor_mode_enum() << std::endl
        << prefix << "stealth_spectrum.padding(16 bits) "
        << to_bin_string(padding, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stealth_spectrum_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for stealth_spectrum_t: %d", length);
    }

    stream.read(spectrum_type);
    stream.read(invert);
    stream.read(magnification);
    stream.read(sensor_mode);
    stream.read(padding);
}

// ----------------------------------------------------------------------------
void vdis::stealth_spectrum_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(spectrum_type);
    stream.write(invert);
    stream.write(magnification);
    stream.write(sensor_mode);
    stream.write(padding);
}

// ----------------------------------------------------------------------------
void vdis::stealth_location_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "stealth_location.padding(16 bits) "
        << to_bin_string(padding16, true) << std::endl
        << prefix << "stealth_location.location "
        << location << std::endl
        << prefix << "stealth_location.orientation "
        << orientation << std::endl
        << prefix << "stealth_location.padding(32 bits) "
        << to_bin_string(padding32, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stealth_location_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for stealth_location_t: %d", length);
    }

    stream.read(padding16);
    location.read(stream);
    orientation.read(stream);
    stream.read(padding32);
}

// ----------------------------------------------------------------------------
void vdis::stealth_location_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(padding16);
    location.write(stream);
    orientation.write(stream);
    stream.write(padding32);
}

// ----------------------------------------------------------------------------
void vdis::stealth_attachment_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "stealth_attachment.padding(16 bits) "
        << to_bin_string(padding16, true) << std::endl
        << prefix << "stealth_attachment.attached_entity "
        << attached_entity << std::endl
        << prefix << "stealth_attachment.attach_command "
        << attach_command_enum() << std::endl
        << prefix << "stealth_attachment.attach_mode "
        << attach_mode_enum() << std::endl
        << prefix << "stealth_attachment.relative_location "
        << relative_location << std::endl
        << prefix << "stealth_attachment.relative_orientation "
        << relative_orientation << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stealth_attachment_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for stealth_attachment_t: %d", length);
    }

    stream.read(padding16);
    attached_entity.read(stream);
    stream.read(attach_command);
    stream.read(attach_mode);
    relative_location.read(stream);
    relative_orientation.read(stream);
}

// ----------------------------------------------------------------------------
void vdis::stealth_attachment_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(padding16);
    attached_entity.write(stream);
    stream.write(attach_command);
    stream.write(attach_mode);
    relative_location.write(stream);
    relative_orientation.write(stream);
}

// ----------------------------------------------------------------------------
void vdis::stealth_velocity_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "stealth_velocity.relative "
        << relative_enum() << std::endl
        << prefix << "stealth_velocity.algorithm "
        << algorithm_enum() << std::endl
        << prefix << "stealth_velocity.linear_velocity "
        << linear_velocity << std::endl
        << prefix << "stealth_velocity.angular_velocity "
        << angular_velocity << std::endl
        << prefix << "stealth_velocity.linear_acceleration "
        << linear_acceleration << std::endl
        << prefix << "stealth_velocity.padding(4 bytes) "
        << to_hex_string(padding32, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stealth_velocity_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for stealth_velocity_t: %d", length);
    }

    stream.read(relative);
    stream.read(algorithm);
    linear_velocity.read(stream);
    angular_velocity.read(stream);
    linear_acceleration.read(stream);
    stream.read(padding32);
}

// ----------------------------------------------------------------------------
void vdis::stealth_velocity_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(relative);
    stream.write(algorithm);
    linear_velocity.write(stream);
    angular_velocity.write(stream);
    linear_acceleration.write(stream);
    stream.write(padding32);
}

// ----------------------------------------------------------------------------
void vdis::stealth_field_of_view_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "stealth_field_of_view.padding(16 bits) "
        << to_bin_string(padding16, true) << std::endl
        << prefix << "stealth_field_of_view.horizontal "
        << to_string(horizontal, 1, 4) << std::endl
        << prefix << "stealth_field_of_view.vertical "
        << to_string(vertical, 1, 4) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stealth_field_of_view_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for stealth_field_of_view_t: %d", length);
    }

    stream.read(padding16);
    stream.read(horizontal);
    stream.read(vertical);
}

// ----------------------------------------------------------------------------
void vdis::stealth_field_of_view_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(padding16);
    stream.write(horizontal);
    stream.write(vertical);
}

// ----------------------------------------------------------------------------
void vdis::stealth_marking_id_t::print(
    const std::string &prefix,
    std::ostream &out) const
{
    out << prefix << "length " << (int)length() << std::endl
        << prefix << "stealth_marking_id.id "
        << (int)id << std::endl
        << prefix << "stealth_marking_id.marking "
        << marking << std::endl
        << prefix << "stealth_marking_id.padding32(4 bytes) "
        << to_hex_string(padding32, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::stealth_marking_id_t::read(byte_stream_t &stream)
{
    clear();

    uint16_t length = read_length(stream);

    if (length != LENGTH_BYTES)
    {
        LOG_ERROR("Inconsistent length for stealth_marking_id_t: %d", length);
    }

    stream.read(id);
    marking.read(stream);
    stream.read(padding32);
}

// ----------------------------------------------------------------------------
void vdis::stealth_marking_id_t::write(byte_stream_t &stream)
{
    uint16_t record_length = length();

    stream.write(record_length);
    stream.write(id);
    marking.write(stream);
    stream.write(padding32);
}
