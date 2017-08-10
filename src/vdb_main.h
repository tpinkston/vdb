#ifndef VDB_MAIN_H
#define VDB_MAIN_H

namespace vdb
{
    // ------------------------------------------------------------------------
    int get_command(
        std::string argument,
        vdb::command_e &command,
        bool &help,
        bool &version,
        bool &parse);

    // ------------------------------------------------------------------------
    void print_version(void);

    // ------------------------------------------------------------------------
    void print_help(command_e);
};

#endif
