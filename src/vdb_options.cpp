#include "vdb_command.h"
#include "vdb_common.h"
#include "vdb_filter.h"
#include "vdb_options.h"

#include "vdis_data_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_network.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

namespace vdb
{
    std::vector<std::string>
        options::command_arguments;
    bool
        options::quiet = false,
        options::version = false,
        options::help = false,
        options::dump = false,
        options::extracted = false,
        options::extra = false;
}

// ----------------------------------------------------------------------------
vdb::options_t::options_t(
    const std::string &command,
    const std::vector<std::string> &args
) :
    command(command),
    count(args.size()),
    arguments(args),
    command_ptr(0)
{

}

// ----------------------------------------------------------------------------
void vdb::options_t::add(option_t option)
{
    bool option_found = false;

    for(uint32_t i = 0; (i < options.size()) and not option_found; ++i)
    {
        if ((options[i].long_option == option.long_option) or
            (options[i].short_option == option.short_option))
        {
            option_found = true;
        }
    }

    if (option_found)
    {
        LOG_FATAL(
            "%s: duplicate option added: %s",
            command.c_str(),
            vdis::to_string(option).c_str());
    }
    else
    {
        options.push_back(option);
    }
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse(void)
{
    bool success = true;

    if (not command_ptr)
    {
        LOG_FATAL("%s: Missing command pointer!", command.c_str());
        success = false;
    }
    else for(int i = 0; success and (i < count); ++i)
    {
        if (debug_enabled(DEBUG_OPTIONS))
        {
            CONSOLE_OUT(
                "DEBUG: parse: arguments[%d]='%s'",
                i,
                arguments[i].c_str());
        }

        if (vdis::starts_with(arguments[i], "--"))
        {
            success = parse_long_option(i);
        }
        else if (vdis::starts_with(arguments[i], "-"))
        {
            int
                next = ((i + 1) < count) ? (i + 1) : -1;
            bool
                advance = false;

            success = parse_short_options(i, next, advance);

            if (debug_enabled(DEBUG_OPTIONS))
            {
                CONSOLE_OUT(
                    "DEBUG: parse: advance: %s",
                    (advance ? "true" : "false"));
            }

            if (advance)
            {
                ++i;
            }
        }
        else
        {
            options::command_arguments.push_back(arguments[i]);
        }
    }

    if (debug_enabled(DEBUG_OPTIONS))
    {
        CONSOLE_OUT("DEBUG: parse: returning: %s", success ? "true" : "false");
    }

    return success;
}

// ----------------------------------------------------------------------------
bool vdb::options_t::parse_long_option(int current)
{
    const std::string
        argument = std::string(arguments[current]).substr(2);
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

    if (debug_enabled(DEBUG_OPTIONS))
    {
        CONSOLE_OUT(
            "DEBUG: parse_long_option: --%s=%s",
            name.c_str(),
            value.c_str());
    }

    // Option search
    //
    for(uint32_t i = 0; (i < options.size()) and not option_found; ++i)
    {
        const option_t *option_ptr = &(options[i]);

        if (option_ptr->long_option == name)
        {
            option_found = true;

            if (debug_enabled(DEBUG_OPTIONS))
            {
                CONSOLE_OUT(
                    "DEBUG: parse_long_option: found: %s",
                    vdis::to_string(*option_ptr).c_str());
            }

            if (option_ptr->needs_value and value.empty())
            {
                LOG_FATAL(
                    "%s: option requires a value: --%s",
                    command.c_str(),
                    name.c_str());
            }
            else if (not option_ptr->needs_value and not value.empty())
            {
                LOG_FATAL(
                    "%s: option does not take a value: --%s",
                    command.c_str(),
                    name.c_str());
            }
            else
            {
                success = parse_option(*option_ptr, value);
            }
        }
    }

    if (not option_found)
    {
        LOG_FATAL(
            "%s: invalid option: --%s",
            command.c_str(),
            name.c_str());
    }

    return success;
}

// ----------------------------------------------------------------------------
// Parses command line argument starting with a single dash like so: -cxR
//
bool vdb::options_t::parse_short_options(int current, int next, bool &advance)
{
    std::string
        argument = arguments[current].substr(1);
    bool
        option_found = false,
        value_usable = false,
        success = true;

    if (debug_enabled(DEBUG_OPTIONS))
    {
        CONSOLE_OUT(
            "DEBUG: parse_short_options: parsing '%s', '%s'",
            arguments[current].c_str(),
            ((next > -1) ? arguments[next].c_str() : "null"));
    }

    // Go through short options character by character.
    //
    for(std::string::size_type i = 0; success and (i < argument.length()); ++i)
    {
        if (debug_enabled(DEBUG_OPTIONS))
        {
            CONSOLE_OUT(
                "DEBUG: parse_short_options: searching for '%c'",
                argument[i]);
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

                if (debug_enabled(DEBUG_OPTIONS))
                {
                    CONSOLE_OUT(
                        "DEBUG: parse_short_options: found: %s",
                        vdis::to_string(*option_ptr).c_str());
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
                    LOG_FATAL(
                        "%s: option requires a value: -%c",
                        command.c_str(),
                        argument[i]);
                    success = false;
                }
                else if (value_usable)
                {
                    success = parse_option(*option_ptr, arguments[next]);
                    advance = true;
                }
                else
                {
                    success = parse_option(*option_ptr, std::string());
                }
            }
        }

        if (not option_found)
        {
            LOG_FATAL(
                "%s: invalid option: -%c",
                command.c_str(),
                argument[i]);
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

    if (command_ptr->option_callback(option, value, success))
    {
        if (debug_enabled(DEBUG_OPTIONS))
        {
            CONSOLE_OUT(
                "DEBUG: parse_option: callback returned true, success: %s",
                (success ? "true" : "false"));
        }

        return success;
    }

    if (success)
    {
        switch(option.short_option)
        {
            case O_ADDRESS:
                vdis::network_options::address = value;
                break;
            case O_DUMP:
                options::dump = true;
                break;
            case O_ERRORS:
                logger::set_enabled(logger::ERROR, false);
                break;
            case O_EXERCISE:
                success = parse_integers(value, filter::include_exercises);
                break;
            case O_EXTRA:
                options::extra = true;
                break;
            case O_EXTRACT:
                options::extracted = true;
                break;
            case O_FAMILY:
                success = parse_integers(value, filter::include_families);
                break;
            case O_HELP:
                options::help = true;
                break;
            case O_HOSTS:
                success = parse_string_set(value, filter::include_hostnames);
                break;
            case O_ID:
                success = parse_entity_ids(value, filter::include_entity_ids);
                break;
            case O_INTERFACE:
                vdis::network_options::interface = value;
                break;
            case O_IPV6:
                vdis::network_options::ipv6 = true;
                break;
                break;
            case O_MONO:
                color::set_enabled(false);
                break;
            case O_RANGE:
                success = parse_integers_in_range(
                    value,
                    0x0,
                    0xFFFFFFFF,
                    filter::pdu_index_range);
                break;
            case O_TYPE:
                success = parse_integers(value, filter::include_types);
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
                success = parse_integers(value, filter::exclude_exercises);
                break;
            case O_XFAMILY:
                success = parse_integers(value, filter::exclude_families);
                break;
            case O_XHOSTS:
                success = parse_string_set(value, filter::exclude_hostnames);
                break;
            case O_XID:
                success = parse_entity_ids(value, filter::exclude_entity_ids);
                break;
            case O_XTYPE:
                success = parse_integers(value, filter::exclude_types);
                break;
            default:
                LOG_FATAL(
                    "%s: unexpected option: -%c/--%s",
                    command.c_str(),
                    option.short_option,
                    option.long_option.c_str());
        }
    }

    return success;
}

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

    if (debug_enabled(DEBUG_OPTIONS))
    {
        CONSOLE_OUT("DEBUG: parse_string_set: '%s'", input.c_str());
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
bool vdb::options_t::parse_entity_ids(
    const std::string &input,
    std::set<vdis::id_t> &output)
{
    std::vector<std::string>
        values;
    bool
        success = true;

    vdis::tokenize_csv(input, values);

    if (debug_enabled(DEBUG_OPTIONS))
    {
        CONSOLE_OUT("DEBUG: parse_entity_ids: '%s'", input.c_str());
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
        LOG_FATAL(
            "%s: expected entity ID fomat is '1.2.3' not '%s'",
            command.c_str(),
            input.c_str());
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
                LOG_FATAL(
                    "%s: value out of range in '%s'",
                    command.c_str(),
                    input.c_str());
            }
        }
        else
        {
            LOG_FATAL(
                "%s: invalid value in '%s'",
                command.c_str(),
                input.c_str());
        }
    }

    if (success)
    {
        vdis::id_t entity_id = {
            (uint16_t)id[0],
            (uint16_t)id[1],
            (uint16_t)id[2] };

        output.insert(entity_id);

        if (debug_enabled(DEBUG_OPTIONS))
        {
            CONSOLE_OUT(
                "DEBUG: parse_entity_id: '%s' -> %s",
                input.c_str(),
                vdis::to_string(entity_id).c_str());
        }
    }
    else
    {
        LOG_FATAL(
            "%s: invalid entity ID '%s'",
            command.c_str(),
            input.c_str());
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

    if (debug_enabled(DEBUG_OPTIONS))
    {
        CONSOLE_OUT(
            "DEBUG: parse_integers_in_range (%d-%d): '%s'",
            min,
            max,
            input.c_str());
    }

    vdis::tokenize_csv(input, values);

    for(uint32_t i = 0; success and (i < values.size()); ++i)
    {
        if (debug_enabled(DEBUG_OPTIONS))
        {
            CONSOLE_OUT(
                "DEBUG: parse_integers_in_range: value '%s'",
                values[i].c_str());
        }

        // Is this token a range "N-M" or a single value N
        //
        if (values[i].find('-') == std::string::npos)
        {
            int64_t value = 0;

            if (not vdis::to_int64(values[i], value))
            {
                LOG_FATAL(
                    "%s: not a valid number: %s",
                    command.c_str(),
                    values[i].c_str());
                success = false;
            }
            else
            {
                if (debug_enabled(DEBUG_OPTIONS))
                {
                    CONSOLE_OUT("DEBUG: parse_integers: value %d", value);
                }

                if ((value >= min) and (value <= max))
                {
                    output.insert((uint32_t)value);
                }
                else
                {
                    LOG_FATAL(
                        "%s: value out of range: %s (%d-%d)",
                        command.c_str(),
                        values[i].c_str(),
                        min,
                        max);
                    success = false;
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

            if (not success)
            {
                LOG_FATAL(
                    "%s: not a valid format for range: %s",
                    command.c_str(),
                    values[i].c_str());
                success = false;
            }
            else
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
                        LOG_FATAL(
                            "%s: value out of range: %d (%d-%d)",
                            command.c_str(),
                            value0,
                            min,
                            max);
                    }
                    else if ((value1 < min) or (value1 > max))
                    {
                        LOG_FATAL(
                            "%s: value out of range: %d (%d-%d)",
                            command.c_str(),
                            value1,
                            min,
                            max);
                    }
                    else if (value0 >= value1)
                    {
                        LOG_FATAL(
                            "%s: invalid range: %d-%d",
                            command.c_str(),
                            value0,
                            value1);
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
