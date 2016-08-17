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

#ifndef VDB_COLOR_H
#define VDB_COLOR_H

#include "vdb_common.h"
#include "vdb_options.h"

namespace color
{
    // ------------------------------------------------------------------------
    // Color definitions for TTY output

    typedef enum
    {
        none = 0,
        red = 31,
        green = 32,
        yellow = 33,
        blue = 34,
        purple = 35,
        cyan = 36,
        white = 37
    } color_e;

    typedef enum
    {
        bold_red = 31,
        bold_green = 32,
        bold_yellow = 33,
        bold_blue = 34,
        bold_purple = 35,
        bold_cyan = 36,
        bold_white = 37
    } bold_color_e;
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(std::ostream &stream, color::color_e c)
{
    if (isatty(STDOUT_FILENO) and not vdb::options::flag(vdb::OPT_COLOR_OFF))
    {
        return (stream << "\e[" << (int)c << "m");
    }
    else
    {
        return stream;
    }
}

// ----------------------------------------------------------------------------
inline std::ostream &operator<<(std::ostream &stream, color::bold_color_e c)
{
    if (isatty(STDOUT_FILENO) and not vdb::options::flag(vdb::OPT_COLOR_OFF))
    {
        return (stream << "\e[;1;" << (int)c << "m");
    }
    else
    {
        return stream;
    }
}

#endif
