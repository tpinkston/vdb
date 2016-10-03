#include <string>
#include <vector>

#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_pdus.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void add_entity_marking(
    const std::string &name,
    uint16_t site,
    uint16_t application,
    uint16_t entity)
{
    vdis::id_t id = { site, application, entity };
    vdis::marking_t marking;

    marking.str(name);

    vdis::entity_marking::set(id, marking);
}

// ----------------------------------------------------------------------------
void vdis::test::test_pdus(void)
{
    std::vector<std::string>
        filenames;

    add_entity_marking("SHOOTER-1", 50, 126, 5);
    add_entity_marking("TARGET-1", 50, 126, 27);
    add_entity_marking("SHOOTER-2", 50, 126, 39);
    add_entity_marking("TARGET-2", 50, 126, 20);

    filenames.push_back("001_entity_state.UH60M");
    filenames.push_back("001_entity_state.M1A2");
    filenames.push_back("001_entity_state.DI_AK74");
    filenames.push_back("001_entity_state.SLING_HMMWV");
    filenames.push_back("002_fire.40MM");
    filenames.push_back("002_fire.M799");
    filenames.push_back("003_detonation.40MM");
    filenames.push_back("003_detonation.M799");
    filenames.push_back("200_application_control.STATUS1");
    filenames.push_back("200_application_control.STEALTH1");

    out << "==============================================" << std::endl;
    out << "vdis::test::test_pdus" << std::endl;
    out << "==============================================" << std::endl;

    for(uint32_t i = 0; i < filenames.size(); ++i)
    {
        byte_stream_t
            stream;

        out << std::endl
            << "==========================================" << std::endl
            << "PDU[" << filenames[i] << "]:" << std::endl
            << "==========================================" << std::endl;

        if (import_pdu(filenames[i], stream))
        {
            pdu_t
                pdu(stream);

            out << pdu << std::endl;
        }
    }
}
