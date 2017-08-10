#include "vdb_command.h"

// ----------------------------------------------------------------------------
vdb::command_t::command_t(
    const std::string &name,
    const std::vector<std::string> &arguments
) :
    name(name),
    options(name, arguments)
{
    options.set_command(this);

    // Add global options for all commands:
    //
    options.add(OPTION_ERRORS);
    options.add(OPTION_EXTRA);
    options.add(OPTION_MONO);
    options.add(OPTION_VERBOSE);
    options.add(OPTION_WARNINGS);
}

// ----------------------------------------------------------------------------
vdb::command_t::~command_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::command_t::parse_options(void)
{
    return options.parse();
}

// ----------------------------------------------------------------------------
bool vdb::command_t::option_callback(
    const option_t &option,
    const std::string &value,
    bool &success)
{
    // Return false to indicate that option was not processed, child
    // classes should override as needed.
    //
    return false;
}
