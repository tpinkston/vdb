#include "vdb_common.h"
#include "vdb_file_reader.h"
#include "vdb_file_writer.h"
#include "vdb_filter.h"
#include "vdb_list.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"
#include "vdb_scan.h"

#include "vdis_entity_types.h"
#include "vdis_enums.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_object_types.h"

// ----------------------------------------------------------------------------
vdb::list_t::list_t(
    const std::string &command,
    const std::vector<std::string> &arguments
) :
    file_read_command_t(command, arguments),
    file_ptr(0),
    output_index(0),
    pdus_listed(0),
    pdus_filtered_out(0)
{
    options.add(OPTION_EXTRACT);
    options.add(OPTION_DUMP);
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
    options.add(vdb::option_t("output", 'o', true));
    options.add(vdb::option_t("scan", 'S', true));
    options.add(vdb::option_t("scanall", 'A', false));
}

// ----------------------------------------------------------------------------
vdb::list_t::~list_t(void)
{
    if (file_ptr)
    {
        delete file_ptr;
        file_ptr = 0;
    }
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
        LOG_FATAL("Missing file argument");
    }
    else if (options::command_arguments.size() > 1)
    {
        LOG_FATAL("Too many arguments");
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
bool vdb::list_t::option_callback(
    const option_t &option,
    const std::string &value,
    bool &success)
{
    bool result = true;

    if (option.short_option == 'o')
    {
        output_file = value;
    }
    else if (option.short_option == 'A')
    {
        vdb::scan::scan_all();
    }
    else if (option.short_option == 'S')
    {
        success = vdb::scan::parse(value);

        if (not success)
        {
            LOG_FATAL("Invalid scan parameters: %s", value.c_str());
        }
    }
    else
    {
        result = false;
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
                    if (scan::scanning)
                    {
                        scan::process_pdu(data, *pdu_ptr);
                    }
                    else
                    {
                        print::print_pdu(data, *pdu_ptr, std::cout);
                    }

                    if (file_ptr)
                    {
                        // Make a copy of the data, overwrite the index
                        // and write it to the output file.
                        //
                        pdu_data_t data_copy(data);

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
