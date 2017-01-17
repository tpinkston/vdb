#include "vdis_byte_stream.h"
#include "vdis_entity_types.h"
#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"
#include "vdis_test.h"

#include <fstream>
#include <iostream>

namespace vdis
{
    std::ostream
        &test::err = std::cerr,
        &test::out = std::cout;
    uint32_t
        test::failures = 0;
}

// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    return vdis::test::run(argc, argv);
}

// ----------------------------------------------------------------------------
void vdis::test::assert(
    float test,
    const char *expression,
    bool result,
    bool expectation)
{
    out << color::bold_blue << to_string(test, 2, 2) << color::none << ": ";

    if (result == expectation)
    {
        out << color::bold_green << "SUCCESS ";
    }
    else
    {
        failures++;
        out << color::bold_red   << " FAILED ";
    }

    out << color::none << (result ? " true" : "false") << ": "
        << expression << std::endl;
}

// ----------------------------------------------------------------------------
int vdis::test::run(int argc, char *argv[])
{
    //logger::set_enabled(logger::EXTRA_VERBOSE, true);
    logger::set_enabled(logger::VERBOSE, true);
    logger::set_enabled(logger::WARNING, true);
    logger::set_enabled(logger::ERROR, true);

    LOG_EXTRA_VERBOSE("Extra verbose on...");
    LOG_VERBOSE("Verbose on...");
    LOG_WARNING("Warnings on...");
    LOG_ERROR("Errors on...");

    set_byteswapping();
    enumerations::load();
    entity_types::load();

    if (argc == 1)
    {
        out << "Try one of the following:" << std::endl
            << "  vdis_test all" << std::endl
            << "  vdis_test byte_buffer" << std::endl
            << "  vdis_test byte_stream" << std::endl
            << "  vdis_test common" << std::endl
            << "  vdis_test entity_types" << std::endl
            << "  vdis_test object_types" << std::endl
            << "  vdis_test pdus" << std::endl
            << "  vdis_test services" << std::endl;
    }
    else if (argc > 2)
    {
        err << "vdis_test: too many arguments" << std::endl;
    }
    else
    {
        std::string
            test_name(argv[1]);
        bool
            tested = true;

        if (test_name == "byte_buffer")
        {
            test_byte_buffer();
        }
        else if (test_name == "byte_stream")
        {
            test_byte_stream();
        }
        else if (test_name == "common")
        {
            test_common();
        }
        else if (test_name == "entity_types")
        {
            test_entity_types();
        }
        else if (test_name == "object_types")
        {
            test_object_types();
        }
        else if (test_name == "pdus")
        {
            test_pdus();
        }
        else if (test_name == "services")
        {
            test_services();
        }
        else if (test_name == "all")
        {
            test_byte_buffer();
            test_byte_stream();
            test_common();
            test_services();
            test_entity_types();
            test_object_types();
            test_pdus();
        }
        else
        {
            out << "Unknown test name: " << test_name << std::endl;
        }

        if (tested)
        {
            out << "==============================================" << std::endl
                << "Test complete (" << failures << " FAILURES)" << std::endl
                << "==============================================" << std::endl;
        }
    }

    return failures;
}

// ----------------------------------------------------------------------------
bool vdis::test::import_pdu(const std::string &filename, byte_stream_t &stream)
{
    const std::string
        path = ("data/pdus/" + filename);
    std::ifstream
        input;
    bool
        success = false;

    input.open(path.c_str(), std::ios::in | std::ios::binary);

    if (input.fail())
    {
        LOG_ERROR("Failed to open file: %s", path.c_str());
    }
    else
    {
        std::streamsize
            buffer_size = 0;

        // Go to EOF to get the length.
        //
        input.seekg(0, std::ios::end);

        buffer_size = input.tellg();

        if (input.fail())
        {
            LOG_ERROR("Failed to get file size: %s", path.c_str());
        }
        else
        {
            uint8_t
                buffer[buffer_size];

            // Go back to the beginning and read the entire file.
            //
            input.seekg(0, std::ios::beg);
            input.read((char *)buffer, buffer_size);

            if (input.fail())
            {
                LOG_ERROR("Failed to read file: %s", path.c_str());
            }
            else
            {
                std::streamsize
                    bytes_read = input.gcount();

                stream.reset(buffer, bytes_read);

                input.close();

                success = true;

                LOG_VERBOSE(
                    "%d bytes read from '%s'",
                    bytes_read,
                    filename.c_str());
            }
        }
    }

    return success;
}
