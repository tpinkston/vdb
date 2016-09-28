#include "vdis_byte_stream.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void vdis::test::test_byte_stream(void)
{
    out << "==============================================" << std::endl;
    out << "vdis::test::test_data_stream" << std::endl;
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

