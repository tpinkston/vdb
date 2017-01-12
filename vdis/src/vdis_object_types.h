#ifndef VDIS_OBJECT_TYPES_H
#define VDIS_OBJECT_TYPES_H

#include "vdis_data_types.h"

namespace vdis
{
    struct object_type_t;

    class object_types
    {
      public:

        static void load(void);

        static void print(std::ostream &stream);

        static const std::string &get_description(
            object_geometry_e geometry,
            uint32_t value);

        static bool get_parent(
            const object_type_t &child,
            object_type_t &parent
        );

        static bool get_valid_parent(
            const object_geometry_e geometry,
            const object_type_t &child,
            object_type_t &parent
        );

      private:

        static void print(
            const std::map<uint32_t, std::string> &descriptions,
            object_geometry_e geometry,
            std::ostream &stream);

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
            point_descriptions,
            linear_descriptions,
            areal_descriptions;
        static bool
            loaded;
    };
}

#endif
