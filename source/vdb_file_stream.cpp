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

#include "vdb_file_stream.h"
#include "vdb_logger.h"
#include "vdb_options.h"

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(void)
{

}

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(uint32_t length) : byte_stream(length)
{

}

// ----------------------------------------------------------------------------
vdb::file_stream::file_stream(const file_stream &copy) : byte_stream(copy)
{

}

// ----------------------------------------------------------------------------
vdb::file_stream::~file_stream(void)
{

}

// ----------------------------------------------------------------------------
void vdb::file_stream::read_file(const std::string &filename)
{
    std::ifstream
        stream;
    std::streamsize
        bytes_read = 0;

    reset();

    LOG_VERBOSE("Opening file %s...", filename.c_str());

    stream.open(filename.c_str(), std::ios::in | std::ios::binary);

    if (stream.fail())
    {
        std::cerr << options::get_terminal_command()
                  << ": failed to open file: " << filename << std::endl;
        exit(-1);
    }

    // Go to EOF to get the length.
    //
    stream.seekg(0, std::ios::end);

    buffer_length = stream.tellg();

    if (stream.fail())
    {
        std::cerr << options::get_terminal_command()
                  << ": failed to read file: " << filename << std::endl;
        exit(1);
    }

    buffer = new uint8_t[buffer_length];

    LOG_VERBOSE("Allocated buffer with %d bytes...", buffer_length);

    // Go back to the beginning and read the entire file.
    //
    stream.seekg(0, std::ios::beg);
    stream.read((char *)buffer, buffer_length);

    if (stream.fail())
    {
        std::cerr << options::get_terminal_command()
                  << ": failed to read file: " << filename << std::endl;
        exit(1);
    }

    bytes_read = stream.gcount();

    if (bytes_read != buffer_length)
    {
        std::cerr << options::get_terminal_command()
                  << ": failed to read (" << bytes_read
                  << " of " << buffer_length << " bytes) from file: "
                  << filename << std::endl;
        exit(1);
    }

    stream.close();

    LOG_VERBOSE("Bytes read from file is %d...", bytes_read);

    reset_index(0);
}

// ----------------------------------------------------------------------------
void vdb::file_stream::write_file(const std::string &filename)
{
    std::ofstream
        stream;
    std::streamsize
        bytes_written = 0;

    LOG_VERBOSE("Opening file %s...", filename.c_str());

    stream.open(filename.c_str(), std::ios::out | std::ios::binary);

    if (stream.fail())
    {
        std::cerr << options::get_terminal_command()
                  << ": failed to open file: " << filename << std::endl;
        exit(1);
    }

    if (buffer and (buffer_length > 0))
    {
        stream.write((char *)buffer, buffer_length);

        if (stream.fail())
        {
            std::cerr << options::get_terminal_command()
                      << ": failed to write to file: " << filename << std::endl;
            exit(1);
        }
    }

    bytes_written = stream.tellp();

    if (bytes_written != buffer_length)
    {
        std::cerr << options::get_terminal_command()
                  << ": failed to write (" << bytes_written
                  << " of " << buffer_length << " bytes) from file: "
                  << filename << std::endl;
        exit(1);
    }

    stream.close();

    LOG_VERBOSE("Bytes written to file is %d...", bytes_written);
}
