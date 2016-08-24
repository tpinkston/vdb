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

#ifndef VDB_ENUMERATIONS_H
#define VDB_ENUMERATIONS_H

#include "vdb_common.h"

namespace vdb
{
    class enumerations
    {
      public:

        static void print(std::ostream &stream);

        static void load(void);

        // Returns true if the provided value is a valid value for the
        // provided enumeration.
        //
        static bool valid(const std::string &enumeration, int value);

        // Returns string with format "%s [%d (%h)]" where:
        //   %s - String name of enumeration value
        //   %d - Decimal enumeration value
        //   %h - Hexadecimal enumeration value
        //
        static std::string print(
            const std::string &enumeration,
            int value
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
