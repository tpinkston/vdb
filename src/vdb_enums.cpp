#include "vdb_enums.h"
#include "vdb_enums_help.h"
#include "vdb_options.h"
#include "vdb_version.h"

#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"

namespace
{
    vdb::enums_t
        enums;
}

bool option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    vdb::options_t
        options("vdb-enums", argc, argv);
    int
        result = 1;

    options.add(OPTION_ERRORS);
    options.add(OPTION_WARNINGS);
    options.add(OPTION_VERBOSE);
    options.add(OPTION_HELP);

    options.set_callback(*option_callback);

    if (options.parse())
    {
        if (vdb::options::version)
        {
            print_vdb_version();
            result = 0;
        }
        else if (vdb::options::help)
        {
            print_help();
            result = 0;
        }
        else
        {
            result = enums.run();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success)
{
    return false;
}

// ----------------------------------------------------------------------------
int vdb::enums_t::run(void)
{
    int result = 1;

    // File argument required
    //
    if (options::command_arguments.size() > 0)
    {
        std::cerr << "vdb-enums: too many arguments" << std::endl;
    }
    else
    {
         vdis::enumerations::load();
         vdis::enumerations::print(std::cout);
         result = 0;
    }

    return result;
}
