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

#include "vdb_byte_stream.h"
#include "vdb_binary.h"
#include "vdb_entity_type.h"
#include "vdb_enums.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_radio_pdus.h"
#include "vdb_services.h"
#include "vdb_string.h"

// ----------------------------------------------------------------------------
vdb::modulation_type_t::modulation_type_t(void) :
    spread_spectrum(0),
    major_modulation(0),
    modulation_detail(0),
    radio_system(0)
{

}

// ----------------------------------------------------------------------------
vdb::modulation_type_t::modulation_type_t(
    const modulation_type_t &copy
) :
    spread_spectrum(copy.spread_spectrum),
    major_modulation(copy.major_modulation),
    modulation_detail(copy.modulation_detail),
    radio_system(copy.radio_system)
{

}

// ----------------------------------------------------------------------------
vdb::modulation_type_t::~modulation_type_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::modulation_type_t::clear(void)
{
    spread_spectrum = 0;
    major_modulation = 0;
    modulation_detail = 0;
    radio_system = 0;
}

// ----------------------------------------------------------------------------
void vdb::modulation_type_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    const uint32_t
        frequency_hopping = binary::get_1_bit_16(0, spread_spectrum),
        pseudo_noise = binary::get_1_bit_16(1, spread_spectrum),
        time_hopping = binary::get_1_bit_16(2, spread_spectrum);

    stream << prefix << "modulation_type.spread_spectrum "
           << binary::str(spread_spectrum, true) << std::endl
           << prefix << "modulation_type.spread_spectrum.frequency_hopping[0] "
           << (yes_no_e)frequency_hopping << std::endl
           << prefix << "modulation_type.spread_spectrum.pseudo_noise[1] "
           << (yes_no_e)pseudo_noise << std::endl
           << prefix << "modulation_type.spread_spectrum.time_hopping[2] "
           << (yes_no_e)time_hopping << std::endl
           << prefix << "modulation_type.major_modulation "
           << (major_modulation_e)major_modulation
           << " [" << major_modulation << "]" << std::endl
           << prefix << "modulation_type.modulation_detail ";

    switch(major_modulation)
    {
        case MAJOR_MODULATION_MAJ_MOD_AMPLITUDE:
            stream << (amplitude_e)modulation_detail << " ";
            break;
        case MAJOR_MODULATION_MAJ_MOD_AMPLITUDE_AND_ANGLE:
            stream << (amplitude_and_angle_e)modulation_detail << " ";
            break;
        case MAJOR_MODULATION_MAJ_MOD_ANGLE:
            stream << (angle_e)modulation_detail << " ";
            break;
        case MAJOR_MODULATION_MAJ_MOD_COMBINATION:
            stream << (combination_e)modulation_detail << " ";
            break;
        case MAJOR_MODULATION_MAJ_MOD_PULSE:
            stream << (pulse_e)modulation_detail << " ";
            break;
        case MAJOR_MODULATION_MAJ_MOD_UNMODULATED:
            stream << (unmodulated_e)modulation_detail << " ";
            break;
    }

    stream << "[" << modulation_detail << "]" << std::endl
           << prefix << "modulation_type.radio_system "
           << (radio_system_e)radio_system << " ["
           << (int)radio_system << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::modulation_type_t::read(byte_stream &stream)
{
    stream.read(spread_spectrum);
    stream.read(major_modulation);
    stream.read(modulation_detail);
    stream.read(radio_system);
}

// ----------------------------------------------------------------------------
void vdb::modulation_type_t::write(byte_stream &stream) const
{
    stream.write(spread_spectrum);
    stream.write(major_modulation);
    stream.write(modulation_detail);
    stream.write(radio_system);
}

// ----------------------------------------------------------------------------
vdb::transmitter_pdu_t::transmitter_pdu_t(void) :
    radio_id(0),
    transmit_state(0),
    input_source(0),
    transmitter_parameters(0),
    antenna_pattern_type(0),
    antenna_pattern_length(0),
    frequency(0),
    bandwidth(0),
    power(0),
    crypto_system(0),
    crypto_key(0),
    modulation_parameter_length(0),
    padding8(0),
    padding24(0),
    antenna_patterns(0),
    modulation_parameters(0)
{

}

// ----------------------------------------------------------------------------
vdb::transmitter_pdu_t::~transmitter_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
void vdb::transmitter_pdu_t::clear(void)
{
    entity_id.clear();
    radio_id = 0;
    radio_type.clear();
    transmit_state = 0;
    input_source = 0;
    transmitter_parameters = 0;
    antenna_location.clear();
    relative_antenna_location.clear();
    antenna_pattern_type = 0;
    antenna_pattern_length = 0;
    frequency = 0;
    bandwidth = 0;
    power = 0;
    modulation_type.clear();
    crypto_system = 0;
    crypto_key = 0;
    modulation_parameter_length = 0;
    padding8 = 0;
    padding24 = 0;

    if (antenna_patterns)
    {
        delete antenna_patterns;
        antenna_patterns = 0;
    }

    if (modulation_parameters)
    {
        delete modulation_parameters;
        modulation_parameters = 0;
    }
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::transmitter_pdu_t::get_initator(void) const
{
    return &entity_id;
}

// ----------------------------------------------------------------------------
bool vdb::transmitter_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? entity_id.matches(*id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::transmitter_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "entity_id ";

    entity_id.print_full(stream);

    stream << std::endl
           << prefix << "radio_id " << radio_id << std::endl
           << prefix << "radio_type " << radio_type << std::endl
           << prefix << "radio_type.description "
           << radio_type.get_description() << std::endl
           << prefix << "transmit_state "
           << enumerations::print(ENUM_TRANSMIT_STATE, transmit_state)
           << std::endl
           << prefix << "input_source "
           << enumerations::print(ENUM_INPUT_SOURCE, input_source)
           << std::endl
           << prefix << "antenna_location "
           << antenna_location << std::endl
           << prefix << "relative_antenna_location "
           << relative_antenna_location << std::endl
           << prefix << "antenna_pattern_type "
           << enumerations::print(ENUM_ANTENNA_PATTERN_TYPE, antenna_pattern_type)
           << std::endl
           << prefix << "antenna_pattern_length(bytes) "
           << (int)antenna_pattern_length << std::endl;

    if ((antenna_pattern_length > 0) and antenna_patterns)
    {
        hexadecimal::stream(
            (prefix + "antenna_patterns "),
            antenna_patterns,
            antenna_pattern_length,
            16,
            stream);
    }

    stream << prefix << "frequency(Hz) " << frequency << std::endl
           << prefix << "bandwidth "
           << float_to_string(bandwidth, 1, 4) << std::endl
           << prefix << "power(dBm) "
           << float_to_string(power, 1, 4) << std::endl;

    modulation_type.print(prefix, stream);

    stream << prefix << "modulation_parameter_length(bytes) "
           << (int)modulation_parameter_length << std::endl;

    if ((modulation_parameter_length > 0) and modulation_parameters)
    {
        hexadecimal::stream(
            (prefix + "modulation_parameters "),
            modulation_parameters,
            modulation_parameter_length,
            16,
            stream);
    }

    stream << prefix << "crypto_system "
           << enumerations::print(ENUM_CRYPTO_SYS, crypto_system)
           << std::endl
           << prefix << "crypto_key "
           << (int)crypto_key << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::transmitter_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  [" << radio_id << "]  " << (transmit_state_e)transmit_state
           << "  " << (input_source_e)input_source << std::endl
           << "  " << radio_type
           << " '" << radio_type.get_description() << "'" << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::transmitter_pdu_t::length(void) const
{
    return (BASE_LENGTH + antenna_pattern_length + modulation_parameter_length);
}

// ----------------------------------------------------------------------------
void vdb::transmitter_pdu_t::read(byte_stream &stream)
{
    clear();

    stream.read(entity_id);
    stream.read(radio_id);
    stream.read(radio_type);
    stream.read(transmit_state);
    stream.read(input_source);
    stream.read(transmitter_parameters);
    stream.read(antenna_location);
    stream.read(relative_antenna_location);
    stream.read(antenna_pattern_type);
    stream.read(antenna_pattern_length);
    stream.read(frequency);
    stream.read(bandwidth);
    stream.read(power);
    stream.read(modulation_type);
    stream.read(crypto_system);
    stream.read(crypto_key);
    stream.read(modulation_parameter_length);
    stream.read(padding8);
    stream.read(padding24);

    if (antenna_pattern_length > 0)
    {
        antenna_patterns = new uint8_t[antenna_pattern_length];

        for(int i = 0; (i < antenna_pattern_length); ++i)
        {
            stream.read(antenna_patterns[i]);
        }
    }

    if (modulation_parameter_length > 0)
    {
        modulation_parameters = new uint8_t[modulation_parameter_length];

        for(int i = 0; (i < modulation_parameter_length); ++i)
        {
            stream.read(modulation_parameters[i]);
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::transmitter_pdu_t::write(byte_stream &stream) const
{
    stream.write(entity_id);
    stream.write(radio_id);
    stream.write(radio_type);
    stream.write(transmit_state);
    stream.write(input_source);
    stream.write(transmitter_parameters);
    stream.write(antenna_location);
    stream.write(relative_antenna_location);
    stream.write(antenna_pattern_type);
    stream.write(antenna_pattern_length);
    stream.write(frequency);
    stream.write(bandwidth);
    stream.write(power);
    stream.write(modulation_type);
    stream.write(crypto_system);
    stream.write(crypto_key);
    stream.write(modulation_parameter_length);
    stream.write(padding8);
    stream.write(padding24);

    if (antenna_patterns)
    {
        for(int i = 0; (i < antenna_pattern_length); ++i)
        {
            stream.write(antenna_patterns[i]);
        }
    }

    if (modulation_parameters)
    {
        for(int i = 0; (i < modulation_parameter_length); ++i)
        {
            stream.write(modulation_parameters[i]);
        }
    }
}

// ----------------------------------------------------------------------------
vdb::receiver_pdu_t::receiver_pdu_t(void) :
    radio_id(0),
    receiver_state(0),
    padding(0),
    power(0),
    transmitter_radio(0)
{

}

// ----------------------------------------------------------------------------
vdb::receiver_pdu_t::~receiver_pdu_t(void)
{

}

// ----------------------------------------------------------------------------
void vdb::receiver_pdu_t::clear(void)
{
    entity_id.clear();
    radio_id = 0;
    receiver_state = 0;
    power = 0;
    transmitter_entity.clear();
    transmitter_radio = 0;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::receiver_pdu_t::get_initator(void) const
{
    return &entity_id;
}

// ----------------------------------------------------------------------------
bool vdb::receiver_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ?
        (entity_id.matches(*id_ptr) or transmitter_entity.matches(*id_ptr)) :
        false);
}

// ----------------------------------------------------------------------------
void vdb::receiver_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    std::string
        radio_type_name;

    stream << prefix << "entity_id ";

    entity_id.print_full(stream);

    stream << std::endl
           << prefix << "radio_id " << radio_id << std::endl
           << prefix << "receiver_state "
           << enumerations::print(ENUM_RECEIVER, receiver_state)
           << std::endl
           << prefix << "power(dBm) "
           << float_to_string(power, 1, 4) << std::endl
           << prefix << "transmitter_entity ";

    transmitter_entity.print_full(stream);

    stream << std::endl
           << prefix << "transmitter_radio " << std::endl
           << transmitter_radio << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::receiver_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  " << entity_id << "[" << radio_id << "]->"
           << transmitter_entity << "[" << transmitter_radio << "]"
           << std::endl
           << "  " << (receiver_e)receiver_state << "  "
           << float_to_string(power, 1, 4) << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::receiver_pdu_t::read(byte_stream &stream)
{
    clear();

    stream.read(entity_id);
    stream.read(radio_id);
    stream.read(receiver_state);
    stream.read(padding);
    stream.read(power);
    stream.read(transmitter_entity);
    stream.read(transmitter_radio);
}

// ----------------------------------------------------------------------------
void vdb::receiver_pdu_t::write(byte_stream &stream) const
{
    stream.write(entity_id);
    stream.write(radio_id);
    stream.write(receiver_state);
    stream.write(padding);
    stream.write(power);
    stream.write(transmitter_entity);
    stream.write(transmitter_radio);
}

// ----------------------------------------------------------------------------
vdb::signal_pdu_t::signal_pdu_t(void) :
    radio_id(0),
    encoding(0),
    tdl_type(0),
    sample_rate(0),
    samples(0),
    data(0)
{

}

// ----------------------------------------------------------------------------
vdb::signal_pdu_t::~signal_pdu_t(void)
{
    clear();
}

// ----------------------------------------------------------------------------
vdb::encoding_class_e vdb::signal_pdu_t::get_encoding_class(void) const
{
    return (encoding_class_e)binary::get_2_bits_16(15, encoding);
}

// ----------------------------------------------------------------------------
vdb::tdl_type_e vdb::signal_pdu_t::get_tdl_type(void) const
{
    return (tdl_type_e)tdl_type;
}

// ----------------------------------------------------------------------------
const vdb::id_t *vdb::signal_pdu_t::get_initator(void) const
{
    return &entity_id;
}

// ----------------------------------------------------------------------------
bool vdb::signal_pdu_t::contains(const id_t *id_ptr) const
{
    return (id_ptr ? entity_id.matches(*id_ptr) : false);
}

// ----------------------------------------------------------------------------
void vdb::signal_pdu_t::clear(void)
{
    entity_id.clear();
    radio_id = 0;
    encoding = 0;
    tdl_type = 0;
    sample_rate = 0;
    samples = 0;
    data.clear();
}

// ----------------------------------------------------------------------------
void vdb::signal_pdu_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    // The first two bits (15-14) of the 'encoding' value is the encoding class.
    //
    const encoding_class_e
        encoding_class = get_encoding_class();

    stream << prefix << "entity_id ";

    entity_id.print_full(stream);

    stream << std::endl
           << prefix << "radio_id " << radio_id << std::endl
           << prefix << "encoding_class "
           << enumerations::print(ENUM_ENCODING_CLASS, encoding_class)
           << std::endl;

    if (encoding_class == ENCODING_CLASS_ENCODED_AUDIO)
    {
        const encoding_type_e
            encoding_type = (encoding_type_e)(encoding & 0x3FFF);

        stream << prefix << "encoding_type "
               << enumerations::print(ENUM_ENCODING_TYPE, encoding_type)
               << std::endl;
    }
    else
    {
        const uint16_t
            tdl_message_count = (encoding & 0x3FFF);

        stream << prefix << "tdl_message_count " << (int)tdl_message_count
               << std::endl;
    }

    stream << prefix << "tdl_type "
           << enumerations::print(ENUM_TDL_TYPE, tdl_type) << std::endl
           << prefix << "sample_rate " << sample_rate << std::endl
           << prefix << "samples " << samples << std::endl;

    data.print((prefix + "data "), stream);
}

// ----------------------------------------------------------------------------
void vdb::signal_pdu_t::print_summary(std::ostream &stream) const
{
    stream << "  [" << radio_id << "]  "
           << get_encoding_class() << "  "
           << get_tdl_type() << std::endl;
}

// ----------------------------------------------------------------------------
uint32_t vdb::signal_pdu_t::length(void) const
{
    return (BASE_LENGTH + data.length() + padding_length());
}

// ----------------------------------------------------------------------------
uint32_t vdb::signal_pdu_t::padding_length(void) const
{
    return get_padding_length(data.length(), 4);
}

// ----------------------------------------------------------------------------
void vdb::signal_pdu_t::read(byte_stream &stream)
{
    clear();

    stream.read(entity_id);
    stream.read(radio_id);
    stream.read(encoding);
    stream.read(tdl_type);
    stream.read(sample_rate);
    stream.read(data.data_length);
    stream.read(samples);

    // Data length should be in bits.
    //
    if (data.data_length % 8)
    {
        LOG_ERROR("Unexpected signal data length: %d", data.data_length);
    }

    data.data_length /= 8;

    const uint32_t
        padding = padding_length();

    LOG_EXTRA_VERBOSE(
        "Signal data length (%d data + %d padding)",
        data.data_length,
        padding);

    data.read(stream);

    if (padding > 0)
    {
        stream.skip(padding);
    }
}

// ----------------------------------------------------------------------------
void vdb::signal_pdu_t::write(byte_stream &stream) const
{
    const uint32_t
        padding = padding_length();

    stream.write(entity_id);
    stream.write(radio_id);
    stream.write(encoding);
    stream.write(tdl_type);
    stream.write(sample_rate);
    stream.write((uint16_t)(data.data_length * 8));
    stream.write(samples);

    data.write(stream);

    for(uint32_t i = 0; i < padding; ++i)
    {
        stream.write((uint8_t)0);
    }
}
