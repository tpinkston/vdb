#include "vdb_scan.h"

#include "vdis_string.h"

bool
    vdb::scan::scanning = false,
    vdb::scan::associations = false,
    vdb::scan::lasers = false,
    vdb::scan::fires = false,
    vdb::scan::collisions = false,
    vdb::scan::entities = false,
    vdb::scan::objects = false;
std::ostream
    &vdb::scan::err = std::cerr,
    &vdb::scan::out = std::cout;

// ----------------------------------------------------------------------------
void vdb::scan::scan_all(void)
{
    scanning = true;
    associations = true;
    lasers = true;
    fires = true;
    collisions = true;
    entities = true;
    objects = true;
}

// ----------------------------------------------------------------------------
bool vdb::scan::parse(const std::string &value)
{
    std::vector<std::string>
        values;
    bool
        success = false;

    if (vdis::tokenize_csv(vdis::trim(value), values) > 0)
    {
        success = true;

        for(uint32_t i = 0; success and (i < values.size()); ++i)
        {
            std::string item = vdis::trim(values[i]);

            if (item == "associations")
            {
                scanning = true;
                associations = true;
            }
            else if (item == "lasers")
            {
                scanning = true;
                lasers = true;
            }
            else if (item == "fires")
            {
                scanning = true;
                fires = true;
            }
            else if (item == "collisions")
            {
                scanning = true;
                collisions = true;
            }
            else if (item == "entities")
            {
                scanning = true;
                entities = true;
            }
            else if (item == "objects")
            {
                scanning = true;
                objects = true;
            }
            else
            {
                success = false;
            }
        }
    }

    return success;
}
