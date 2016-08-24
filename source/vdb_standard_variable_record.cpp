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
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_services.h"
#include "vdb_standard_variable_record.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
vdb::svr_t::svr_t(void) : type(0), content_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdb::svr_t::svr_t(const svr_t &copy) : type(copy.type), content_ptr(0)
{
    #define COPY_SVR(T, V) new T(*static_cast<const T*>(V))

    switch(type)
    {
        case DATUM_IDS_DID_APPLICATION_STATE_RECORD:
            content_ptr = COPY_SVR(application_state_t, copy.content_ptr);
            break;
        default:
            content_ptr = COPY_SVR(svr_default_content_t, copy.content_ptr);
    }

    #undef COPY_SVR
}

// ----------------------------------------------------------------------------
vdb::svr_t::~svr_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::svr_t::clear(void)
{
    type = 0;

    if (content_ptr)
    {
        delete content_ptr;
        content_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::svr_t::print(const std::string &prefix, std::ostream &stream) const
{
    stream << prefix << "type ";

    if (enumerations::valid(ENUM_ENV_GEOMETRY_REC_TYPE, type))
    {
        stream << enumerations::print(ENUM_ENV_GEOMETRY_REC_TYPE, type)
               << std::endl;
    }
    else if (enumerations::valid(ENUM_ENV_STATE_REC_TYPE, type))
    {
        stream << enumerations::print(ENUM_ENV_STATE_REC_TYPE, type)
               << std::endl;
    }
    else if (enumerations::valid(ENUM_DATUM_IDS, type))
    {
        stream << enumerations::print(ENUM_DATUM_IDS, type)
               << std::endl;
    }
    else
    {
        stream << type << " (0x" << hexadecimal::str(type) << ")"
               << std::endl;
    }

    if (content_ptr)
    {
        content_ptr->print(prefix, stream);
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::svr_t::length(void) const
{
    uint32_t
        record_length = BASE_LENGTH;

    if (content_ptr)
    {
        record_length += content_ptr->length();
    }

    return (record_length + get_padding_length(record_length));
}

// ----------------------------------------------------------------------------
void vdb::svr_t::read(byte_stream &stream)
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
            content_ptr = new svr_default_content_t();
    }

    // Should read any padding in the record.
    //
    content_ptr->read(stream);
}

// ----------------------------------------------------------------------------
void vdb::svr_t::write(byte_stream &stream) const
{
    stream.write(type);

    if (content_ptr)
    {
        content_ptr->write(stream);
    }
    else
    {
        // Write dummy data (length 8, 2 null bytes padding).
        //
        stream.write((uint32_t)0x80000);
    }
}

// ----------------------------------------------------------------------------
// Returns the 32-bit value for the length of the record read from the stream
// minus the bytes for the record and record length.
//
uint32_t vdb::svr_content_t::read_length(byte_stream &stream)
{
    uint16_t
        record_length = 0;
    uint32_t
        length = 0;

    stream.read(record_length);

    LOG_EXTRA_VERBOSE("Standard variable record length: %d...", record_length);

    if (record_length >= 8)
    {
        length = ((uint32_t)record_length - svr_t::BASE_LENGTH);
    }
    else
    {
        LOG_ERROR(
            "Invalid length for standard variable record: %d",
            record_length);
    }

    return length;
}

// ----------------------------------------------------------------------------
vdb::svr_default_content_t::svr_default_content_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
vdb::svr_default_content_t::svr_default_content_t(
    const svr_default_content_t &copy
) :
    data(copy.data)
{

}

// ----------------------------------------------------------------------------
vdb::svr_default_content_t::~svr_default_content_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::svr_default_content_t::clear(void)
{
    data.clear();
}

// ----------------------------------------------------------------------------
void vdb::svr_default_content_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    data.print(prefix, stream);
}

// ----------------------------------------------------------------------------
uint32_t vdb::svr_default_content_t::length(void) const
{
    return data.length();
}

// ----------------------------------------------------------------------------
void vdb::svr_default_content_t::read(byte_stream &stream)
{
    clear();

    data.data_length = read_length(stream);
    data.read(stream);
}

// ----------------------------------------------------------------------------
void vdb::svr_default_content_t::write(byte_stream &stream) const
{
    uint16_t
        record_length = svr_t::BASE_LENGTH;

    record_length += length();
    record_length += get_padding_length(record_length);

    stream.write(record_length);
    stream.write(data);
}

// ----------------------------------------------------------------------------
vdb::exercise_state_t::exercise_state_t(void) :
    id(0),
    transition(0),
    current_state(0),
    requested_state(0)
{

}

// ----------------------------------------------------------------------------
vdb::exercise_state_t::exercise_state_t(const exercise_state_t &copy) :
    id(copy.id),
    transition(copy.current_state),
    current_state(copy.current_state),
    requested_state(copy.requested_state)
{

}

// ----------------------------------------------------------------------------
vdb::exercise_state_t::~exercise_state_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::exercise_state_t::clear(void)
{
    id = 0;
    transition = 0;
    current_state = 0;
    requested_state = 0;
}

// ----------------------------------------------------------------------------
void vdb::exercise_state_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "id " << (int)id << std::endl
           << prefix << "transition " << (int)transition << std::endl
           << prefix << "current_state " << (int)current_state << std::endl
           << prefix << "requested_state " << (int)requested_state << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::exercise_state_t::read(byte_stream &stream)
{
    stream.read(id);
    stream.read(transition);
    stream.read(current_state);
    stream.read(requested_state);
}

// ----------------------------------------------------------------------------
void vdb::exercise_state_t::write(byte_stream &stream) const
{
    stream.write(id);
    stream.write(transition);
    stream.write(current_state);
    stream.write(requested_state);
}

// ----------------------------------------------------------------------------
vdb::application_state_t::application_state_t(void) :
    padding(0),
    transition(0),
    current_state(0),
    requested_state(0),
    current_configuration(0)
{

}

// ----------------------------------------------------------------------------
vdb::application_state_t::application_state_t(
    const application_state_t &copy
) :
    padding(copy.padding),
    transition(copy.current_state),
    current_state(copy.current_state),
    requested_state(copy.requested_state),
    current_configuration(copy.current_configuration),
    exercise_states(copy.exercise_states)
{

}

// ----------------------------------------------------------------------------
vdb::application_state_t::~application_state_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::application_state_t::clear(void)
{
    padding = 0;
    transition = 0;
    current_state = 0;
    requested_state = 0;
    current_configuration = 0;
    exercise_states.clear();
}

// ----------------------------------------------------------------------------
void vdb::application_state_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "application_state.padding "
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
           << (int)exercise_states.size() << std::endl;

    for(uint32_t i = 0; i < exercise_states.size(); ++i)
    {
        exercise_states[i].print(
            (prefix + "application_state.exercise_states[" +
                generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::application_state_t::length(void) const
{
    return (BASE_LENGTH + (((exercise_states.size() % 2) == 1) ? 4 : 0));
}

// ----------------------------------------------------------------------------
void vdb::application_state_t::read(byte_stream &stream)
{
    uint8_t
        exercise_count = 0;

    clear();
    read_length(stream);

    stream.read(padding);
    stream.read(transition);
    stream.read(current_state);
    stream.read(requested_state);
    stream.read(current_configuration);
    stream.read(exercise_count);

    for(uint8_t i = 0; i < exercise_count; ++i)
    {
        exercise_state_t
            state;

        state.read(stream);

        exercise_states.push_back(state);
    }

    // Four bytes of padding if number of exercises is even...
    //
    if ((exercise_count % 2) == 0)
    {
        stream.skip(4);
    }
}

// ----------------------------------------------------------------------------
void vdb::application_state_t::write(byte_stream &stream) const
{
    const uint8_t
        exercise_count = exercise_states.size();

    stream.write(padding);
    stream.write(transition);
    stream.write(current_state);
    stream.write(requested_state);
    stream.write(current_configuration);
    stream.write(exercise_count);

    for(uint8_t i = 0; i < exercise_states.size(); ++i)
    {
        exercise_states[i].write(stream);
    }

    // Four bytes of padding if number of exercises is even...
    //
    if ((exercise_count % 2) == 0)
    {
        stream.write((uint32_t)0);
    }
}

// ----------------------------------------------------------------------------
vdb::standard_variable_records_t::standard_variable_records_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::standard_variable_records_t::standard_variable_records_t(
    const standard_variable_records_t &copy)
{
    for(size_t i = 0, size = copy.records.size(); i < size; ++i)
    {
        records.push_back(copy.records[i]);
    }
}

// ----------------------------------------------------------------------------
vdb::standard_variable_records_t::~standard_variable_records_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::standard_variable_records_t::clear(void)
{
    records.clear();
}

// ----------------------------------------------------------------------------
void vdb::standard_variable_records_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].print(
            (prefix + "record[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::standard_variable_records_t::print_summary(
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        stream << "  " << records[i].type
               << "  (" << records[i].length() << " bytes)" << std::endl;
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::standard_variable_records_t::length(void) const
{
    uint32_t
        record_length = BASE_LENGTH;

    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        record_length += records[i].length();
    }

    return record_length;
}

// ----------------------------------------------------------------------------
void vdb::standard_variable_records_t::read(byte_stream &stream)
{
    uint16_t
        record_count = 0;

    stream.read(record_count);

    LOG_EXTRA_VERBOSE("Reading %d standard variable records...", record_count);

    for(uint16_t i = 0; (i < record_count); ++i)
    {
        svr_t
            record;

        record.read(stream);
        records.push_back(record);
    }
}

// ----------------------------------------------------------------------------
void vdb::standard_variable_records_t::write(byte_stream &stream) const
{
    const uint16_t
        record_count = records.size();

    stream.write(record_count);

    for(uint16_t i = 0, size = record_count; i < size; ++i)
    {
        records[i].write(stream);
    }
}
