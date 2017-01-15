#include "vdb_associations.h"
#include "vdb_common.h"
#include "vdb_entities.h"
#include "vdb_file_reader.h"
#include "vdb_file_writer.h"
#include "vdb_filter.h"
#include "vdb_fires.h"
#include "vdb_lasers.h"
#include "vdb_list.h"
#include "vdb_list_help.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"
#include "vdb_version.h"

#include "vdis_entity_types.h"
#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_object_types.h"

namespace
{
    vdb::list_t
        list;
}

bool option_callback(const vdb::option_t &option, const std::string &value);

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    vdb::options_t
        options("vdb-list", argc, argv);
    int
        result = 1;

    options.add(OPTION_EXTRA);
    options.add(OPTION_EXTRACT);
    options.add(OPTION_DUMP);
    options.add(OPTION_COLOR);
    options.add(OPTION_ERRORS);
    options.add(OPTION_WARNINGS);
    options.add(OPTION_VERBOSE);
    options.add(vdb::option_t("output", 'o', true));
    options.add(OPTION_HOSTNAME);
    options.add(OPTION_XHOSTNAME);
    options.add(OPTION_EXERCISE);
    options.add(OPTION_XEXERCISE);
    options.add(OPTION_TYPE);
    options.add(OPTION_XTYPE);
    options.add(OPTION_FAMILY);
    options.add(OPTION_XFAMILY);
    options.add(OPTION_ID);
    options.add(OPTION_XID);
    options.add(OPTION_RANGE);
    options.add(OPTION_HELP);
    options.add(OPTION_VERSION);

    options.set_callback(*option_callback);

    if (options.parse())
    {
        if (vdb::options::version)
        {
            print_vdb_version();
            result = 0;
        }
        else if (vdb::options::help)
        {
            print_help();
            result = 0;
        }
        else
        {
            result = list.run();
        }
    }

    return result;
}

// ----------------------------------------------------------------------------
bool option_callback(const vdb::option_t &option, const std::string &value)
{
    bool success = true;

    if (option.short_option == 'o')
    {
        list.output_file = value;
    }
    else
    {
        std::cerr << "vdb-list: unexpected argument: "
                  << option << std::endl;
        success = false;
    }

    return false;
}

// ----------------------------------------------------------------------------
int vdb::list_t::run(void)
{
    std::string
        filename;
    file_reader_t
        *reader_ptr = 0;
    int
        result = 1;

    // File argument required
    //
    if (options::command_arguments.empty())
    {
        std::cerr << "vdb list: missing file argument" << std::endl;
    }
    else if (options::command_arguments.size() > 1)
    {
        std::cerr << "vdb list: too many arguments" << std::endl;
    }
    else
    {
        LOG_EXTRA_VERBOSE("Starting list...");

        filename = options::command_arguments[0];
        reader_ptr = new standard_reader_t(filename);

        if (reader_ptr->good())
        {
            bool listing = true;

            if (not output_file.empty())
            {
                struct stat
                    file_stat;

                LOG_VERBOSE("Checking file '%s'...", output_file.c_str());

                if (stat(output_file.c_str(), &file_stat) == 0)
                {
                    std::cout << "Overwrite '" << output_file << "'? ";
                    listing = user_confirmation();
                }

                if (listing)
                {
                    std::string
                        comment = ("List output from '" + filename + "'");

                    file_ptr = new file_writer_t(output_file, &comment);

                    // Ensure that file was successfully opened.
                    //
                    result = file_ptr->get_file() ? 0 : 1;
                }
            }

            if (listing)
            {
                vdis::set_byteswapping();
                vdis::enumerations::load();
                vdis::entity_types::load();
                vdis::object_types::load();

                if (reader_ptr->parse(this))
                {
                    result = 0;
                    LOG_VERBOSE("PDUs listed: %d...", pdus_listed);
                    LOG_VERBOSE("PDUs filtered out: %d...", pdus_filtered_out);
                }
            }
        }

        delete reader_ptr;
        reader_ptr = 0;
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::list_t::process_pdu_data(const pdu_data_t &data)
{
    bool
        processed = false,
        past_end = false;

    if (filter::filter_by_range(data.get_index(), past_end))
    {
        if (filter::filter_by_header(data) and
            filter::filter_by_metadata(data))
        {
            const vdis::pdu_t
                *pdu_ptr = data.generate_pdu();

            if (pdu_ptr)
            {
                if (filter::filter_by_content(*pdu_ptr))
                {
                    if (not options::scanning)
                    {
                        print::print_pdu(data, *pdu_ptr, std::cout);
                    }
                    else
                    {
                        if (options::scan_entities)
                        {
                            entities::process_pdu(data, *pdu_ptr);
                        }

                        if (options::scan_associations)
                        {
                            associations::process_pdu(data, *pdu_ptr);
                        }

                        if (options::scan_lasers)
                        {
                            lasers::process_pdu(data, *pdu_ptr);
                        }

                        if (options::scan_fires)
                        {
                            fires::process_pdu(data, *pdu_ptr);
                        }

                        if (options::scan_collisions)
                        {
                            // TODO scan_collisions
                        }

                        if (options::scan_objects)
                        {
                            // TODO scan_objects
                        }
                    }

                    if (file_ptr)
                    {
                        pdu_data_t
                            data_copy(data);

                        data_copy.set_index(output_index++);

                        file_ptr->write_pdu_data(data_copy);
                    }

                    processed = true;
                }

                delete pdu_ptr;
                pdu_ptr = 0;
            }
        }
        else
        {
            const vdis::pdu_type_e
                pdu_type = (vdis::pdu_type_e)data.get_pdu_type();

            if (pdu_type == vdis::PDU_TYPE_ENTITY_STATE)
            {
                vdis::id_t
                    id;
                vdis::marking_t
                    marking;
                vdis::byte_stream_t
                    bio(data.get_pdu_buffer(), data.get_pdu_length());

                bio.skip(12); // header, up to 'id'
                id.read(bio);

                bio.skip(110); // up to 'marking'
                marking.read(bio);

                vdis::entity_marking::set(id, marking);
            }
        }
    }

    if (processed)
    {
        pdus_listed += 1;
    }
    else
    {
        pdus_filtered_out += 1;
    }

    return not past_end;
}
