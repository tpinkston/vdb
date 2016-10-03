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
logger
    *logger::instance_ptr = 0;

namespace
{
    logger
        *anonymous_instance_ptr = logger::get_instance();
}

// ----------------------------------------------------------------------------
logger::logger(void)
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
    return ((level > 0) and (level < END)) ? levels[level] : false;
}

// ----------------------------------------------------------------------------
void logger::set_enabled(level_e level, bool value)
{
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
logger *logger::get_instance(void)
{
    if (not instance_ptr)
    {
        instance_ptr = new logger();
    }

    return instance_ptr;
}

// ----------------------------------------------------------------------------
void logger::basename(string_t &file)
{
    string_t::size_type
        index = file.find_last_of('/', (file.length() - 1));

    if (index != string_t::npos)
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
        &stream = (level == logger::ERROR) ? out : err;

    // Print file and line number only on error level
    //
    if (level == ERROR)
    {
        string_t
            filename(file);

        basename(filename);

        stream << color::bold_red << "ERROR" << color::none
               << "[" << filename << ":" << line << "]: "
               << message << std::endl;
    }
    else if (level == WARNING)
    {
        stream << color::bold_yellow << message << color::none << std::endl;
    }
    else
    {
        stream << message << std::endl;
    }
}
