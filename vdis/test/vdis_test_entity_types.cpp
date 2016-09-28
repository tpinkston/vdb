#include "vdis_data_types.h"
#include "vdis_entity_types.h"
#include "vdis_string.h"
#include "vdis_test.h"

// ----------------------------------------------------------------------------
void vdis::test::test_entity_types(void)
{
    uint64_t
        value = 0;
    entity_type_t
        type,
        parent,
        grandparent;

    out << "==============================================" << std::endl;
    out << "vdis::test::test_entity_types" << std::endl;
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
