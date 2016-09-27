#ifndef VDIS_OBJECT_TYPES_H
#define VDIS_OBJECT_TYPES_H

#include <cstdint>
#include <iostream>
#include <map>
#include <string>

#include "vdis_enums.h"

namespace vdis
{
    struct object_type_t;

    class object_types
    {
      public:

        static void load(void);

        static void print(std::ostream &stream);

        static void convert(const object_type_t &type, uint32_t &value);
        static void convert(const uint32_t value, object_type_t &type);

        static const std::string &get_description(uint32_t value);

        static object_geometry_e get_geometry(uint32_t value);

      private:

        static void add_all(void);

        static void add(
            const int domain,
            const int kind,
            const int category,
            const int subcategory,
            const char *description_ptr,
            const char *geometry_ptr
        );

        static std::map<uint32_t, std::string>
            descriptions;
        static std::map<uint32_t, object_geometry_e>
            geometries;
        static bool
            loaded;
    };
}

#endif
