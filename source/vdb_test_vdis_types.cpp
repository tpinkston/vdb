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

#include "vdb_byte_stream.h"
#include "vdb_enumerations.h"
#include "vdb_enums.h"
#include "vdb_entity_type.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_object_type.h"
#include "vdb_string.h"
#include "vdb_test.h"
#include "vdb_time.h"

int test_enum(const std::string &enumeration, int value);
int test_enums(void);
int test_entity_types(void);
int test_object_types(void);
int test_timestamp(void);

// ----------------------------------------------------------------------------
void vdb::test::test_vdis_types(void)
{
    failure_counter += test_enums();
    failure_counter += test_entity_types();
    failure_counter += test_object_types();
    failure_counter += test_timestamp();
}

// ----------------------------------------------------------------------------
int test_enum(const std::string &enumeration, int value)
{
    std::string
        name = vdb::enumerations::get_name(enumeration, value);
    int
        failure_counter = 0;

    std::cout << "name: " << name << std::endl;

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_enums(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_enums" << std::endl;
    std::cout << "==============================================" << std::endl;
    int
        failure_counter = 0;

    failure_counter += test_enum(
        ENUM_DOMAIN,
        vdb::DOMAIN_LAND);
    failure_counter += test_enum(
        ENUM_RADAR_TRACK,
        vdb::RADAR_TRACK_RADAR_TRACK_BROKEN);

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_entity_types(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_entity_types" << std::endl;
    std::cout << "==============================================" << std::endl;

    const uint16_t
        car[] = { 1,1,224,81,0,0,0 };
    uint64_t
        value = 0;
    vdb::entity_type_t
        type,
        parent,
        grandparent;
    int
        failure_counter = 0;

    vdb::entity_type_data::convert_to_value(1, 1, 225, 1, 1, 4, 0, value);

    type.set_value(value);

    ASSERT_TRUE(A1, type.get_value() == 72340035399123968L);
    ASSERT_TRUE(A2, type.str() == "1.1.225.1.1.4.0");

    type.set_value(value + 1);

    ASSERT_TRUE(A3, type.str() == "1.1.225.1.1.4.1");

    type.set_value(value);

    ASSERT_TRUE(A4, type.get_valid_parent(parent));
    ASSERT_TRUE(A5, parent.get_valid_parent(grandparent));

    std::cout << "type: " << type
              << std::endl
              << "type.name: " << type.get_name()
              << std::endl
              << "type.description: " << type.get_description()
              << std::endl
              << "parent: " << parent
              << std::endl
              << "parent.name: " << parent.get_name()
              << std::endl
              << "parent.description: " << parent.get_description()
              << std::endl
              << "grandparent: " << grandparent
              << std::endl
              << "grandparent.name: " << grandparent.get_name()
              << std::endl
              << "grandparent.description: " << grandparent.get_description()
              << std::endl;

    type = vdb::entity_type_t(car);

    ASSERT_FALSE(A6, type.get_valid_parent(parent));

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_object_types(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_object_types" << std::endl;
    std::cout << "==============================================" << std::endl;

    uint32_t
        value = 0;
    vdb::object_type_t
        type;
    int
        failure_counter = 0;

    vdb::object_type_data::convert_to_value(1, 8, 0, 0, value);

    type.set_value(value);

    std::cout << "type.description: " << type.get_description() << std::endl;

    ASSERT_TRUE(A1, type.get_geometry() == vdb::OBJECT_GEOMETRY_POINT);
    ASSERT_TRUE(A2, type.get_value() == 0x01080000);
    ASSERT_TRUE(A3, type.str() == "1.8.0.0");

    type.set_value(value + 1);

    ASSERT_TRUE(A4, type.str() == "1.8.0.1");

    return (failure_counter > 0) ? 1 : 0;
}

// ----------------------------------------------------------------------------
int test_timestamp(void)
{
    std::cout << "==============================================" << std::endl;
    std::cout << "test_timestamp" << std::endl;
    std::cout << "==============================================" << std::endl;

    uint32_t
        minutes;
    float32_t
        seconds;
    bool
        absolute;
    int
        failure_counter = 0;
    vdb::timestamp_t
        timestamp1,
        timestamp2;
    vdb::byte_stream
        buffer(32);

    timestamp1.set(30, 45.678f, true);
    timestamp1.get(minutes, seconds, absolute);

    ASSERT_TRUE(A1, minutes == 30);
    ASSERT_TRUE(A2, seconds == 45.678f);
    ASSERT_TRUE(A3, absolute);

    buffer.write(timestamp1);
    buffer.reset_index();
    buffer.read(timestamp2);

    timestamp1.get(minutes, seconds, absolute);

    ASSERT_TRUE(B1, minutes == 30);
    ASSERT_TRUE(B2, seconds == 45.678f);
    ASSERT_TRUE(B3, absolute);

    return (failure_counter > 0) ? 1 : 0;
}
