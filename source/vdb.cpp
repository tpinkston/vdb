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

#include "vdb_capture.h"
#include "vdb_command.h"
#include "vdb_comments.h"
#include "vdb_entity_type.h"
#include "vdb_enumerations.h"
#include "vdb_git.h"
#include "vdb_list.h"
#include "vdb_logger.h"
#include "vdb_object_type.h"
#include "vdb_options.h"
#include "vdb_playback.h"
#include "vdb_query.h"
#include "vdb_test.h"
#include "vdb_version.h"

void print_version(void);
void print_usage(void);
void print_usage_main(void);
void print_usage_capture(void);
void print_usage_playback(void);
void print_usage_list(void);
void print_usage_query(void);
void print_usage_comment(void);
void print_usage_uncomment(void);
void print_usage_enums(void);
void print_usage_entities(void);
void print_usage_objects(void);

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    vdb::command
        *command_ptr = 0;
    int
        result = 0;

    if (not vdb::options::initialize(argc, argv))
    {
        result = 1;
    }
    else
    {
        if (vdb::options::flag(vdb::OPT_VERSION))
        {
            print_version();
        }

        if (vdb::options::flag(vdb::OPT_HELP))
        {
            print_usage();
        }
        else
        {
            vdb::set_byteswapping();

            vdb::enumerations::load();
            vdb::entity_type_data::load();
            vdb::object_type_data::load();

            switch(vdb::options::get_command())
            {
                case vdb::USER_COMMAND_CAPTURE:
                    command_ptr = new vdb::capture();
                    break;
                case vdb::USER_COMMAND_PLAYBACK:
                    command_ptr = new vdb::playback();
                    break;
                case vdb::USER_COMMAND_LIST:
                    command_ptr = new vdb::list();
                    break;
                case vdb::USER_COMMAND_QUERY:
                    command_ptr = new vdb::query();
                    break;
                case vdb::USER_COMMAND_COMMENT:
                    command_ptr = new vdb::comment();
                    break;
                case vdb::USER_COMMAND_UNCOMMENT:
                    command_ptr = new vdb::uncomment();
                    break;
                case vdb::USER_COMMAND_ENUMS:
                    command_ptr = new vdb::enumerations();
                    break;
                case vdb::USER_COMMAND_ENTITIES:
                    command_ptr = new vdb::entity_type_data();
                    break;
                case vdb::USER_COMMAND_OBJECTS:
                    command_ptr = new vdb::object_type_data();
                    break;
                case vdb::USER_COMMAND_TEST:
                    command_ptr = new vdb::test();
                    break;
                default:
                    if (not vdb::options::flag(vdb::OPT_VERSION))
                    {
                        std::cerr << vdb::options::get_terminal_command()
                                  << ": missing command (try --help)"
                                  << std::endl;
                        result = 1;
                    }
                    break;
            }

            if (command_ptr)
            {
                LOG_VERBOSE("Running command...");
                result = command_ptr->run();

                delete command_ptr;
                command_ptr = 0;
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
void print_version(void)
{
    // The macros printed here are available in headers that get generated
    // during build.
    //
    std::cout << "V-DIS Debugger (vdb) Version "
              << (int)VDB_VERSION_MAJOR << "." << (int)VDB_VERSION_MINOR
              << " [" << VDB_GIT_BRANCH << "-" << VDB_GIT_COMMIT
              << " " << VDB_VERSION_DATE << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void print_usage(void)
{
    switch(vdb::options::get_command())
    {
        case vdb::USER_COMMAND_CAPTURE:
            print_usage_capture();
            break;
        case vdb::USER_COMMAND_PLAYBACK:
            print_usage_playback();
            break;
        case vdb::USER_COMMAND_LIST:
            print_usage_list();
            break;
        case vdb::USER_COMMAND_QUERY:
            print_usage_query();
            break;
        case vdb::USER_COMMAND_COMMENT:
            print_usage_comment();
            break;
        case vdb::USER_COMMAND_UNCOMMENT:
            print_usage_uncomment();
            break;
        case vdb::USER_COMMAND_ENUMS:
            print_usage_enums();
            break;
        case vdb::USER_COMMAND_ENTITIES:
            print_usage_entities();
            break;
        case vdb::USER_COMMAND_OBJECTS:
            print_usage_objects();
            break;
        default:
            print_usage_main();
            break;
    }
}

#define USAGE(x) std::cout << (x) << std::endl;

// ----------------------------------------------------------------------------
void print_usage_main(void)
{
#include "vdb_usage_main.txt"
}

// ----------------------------------------------------------------------------
void print_usage_capture(void)
{
#include "vdb_usage_capture.txt"
}

// ----------------------------------------------------------------------------
void print_usage_playback(void)
{
#include "vdb_usage_playback.txt"
}

// ----------------------------------------------------------------------------
void print_usage_list(void)
{
#include "vdb_usage_list.txt"
}

// ----------------------------------------------------------------------------
void print_usage_query(void)
{
#include "vdb_usage_query.txt"
}

// ----------------------------------------------------------------------------
void print_usage_comment(void)
{
#include "vdb_usage_comment.txt"
}

// ----------------------------------------------------------------------------
void print_usage_uncomment(void)
{
#include "vdb_usage_uncomment.txt"
}

// ----------------------------------------------------------------------------
void print_usage_enums(void)
{
#include "vdb_usage_enums.txt"
}

// ----------------------------------------------------------------------------
void print_usage_entities(void)
{
#include "vdb_usage_entities.txt"
}

// ----------------------------------------------------------------------------
void print_usage_objects(void)
{
#include "vdb_usage_objects.txt"
}

#undef USAGE
