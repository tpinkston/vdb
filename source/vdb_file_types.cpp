#include "vdb_file_types.h"

#include "vdis_byte_stream.h"
#include "vdis_logger.h"
#include "vdis_services.h"

const uint32_t
    vdb::file_header_t::BASE_RECORD_SIZE = 32, // bytes
    vdb::file_header_comment_t::BASE_RECORD_SIZE = 16, // bytes
    vdb::file_header_comment_t::BOUNDARY_SIZE = 8, // bytes
    vdb::file_header_record_t::RECORD_SIZE = 16; // bytes

// ----------------------------------------------------------------------------
void vdb::file_header_t::print(
    const string_t &prefix,
    std::ostream &stream) const
{
    stream << prefix << "File version: " << (int)major_version << "."
           << (int)minor_version << std::endl
           << prefix << "File created: "
           << vdis::time_to_string(time_created) << std::endl;

    for(uint32_t i = 0; i < comments.size(); ++i)
    {
        print_comment(prefix, i, stream);
    }
}

// ----------------------------------------------------------------------------
std::ostream &vdb::file_header_t::print_comment(
    const string_t &prefix,
    uint32_t index,
    std::ostream &out) const
{
    out << "---- " << color::bold_yellow << "COMMENT " << (index + 1)
        << color::none << ": ";

    if (index < comments.size())
    {
        comments[index].print(prefix, out);
    }
    else
    {
        out << prefix
            << color::red << "(invalid index: " << index << ")"
            << color::none << std::endl;
    }

    return out;
}

// ----------------------------------------------------------------------------
bool vdb::file_header_t::invalid_title(void) const
{
    return (title[0] != 'v') ||
           (title[1] != 'd') ||
           (title[2] != 'b') ||
           (title[3] != 'c') ||
           (title[4] != 'a') ||
           (title[5] != 'p') ||
           (title[6] != 't') ||
           (title[7] != 'u') ||
           (title[8] != 'r') ||
           (title[9] != 'e');
}

// ----------------------------------------------------------------------------
void vdb::file_header_t::clear(void)
{
    title = { 'v', 'd', 'b', 'c', 'a', 'p', 't', 'u', 'r', 'e' };
    major_version = 0;
    minor_version = 0;
    status = 0;
    time_created = 0;
    padding = 0;

    records.clear();
    comments.clear();
}

// ----------------------------------------------------------------------------
uint32_t vdb::file_header_t::length(void) const
{
    return (BASE_RECORD_SIZE + comments_length());
}

// ----------------------------------------------------------------------------
uint32_t vdb::file_header_t::comments_length(void) const
{
    uint32_t
        size = 0;

    for(unsigned i = 0; i < comments.size(); ++i)
    {
        size += comments[i].length();
    }

    return size;
}

// ----------------------------------------------------------------------------
void vdb::file_header_t::read(vdis::byte_stream_t &stream)
{
    uint16_t
        record_count = 0,
        comment_count = 0;

    clear();

    LOG_VERBOSE(
        "Reading file header at index %d of %d...",
        stream.index(),
        stream.length());

    stream.read(title, 10);
    stream.read(major_version);
    stream.read(minor_version);
    stream.read(status);
    stream.read(time_created);
    stream.read(padding);
    stream.read(record_count);
    stream.read(comment_count);

    for(uint32_t i = 0; (i < record_count) and not stream.error(); ++i)
    {
        file_header_record_t
            record;

        LOG_VERBOSE("Reading record %d/%d...", (i + 1), record_count);

        record.read(stream);

        records.push_back(record);
    }

    for(uint32_t i = 0; (i < comment_count) and not stream.error(); ++i)
    {
        file_header_comment_t
            comment;

        LOG_VERBOSE("Reading comment %d/%d...", (i + 1), comment_count);

        comment.read(stream);

        comments.push_back(comment);
    }
}

// ----------------------------------------------------------------------------
void vdb::file_header_t::write(vdis::byte_stream_t &stream) const
{
    const uint16_t
        record_count = records.size(),
        comment_count = comments.size();

    LOG_VERBOSE(
        "Writing file header at index %d of %d...",
        stream.index(),
        stream.length());

    stream.write(title, 10);
    stream.write(major_version);
    stream.write(minor_version);
    stream.write(status);
    stream.write(time_created);
    stream.write(padding);
    stream.write(record_count);
    stream.write(comment_count);

    for(uint32_t i = 0; (i < record_count) and not stream.error(); ++i)
    {
        LOG_VERBOSE("Writing record %d/%d...", (i + 1), record_count);

        records[i].write(stream);
    }

    for(uint32_t i = 0; (i < comment_count) and not stream.error(); ++i)
    {
        LOG_VERBOSE("Writing comment %d/%d...", (i + 1), comment_count);

        comments[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::file_header_comment_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << color::bold_yellow << vdis::time_to_string(time)
        << color::none << " (" << value.length() << " bytes)" << std::endl
        << value << std::endl << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::file_header_comment_t::clear(void)
{
    time = 0;

    value.clear();
}

// ----------------------------------------------------------------------------
uint32_t vdb::file_header_comment_t::length(void) const
{
    uint32_t
        size = BASE_RECORD_SIZE;

    size += value.length();
    size += vdis::padding_length(size, BOUNDARY_SIZE);

    return size;
}

// ----------------------------------------------------------------------------
void vdb::file_header_comment_t::read(vdis::byte_stream_t &stream)
{
    uint64_t
        comment_length = 0;
    char
        *string_ptr = 0;

    LOG_VERBOSE(
        "Reading header comment at index %d of %d...",
        stream.index(),
        stream.length());

    stream.read(time);
    stream.read(comment_length);

    if (comment_length and not stream.error())
    {
        LOG_VERBOSE("Reading %d bytes for comment string...", comment_length);

        string_ptr = new char[comment_length];

        stream.read(string_ptr, comment_length);

        value = string_ptr;

        delete[] string_ptr;
        string_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
void vdb::file_header_comment_t::write(vdis::byte_stream_t &stream) const
{
    uint64_t
        comment_length = value.length();
    uint32_t
        padding_bytes = vdis::padding_length(comment_length, BOUNDARY_SIZE);

    comment_length += padding_bytes;

    LOG_VERBOSE(
        "Writing header comment with length %d at index %d of %d...",
        comment_length,
        stream.index(),
        stream.length());

    stream.write(time);
    stream.write(comment_length);

    if ((comment_length > 0) and not stream.error())
    {
        LOG_VERBOSE(
            "Writing %d bytes for comment string...",
            value.length());

        stream.write(value.c_str(), value.length());

        LOG_VERBOSE(
            "Writing %d padding bytes after comment string...",
            padding_bytes);

        for(uint32_t i = 0; i < padding_bytes; ++i)
        {
            stream.write((uint8_t)0);
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::file_header_record_t::print(
    const string_t &prefix,
    std::ostream &out) const
{
    out << prefix << "type " << type << std::endl
        << prefix << "status " << vdis::to_bin_string(status, true) << std::endl
        << prefix << "data " << vdis::to_hex_string(data, true) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::file_header_record_t::clear(void)
{
    type = 0;
    status = 0;
    data = 0;
}

// ----------------------------------------------------------------------------
void vdb::file_header_record_t::read(vdis::byte_stream_t &stream)
{
    clear();

    stream.read(type);
    stream.read(status);
    stream.read(data);
}

// ----------------------------------------------------------------------------
void vdb::file_header_record_t::write(vdis::byte_stream_t &stream) const
{
    stream.write(type);
    stream.write(status);
    stream.write(data);
}
