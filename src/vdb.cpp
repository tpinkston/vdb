#include "vdb_capture.h"
#include "vdb_comments.h"
#include "vdb_git.h"
#include "vdb_list.h"
#include "vdb_options.h"
#include "vdb_playback.h"
#include "vdb_summary.h"
#include "vdb_version.h"

#include "vdis_entity_types.h"
#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_services.h"

void print_version(void);
void print_help(void);

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
        if (vdb::options::version)
        {
            print_version();
        }

        if (vdb::options::help)
        {
            print_help();
        }
        else
        {
            vdis::set_byteswapping();

            vdis::enumerations::load();
            vdis::entity_types::load();
            vdis::object_types::load();

            switch(vdb::options::command)
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
                case vdb::USER_COMMAND_SUMMARY:
                    result = vdb::summary::summarize_pdus();
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
                    if (not vdb::options::version)
                    {
                        std::cerr << "vdb: missing command (try --help)"
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
              << " " << __DATE__ << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void print_help(void)
{
    #define PRINT(x) std::cout << (x) << std::endl;
    #include "vdb_help.txt"
    #undef PRINT
}
