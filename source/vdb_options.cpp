#include "vdb_options.h"

#include "vdis_data_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

namespace vdb
{
    user_command_e
        options::command = USER_COMMAND_NONE;
    std::map<user_command_e, std::string>
        options::command_names;
    std::vector<std::string>
        options::command_arguments;
    std::set<std::string>
        options::include_hostnames;
    std::set<vdis::id_t>
        options::include_entity_ids,
        options::exclude_entity_ids;
    std::set<uint8_t>
        options::include_exercises,
        options::exclude_exercises,
        options::include_types,
        options::exclude_types,
        options::include_families,
        options::exclude_families;
    std::set<uint32_t>
        options::pdu_index_range;
    std::string
        options::network_address,
        options::network_interface;
    bool
        options::initialized = false,
        options::quiet_mode = false,
        options::use_ipv6 = false,
        options::use_pcap = false,
        options::show_version = false,
        options::show_help = false,
        options::show_examples = false,
        options::show_pdu_dump = false,
        options::show_pdu_extracted = false,
        options::show_pdu_summary = false,
        options::associations = false,
        options::extra = false,
        options::summary_collisions = false,
        options::summary_emissions = false,
        options::summary_fires = false,
        options::summary_lasers = false,
        options::summary_objects = false,
        options::summary_radios = false;
    const bool
        options::DEBUG = (getenv("VDB_OPTIONS_DEBUG") != 0);
}

// ----------------------------------------------------------------------------
void vdb::options::configure(void)
{
    command_names[USER_COMMAND_CAPTURE] = "capture";
    command_names[USER_COMMAND_PLAYBACK] = "playback";
    command_names[USER_COMMAND_LIST] = "list";
    command_names[USER_COMMAND_SUMMARY] = "query";
    command_names[USER_COMMAND_COMMENT] = "comment";
    command_names[USER_COMMAND_UNCOMMENT] = "uncomment";
    command_names[USER_COMMAND_ENUMS] = "enums";
    command_names[USER_COMMAND_ENTITIES] = "entities";
    command_names[USER_COMMAND_OBJECTS] = "objects";
}

// ----------------------------------------------------------------------------
bool vdb::options::verify_long_argument_value(
    const std::string &name,
    const std::string &value,
    bool required,
    bool &success)
{
    if (required and value.empty())
    {
        std::cerr << "vdb: option requires a value: --"
                  << name << std::endl;

        success = false;
    }
    else if (not required and not value.empty())
    {
        std::cerr << "vdb: option does not require a value: --"
                  << name << std::endl;

        success = false;
    }
    else
    {
        success = true;
    }

    return success;
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

    if (initialized)
    {
        LOG_ERROR("Already initialized!");
        success = false;
    }
    else
    {
        configure();
        initialized = true;
    }

    for(int i = 0; success and (i < argc); ++i)
    {
        current_argument = argv[i];
        next_argument = ((i + 1) < argc) ? argv[i + 1] : 0;

        LOG_VERBOSE("Current argument '%s'...", current_argument);

        if (vdis::starts_with(current_argument, "--"))
        {
            success = parse_long_option(current_argument);
        }
        else if (vdis::starts_with(current_argument, "-"))
        {
            advance = false;

            success = parse_short_options(
                current_argument,
                next_argument,
                advance);

            i += (advance ? 1 : 0);
        }
        else if (i > 0)
        {
            if (command == USER_COMMAND_NONE)
            {
                success = parse_command(current_argument);
            }
            else
            {
                command_arguments.push_back(std::string(current_argument));
            }
        }
    }

    return success;
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
        std::cerr << "vdb: unrecognized command '" << current_argument
                  << "', valid commands are: " << std::endl;

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
    std::string
        argument = std::string(current_argument).substr(2),
        name,
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
        name = argument;
    }
    else
    {
        name = argument.substr(0, index);
        value = argument.substr(index + 1);
    }

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_long_option: name: " << name << std::endl;
        std::cout << "DEBUG: parse_long_option: value: " << value << std::endl;
    }

    if (name == "address")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            network_address = value;
        }
    }
    else if (name == "help")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            show_help = true;
        }
    }
    else if (name == "mono")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            color::set_enabled(false);
        }
    }
    else if (name == "verbose")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            if (logger::is_enabled(logger::VERBOSE))
            {
                logger::set_enabled(logger::EXTRA_VERBOSE, true);
            }
            else
            {
                logger::set_enabled(logger::VERBOSE, true);
            }
        }
    }
    else if (name == "warnings")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            logger::set_enabled(logger::WARNING, true);
        }
    }
    else if (name == "suppress")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            logger::set_enabled(logger::ERROR, false);
        }
    }
    else if (name == "examples")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            show_examples = true;
        }
    }
    else if (name == "version")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            show_version = true;
        }
    }
    else if (name == "exercise")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set(
                "--exercise",
                value.c_str(),
                include_exercises);
        }
    }
    else if (name == "xexercise")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set(
                "--xexercise",
                value.c_str(),
                exclude_exercises);
        }
    }
    else if (name == "type")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set(
                "--type",
                value.c_str(),
                include_types);
        }
    }
    else if (name == "xtype")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set(
                "--xtype",
                value.c_str(),
                exclude_types);
        }
    }
    else if (name == "family")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set(
                "--family",
                value.c_str(),
                include_families);
        }
    }
    else if (name == "xfamily")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set(
                "--xfamily",
                value.c_str(),
                exclude_families);
        }
    }
    else if (name == "ids")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_entity_ids(
                "--ids",
                value.c_str(),
                include_entity_ids);
        }
    }
    else if (name == "xids")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_entity_ids(
                "--xids",
                value.c_str(),
                exclude_entity_ids);
        }
    }
    else if (name == "pcap")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            use_pcap = true;
        }
    }
    else if (name == "ipv6")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            use_ipv6 = true;
        }
    }
    else if (name == "iface")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            network_interface = value;
        }
    }
    else if (name == "range")
    {
        if (verify_long_argument_value(name, value, true, success))
        {
            success = parse_integer_set_in_range(
                "--range",
                value.c_str(),
                0x0,
                0xFFFFFFFF,
                pdu_index_range);
        }
    }
    else if (name == "quiet")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            quiet_mode = true;
        }
    }
    else if (name == "summarize")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            show_pdu_summary = true;
        }
    }
    else if (name == "dump")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            show_pdu_dump = true;
        }
    }
    else if (name == "extract")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            show_pdu_extracted = true;
        }
    }
    else if (name == "associations")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            associations = true;
        }
    }
    else if (name == "extra")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            extra = true;
        }
    }
    else if (name == "collisions")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_collisions = true;
        }
    }
    else if (name == "emissions")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_collisions = true;
        }
    }
    else if (name == "fires")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_fires = true;
        }
    }
    else if (name == "lasers")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_lasers = true;
        }
    }
    else if (name == "objects")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_objects = true;
        }
    }
    else if (name == "radios")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_radios = true;
        }
    }
    else if (name == "all")
    {
        if (verify_long_argument_value(name, value, false, success))
        {
            summary_collisions = true;
            summary_emissions = true;
            summary_fires = true;
            summary_lasers = true;
            summary_objects = true;
            summary_radios = true;
        }
    }
    else
    {
        std::cerr << "vdb: invalid option: --" << name << std::endl;
        success = false;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_short_options(
    const char *current_argument,
    const char *next_argument,
    bool &advance)
{
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
        switch(argument[i])
        {
            case '6':
                use_ipv6 = true;
                break;
            case 'a':
                success = parse_string(
                    "-a",
                    next_argument,
                    network_address);
                advance = true;
                break;
            case 'A':
                summary_collisions = true;
                summary_emissions = true;
                summary_fires = true;
                summary_lasers = true;
                summary_objects = true;
                summary_radios = true;
                break;
            case 'C':
                command = USER_COMMAND_CAPTURE;
                break;
            case 'd':
                show_pdu_dump = true;
                break;
            case 'e':
                success = parse_integer_set(
                    "-e",
                    next_argument,
                    include_exercises);
                advance = true;
                break;
            case 'E':
                success = parse_integer_set(
                    "-E",
                    next_argument,
                    exclude_exercises);
                advance = true;
                break;
            case 'f':
                success = parse_integer_set(
                    "-f",
                    next_argument,
                    include_families);
                advance = true;
                break;
            case 'F':
                success = parse_integer_set(
                    "-F",
                    next_argument,
                    exclude_families);
                advance = true;
                break;
            case 'h':
                show_help = true;
                break;
            case 'H':
                success = parse_string_set(
                    "-H",
                    next_argument,
                    include_hostnames);
                advance = true;
                break;
            case 'i':
                success = parse_entity_ids(
                    "-i",
                    next_argument,
                    include_entity_ids);
                advance = true;
                break;
            case 'I':
                success = parse_entity_ids(
                    "-I",
                    next_argument,
                    include_entity_ids);
                advance = true;
                break;
            case 'L':
                command = USER_COMMAND_LIST;
                break;
            case 'm':
                color::set_enabled(false);
                break;
            case 'M':
                command = USER_COMMAND_ENUMS;
                break;
            case 'n':
                success = parse_string(
                    "-n",
                    next_argument,
                    network_interface);
                advance = true;
                break;
            case 'p':
                use_pcap = true;
                break;
            case 'P':
                command = USER_COMMAND_PLAYBACK;
                break;
            case 'q':
                quiet_mode = true;
                break;
            case 'r':
                success = parse_integer_set_in_range(
                    "-r",
                    next_argument,
                    0x0,
                    0xFFFFFFFF,
                    pdu_index_range);
                advance = true;
                break;
            case 'R':
                logger::set_enabled(logger::ERROR, false);
                break;
            case 's':
                show_pdu_summary = true;
                break;
            case 'S':
                command = USER_COMMAND_SUMMARY;
                break;
            case 't':
                success = parse_integer_set(
                    "-t",
                    next_argument,
                    include_types);
                advance = true;
                break;
            case 'T':
                success = parse_integer_set(
                    "-T",
                    next_argument,
                    exclude_types);
                advance = true;
                break;
            case 'v':
                if (logger::is_enabled(logger::VERBOSE))
                {
                    logger::set_enabled(logger::EXTRA_VERBOSE, true);
                }
                else
                {
                    logger::set_enabled(logger::VERBOSE, true);
                }
                break;
            case 'U':
                command = USER_COMMAND_UNCOMMENT;
                break;
            case 'V':
                show_version = true;
                break;
            case 'w':
                logger::set_enabled(logger::WARNING, true);
                break;
            case 'W':
                command = USER_COMMAND_COMMENT;
                break;
            case 'x':
                show_pdu_extracted = true;
                break;
            case 'X':
                extra = true;
                break;
            default:
                std::cerr << "vdb: invalid option: -" << argument[i]
                          << std::endl;
                success = false;
                break;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_string_set(
    const char *name_ptr,
    const char *value_ptr,
    std::set<std::string> &set)
{
    std::vector<std::string>
        values;
    bool
        success = false;

    if (not value_ptr)
    {
        std::cerr << "vdb: option requires a value: " << name_ptr << std::endl;
    }
    else
    {
        vdis::tokenize_csv(std::string(value_ptr), values);

        if (DEBUG)
        {
            std::cout << "DEBUG: parse_string_set: '" << name_ptr
                      << "'='" << value_ptr << "'" << std::endl;
        }

        for(uint32_t i = 0; (i < values.size()) and not success; ++i)
        {
            if (not vdis::trim(values[i]).empty())
            {
                set.insert(values[i]);
                success = true;
            }
        }

        if (not success)
        {
            std::cerr << "vdb: invalid value for option: "
                      << name_ptr << std::endl;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_entity_ids(
    const char *name_ptr,
    const char *value_ptr,
    std::set<vdis::id_t> &ids)
{
    std::vector<std::string>
        values;
    bool
        success = true;

    if (not value_ptr)
    {
        std::cerr << "vdb: option requires a value: " << name_ptr << std::endl;
    }
    else
    {
        vdis::tokenize_csv(std::string(value_ptr), values);

        if (DEBUG)
        {
            std::cout << "DEBUG: parse_entity_ids: '" << name_ptr
                      << "'='" << value_ptr << "'" << std::endl;
        }

        for(uint32_t i = 0; success and (i < values.size()); ++i)
        {
            success = parse_entity_id(name_ptr, values[i].c_str(), ids);
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_entity_id(
    const char *name_ptr,
    const char *value_ptr,
    std::set<vdis::id_t> &ids)
{
    bool
        success = false;

    if (not value_ptr)
    {
        std::cerr << "vdb: option requires a value: " << name_ptr << std::endl;
    }
    else
    {
        std::vector<std::string>
            tokens;
        std::string
            swapped = value_ptr;
        uint32_t
            id[3];

        // Tokenization uses space character as delimiter, swap '.' with ' '
        //
        for(unsigned i = 0; i < swapped.length(); ++i)
        {
            swapped[i] = (swapped[i] == '.') ? ' ' : swapped[i];
        }

        if (vdis::tokenize(swapped, tokens) != 3)
        {
            std::cerr << "vdb: expected entity ID in form '1.2.3' instead of '"
                      << value_ptr << "'" << std::endl;
        }
        else
        {
            // Convert wildcards to the 'ALL' value
            //
            for(unsigned i = 0; i < 3; ++i)
            {
                if ((tokens[i] == "*") or (tokens[i] == "?"))
                {
                    tokens[i] = vdis::to_string((int)vdis::id_t::ALL);
                }
            }

            if (vdis::to_uint32(tokens[0], id[0]) and
                vdis::to_uint32(tokens[1], id[1]) and
                vdis::to_uint32(tokens[2], id[2]))
            {
                if ((id[0] >= 0) and (id[0] <= vdis::id_t::ALL) and
                    (id[1] >= 0) and (id[1] <= vdis::id_t::ALL) and
                    (id[2] >= 0) and (id[2] <= vdis::id_t::ALL))
                {
                    success = true;
                }
                else
                {
                    std::cerr << "vdb: value out of range in '" << value_ptr
                              << "'" << std::endl;
                }
            }
            else
            {
                std::cerr << "vdb: invalid value in '" << value_ptr
                          << "'" << std::endl;
            }
        }

        if (success)
        {
            vdis::id_t entity_id = {
                (uint16_t)id[0],
                (uint16_t)id[1],
                (uint16_t)id[2] };

            ids.insert(entity_id);

            if (DEBUG)
            {
                std::cout << "DEBUG: parse_entity_id: '"
                          << value_ptr << "' -> " << entity_id << std::endl;
            }
        }
        else
        {
            std::cerr << "vdb: option '" << name_ptr
                      << "': invalid entity ID: " << value_ptr << std::endl;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_integer_set(
    const char *name_ptr,
    const char *value_ptr,
    std::set<uint8_t> &set)
{
    std::set<uint32_t>
        temp_set;
    bool
        success = parse_integer_set_in_range(
            name_ptr,
            value_ptr,
            0,
            255,
            temp_set);

    if (success)
    {
        std::set<uint32_t>::const_iterator
            itor = temp_set.begin();

        for(itor = temp_set.begin(); itor != temp_set.end(); ++itor)
        {
            set.insert((uint8_t)*itor);
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_integer_set_in_range(
    const char *name_ptr,
    const char *value_ptr,
    int64_t min,
    int64_t max,
    std::set<uint32_t> &set)
{
    std::vector<std::string>
        values,
        subvalues;
    bool
        success = true;

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_integer_set: " << name_ptr << "='"
                  << (value_ptr ? value_ptr : "null") << "'" << std::endl;
    }

    if (value_ptr)
    {
        vdis::tokenize_csv(std::string(value_ptr), values);
    }
    else
    {
        std::cerr << "vdb: option requires a value: " << name_ptr << std::endl;
        success = false;
    }

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
                value = 0;

            if (vdis::to_int64(values[i], value))
            {
                if (DEBUG)
                {
                    std::cout << "DEBUG: parse_integer_set: value "
                              << value << std::endl;
                }

                if ((value < min) or (value > max))
                {
                    std::cerr << "vdb: value out of range: " << value
                              << " (" << min << "-" << max << ")" << std::endl;
                    success = false;
                }
                else
                {
                    set.insert((uint32_t)value);
                }
            }
        }
        else
        {
            // Replace '-' with ',' and split
            //
            for(uint32_t j = 0; j < values[i].length(); ++j)
            {
                values[i][j] = (values[i][j] == '-') ? ',' : values[i][j];
            }

            success = (vdis::tokenize_csv(values[i], subvalues) == 2);

            if (success)
            {
                int64_t
                    value0 = 0,
                    value1 = 0;

                success = false;

                if (vdis::to_int64(subvalues[0], value0) and
                    vdis::to_int64(subvalues[1], value1))
                {
                    if ((value0 < min) or (value0 > max))
                    {
                        std::cerr << "vdb: value out of range: " << value0
                                  << " (" << min << "-" << max << ")"
                                  << std::endl;
                    }
                    else if ((value1 < min) or (value1 > max))
                    {
                        std::cerr << "vdb: value out of range: " << value1
                                  << " (" << min << "-" << max << ")"
                                  << std::endl;
                    }
                    else if (value0 >= value1)
                    {
                        std::cerr << "vdb: invalid range: " << value0
                                  << "-" << value1 << std::endl;
                    }
                    else
                    {
                        success = true;

                        for(int64_t j = value0; j <= value1; ++j)
                        {
                            set.insert(j);
                        }
                    }
                }
            }
        }
    }

    if (value_ptr and not success)
    {
        std::cerr << "vdb: option '" << name_ptr
                  << "' invalid argument '" << value_ptr << "'" << std::endl;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::parse_string(
    const char *name_ptr,
    const char *value_ptr,
    std::string &value)
{
    bool
        success = true;

    if (value_ptr)
    {
        value = value_ptr;
    }
    else
    {
        std::cerr << "vdb: option requires a value: " << name_ptr << std::endl;
    }

    return success;
}
