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
void print_examples(void);
void print_examples_all(void);
void print_examples_capture(void);
void print_examples_playback(void);
void print_examples_list(void);
void print_examples_summary(void);
void print_examples_comment(void);
void print_examples_uncomment(void);
void print_examples_enums(void);
void print_examples_entities(void);
void print_examples_objects(void);

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
        if (vdb::options::show_version)
        {
            print_version();
        }

        if (vdb::options::show_help)
        {
            print_help();
        }
        else if (vdb::options::show_examples)
        {
            print_examples();
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
                    if (not vdb::options::show_version)
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
              << " " << VDB_VERSION_DATE << "]" << std::endl;
}

#define PRINT(x) std::cout << (x) << std::endl;

// ----------------------------------------------------------------------------
void print_help(void)
{
    #include "vdb_help.txt"
}

// ----------------------------------------------------------------------------
void print_examples(void)
{
    switch(vdb::options::command)
    {
        case vdb::USER_COMMAND_CAPTURE:
            print_examples_capture();
            break;
        case vdb::USER_COMMAND_PLAYBACK:
            print_examples_playback();
            break;
        case vdb::USER_COMMAND_LIST:
            print_examples_list();
            break;
        case vdb::USER_COMMAND_SUMMARY:
            print_examples_summary();
            break;
        case vdb::USER_COMMAND_COMMENT:
            print_examples_comment();
            break;
        case vdb::USER_COMMAND_UNCOMMENT:
            print_examples_uncomment();
            break;
        case vdb::USER_COMMAND_ENUMS:
            print_examples_enums();
            break;
        case vdb::USER_COMMAND_ENTITIES:
            print_examples_entities();
            break;
        case vdb::USER_COMMAND_OBJECTS:
            print_examples_objects();
            break;
        default:
            print_examples_all();
            break;
    }
}

// ----------------------------------------------------------------------------
void print_examples_all(void)
{
    print_examples_capture();
    print_examples_playback();
    print_examples_list();
    print_examples_summary();
    print_examples_comment();
    print_examples_uncomment();
    print_examples_enums();
    print_examples_entities();
    print_examples_objects();
}

// ----------------------------------------------------------------------------
void print_examples_capture(void)
{
    std::cout << "capture examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_capture.txt"
}

// ----------------------------------------------------------------------------
void print_examples_playback(void)
{
    std::cout << "playback examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_playback.txt"
}

// ----------------------------------------------------------------------------
void print_examples_list(void)
{
    std::cout << "list examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_list.txt"
}

// ----------------------------------------------------------------------------
void print_examples_summary(void)
{
    std::cout << "summary examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_summary.txt"
}

// ----------------------------------------------------------------------------
void print_examples_comment(void)
{
    std::cout << "comment examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_comment.txt"
}

// ----------------------------------------------------------------------------
void print_examples_uncomment(void)
{
    std::cout << "uncomment examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_uncomment.txt"
}

// ----------------------------------------------------------------------------
void print_examples_enums(void)
{
    std::cout << "enums examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_enums.txt"
}

// ----------------------------------------------------------------------------
void print_examples_entities(void)
{
    std::cout << "entities examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_entities.txt"
}

// ----------------------------------------------------------------------------
void print_examples_objects(void)
{
    std::cout << "objects examples:" << std::endl << std::endl;
    #include "examples/vdb_examples_objects.txt"
}

#undef PRINT
