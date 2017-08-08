#include "vdb_common.h"
#include "vdb_file_reader.h"
#include "vdb_file_stream.h"
#include "vdb_extract.h"
#include "vdb_extract_help.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"
#include "vdb_version.h"

#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_object_types.h"
#include "vdis_pdus.h"
#include "vdis_string.h"

namespace
{
    vdb::extract_t
        extract;
}

bool extract_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success
);

// ----------------------------------------------------------------------------
int extract_main(int argc, char *argv[])
{
    vdb::options_t
        options("vdb-extract", argc, argv);
    int
        result = 1;

    options.add(OPTION_HELP);
    options.add(OPTION_EXTRA);
    options.add(OPTION_EXTRACT);
    options.add(OPTION_DUMP);
    options.add(OPTION_COLOR);
    options.add(OPTION_ERRORS);
    options.add(OPTION_WARNINGS);
    options.add(OPTION_VERBOSE);
    options.add(vdb::option_t("output", 'o', true));
    options.add(vdb::option_t("index", 'i', true));

    options.set_callback(*extract_option_callback);

    if (options.parse())
    {
        if (vdb::options::version)
        {
            print_vdb_version();
            result = 0;
        }
        else if (vdb::options::help)
        {
            print_extract_help();
            result = 0;
        }
        else
        {
            result = extract.run();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool extract_option_callback(
    const vdb::option_t &option,
    const std::string &value,
    bool &success)
{
    return false;
}

// ----------------------------------------------------------------------------
int vdb::extract_t::run(void)
{
    std::string
        filename;
    file_reader_t
        *reader_ptr = 0;
    int
        result = 1;

    if (verify_command_arguments())
    {
        LOG_VERBOSE("Starting extract...");

        filename = options::command_arguments[1];
        reader_ptr = new standard_reader_t(filename);

        if (reader_ptr->good())
        {
            bool extracting = true;

            if (not output_file.empty())
            {
                struct stat
                    file_stat;

                LOG_VERBOSE("Checking file '%s'...", output_file.c_str());

                if (stat(output_file.c_str(), &file_stat) == 0)
                {
                    std::cout << "Overwrite '" << output_file << "'? ";
                    extracting = user_confirmation();
                }
            }

            if (extracting)
            {
                vdis::set_byteswapping();
                vdis::enumerations::load();
                vdis::entity_types::load();
                vdis::object_types::load();

                if (reader_ptr->parse(this))
                {
                    result = 0;
                }
            }
        }

        delete reader_ptr;
        reader_ptr = 0;
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::extract_t::verify_command_arguments(void)
{
    const std::vector<std::string>
        &arguments = options::command_arguments;
    bool
        valid = false;

    if (arguments.size() < 3)
    {
        CONSOLE_ERR("Too few arguments");
    }
    else if (arguments.size() > 3)
    {
        CONSOLE_ERR("Too many arguments");
    }
    else if (not vdis::to_int64(arguments[0], index))
    {
        CONSOLE_ERR("Invalid index: %s", arguments[0].c_str());
    }
    else if (index < 0)
    {
        CONSOLE_ERR("Negative index: %d", index);
    }
    else
    {
        output_file = arguments[2];
        valid = true;
    }

    return valid;
}

// ----------------------------------------------------------------------------
bool vdb::extract_t::process_pdu_data(const pdu_data_t &data)
{
    const vdis::pdu_t *pdu_ptr = 0;

    if (data.get_index() == index)
    {
        LOG_VERBOSE("Found PDU index %d, generating PDU...", index);

        pdu_ptr = data.generate_pdu();

        if (pdu_ptr)
        {
            print::print_pdu(data, *pdu_ptr, std::cout);

            write_to_output_file(data, *pdu_ptr);

            delete pdu_ptr;
            pdu_ptr = 0;
        }
        else
        {
            LOG_ERROR("Failed to generate PDU!");
        }

        pdu_found = true;
    }

    return not pdu_found;
}

// ----------------------------------------------------------------------------
void vdb::extract_t::write_to_output_file(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu)
{
    file_stream stream;

    std::cout << "Writing PDU (" << data.get_pdu_length()
              << " bytes) to file '"
              << color::bold_cyan << output_file << color::none
              << "'" << std::endl;

    stream.reset(data.get_pdu_length());
    stream.write(data.get_pdu_buffer(), data.get_pdu_length());
    stream.write_file(output_file);
}
