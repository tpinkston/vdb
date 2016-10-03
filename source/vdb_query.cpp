#include "vdb_file_readers.h"
#include "vdb_pdu_data.h"
#include "vdb_query.h"

#include "vdis_logger.h"
#include "vdis_services.h"
#include "vdis_string.h"

namespace
{
    const string_t
        separator = "----------------------------------------------------";

    // ------------------------------------------------------------------------
    void increment(std::map<uint8_t, uint32_t> &counters, uint8_t value)
    {
        if (counters.find(value) == counters.end())
        {
            counters[value] = 0;
        }

        counters[value] += 1;
    }
}

vdb::file_reader_t
    *vdb::query::reader_ptr = 0;
string_t
    vdb::query::filename,
    vdb::query::current_source;
uint64_t
    vdb::query::first_pdu_time = 0,
    vdb::query::last_pdu_time = 0;
vdb::source_data_node_t
    vdb::query::all_sources;
std::map<string_t, vdb::source_data_node_t>
    vdb::query::source_data;
std::map<vdis::id_t, vdb::entity_data_node_t>
    vdb::query::entity_data;
std::map<vdis::id_t, vdb::object_data_node_t>
    vdb::query::object_data;

// ----------------------------------------------------------------------------
bool vdb::designator_node_t::matches(const vdis::designator_pdu_t &pdu) const
{
    return (pdu.designated_id == target) && (pdu.code == code);
}

// ----------------------------------------------------------------------------
void vdb::designator_node_t::print(std::ostream &out) const
{
    std::map<uint8_t, uint32_t>::const_iterator
        itor;
    string_t
        name;

    out << "      Target: " << vdis::entity_marking(target) << std::endl
        << "      Code: " << (int)code << ", Count: " << (int)total_count
        << ", Power: " << vdis::to_string(minimum_power, 1, 2) << " min, "
        << vdis::to_string(maximum_power, 1, 2) << " max, "
        << vdis::to_string(average_power, 1, 2) << " average" << std::endl
        << "      Functions: " << std::endl;

    itor = functions.begin();

    while(itor != functions.end())
    {
        name = vdis::enumerations::get_name(ENUM_LASER_FUNCTION, itor->first);

        out << "        " << name << " (count: " << itor->second
            << ")" << std::endl;

        ++itor;
    }

    if (options::flag(OPT_VERBOSE))
    {
        out << "      Spot types: " << std::endl;

        itor = spot_types.begin();

        while(itor != spot_types.end())
        {
            name = vdis::enumerations::get_name(
                ENUM_DESIG_SPOT_TYPE,
                itor->first);

            out << "        " << name << " (count: " << itor->second
                << ")" << std::endl;

            ++itor;
        }

        out << "      System names: " << std::endl;

        itor = system_names.begin();

        while(itor != system_names.end())
        {
            name = vdis::enumerations::get_name(
                ENUM_DESIG_SYSTEM_NAME,
                itor->first);

            out << "        " << name << " (count: " << itor->second
                << ")" << std::endl;

            ++itor;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::collision_node_t::print(std::ostream &out) const
{
    out << "      Collision: " << event << " ("
        << vdis::enumerations::get_name(ENUM_COLLISION, type) << ")"
        << std::endl
        << "      Other entity: " << vdis::entity_marking(colliding_entity)
        << std::endl;

    if (options::flag(OPT_VERBOSE))
    {
        out << "      Location: " << location << std::endl
            << "      Velocity: " << velocity << " ("
            << vdis::to_string(velocity.length()) << " m/s)" << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::warfare_data_node_t::print(std::ostream &out) const
{
    string_t
        result_string = vdis::enumerations::get_name(
            ENUM_DETONATION_RESULT,
            result);

    if (vdis::starts_with(result_string, "DET_RESULT_"))
    {
        result_string = result_string.substr(11);
    }

    out << "      Event: " << event << std::endl;

    if (not target.is_none())
    {
        out << "      Target: " << vdis::entity_marking(target) << std::endl;
    }

    if (not munition.is_none())
    {
        out << "      Munition Entity: " << vdis::entity_marking(munition)
            << std::endl;
    }

    out << "      Munition: " << burst.munition << " '"
        << burst.munition.description() << "'" << std::endl;

    if (options::flag(OPT_VERBOSE))
    {
        out << "      Fire: ";

        if (fire_time > 0)
        {
            out << vdis::time_to_string(fire_time) << ", Detonation: ";
        }
        else
        {
            out << "N/A, Detonation: ";
        }

        if (detonation_time > 0)
        {
            out << vdis::time_to_string(detonation_time);
        }
        else
        {
            out << "N/A";
        }

        if ((fire_time > 0) and (detonation_time > 0))
        {
            out << ", Duration: " << (detonation_time - fire_time)
                   << " (ms)" << std::endl;

        }
        else
        {
            out << std::endl;
        }

        out << "      Parameters: " << burst.quantity << "/"
            << (vdis::warhead_burst_desc_e)burst.warhead << "/"
            << (vdis::fuse_burst_desc_e)burst.fuse << "/"
            << result_string << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::entity_data_node_t::print(std::ostream &out) const
{
    out << "  " << id << ": "
           << color::get(force_id) << marking << color::none << ", "
           << type << " '" << type.description()  << "'" << std::endl;

    if (options::flag(OPT_VERBOSE) and not source.empty())
    {
        out << "    Source: " << color::bold_cyan << source
               << color::none << std::endl;
    }

    if (query::print(OPT_COLLISIONS) and not collisions.empty())
    {
        out << "    Collisions(" << collisions.size() << "):" << std::endl;

        for(uint32_t i = 0; i < collisions.size(); ++i)
        {
            out << "      " << separator << std::endl;

            collisions[i].print(out);
        }

        out << "      " << separator << std::endl;
    }

    if (query::print(OPT_LASERS) and not designations.empty())
    {
        out << "    Designations(" << designations.size() << "):"
               << std::endl;

        for(uint32_t i = 0; i < designations.size(); ++i)
        {
            out << "      " << separator << std::endl;

            designations[i].print(out);
        }

        out << "      " << separator << std::endl;
    }

    if (query::print(OPT_FIRES) and not fires.empty())
    {
        std::map<vdis::id_t, warfare_data_node_t>::const_iterator
            fire_itor = fires.begin();

        out << "    Fires(" << fires.size() << "):" << std::endl;

        while(fire_itor != fires.end())
        {
            out << "      " << separator << std::endl;

            fire_itor->second.print(out);
            ++fire_itor;
        }

        out << "      " << separator << std::endl;
    }
}

// ----------------------------------------------------------------------------
void vdb::object_data_node_t::print(std::ostream &out) const
{

}

// ----------------------------------------------------------------------------
void vdb::source_data_node_t::print(std::ostream &out) const
{

}

// ----------------------------------------------------------------------------
int vdb::query::query_pdus(void)
{
    int
        result = 0;

    // File argument required
    //
    if (options::get_command_arguments().empty())
    {
        std::cerr << options::get_terminal_command()
                  << " query: missing file argument" << std::endl;

        result = 1;
    }
    else if (options::get_command_arguments().size() > 1)
    {
        std::cerr << options::get_terminal_command()
                  << " query: too many arguments" << std::endl;

        result = 1;
    }
    else
    {
        LOG_EXTRA_VERBOSE("Starting query...");

        filename = *options::get_command_argument(0);

        if (options::flag(OPT_PCAP))
        {
            reader_ptr = new pcap_reader_t(filename);
        }
        else
        {
            reader_ptr = new standard_reader_t(filename);
        }

        if (not reader_ptr->good())
        {
            result = 1;
        }
        else if (not reader_ptr->parse(process_pdu_data))
        {
            result = 1;
        }
        else
        {
            print_results(std::cout);
        }

        delete reader_ptr;
        reader_ptr = 0;
    }

    return result;
}

// ----------------------------------------------------------------------------
bool vdb::query::print(option_e o)
{
    switch(o)
    {
        case OPT_COLLISIONS:
        case OPT_EMISSIONS:
        case OPT_FIRES:
        case OPT_LASERS:
        case OPT_RADIOS:
        case OPT_OBJECTS:
            return (options::flag(o) || options::flag(OPT_ALL));
            break;
        default:
            LOG_ERROR("Invalid option: %d", o);
            break;
    }

    return false;
}

// ----------------------------------------------------------------------------
bool vdb::query::process_pdu_data(const pdu_data_t &data)
{
    const vdis::pdu_t *pdu_ptr = data.generate_pdu();

    if (pdu_ptr)
    {
        const vdis::pdu_type_e
            pdu_type = pdu_ptr->type();

        current_source = data.get_source();

        process_pdu(data, *pdu_ptr, all_sources);
        process_pdu(data, *pdu_ptr, source_data[current_source]);

        #define PDU_CONTENT(T, P) *static_cast<const vdis::T*>((P)->base())

        switch(pdu_type)
        {
            case vdis::PDU_TYPE_ENTITY_STATE:
                process(PDU_CONTENT(entity_state_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_FIRE:
                process(data, PDU_CONTENT(fire_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_DETONATION:
                process(data, PDU_CONTENT(detonation_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_COLLISION:
                process(PDU_CONTENT(collision_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_DESIGNATOR:
                process(PDU_CONTENT(designator_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_POINT_OBJECT_STATE:
                process(PDU_CONTENT(point_object_state_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_LINEAR_OBJECT_STATE:
                process(PDU_CONTENT(linear_object_state_pdu_t, pdu_ptr));
                break;
            case vdis::PDU_TYPE_AREAL_OBJECT_STATE:
                process(PDU_CONTENT(areal_object_state_pdu_t, pdu_ptr));
                break;
            default:
                break;
        }

        delete pdu_ptr;
        pdu_ptr = 0;
    }

    return true;
}

// ----------------------------------------------------------------------------
void vdb::query::process_pdu(
    const pdu_data_t &data,
    const vdis::pdu_t &pdu,
    source_data_node_t &source)
{
    const vdis::pdu_type_e pdu_type = pdu.type();

    source.total_pdus += 1;
    source.total_bytes += data.get_pdu_length();
    source.exercises.insert(data.get_pdu_exercise());

    if (source.pdu_types.find(pdu_type) == source.pdu_types.end())
    {
        source.pdu_types[pdu_type] = 1;
    }
    else
    {
        source.pdu_types[pdu_type]++;
    }
}

// ----------------------------------------------------------------------------
void vdb::query::process(const vdis::entity_state_pdu_t &pdu)
{
    entity_data_node_t
        &node = entity_data[pdu.id];

    node.id = pdu.id;
    node.marking = pdu.marking;
    node.force_id = (vdis::force_id_e)pdu.force;
    node.type = pdu.type;
    node.source = current_source;
}

// ----------------------------------------------------------------------------
void vdb::query::process(
    const pdu_data_t &data,
    const vdis::fire_pdu_t &pdu)
{
    entity_data_node_t
        &node = entity_data[pdu.shooter];
    warfare_data_node_t
        &warfare = node.fires[pdu.event];

    node.id = pdu.shooter;

    warfare.shooter = pdu.shooter;
    warfare.target = pdu.target;
    warfare.munition = pdu.munition;
    warfare.event = pdu.event;
    warfare.burst = pdu.burst_descriptor;
    warfare.fire_time = data.get_time();
}

// ----------------------------------------------------------------------------
void vdb::query::process(
    const pdu_data_t &data,
    const vdis::detonation_pdu_t &pdu)
{
    entity_data_node_t
        &node = entity_data[pdu.shooter];
    warfare_data_node_t
        &warfare = node.fires[pdu.event];

    node.id = pdu.shooter;

    warfare.shooter = pdu.shooter;
    warfare.target = pdu.target;
    warfare.munition = pdu.munition;
    warfare.event = pdu.event;
    warfare.burst = pdu.burst_descriptor;
    warfare.result = (vdis::detonation_result_e)pdu.result;
    warfare.detonation_time = data.get_time();
}

// ----------------------------------------------------------------------------
void vdb::query::process(const vdis::collision_pdu_t &pdu)
{
    entity_data_node_t
        &node = entity_data[pdu.issuing_entity];
    collision_node_t
        collision;

    node.id = pdu.issuing_entity;

    collision.colliding_entity = pdu.colliding_entity;
    collision.event = pdu.event;
    collision.type = (vdis::collision_e)pdu.collision_type;
    collision.velocity = pdu.velocity;
    collision.location = pdu.location;

    node.collisions.push_back(collision);
}

// ----------------------------------------------------------------------------
void vdb::query::process(const vdis::designator_pdu_t &pdu)
{
    entity_data_node_t
        &node = entity_data[pdu.designating_id];
    designator_node_t
        *designator_ptr = 0;
    uint32_t
        count = node.designations.size();

    node.id = pdu.designating_id;

    for(uint32_t i = 0; (i < count) and not designator_ptr; ++i)
    {
        if (node.designations[i].matches(pdu))
        {
            designator_ptr = &node.designations[i];
        }
    }

    if (not designator_ptr)
    {
        designator_node_t
            designator;

        designator.target = pdu.designated_id;
        designator.code = pdu.code;

        node.designations.push_back(designator);

        designator_ptr = &node.designations[count];
    }

    if (pdu.power > designator_ptr->maximum_power)
    {
        designator_ptr->maximum_power = pdu.power;
    }

    if (pdu.power < designator_ptr->minimum_power)
    {
        designator_ptr->minimum_power = pdu.power;
    }

    if (pdu.power != 0)
    {
        designator_ptr->average_power += pdu.power;

        if (designator_ptr->total_count > 0)
        {
            designator_ptr->average_power /= 2.0f;
        }
    }

    increment(designator_ptr->functions, pdu.function);
    increment(designator_ptr->spot_types, pdu.spot_type);
    increment(designator_ptr->system_names, pdu.system_name);

    designator_ptr->total_count++;
}

// ----------------------------------------------------------------------------
void vdb::query::process(const vdis::point_object_state_pdu_t &pdu)
{
    object_data_node_t
        &node = object_data[pdu.object_id];

    node.force_id = (vdis::force_id_e)pdu.force_id;
    node.type = pdu.object_type;
}

// ----------------------------------------------------------------------------
void vdb::query::process(const vdis::linear_object_state_pdu_t &pdu)
{
    object_data_node_t
        &node = object_data[pdu.object_id];

    node.force_id = (vdis::force_id_e)pdu.force_id;
    node.type = pdu.object_type;
}

// ----------------------------------------------------------------------------
void vdb::query::process(const vdis::areal_object_state_pdu_t &pdu)
{
    object_data_node_t
        &node = object_data[pdu.object_id];

    node.force_id = (vdis::force_id_e)pdu.force_id;
    node.type = pdu.object_type;
}

// ----------------------------------------------------------------------------
void vdb::query::print_results(std::ostream &out)
{
    struct stat
        file_stat;
    std::map<string_t, source_data_node_t>::const_iterator
        source_itor = source_data.begin();
    std::map<vdis::id_t, entity_data_node_t>::const_iterator
        entity_itor = entity_data.begin();
    std::map<vdis::id_t, object_data_node_t>::const_iterator
        object_itor = object_data.begin();

    out << "File name: " << filename << std::endl
        << "File size: " << std::fixed;

    if (stat(filename.c_str(), &file_stat) == 0)
    {
        out << file_stat.st_size << " bytes" << std::endl;
    }
    else
    {
        out << "[" << strerror(errno) << "]" << std::endl;
    }

    static_cast<standard_reader_t*>(reader_ptr)->header.print(
        string_t(),
        out);

    if (first_pdu_time > 0)
    {
        out << "First PDU time: "
            << vdis::time_to_string(first_pdu_time) << std::endl;
    }

    if (last_pdu_time > 0)
    {
        uint32_t
            hours = 0,
            minutes = 0,
            seconds = 0,
            milliseconds = 0;

        vdis::parse_time(
            (last_pdu_time - first_pdu_time),
            hours,
            minutes,
            seconds,
            milliseconds);

        out << "Last PDU time: "
            << vdis::time_to_string(last_pdu_time) << std::endl
            << "Duration: " << hours << "h " << minutes << "m "
            << seconds << "s " << milliseconds << "ms" << std::endl;
    }

    if (source_itor != source_data.end())
    {
        if (options::flag(OPT_VERBOSE))
        {
            while(source_itor != source_data.end())
            {
                out << separator << std::endl
                    << "Source: " << color::bold_cyan << source_itor->first
                    << color::none << std::endl;

                print_results(source_itor->second, out);
                ++source_itor;
            }

            out << separator << std::endl
                << "Source: " << color::bold_cyan << "ALL"
                << color::none << std::endl;
        }
        else
        {
            out << "Sources:" << std::endl << color::bold_cyan;

            while(source_itor != source_data.end())
            {
                out << "  " << source_itor->first << std::endl;
                ++source_itor;
            }

            out << color::none;
        }

        print_results(all_sources, out);

        if (options::flag(OPT_VERBOSE))
        {
            out << separator << std::endl;
        }
    }

    if (entity_itor != entity_data.end())
    {
        out << "Entities(" << entity_data.size() << "):" << std::endl;

        while(entity_itor != entity_data.end())
        {
            entity_itor->second.print(out);
            ++entity_itor;
        }
    }

    if (object_itor != object_data.end())
    {
        out << "Object count: " << object_data.size() << std::endl;

        while(object_itor != object_data.end())
        {
            const object_data_node_t &node = object_itor->second;

            out << "  " << object_itor->first << ": "
                << color::get(node.force_id) << node.type.description()
                << color::none << ", " << node.type << " "
                << (vdis::object_geometry_e)node.type.geometry()
                << std::endl;

            ++object_itor;
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::query::print_results(
    const source_data_node_t &source,
    std::ostream &out)
{
    std::set<uint8_t>::const_iterator
        exercise_itor = source.exercises.begin();
    std::map<vdis::pdu_type_e, uint32_t>::const_iterator
        type_itor = source.pdu_types.begin();

    if (exercise_itor != source.exercises.end())
    {
        out << "Exercise(s): ";

        while(exercise_itor != source.exercises.end())
        {
            out << (int)*exercise_itor;

            ++exercise_itor;

            if (exercise_itor != source.exercises.end())
            {
                out << ", ";
            }
        }
    }

    out << std::endl
        << "PDUs (" << source.total_pdus
        << " total, " << source.total_bytes << " bytes):" << std::endl;

    while(type_itor != source.pdu_types.end())
    {
        out << "  " << color::yellow << type_itor->first
               << color::none << "[" << (int)type_itor->first
               << "]: " << type_itor->second << std::endl;

        ++type_itor;
    }
}
