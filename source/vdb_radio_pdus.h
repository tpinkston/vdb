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

#ifndef VDB_RADIO_PDUS_H
#define VDB_RADIO_PDUS_H

#include "vdb_data_buffer.h"
#include "vdb_ids.h"
#include "vdb_pdu.h"
#include "vdb_spatial.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class modulation_type_t : public record_t
    {
      public:

        modulation_type_t(void);
        modulation_type_t(const modulation_type_t &copy);
        virtual ~modulation_type_t(void);

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint16_t
            spread_spectrum,
            major_modulation,
            modulation_detail,
            radio_system;

        static const uint32_t
            LENGTH = 8; // bytes
    };

    // ------------------------------------------------------------------------
    // Transmitter PDU
    //
    class transmitter_pdu_t : public pdu_content_t
    {
      public:

        transmitter_pdu_t(void);
        virtual ~transmitter_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity_id;
        uint16_t
            radio_id;
        entity_type_t
            radio_type;
        uint8_t
            transmit_state,
            input_source;
        uint16_t
            transmitter_parameters;
        location24_t
            antenna_location;
        location12_t
            relative_antenna_location;
        uint16_t
            antenna_pattern_type,
            antenna_pattern_length;
        uint64_t
            frequency;
        float32_t
            bandwidth,
            power;
        modulation_type_t
            modulation_type;
        uint16_t
            crypto_system,
            crypto_key;
        uint8_t
            modulation_parameter_length;
        uint8_t
            padding8;
        uint16_t
            padding24;
        uint8_t
            *antenna_patterns,
            *modulation_parameters;

        static const uint32_t
            BASE_LENGTH = 92; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Receiver PDU
    //
    class receiver_pdu_t : public pdu_content_t
    {
      public:

        receiver_pdu_t(void);
        virtual ~receiver_pdu_t(void);

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity_id;
        uint16_t
            radio_id,
            receiver_state,
            padding;
        float32_t
            power;
        id_t
            transmitter_entity;
        uint16_t
            transmitter_radio;

        static const uint32_t
            LENGTH = 24; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    // Signal PDU
    //
    class signal_pdu_t : public pdu_content_t
    {
      public:

        signal_pdu_t(void);
        virtual ~signal_pdu_t(void);

        encoding_class_e get_encoding_class(void) const;
        tdl_type_e get_tdl_type(void) const;

        virtual const id_t *get_initator(void) const;

        virtual bool contains(const id_t *) const;

        virtual void clear(void);

        virtual void print_summary(std::ostream &) const;
        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const;
        virtual uint32_t padding_length(void) const;

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        id_t
            entity_id;
        uint16_t
            radio_id,
            encoding,
            tdl_type;
        int32_t
            sample_rate;
        uint16_t
            samples;
        data_buffer_t
            data;

        static const uint32_t
            BASE_LENGTH = 20; // bytes, minus header
    };
}

#endif
