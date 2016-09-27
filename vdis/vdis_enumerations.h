#ifndef VDIS_ENUMERATIONS_H
#define VDIS_ENUMERATIONS_H

#include <map>
#include <string>

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
        static bool valid(const std::string &enumeration, int value);

        // Streams with format "%s [%d (%h)]" where:
        //   %s - String name of enumeration value
        //   %d - Decimal enumeration value
        //   %h - Hexadecimal enumeration value
        //
        static std::ostream &print(
            const std::string &enumeration,
            int value,
            std::ostream &out
        );

        // Returns just the string name of the enumeration value.
        //
        static std::string get_name(
            const std::string &enumeration,
            int value
        );

        typedef std::map<int, std::string>
            enum_entries_t;
        typedef std::map<std::string, enum_entries_t>
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
