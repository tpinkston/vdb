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
