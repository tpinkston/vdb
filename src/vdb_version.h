#ifndef VDB_VERSION_H
#define VDB_VERSION_H

#include "vdb_git.h"

#define VDB_VERSION_MAJOR 2
#define VDB_VERSION_MINOR 0

// ----------------------------------------------------------------------------
inline void print_vdb_version(void)
{
    std::cout << "vdb (V-DIS Debugger) Version "
              << (int)VDB_VERSION_MAJOR << "." << (int)VDB_VERSION_MINOR
              << " [" << VDB_GIT_BRANCH << "-" << VDB_GIT_COMMIT
              << " " << __DATE__ << "]" << std::endl;
}

#endif
