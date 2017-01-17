#ifndef VDIS_LOGGER_H
#define VDIS_LOGGER_H

#include "vdis_system.h"

#define CONSOLE_OUT(format, ...)        logger::console_out(__FILE__, __LINE__, format, ##__VA_ARGS__)
#define CONSOLE_ERR(format, ...)        logger::console_err(__FILE__, __LINE__, format, ##__VA_ARGS__)

#define LOG_EXTRA_VERBOSE(format, ...)  logger::log(logger::EXTRA_VERBOSE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_VERBOSE(format, ...)        logger::log(logger::VERBOSE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...)        logger::log(logger::WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...)          logger::log(logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...)          logger::log(logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)

class logger
{
  public:

    enum level_e
    {
        BEGIN = 0,
        FATAL = 1,
        ERROR = 2,
        WARNING = 3,
        VERBOSE = 4,
        EXTRA_VERBOSE = 5,
        END = 6
    };

    static bool is_valid(level_e level);
    static bool is_enabled(level_e level);
    static void set_enabled(level_e level, bool value);

    static void log(
        level_e level,
        const char *file,
        int line,
        const char *format,
        ...
    );

    static void console_out(
        const char *file,
        int line,
        const char *format,
        ...
    );

    static void console_err(
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
