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

#include "vdb_hexadecimal.h"
#include "vdb_ids.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_pdu.h"
#include "vdb_string.h"

namespace vdb
{
    std::vector<option_definition_t>
        options::option_definitions;
    std::map<user_command_e, std::string>
        options::command_names;
    std::map<user_command_e, option_set_t>
        options::command_options;
    std::vector<std::string>
        options::command_arguments;
    std::map<option_e, std::string>
        options::string_options;
    std::map<option_e, uint32_t>
        options::integer_options;
    std::map<option_e, uint32_set_t>
        options::integer_set_options;
    std::set<option_e>
        options::flags;
    std::vector<id_t>
        options::entity_ids;
    std::string
        options::terminal_command;
    user_command_e
        options::command = USER_COMMAND_NONE;
    bool
        options::initialized = false;
    const bool
        options::DEBUG = false;
}

// ----------------------------------------------------------------------------
void vdb::options::configure(void)
{
    command_names[USER_COMMAND_CAPTURE] = "capture";
    command_names[USER_COMMAND_PLAYBACK] = "playback";
    command_names[USER_COMMAND_LIST] = "list";
    command_names[USER_COMMAND_QUERY] = "query";
    command_names[USER_COMMAND_COMMENT] = "comment";
    command_names[USER_COMMAND_UNCOMMENT] = "uncomment";
    command_names[USER_COMMAND_ENUMS] = "enums";
    command_names[USER_COMMAND_ENTITIES] = "entities";
    command_names[USER_COMMAND_OBJECTS] = "objects";
    command_names[USER_COMMAND_TEST] = "test";

    add("all", 'A', OPT_ALL, OPT_ARG_NONE);
    add("address", 'a', OPT_NET_ADDRESS, OPT_ARG_REQUIRED);
    add("collisions", 0, OPT_COLLISIONS, OPT_ARG_NONE);
    add("color-off", 'c', OPT_COLOR_OFF, OPT_ARG_NONE);
    add("dump", 'd', OPT_DUMP, OPT_ARG_NONE);
    add("emissions", 0, OPT_EMISSIONS, OPT_ARG_NONE);
    add("errors-off", 'E', OPT_ERRORS_OFF, OPT_ARG_NONE);
    add("exercise", 'e', OPT_EXERCISE, OPT_ARG_REQUIRED);
    add("extract", 'x', OPT_EXTRACT, OPT_ARG_NONE);
    add("fires", 0, OPT_FIRES, OPT_ARG_NONE);
    add("help", 'h', OPT_HELP, OPT_ARG_NONE);
    add("ids", 'i', OPT_ENTITY_IDS, OPT_ARG_REQUIRED);
    add("iface", 'n', OPT_NET_INTERFACE, OPT_ARG_REQUIRED);
    add("ipv6", '6', OPT_IPV6, OPT_ARG_NONE);
    add("lasers", 0, OPT_LASERS, OPT_ARG_NONE);
    add("objects", 0, OPT_OBJECTS, OPT_ARG_NONE);
    add("quiet", 'q', OPT_QUIET, OPT_ARG_NONE);
    add("pcap", 'p', OPT_PCAP, OPT_ARG_NONE);
    add("pdu-family", 'f', OPT_PDU_FAMILY, OPT_ARG_REQUIRED);
    add("pdu-type", 't', OPT_PDU_TYPE, OPT_ARG_REQUIRED);
    add("range", 'r', OPT_PDU_RANGE, OPT_ARG_REQUIRED);
    add("radios", 0, OPT_RADIOS, OPT_ARG_NONE);
    add("summarize", 's', OPT_SUMMARIZE, OPT_ARG_NONE);
    add("verbose", 'v', OPT_VERBOSE, OPT_ARG_NONE);
    add("version", 0, OPT_VERSION, OPT_ARG_NONE);
    add("warnings", 'w', OPT_WARNINGS, OPT_ARG_NONE);

    command_options[USER_COMMAND_CAPTURE].insert(OPT_NET_ADDRESS);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_COLOR_OFF);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_DUMP);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_EXERCISE);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_EXTRACT);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_PDU_FAMILY);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_ENTITY_IDS);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_NET_INTERFACE);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_HELP);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_IPV6);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_QUIET);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_PDU_TYPE);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_SUMMARIZE);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_CAPTURE].insert(OPT_WARNINGS);

    command_options[USER_COMMAND_PLAYBACK].insert(OPT_NET_ADDRESS);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_COLOR_OFF);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_DUMP);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_EXERCISE);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_EXTRACT);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_PDU_FAMILY);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_ENTITY_IDS);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_NET_INTERFACE);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_HELP);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_IPV6);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_QUIET);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_PDU_RANGE);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_PDU_TYPE);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_SUMMARIZE);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_PLAYBACK].insert(OPT_PCAP);

    command_options[USER_COMMAND_LIST].insert(OPT_NET_ADDRESS);
    command_options[USER_COMMAND_LIST].insert(OPT_COLOR_OFF);
    command_options[USER_COMMAND_LIST].insert(OPT_DUMP);
    command_options[USER_COMMAND_LIST].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_LIST].insert(OPT_EXERCISE);
    command_options[USER_COMMAND_LIST].insert(OPT_EXTRACT);
    command_options[USER_COMMAND_LIST].insert(OPT_HELP);
    command_options[USER_COMMAND_LIST].insert(OPT_PDU_FAMILY);
    command_options[USER_COMMAND_LIST].insert(OPT_ENTITY_IDS);
    command_options[USER_COMMAND_LIST].insert(OPT_PDU_RANGE);
    command_options[USER_COMMAND_LIST].insert(OPT_SUMMARIZE);
    command_options[USER_COMMAND_LIST].insert(OPT_PDU_TYPE);
    command_options[USER_COMMAND_LIST].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_LIST].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_LIST].insert(OPT_PCAP);

    command_options[USER_COMMAND_QUERY].insert(OPT_COLOR_OFF);
    command_options[USER_COMMAND_QUERY].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_QUERY].insert(OPT_EXERCISE);
    command_options[USER_COMMAND_QUERY].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_QUERY].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_QUERY].insert(OPT_HELP);
    command_options[USER_COMMAND_QUERY].insert(OPT_COLLISIONS);
    command_options[USER_COMMAND_QUERY].insert(OPT_EMISSIONS);
    command_options[USER_COMMAND_QUERY].insert(OPT_FIRES);
    command_options[USER_COMMAND_QUERY].insert(OPT_LASERS);
    command_options[USER_COMMAND_QUERY].insert(OPT_RADIOS);
    command_options[USER_COMMAND_QUERY].insert(OPT_OBJECTS);
    command_options[USER_COMMAND_QUERY].insert(OPT_ALL);
    command_options[USER_COMMAND_QUERY].insert(OPT_PCAP);

    command_options[USER_COMMAND_COMMENT].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_COMMENT].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_COMMENT].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_COMMENT].insert(OPT_HELP);

    command_options[USER_COMMAND_UNCOMMENT].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_UNCOMMENT].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_UNCOMMENT].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_UNCOMMENT].insert(OPT_HELP);

    command_options[USER_COMMAND_ENUMS].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_ENUMS].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_ENUMS].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_ENUMS].insert(OPT_HELP);

    command_options[USER_COMMAND_ENTITIES].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_ENTITIES].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_ENTITIES].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_ENTITIES].insert(OPT_HELP);

    command_options[USER_COMMAND_OBJECTS].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_OBJECTS].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_OBJECTS].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_OBJECTS].insert(OPT_HELP);

    command_options[USER_COMMAND_TEST].insert(OPT_ERRORS_OFF);
    command_options[USER_COMMAND_TEST].insert(OPT_VERBOSE);
    command_options[USER_COMMAND_TEST].insert(OPT_WARNINGS);
    command_options[USER_COMMAND_TEST].insert(OPT_HELP);
}

// ----------------------------------------------------------------------------
bool vdb::options::initialize(int argc, char *argv[])
{
    const char
        *current_argument = 0,
        *next_argument = 0;
    bool
        advance = false,
        success = true;

    if (not initialized)
    {
        configure();
        initialized = true;
    }
    else
    {
        LOG_ERROR("Already initialized!");
        success = false;
    }

    for(int i = 0; success and (i < argc); ++i)
    {
        current_argument = argv[i];
        next_argument = ((i + 1) < argc) ? argv[i + 1] : 0;

        LOG_VERBOSE("Current argument '%s'...", current_argument);

        if (starts_with(current_argument, "--"))
        {
            success = parse_long_option(current_argument);
        }
        else if (starts_with(current_argument, "-"))
        {
            advance = false;

            success = parse_short_options(
                current_argument,
                next_argument,
                advance);

            i += (advance ? 1 : 0);
        }
        else if (terminal_command.empty())
        {
            // First non-option argument is the name of the terminal command.
            //
            parse_terminal_command(current_argument);
        }
        else if (command == USER_COMMAND_NONE)
        {
            // Second non-option argument is the VDB subcommand.
            //
            success = parse_command(current_argument);
        }
        else
        {
            command_arguments.push_back(std::string(current_argument));
        }
    }

    if (success)
    {
        success = verify();
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::verify(void)
{
    std::map<user_command_e, option_set_t>::const_iterator
        options_itor = command_options.find(command);
    bool
        success = true;

    LOG_VERBOSE("Verifying arguments...");

    if (options_itor != command_options.end())
    {
        const option_set_t
            &set = options_itor->second;

        std::map<option_e, std::string>::const_iterator
            string_itor = string_options.begin();
        std::map<option_e, uint32_t>::const_iterator
            integer_itor = integer_options.begin();
        std::set<option_e>::const_iterator
            flag_itor = flags.begin();

        while(string_itor != string_options.end())
        {
            if (set.find(string_itor->first) == set.end())
            {
                print_option_error(string_itor->first);
                success = false;
            }

            ++string_itor;
        }

        while(integer_itor != integer_options.end())
        {
            if (set.find(integer_itor->first) == set.end())
            {
                print_option_error(integer_itor->first);
                success = false;
            }

            ++integer_itor;
        }

        while(flag_itor != flags.end())
        {
            if (*flag_itor != OPT_EXTRA_VERBOSE)
            {
                if (set.find(*flag_itor) == set.end())
                {
                    print_option_error(*flag_itor);
                    success = false;
                }
            }

            ++flag_itor;
        }
   }

    return success;
}

// ----------------------------------------------------------------------------
void vdb::options::print_option_error(option_e option)
{
    const option_definition_t
        *definition_ptr = get_option(option);

    if (not definition_ptr)
    {
        LOG_ERROR("No option definition for %d!", option);
    }
    else
    {
        std::cerr << terminal_command << " " << command_names[command]
                  << ": option '--" << definition_ptr->long_value;

        if (definition_ptr->short_value > 0)
        {
            std::cerr << "|-" << definition_ptr->short_value;
        }

        std::cerr << "' not allowed" << std::endl;
    }
}

// ----------------------------------------------------------------------------
const std::string *vdb::options::string(option_e o)
{
    std::map<option_e, std::string>::const_iterator
        itor = string_options.find(o);

    return (itor != string_options.end()) ? &(itor->second) : 0;
}

// ----------------------------------------------------------------------------
const uint32_t *vdb::options::integer(option_e o)
{
    std::map<option_e, uint32_t>::const_iterator
        itor = integer_options.find(o);

    return (itor != integer_options.end()) ? &(itor->second) : 0;
}

// ----------------------------------------------------------------------------
const uint32_set_t *vdb::options::integer_set(option_e o)
{
    std::map<option_e, uint32_set_t>::const_iterator
        itor = integer_set_options.find(o);

    return (itor != integer_set_options.end()) ? &(itor->second) : 0;
}

// ----------------------------------------------------------------------------
bool vdb::options::entity_id_match(const pdu_t &pdu)
{
    if (entity_ids.empty())
    {
        return true;
    }
    else if (pdu.content_ptr)
    {
        for(uint32_t i = 0; i < entity_ids.size(); ++i)
        {
            if (pdu.content_ptr->contains(&entity_ids[i]))
            {
                return true;
            }
        }
    }

    LOG_VERBOSE("Filtered out PDU with no entity id match...");

    return false;
}

// ----------------------------------------------------------------------------
void vdb::options::parse_terminal_command(const char *current_argument)
{
    std::string::size_type
        index;

    terminal_command = current_argument;

    // Strip off the leading path if exists to get the base name.
    //
    index = terminal_command.find_last_of('/', terminal_command.length());

    if (index != std::string::npos)
    {
        terminal_command = terminal_command.substr(
            (index + 1),
            (terminal_command.length() - index));
    }

    if (DEBUG)
    {
        std::cout << "DEBUG: terminal_command: " << terminal_command
                  << std::endl;
    }
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_command(const char *current_argument)
{
    std::map<user_command_e, std::string>::const_iterator
        itor = command_names.begin();
    bool
        success = false;

    while(itor != command_names.end())
    {
        if (current_argument == itor->second)
        {
            if (DEBUG)
            {
                std::cout << "DEBUG: parse_command: " << current_argument
                          << std::endl;
            }

            command = itor->first;
            success = true;
        }

        ++itor;
    }

    if (not success)
    {
        std::cerr << terminal_command << ": unrecognized command '"
                  << current_argument << "', valid commands are: "
                  << std::endl;

        itor = command_names.begin();

        while(itor != command_names.end())
        {
            std::cerr << "  " << itor->second << std::endl;
            ++itor;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_long_option(const char *current_argument)
{
    const option_definition_t
        *option_ptr = 0;
    std::string
        argument = std::string(current_argument).substr(2),
        key,
        value;
    std::string::size_type
        index = argument.find_first_of('=');
    bool
        success = false;

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_long_option: index: " << index << std::endl;
    }

    if ((index == std::string::npos) or (index < 2))
    {
        key = argument;
    }
    else
    {
        key = argument.substr(0, index);
        value = argument.substr(index + 1);
    }

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_long_option: key: " << key << std::endl;
        std::cout << "DEBUG: parse_long_option: value: " << value << std::endl;
    }

    option_ptr = get_long_option(key);

    if (not option_ptr)
    {
        std::cerr << terminal_command << ": unrecognized option '--"
                  << key << "'" << std::endl;
    }
    else if (option_ptr->argument_required() and value.empty())
    {
        std::cerr << terminal_command << ": option '--" << key
                  << "' requires an argument" << std::endl;
    }
    else if (option_ptr->argument_none() and not value.empty())
    {
        std::cerr << terminal_command << ": option '--" << key
                  << "' doesn't allow an argument" << std::endl;
    }
    else if (not value.empty())
    {
        success = process_option(option_ptr->option, ("--" + key), value);
    }
    else
    {
        success = process_flag(option_ptr->option, ("--" + key));
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_short_options(
    const char *current_argument,
    const char *next_argument,
    bool &advance)
{
    const option_definition_t
        *option_ptr = 0;
    std::string
        argument = std::string(current_argument).substr(1);
    bool
        success = true;

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_short_options: '" << current_argument
                  << "', '" << (next_argument ? next_argument : "null")
                  << "'" << std::endl;
    }

    for(std::string::size_type i = 0; success and (i < argument.length()); ++i)
    {
        std::string
            name = "-";

        name += argument[i];

        option_ptr = get_short_option(argument[i]);

        if (not option_ptr)
        {
            std::cerr << terminal_command << ": invalid option -- '"
                      << argument[i] << "'" << std::endl;

            success = false;
        }
        else if (option_ptr->argument_required())
        {
            if ((i == (argument.length() - 1)) and next_argument)
            {
                success =  process_option(
                    option_ptr->option,
                    name,
                    next_argument);

                advance = true;
            }
            else
            {
                std::cerr << terminal_command
                          << ": option requires an argument -- '"
                          << argument[i] << "'" << std::endl;

                success = false;
            }
        }
        else
        {
            success = process_flag(option_ptr->option, name);
        }
    }

    return success;
}


// ----------------------------------------------------------------------------
bool vdb::options::process_option(
    option_e option,
    const std::string &name,
    const std::string &value)
{
    bool
        success = false;

    if (option == OPT_ENTITY_IDS)
    {
        success = parse_entity_ids(value);
    }
    else if ((option & OPT_STRING_MASK) == OPT_STRING_MASK)
    {
        if (DEBUG)
        {
            std::cout << "DEBUG: process_option: "
                      << " string: " << name << "='" << value << "'"
                      << std::endl;
        }

        // It's a string argument.
        //
        string_options[option] = value;
        success = true;
    }
    else if ((option & OPT_INTEGER_MASK) == OPT_INTEGER_MASK)
    {
        // It's an integer argument, make sure it's an integer.
        //
        uint32_t
            int_value;

        if (DEBUG)
        {
            std::cout << "DEBUG: process_option: "
                      << " integer: " << name << "='" << value << "'"
                      << std::endl;
        }

        if (string_to_uint32(value, int_value))
        {
            integer_options[option] = int_value;
            success = true;
        }
        else
        {
            std::cerr << terminal_command << ": option '" << name
                      << "' value '" << value << "' is not an integer"
                      << std::endl;
        }
    }
    else if ((option & OPT_INTEGER_SET_MASK) == OPT_INTEGER_SET_MASK)
    {
        if (DEBUG)
        {
            std::cout << "DEBUG: process_option: "
                      << " integer set: " << name << "='" << value << "'"
                      << std::endl;
        }

        success = parse_integer_set(option, name, value);
    }
    else
    {
        LOG_ERROR("Not sure what to do with this: %s", name.c_str());
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::process_flag(option_e option, const std::string &name)
{
    bool
        success = false;

    if ((option & OPT_FLAG_MASK) == OPT_FLAG_MASK)
    {
        if ((option == OPT_WARNINGS) and flag(OPT_VERBOSE))
        {
            std::cout << "Warning messages turned on..." << std::endl;
        }
        else if ((option == OPT_ERRORS_OFF) and flag(OPT_VERBOSE))
        {
            std::cout << "Error messages turned off..." << std::endl;
        }
        else if (option == OPT_VERBOSE)
        {
            if (not flag(OPT_VERBOSE))
            {
                std::cout << "Verboseness turned on..." << std::endl;
            }
            else if (not flag(OPT_EXTRA_VERBOSE))
            {
                std::cout << "Extra verboseness turned on..." << std::endl;

                option = OPT_EXTRA_VERBOSE;
            }
        }

        if (DEBUG)
        {
            std::cout << "DEBUG: process_flag: " << name
                      << std::endl;
        }

        flags.insert(option);
        success = true;
    }
    else
    {
        LOG_ERROR("Not sure what to do with '%s'", name.c_str());
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_entity_ids(const std::string &value)
{
    std::vector<std::string>
        values;
    bool
        success = true;

    tokenize_csv(value, values);

    for(uint32_t i = 0; success and (i < values.size()); ++i)
    {
        success = parse_entity_id(values[i]);
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_entity_id(std::string &value)
{
    std::vector<std::string>
        tokens;
    uint32_t
        id[3];
    bool
        success = false;

    // Tokenization uses space character as delimiter, swap ':' with ' '
    //
    for(unsigned i = 0; i < value.length(); ++i)
    {
        value[i] = (value[i] == '.') ? ' ' : value[i];
    }

    if (tokenize(value, tokens) == 3)
    {
        // Convert wildcards to the 'ALL' value
        //
        for(unsigned i = 0; i < 3; ++i)
        {
            if ((tokens[i] == "*") or (tokens[i] == "?"))
            {
                tokens[i] = "65535";
            }
        }

        if (string_to_uint32(tokens[0], id[0]) and
            string_to_uint32(tokens[1], id[1]) and
            string_to_uint32(tokens[2], id[2]))
        {
            if ((id[0] >= 0) and (id[0] <= 65535) and
                (id[1] >= 0) and (id[1] <= 65535) and
                (id[2] >= 0) and (id[2] <= 65535))
            {
                success = true;
            }
        }
    }

    if (success)
    {
        id_t
            entity_id(id[0], id[1], id[2]);

        entity_ids.push_back(entity_id);

        if (flag(OPT_VERBOSE))
        {
            std::cout << "Filtering on entity ID: " << entity_id.str()
                      << std::endl;
        }
    }
    else
    {
        std::cerr << terminal_command << ": invalid entity ID: " << value
                  << std::endl;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_integer_set(
    option_e option,
    const std::string &name,
    const std::string &value)
{
    std::vector<std::string>
        values,
        subvalues;
    uint32_set_t
        set;
    bool
        success = true;

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_integer_set: complete value '"
                  << value << "'" << std::endl;
    }

    tokenize_csv(value, values);

    for(uint32_t i = 0; success and (i < values.size()); ++i)
    {
        if (DEBUG)
        {
            std::cout << "DEBUG: parse_integer_set: value '"
                      << values[i] << "'" << std::endl;
        }

        // Is this token a range "N-M" or a single value N
        //
        if (values[i].find('-') == std::string::npos)
        {
            int64_t
                index = 0;

            if (string_to_int64(values[i], index))
            {
                if (index < 0)
                {
                    success = false;
                }
                else
                {
                    set.insert(index);
                }
            }
        }
        else
        {
            // Replace '-' with ' ' and split
            //
            for(unsigned j = 0; j < values[i].length(); ++j)
            {
                values[i][j] = (values[i][j] == '-') ? ',' : values[i][j];
            }

            success = (tokenize_csv(values[i], subvalues) == 2);

            if (success)
            {
                int64_t
                    index0 = 0,
                    index1 = 0;

                if (string_to_int64(subvalues[0], index0) and
                    string_to_int64(subvalues[1], index1))
                {
                    if ((index0 < 0) or (index1 < 0) or (index0 >= index1))
                    {
                        success = false;
                    }
                    else for(int64_t j = index0; j <= index1; ++j)
                    {
                        set.insert(j);
                    }
                }
                else
                {
                    success = false;
                }
            }
        }
    }

    if (success)
    {
        if (not set.empty())
        {
            integer_set_options[option] = set;
        }
    }
    else
    {
        std::cerr << terminal_command << ": option '" << name
                  << "' invalid argument '" << value << "'" << std::endl;
    }

    return success;
}

// ----------------------------------------------------------------------------
const vdb::option_definition_t *vdb::options::get_option(
    option_e value)
{
    for(uint32_t i = 0; i < option_definitions.size(); ++i)
    {
        if (option_definitions[i].option == value)
        {
            return &option_definitions[i];
        }
    }

    return 0;
}

// ----------------------------------------------------------------------------
const vdb::option_definition_t *vdb::options::get_short_option(
    char value)
{
    for(uint32_t i = 0; i < option_definitions.size(); ++i)
    {
        if (option_definitions[i].short_value == value)
        {
            return &option_definitions[i];
        }
    }

    return 0;
}

// ----------------------------------------------------------------------------
const vdb::option_definition_t *vdb::options::get_long_option(
    const std::string &value)
{
    for(uint32_t i = 0; i < option_definitions.size(); ++i)
    {
        if (option_definitions[i].long_value == value)
        {
            return &option_definitions[i];
        }
    }

    return 0;
}
