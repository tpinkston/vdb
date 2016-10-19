#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"

char
    logger::buffer[BUFFER_SIZE];
std::ostream
    &logger::err = std::cerr,
    &logger::out = std::cout;
std::map<logger::level_e, bool>
    logger::levels;
bool
    logger::console_logging_out = true,
    logger::console_logging_err = true;

// ----------------------------------------------------------------------------
void logger::initialize(void)
{
    if (levels.empty())
    {
        levels[ERROR] = true;
        levels[WARNING] = false;
        levels[VERBOSE] = false;
        levels[EXTRA_VERBOSE] = false;
    }
}

// ----------------------------------------------------------------------------
bool logger::is_enabled(level_e level)
{
    initialize();

    return ((level > 0) and (level < END)) ? levels[level] : false;
}

// ----------------------------------------------------------------------------
void logger::set_enabled(level_e level, bool value)
{
    initialize();

    if ((level > 0) and (level < END))
    {
        levels[level] = value;
    }
}

// ----------------------------------------------------------------------------
void logger::log(
    level_e level,
    const char *file,
    int line,
    const char *format,
    ...)
{
    if (is_enabled(level))
    {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        log_message(level, buffer, file, line);
    }
}

// ----------------------------------------------------------------------------
void logger::basename(std::string &file)
{
    std::string::size_type
        index = file.find_last_of('/', (file.length() - 1));

    if (index != std::string::npos)
    {
        file = file.substr((index + 1), file.length() - index);
    }
}

// ----------------------------------------------------------------------------
void logger::log_message(
    logger::level_e level,
    const char *message,
    const char *file,
    int line)
{
    std::ostream
        *stream_ptr = &out;
    bool
        console_logging = console_logging_out;

    if ((level == logger::ERROR) or (level == logger::WARNING))
    {
        stream_ptr = &err;
        console_logging = console_logging_err;
    }

    // For console logging only print file and line number on errors
    // or warnings, always when console logging is off.
    //
    if (not console_logging)
    {
        std::string
            filename(file);

        basename(filename);

        switch(level)
        {
            case ERROR:
                *stream_ptr << "ERROR";
                break;
            case WARNING:
                *stream_ptr << "WARNING";
                break;
            case VERBOSE:
                *stream_ptr << "VERBOSE";
                break;
            case EXTRA_VERBOSE:
                *stream_ptr << "EXTRA_VERBOSE";
                break;
            default:
                *stream_ptr << "DEFAULT";
                break;
        }

        *stream_ptr << "[" << filename << ":" << line << "]: ";
    }
    else if ((level == ERROR) or (level == WARNING))
    {
        std::string
            filename(file);

        basename(filename);

        if (level == ERROR)
        {
            *stream_ptr << color::bold_red << "ERROR" << color::none;
        }
        else if (level == WARNING)
        {
            *stream_ptr << color::bold_yellow << "WARNING" << color::none;
        }

        *stream_ptr << "[" << filename << ":" << line << "]: ";
    }

    *stream_ptr << message << std::endl;
}
