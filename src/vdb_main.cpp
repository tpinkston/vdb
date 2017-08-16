#include <iostream>
#include <vector>

#include "vdb_capture.h"
#include "vdb_capture_help.h"
#include "vdb_command.h"
#include "vdb_comment.h"
#include "vdb_comment_help.h"
#include "vdb_common.h"
#include "vdb_entities.h"
#include "vdb_entities_help.h"
#include "vdb_enums.h"
#include "vdb_enums_help.h"
#include "vdb_extract.h"
#include "vdb_extract_help.h"
#include "vdb_git.h"
#include "vdb_global_help.h"
#include "vdb_list.h"
#include "vdb_list_help.h"
#include "vdb_main.h"
#include "vdb_objects.h"
#include "vdb_objects_help.h"
#include "vdb_playback.h"
#include "vdb_playback_help.h"
#include "vdb_summarize.h"
#include "vdb_summarize_help.h"
#include "vdb_version.h"

#include "vdis_logger.h"
#include "vdis_string.h"

// ----------------------------------------------------------------------------
// Application starting point.
//
int main(int argc, char *argv[])
{
    const char
        *debug_level_ptr = getenv("VDB_DEBUG");
    std::vector<std::string>
        arguments;
    vdb::command_e
        command = vdb::NOCOMMAND;
    vdb::command_t
        *command_ptr = 0;
    bool
        help = false,
        version = false,
        parse = false;
    int
        result = 0;

    if (debug_level_ptr)
    {
        const std::string
            level = debug_level_ptr;
        uint32_t
            value = 0;

        if (vdis::to_uint32(level, value))
        {
            vdb::set_debug(value);
        }
        else
        {
            LOG_ERROR("Invalid debug level '%s'", debug_level_ptr);
        }
    }

    // For command line arguments, the 1st is the program (i.e. executable)
    // name and the 2nd is the command name (or the 'help' or'version'
    // options).
    //
    if (argc < 2)
    {
        LOG_ERROR("Missing command, try --help");
        result = 1;
    }
    else
    {
        result = vdb::get_command(argv[1], command, help, version, parse);
    }

    if (result == 0)
    {
        if (parse and !help and !version)
        {
            // Start iterating on the 3rd argument.
            //
            for(int i = 2; (i < argc); ++i)
            {
                std::string argument(argv[i]);

                arguments.push_back(argument);

                if (vdis::starts_with(argument, "--"))
                {
                    if (argument == "--help")
                    {
                        help = true;
                    }
                }
                else if (vdis::starts_with(argument, "-"))
                {
                    if (argument.find('h') != std::string::npos)
                    {
                        help = true;
                    }
                }
            }
        }

        if (help)
        {
            vdb::print_help(command);
        }
        else if (version)
        {
            vdb::print_version();
        }
        else
        {
            switch(command)
            {
                case vdb::NOCOMMAND:
                    break;
                case vdb::CAPTURE:
                    command_ptr = new vdb::capture_t("capture", arguments);
                    break;
                case vdb::COMMENT:
                    command_ptr = new vdb::comment_t("comment", arguments);
                    break;
                case vdb::ENTITIES:
                    command_ptr = new vdb::entities_t("entities", arguments);
                    break;
                case vdb::ENUMS:
                    command_ptr = new vdb::enums_t("enums", arguments);
                    break;
                case vdb::EXTRACT:
                    command_ptr = new vdb::extract_t("extract", arguments);
                    break;
                case vdb::LIST:
                    command_ptr = new vdb::list_t("list", arguments);
                    break;
                case vdb::OBJECTS:
                    command_ptr = new vdb::objects_t("objects", arguments);
                    break;
                case vdb::PLAYBACK:
                    command_ptr = new vdb::playback_t("playback", arguments);
                    break;
                case vdb::SUMMARIZE:
                    command_ptr = new vdb::summarize_t("summarize", arguments);
                    break;
            }

            if (command_ptr)
            {
                if (command_ptr->parse_options())
                {
                    if (vdb::options::version)
                    {
                        vdb::print_version();
                    }
                    else
                    {
                        result = command_ptr->run();
                    }
                }
                else
                {
                    // Error condition on options parsing.
                    //
                    result = 2;
                }

                delete command_ptr;
                command_ptr = 0;
            }
            else
            {
                LOG_ERROR("Command not available: %s", argv[1]);
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
// Makes determination about which command gets executed based on the
// provided argument.
//
int vdb::get_command(
    std::string argument,
    command_e &command,
    bool &help,
    bool &version,
    bool &parse)
{
    int
        result = 0;

    if ((argument == "help") or (argument == "--help") or (argument == "-h"))
    {
        help = true;
    }
    else if ((argument == "version") or (argument == "--version"))
    {
        version = true;
    }
    else if (argument == "capture")
    {
        command = CAPTURE;
    }
    else if (argument == "comment")
    {
        command = COMMENT;
    }
    else if (argument == "entities")
    {
        command = ENTITIES;
    }
    else if (argument == "enums")
    {
        command = ENUMS;
    }
    else if (argument == "extract")
    {
        command = EXTRACT;
    }
    else if (argument == "list")
    {
        command = LIST;
    }
    else if (argument == "objects")
    {
        command = OBJECTS;
    }
    else if (argument == "playback")
    {
        command = PLAYBACK;
    }
    else if (argument == "summarize")
    {
        command = SUMMARIZE;
    }
    else
    {
        LOG_ERROR("Invalid command '%s' (try --help)", argument.c_str());
        result = 1;
    }

    // Parse remaining arguments if there's a valid command.
    //
    parse = (command != NOCOMMAND);

    return result;
}

// ----------------------------------------------------------------------------
void vdb::print_version(void)
{
    std::cout << "vdb version "
              << (int)VDB_VERSION_MAJOR << "." << (int)VDB_VERSION_MINOR
              << " - VDIS debugger [" << VDB_GIT_BRANCH << "-" << VDB_GIT_COMMIT
              << " - " << __DATE__ << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::print_help(command_e command)
{
    switch(command)
    {
        case CAPTURE:
            print_capture_help();
            break;
        case COMMENT:
            print_comment_help();
            break;
        case ENTITIES:
            print_entities_help();
            break;
        case ENUMS:
            print_enums_help();
            break;
        case EXTRACT:
            print_extract_help();
            break;
        case LIST:
            print_list_help();
            break;
        case OBJECTS:
            print_objects_help();
            break;
        case PLAYBACK:
            print_playback_help();
            break;
        case SUMMARIZE:
            print_summarize_help();
            break;
        default:
            print_global_help();
            break;
    }
}
