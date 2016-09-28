#include <string>
#include <vector>

#include "vdis_byte_stream.h"
#include "vdis_pdus.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void vdis::test::test_pdus(void)
{
    std::vector<std::string>
        filenames;

    filenames.push_back("001_entity_state.UH60M");
    filenames.push_back("001_entity_state.M1A2");
    filenames.push_back("001_entity_state.DI_AK74");
    filenames.push_back("001_entity_state.SLING_HMMWV");

    out << "==============================================" << std::endl;
    out << "vdis::test::test_pdus" << std::endl;
    out << "==============================================" << std::endl;

    for(uint32_t i = 0; i < filenames.size(); ++i)
    {
        byte_stream_t
            stream;

        if (import_pdu(filenames[i], stream))
        {
            pdu_t
                pdu(stream);

            out << "PDU:" << std::endl << pdu;
        }
    }
}
