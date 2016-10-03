#ifndef VDIS_ENTITY_TYPES_H
#define VDIS_ENTITY_TYPES_H

#include "vdis_data_types.h"

namespace vdis
{
    struct entity_type_t;

    class entity_types
    {
      public:

        static void load(void);

        static void print(std::ostream &stream);

        static const string_t &get_name(uint64_t value);
        static const string_t &get_description(uint64_t value);

        static bool get_name(uint64_t value, string_t &name);
        static bool get_description(uint64_t value, string_t &description);

        static bool get_parent(
            const entity_type_t &child,
            entity_type_t &parent
        );

        static bool get_valid_parent(
            const entity_type_t &child,
            entity_type_t &parent
        );

        static void get_values(const entity_type_t &type, uint16_t values[7]);

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

        static std::map<uint64_t, string_t>
            names,
            descriptions;
        static bool
            loaded;
    };
}

#endif
