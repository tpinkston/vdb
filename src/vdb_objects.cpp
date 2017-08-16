#include "vdb_objects.h"
#include "vdb_options.h"

#include "vdis_object_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"

// ----------------------------------------------------------------------------
vdb::objects_t::objects_t(
    const std::string &name,
    const std::vector<std::string> &arguments
) :
    command_t(name, arguments)
{
    options.add(vdb::option_t("geometry", 'g', true));
    options.add(vdb::option_t("domain", 'd', true));
    options.add(vdb::option_t("kind", 'k', true));
}

// ----------------------------------------------------------------------------
vdb::objects_t::~objects_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::objects_t::option_callback(
    const option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (option.short_option == 'g')
    {
        success = options.parse_integers_in_range(
            value,
            0,
            (vdis::object_geometry_e::OBJECT_GEOMETRY_END - 1),
            geometries);

        if (not success)
        {
            LOG_FATAL(
                "Invalid value for object geometry: %s",
                value.c_str());
        }
    }
    else if (option.short_option == 'd')
    {
        success = options.parse_integers_in_range(
            value,
            0,
            (vdis::domain_e::DOMAIN_END - 1),
            domains);

        if (not success)
        {
            LOG_FATAL(
                "Invalid value for object domain: %s",
                value.c_str());
        }
    }
    else if (option.short_option == 'k')
    {
        success = options.parse_integers_in_range(
            value,
            0,
            (vdis::object_kind_e::OBJECT_KIND_END - 1),
            kinds);

        if (not success)
        {
            LOG_FATAL(
                "Invalid value for object kind: %s",
                value.c_str());
        }
    }
    else
    {
        result = false;
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::objects_t::run(void)
{
    int result = 0;

    if (options::command_arguments.size() > 0)
    {
        LOG_FATAL("Too many arguments");
        result = 1;
    }
    else
    {
        vdis::enumerations::load();
        vdis::object_types::load();

        if (geometries.empty())
        {
            // List all geometries
            //
            geometries.insert(vdis::OBJECT_GEOMETRY_POINT);
            geometries.insert(vdis::OBJECT_GEOMETRY_LINEAR);
            geometries.insert(vdis::OBJECT_GEOMETRY_AREAL);
        }

        std::set<uint32_t>::const_iterator
            geometry_itor = geometries.begin();

        while(geometry_itor != geometries.end())
        {
            const vdis::object_geometry_e
                geometry = (vdis::object_geometry_e)*geometry_itor;
            vdis::object_types::mapping_t::const_iterator
                itor = vdis::object_types::begin(geometry),
                itor_end = vdis::object_types::end(geometry);
            vdis::object_type_t
                type;

            while(itor != itor_end)
            {
                const uint64_t value = itor->first;

                type.set(value);

                if (list_type(type))
                {
                    const std::string
                        geometry_name = vdis::enumerations::get_name(
                            ENUM_OBJECT_GEOMETRY,
                            geometry),
                        &description = vdis::object_types::get_description(
                            geometry,
                            value);

                    std::cout << color::blue << geometry_name << " "
                              << color::none << type << " "
                              << color::yellow << description
                              << color::none << std::endl;

                    if (options::extra)
                    {
                        std::cout << "  kind: " << color::green
                                  << type.kind_enum()
                                  << color::none << std::endl;
                        std::cout << "  domain: " << color::green
                                  << type.domain_enum()
                                  << color::none << std::endl;
                    }
                }

                ++itor;
            }

            ++geometry_itor;
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::objects_t::list_type(const vdis::object_type_t &type)
{
    if (not kinds.empty())
    {
        if (kinds.find((uint32_t)type.kind) == kinds.end())
        {
            return false;
        }
    }

    if (not domains.empty())
    {
        if (domains.find((uint32_t)type.domain) == domains.end())
        {
            return false;
        }
    }

    return true;
}
