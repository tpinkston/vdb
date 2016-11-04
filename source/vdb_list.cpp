#include "vdb_associations.h"
#include "vdb_entities.h"
#include "vdb_file_readers.h"
#include "vdb_filter.h"
#include "vdb_fires.h"
#include "vdb_lasers.h"
#include "vdb_list.h"
#include "vdb_options.h"
#include "vdb_output_file.h"
#include "vdb_pdu_data.h"
#include "vdb_print.h"

#include "vdis_enums.h"
#include "vdis_pdus.h"
#include "vdis_logger.h"

vdb::output_file_t
    *vdb::list::file_ptr = 0;
uint32_t
    vdb::list::pdus_listed = 0,
    vdb::list::pdus_filtered_out = 0;

// ----------------------------------------------------------------------------
int vdb::list::list_pdus(void)
{
    int
        result = 0;

    // File argument required
    //
    if (options::command_arguments.empty())
    {
        std::cerr << "vdb list: missing file argument" << std::endl;

        result = 1;
    }
    else if (options::command_arguments.size() > 1)
    {
        std::cerr << "vdb list: too many arguments" << std::endl;

        result = 1;
    }
    else
    {
        const string_t
            filename = options::command_arguments[0];

        LOG_EXTRA_VERBOSE("Starting list...");

        file_reader_t
            *reader_ptr = 0;

        if (options::pcap)
        {
            reader_ptr = new pcap_reader_t(filename);
        }
        else
        {
            reader_ptr = new standard_reader_t(filename);
        }

        if (not reader_ptr->good())
        {
            result = 1;
        }
        else
        {
            if (not options::output_file.empty())
            {
                std::string
                    comment = ("List output from '" + filename + "'");

                file_ptr = new output_file_t(options::output_file, &comment);

                // Ensure that file was successfully opened.
                //
                result = file_ptr->get_file() ? 0 : 1;
            }

            if (result == 0)
            {
                if (not reader_ptr->parse(process_pdu_data))
                {
                    result = 1;
                }
                else
                {
                    LOG_VERBOSE("PDUs listed: %d...", pdus_listed);
                    LOG_VERBOSE("PDUs filtered out: %d...", pdus_filtered_out);
                }
            }

            if (file_ptr)
            {
                delete file_ptr;
                file_ptr = 0;
            }
        }

        delete reader_ptr;
        reader_ptr = 0;
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::list::process_pdu_data(const pdu_data_t &data)
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
                    if (file_ptr)
                    {
                        file_ptr->write_pdu_data(data);
                    }

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
