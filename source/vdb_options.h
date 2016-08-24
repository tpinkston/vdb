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

#ifndef VDB_OPTIONS_H
#define VDB_OPTIONS_H

#include "vdb_common.h"
#include "vdb_enums.h"

#define OPT_STRING_MASK         0x0100
#define OPT_INTEGER_MASK        0x0200
#define OPT_INTEGER_SET_MASK    0x0400
#define OPT_FLAG_MASK           0x0800
#define OPT_COMPLEX_MASK        0x1000

namespace vdb
{
    class id_t;
    class pdu_t;

    // ------------------------------------------------------------------------
    typedef enum
    {
        USER_COMMAND_NONE,
        USER_COMMAND_CAPTURE,
        USER_COMMAND_PLAYBACK,
        USER_COMMAND_LIST,
        USER_COMMAND_QUERY,
        USER_COMMAND_COMMENT,
        USER_COMMAND_UNCOMMENT,
        USER_COMMAND_ENUMS,
        USER_COMMAND_ENTITIES,
        USER_COMMAND_OBJECTS,
        USER_COMMAND_TEST

    } user_command_e;

    // ------------------------------------------------------------------------
    typedef enum
    {
        // Options with string parameters
        OPT_NET_ADDRESS         = OPT_STRING_MASK | 1,
        OPT_NET_INTERFACE       = OPT_STRING_MASK | 2,

        // Options with integer parameters
        OPT_EXERCISE            = OPT_INTEGER_MASK | 1,

        // Options with integer set parameters
        OPT_PDU_TYPE            = OPT_INTEGER_SET_MASK | 1,
        OPT_PDU_FAMILY          = OPT_INTEGER_SET_MASK | 2,
        OPT_PDU_RANGE           = OPT_INTEGER_SET_MASK | 3,

        // Options (flags) with no parameters
        OPT_IPV6                = OPT_FLAG_MASK | 1,
        OPT_COLOR_OFF           = OPT_FLAG_MASK | 2,
        OPT_EXTRACT             = OPT_FLAG_MASK | 3,
        OPT_DUMP                = OPT_FLAG_MASK | 4,
        OPT_SUMMARIZE           = OPT_FLAG_MASK | 5,
        OPT_QUIET               = OPT_FLAG_MASK | 6,
        OPT_VERBOSE             = OPT_FLAG_MASK | 7,
        OPT_EXTRA_VERBOSE       = OPT_FLAG_MASK | 8,
        OPT_WARNINGS            = OPT_FLAG_MASK | 9,
        OPT_ERRORS_OFF          = OPT_FLAG_MASK | 0,
        OPT_HELP                = OPT_FLAG_MASK | 11,
        OPT_VERSION             = OPT_FLAG_MASK | 12,
        OPT_COLLISIONS          = OPT_FLAG_MASK | 13,
        OPT_EMISSIONS           = OPT_FLAG_MASK | 14,
        OPT_FIRES               = OPT_FLAG_MASK | 15,
        OPT_LASERS              = OPT_FLAG_MASK | 16,
        OPT_RADIOS              = OPT_FLAG_MASK | 17,
        OPT_OBJECTS             = OPT_FLAG_MASK | 18,
        OPT_ALL                 = OPT_FLAG_MASK | 19,

        // Options with complex parameters
        OPT_ENTITY_IDS          = OPT_COMPLEX_MASK | 1

    } option_e;

    typedef std::set<option_e>
        option_set_t;

    // ------------------------------------------------------------------------
    typedef enum
    {
        OPT_ARG_NONE,
        OPT_ARG_OPTIONAL,
        OPT_ARG_REQUIRED

    } option_argument_e;

    // ------------------------------------------------------------------------
    class option_definition_t
    {
      private:

        friend class options;

        bool argument_none(void) const { return argument == OPT_ARG_NONE; }
        bool argument_optional(void) const { return argument == OPT_ARG_OPTIONAL; }
        bool argument_required(void) const { return argument == OPT_ARG_REQUIRED; }

        const char
            *long_value;
        char
            short_value;
        option_e
            option;
        option_argument_e
            argument;
    };

    // ------------------------------------------------------------------------
    class options
    {
      public:

        // Initializes with command line arguments
        //
        static bool initialize(int argc, char *argv[]);

        // Returns true if option provided or set
        //
        static bool provided(option_e o);

        // Returns true if boolean option provided, false if not
        //
        static bool flag(option_e o);

        // Returns string option value or null if not provided
        //
        static const std::string *string(option_e o);

        // Returns integer option value or null if not provided
        //
        static const uint32_t *integer(option_e o);

        // Returns integer set option value or null if not provided
        //
        static const uint32_set_t *integer_set(option_e o);

        // Returns the terminal command
        //
        static const std::string &get_terminal_command(void);

        // Returns the specified user command
        //
        static user_command_e get_command(void);

        // Returns true if there's no entity ID filters or if PDU matches one
        //
        static bool entity_id_match(const pdu_t &pdu);

        // Returns true is value is in specified integer set, false if set is
        // empty (use provided(option) first)
        //
        static bool integer_in_set(option_e o, uint32_t value);

        // Returns true if PDU index is in specified range or if no PDU range
        // was specified
        //
        static bool pdu_index_in_range(uint32_t index, bool &past_end);

        // Returns all arguments for the specified command
        //
        static const std::vector<std::string> get_command_arguments(void);

        // Returns indexed argument for the specified command, null if N/A
        //
        static const std::string *get_command_argument(uint32_t index);

      private:

        options(void) { }
        ~options(void) { }

        static void configure(void);

        static bool verify(void);

        static void print_option_error(option_e option);

        static void parse_terminal_command(const char *current_argument);

        static bool parse_command(const char *current_argument);

        static bool parse_long_option(const char *current_argument);

        static bool parse_short_options(
            const char *current_argument,
            const char *next_argument,
            bool &advance
        );

        static bool process_option(
            option_e option,
            const std::string &name,
            const std::string &value
        );

        static bool process_flag(option_e option, const std::string &name);

        static bool parse_entity_ids(const std::string &value);

        static bool parse_entity_id(std::string &value);

        static bool parse_integer_set(
            option_e,
            const std::string &name,
            const std::string &value
        );

        static void add(
            const char *long_value,
            char short_value,
            option_e option,
            option_argument_e argument
        );

        static const option_definition_t *get_option(option_e);
        static const option_definition_t *get_short_option(char);
        static const option_definition_t *get_long_option(const std::string &);

        // Option definitions
        //
        static std::vector<option_definition_t>
            option_definitions;

        // Names for command enumerations
        //
        static std::map<user_command_e, std::string>
            command_names;

        // Array of valid options for each command
        //
        static std::map<user_command_e, option_set_t>
            command_options;

        // All command-line arguments after the command
        //
        static std::vector<std::string>
            command_arguments;

        // Values for string options
        //
        static std::map<option_e, std::string>
            string_options;

        // Values for integer options
        //
        static std::map<option_e, uint32_t>
            integer_options;

        // Values for integer set options
        //
        static std::map<option_e, uint32_set_t>
            integer_set_options;

        // Values for flags, options with no arguments
        //
        static std::set<option_e>
            flags;

        // ID specified for 'OPT_ENTITY_ID' option
        //
        static std::vector<id_t>
            entity_ids;

        // The program name as executed from command line (argv[0])
        static std::string
            terminal_command;

        // The command to execute
        //
        static user_command_e
            command;

        // True if initialized
        //
        static bool
            initialized;

        static const bool
            DEBUG;
    };
}

// ----------------------------------------------------------------------------
inline bool vdb::options::provided(option_e o)
{
    if (o == OPT_ENTITY_IDS)
    {
        return not entity_ids.empty();
    }
    else if (flag(o) or string(o) || integer(o) || integer_set(o))
    {
        return true;
    }

    return false;
}

// ----------------------------------------------------------------------------
inline bool vdb::options::flag(option_e o)
{
    if ((o & OPT_FLAG_MASK) == OPT_FLAG_MASK)
    {
        return (flags.find(o) != flags.end());
    }

    return false;
}

// ----------------------------------------------------------------------------
inline const std::string &vdb::options::get_terminal_command(void)
{
    return terminal_command;
}

// ----------------------------------------------------------------------------
inline vdb::user_command_e vdb::options::get_command(void)
{
    return command;
}

// ----------------------------------------------------------------------------
inline bool vdb::options::integer_in_set(option_e o, uint32_t value)
{
    std::map<option_e, uint32_set_t>::const_iterator
        itor = integer_set_options.find(o);

    if (itor != integer_set_options.end())
    {
        return (itor->second.find(value) != itor->second.end());
    }

    return false;
}

// ----------------------------------------------------------------------------
inline bool vdb::options::pdu_index_in_range(uint32_t index, bool &past_end)
{
    std::map<option_e, uint32_set_t>::const_iterator
        itor = integer_set_options.find(OPT_PDU_RANGE);

    if ((itor == integer_set_options.end()) or itor->second.empty())
    {
        // No PDU range was specified...
        //
        return true;
    }
    else
    {
        // Set 'past_end' to true if the provided index is beyond the
        // last index in the set.
        //
        past_end = (index > *itor->second.rbegin());

        return (itor->second.find(index) != itor->second.end());
    }
}

// ----------------------------------------------------------------------------
inline const std::vector<std::string> vdb::options::get_command_arguments(void)
{
    return command_arguments;
}

// ----------------------------------------------------------------------------
inline const std::string *vdb::options::get_command_argument(uint32_t index)
{
    if (index < command_arguments.size())
    {
        return &command_arguments[index];
    }

    return 0;
}

// ----------------------------------------------------------------------------
inline void vdb::options::add(
    const char *long_value,
    char short_value,
    option_e option,
    option_argument_e argument)
{
    option_definition_t
        definition;

    definition.long_value = long_value;
    definition.short_value = short_value;
    definition.option = option;
    definition.argument = argument;

    option_definitions.push_back(definition);
}

#endif
