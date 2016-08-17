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
#include "vdb_binary.h"
#include "vdb_common.h"
#include "vdb_datum.h"
#include "vdb_enums.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_services.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
// Returns the number of bytes of padding based on the length
// of the content (length()).
//
uint32_t vdb::variable_datum_content_t::padding_length(void) const
{
    return get_padding_length(length(), 8);
}

// ----------------------------------------------------------------------------
// Reads 4 bytes for datum length, converts value in bits to value
// in bytes, and checks the byte stream for the needed bytes.
// Returns datum length in bytes.
//
uint32_t vdb::variable_datum_content_t::read_length(byte_stream &stream)
{
    uint32_t
        length = 0;

    // Value should be in bits not bytes
    //
    stream.read(length);

    if ((length % 64) > 0)
    {
        LOG_WARNING(
            "Reading variable datum content with invalid length: %d",
            length);
    }
    else
    {
        // Convert value in bits to bytes
        //
        length /= 8;

        if (length > 0)
        {
            if (stream.get_length_remaining() >= length)
            {
                LOG_EXTRA_VERBOSE(
                    "Reading variable datum content %d bytes...",
                    length);
            }
            else
            {
                LOG_ERROR(
                    "Cannot create variable datum content with "
                    " %d/%d bytes remaining",
                    stream.get_length_remaining(),
                    length);
            }
        }
    }

    return length;
}

// ----------------------------------------------------------------------------
vdb::fixed_datum_t::fixed_datum_t(void) : datum_id(0), datum_value(0)
{

}

// ----------------------------------------------------------------------------
vdb::fixed_datum_t::fixed_datum_t(const fixed_datum_t &copy) :
    datum_id(copy.datum_id),
    datum_value(copy.datum_value)
{

}

// ----------------------------------------------------------------------------
vdb::fixed_datum_t::~fixed_datum_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_t::clear(void)
{
    datum_id = 0;
    datum_value = 0;
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "id "
           << enumerations::print(ENUM_DATUM_IDS, datum_id) << std::endl
           << prefix << "value "
           << (int)datum_value << " ["
           << hexadecimal::str(datum_value) << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_t::read(byte_stream &stream)
{
    stream.read(datum_id);
    stream.read(datum_value);
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_t::write(byte_stream &stream) const
{
    stream.write(datum_id);
    stream.write(datum_value);
}

// ----------------------------------------------------------------------------
vdb::variable_datum_t::variable_datum_t(void) :
    datum_id(0),
    datum_content_ptr(0)
{

}

// ----------------------------------------------------------------------------
vdb::variable_datum_t::variable_datum_t(
    const variable_datum_t &copy
) :
    datum_id(copy.datum_id),
    datum_content_ptr(0)
{
    #define COPY_DATUM(T, V) new T(*static_cast<const T*>(V))

    if (copy.datum_content_ptr)
    {
        switch((datum_ids_e)datum_id)
        {
            case DATUM_IDS_DID_STATUS_DAMAGE_AGGR:
                datum_content_ptr = COPY_DATUM(
                    damage_status_t,
                    copy.datum_content_ptr);
                break;
            case DATUM_IDS_DID_TASK_ORG_FORCE_ID_AFFILIATION:
                datum_content_ptr = COPY_DATUM(
                    force_id_affiliation_t,
                    copy.datum_content_ptr);
                break;
            case DATUM_IDS_DID_SLING_LOAD_CAPABILITY:
                datum_content_ptr = COPY_DATUM(
                    sling_load_capability_t,
                    copy.datum_content_ptr);
                break;
            default:
                datum_content_ptr = COPY_DATUM(
                    default_datum_content_t,
                    copy.datum_content_ptr);
                break;
        }
    }

    #undef COPY_DATUM
}

// ----------------------------------------------------------------------------
vdb::variable_datum_t::~variable_datum_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_t::clear(void)
{
    datum_id = 0;

    if (datum_content_ptr)
    {
        delete datum_content_ptr;
        datum_content_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "id = "
           << enumerations::print(ENUM_DATUM_IDS, datum_id) << std::endl
           << prefix << "length(bytes) = ";

    if (datum_content_ptr)
    {
        stream << datum_content_ptr->length() << std::endl;

        datum_content_ptr->print(prefix, stream);
    }
    else
    {
        stream << "0" << std::endl;
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::variable_datum_t::length(void) const
{
    uint32_t
        length = BASE_LENGTH;

    if (datum_content_ptr)
    {
        length += datum_content_ptr->length();
        length += datum_content_ptr->padding_length();
    }

    return length;
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_t::read(byte_stream &stream)
{
    clear();

    stream.read(datum_id);

    switch((datum_ids_e)datum_id)
    {
        case DATUM_IDS_DID_STATUS_DAMAGE_AGGR:
            datum_content_ptr = new damage_status_t();
            break;
        case DATUM_IDS_DID_TASK_ORG_FORCE_ID_AFFILIATION:
            datum_content_ptr = new force_id_affiliation_t();
            break;
        case DATUM_IDS_DID_SLING_LOAD_CAPABILITY:
            datum_content_ptr = new sling_load_capability_t();
            break;
        default:
            datum_content_ptr = new default_datum_content_t();
            break;
    }

    datum_content_ptr->read(stream);
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_t::write(byte_stream &stream) const
{
    stream.write(datum_id);

    if (datum_content_ptr)
    {
        datum_content_ptr->write(stream);
    }
    else
    {
        // Write length 0 for null content.
        //
        stream.write((uint32_t)0);
    }
}

// ----------------------------------------------------------------------------
vdb::fixed_datum_records_t::fixed_datum_records_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::fixed_datum_records_t::fixed_datum_records_t(
    const fixed_datum_records_t &copy)
{
    for(size_t i = 0, size = copy.records.size(); i < size; ++i)
    {
        records.push_back(copy.records[i]);
    }
}

// ----------------------------------------------------------------------------
vdb::fixed_datum_records_t::~fixed_datum_records_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_records_t::clear(void)
{
    records.clear();
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_records_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].print(
            (prefix + "fixed_datum[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_records_t::print_summary(
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        stream << "  " << (datum_ids_e)records[i].datum_id
               << "  (fixed_datum)" << std::endl;
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::fixed_datum_records_t::length(void) const
{
    return (records.size() * fixed_datum_t::LENGTH);
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_records_t::read(byte_stream &stream)
{
    LOG_ERROR("Cannot read fixed datum records without record count!");
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_records_t::read(
    byte_stream &stream,
    uint32_t record_count)
{
    for(uint32_t i = 0; (i < record_count) and stream(); ++i)
    {
        fixed_datum_t
            record;

        record.read(stream);

        records.push_back(record);
    }
}

// ----------------------------------------------------------------------------
void vdb::fixed_datum_records_t::write(byte_stream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
vdb::variable_datum_records_t::variable_datum_records_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::variable_datum_records_t::variable_datum_records_t(
    const variable_datum_records_t &copy)
{
    for(size_t i = 0, size = copy.records.size(); i < size; ++i)
    {
        records.push_back(copy.records[i]);
    }
}

// ----------------------------------------------------------------------------
vdb::variable_datum_records_t::~variable_datum_records_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_records_t::clear(void)
{
    records.clear();
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_records_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].print(
            (prefix + "variable_datum[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_records_t::print_summary(
    std::ostream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        stream << "  " << (datum_ids_e)records[i].datum_id
               << "  " << records[i].length() << "  (variable)"
               << std::endl;
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::variable_datum_records_t::length(void) const
{
    uint32_t
        length_of_records = 0;

    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        length_of_records += records[i].length();
    }

    return length_of_records;
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_records_t::read(byte_stream &stream)
{
    LOG_ERROR("Cannot read variable datum records without record count!");
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_records_t::read(
    byte_stream &stream,
    uint32_t record_count)
{
    for(uint32_t i = 0; (i < record_count); ++i)
    {
        variable_datum_t
            record;

        record.read(stream);

        records.push_back(record);
    }
}

// ----------------------------------------------------------------------------
void vdb::variable_datum_records_t::write(byte_stream &stream) const
{
    for(size_t i = 0, size = records.size(); i < size; ++i)
    {
        records[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
vdb::default_datum_content_t::default_datum_content_t(void)
{

}

// ----------------------------------------------------------------------------
vdb::default_datum_content_t::default_datum_content_t(
    const default_datum_content_t &copy
) :
    variable_datum_content_t(copy),
    data(copy.data)
{

}

// ----------------------------------------------------------------------------
vdb::default_datum_content_t::~default_datum_content_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::default_datum_content_t::clear(void)
{
    data.clear();
}

// ----------------------------------------------------------------------------
void vdb::default_datum_content_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    data.print(prefix, stream);
}

// ----------------------------------------------------------------------------
uint32_t vdb::default_datum_content_t::length(void) const
{
    return data.length();
}

// ----------------------------------------------------------------------------
void vdb::default_datum_content_t::read(byte_stream &stream)
{
    clear();

    data.data_length = read_length(stream);
    data.read(stream);
}

// ----------------------------------------------------------------------------
void vdb::default_datum_content_t::write(byte_stream &stream) const
{
    data.write(stream);
}

// ----------------------------------------------------------------------------
vdb::damage_status_t::damage_status_t(void) :
    casualties(0),
    cause(0),
    padding8(0),
    extent(0),
    padding32(0)
{

}

// ----------------------------------------------------------------------------
vdb::damage_status_t::damage_status_t(const damage_status_t &copy
) :
    variable_datum_content_t(copy),
    casualties(copy.casualties),
    cause(copy.cause),
    padding8(copy.padding8),
    event_id(copy.event_id),
    extent(copy.extent),
    padding32(copy.padding32)
{

}

// ----------------------------------------------------------------------------
vdb::damage_status_t::~damage_status_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::damage_status_t::clear(void)
{
    casualties = 0;
    cause = 0;
    padding8 = 0;
    event_id.clear();
    extent = 0;
    padding32 = 0;
}

// ----------------------------------------------------------------------------
void vdb::damage_status_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "damage_status.casualties "
           << (int)casualties << std::endl
           << prefix << "damage_status.cause "
           << enumerations::print(ENUM_DAMAGE_CAUSE, cause) << std::endl
           << prefix << "damage_status.padding(1 byte) "
           << hexadecimal::str(padding8) << std::endl
           << prefix << "damage_status.event_id "
           << event_id << std::endl
           << prefix << "damage_status.extent "
           << enumerations::print(ENUM_DAMAGE_EXTENT, extent) << std::endl
           << prefix << "damage_status.padding(4 bytes) "
           << hexadecimal::str(padding32, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::damage_status_t::read(byte_stream &stream)
{
    uint32_t
        record_length = read_length(stream);

    clear();

    if (record_length != LENGTH)
    {
        LOG_WARNING(
            "Unexpected length for damage status record: ",
            record_length);
    }

    stream.read(casualties); // 2 bytes
    stream.read(cause); // 1 byte
    stream.read(padding8); // 1 byte
    stream.read(event_id); // 6 bytes
    stream.read(extent); // 2 bytes
    stream.read(padding32); // 4 bytes
}

// ----------------------------------------------------------------------------
void vdb::damage_status_t::write(byte_stream &stream) const
{
    // Write length as 32-bit unsigned integer (in bits)
    //
    stream.write((uint32_t)(LENGTH * 8));

    stream.write(casualties); // 2 bytes
    stream.write(cause); // 1 byte
    stream.write(padding8); // 1 byte
    stream.write(event_id); // 6 bytes
    stream.write(extent); // 2 bytes
    stream.write(padding32); // 4 bytes
}

// ----------------------------------------------------------------------------
vdb::sling_load_capability_t::sling_load_capability_t(void) :
    drag_coeffficient(0),
    current_mass(0),
    padding(0),
    hook_type(0)
{

}

// ----------------------------------------------------------------------------
vdb::sling_load_capability_t::sling_load_capability_t(
    const sling_load_capability_t &copy
) :
    variable_datum_content_t(copy),
    payload(copy.payload),
    carrier(copy.carrier),
    drag_coeffficient(copy.drag_coeffficient),
    current_mass(copy.current_mass),
    padding(copy.padding),
    hook_type(copy.hook_type),
    lines(copy.lines)
{

}

// ----------------------------------------------------------------------------
vdb::sling_load_capability_t::~sling_load_capability_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::sling_load_capability_t::clear(void)
{
    payload.clear();
    carrier.clear();

    drag_coeffficient = 0;
    current_mass = 0;
    padding = 0;
    hook_type = 0;

    lines.clear();
}

// ----------------------------------------------------------------------------
void vdb::sling_load_capability_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "sling_load_capability.payload " << payload << std::endl
           << prefix << "sling_load_capability.carrier " << carrier << std::endl
           << prefix << "sling_load_capability.drag_coeffficient "
           << float_to_string(drag_coeffficient) << std::endl
           << prefix << "sling_load_capability.current_mass "
           << float_to_string(current_mass) << std::endl
           << prefix << "sling_load_capability.padding "
           << binary::str(padding, true) << std::endl
           << prefix << "sling_load_capability.hook_type "
           << enumerations::print(ENUM_HOOK_TYPE, hook_type) << std::endl
           << prefix << "sling_load_capability.lines "
           << lines.size() << std::endl;

    for(uint32_t i = 0, size = lines.size(); i < size; ++i)
    {
        stream << prefix << "sling_load_capability.lines[" << i << "].length "
               << float_to_string(lines[i].first) << std::endl
               << prefix << "sling_load_capability.lines[" << i << "].offset "
               << float_to_string(lines[i].second) << std::endl;
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::sling_load_capability_t::length(void) const
{
    return (BASE_LENGTH + (lines.size() * 8));
}

// ----------------------------------------------------------------------------
void vdb::sling_load_capability_t::read(byte_stream &stream)
{
    uint8_t
        lines_needed = 0;

    clear();
    read_length(stream);

    stream.read(payload); // 6 bytes
    stream.read(carrier); // 6 bytes
    stream.read(drag_coeffficient); // 4 bytes
    stream.read(current_mass); // 4 bytes
    stream.read(padding); // 2 bytes padding
    stream.read(hook_type); // 1 byte
    stream.read(lines_needed); // 1 byte

    for(uint8_t i = 0; i < lines_needed; ++i)
    {
        sling_line_t
            line;

        stream.read(line.first); // 4 bytes
        stream.read(line.second); // 4 bytes

        lines.push_back(line);
    }
}

// ----------------------------------------------------------------------------
void vdb::sling_load_capability_t::write(byte_stream &stream) const
{
    // TODO: vdb::sling_load_capability_t::write
}

// ----------------------------------------------------------------------------
vdb::force_id_affiliation_t::force_id_affiliation_t(void) :
    force(0),
    padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::force_id_affiliation_t::force_id_affiliation_t(
    const force_id_affiliation_t &copy
) :
    variable_datum_content_t(copy),
    force(copy.force),
    padding(copy.padding),
    name(copy.name)
{

}

// ----------------------------------------------------------------------------
vdb::force_id_affiliation_t::~force_id_affiliation_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::force_id_affiliation_t::clear(void)
{
    force = 0;
    padding = 0;

    name.clear();
}

// ----------------------------------------------------------------------------
void vdb::force_id_affiliation_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "force_id_affiliation.force "
           << enumerations::print(ENUM_FORCE_ID, force) << std::endl
           << prefix << "force_id_affiliation.name '"
           << name << "'" << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::force_id_affiliation_t::length(void) const
{
    return (BASE_LENGTH + name.size());
}

// ----------------------------------------------------------------------------
void vdb::force_id_affiliation_t::read(byte_stream &stream)
{
    const uint32_t
        length = read_length(stream);
    uint32_t
        name_length = 0;

    clear();

    stream.read(force); // 2 bytes
    stream.read(padding); // 2 bytes

    name_length = (length - BASE_LENGTH);

    if (name_length > 0)
    {
        char
            name_buffer[name_length + 1];

        stream.read(name_buffer, name_length);

        // Replace null characters with spaces, sometimes there are leading
        // zeros...
        //
        for(uint32_t i = 0; i < name_length; ++i)
        {
            name_buffer[i] = (name_buffer[i] == 0) ? ' ' : name_buffer[i];
        }

        name = trim(name_buffer);
    }
}

// ----------------------------------------------------------------------------
void vdb::force_id_affiliation_t::write(byte_stream &stream) const
{
    // TODO: vdb::force_id_affiliation_t::write
}
