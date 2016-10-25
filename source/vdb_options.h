#ifndef VDB_OPTIONS_H
#define VDB_OPTIONS_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    typedef enum
    {
        USER_COMMAND_NONE,
        USER_COMMAND_CAPTURE,
        USER_COMMAND_PLAYBACK,
        USER_COMMAND_LIST,
        USER_COMMAND_SUMMARY,
        USER_COMMAND_COMMENT,
        USER_COMMAND_UNCOMMENT,
        USER_COMMAND_ENUMS,
        USER_COMMAND_ENTITIES,
        USER_COMMAND_OBJECTS

    } user_command_e;

    // ------------------------------------------------------------------------
    class options
    {
      public:

        // Initializes with command line arguments
        //
        static bool initialize(int argc, char *argv[]);

        static user_command_e
            command;
        static std::map<user_command_e, std::string>
            command_names;
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
            network_address,
            network_interface;
        static bool
            initialized,
            quiet_mode,
            use_ipv6,
            use_pcap,
            show_version,
            show_help,
            show_examples,
            show_pdu_dump,
            show_pdu_extracted,
            show_pdu_summary,
            summary_extra,
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

        static void configure(void);

        static bool verify_long_argument_value(
            const std::string &name,
            const std::string &value,
            bool required,
            bool &success
        );

        static bool parse_command(const char *current_argument);

        static bool parse_long_option(const char *current_argument);

        static bool parse_short_options(
            const char *current_argument,
            const char *next_argument,
            bool &advance
        );

        static bool parse_string_set(
            const char *name_ptr,
            const char *value_ptr,
            std::set<std::string> &set
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

        static bool parse_integer_set(
            const char *name_ptr,
            const char *value_ptr,
            std::set<uint8_t> &set
        );

        static bool parse_integer_set_in_range(
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
    };
}

#endif
