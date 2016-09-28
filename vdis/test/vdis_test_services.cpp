#include "vdis_services.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void vdis::test::test_services(void)
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
    out << "vdis::test::test_services" << std::endl;
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
