#include "vdb_options.h"

#include "vdis_data_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

namespace
{
    const bool OPTIONS_DEBUG = (getenv("VDB_OPTIONS_DEBUG") != 0);
}

namespace vdb
{
    std::vector<std::string>
        options::command_arguments;
    std::set<std::string>
        options::include_hostnames,
        options::exclude_hostnames;
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
        options::version = false,
        options::help = false,
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
}

// ----------------------------------------------------------------------------
vdb::options_t::options_t(const char *command, int argc, char *argv[]) :
    command(command),
    count(argc),
    callback(0)
{
    for(int i = 0; i < argc; ++i)
    {
        values.push_back(std::string(argv[i]));
    }
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse(void)
{
    bool success = true;

    if (not callback)
    {
        std::cerr << command << ": No option callback!" << std::endl;
        success = false;
    }
    else for(int i = 0; success and (i < count); ++i)
    {
        if (OPTIONS_DEBUG)
        {
            std::cout << "DEBUG: values[" << i << "]='"
                      << values[i] << "'" << std::endl;
        }

        if (vdis::starts_with(values[i], "--"))
        {
            success = parse_long_option(i);
        }
        else if (vdis::starts_with(values[i], "-"))
        {
            int next = ((i + 1) < count) ? (i + 1) : -1;
            bool advance = false;

            success = parse_short_options(i, next, advance);

            if (advance)
            {
                ++i;
            }
        }
        else if (i > 0)
        {
            options::command_arguments.push_back(values[i]);
        }
    }

    if (OPTIONS_DEBUG)
    {
        std::cout << "DEBUG: parse: returning: "
                  << (success ? "true" : "false")
                  << std::endl;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_long_option(int current)
{
    const std::string
        argument = std::string(values[current]).substr(2);
    std::string
        name,
        value;
    std::string::size_type
        index = argument.find_first_of('=');
    bool
        option_found = false,
        success = false;

    if ((index == std::string::npos) or (index < 2))
    {
        name = argument;
    }
    else
    {
        name = argument.substr(0, index);
        value = argument.substr(index + 1);
    }

    if (OPTIONS_DEBUG)
    {
        std::cout << "DEBUG: parse_long_option: --" << name << "=\""
                  << value << "\"" << std::endl;
    }

    // Option search
    //
    for(uint32_t i = 0; (i < options.size()) and not option_found; ++i)
    {
        const option_t *option_ptr = &(options[i]);

        if (option_ptr->long_option == name)
        {
            option_found = true;

            if (OPTIONS_DEBUG)
            {
                std::cout << "DEBUG: parse_long_option: found: "
                          << *option_ptr << std::endl;
            }

            if (option_ptr->needs_value and value.empty())
            {
                std::cerr << command << ": option requires a value: --"
                          << name << std::endl;
            }
            else if (not option_ptr->needs_value and not value.empty())
            {
                std::cerr << command << ": option does not take a value: --"
                          << name << std::endl;
            }
            else
            {
                success = parse_option(*option_ptr, value);
            }
        }
    }

    if (not option_found)
    {
        std::cerr << command << ": invalid option: --" << name << std::endl;
    }

    return success;
}

// TODO: REMOVE
//    if (name == "address")
//    {
//        success = parse_string("--address", value, network_address);
//    }
//    else if (name == "help")
//    {
//        help = true;
//        success = true;
//    }
//    else if (name == "mono")
//    {
//        color::set_enabled(false);
//    }
//    else if (name == "verbose")
//    {
//        if (logger::is_enabled(logger::VERBOSE))
//        {
//            logger::set_enabled(logger::EXTRA_VERBOSE, true);
//        }
//        else
//        {
//            logger::set_enabled(logger::VERBOSE, true);
//        }
//    }
//    else if (name == "warnings")
//    {
//        logger::set_enabled(logger::WARNING, true);
//    }
//    else if (name == "suppress")
//    {
//        logger::set_enabled(logger::ERROR, false);
//    }
//    else if (name == "version")
//    {
//        version = true;
//        success = true;
//    }
//    else if (name == "hostname")
//    {
//        success = parse_string_set("--hostname", value, include_hostnames);
//    }
//    else if (name == "exercise")
//    {
//        success = parse_integers("--exercise", value, include_exercises);
//    }
//    else if (name == "xexercise")
//    {
//        success = parse_integers("--xexercise", value, exclude_exercises);
//    }
//    else if (name == "type")
//    {
//        success = parse_integers("--type", value, include_types);
//    }
//    else if (name == "xtype")
//    {
//        success = parse_integers("--xtype", value, exclude_types);
//    }
//    else if (name == "family")
//    {
//        success = parse_integers("--family", value, include_families);
//    }
//    else if (name == "xfamily")
//    {
//        success = parse_integers("--xfamily", value, exclude_families);
//    }
//    else if (name == "ids")
//    {
//        success = parse_entity_ids("--ids", value, include_entity_ids);
//    }
//    else if (name == "xids")
//    {
//        success = parse_entity_ids("--xids", value, exclude_entity_ids);
//    }
//    else if (name == "ipv6")
//    {
//        ipv6 = true;
//        success = true;
//    }
//    else if (name == "iface")
//    {
//        success = parse_string("--iface", value, network_interface);
//    }
//    else if (name == "range")
//    {
//        success = parse_integers_in_range(
//            "--range",
//            value,
//            0x0,
//            0xFFFFFFFF,
//            pdu_index_range);
//    }
//    else if (name == "pdu")
//    {
//        success = parse_uint64("--pdu", value, pdu_interval);
//    }
//    else if (name == "output")
//    {
//        success = parse_string("--output", value, output_file);
//    }
//    else if (name == "quiet")
//    {
//        quiet = true;
//        success = true;
//    }
//    else if (name == "dump")
//    {
//        dump = true;
//        success = true;
//    }
//    else if (name == "extract")
//    {
//        extracted = true;
//        success = true;
//    }
//    else if (name == "extra")
//    {
//        extra = true;
//        success = true;
//    }
//    else if (name == "scan")
//    {
//        std::set<std::string> scans;
//
//        success =
//            parse_string_set("--hostname", value, scans) and
//            parse_scans("--name", scans);
//    }
//    else if (name == "collisions")
//    {
//        summary_collisions = true;
//        success = true;
//    }
//    else if (name == "emissions")
//    {
//        summary_collisions = true;
//        success = true;
//    }
//    else if (name == "fires")
//    {
//        summary_fires = true;
//        success = true;
//    }
//    else if (name == "lasers")
//    {
//        summary_lasers = true;
//        success = true;
//    }
//    else if (name == "objects")
//    {
//        summary_objects = true;
//        success = true;
//    }
//    else if (name == "radios")
//    {
//        summary_radios = true;
//        success = true;
//    }
//    else if (name == "all")
//    {
//        summary_collisions = true;
//        summary_emissions = true;
//        summary_fires = true;
//        summary_lasers = true;
//        summary_objects = true;
//        summary_radios = true;
//    }

// ----------------------------------------------------------------------------
// Parses command line argument starting with a single dash like so: -cxR
//
bool vdb::options_t::parse_short_options(int current, int next, bool advance)
{
    std::string
        argument = values[current].substr(1);
    bool
        option_found = false,
        value_usable = false,
        success = true;

    if (OPTIONS_DEBUG)
    {
        std::cout << "DEBUG: parse_short_options: parsing '" << values[current]
                  << "', '" << ((next > -1) ? values[next] : "null")
                  << "'" << std::endl;
    }

    // Go through short options character by character.
    //
    for(std::string::size_type i = 0; success and (i < argument.length()); ++i)
    {
        if (OPTIONS_DEBUG)
        {
            std::cout << "DEBUG: parse_short_options: searching for '"
                      << argument[i] << "'" << std::endl;
        }

        option_found = false;

        // Option search
        //
        for(uint32_t j = 0; (j < options.size()) and not option_found; ++j)
        {
            const option_t *option_ptr = &(options[j]);

            if (option_ptr->short_option == argument[i])
            {
                option_found = true;

                if (OPTIONS_DEBUG)
                {
                    std::cout << "DEBUG: parse_short_options: found: "
                              << *option_ptr << std::endl;
                }

                // Value in the next argument is only usable if this option
                // is the last character in the current argument.
                //
                value_usable =
                    option_ptr->needs_value and
                    (next > -1) and
                    (i == (argument.length() - 1));

                if (option_ptr->needs_value and not value_usable)
                {
                    std::cerr << command << ": option requires a value: -"
                              << argument[i] << std::endl;
                }
                else if (value_usable)
                {
                    success = parse_option(*option_ptr, values[next]);
                }
                else
                {
                    success = parse_option(*option_ptr, std::string());
                }
            }
        }

        if (not option_found)
        {
            std::cerr << command << ": invalid option: -"
                      << argument[i] << std::endl;

            success = false;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_option(
    const option_t &option,
    const std::string &value)
{
    bool success = true;

    switch(option.short_option)
    {
        case O_ADDRESS:
            options::network_address = value;
            break;
        case O_COLOR:
            color::set_enabled(false);
            break;
        case O_DUMP:
            options::dump = true;
            break;
        case O_ERRORS:
            logger::set_enabled(logger::ERROR, true);
            break;
        case O_EXERCISE:
            success = parse_integers(value, options::include_exercises);
            break;
        case O_EXTRA:
            options::extra = true;
            break;
        case O_EXTRACT:
            options::extracted = true;
            break;
        case O_FAMILY:
            success = parse_integers(value, options::include_families);
            break;
        case O_HELP:
            options::help = true;
            break;
        case O_HOSTS:
            success = parse_string_set(value, options::include_hostnames);
            break;
        case O_ID:
            success = parse_entity_ids(value, options::include_entity_ids);
            break;
        case O_INTERFACE:
            options::network_interface = value;
            break;
        case O_IPV6:
            options::ipv6 = true;
            break;
        case O_PDU:
            success = parse_uint64(value, options::pdu_interval);
            break;
        case O_TYPE:
            success = parse_integers(value, options::include_types);
            break;
        case O_VERBOSE:
            if (logger::is_enabled(logger::VERBOSE))
            {
                logger::set_enabled(logger::EXTRA_VERBOSE, true);
            }
            else
            {
                logger::set_enabled(logger::VERBOSE, true);
            }
            break;
        case O_VERSION:
            options::version = true;
            break;
        case O_WARNINGS:
            logger::set_enabled(logger::WARNING, true);
            break;
        case O_XEXERCISE:
            success = parse_integers(value, options::exclude_exercises);
            break;
        case O_XFAMILY:
            success = parse_integers(value, options::exclude_families);
            break;
        case O_XHOSTS:
            success = parse_string_set(value, options::exclude_hostnames);
            break;
        case O_XID:
            success = parse_entity_ids(value, options::exclude_entity_ids);
            break;
        case O_XTYPE:
            success = parse_integers(value, options::exclude_types);
            break;
        default:
            success = (*callback)(option, value);
    }

    if (not success)
    {
        // TODO
    }

    return success;
}

// TODO: REMOVE
//            case 'A':
//                summary_collisions = true;
//                summary_emissions = true;
//                summary_fires = true;
//                summary_lasers = true;
//                summary_objects = true;
//                summary_radios = true;
//                break;
//            case 'C':
//                success = set_command(USER_COMMAND_CAPTURE);
//                break;
//            case 'D':
//                success = parse_uint64(
//                    "-D",
//                    next_argument,
//                    pdu_interval);
//                advance = true;
//                break;
//            case 'd':
//                dump = true;
//                break;
//            case 'e':
//                success = parse_integers(
//                    "-e",
//                    next_argument,
//                    include_exercises);
//                advance = true;
//                break;
//            case 'E':
//                success = parse_integers(
//                    "-E",
//                    next_argument,
//                    exclude_exercises);
//                advance = true;
//                break;
//            case 'f':
//                success = parse_integers(
//                    "-f",
//                    next_argument,
//                    include_families);
//                advance = true;
//                break;
//            case 'F':
//                success = parse_integers(
//                    "-F",
//                    next_argument,
//                    exclude_families);
//                advance = true;
//                break;
//            case 'h':
//                help = true;
//                break;
//            case 'H':
//                success = parse_string_set(
//                    "-H",
//                    next_argument,
//                    include_hostnames);
//                advance = true;
//                break;
//            case 'i':
//                success = parse_entity_ids(
//                    "-i",
//                    next_argument,
//                    include_entity_ids);
//                advance = true;
//                break;
//            case 'I':
//                success = parse_entity_ids("-I",
//                    next_argument,
//                    include_entity_ids);
//                advance = true;
//                break;
//            case 'L':
//                success = set_command(USER_COMMAND_LIST);
//                break;
//            case 'm':
//                color::set_enabled(false);
//                break;
//            case 'M':
//                success = set_command(USER_COMMAND_ENUMS);
//                break;
//            case 'n':
//                success = parse_string("-n", next_argument, network_interface);
//                advance = true;
//                break;
//            case 'N':
//                success =
//                    parse_string_set("-n", next_argument, scans) and
//                    parse_scans("-n", scans);
//                advance = true;
//                break;
//            case 'O':
//                success = parse_string("-O", next_argument, output_file);
//                advance = true;
//                break;
//            case 'P':
//                success = set_command(USER_COMMAND_PLAYBACK);
//                break;
//            case 'q':
//                quiet = true;
//                break;
//            case 'r':
//                success = parse_integers_in_range(
//                    "-r",
//                    next_argument,
//                    0x0,
//                    0xFFFFFFFF,
//                    pdu_index_range);
//                advance = true;
//                break;
//            case 'R':
//                logger::set_enabled(logger::ERROR, false);
//                break;
//            case 'S':
//                success = set_command(USER_COMMAND_SUMMARY);
//                break;
//            case 't':
//                success = parse_integers("-t", next_argument, include_types);
//                advance = true;
//                break;
//            case 'T':
//                success = parse_integers("-T", next_argument, exclude_types);
//                advance = true;
//                break;
//            case 'v':
//                if (logger::is_enabled(logger::VERBOSE))
//                {
//                    logger::set_enabled(logger::EXTRA_VERBOSE, true);
//                }
//                else
//                {
//                    logger::set_enabled(logger::VERBOSE, true);
//                }
//                break;
//            case 'U':
//                success = set_command(USER_COMMAND_UNCOMMENT);
//                break;
//            case 'V':
//                version = true;
//                break;
//            case 'w':
//                logger::set_enabled(logger::WARNING, true);
//                break;
//            case 'W':
//                success = set_command(USER_COMMAND_COMMENT);
//                break;
//            case 'x':
//                extracted = true;
//                break;
//            case 'X':
//                extra = true;
//                break;
//            default:
//                std::cerr << "vdb: invalid option: -" << argument[i]
//                          << std::endl;
//                success = false;
//                break;

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_string_set(
    const std::string &input,
    std::set<std::string> &output)
{
    std::vector<std::string>
        values;
    bool
        success = false;

    vdis::tokenize_csv(input, values);

    if (OPTIONS_DEBUG)
    {
        std::cout << "DEBUG: parse_string_set: '" << input << "'" << std::endl;
    }

    for(uint32_t i = 0; i < values.size(); ++i)
    {
        std::string value = vdis::trim(values[i]);

        if (not value.empty())
        {
            output.insert(value);
            success = true;
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_scans(
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
            options::scan_associations = true;
        }
        else if (vdis::to_lowercase(*itor) == "lasers")
        {
            options::scan_lasers = true;
        }
        else if (vdis::to_lowercase(*itor) == "fires")
        {
            options::scan_fires = true;
        }
        else if (vdis::to_lowercase(*itor) == "collisions")
        {
            options::scan_collisions = true;
        }
        else if (vdis::to_lowercase(*itor) == "entities")
        {
            options::scan_entities = true;
        }
        else if (vdis::to_lowercase(*itor) == "objects")
        {
            options::scan_objects = true;
        }
        else if (vdis::to_lowercase(*itor) == "all")
        {
            options::scan_associations = true;
            options::scan_lasers = true;
            options::scan_fires = true;
            options::scan_collisions = true;
            options::scan_entities = true;
            options::scan_objects = true;
        }
        else
        {
            std::cerr << "vdb: invalid scan token '" << *itor << "'"
                      << std::endl;

            success = false;
        }

        ++itor;
    }

    options::scanning = true;

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_entity_ids(
    const std::string &input,
    std::set<vdis::id_t> &output)
{
    std::vector<std::string>
        values;
    bool
        success = true;

    vdis::tokenize_csv(input, values);

    if (OPTIONS_DEBUG)
    {
        std::cout << "DEBUG: parse_entity_ids: '" << input << "'" << std::endl;
    }

    for(uint32_t i = 0; success and (i < values.size()); ++i)
    {
        success = parse_entity_id(values[i], output);
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_entity_id(
    const std::string &input,
    std::set<vdis::id_t> &output)
{
    std::vector<std::string>
        tokens;
    std::string
        swapped = input;
    uint32_t
        id[3];
    bool
        success = false;

    // Tokenization uses space character as delimiter, swap '.' with ' '
    //
    for(unsigned i = 0; i < swapped.length(); ++i)
    {
        swapped[i] = (swapped[i] == '.') ? ' ' : swapped[i];
    }

    if (vdis::tokenize(swapped, tokens) != 3)
    {
        std::cerr << command << ": expected entity ID fomat is '1.2.3' not '"
                  << input << "'" << std::endl;
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
                std::cerr << command << ": value out of range in '" << input
                          << "'" << std::endl;
            }
        }
        else
        {
            std::cerr << command << ": invalid value in '" << input
                      << "'" << std::endl;
        }
    }

    if (success)
    {
        vdis::id_t entity_id = {
            (uint16_t)id[0],
            (uint16_t)id[1],
            (uint16_t)id[2] };

        output.insert(entity_id);

        if (OPTIONS_DEBUG)
        {
            std::cout << "DEBUG: parse_entity_id: '"
                      << input << "' -> " << entity_id << std::endl;
        }
    }
    else
    {
        std::cerr << command << ": invalid entity ID: " << input << std::endl;
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_integers(
    const std::string &input,
    std::set<uint8_t> &output)
{
    std::set<uint32_t>
        temp_set;
    bool
        success = parse_integers_in_range(
            input,
            0,
            255,
            temp_set);

    if (success)
    {
        std::set<uint32_t>::const_iterator
            itor = temp_set.begin();

        for(itor = temp_set.begin(); itor != temp_set.end(); ++itor)
        {
            output.insert((uint8_t)*itor);
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_integers_in_range(
    const std::string &input,
    int64_t min,
    int64_t max,
    std::set<uint32_t> &output)
{
    std::vector<std::string>
        values,
        subvalues;
    bool
        success = true;

    if (OPTIONS_DEBUG)
    {
        std::cout << "DEBUG: parse_integers_in_range (" << min << "-" << max
                  << "): '" << input << "'" << std::endl;
    }

    vdis::tokenize_csv(input, values);

    for(uint32_t i = 0; success and (i < values.size()); ++i)
    {
        if (OPTIONS_DEBUG)
        {
            std::cout << "DEBUG: parse_integers_in_range: value '"
                      << values[i] << "'" << std::endl;
        }

        // Is this token a range "N-M" or a single value N
        //
        if (values[i].find('-') == std::string::npos)
        {
            int64_t value = 0;

            if (vdis::to_int64(values[i], value))
            {
                if (OPTIONS_DEBUG)
                {
                    std::cout << "DEBUG: parse_integers: value "
                              << value << std::endl;
                }

                if ((value < min) or (value > max))
                {
                    std::cerr << command << ": value out of range: " << value
                              << " (" << min << "-" << max << ")" << std::endl;
                    success = false;
                }
                else
                {
                    output.insert((uint32_t)value);
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
                        std::cerr << command << ": value out of range: "
                                  << value0 << " (" << min << "-" << max << ")"
                                  << std::endl;
                    }
                    else if ((value1 < min) or (value1 > max))
                    {
                        std::cerr << command << ": value out of range: "
                                  << value1 << " (" << min << "-" << max << ")"
                                  << std::endl;
                    }
                    else if (value0 >= value1)
                    {
                        std::cerr << command << ": invalid range: " << value0
                                  << "-" << value1 << std::endl;
                    }
                    else
                    {
                        success = true;

                        for(int64_t j = value0; j <= value1; ++j)
                        {
                            output.insert(j);
                        }
                    }
                }
            }
        }
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_uint64(
    const std::string &input,
    uint64_t &output)
{
    return vdis::to_uint64(input, output);
}
