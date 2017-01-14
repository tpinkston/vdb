#ifndef VDIS_LOGGER_H
#define VDIS_LOGGER_H

#include "vdis_system.h"

#define LOG_EXTRA_VERBOSE(format, ...)  logger::log(logger::EXTRA_VERBOSE, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define LOG_VERBOSE(format, ...)        logger::log(logger::VERBOSE, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define LOG_WARNING(format, ...)        logger::log(logger::WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define LOG_ERROR(format, ...)          logger::log(logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define LOG_FATAL(format, ...)          logger::log(logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

class logger
{
  public:

    enum level_e
    {
        FATAL = 0,
        ERROR = 1,
        WARNING = 2,
        VERBOSE = 3,
        EXTRA_VERBOSE = 4,
        END = 5
    };

    static bool is_enabled(level_e level);
    static void set_enabled(level_e level, bool value);

    static void log(
        level_e level,
        const char *file,
        int line,
        const char *format,
        ...
    );

    static std::ostream &serr(void) { return err; }
    static std::ostream &sout(void) { return out; }

  private:

    static void initialize(void);

    static void basename(std::string &file);

    static void log_message(
        level_e level,
        const char *message,
        const char *file,
        int line
    );

    static const int
        BUFFER_SIZE = 2048;
    static char
        buffer[BUFFER_SIZE];
    static std::ostream
        &err,
        &out;
    static std::map<level_e, bool>
        levels;
    static bool
        console_logging_out,
        console_logging_err;
};

#endif
