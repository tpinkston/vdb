#ifndef VDIS_OBJECT_TYPES_H
#define VDIS_OBJECT_TYPES_H

#include "vdis_data_types.h"

namespace vdis
{
    struct object_type_t;

    class object_types
    {
      public:

        typedef std::map<uint32_t, std::string>
            mapping_t;

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

        inline
        static mapping_t::const_iterator begin(object_geometry_e geometry)
        {
            switch(geometry)
            {
                case vdis::OBJECT_GEOMETRY_POINT:
                    return point_descriptions.begin();
                case vdis::OBJECT_GEOMETRY_LINEAR:
                    return linear_descriptions.begin();
                case vdis::OBJECT_GEOMETRY_AREAL:
                    return areal_descriptions.begin();
                default:
                    return unknown_descriptions.begin();
            }
        }

        inline
        static mapping_t::const_iterator end(object_geometry_e geometry)
        {
            switch(geometry)
            {
                case vdis::OBJECT_GEOMETRY_POINT:
                    return point_descriptions.end();
                case vdis::OBJECT_GEOMETRY_LINEAR:
                    return linear_descriptions.end();
                case vdis::OBJECT_GEOMETRY_AREAL:
                    return areal_descriptions.end();
                default:
                    return unknown_descriptions.end();
            }
        }

      private:

        static void print(
            const mapping_t &descriptions,
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

        static mapping_t
            unknown_descriptions,
            point_descriptions,
            linear_descriptions,
            areal_descriptions;
        static bool
            loaded;
    };
}

#endif
