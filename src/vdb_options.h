#ifndef VDB_OPTIONS_H
#define VDB_OPTIONS_H

#include "vdb_system.h"

#include "vdis_data_types.h"

// Options with negative value do not have a usable short value
// on the command line:
//
#define O_ADDRESS       'a'
#define O_DUMP          'd'
#define O_ERRORS        'E'
#define O_EXERCISE      (char)-10
#define O_EXTRA         'e'
#define O_EXTRACT       'x'
#define O_FAMILY        'f'
#define O_HELP          'h'
#define O_HOSTS         (char)-15
#define O_ID            'i'
#define O_MONO          'm'
#define O_INTERFACE     'N'
#define O_IPV6          '6'
#define O_RANGE         'r'
#define O_TYPE          't'
#define O_VERBOSE       'v'
#define O_VERSION       'V'
#define O_WARNINGS      'W'
#define O_XEXERCISE     (char)-20
#define O_XFAMILY       'F'
#define O_XHOSTS        (char)-30
#define O_XID           'I'
#define O_XTYPE         'T'

#define OPTION_ADDRESS      vdb::option_t("address",     O_ADDRESS,     true)
#define OPTION_DUMP         vdb::option_t("dump",        O_DUMP,        false)
#define OPTION_ERRORS       vdb::option_t("errors",      O_ERRORS,      false)
#define OPTION_EXERCISE     vdb::option_t("exercise",    O_EXERCISE,    true)
#define OPTION_EXTRA        vdb::option_t("extra",       O_EXTRA,       false)
#define OPTION_EXTRACT      vdb::option_t("extract",     O_EXTRACT,     false)
#define OPTION_FAMILY       vdb::option_t("family",      O_FAMILY,      true)
#define OPTION_HELP         vdb::option_t("help",        O_HELP,        false)
#define OPTION_HOSTNAME     vdb::option_t("hostname",    O_HOSTS,       true)
#define OPTION_ID           vdb::option_t("id",          O_ID,          true)
#define OPTION_INTERFACE    vdb::option_t("interface",   O_INTERFACE,   true)
#define OPTION_IPV6         vdb::option_t("ipv6",        O_IPV6,        false)
#define OPTION_MONO         vdb::option_t("mono",        O_MONO,        false)
#define OPTION_RANGE        vdb::option_t("range",       O_RANGE,       true)
#define OPTION_TYPE         vdb::option_t("type",        O_TYPE,        true)
#define OPTION_VERBOSE      vdb::option_t("verbose",     O_VERBOSE,     false)
#define OPTION_VERSION      vdb::option_t("version",     O_VERSION,     false)
#define OPTION_WARNINGS     vdb::option_t("warnings",    O_WARNINGS,    false)
#define OPTION_XEXERCISE    vdb::option_t("xexercise",   O_XEXERCISE,   true)
#define OPTION_XFAMILY      vdb::option_t("xfamily",     O_XFAMILY,     true)
#define OPTION_XHOSTNAME    vdb::option_t("xhostname",   O_XHOSTS,      true)
#define OPTION_XID          vdb::option_t("xid",         O_XID,         true)
#define OPTION_XTYPE        vdb::option_t("xtype",       O_XTYPE,       true)

namespace vdb
{
    class command_t;

    // ------------------------------------------------------------------------
    struct option_t
	{
        option_t(
            const std::string &long_option,
            const char short_option,
            bool needs_value
        ) :
            long_option(long_option),
            short_option(short_option),
            needs_value(needs_value)
        {

        }

    	std::string
    	    long_option;
        char
            short_option;
    	bool
    	    needs_value;
	};

    // ------------------------------------------------------------------------
    class options_t
    {
      public:

        options_t(
            const std::string &command,
            const std::vector<std::string> &args
        );

        // Adds usable option for provided command.
        //
        void add(option_t option);

        // Parse options, return true if all options provided in 'argv'
        // are valid.
        //
        bool parse(void);

        bool parse_string_set(
            const std::string &input,
            std::set<std::string> &output
        );

        bool parse_entity_ids(
            const std::string &input,
            std::set<vdis::id_t> &output
        );

        bool parse_entity_id(
            const std::string &input,
            std::set<vdis::id_t> &output
        );

        bool parse_integers(
            const std::string &input,
            std::set<uint8_t> &output
        );

        bool parse_integers_in_range(
            const std::string &input,
            int64_t min,
            int64_t max,
            std::set<uint32_t> &set
        );

        inline void set_command(command_t *c_ptr)
        {
            command_ptr = c_ptr;
        }

      private:

        bool parse_long_option(int current);
        bool parse_short_options(int current, int next, bool &advance);
        bool parse_option(const option_t &option, const std::string &value);

        const std::string
            command;
        const int
            count;
        std::vector<std::string>
            arguments;
        std::vector<option_t>
            options;
        command_t
            *command_ptr;
    };

    // ------------------------------------------------------------------------
    // TODO: REMOVE
    class options
    {
      public:

        static std::vector<std::string>
            command_arguments;
        static bool
            all,
            quiet,
            version,
            help,
            dump,
            extracted,
            extra;
    };
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(std::ostream &out, const vdb::option_t &option)
{
    if (option.short_option > 0)
    {
        out << "-" << option.short_option << ",";
    }

    out << option.long_option << ","
        << (option.needs_value ? "true" : "false");

    return out;
}

#endif
