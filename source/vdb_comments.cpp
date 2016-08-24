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

#include "vdb_comments.h"
#include "vdb_common.h"
#include "vdb_file_readers.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_string.h"
#include "vdb_time.h"

namespace
{
    const uint32_t
        COPY_BUFFER_LENGTH = 0x1000;
}

// ----------------------------------------------------------------------------
int vdb::comments::add(void)
{
    int
        result = 1;

    // File argument (1st) required, comment text argument (2nd) optional
    //
    if (options::get_command_arguments().empty())
    {
        std::cerr << options::get_terminal_command()
                  << " comment: missing file argument" << std::endl;
    }
    else if (options::get_command_arguments().size() > 2)
    {
        std::cerr << options::get_terminal_command()
                  << " comment: too many arguments" << std::endl;
    }
    else
    {
        const std::string
            filename = *options::get_command_argument(0);
        file_header_comment_t
            comment;

        comment.time = time::get_system();

        if (options::get_command_argument(1))
        {
            // Comment to add provided in command line argument
            //
            comment.value = *options::get_command_argument(1);
        }
        else
        {
            // Extract comment to add from STDIN
            //
            const uint32_t
                text_size = 0x100;
            char
                text[text_size];

            std::memset(text, 0, text_size);
            std::cin.read(text, text_size);
            comment.value = text;
            comment.value = trim(comment.value);
            std::cout << std::endl << std::endl;
        }

        comment.value = trim(comment.value);

        if (comment.value.empty())
        {
            comment.value = "NULL";
        }

        standard_reader_t
            reader(filename);

        if (reader.good() and reader.read_header())
        {
            const uint32_t
                comment_length = comment.length(),
                new_length = (reader.stream.get_length() + comment_length);
            file_stream
                &input = reader.stream;
            file_stream
                output(new_length);
            uint32_t
                copy_buffer_length = COPY_BUFFER_LENGTH;
            uint8_t
                copy_buffer[copy_buffer_length];

            // Add the comment to the header and write header to output buffer
            //
            reader.header.comments.push_back(comment);
            reader.header.write(output);

            uint32_t
                remainder = input.get_length_remaining();

            LOG_VERBOSE(
                "Input buffer is at %d/%d",
                input.get_index(),
                input.get_length());
            LOG_VERBOSE(
                "Output buffer is at %d/%d",
                output.get_index(),
                output.get_length());

            while((remainder > 0) and not output.error())
            {
                if (copy_buffer_length > remainder)
                {
                    copy_buffer_length = remainder;
                }

                input.read(copy_buffer, copy_buffer_length);
                output.write(copy_buffer, copy_buffer_length);

                remainder -= copy_buffer_length;

                LOG_VERBOSE("Bytes remaining is %d", remainder);
            }

            if (not output.error())
            {
                // Overwrite the file with new stream.
                //
                output.write_file(filename);
                result = output.error() ? 1 : 0;

                std::cout << "Comment (" << comment.value.length()
                          << " bytes) added to file '" << filename
                          << "'" << std::endl
                          << "Total number of comments is now "
                          << reader.header.comments.size() << std::endl;
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comments::remove(void)
{
    int
        result = 1;

    // File argument (1st) required, comment number argument (2nd) optional
    //
    if (options::get_command_arguments().empty())
    {
        std::cerr << options::get_terminal_command()
                  << " uncomment: missing file argument" << std::endl;
    }
    else if (options::get_command_arguments().size() > 2)
    {
        std::cerr << options::get_terminal_command()
                  << " uncomment: too many arguments" << std::endl;
    }
    else
    {
        const std::string
            filename = *options::get_command_argument(0);

        standard_reader_t
            reader(filename);

        if (reader.good() and reader.read_header())
        {
            if (reader.header.comments.size() == 0)
            {
                std::cout << options::get_terminal_command()
                          << " uncomment: no comments in file '"
                          << filename << "'" << std::endl;
            }
            else if (not options::get_command_argument(1))
            {
                // Not comment number specified...
                //
                result = remove_all_comments(reader);
            }
            else
            {
                // User provided a number for the comment to remove.
                // Numbers start at 1 so it must be decremented to an index.
                //
                const std::string
                    value(*options::get_command_argument(1));
                int32_t
                    number = 0;

                if (not string_to_int32(value, number))
                {
                    std::cout << options::get_terminal_command()
                              << " uncomment: invalid comment number '"
                              << value << "'" << std::endl;
                }
                else
                {
                    if (number < 1)
                    {
                        std::cerr << options::get_terminal_command()
                                  << " uncomment: invalid comment number '"
                                  << value << "'" << std::endl;
                    }
                    else
                    {
                        const int32_t
                            current_count = reader.header.comments.size();

                        if (number <= current_count)
                        {
                            // Convert number to index
                            //
                            result = remove_comment(reader, (number - 1));
                        }
                        else
                        {
                            std::cerr << options::get_terminal_command()
                                      << " uncomment: comment number "
                                      << "out of range: " << number
                                      << ", range is 1.." << current_count
                                      << std::endl;
                        }
                    }
                }
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comments::remove_comment(standard_reader_t &reader, int32_t index)
{
    file_stream
        &input = reader.stream;
    int
        result = 0;

    // Print comment and get user confirmation
    //
    reader.header.print_comment(std::string(), index, std::cout);
    std::cout << "Delete this comment (y/n)? ";

    if (not user_confirmation())
    {
        std::cout << "No comments removed." << std::endl;
    }
    else
    {
        const uint32_t
            comment_length = reader.header.comments[index].length(),
            new_length = (input.get_length() - comment_length);
        file_stream
            output(new_length);

        LOG_VERBOSE(
            "Input buffer is at %d/%d",
            input.get_index(),
            input.get_length());
        LOG_VERBOSE(
            "Output buffer is at %d/%d",
            output.get_index(),
            output.get_length());
        LOG_VERBOSE(
            "Size of all comments before removal is %d...",
            reader.header.comments_length());

        reader.header.comments.erase(reader.header.comments.begin() + index);

        LOG_VERBOSE(
            "Size of all comments after removal is %d...",
            reader.header.comments_length());

        reader.header.write(output);

        uint32_t
            copy_buffer_length = COPY_BUFFER_LENGTH;
        uint8_t
            copy_buffer[copy_buffer_length];
        uint32_t
            remainder = input.get_length_remaining();

        LOG_VERBOSE(
            "Input buffer is at %d/%d",
            input.get_index(),
            input.get_length());
        LOG_VERBOSE(
            "Output buffer is at %d/%d",
            output.get_index(),
            output.get_length());

        while((remainder > 0) and not output.error())
        {
            if (copy_buffer_length > remainder)
            {
                copy_buffer_length = remainder;
            }

            input.read(copy_buffer, copy_buffer_length);
            output.write(copy_buffer, copy_buffer_length);

            remainder -= copy_buffer_length;

            LOG_VERBOSE("Bytes remaining is %d", remainder);
        }

        if (input.error() or output.error())
        {
            result = 1;
        }
        else
        {
            // Overwrite the file with new stream.
            //
            output.write_file(reader.get_filename());

            std::cout << "Comment #" << (index + 1)
                      << " removed." << std::endl;
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comments::remove_all_comments(standard_reader_t &reader)
{
    file_stream
        &input = reader.stream;
    int
        result = 0;

    std::cout << "Delete all comments in '" << reader.get_filename() << "'? ";

    if (user_confirmation())
    {
        const uint32_t
            new_length =
                input.get_length() -
                reader.header.comments_length();
        file_stream
            output(new_length);

        reader.header.comments.clear();
        reader.header.write(output);

        uint32_t
            copy_buffer_length = COPY_BUFFER_LENGTH;
        uint8_t
            copy_buffer[copy_buffer_length];
        uint32_t
            remainder = input.get_length_remaining();

        while((remainder > 0) and not output.error())
        {
            if (copy_buffer_length > remainder)
            {
                copy_buffer_length = remainder;
            }

            input.read(copy_buffer, copy_buffer_length);
            output.write(copy_buffer, copy_buffer_length);

            remainder -= copy_buffer_length;
        }

        if (input.error() or output.error())
        {
            result = 1;
        }
        else
        {
            // Overwrite the file with new stream.
            //
            output.write_file(reader.get_filename());

            std::cout << "All comments removed." << std::endl;
        }
     }

    return result;
}
