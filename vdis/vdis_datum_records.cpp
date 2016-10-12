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
    const string_t &prefix,
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

    // Value should be in bits not bytes
    //
    stream.read(length_bits);

    if ((length_bits % 64) > 0)
    {
        LOG_WARNING(
            "Reading variable datum content with invalid length: %d",
            length_bits);
    }
    else
    {
        // Convert value in bits to bytes
        //
        uint32_t length_bytes = (length_bits / 8);

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

    return length_bits;
}

// ----------------------------------------------------------------------------
vdis::variable_datum_record_t::variable_datum_record_t(void) :
    id(0),
    content_ptr(0)
{
    LOG_EXTRA_VERBOSE(
        "Created variable datum record with content: %p",
        content_ptr);
}

// ----------------------------------------------------------------------------
vdis::variable_datum_record_t::~variable_datum_record_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdis::variable_datum_record_t::clear(void)
{
    LOG_EXTRA_VERBOSE(
        "Clearing variable datum record with content: %p",
        content_ptr);

    if (content_ptr)
    {
        delete content_ptr;
    }

    id = 0;
    content_ptr = 0;
}

// ----------------------------------------------------------------------------
void vdis::variable_datum_record_t::print(
    const string_t &prefix,
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
        {
            content_ptr = new damage_status_t;
            break;
        }
        case DATUM_IDS_DID_SLING_LOAD_CAPABILITY:
        {
            content_ptr = new sling_load_capability_t;
            break;
        }
        case DATUM_IDS_DID_TASK_ORG_FORCE_ID_AFFILIATION:
        {
            content_ptr = new force_id_affiliation_t;
            break;
        }
        default:
        {
            content_ptr = new default_variable_datum_content_t;
            break;
        }
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
    const string_t &prefix,
    std::ostream &out) const
{
    if (buffer.length() > 0)
    {
        buffer.print((prefix + "data"), out);
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
    buffer.read(stream, (length / 8));
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
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "damage_status.casualties "
        << (int)casualties << std::endl
        << prefix << "damage_status.cause "
        << cause_enum() << std::endl
        << prefix << "damage_status.padding "
        << to_hex_string(padding1) << std::endl
        << prefix << "damage_status.event_id "
        << event_id << std::endl
        << prefix << "damage_status.extent "
        << extent_enum() << std::endl
        << prefix << "damage_status.padding "
        << to_hex_string(padding2, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::damage_status_t::read(byte_stream_t &stream)
{
    const uint32_t length = read_length(stream);

    if (length != LENGTH_BITS)
    {
        LOG_ERROR(
            "Inconsistent datum length for damage_status_t: %d/%d",
            length,
            LENGTH_BITS);
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
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "sling_load_capability.payload "
        << payload << std::endl
        << prefix << "sling_load_capability.carrier "
        << carrier << std::endl
        << prefix << "sling_load_capability.drag_coeffficient "
        << to_string(drag_coeffficient, 1, 4) << std::endl
        << prefix << "sling_load_capability.current_mass "
        << to_string(current_mass, 1, 4) << std::endl
        << prefix << "sling_load_capability.padding "
        << to_hex_string(padding, true) << std::endl
        << prefix << "sling_load_capability.hook_type "
        << hook_type_enum() << std::endl
        << prefix << "sling_load_capability.lines_needed "
        << lines_needed << std::endl;

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

    if (length < BASE_LENGTH_BITS)
    {
        LOG_ERROR(
            "Inconsistent datum length for sling_load_capability_t: %d/%d",
            length,
            BASE_LENGTH_BITS);
    }

    payload.read(stream);
    carrier.read(stream);
    stream.read(drag_coeffficient);
    stream.read(current_mass);
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
    stream.write(hook_type);
    stream.write(lines_needed);

    for(uint8_t i = 0; ((i < lines_needed) and lines); ++i)
    {
        lines[i]->write(stream);
    }
}

// ----------------------------------------------------------------------------
string_t vdis::force_id_affiliation_t::force_name(void) const
{
    if (name.length())
    {
        return (const char *)name.buffer();
    }
    else
    {
        return "(NULL)";
    }
}

// ----------------------------------------------------------------------------
uint32_t vdis::force_id_affiliation_t::length(void) const
{
    // The 'name' buffer's length includes trailing zero bytes for padding
    //
    return (BASE_LENGTH_BITS + name.length());
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
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "force_id_affiliation.force " << force_enum() << std::endl
        << prefix << "force_id_affiliation.name " << force_name() << std::endl;
}

// ----------------------------------------------------------------------------
void vdis::force_id_affiliation_t::read(byte_stream_t &stream)
{
    const uint32_t
        length = (read_length(stream) / 8);
    uint32_t
        name_length = 0;

    clear();

    stream.read(force); // 2 bytes
    stream.read(padding); // 2 bytes

    name_length = (length - BASE_LENGTH_BITS);

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
