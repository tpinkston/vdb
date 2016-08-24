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

#include "vdb_logger.h"
#include "vdb_test.h"

uint32_t
    vdb::test::failure_counter = 0;

// ----------------------------------------------------------------------------
int vdb::test::run(void)
{
    bool
        show_results = true;

    if (not options::get_command_argument(0))
    {
        LOG_VERBOSE("No test name specified, running all tests...");

        test_common();
        test_vdis_types();
    }
    else
    {
        const std::string
            name = *options::get_command_argument(0);

        LOG_VERBOSE("Test name is %s...", name.c_str());

        if (name == "common")
        {
            test_common();
        }
        else if (name == "vdis_types")
        {
            test_vdis_types();
        }
        else
        {
            std::cerr << options::get_terminal_command()
                      << " test: invalid test name '"
                      << name << "'" << std::endl;

            show_results = false;
        }
    }

    if (show_results)
    {
        if (failure_counter == 0)
        {
            std::cout << std::endl
                      << "TESTING SUCCESS" << std::endl;
        }
        else
        {
            std::cout << std::endl
                      << "TESTING FAILED(" << failure_counter << " failures)"
                      << std::endl;
        }
    }

    return failure_counter;
}

