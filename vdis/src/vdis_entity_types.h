#ifndef VDIS_ENTITY_TYPES_H
#define VDIS_ENTITY_TYPES_H

#include "vdis_data_types.h"

namespace vdis
{
    struct entity_type_t;

    class entity_types
    {
      public:

        typedef std::map<uint64_t, std::string>
            mapping_t;

        static void load(void);

        static void print(std::ostream &stream);

        static const std::string &get_name(uint64_t value);
        static const std::string &get_description(uint64_t value);

        static bool get_name(uint64_t value, std::string &name);
        static bool get_description(uint64_t value, std::string &description);

        static bool get_parent(
            const entity_type_t &child,
            entity_type_t &parent
        );

        static bool get_valid_parent(
            const entity_type_t &child,
            entity_type_t &parent
        );

        static void get_values(const entity_type_t &type, uint16_t values[7]);

        inline static mapping_t::const_iterator begin(void)
        {
            return names.begin();
        }

        inline static mapping_t::const_iterator end(void)
        {
            return names.end();
        }

      private:

        static void add_all(void);

        static void add(
            const int kind,
            const int domain,
            const int country,
            const int category,
            const int subcategory,
            const int specific,
            const int extra,
            const char *name_ptr,
            const char *description_ptr
        );

        static mapping_t
            names,
            descriptions;
        static bool
            loaded;
    };
}

#endif
