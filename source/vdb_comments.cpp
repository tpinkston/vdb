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
#include "vdb_options.h"
#include "vdb_string.h"
#include "vdb_time.h"

// ----------------------------------------------------------------------------
vdb::comment::comment(void)
{

}

// ----------------------------------------------------------------------------
vdb::comment::~comment(void)
{

}

// ----------------------------------------------------------------------------
int vdb::comment::run(void)
{
    int result = 1;

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
        // If comment is provided then use it, otherwise extract comment
        // from STDIN.
        //
        std::string
            comment_text;

        if (options::get_command_argument(1))
        {
            comment_text = *options::get_command_argument(1);
        }
        else
        {
            const uint32_t
                comment_size = 0x100;
            char
                comment[comment_size];

            std::memset(comment, 0, comment_size);
            std::cin.read(comment, comment_size);
            comment_text = comment;
            std::cout << std::endl << std::endl;
        }

        filename = *options::get_command_argument(0);

        if (read_content())
        {
            // Create new comment.
            //
            file_header_comment_t
                new_comment;

            new_comment.time = time::get_system();
            new_comment.value = comment_text;

            header.comments.push_back(new_comment);

            const uint32_t
                new_length = (stream.get_length() + new_comment.length());
            file_stream
                new_stream(new_length);

            header.write(new_stream);

            // Original stream's index should be at the first PDU index if
            // there is one (right after the header).
            //
            uint32_t
                i = stream.get_index();

            while((i < stream.get_length()) and not new_stream.error())
            {
                new_stream.write(stream[i]);
                ++i;
            }

            if (not new_stream.error())
            {
                // Overwrite the file with new stream.
                //
                new_stream.write_file(filename);

                std::cout << "Comment (" << comment_text.length()
                          << " bytes) added to file '" << filename
                          << "'" << std::endl
                          << "Total number of comments is now "
                          << header.comments.size() << std::endl;
            }

            result = 0;
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
vdb::uncomment::uncomment(void)
{

}

// ----------------------------------------------------------------------------
vdb::uncomment::~uncomment(void)
{

}

// ----------------------------------------------------------------------------
int vdb::uncomment::run(void)
{
    int result = 1;

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
        filename = *options::get_command_argument(0);

        if (read_content())
        {
            if (header.comments.size() == 0)
            {
                std::cout << options::get_terminal_command()
                          << " uncomment: no comments in file '"
                          << filename << "'" << std::endl;
            }
            else if (not options::get_command_argument(1))
            {
                // Not comment number specified...
                //
                remove_all_comments();
                result = 0;
            }
            else
            {
                // User provided a number for the comment to remove.
                // Numbers start at 1 so it must be decremented to an index.
                //
                std::string
                    value(*options::get_command_argument(1));
                int32_t
                    number = 0;

                string_to_int32(value, number);

                // Number to index.
                //
                number--;

                if (number < 1)
                {
                    std::cerr << options::get_terminal_command()
                              << " uncomment: invalid comment number '"
                              << value << "'" << std::endl;
                }
                else if ((unsigned)number >= header.comments.size())
                {
                    std::cerr << options::get_terminal_command()
                              << " uncomment: comment number out of range: "
                              << (number + 1) << ", range is 1.."
                              << header.comments.size() << std::endl;
                }
                else
                {
                    remove_comment(number);
                    result = 0;
                }
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
void vdb::uncomment::remove_comment(int32_t index)
{
    header.print_comment(std::string(), index, std::cout);

    std::cout << "Delete this comment (y/n)? ";

    if (user_confirmation())
    {
        const uint32_t
            comment_length = header.comments[index].length(),
            new_length = (stream.get_length() - comment_length);
        file_stream
            new_stream(new_length);

        header.comments.erase(header.comments.begin() + index);
        header.write(new_stream);

        uint32_t
            i = stream.get_index();

        while((i < stream.get_length()) and not new_stream.error())
        {
            new_stream.write(stream[i]);
            ++i;
        }

        if (not new_stream.error())
        {
            // Overwrite the file with new stream.
            //
            new_stream.write_file(filename);

            std::cout << "Comment #" << (index + 1) << " removed." << std::endl;
        }
    }
    else
    {
        std::cout << "No comments removed." << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::uncomment::remove_all_comments(void)
{
    std::cout << "Delete all comments in '" << filename << "'? ";

    if (user_confirmation())
    {
        const uint32_t
            new_length = stream.get_length() - header.comments_length();
        file_stream
            new_stream(new_length);

        header.comments.clear();
        header.write(new_stream);

        // Original stream's index should be at the first PDU index if
        // there is one.
        //
        for(uint32_t i = stream.get_index(); i < stream.get_length(); ++i)
        {
            new_stream.write(stream[i]);
        }

        // Overwrite the file with new stream.
        //
        new_stream.write_file(filename);

        std::cout << "All comments removed." << std::endl;
    }
}
