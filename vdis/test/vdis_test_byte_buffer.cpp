#include "vdis_byte_buffer.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void vdis::test::test_byte_buffer(void)
{
    out << "==============================================" << std::endl;
    out << "vdis::test::test_byte_buffer" << std::endl;
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

