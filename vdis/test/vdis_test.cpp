#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_datum_records.h"
#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_pdus.h"
#include "vdis_services.h"
#include "vdis_string.h"

#include <fstream>
#include <iostream>

#define TEST_TRUE(t,e) assert(#t, #e, e, true)
#define TEST_FALSE(t,e) assert(#t, #e, e, false)

namespace
{
    std::ostream
        &err = std::cerr,
        &out = std::cout;
    int
        failures = 0;
}

namespace vdis
{
    void test_data_buffer(void);
    void test_data_stream(void);
    void test_common(void);
    void test_services(void);
    void test_entity_types(void);
    void test_pdus(void);

    bool import_pdu(const std::string &filename, byte_stream_t &stream);
}

// ----------------------------------------------------------------------------
void assert(
    const char *index,
    const char *expression,
    bool result,
    bool expectation)
{
    out << color::bold_blue << index << color::none << ": ";

    if (result == expectation)
    {
        out << color::bold_green << "SUCCESS ";
    }
    else
    {
        failures++;
        out << color::bold_red   << " FAILED ";
    }

    out << color::none << (result ? " true" : "false") << ": "
        << expression << std::endl;
}

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    //logger::set_enabled(logger::EXTRA_VERBOSE, true);
    logger::set_enabled(logger::VERBOSE, true);
    logger::set_enabled(logger::WARNING, true);

    LOG_EXTRA_VERBOSE("Extra verbose on...");
    LOG_VERBOSE("Verbose on...");
    LOG_WARNING("Warnings on...");

    vdis::set_byteswapping();
    vdis::enumerations::load();
    vdis::entity_types::load();

    vdis::test_data_buffer();
    vdis::test_data_stream();
    vdis::test_common();
    vdis::test_services();
    vdis::test_entity_types();
    vdis::test_pdus();

    out << "==============================================" << std::endl;
    out << "VDIS TEST COMPLETE (" << failures << " FAILURES)" << std::endl;
    out << "==============================================" << std::endl;

    return failures;
}

// ----------------------------------------------------------------------------
void vdis::test_data_buffer(void)
{
    out << "==============================================" << std::endl;
    out << "vdis::test_data_buffer" << std::endl;
    out << "==============================================" << std::endl;

    uint8_t
        array[256];

    for(uint16_t i = 0; i < 256; ++i)
    {
        array[i] = i;
    }

    byte_buffer_t
        buffer(array, 256, false);

    out << buffer << std::endl;

    TEST_TRUE(1.01, buffer[100] == 100);
    TEST_TRUE(1.02, buffer[110] == 110);
    TEST_TRUE(1.03, buffer[120] == 120);
    TEST_TRUE(1.04, buffer[300] == 0);
    TEST_TRUE(1.05, buffer.length() == 256);
    TEST_TRUE(1.06, buffer.buffer() != 0);

    array[100]++;
    array[110]++;
    array[120]++;

    TEST_TRUE(2.01, buffer[100] == 101);
    TEST_TRUE(2.02, buffer[110] == 111);
    TEST_TRUE(2.03, buffer[120] == 121);

    buffer.clear();

    TEST_TRUE(3.01, buffer.length() == 0);
    TEST_TRUE(3.02, buffer.buffer() == 0);
}

// ----------------------------------------------------------------------------
void vdis::test_data_stream(void)
{
    out << "==============================================" << std::endl;
    out << "vdis::test_data_stream" << std::endl;
    out << "==============================================" << std::endl;

    int8_t
        value_int8 = 0;
    uint8_t
        value_uint8 = 0;
    int16_t
        value_int16 = 0;
    uint16_t
        value_uint16 = 0;
    int32_t
        value_int32 = 0;
    uint32_t
        value_uint32 = 0;
    int64_t
        value_int64 = 0;
    uint64_t
        value_uint64 = 0;
    byte_stream_t
        stream(128);

    stream.write((int8_t)0x0A);
    stream.write((uint8_t)0xA0);
    stream.write((int16_t)0x0B0B);
    stream.write((uint16_t)0xB00B);
    stream.write((int32_t)0x0C00000C);
    stream.write((uint32_t)0xC000000C);
    stream.write((int64_t)0x0D0000000000000D);
    stream.write((uint64_t)0xD00000000000000D);

    out << stream << std::endl;

    stream.reset_index(0);

    stream.read(value_int8);
    stream.read(value_uint8);
    stream.read(value_int16);
    stream.read(value_uint16);
    stream.read(value_int32);
    stream.read(value_uint32);
    stream.read(value_int64);
    stream.read(value_uint64);

    TEST_TRUE(1.01, value_int8 == 0x0A);
    TEST_TRUE(1.02, value_uint8 == 0xA0);
    TEST_TRUE(1.03, value_int16 == 0x0B0B);
    TEST_TRUE(1.04, value_uint16 == 0xB00B);
    TEST_TRUE(1.05, value_int32 == 0x0C00000C);
    TEST_TRUE(1.06, value_uint32 == 0xC000000C);
    TEST_TRUE(1.07, value_int64 == 0x0D0000000000000D);
    TEST_TRUE(1.08, value_uint64 == 0xD00000000000000D);
}

// ----------------------------------------------------------------------------
void vdis::test_common(void)
{
    out << "==============================================" << std::endl;
    out << "vdis::test_common" << std::endl;
    out << "==============================================" << std::endl;

    timestamp_t
        timestamp;
    uint32_t
        minutes;
    float32_t
        seconds;
    bool
        absolute;

    timestamp.set(4, 35, true);
    timestamp.get(minutes, seconds, absolute);

    out << "timestamp " << to_string(timestamp) << std::endl
        << "timestamp.minutes " << to_string(minutes) << std::endl
        << "timestamp.seconds " << to_string(seconds) << std::endl
        << "timestamp.seconds " << to_string(absolute) << std::endl;

    TEST_TRUE(2.01, (int)minutes == 4);
    TEST_TRUE(2.02, (int)seconds == 35);
    TEST_TRUE(2.03, absolute);

    entity_id_t
        entity_id = { 1, 2, 3 };

    out << "entity_id = " << to_string(entity_id) << std::endl;

    TEST_TRUE(3.01, (entity_id.site == 1) and (entity_id.application == 2));

    clear_memory(entity_id);

    TEST_TRUE(3.02, (entity_id.site == 0) and (entity_id.application == 0));

    pdu_header_t
        header = { 7, 1, 1, 1, { timestamp.value }, 144, 0, 0 };

    header.print("test", out);
}

// ----------------------------------------------------------------------------
void vdis::test_services(void)
{
    const int16_t
        value_int16 = 0xF0A0;
    const uint16_t
        value_uint16 = 0xF0A0;
    const int32_t
        value_int32 = 0xF0A00000;
    const uint32_t
        value_uint32 = 0xF0A00000;
    const int64_t
        value_int64 = 0xF0A0000000000000;
    const uint64_t
        value_uint64 = 0xF0A0000000000000;

    out << "==============================================" << std::endl;
    out << "vdis::test_services" << std::endl;
    out << "==============================================" << std::endl;

    TEST_TRUE(1.01, little_endian());
    TEST_FALSE(1.02, big_endian());
    TEST_FALSE(1.03, byteswapping());

    TEST_TRUE(2.01, byteswap(value_int16, true) == (int16_t)0xA0F0);
    TEST_TRUE(2.02, byteswap(value_uint16, true) == (uint16_t)0xA0F0);
    TEST_TRUE(2.03, byteswap(value_int32, true) == (int32_t)0xA0F0);
    TEST_TRUE(2.04, byteswap(value_uint32, true) == (uint32_t)0xA0F0);
    TEST_TRUE(2.05, byteswap(value_int64, true) == (int64_t)0xA0F0);
    TEST_TRUE(2.06, byteswap(value_uint64, true) == (uint64_t)0xA0F0);
}

// ----------------------------------------------------------------------------
void vdis::test_entity_types(void)
{
    uint64_t
        value = 0;
    entity_type_t
        type,
        parent,
        grandparent;

    out << "==============================================" << std::endl;
    out << "vdis::test_entity_types" << std::endl;
    out << "==============================================" << std::endl;

    type = { 1, 1, 225, 1, 1, 4, 0 };

    TEST_TRUE(1.01, type.get() == 72340035399123968L);
    TEST_TRUE(1.02, to_string(type) == "1.1.225.1.1.4.0");

    value = type.get();
    type.set(value + 1);

    TEST_TRUE(1.03, to_string(type) == "1.1.225.1.1.4.1");

    type.set(value);

    TEST_TRUE(1.04, entity_types::get_valid_parent(type, parent));
    TEST_TRUE(1.05, entity_types::get_valid_parent(parent, grandparent));

    out << "type: " << type << std::endl
        << "type name: "
        << entity_types::get_name(type.get()) << std::endl
        << "type description: "
        << entity_types::get_description(type.get()) << std::endl
        << "parent: " << parent << std::endl
        << "parent name: "
        << entity_types::get_name(parent.get()) << std::endl
        << "parent description: "
        << entity_types::get_description(parent.get()) << std::endl
        << "grandparent: " << grandparent << std::endl
        << "grandparent name: "
        << entity_types::get_name(grandparent.get()) << std::endl
        << "grandparent description: "
        << entity_types::get_description(grandparent.get()) << std::endl;

    type = { 1,1,224,81,0,0,0 };

    TEST_FALSE(1.06, entity_types::get_valid_parent(type, parent));
}

// ----------------------------------------------------------------------------
void vdis::test_pdus(void)
{
    std::vector<std::string>
        filenames;

    filenames.push_back("001_entity_state.UH60M");
    filenames.push_back("001_entity_state.M1A2");
    filenames.push_back("001_entity_state.DI_AK74");
    filenames.push_back("001_entity_state.SLING_HMMWV");

    out << "==============================================" << std::endl;
    out << "vdis::test_pdus" << std::endl;
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

// ----------------------------------------------------------------------------
bool vdis::import_pdu(const std::string &filename, byte_stream_t &stream)
{
    const std::string
        path = ("vdis/data/pdus/" + filename);
    std::ifstream
        input;
    bool
        success = false;

    input.open(path.c_str(), std::ios::in | std::ios::binary);

    if (input.fail())
    {
        LOG_ERROR("Failed to open file: %s", path.c_str());
    }
    else
    {
        std::streamsize
            buffer_size = 0;

        // Go to EOF to get the length.
        //
        input.seekg(0, std::ios::end);

        buffer_size = input.tellg();

        if (input.fail())
        {
            LOG_ERROR("Failed to get file size: %s", path.c_str());
        }
        else
        {
            uint8_t
                buffer[buffer_size];

            // Go back to the beginning and read the entire file.
            //
            input.seekg(0, std::ios::beg);
            input.read((char *)buffer, buffer_size);

            if (input.fail())
            {
                LOG_ERROR("Failed to read file: %s", path.c_str());
            }
            else
            {
                std::streamsize
                    bytes_read = input.gcount();

                stream.reset(buffer, bytes_read);

                input.close();

                success = true;

                LOG_VERBOSE(
                    "%d bytes read from '%s'",
                    bytes_read,
                    filename.c_str());
            }
        }
    }

    return success;
}
