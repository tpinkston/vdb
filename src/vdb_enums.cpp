#include "vdb_enums.h"
#include "vdb_options.h"

#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"

// ----------------------------------------------------------------------------
vdb::enums_t::enums_t(
    const std::string &name,
    const std::vector<std::string> &arguments
) :
    command_t(name, arguments)
{

}

// ----------------------------------------------------------------------------
vdb::enums_t::~enums_t(void)
{

}

// ----------------------------------------------------------------------------
int vdb::enums_t::run(void)
{
    int result = 0;

    if (options::command_arguments.size() > 0)
    {
        LOG_FATAL("No arguments allowed for this command");
        result = 1;
    }
    else
    {
        vdis::enumerations::load();
        vdis::enumerations::print(std::cout);
    }

    return result;
}
