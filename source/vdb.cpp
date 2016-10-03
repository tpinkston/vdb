#include "vdb_capture.h"
#include "vdb_comments.h"
#include "vdb_git.h"
#include "vdb_list.h"
#include "vdb_options.h"
#include "vdb_playback.h"
#include "vdb_query.h"
#include "vdb_version.h"

#include "vdis_entity_types.h"
#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_services.h"

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
            vdis::set_byteswapping();

            vdis::enumerations::load();
            vdis::entity_types::load();
            vdis::object_types::load();

            switch(vdb::options::get_command())
            {
                case vdb::USER_COMMAND_CAPTURE:
                    result = vdb::capture::capture_pdus();
                    break;
                case vdb::USER_COMMAND_PLAYBACK:
                    result = vdb::playback::playback_pdus();
                    break;
                case vdb::USER_COMMAND_LIST:
                    result = vdb::list::list_pdus();
                    break;
                case vdb::USER_COMMAND_QUERY:
                    result = vdb::query::query_pdus();
                    break;
                case vdb::USER_COMMAND_COMMENT:
                    result = vdb::comments::add();
                    break;
                case vdb::USER_COMMAND_UNCOMMENT:
                    result = vdb::comments::remove();
                    break;
                case vdb::USER_COMMAND_ENUMS:
                    vdis::enumerations::print(std::cout);
                    break;
                case vdb::USER_COMMAND_ENTITIES:
                    vdis::entity_types::print(std::cout);
                    break;
                case vdb::USER_COMMAND_OBJECTS:
                    vdis::object_types::print(std::cout);
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
