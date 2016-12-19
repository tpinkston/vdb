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
        options::output_file,
        options::network_address,
        options::network_interface;
    uint64_t
        options::pdu_interval = 0L;
    bool
        options::initialized = false,
        options::quiet = false,
        options::ipv6 = false,
        options::pcap = false,
        options::version = false,
        options::help = false,
        options::examples = false,
        options::dump = false,
        options::extracted = false,
        options::extra = false,
        options::scanning = false,
        options::scan_associations = false,
        options::scan_lasers = false,
        options::scan_fires = false,
        options::scan_collisions = false,
        options::scan_entities = false,
        options::scan_objects = false,
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

    if (success)
    {
        // Do a little sanity check
        //
        if ((command != USER_COMMAND_LIST) and not output_file.empty())
        {
            std::cerr << "vdb: output file option (-o or --output) "
                      << "for list command only" << std::endl;
            success = false;
        }
        else if (scanning and
                (command != USER_COMMAND_CAPTURE) and
                (command != USER_COMMAND_LIST))
        {
            std::cerr << "vdb: scan option (-N or --scan) "
                      << "for capture and list commands only" << std::endl;
            success = false;
        }
    }

    if (DEBUG)
    {
        std::cout << "DEBUG: initialize returning: "
                  << (success ? "true" : "false")
                  << std::endl;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options::set_command(user_command_e user_command)
{
    bool
        success = false;

    if (command == USER_COMMAND_NONE)
    {
        command = user_command;
        success = true;
    }
    else
    {
        std::cerr << "vdb: extra command '" << command_names[user_command]
                  << "' - try --help" << std::endl;
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
    const std::string
        argument = std::string(current_argument).substr(2);
    std::string
        name;
    const char
        *value;
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
        value = argument.substr(index + 1).c_str();
    }

    if (DEBUG)
    {
        std::cout << "DEBUG: parse_long_option: name: "
                  << name << std::endl
                  << "DEBUG: parse_long_option: value: "
                  << value << std::endl;
    }

    if (name == "address")
    {
        success = parse_string("--address", value, network_address);
    }
    else if (name == "help")
    {
        help = true;
    }
    else if (name == "mono")
    {
        color::set_enabled(false);
    }
    else if (name == "verbose")
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
    else if (name == "warnings")
    {
        logger::set_enabled(logger::WARNING, true);
    }
    else if (name == "suppress")
    {
        logger::set_enabled(logger::ERROR, false);
    }
    else if (name == "examples")
    {
        examples = true;
    }
    else if (name == "version")
    {
        version = true;
    }
    else if (name == "hostname")
    {
        success = parse_string_set("--hostname", value, include_hostnames);
    }
    else if (name == "exercise")
    {
        success = parse_integers("--exercise", value, include_exercises);
    }
    else if (name == "xexercise")
    {
        success = parse_integers("--xexercise", value, exclude_exercises);
    }
    else if (name == "type")
    {
        success = parse_integers("--type", value, include_types);
    }
    else if (name == "xtype")
    {
        success = parse_integers("--xtype", value, exclude_types);
    }
    else if (name == "family")
    {
        success = parse_integers("--family", value, include_families);
    }
    else if (name == "xfamily")
    {
        success = parse_integers("--xfamily", value, exclude_families);
    }
    else if (name == "ids")
    {
        success = parse_entity_ids("--ids", value, include_entity_ids);
    }
    else if (name == "xids")
    {
        success = parse_entity_ids("--xids", value, exclude_entity_ids);
    }
    else if (name == "pcap")
    {
        pcap = true;
    }
    else if (name == "ipv6")
    {
        ipv6 = true;
    }
    else if (name == "iface")
    {
        success = parse_string("--iface", value, network_interface);
    }
    else if (name == "range")
    {
        success = parse_integers_in_range(
            "--range",
            value,
            0x0,
            0xFFFFFFFF,
            pdu_index_range);
    }
    else if (name == "pdu")
    {
        success = parse_uint64("--pdu", value, pdu_interval);
    }
    else if (name == "output")
    {
        success = parse_string("--output", value, output_file);
    }
    else if (name == "quiet")
    {
        quiet = true;
    }
    else if (name == "dump")
    {
        dump = true;
    }
    else if (name == "extract")
    {
        extracted = true;
    }
    else if (name == "extra")
    {
        extra = true;
    }
    else if (name == "scan")
    {
        std::set<std::string> scans;

        success =
            parse_string_set("--hostname", value, scans) and
            parse_scans("--name", scans);
    }
    else if (name == "collisions")
    {
        summary_collisions = true;
    }
    else if (name == "emissions")
    {
        summary_collisions = true;
    }
    else if (name == "fires")
    {
        summary_fires = true;
    }
    else if (name == "lasers")
    {
        summary_lasers = true;
    }
    else if (name == "objects")
    {
        summary_objects = true;
    }
    else if (name == "radios")
    {
        summary_radios = true;
    }
    else if (name == "all")
    {
        summary_collisions = true;
        summary_emissions = true;
        summary_fires = true;
        summary_lasers = true;
        summary_objects = true;
        summary_radios = true;
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
    std::set<std::string>
        scans;
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
                ipv6 = true;
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
                success = set_command(USER_COMMAND_CAPTURE);
                break;
            case 'D':
                success = parse_uint64(
                    "-D",
                    next_argument,
                    pdu_interval);
                advance = true;
                break;
            case 'd':
                dump = true;
                break;
            case 'e':
                success = parse_integers(
                    "-e",
                    next_argument,
                    include_exercises);
                advance = true;
                break;
            case 'E':
                success = parse_integers(
                    "-E",
                    next_argument,
                    exclude_exercises);
                advance = true;
                break;
            case 'f':
                success = parse_integers(
                    "-f",
                    next_argument,
                    include_families);
                advance = true;
                break;
            case 'F':
                success = parse_integers(
                    "-F",
                    next_argument,
                    exclude_families);
                advance = true;
                break;
            case 'h':
                help = true;
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
                success = parse_entity_ids("-I",
                    next_argument,
                    include_entity_ids);
                advance = true;
                break;
            case 'L':
                success = set_command(USER_COMMAND_LIST);
                break;
            case 'm':
                color::set_enabled(false);
                break;
            case 'M':
                success = set_command(USER_COMMAND_ENUMS);
                break;
            case 'n':
                success = parse_string("-n", next_argument, network_interface);
                advance = true;
                break;
            case 'N':
                success =
                    parse_string_set("-n", next_argument, scans) and
                    parse_scans("-n", scans);
                advance = true;
                break;
            case 'O':
                success = parse_string("-O", next_argument, output_file);
                advance = true;
                break;
            case 'p':
                pcap = true;
                break;
            case 'P':
                success = set_command(USER_COMMAND_PLAYBACK);
                break;
            case 'q':
                quiet = true;
                break;
            case 'r':
                success = parse_integers_in_range(
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
            case 'S':
                success = set_command(USER_COMMAND_SUMMARY);
                break;
            case 't':
                success = parse_integers("-t", next_argument, include_types);
                advance = true;
                break;
            case 'T':
                success = parse_integers("-T", next_argument, exclude_types);
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
                success = set_command(USER_COMMAND_UNCOMMENT);
                break;
            case 'V':
                version = true;
                break;
            case 'w':
                logger::set_enabled(logger::WARNING, true);
                break;
            case 'W':
                success = set_command(USER_COMMAND_COMMENT);
                break;
            case 'x':
                extracted = true;
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
bool  vdb::options::parse_scans(
    const char *name_ptr,
    const std::set<std::string> &set)
{
    std::set<std::string>::const_iterator
        itor = set.begin();
    bool
        success = true;

    while(success and (itor != set.end()))
    {
        if (vdis::to_lowercase(*itor) == "associations")
        {
            scan_associations = true;
        }
        else if (vdis::to_lowercase(*itor) == "lasers")
        {
            scan_lasers = true;
        }
        else if (vdis::to_lowercase(*itor) == "fires")
        {
            scan_fires = true;
        }
        else if (vdis::to_lowercase(*itor) == "collisions")
        {
            scan_collisions = true;
        }
        else if (vdis::to_lowercase(*itor) == "entities")
        {
            scan_entities = true;
        }
        else if (vdis::to_lowercase(*itor) == "objects")
        {
            scan_objects = true;
        }
        else if (vdis::to_lowercase(*itor) == "all")
        {
            scan_associations = true;
            scan_lasers = true;
            scan_fires = true;
            scan_collisions = true;
            scan_entities = true;
            scan_objects = true;
        }
        else
        {
            std::cerr << "vdb: invalid scan token '" << *itor << "'"
                      << std::endl;

            success = false;
        }

        ++itor;
    }

    scanning = true;

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
bool vdb::options::parse_integers(
    const char *name_ptr,
    const char *value_ptr,
    std::set<uint8_t> &set)
{
    std::set<uint32_t>
        temp_set;
    bool
        success = parse_integers_in_range(
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
bool vdb::options::parse_integers_in_range(
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
        std::cout << "DEBUG: parse_integers: " << name_ptr << "='"
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
            std::cout << "DEBUG: parse_integers: value '"
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
                    std::cout << "DEBUG: parse_integers: value "
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

// ----------------------------------------------------------------------------
bool vdb::options::parse_uint64(
    const char *name_ptr,
    const char *value_ptr,
    uint64_t &value)
{
    bool
        success = false;

    if (not value_ptr)
    {
        std::cerr << "vdb: option requires a value: " << name_ptr << std::endl;
    }
    else
    {
        std::string
            string_value = value_ptr;

        success = vdis::to_uint64(string_value, value);

        if (not success)
        {
            std::cerr << "vdb: invalid value for option: "
                      << name_ptr << std::endl;
        }
    }

    return success;
}
