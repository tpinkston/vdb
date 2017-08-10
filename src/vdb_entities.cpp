#include "vdb_entities.h"
#include "vdb_options.h"

#include "vdis_entity_types.h"
#include "vdis_integer.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"

namespace
{
    vdb::options_t
        *options_ptr = 0;
    vdb::entities_t
        entities;
}

bool entities_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
int entities_main(int argc, char *argv[])
{
    int result = 1;

    options_ptr = new vdb::options_t("vdb-entities", argc, argv);
    options_ptr->add(OPTION_HELP);
    options_ptr->add(OPTION_EXTRA);
    options_ptr->add(OPTION_ERRORS);
    options_ptr->add(OPTION_MONO);
    options_ptr->add(OPTION_VERBOSE);
    options_ptr->add(OPTION_WARNINGS);
    options_ptr->add(vdb::option_t("kind", 'k', true));
    options_ptr->add(vdb::option_t("domain", 'd', true));
    options_ptr->add(vdb::option_t("country", 't', true));
    options_ptr->add(vdb::option_t("countries", 'T', false));

    options_ptr->set_callback(*entities_option_callback);

    if (options_ptr->parse())
    {
        result = entities.run();
    }

    return result;
}

// ----------------------------------------------------------------------------
bool entities_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (not options_ptr)
    {
        LOG_ERROR("Options parser not available!");
    }
    else if (option.short_option == 'k')
    {
        success = options_ptr->parse_integers_in_range(
            value,
            0,
            (vdis::ent_kind_e::ENT_KIND_END - 1),
            entities.kinds);

        if (not success)
        {
            std::cerr << "vdb-entities: invalid value for entity kind: "
                      << value << std::endl;
        }
    }
    else if (option.short_option == 'd')
    {
        success = options_ptr->parse_integers_in_range(
            value,
            0,
            (vdis::domain_e::DOMAIN_END - 1),
            entities.domains);

        if (not success)
        {
            std::cerr << "vdb-entities: invalid value for entity domain: "
                      << value << std::endl;
        }
    }
    else if (option.short_option == 't')
    {
        success = options_ptr->parse_integers_in_range(
            value,
            0,
            (vdis::country_e::COUNTRY_END - 1),
            entities.countries);

        if (not success)
        {
            std::cerr << "vdb-entities: invalid value for entity country: "
                      << value << std::endl;
        }
    }
    else if (option.short_option == 'T')
    {
        entities.print_countries = true;
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
    int result = 1;

    // File argument required
    //
    if (options::command_arguments.size() > 0)
    {
        std::cerr << "vdb-entities: too many arguments" << std::endl;
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

        result = 0;
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
