#include "vdb_objects.h"
#include "vdb_objects_help.h"
#include "vdb_options.h"
#include "vdb_version.h"

#include "vdis_object_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"

namespace
{
    vdb::options_t
        *options_ptr = 0;
    vdb::objects_t
        objects;
}

bool option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
int objects_main(int argc, char *argv[])
{
    int result = 1;

    options_ptr = new vdb::options_t("vdb-objects", argc, argv);
    options_ptr->add(OPTION_COLOR);
    options_ptr->add(OPTION_EXTRA);
    options_ptr->add(OPTION_ERRORS);
    options_ptr->add(OPTION_WARNINGS);
    options_ptr->add(OPTION_VERBOSE);
    options_ptr->add(OPTION_HELP);
    options_ptr->add(vdb::option_t("geometry", 'g', true));
    options_ptr->add(vdb::option_t("domain", 'd', true));
    options_ptr->add(vdb::option_t("kind", 'k', true));

    options_ptr->set_callback(*option_callback);

    if (options_ptr->parse())
    {
        if (vdb::options::version)
        {
            print_vdb_version();
            result = 0;
        }
        else if (vdb::options::help)
        {
            print_objects_help();
            result = 0;
        }
        else
        {
            result = objects.run();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (not options_ptr)
    {
        LOG_ERROR("Options parser not available!");
    }
    else if (option.short_option == 'g')
    {
        success = options_ptr->parse_integers_in_range(
            value,
            0,
            (vdis::object_geometry_e::OBJECT_GEOMETRY_END - 1),
            objects.geometries);

        if (not success)
        {
            std::cerr << "vdb-objects: invalid value for object geometry: "
                      << value << std::endl;
        }
    }
    else if (option.short_option == 'd')
    {
        success = options_ptr->parse_integers_in_range(
            value,
            0,
            (vdis::domain_e::DOMAIN_END - 1),
            objects.domains);

        if (not success)
        {
            std::cerr << "vdb-objects: invalid value for object domain: "
                      << value << std::endl;
        }
    }
    else if (option.short_option == 'k')
    {
        success = options_ptr->parse_integers_in_range(
            value,
            0,
            (vdis::object_kind_e::OBJECT_KIND_END - 1),
            objects.kinds);

        if (not success)
        {
            std::cerr << "vdb-objects: invalid value for object kind: "
                      << value << std::endl;
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
    int result = 1;

    // File argument required
    //
    if (options::command_arguments.size() > 0)
    {
        std::cerr << "vdb-objects: too many arguments" << std::endl;
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

        result = 0;
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
