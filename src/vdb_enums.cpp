#include "vdb_enums.h"
#include "vdb_options.h"

#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"

namespace
{
    vdb::enums_t
        enums;
}

bool enums_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
int enums_main(int argc, char *argv[])
{
    vdb::options_t
        options("vdb-enums", argc, argv);
    int
        result = 1;

    options.add(OPTION_ERRORS);
    options.add(OPTION_HELP);
    options.add(OPTION_MONO);
    options.add(OPTION_VERBOSE);
    options.add(OPTION_WARNINGS);

    options.set_callback(*enums_option_callback);

    if (options.parse())
    {
        result = enums.run();
    }

    return result;
}

// ----------------------------------------------------------------------------
bool enums_option_callback(
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
