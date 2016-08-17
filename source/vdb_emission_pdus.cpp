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

#include "vdb_binary.h"
#include "vdb_byte_stream.h"
#include "vdb_emission_pdus.h"
#include "vdb_enums.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
vdb::emitter_target_t::emitter_target_t(void) : emitter(0), beam(0)
{

}

// ----------------------------------------------------------------------------
vdb::emitter_target_t::emitter_target_t(const emitter_target_t &copy) :
    entity(copy.entity),
    emitter(copy.emitter),
    beam(copy.beam)
{

}

// ----------------------------------------------------------------------------
vdb::emitter_target_t::~emitter_target_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::emitter_target_t::clear(void)
{
    entity.clear();
    emitter = 0;
    beam = 0;
}

// ----------------------------------------------------------------------------
void vdb::emitter_target_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "emitter_target.entity ";

    entity.print_full(stream);

    stream << std::endl
           << prefix << "emitter_target.emitter " << (int)emitter << std::endl
           << prefix << "emitter_target.beam " << (int)beam << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::emitter_target_t::read(byte_stream &stream)
{
    clear();

    stream.read(entity);
    stream.read(emitter);
    stream.read(beam);
}

// ----------------------------------------------------------------------------
void vdb::emitter_target_t::write(byte_stream &stream) const
{
    stream.write(entity);
    stream.write(emitter);
    stream.write(beam);
}

// ----------------------------------------------------------------------------
vdb::emitter_beam_t::emitter_beam_t(void) :
    data_length(0),
    id_number(0),
    parameter_index(0),
    frequency(0),
    frequency_range(0),
    effective_radiated_power(0),
    pulse_repetition_frequency(0),
    pulse_width(0),
    azimuth_center(0),
    azimuth_sweep(0),
    elevation_center(0),
    elevation_sweep(0),
    sweep_sync(0),
    function(0),
    hd_track_jam(0),
    status(0),
    jamming_technique(0)
{

}

// ----------------------------------------------------------------------------
vdb::emitter_beam_t::emitter_beam_t(const emitter_beam_t &copy) :
    data_length(copy.data_length),
    id_number(copy.id_number),
    parameter_index(copy.parameter_index),
    frequency(copy.frequency),
    frequency_range(copy.frequency_range),
    effective_radiated_power(copy.effective_radiated_power),
    pulse_repetition_frequency(copy.pulse_repetition_frequency),
    pulse_width(copy.pulse_width),
    azimuth_center(copy.azimuth_center),
    azimuth_sweep(copy.azimuth_sweep),
    elevation_center(copy.elevation_center),
    elevation_sweep(copy.elevation_sweep),
    sweep_sync(copy.sweep_sync),
    function(copy.function),
    hd_track_jam(copy.hd_track_jam),
    status(copy.status),
    jamming_technique(copy.jamming_technique),
    targets(copy.targets)
{

}

// ----------------------------------------------------------------------------
vdb::emitter_beam_t::~emitter_beam_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::emitter_beam_t::clear(void)
{
    data_length = 0;
    id_number = 0;
    parameter_index = 0;
    frequency = 0;
    frequency_range = 0;
    effective_radiated_power = 0;
    pulse_repetition_frequency = 0;
    pulse_width = 0;
    azimuth_center = 0;
    azimuth_sweep = 0;
    elevation_center = 0;
    elevation_sweep = 0;
    sweep_sync = 0;
    function = 0;
    hd_track_jam = 0;
    status = 0;
    jamming_technique = 0;
    targets.clear();
}

// ----------------------------------------------------------------------------
void vdb::emitter_beam_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "emitter_beam.data_length "
           << (int)data_length << std::endl
           << prefix << "emitter_beam.id_number "
           << (int)id_number << std::endl
           << prefix << "emitter_beam.parameter_index "
           << (int)parameter_index << std::endl
           << prefix << "emitter_beam.frequency(Hz) "
           << float_to_string(frequency) << std::endl
           << prefix << "emitter_beam.frequency_range(Hz) "
           << float_to_string(frequency_range) << std::endl
           << prefix << "emitter_beam.effective_radiated_power(dBm) "
           << float_to_string(effective_radiated_power) << std::endl
           << prefix << "emitter_beam.pulse_repetition_frequency "
           << float_to_string(pulse_repetition_frequency) << std::endl
           << prefix << "emitter_beam.pulse_width "
           << float_to_string(pulse_width) << std::endl
           << prefix << "emitter_beam.azimuth_center "
           << float_to_string(azimuth_center) << std::endl
           << prefix << "emitter_beam.azimuth_sweep "
           << float_to_string(azimuth_sweep) << std::endl
           << prefix << "emitter_beam.elevation_center "
           << float_to_string(elevation_center) << std::endl
           << prefix << "emitter_beam.elevation_sweep "
           << float_to_string(elevation_sweep) << std::endl
           << prefix << "emitter_beam.sweep_sync "
           << float_to_string(sweep_sync) << std::endl
           << prefix << "emitter_beam.function " << (emitter_function_e)function
           << " [" << (int)function << "]" << std::endl
           << prefix << "emitter_beam.hd_track_jam " << (yes_no_e)hd_track_jam
           << " [" << (int)hd_track_jam << "]" << std::endl
           << prefix << "emitter_beam.status "
           << (int)status << std::endl
           << prefix << "emitter_beam.jamming_technique "
           << (jamming_technique_e)jamming_technique
           << " [" << (int)jamming_technique << "]" << std::endl
           << prefix << "emitter_beam.targets " << targets.size()
           << std::endl;

    for(uint32_t i = 0; i < targets.size(); ++i)
    {
        targets[i].print(
            (prefix + "emitter_beam.targets[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::emitter_beam_t::write(byte_stream &stream) const
{
    const uint8_t
        target_count = targets.size();

    stream.write(data_length);
    stream.write(id_number);
    stream.write(parameter_index);
    stream.write(frequency);
    stream.write(frequency_range);
    stream.write(effective_radiated_power);
    stream.write(pulse_repetition_frequency);
    stream.write(pulse_width);
    stream.write(azimuth_center);
    stream.write(azimuth_sweep);
    stream.write(elevation_center);
    stream.write(elevation_sweep);
    stream.write(sweep_sync);
    stream.write(function);
    stream.write(target_count);
    stream.write(hd_track_jam);
    stream.write(status);
    stream.write(jamming_technique);

    for(uint8_t i = 0; i < target_count; ++i)
    {
        targets[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::emitter_beam_t::length(void) const
{
    uint32_t
        record_length = BASE_LENGTH;

    for(uint32_t i = 0; i < targets.size(); ++i)
    {
        record_length += targets[i].length();
    }

    return record_length;
}

// ----------------------------------------------------------------------------
void vdb::emitter_beam_t::read(byte_stream &stream)
{
    uint8_t
        target_count = 0;

    clear();

    stream.read(data_length);
    stream.read(id_number);
    stream.read(parameter_index);
    stream.read(frequency);
    stream.read(frequency_range);
    stream.read(effective_radiated_power);
    stream.read(pulse_repetition_frequency);
    stream.read(pulse_width);
    stream.read(azimuth_center);
    stream.read(azimuth_sweep);
    stream.read(elevation_center);
    stream.read(elevation_sweep);
    stream.read(sweep_sync);
    stream.read(function);
    stream.read(target_count);
    stream.read(hd_track_jam);
    stream.read(status);
    stream.read(jamming_technique);

    for(uint8_t i = 0; i < target_count; ++i)
    {
        emitter_target_t
            target;

        target.read(stream);

        targets.push_back(target);
    }
}

// ----------------------------------------------------------------------------
vdb::emitter_system_t::emitter_system_t(void) :
    data_length(0),
    padding(0),
    name(0),
    function(0),
    number(0)
{

}

// ----------------------------------------------------------------------------
vdb::emitter_system_t::emitter_system_t(const emitter_system_t &copy) :
    data_length(copy.data_length),
    padding(copy.padding),
    name(copy.name),
    function(copy.function),
    number(copy.number),
    location(copy.location),
    beams(copy.beams)
{

}

// ----------------------------------------------------------------------------
vdb::emitter_system_t::~emitter_system_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::emitter_system_t::clear(void)
{
    data_length = 0;
    padding = 0;
    name = 0;
    function = 0;
    number = 0;
    location.clear();
    beams.clear();
}

// ----------------------------------------------------------------------------
void vdb::emitter_system_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "emitter_system.name "
           << enumerations::print(ENUM_EMITTER_NAME, name) << std::endl
           << prefix << "emitter_system.function "
           << enumerations::print(ENUM_EMITTER_FUNCTION, function) << std::endl
           << prefix << "emitter_system.number "
           << (int)number << std::endl
           << prefix << "emitter_system.data_length "
           << (int)data_length << std::endl
           << prefix << "emitter_system.padding "
           << binary::str(padding, true) << std::endl
           << prefix << "emitter_system.location "
           << location << std::endl
           << prefix << "emitter_system.beams " << beams.size()
           << std::endl;

    for(uint32_t i = 0; i < beams.size(); ++i)
    {
        beams[i].print(
            (prefix + "emitter_system.beams[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::emitter_system_t::write(byte_stream &stream) const
{
    const uint8_t
        beam_count = beams.size();

    stream.write(data_length);
    stream.write(beam_count);
    stream.write(padding);
    stream.write(name);
    stream.write(function);
    stream.write(number);
    stream.write(location);

    for(uint8_t i = 0; i < beam_count; ++i)
    {
        beams[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::emitter_system_t::length(void) const
{
    uint32_t
        record_length = BASE_LENGTH;

    for(uint32_t i = 0; i < beams.size(); ++i)
    {
        record_length += beams[i].length();
    }

    return record_length;
}

// ----------------------------------------------------------------------------
void vdb::emitter_system_t::read(byte_stream &stream)
{
    uint8_t
        beam_count = 0;

    stream.read(data_length);
    stream.read(beam_count);
    stream.read(padding);
    stream.read(name);
    stream.read(function);
    stream.read(number);
    stream.read(location);

    for(uint8_t i = 0; i < beam_count; ++i)
    {
        emitter_beam_t
            beam;

        beam.read(stream);

        beams.push_back(beam);
    }
}

// ----------------------------------------------------------------------------
vdb::iff_system_id_t::iff_system_id_t(void) :
    type(0),
    name(0),
    mode(0),
    options(0)
{

}

// ----------------------------------------------------------------------------
vdb::iff_system_id_t::iff_system_id_t(const iff_system_id_t &copy) :
    type(copy.type),
    name(copy.name),
    mode(copy.mode),
    options(copy.options)
{

}

// ----------------------------------------------------------------------------
vdb::iff_system_id_t::~iff_system_id_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::iff_system_id_t::clear(void)
{
    type = 0;
    name = 0;
    mode = 0;
    options = 0;
}

// ----------------------------------------------------------------------------
void vdb::iff_system_id_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "system_id.type "
           << enumerations::print(ENUM_IFF_SYSTEM_TYPE, type) << std::endl
           << prefix << "system_id.name "
           << enumerations::print(ENUM_IFF_SYSTEM_NAME, name) << std::endl
           << prefix << "system_id.mode "
           << enumerations::print(ENUM_IFF_SYSTEM_MODE, mode) << std::endl
           << prefix << "system_id.options "
           << binary::str(options) << std::endl
           << prefix << "system_id.options.change_indicator "
           << (yes_no_e)binary::get_1_bit_8(0, options) << std::endl
           << prefix << "system_id.options.system_specific_field_1 "
           << (yes_no_e)binary::get_1_bit_8(1, options) << std::endl
           << prefix << "system_id.options.system_specific_field_2 "
           << (yes_no_e)binary::get_1_bit_8(2, options) << std::endl
           << prefix << "system_id.options.heartbeat_indicator "
           << (yes_no_e)binary::get_1_bit_8(3, options) << std::endl
           << prefix << "system_id.options.transponder_interrogator_indicator "
           << (yes_no_e)binary::get_1_bit_8(4, options) << std::endl
           << prefix << "system_id.options.simulation_mode "
           << (yes_no_e)binary::get_1_bit_8(5, options) << std::endl
           << prefix << "system_id.options.test_mode "
           << (yes_no_e)binary::get_1_bit_8(7, options) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::iff_system_id_t::write(byte_stream &stream) const
{
    stream.write(type);
    stream.write(name);
    stream.write(mode);
    stream.write(options);
}

// ----------------------------------------------------------------------------
void vdb::iff_system_id_t::read(byte_stream &stream)
{
    stream.read(type);
    stream.read(name);
    stream.read(mode);
    stream.read(options);
}

// ----------------------------------------------------------------------------
vdb::iff_operational_data_t::iff_operational_data_t(void) :
    status(0),
    data_field_1(0),
    information_layers(0),
    data_field_2(0),
    parameter_1(0),
    parameter_2(0),
    parameter_3(0),
    parameter_4(0),
    parameter_5(0),
    parameter_6(0)
{

}

// ----------------------------------------------------------------------------
vdb::iff_operational_data_t::iff_operational_data_t(
    const iff_operational_data_t &copy
) :
    status(copy.status),
    data_field_1(copy.data_field_1),
    information_layers(copy.information_layers),
    data_field_2(copy.data_field_2),
    parameter_1(copy.parameter_1),
    parameter_2(copy.parameter_2),
    parameter_3(copy.parameter_3),
    parameter_4(copy.parameter_4),
    parameter_5(copy.parameter_5),
    parameter_6(copy.parameter_6)
{

}

// ----------------------------------------------------------------------------
vdb::iff_operational_data_t::~iff_operational_data_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::iff_operational_data_t::clear(void)
{
    status = 0;
    data_field_1 = 0;
    information_layers = 0;
    data_field_2 = 0;
    parameter_1 = 0;
    parameter_2 = 0;
    parameter_3 = 0;
    parameter_4 = 0;
    parameter_5 = 0;
    parameter_6 = 0;
}

// ----------------------------------------------------------------------------
void vdb::iff_operational_data_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "operational_data.status "
           << binary::str(status) << std::endl
           << prefix << "operational_data.options.status.system "
           << (on_off_e)binary::get_1_bit_8(0, status) << std::endl
           << prefix << "operational_data.options.status.parameter_1_capable "
           << (yes_no_e)binary::get_1_bit_8(1, status) << std::endl
           << prefix << "operational_data.options.status.parameter_2_capable "
           << (yes_no_e)binary::get_1_bit_8(2, status) << std::endl
           << prefix << "operational_data.options.status.parameter_3_capable "
           << (yes_no_e)binary::get_1_bit_8(3, status) << std::endl
           << prefix << "operational_data.options.status.parameter_4_capable "
           << (yes_no_e)binary::get_1_bit_8(4, status) << std::endl
           << prefix << "operational_data.options.status.parameter_5_capable "
           << (yes_no_e)binary::get_1_bit_8(5, status) << std::endl
           << prefix << "operational_data.options.status.parameter_6_capable "
           << (yes_no_e)binary::get_1_bit_8(6, status) << std::endl
           << prefix << "operational_data.options.status.operational "
           << (yes_no_e)binary::get_1_bit_8(7, status) << std::endl
           << prefix << "operational_data.information_layers "
           << binary::str(information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.1 "
           << (yes_no_e)binary::get_1_bit_8(1, information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.2 "
           << (yes_no_e)binary::get_1_bit_8(2, information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.3 "
           << (yes_no_e)binary::get_1_bit_8(3, information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.4 "
           << (yes_no_e)binary::get_1_bit_8(4, information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.5 "
           << (yes_no_e)binary::get_1_bit_8(5, information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.6 "
           << (yes_no_e)binary::get_1_bit_8(6, information_layers) << std::endl
           << prefix << "operational_data.options.information_layers.7 "
           << (yes_no_e)binary::get_1_bit_8(7, information_layers) << std::endl
           << prefix << "operational_data.data_field.1 "
           << (int)data_field_1 << std::endl
           << prefix << "operational_data.data_field.2 "
           << (int)data_field_2 << std::endl
           << prefix << "operational_data.parameter.1 "
           << binary::str(parameter_1) << std::endl
           << prefix << "operational_data.parameter.2 "
           << binary::str(parameter_2) << std::endl
           << prefix << "operational_data.parameter.3 "
           << binary::str(parameter_3) << std::endl
           << prefix << "operational_data.parameter.4 "
           << binary::str(parameter_4) << std::endl
           << prefix << "operational_data.parameter.5 "
           << binary::str(parameter_5) << std::endl
           << prefix << "operational_data.parameter.6 "
           << binary::str(parameter_6) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::iff_operational_data_t::write(byte_stream &stream) const
{
    stream.write(status);
    stream.write(data_field_1);
    stream.write(information_layers);
    stream.write(data_field_2);
    stream.write(parameter_1);
    stream.write(parameter_2);
    stream.write(parameter_3);
    stream.write(parameter_4);
    stream.write(parameter_5);
    stream.write(parameter_6);
}

// ----------------------------------------------------------------------------
void vdb::iff_operational_data_t::read(byte_stream &stream)
{
    stream.read(status);
    stream.read(data_field_1);
    stream.read(information_layers);
    stream.read(data_field_2);
    stream.read(parameter_1);
    stream.read(parameter_2);
    stream.read(parameter_3);
    stream.read(parameter_4);
    stream.read(parameter_5);
    stream.read(parameter_6);
}

// ----------------------------------------------------------------------------
vdb::em_emission_pdu_t::em_emission_pdu_t(void) : update(0), padding(0)
{

}

// ----------------------------------------------------------------------------
vdb::em_emission_pdu_t::~em_emission_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::em_emission_pdu_t::get_initator(void) const
{
    return &emitter;
}

// ----------------------------------------------------------------------------
bool vdb::em_emission_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? emitter.matches(*id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::em_emission_pdu_t::clear(void)
{
    emitter.clear();
    event.clear();
    update = 0;
    padding = 0;
    systems.clear();
}

// ----------------------------------------------------------------------------
void vdb::em_emission_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "emitter ";

    emitter.print_full(stream);

    stream << std::endl
           << prefix << "event " << event << std::endl
           << prefix << "update " << (int)update << std::endl
           << prefix << "padding " << binary::str(padding, true) << std::endl
           << prefix << "systems " << systems.size() << std::endl;

    for(uint32_t i = 0; i < systems.size(); ++i)
    {
        systems[i].print(
            (prefix + "systems[" + generic_to_string(i) + "]."),
            stream);
    }
}

// ----------------------------------------------------------------------------
void vdb::em_emission_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << event.str() << std::endl;

    for(uint32_t i = 0; i < systems.size(); ++i)
    {
        stream << "  " << (emitter_name_e)systems[i].name << "  "
               << (emitter_function_e)systems[i].function << std::endl;
    }
}

// ----------------------------------------------------------------------------
uint32_t vdb::em_emission_pdu_t::length(void) const
{
    uint32_t
        record_length = BASE_LENGTH;

    for(uint32_t i = 0; i < systems.size(); ++i)
    {
        record_length += systems[i].length();
    }

    return record_length;
}

// ----------------------------------------------------------------------------
void vdb::em_emission_pdu_t::read(byte_stream &stream)
{
    uint8_t
        system_count = 0;

    stream.read(emitter);
    stream.read(event);
    stream.read(update);
    stream.read(system_count);
    stream.read(padding);

    for(uint8_t i = 0; i < system_count; ++i)
    {
        emitter_system_t
            system;

        system.read(stream);

        systems.push_back(system);
    }
}

// ----------------------------------------------------------------------------
void vdb::em_emission_pdu_t::write(byte_stream &stream) const
{
    const uint8_t
        system_count = systems.size();

    stream.write(emitter);
    stream.write(event);
    stream.write(update);
    stream.write(system_count);
    stream.write(padding);

    for(uint8_t i = 0; i < system_count; ++i)
    {
        systems[i].write(stream);
    }
}

// ----------------------------------------------------------------------------
vdb::designator_pdu_t::designator_pdu_t(void) :
    spot_type(0),
    system_name(0),
    code(0),
    power(0.0f),
    wavelength(0.0f),
    algorithm(0),
    flash_rate(0),
    system_number(0),
    function(0)
{

}

// ----------------------------------------------------------------------------
vdb::designator_pdu_t::~designator_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::designator_pdu_t::get_initator(void) const
{
    return &designating_id;
}

// ----------------------------------------------------------------------------
bool vdb::designator_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        designating_id.matches(*id_ptr) or
        designated_id.matches(*id_ptr) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::designator_pdu_t::clear(void)
{
    designating_id.clear();
    spot_type = 0;
    system_name = 0;
    designated_id.clear();
    code = 0;
    power = 0;
    wavelength = 0;
    spot_offset.clear();
    spot_location.clear();
    algorithm = 0;
    flash_rate = 0;
    system_number = 0;
    function = 0;
    beam_offset.clear();
}

// ----------------------------------------------------------------------------
void vdb::designator_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "designating_id ";

    designating_id.print_full(stream);

    stream << std::endl
           << prefix << "designated_id ";

    designated_id.print_full(stream);

    stream << std::endl
           << prefix << "spot_type "
           << enumerations::print(ENUM_DESIG_SPOT_TYPE, spot_type) << std::endl
           << prefix << "system_name "
           << enumerations::print(ENUM_DESIG_SYSTEM_NAME, system_name) << std::endl
           << prefix << "code "
           << (int)code << std::endl
           << prefix << "power "
           << float_to_string(power, 1, 3) << std::endl
           << prefix << "wavelength "
           << float_to_string(wavelength, 1, 3) << std::endl
           << prefix << "spot_offset "
           << spot_offset << std::endl
           << prefix << "spot_location.gcc "
           << spot_location << std::endl
           << prefix << "spot_location.gdc "
           << spot_location.geodetic_str() << std::endl
           << prefix << "algorithm "
           << enumerations::print(ENUM_DEAD_RECKONING, algorithm) << std::endl
           << prefix << "flash_rate " << (int)flash_rate << std::endl
           << prefix << "system_number " << (int)system_number << std::endl
           << prefix << "function "
           << enumerations::print(ENUM_LASER_FUNCTION, function) << std::endl
           << prefix << "beam_offset " << beam_offset << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::designator_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << designated_id << "  " << (int)code << "  "
           << float_to_string(power, 1, 3) << std::endl
           << "  " << (laser_function_e)function << "  "
           << "  " << (desig_spot_type_e)spot_type << "  "
           << (desig_system_name_e)system_name << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::designator_pdu_t::read(byte_stream &stream)
{
    stream.read(designating_id);
    stream.read(spot_type);
    stream.read(system_name);
    stream.read(designated_id);
    stream.read(code);
    stream.read(power);
    stream.read(wavelength);
    stream.read(spot_offset);
    stream.read(spot_location);
    stream.read(algorithm);
    stream.read(flash_rate);
    stream.read(system_number);
    stream.read(function);
    stream.read(beam_offset);
}

// ----------------------------------------------------------------------------
void vdb::designator_pdu_t::write(byte_stream &stream) const
{
    stream.write(designating_id);
    stream.write(spot_type);
    stream.write(system_name);
    stream.write(designated_id);
    stream.write(code);
    stream.write(power);
    stream.write(wavelength);
    stream.write(spot_offset);
    stream.write(spot_location);
    stream.write(algorithm);
    stream.write(flash_rate);
    stream.write(system_number);
    stream.write(function);
    stream.write(beam_offset);
}

// ----------------------------------------------------------------------------
vdb::iff_pdu_t::iff_pdu_t(void) :
    designator(0),
    specific(0)
{

}

// ----------------------------------------------------------------------------
vdb::iff_pdu_t::~iff_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::iff_pdu_t::get_initator(void) const
{
    return &emitter;
}

// ----------------------------------------------------------------------------
bool vdb::iff_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? emitter.matches(*id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::iff_pdu_t::clear(void)
{
    emitter.clear();
    event.clear();
    antenna_location.clear();
    system_id.clear();
    designator = 0;
    specific = 0;
    operational_data.clear();
    extra_layers.clear();
}

// ----------------------------------------------------------------------------
void vdb::iff_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "emitter ";

    emitter.print_full(stream);

    stream << std::endl
            << prefix << "event " << event << std::endl
            << prefix << "antenna_location " << antenna_location << std::endl;

    system_id.print(prefix, stream);

    stream << prefix << "designator " << (int)designator << std::endl
           << prefix << "specific " << (int)specific << std::endl;

    operational_data.print(prefix, stream);
}

// ----------------------------------------------------------------------------
void vdb::iff_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << event << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::iff_pdu_t::length(void) const
{
    return (BASE_LENGTH + extra_layers.length());
}

// ----------------------------------------------------------------------------
void vdb::iff_pdu_t::read(byte_stream &stream)
{
    stream.read(emitter);
    stream.read(event);
    stream.read(antenna_location);
    stream.read(system_id);
    stream.read(designator);
    stream.read(specific);
    stream.read(operational_data);

    // This is a workaround to read any remaining bytes for extra layers
    // until data types for those extra layers can be implemented.
    //
    extra_layers.data_length = stream.get_length_remaining();
    extra_layers.read(stream);
}

// ----------------------------------------------------------------------------
void vdb::iff_pdu_t::write(byte_stream &stream) const
{
    stream.write(emitter);
    stream.write(event);
    stream.write(antenna_location);
    stream.write(system_id);
    stream.write(designator);
    stream.write(specific);
    stream.write(operational_data);

    extra_layers.write(stream);
}
