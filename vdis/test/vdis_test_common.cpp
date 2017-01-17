#include "vdis_data_types.h"
#include "vdis_string.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void vdis::test::test_common(void)
{
    out << "==============================================" << std::endl;
    out << "vdis::test::test_common" << std::endl;
    out << "==============================================" << std::endl;

    timestamp_t
        timestamp;
    uint32_t
        minutes;
    float32_t
        seconds;
    bool
        absolute;

    timestamp.set(4, 35.0f, true);
    timestamp.get(minutes, seconds, absolute);

    out << "timestamp " << to_string(timestamp) << std::endl
        << "timestamp.minutes " << to_string(minutes) << std::endl
        << "timestamp.seconds " << to_string(seconds) << std::endl
        << "timestamp.absolute " << to_string(absolute)
        << " (" << timestamp.is_absolute() << ")" << std::endl
        << "timestamp.milliseconds " << timestamp.get_milliseconds()
        << std::endl;

    TEST_TRUE(2.01, minutes == 4);
    TEST_TRUE(2.02, is_close(seconds, 35.0f));
    TEST_TRUE(2.03, absolute);

    timestamp.set_milliseconds(630000);
    timestamp.set_absolute(false);
    timestamp.get(minutes, seconds, absolute);

    out << "timestamp " << to_string(timestamp) << std::endl
        << "timestamp.minutes " << to_string(minutes) << std::endl
        << "timestamp.seconds " << to_string(seconds) << std::endl
        << "timestamp.absolute " << to_string(absolute)
        << " (" << timestamp.is_absolute() << ")" << std::endl
        << "timestamp.milliseconds " << timestamp.get_milliseconds()
        << std::endl;

    TEST_TRUE(2.04, (int)minutes == 10);
    TEST_TRUE(2.05, is_close(seconds, 30.0f));
    TEST_FALSE(2.06, absolute);

    id_t
        entity_id = { 1, 2, 3 };

    out << "entity_id = " << to_string(entity_id) << std::endl;

    TEST_TRUE(3.01, (entity_id.site == 1) and (entity_id.application == 2));

    clear_memory(entity_id);

    TEST_TRUE(3.02, (entity_id.site == 0) and (entity_id.application == 0));

    pdu_header_t
        header = { 7, 1, 1, 1, { timestamp.value }, 144, 0, 0 };

    header.print("test.", out);
}
