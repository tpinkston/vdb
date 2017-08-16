#include "vdb_file_stream.h"
#include "vdb_options.h"

#include "vdis_logger.h"

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(void) : file_error(false)
{

}

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(uint32_t length) :
    vdis::byte_stream_t(length),
    file_error(false)
{

}

// ----------------------------------------------------------------------------
vdb::file_stream::~file_stream(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::file_stream::read_file(const string_t &filename)
{
    std::ifstream
        stream;
    std::streamsize
        bytes_read = 0;
    bool
        success = false;

    clear();

    LOG_VERBOSE("Opening file %s...", filename.c_str());

    stream.open(filename.c_str(), std::ios::in | std::ios::binary);

    if (stream.fail())
    {
        LOG_ERROR("Failed to open file '%s'", filename.c_str());
    }
    else
    {
        // Go to EOF to get the length.
        //
        stream.seekg(0, std::ios::end);

        data_length = stream.tellg();

        if (stream.fail())
        {
            LOG_ERROR("Failed to read file '%s'", filename.c_str());
        }
        else
        {
            data_buffer = new uint8_t[data_length];

            LOG_VERBOSE("Allocated buffer with %d bytes...", data_length);

            // Go back to the beginning and read the entire file.
            //
            stream.seekg(0, std::ios::beg);
            stream.read((char *)data_buffer, data_length);

            if (stream.fail())
            {
                LOG_ERROR("Failed to read file '%s'", filename.c_str());
            }
            else
            {
                bytes_read = stream.gcount();

                if (bytes_read != data_length)
                {
                    LOG_ERROR(
                        "Failed to read (%d of %d bytes) from file '%s'",
                        bytes_read,
                        data_length,
                        filename.c_str());
                }
                else
                {
                    LOG_VERBOSE("Bytes read from file is %d...", bytes_read);

                    reset_index(0);
                    success = true;
                }
            }
        }
    }

    stream.close();

    return success;
}

// ----------------------------------------------------------------------------
void vdb::file_stream::write_file(const string_t &filename)
{
    std::ofstream
        stream;
    std::streamsize
        bytes_written = 0;

    file_error = false;

    LOG_VERBOSE("Opening file %s...", filename.c_str());

    stream.open(filename.c_str(), std::ios::out | std::ios::binary);

    if (stream.fail())
    {
        LOG_ERROR("Failed to open file '%s'", filename.c_str());
        file_error = true;
    }
    else if (data_buffer and (data_length > 0))
    {
        stream.write((char *)data_buffer, data_length);

        if (stream.fail())
        {
            LOG_ERROR("Failed to write to file '%s'", filename.c_str());
            file_error = true;
        }
        else
        {
            bytes_written = stream.tellp();

            if (bytes_written != data_length)
            {
                LOG_ERROR(
                    "Failed to write bytes (%d of %s) to file '%s'",
                    bytes_written,
                    data_length,
                    filename.c_str());

                file_error = true;
            }
        }
    }

    stream.close();

    LOG_VERBOSE("Bytes written to file is %d...", bytes_written);
}
