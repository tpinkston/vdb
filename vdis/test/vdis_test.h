#ifndef VDIS_TEST_H
#define VDIS_TEST_H

#include <iostream>

#define TEST_TRUE(t,e) vdis::test::assert(t, #e, e, true)
#define TEST_FALSE(t,e) vdis::test::assert(t, #e, e, false)

namespace vdis
{
    class byte_stream_t;

    class test
    {
      public:

        static int run(int argc, char *argv[]);

      private:

        static void test_byte_buffer(void);
        static void test_byte_stream(void);
        static void test_common(void);
        static void test_entity_types(void);
        static void test_object_types(void);
        static void test_pdus(void);
        static void test_services(void);

        static bool import_pdu(
            const std::string &filename,
            byte_stream_t &stream
        );

        static void assert(
            float test,
            const char *expression,
            bool result,
            bool expectation
        );

        static std::ostream
            &err,
            &out;
        static uint32_t
            failures;
    };
}

#endif
