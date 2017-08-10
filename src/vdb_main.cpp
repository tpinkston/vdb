#include <iostream>
#include <vector>

#include "vdb_capture.h"
#include "vdb_capture_help.h"
#include "vdb_command.h"
#include "vdb_comment_help.h"
#include "vdb_common.h"
#include "vdb_entities_help.h"
#include "vdb_enums_help.h"
#include "vdb_extract_help.h"
#include "vdb_git.h"
#include "vdb_global_help.h"
#include "vdb_list_help.h"
#include "vdb_main.h"
#include "vdb_objects_help.h"
#include "vdb_playback.h"
#include "vdb_playback_help.h"
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
            std::cerr << "vdb: invalid debug level = '" << level << "'"
                      << std::endl;
        }
    }

    // For command line arguments, the 1st is the program (i.e. executable)
    // name and the 2nd is the command name (or the 'help' or'version'
    // options).
    //
    if (argc < 2)
    {
        std::cerr << "vdb: missing command, try --help" << std::endl;
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
                    // TODO
                    break;
                case vdb::ENTITIES:
                    // TODO
                    break;
                case vdb::ENUMS:
                    // TODO
                    break;
                case vdb::EXTRACT:
                    // TODO
                    break;
                case vdb::LIST:
                    // TODO
                    break;
                case vdb::OBJECTS:
                    // TODO
                    break;
                case vdb::PLAYBACK:
                    command_ptr = new vdb::playback_t("playback", arguments);
                    break;
                case vdb::SUMMARIZE:
                    // TODO
                    break;
            }

            if (command_ptr)
            {
                if (command_ptr->parse_options())
                {
                    result = command_ptr->run();
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
                std::cerr << "vdb: command not available: " << argv[1]
                          << std::endl;
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
        std::cerr << "vdb: invalid command: '" << argument
                  << "', try --help" << std::endl;

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
