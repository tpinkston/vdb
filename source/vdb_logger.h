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

#ifndef VDB_LOGGER_H
#define VDB_LOGGER_H

#define LOG_EXTRA_VERBOSE(format, ...) vdb::logger::extra_verbose(format, ##__VA_ARGS__);
#define LOG_VERBOSE(format, ...) vdb::logger::verbose(format, ##__VA_ARGS__);
#define LOG_WARNING(format, ...) vdb::logger::warning(__FILE__, __LINE__, format, ##__VA_ARGS__);
#define LOG_ERROR(format, ...) vdb::logger::error(__FILE__, __LINE__, format, ##__VA_ARGS__);

namespace vdb
{
    class logger
    {
      public:

        static void extra_verbose(const char *format, ...);

        static void verbose(const char *format, ...);

        static void warning(
            const char *file,
            int line,
            const char *format,
            ...
        );

        static void error(
            const char *file,
            int line,
            const char *format,
            ...
        );
    };
}

#endif
