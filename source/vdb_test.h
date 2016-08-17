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

#ifndef TEST_H
#define TEST_H

#include "vdb_command.h"
#include "vdb_common.h"
#include "vdb_color.h"

#define ASSERT_TRUE(t,e) assert(#t, #e, e, true, failure_counter)
#define ASSERT_FALSE(t,e) assert(#t, #e, e, false, failure_counter)

namespace vdb
{
    // ------------------------------------------------------------------------
    class test : public command
    {
      public:

        test(void);
        virtual ~test(void);

        virtual int run(void);

        void test_common(void);
        void test_vdis_types(void);

      protected:

        uint32_t
            failure_counter;
    };
}

// ----------------------------------------------------------------------------
inline vdb::test::test(void) : failure_counter(0)
{

}

// ----------------------------------------------------------------------------
inline vdb::test::~test(void)
{

}

// ----------------------------------------------------------------------------
inline void assert(
    const char *index,
    const char *expression,
    bool result,
    bool expectation,
    int &failure_counter)
{
    std::cout << color::bold_blue << index;
    std::cout << color::none << ": ";

    if (result == expectation)
    {
        std::cout << color::bold_green << "SUCCESS";
    }
    else
    {
        failure_counter++;
        std::cout << color::bold_red << "FAILED";
    }

    std::cout << color::none << "[" << (result ? "true" : "false") << "]: "
              << expression << std::endl;
}

#endif
