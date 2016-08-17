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

#include "vdb_color.h"
#include "vdb_logger.h"
#include "vdb_options.h"
#include "vdb_string.h"

namespace
{
    const char
        *ERROR = "ERROR",
        *WARNING = "WARNING";
    const int
        BUFFER_SIZE = 2048;
    char
        buffer[BUFFER_SIZE];

    // ------------------------------------------------------------------------
    void basename(std::string &file)
    {
        std::string::size_type
            index = file.find_last_of('/', (file.length() - 1));

        if (index != std::string::npos)
        {
            file = file.substr((index + 1), file.length() - index);
        }
    }

    // ------------------------------------------------------------------------
    void log_message(
        const char *prefix,
        const char *message,
        const char *file,
        int line)
    {
        std::ostream
            &stream = (prefix == ERROR) ? std::cout : std::cerr;
        std::string
            filename(file);
        color::bold_color_e
            color = (prefix == ERROR) ? color::bold_red : color::bold_yellow;

        basename(filename);

        stream << color << prefix << color::none;
        stream << color::none << "[";
        stream << color << filename;
        stream << color::none << ":";
        stream << color << line;
        stream << color::none << "]: " << message << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::logger::verbose(const char *format, ...)
{
    if (options::flag(OPT_VERBOSE))
    {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        std::cout << color::none << buffer << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::logger::extra_verbose(const char *format, ...)
{
    if (options::flag(OPT_EXTRA_VERBOSE))
    {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        std::cout << color::none << buffer << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::logger::warning(
    const char *file,
    int line,
    const char *format,
    ...)
{
    if (options::flag(OPT_WARNINGS))
    {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        log_message(WARNING, buffer, file, line);
    }
}

// ----------------------------------------------------------------------------
void vdb::logger::error(
    const char *file,
    int line,
    const char *format,
    ...)
{
    if (not options::flag(OPT_ERRORS_OFF))
    {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        log_message(ERROR, buffer, file, line);
    }
}
