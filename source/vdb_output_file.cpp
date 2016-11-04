#include "vdb_file_stream.h"
#include "vdb_file_types.h"
#include "vdb_output_file.h"
#include "vdb_pdu_data.h"
#include "vdb_version.h"

#include "vdis_logger.h"
#include "vdis_services.h"

// ----------------------------------------------------------------------------
// Opens output file, writes header, closes then reopens file in append mode
// for writing PDU data.
//
vdb::output_file_t::output_file_t(
    const std::string &filename,
    const std::string *comment_ptr
) :
    filename(filename),
    file_ptr(0)
{
    file_header_t
        header;
    file_stream
        stream;

    // First save header to the output capture file.
    //
    header.major_version = VDB_VERSION_MAJOR;
    header.minor_version = VDB_VERSION_MINOR;
    header.time_created = vdis::get_system_time();

    if (comment_ptr and not comment_ptr->empty())
    {
        file_header_comment_t
            comment;

        comment.time = header.time_created;
        comment.value = *comment_ptr;

        header.comments.push_back(comment);
    }

    stream.reset(header.length());
    stream.write(header);
    stream.write_file(filename);

    // Reopen the file for manually writing PDU data, appending to the header
    // just written ('a' for append, 'b' for binary).
    //
    file_ptr = fopen(filename.c_str(), "ab");

    if (file_ptr)
    {
        std::cout << "Opened output file: " << filename << std::endl;
    }
    else
    {
        perror("fopen");
    }
}

// ----------------------------------------------------------------------------
// Closes file.
//
vdb::output_file_t::~output_file_t(void)
{
    if (file_ptr)
    {
        if (fclose(file_ptr) == 0)
        {
            std::cout << "Closed output file: " << filename << std::endl;
        }
        else
        {
            perror("fclose");
        }

        file_ptr = 0;
    }
}

// ----------------------------------------------------------------------------
// Writes PDU data record to the file.
//
void vdb::output_file_t::write_pdu_data(const pdu_data_t &data)
{
    if (file_ptr)
    {
        const uint32_t
            size = data.length();
        vdis::byte_stream_t
            stream(size);

        LOG_VERBOSE("Writing %d bytes to file...", size);

        data.write(stream);

        if (not stream.error())
        {
            uint32_t bytes_written = fwrite(
                stream.buffer(),
                1,
                stream.length(),
                file_ptr);

            if (bytes_written != size)
            {
                LOG_ERROR(
                    "File write failed (%d out of %d bytes)",
                    size,
                    stream.length());
            }
        }
    }
}
