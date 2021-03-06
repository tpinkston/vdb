#include "vdb_comment.h"
#include "vdb_common.h"
#include "vdb_file_reader.h"
#include "vdb_options.h"

#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const uint32_t
        COPY_BUFFER_LENGTH = 0x1000;
}

// ----------------------------------------------------------------------------
vdb::comment_t::comment_t(
    const std::string &command,
    const std::vector<std::string> &arguments
) :
    command_t(command, arguments),
    action(PRINT),
    deletion(0)
{
    options.add(vdb::option_t("add", 'A', false));
    options.add(vdb::option_t("remove", 'R', true));
}

// ----------------------------------------------------------------------------
vdb::comment_t::~comment_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::comment_t::option_callback(
    const option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (option.short_option == 'A')
    {
        action = vdb::comment_t::ADD;
    }
    else if (option.short_option == 'R')
    {
        action = vdb::comment_t::REMOVE;

        if (value != "all")
        {
            success = vdis::to_uint32(value, deletion);

            if (not success)
            {
                LOG_FATAL("Invalid comment index: %s", value.c_str());
            }
        }
    }
    else
    {
        result = false;
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comment_t::run(void)
{
    int result = 1;

    // File argument required
    //
    if (options::command_arguments.empty())
    {
        LOG_FATAL("Missing file argument");
    }
    switch(action)
    {
        case ADD:
            result = add();
            break;
        case REMOVE:
            result = remove();
            break;
        case PRINT:
            result = print();
            break;
        default:
            LOG_FATAL("Unexpected action: %d", (int)action);
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comment_t::add(void)
{
    int result = 1;

    // File argument (1st) required, comment text argument (2nd) optional
    //
    if (options::command_arguments.empty())
    {
        LOG_FATAL("Missing file argument");
    }
    else if (options::command_arguments.size() > 2)
    {
        LOG_FATAL("Too many arguments");
    }
    else
    {
        const std::string
            filename = options::command_arguments[0];
        file_header_comment_t
            comment;

        comment.time = vdis::get_system_time();

        if (options::command_arguments.size() > 1)
        {
            // Comment to add provided in command line argument
            //
            comment.value = options::command_arguments[1];
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
            comment.value = vdis::trim(comment.value);
            std::cout << std::endl << std::endl;
        }

        comment.value = vdis::trim(comment.value);

        if (comment.value.empty())
        {
            comment.value = "NULL";
        }

        standard_reader_t
            reader(filename);

        if (reader.good())
        {
            const uint32_t
                comment_length = comment.length(),
                new_length = (reader.stream.length() + comment_length);
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
                remainder = input.remaining_length();

            LOG_VERBOSE(
                "Input buffer is at %d/%d",
                input.index(),
                input.length());
            LOG_VERBOSE(
                "Output buffer is at %d/%d",
                output.index(),
                output.length());

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
                result = (output.error() or output.ferror()) ? 1 : 0;

                if (result == 0)
                {
                    std::cout << "Comment (" << comment.value.length()
                              << " bytes) added to file '" << filename
                              << "'" << std::endl
                              << "Total number of comments is now "
                              << reader.header.comments.size() << std::endl;
                }
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comment_t::remove(void)
{
    int result = 1;

    // File argument (1st) required, comment number argument (2nd) optional
    //
    if (options::command_arguments.empty())
    {
        LOG_FATAL("Missing file argument");
    }
    else if (options::command_arguments.size() > 1)
    {
        LOG_FATAL("Too many arguments");
    }
    else
    {
        const std::string
            filename = options::command_arguments[0];
        standard_reader_t
            reader(filename);

        if (reader.good())
        {
            if (reader.header.comments.size() == 0)
            {
                std::cout << "vdb-comment: no comments in file '"
                          << filename << "'" << std::endl;
            }
            else if (deletion == 0)
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
                deletion--;

                if (deletion <= reader.header.comments.size())
                {
                    // Convert number to index
                    //
                    result = remove_comment(reader, (deletion - 1));
                }
                else
                {
                    LOG_FATAL(
                        "Comment number out of range: %d, range is 1..%d",
                        (deletion + 1),
                        reader.header.comments.size());
                }
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comment_t::remove_comment(standard_reader_t &reader, uint32_t index)
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
            new_length = (input.length() - comment_length);
        file_stream
            output(new_length);

        LOG_VERBOSE(
            "Input buffer is at %d/%d",
            input.index(),
            input.length());
        LOG_VERBOSE(
            "Output buffer is at %d/%d",
            output.index(),
            output.length());
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
            remainder = input.remaining_length();

        LOG_VERBOSE(
            "Input buffer is at %d/%d",
            input.index(),
            input.length());
        LOG_VERBOSE(
            "Output buffer is at %d/%d",
            output.index(),
            output.length());

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

            if (not output.error() and not output.ferror())
            {
                std::cout << "Comment #" << (index + 1)
                          << " removed." << std::endl;
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comment_t::remove_all_comments(standard_reader_t &reader)
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
                input.length() -
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
            remainder = input.remaining_length();

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

            if (not output.error() and not output.ferror())
            {
                std::cout << "All comments removed." << std::endl;
            }
        }
     }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::comment_t::print(void)
{
    const std::vector<std::string>
        &files = options::command_arguments;
    int
        result = 1;

    // At least one file argument required
    //
    if (options::command_arguments.empty())
    {
        LOG_FATAL("Missing file argument(s)");
    }
    else for(uint32_t i = 0; i < files.size(); ++i)
    {
        const std::string
            filename = files[i];

        standard_reader_t
            reader(filename);

        if (reader.good())
        {
            if (reader.header.comments.empty())
            {
                std::cout << "No comments in file: " << color::bold_cyan
                          << filename << color::none << std::endl;
            }
            else
            {
                std::cout << "Printing comments in file: " << color::bold_cyan
                          << filename << color::none << std::endl;


                for(uint32_t j = 0; j < reader.header.comments.size(); ++j)
                {
                    reader.header.print_comment(std::string(), j, std::cout);
                }
            }
        }
    }

    return result;
}
