#ifndef VDIS_ENUMERATIONS_H
#define VDIS_ENUMERATIONS_H

#include "vdis_system.h"

namespace vdis
{
    class enumerations
    {
      public:

        static void load(void);

        static void print(std::ostream &stream);

        // Returns true if the provided value is a valid value for the
        // provided enumeration.
        //
        static bool valid(const string_t &enumeration, int value);

        // Streams with format "%s (%d [%h])" where:
        //   %s - String name of enumeration value
        //   %d - Decimal enumeration value
        //   %h - Hexadecimal enumeration value
        //
        static std::ostream &print(
            const string_t &enumeration,
            int value,
            std::ostream &out
        );

        // Returns just the string name of the enumeration value.
        //
        static string_t get_name(
            const string_t &enumeration,
            int value
        );

        typedef std::map<int, string_t>
            enum_entries_t;
        typedef std::map<string_t, enum_entries_t>
            enum_definitions_t;

      private:

        static void add_all(void);

        static void add(
            const char *enum_ptr,
            int value,
            const char *name_ptr
        );

        static enum_definitions_t
            definitions;
        static bool
            loaded;
    };
}

#endif
