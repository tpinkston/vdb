// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

#include "vdb_enums.h"
#include "vdb_file_readers.h"
#include "vdb_filter.h"
#include "vdb_ids.h"
#include "vdb_list.h"
#include "vdb_logger.h"
#include "vdb_marking.h"
#include "vdb_options.h"
#include "vdb_pdu_data.h"
#include "vdb_pdu.h"
#include "vdb_print.h"
#include "vdb_string.h"

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
    if (options::get_command_arguments().empty())
    {
        std::cerr << options::get_terminal_command()
                  << " list: missing file argument" << std::endl;

        result = 1;
    }
    else if (options::get_command_arguments().size() > 1)
    {
        std::cerr << options::get_terminal_command()
                  << " list: too many arguments" << std::endl;

        result = 1;
    }
    else
    {
        const std::string
            filename = *options::get_command_argument(0);

        LOG_EXTRA_VERBOSE("Starting list...");

        file_reader_t
            *reader_ptr = new standard_reader_t(filename);

        if (not reader_ptr->good())
        {
            result = 1;
        }
        else if (not reader_ptr->read_header())
        {
            result = 1;
        }
        else if (not reader_ptr->parse(process_pdu_data))
        {
            result = 1;
        }
        else
        {
            LOG_VERBOSE("PDUs listed: %d...", pdus_listed);
            LOG_VERBOSE("PDUs filtered out: %d...", pdus_filtered_out);
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

    if (options::pdu_index_in_range(data.get_index(), past_end))
    {
        if (filter::filter_by_header(data))
        {
            const pdu_t
                *pdu_ptr = data.generate_pdu();

            if (pdu_ptr)
            {
                if (filter::filter_by_content(*pdu_ptr))
                {
                    print::print_pdu(data, *pdu_ptr, std::cout);
                    processed = true;
                }

                delete pdu_ptr;
                pdu_ptr = 0;
            }
        }
        else
        {
            const pdu_type_e
                pdu_type = (pdu_type_e)data.get_pdu_type();

            if (pdu_type == PDU_TYPE_ENTITY_STATE)
            {
                id_t
                    id;
                entity_marking_t
                    marking;
                byte_stream
                    bio(data.get_pdu_buffer(), data.get_pdu_length());

                bio.skip(12); // header, up to 'id'
                bio.read(id);

                bio.skip(110); // up to 'marking'
                bio.read(marking);

                id_mappings::set_marking(id, marking);
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
