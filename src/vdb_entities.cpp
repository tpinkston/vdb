#include "vdb_entities.h"
#include "vdb_options.h"

#include "vdis_entity_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"

// ----------------------------------------------------------------------------
vdb::entities_t::entities_t(
    const std::string &name,
    const std::vector<std::string> &arguments
) :
    command_t(name, arguments),
    print_countries(false)
{
    options.add(vdb::option_t("kind", 'k', true));
    options.add(vdb::option_t("domain", 'd', true));
    options.add(vdb::option_t("country", 't', true));
    options.add(vdb::option_t("countries", 'T', false));
}

// ----------------------------------------------------------------------------
vdb::entities_t::~entities_t(void)
{

}

// ----------------------------------------------------------------------------
bool vdb::entities_t::option_callback(
    const option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (option.short_option == 'k')
    {
        success = options.parse_integers_in_range(
            value,
            0,
            (vdis::ent_kind_e::ENT_KIND_END - 1),
            kinds);

        if (not success)
        {
            LOG_FATAL(
                "Invalid value for entity kind: %s",
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
                "Invalid value for entity domain: %s",
                value.c_str());
        }
    }
    else if (option.short_option == 't')
    {
        success = options.parse_integers_in_range(
            value,
            0,
            (vdis::country_e::COUNTRY_END - 1),
            countries);

        if (not success)
        {
            LOG_FATAL(
                "Invalid value for entity country: %s",
                value.c_str());
        }
    }
    else if (option.short_option == 'T')
    {
        print_countries = true;
    }
    else
    {
        result = false;
    }

    return result;
}

// ----------------------------------------------------------------------------
int vdb::entities_t::run(void)
{
    int result = 0;

    if (options::command_arguments.size() > 0)
    {
        LOG_FATAL("Too many arguments (try --help)");
        result = 1;
    }
    else
    {
        vdis::enumerations::load();
        vdis::entity_types::load();

        if (print_countries)
        {
            for(uint16_t i = 0; i < vdis::country_e::COUNTRY_END; ++i)
            {
                if (vdis::enumerations::valid(ENUM_COUNTRY, i))
                {
                    if (i < 10)
                    {
                        std::cout << "  ";
                    }
                    else if (i < 100)
                    {
                        std::cout << " ";
                    }

                    std::cout << i << " "
                              << color::cyan << "0x"
                              << vdis::to_hex_string(i) << " " << color::yellow
                              << vdis::enumerations::get_name(ENUM_COUNTRY, i)
                              << color::none << std::endl;
                }
            }
        }
        else
        {
            vdis::entity_types::mapping_t::const_iterator
                itor = vdis::entity_types::begin();
            vdis::entity_type_t
                type;

            while(itor != vdis::entity_types::end())
            {
                const uint64_t value = itor->first;

                type.set(value);

                if (list_type(type))
                {
                    std::cout << type << " " << color::blue
                              << vdis::entity_types::get_name(value)
                              << " " << color::yellow
                              << vdis::entity_types::get_description(value)
                              << color::none << std::endl;

                    if (options::extra)
                    {
                        std::cout << "  kind: " << color::green
                                  << type.kind_enum()
                                  << color::none << std::endl;
                        std::cout << "  domain: " << color::green
                                  << type.domain_enum()
                                  << color::none << std::endl;
                        std::cout << "  country: " << color::green
                                  << type.country_enum()
                                  << color::none << std::endl;
                    }
                }

                ++itor;
            }
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::entities_t::list_type(const vdis::entity_type_t &type)
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

    if (not countries.empty())
    {
        if (countries.find((uint32_t)type.country) == countries.end())
        {
            return false;
        }
    }

    return true;
}
