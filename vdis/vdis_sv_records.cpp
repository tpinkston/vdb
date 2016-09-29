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
            "Reading standard record %d/%d with stream at index %d/%d",
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
            "Reading standard record content with invalid length: %d",
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
                "Reading standard record content %d bytes...",
                length);
        }
        else
        {
            LOG_ERROR(
                "Cannot create standard record content with "
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
            content_ptr = new application_state_t();
            break;
        default:
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
    record_length += BASE_LENGTH_BITS;
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
    out << prefix << "application_state.padding "
        << (int)padding << std::endl
        << prefix << "application_state.transition "
        << (int)transition << std::endl
        << prefix << "application_state.current_state "
        << (int)current_state << std::endl
        << prefix << "application_state.requested_state "
        << (int)requested_state << std::endl
        << prefix << "application_state.current_configuration "
        << (int)current_configuration << std::endl
        << prefix << "application_state.exercise_states "
        << (int)exercise_count << std::endl;

    for(uint32_t i = 0; exercises and (i < exercise_count); ++i)
    {
        exercises[i]->print(
            (prefix + "application_state.exercise_states[" + to_string(i) + "]."),
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
    uint16_t record_length = (length() + 6);

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

