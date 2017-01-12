#include "vdb_file_stream.h"
#include "vdb_options.h"

#include "vdis_logger.h"

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(void)
{

}

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(uint32_t length) :
    vdis::byte_stream_t(length)
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
        std::cerr << "vdb: failed to open file: " << filename << std::endl;
    }
    else
    {
        // Go to EOF to get the length.
        //
        stream.seekg(0, std::ios::end);

        data_length = stream.tellg();

        if (stream.fail())
        {
            std::cerr << "vdb: failed to read file: " << filename << std::endl;
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
                std::cerr << "vdb: failed to read file: " << filename
                          << std::endl;
            }
            else
            {
                bytes_read = stream.gcount();

                if (bytes_read != data_length)
                {
                    std::cerr << "vdb: failed to read (" << bytes_read
                              << " of " << data_length << " bytes) from file: "
                              << filename << std::endl;
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

    LOG_VERBOSE("Opening file %s...", filename.c_str());

    stream.open(filename.c_str(), std::ios::out | std::ios::binary);

    if (stream.fail())
    {
        std::cerr << "vdb: failed to open file: " << filename << std::endl;
        exit(1);
    }

    if (data_buffer and (data_length > 0))
    {
        stream.write((char *)data_buffer, data_length);

        if (stream.fail())
        {
            std::cerr << "vdb: failed to write to file: " << filename
                      << std::endl;
            exit(1);
        }
    }

    bytes_written = stream.tellp();

    if (bytes_written != data_length)
    {
        std::cerr << "vdb: failed to write (" << bytes_written
                  << " of " << data_length << " bytes) from file: "
                  << filename << std::endl;
        exit(1);
    }

    stream.close();

    LOG_VERBOSE("Bytes written to file is %d...", bytes_written);
}
