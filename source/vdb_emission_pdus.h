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

#ifndef VDB_EMISSION_PDUS_H
#define VDB_EMISSION_PDUS_H

#include "vdb_entity_type.h"
#include "vdb_ids.h"
#include "vdb_pdu.h"
#include "vdb_spatial.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class emitter_target_t : public record_t
    {
      public:

        emitter_target_t(void);
        emitter_target_t(const emitter_target_t &copy);
        virtual ~emitter_target_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity;
        uint8_t
            emitter,
            beam;

        static const uint32_t
            LENGTH = 8; // bytes
    };

    // ------------------------------------------------------------------------
    class emitter_beam_t : public record_t
    {
      public:

        emitter_beam_t(void);
        emitter_beam_t(const emitter_beam_t &copy);
        virtual ~emitter_beam_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            data_length,
            id_number;
        uint16_t
            parameter_index;
        float32_t
            frequency,
            frequency_range,
            effective_radiated_power,
            pulse_repetition_frequency,
            pulse_width,
            azimuth_center,
            azimuth_sweep,
            elevation_center,
            elevation_sweep,
            sweep_sync;
        uint8_t
            function,
            hd_track_jam,
            status;
        uint32_t
            jamming_technique;
        std::vector<emitter_target_t>
            targets;

        static const uint32_t
            BASE_LENGTH = 52; // bytes
    };

    // ------------------------------------------------------------------------
    class emitter_system_t : public record_t
    {
      public:

        emitter_system_t(void);
        emitter_system_t(const emitter_system_t &copy);
        virtual ~emitter_system_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            data_length;
        uint16_t
            padding,
            name;
        uint8_t
            function,
            number;
        location12_t
            location;
        std::vector<emitter_beam_t>
            beams;

        static const uint32_t
            BASE_LENGTH = 20; // bytes
    };

    // ------------------------------------------------------------------------
    class iff_system_id_t : public record_t
    {
      public:

        iff_system_id_t(void);
        iff_system_id_t(const iff_system_id_t &copy);
        virtual ~iff_system_id_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint16_t
            type,
            name;
        uint8_t
            mode,
            options;

        static const uint32_t
            LENGTH = 6; // bytes
    };

    // ------------------------------------------------------------------------
    class iff_operational_data_t : public record_t
    {
      public:

        iff_operational_data_t(void);
        iff_operational_data_t(const iff_operational_data_t &copy);
        virtual ~iff_operational_data_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            status,
            data_field_1,
            information_layers,
            data_field_2;
        uint16_t
            parameter_1,
            parameter_2,
            parameter_3,
            parameter_4,
            parameter_5,
            parameter_6;

        static const uint32_t
            LENGTH = 16; // bytes
    };

    // ------------------------------------------------------------------------
    class iff_parameter_data_t : public record_t
    {
      public:

        iff_parameter_data_t(void);
        iff_parameter_data_t(const iff_parameter_data_t &copy);
        virtual ~iff_parameter_data_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        float32_t
            erp,
            frequency,
            pgrf,
            pulse_width,
            burst_length;
        uint8_t
            applicable_modes;
        uint8_t
            system_specific_data[3];

        static const uint32_t
            LENGTH = 24; // bytes
    };

    // ------------------------------------------------------------------------
    class iff_layer2_data_t : public record_t
    {
      public:

        iff_layer2_data_t(void);
        iff_layer2_data_t(const iff_layer2_data_t &copy);
        virtual ~iff_layer2_data_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        // Layer header
        //
        uint8_t
            number,
            specific_information;
        uint16_t
            layer_length;

        // Beam data
        //
        float32_t
            azimuth_center,
            azimuth_sweep,
            elevation_center,
            elevation_sweep,
            sweep_sync;

        // Secondary operational data
        //
        uint8_t
            parameter_1,
            parameter_2;
        uint16_t
            parameter_data_count;

        std::vector<iff_parameter_data_t>
            parameter_data;

        static const uint32_t
            BASE_LENGTH = 28;
    };

    // ------------------------------------------------------------------------
    // Designator PDU
    //
    class designator_pdu_t : public pdu_content_t
    {
      public:

        designator_pdu_t(void);
        virtual ~designator_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            designating_id;
        uint8_t
            spot_type,
            system_name;
        id_t
            designated_id;
        uint16_t
            code;
        float32_t
            power,
            wavelength;
        location12_t
            spot_offset; // (with respect to designated entity or object)
        location24_t
            spot_location;
        uint8_t
            algorithm,
            flash_rate,
            system_number,
            function;
        location12_t
            beam_offset; // (beam origin offset)

        static const uint32_t
            LENGTH = 88; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Electromagnetic Emission PDU
    //
    class em_emission_pdu_t : public pdu_content_t
    {
      public:

        em_emission_pdu_t(void);
        virtual ~em_emission_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            emitter;
        id_t
            event;
        uint8_t
            update;
        uint16_t
            padding;
        std::vector<emitter_system_t>
            systems;

        static const uint32_t
            BASE_LENGTH = 16; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Identify Friend or Foe (IFF) PDU
    //
    // Layer 1: Basic system data, required for all IFF PDUs, supports all
    //          system types.
    // Layer 2: Basic emissions data to support simulations that need
    //          detailed IFF electromagnetic characteristics.
    // Layer 3: Mode 5 functional data, military mode 5 is the only system
    //          that currently uses layer 3.
    // Layer 4: Mode S functional data, civilian mode S is the only system
    //          that currently uses layer 4.
    // Layer 5: Data communications, supports the emulation of real-world
    //          transponder and interrogator data link messages.
    //
    class iff_pdu_t : public pdu_content_t
    {
      public:

        iff_pdu_t(void);
        virtual ~iff_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            emitter,
            event;
        location12_t
            antenna_location;
        iff_system_id_t
            system_id;
        uint8_t
            designator,
            specific;
        iff_operational_data_t
            operational_data;

        // TODO Layers 2, 3, ...
        //
        data_buffer_t
            extra_layers;

        static const uint32_t
            BASE_LENGTH = 48; // bytes, minus header
    };
}

#endif
