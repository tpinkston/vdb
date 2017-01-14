#ifndef VDB_OPTIONS_H
#define VDB_OPTIONS_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    struct option_t
	{
        option_t(
            const std::string &long_option,
            const char short_option,
            bool value_required
        ) :
            long_option(long_option),
            short_option(short_option),
            value_required(value_required)
        {

        }

    	std::string   long_option;
        char          short_option;
    	bool          value_required;
	};

    // ------------------------------------------------------------------------
    class options_t
    {
      public:

        options_t(const char *command, int argc, char *argv[]);

        void add(option_t option)
        {
            options.push_back(option);
        }

        void set_callback(bool (*function)(const option_t &, const char *))
        {
            callback = function;
        }

        bool parse(void);

      private:

        bool parse_long_option(int current);
        bool parse_short_options(int current, int next, bool advance);

        const std::string
            command;
        const int
            count;
        std::vector<std::string>
            values;
        std::vector<option_t>
            options;
        std::vector<std::string>
            arguments;
        bool
            (*callback)(const option_t &, const char *);
    };

    // ------------------------------------------------------------------------
    class options
    {
      public:

        static std::vector<std::string>
            command_arguments;
        static std::set<std::string>
            include_hostnames;
        static std::set<vdis::id_t>
            include_entity_ids,
            exclude_entity_ids;
        static std::set<uint8_t>
            include_exercises,
            exclude_exercises,
            include_types,
            exclude_types,
            include_families,
            exclude_families;
        static std::set<uint32_t>
            pdu_index_range;
        static std::string
            output_file,
            network_address,
            network_interface;
        static uint64_t
            pdu_interval;
        static bool
            initialized,
            quiet,
            ipv6,
            version,
            help,
            dump,
            extracted,
            extra,
            scanning,
            scan_associations,
            scan_lasers,
            scan_fires,
            scan_collisions,
            scan_entities,
            scan_objects,
            summary_collisions,
            summary_emissions,
            summary_fires,
            summary_lasers,
            summary_objects,
            summary_radios;
        static const bool
            DEBUG;

      private:

        options(void) { }
        ~options(void) { }

        static bool parse_string_set(
            const char *name_ptr,
            const char *value_ptr,
            std::set<std::string> &set
        );

        static bool parse_scans(
            const char *name_ptr,
            const std::set<std::string> &set
        );

        static bool parse_entity_ids(
            const char *name_ptr,
            const char *value_ptr,
            std::set<vdis::id_t> &ids
        );

        static bool parse_entity_id(
            const char *name_ptr,
            const char *value_ptr,
            std::set<vdis::id_t> &ids
        );

        static bool parse_integers(
            const char *name_ptr,
            const char *value_ptr,
            std::set<uint8_t> &set
        );

        static bool parse_integers_in_range(
            const char *name_ptr,
            const char *value_ptr,
            int64_t min,
            int64_t max,
            std::set<uint32_t> &set
        );

        static bool parse_string(
            const char *name_ptr,
            const char *value_ptr,
            std::string &value
        );

        static bool parse_uint64(
            const char *name_ptr,
            const char *value_ptr,
            uint64_t &value
        );
    };
}

#endif
