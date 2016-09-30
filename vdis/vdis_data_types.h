#ifndef VDIS_DATA_TYPES_H
#define VDIS_DATA_TYPES_H

#include <cstdint>
#include <iostream>

#include "vdis_enums.h"

#define VDIS_MARKING_CHARACTERS 11
#define VDIS_DR_PARAMETERS_SIZE 15

typedef float   float32_t;
typedef double  float64_t;

namespace vdis
{
    class byte_stream_t;

    // Utility methods for converting entity, object and event ID values
    // to a single 64-bit unsigned integer and back.
    //
    uint64_t convert(uint16_t, uint16_t, uint16_t);
    void convert(uint64_t, uint16_t &, uint16_t &, uint16_t &);

    // ------------------------------------------------------------------------
    struct simulation_id_t
    {
        uint16_t                site;
        uint16_t                application;

        inline void clear(void)
        {
            site = 0;
            application = 0;
        }

        bool operator==(const simulation_id_t &) const;
        bool operator!=(const simulation_id_t &) const;
        bool operator<(const simulation_id_t &) const;
        bool operator>(const simulation_id_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_id_t
    {
        uint16_t                site;
        uint16_t                application;
        uint16_t                entity;

        bool is_none(void) const;
        bool is_all(void) const;

        inline void clear(void)
        {
            site = 0;
            application = 0;
            entity = 0;
        }

        bool operator==(const entity_id_t &) const;
        bool operator!=(const entity_id_t &) const;
        bool operator<(const entity_id_t &) const;
        bool operator>(const entity_id_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    typedef entity_id_t munition_id_t;

    // ------------------------------------------------------------------------
    struct event_id_t
    {
        uint16_t                site;
        uint16_t                application;
        uint16_t                event;

        inline void clear(void)
        {
            site = 0;
            application = 0;
            event = 0;
        }

        bool operator==(const event_id_t &) const;
        bool operator!=(const event_id_t &) const;
        bool operator<(const event_id_t &) const;
        bool operator>(const event_id_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct object_id_t
    {
        uint16_t                site;
        uint16_t                application;
        uint16_t                object;

        bool is_none(void) const;
        bool is_all(void) const;

        bool operator==(const object_id_t &) const;
        bool operator!=(const object_id_t &) const;
        bool operator<(const object_id_t &) const;
        bool operator>(const object_id_t &) const;

        inline void clear(void)
        {
            site = 0;
            application = 0;
            object = 0;
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_type_t
    {
        uint8_t                 kind;
        uint8_t                 domain;
        uint16_t                country;
        uint8_t                 category;
        uint8_t                 subcategory;
        uint8_t                 specific;
        uint8_t                 extra;

        inline ent_kind_e kind_enum(void) const
        {
            return (ent_kind_e)kind;
        }

        inline domain_e domain_enum(void) const
        {
            return (domain_e)domain;
        }

        inline country_e country_enum(void) const
        {
            return (country_e)country;
        }

        inline void clear(void)
        {
            kind = 0;
            domain = 0;
            country = 0;
            category = 0;
            subcategory = 0;
            specific = 0;
            extra = 0;
        }

        void set(uint64_t value);
        uint64_t get(void) const;

        std::string description(void) const;

        bool operator==(const entity_type_t &) const;
        bool operator!=(const entity_type_t &) const;
        bool operator<(const entity_type_t &) const;
        bool operator>(const entity_type_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    typedef entity_type_t munition_type_t;

    // ------------------------------------------------------------------------
    struct object_type_t
    {
        uint8_t                 domain;
        uint8_t                 kind;
        uint8_t                 category;
        uint8_t                 subcategory;

        inline void clear(void)
        {
            domain = 0;
            kind = 0;
            category = 0;
            subcategory = 0;
        }

        void set(uint32_t value);
        uint32_t get(void) const;

        bool operator==(const object_type_t &) const;
        bool operator!=(const object_type_t &) const;
        bool operator<(const object_type_t &) const;
        bool operator>(const object_type_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct marking_t
    {
        uint8_t                 character_set;
        char                    characters[VDIS_MARKING_CHARACTERS];

        marking_t(void) { clear(); }
        ~marking_t(void) { }

        void str(const std::string &name);
        std::string str(void) const;

        inline entity_marking_e marking_type(void) const
        {
            return (entity_marking_e)character_set;
        }

        void clear(void);

        bool operator==(const marking_t &) const;
        bool operator!=(const marking_t &) const;
        bool operator<(const marking_t &) const;
        bool operator>(const marking_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union entity_capabilities_t
    {
        struct bits_t
        {
            uint32_t            ammunition_supply:1;    // Bit 0
            uint32_t            fuel_supply:1;          // Bit 1
            uint32_t            recovery:1;             // Bit 2
            uint32_t            repair:1;               // Bit 3
            uint32_t            ads_broadcast:1;        // Bit 4
            uint32_t            sling_load_carrier:1;   // Bit 5
            uint32_t            sling_loadable:1;       // Bit 6
            uint32_t            ied_presence:1;         // Bit 7
            uint32_t            task_organizable:1;     // Bit 8
            uint32_t            padding:23;             // Bits 9-31
        };

        uint32_t            value;
        bits_t              bits;

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct location12_t
    {
        float32_t               x;
        float32_t               y;
        float32_t               z;

        inline void clear(void)
        {
            x = 0;
            y = 0;
            z = 0;
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct location24_t
    {
        float64_t               x;
        float64_t               y;
        float64_t               z;

        inline void clear(void)
        {
            x = 0;
            y = 0;
            z = 0;
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct orientation_t
    {
        float32_t               psi;
        float32_t               theta;
        float32_t               phi;

        inline void clear(void)
        {
            psi = 0;
            theta = 0;
            phi = 0;
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct velocity_t
    {
        float32_t               x;
        float32_t               y;
        float32_t               z;

        inline void clear(void)
        {
            x = 0;
            y = 0;
            z = 0;
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct dead_reckoning_t
    {
        uint8_t                 algorithm;
        uint8_t                 parameters[VDIS_DR_PARAMETERS_SIZE];
        velocity_t              linear_acceleration;
        velocity_t              angular_velocity;

        inline dead_reckoning_e algorithm_enum(void) const
        {
            return (dead_reckoning_e)algorithm;
        }

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct timestamp_t
    {
        uint32_t                value;

        inline void clear(void)
        {
            value = 0;
        }

        void set(uint32_t minutes, float32_t seconds, bool absolute);
        void get(uint32_t &minutes, float32_t &seconds, bool &absolute) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct clocktime_t
    {
        uint32_t                hours;
        timestamp_t             offset;

        inline void clear(void)
        {
            hours = 0;
            offset.clear();
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct pdu_header_t
    {
        uint8_t                 protocol;
        uint8_t                 exercise;
        uint8_t                 type;
        uint8_t                 family;
        timestamp_t             timestamp;
        uint16_t                length;
        uint8_t                 status;
        uint8_t                 padding;

        inline protocol_version_e protocol_enum(void) const
        {
            return (protocol_version_e)protocol;
        }

        inline pdu_type_e type_enum(void) const
        {
            return (pdu_type_e)type;
        }

        inline pdu_family_e family_enum(void) const
        {
            return (pdu_family_e)family;
        }

        inline void clear(void)
        {
            protocol = 0;
            exercise = 0;
            type = 0;
            family = 0;
            timestamp.clear();
            length = 0;
            status = 0;
            padding = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct sling_line_t
    {
        float32_t               line_length;            // 4 bytes
        float32_t               line_offset;            // 4 bytes

        inline void clear(void)
        {
            line_offset = 0;
            line_length = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct burst_descriptor_t
    {
        munition_type_t         munition;
        uint16_t                warhead;
        uint16_t                fuse;
        uint16_t                quantity;
        uint16_t                rate;

        inline warhead_burst_desc_e warhead_enum(void) const
        {
            return (warhead_burst_desc_e)warhead;
        }

        inline fuse_burst_desc_e fuse_enum(void) const
        {
            return (fuse_burst_desc_e)fuse;
        }

        inline void clear(void)
        {
            munition.clear();
            warhead = 0;
            fuse = 0;
            quantity = 0;
            rate = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct modulation_type_t
    {
        uint16_t                spread_spectrum;        // 2 bytes
        uint16_t                major_modulation;       // 2 bytes
        uint16_t                modulation_detail;      // 2 bytes
        uint16_t                radio_system;           // 2 bytes

        inline void clear(void)
        {
            spread_spectrum = 0;
            major_modulation = 0;
            modulation_detail = 0;
            radio_system = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct emitter_target_t
    {
        entity_id_t             entity;                    // 6 bytes
        uint8_t                 emitter;                // 1 byte
        uint8_t                 beam;                   // 1 byte

        inline void clear(void)
        {
            entity.clear();
            emitter = 0;
            beam = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct emitter_beam_t
    {
        uint8_t                 data_length;                // 1 byte
        uint8_t                 id_number;                  // 1 byte
        uint16_t                parameter_index;            // 2 bytes
        float32_t               frequency;                  // 4 bytes
        float32_t               frequency_range;            // 4 bytes
        float32_t               effective_radiated_power;   // 4 bytes
        float32_t               pulse_repetition_frequency; // 4 bytes
        float32_t               pulse_width;                // 4 bytes
        float32_t               azimuth_center;             // 4 bytes
        float32_t               azimuth_sweep;              // 4 bytes
        float32_t               elevation_center;           // 4 bytes
        float32_t               elevation_sweep;            // 4 bytes
        float32_t               sweep_sync;                 // 4 bytes
        uint8_t                 function;                   // 1 byte
        uint8_t                 target_count;               // 1 byte
        uint8_t                 hd_track_jam;               // 1 byte
        uint8_t                 status;                     // 1 byte
        uint32_t                jamming_technique;          // 4 bytes
        emitter_target_t      **targets;                    // Variable length

        emitter_beam_t(void);
        ~emitter_beam_t(void);

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct emitter_system_t
    {
        uint8_t                 data_length;                // 1 byte
        uint8_t                 beam_count;                 // 1 byte
        uint16_t                padding;                    // 2 bytes
        uint16_t                name;                       // 2 bytes
        uint8_t                 function;                   // 1 byte
        uint8_t                 number;                     // 1 byte
        location12_t            location;                   // 12 bytes
        emitter_beam_t        **beams;                      // Variable length

        emitter_system_t(void);
        ~emitter_system_t(void);

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_system_id_t
    {
        uint16_t                type;                       // 2 bytes
        uint16_t                name;                       // 2 bytes
        uint8_t                 mode;                       // 1 byte
        uint8_t                 options;                    // 1 byte

        inline void clear(void)
        {
            type = 0;
            name = 0;
            mode = 0;
            options = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_operational_data_t
    {
        uint8_t                 status;                      // 1 byte
        uint8_t                 data_field_1;                // 1 byte
        uint8_t                 information_layers;          // 1 byte
        uint8_t                 data_field_2;                // 1 byte
        uint16_t                parameter_1;                 // 2 bytes
        uint16_t                parameter_2;                 // 2 bytes
        uint16_t                parameter_3;                 // 2 bytes
        uint16_t                parameter_4;                 // 2 bytes
        uint16_t                parameter_5;                 // 2 bytes
        uint16_t                parameter_6;                 // 2 bytes

        inline void clear(void)
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

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_parameter_data_t
    {
        float32_t               erp;                         // 4 bytes
        float32_t               frequency;                   // 4 bytes
        float32_t               pgrf;                        // 4 bytes
        float32_t               pulse_width;                 // 4 bytes
        float32_t               burst_length;                // 4 bytes
        uint8_t                 applicable_modes;            // 1 byte
        uint8_t                 system_specific_data[3];     // 3 bytes

        iff_parameter_data_t(void);
        ~iff_parameter_data_t(void);

        inline void clear(void)
        {
            erp = 0;
            frequency = 0;
            pgrf = 0;
            pulse_width = 0;
            burst_length = 0;
            applicable_modes = 0;
            system_specific_data[0] = 0;
            system_specific_data[1] = 0;
            system_specific_data[2] = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_layer2_data_t
    {
        // Layer header
        uint8_t                 number;                     // 1 byte
        uint8_t                 specific_information;       // 1 byte
        uint16_t                layer_length;               // 2 bytes

        // Beam data
        float32_t               azimuth_center;             // 4 bytes
        float32_t               azimuth_sweep;              // 4 bytes
        float32_t               elevation_center;           // 4 bytes
        float32_t               elevation_sweep;            // 4 bytes
        float32_t               sweep_sync;                 // 4 bytes

        // Secondary operational data
        uint8_t                 parameter_1;                // 1 byte
        uint8_t                 parameter_2;                // 1 byte
        uint16_t                parameter_data_count;       // 2 bytes
        iff_parameter_data_t  **parameter_data;             // Variable

        iff_layer2_data_t(void);
        ~iff_layer2_data_t(void);

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct environment_record_t
    {
        uint32_t                type;                       // 4 bytes
        uint16_t                data_length;                // 2 bytes
        uint8_t                 index;                      // 1 byte
        uint8_t                 padding;                    // 1 byte
        uint8_t                *parameter_data;             // Variable

        environment_record_t(void);
        ~environment_record_t(void);

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct exercise_state_t
    {
        uint8_t                     id;                     // 1 byte
        uint8_t                     transition;             // 1 byte
        uint8_t                     current_state;          // 1 byte
        uint8_t                     requested_state;        // 1 byte

        inline transition_e transition_enum(void) const
        {
            return (transition_e)transition;
        }

        inline exercise_state_e current_state_enum(void) const
        {
            return (exercise_state_e)current_state;
        }

        inline exercise_state_e requested_state_enum(void) const
        {
            return (exercise_state_e)requested_state;
        }

        inline void clear(void)
        {
            id = 0;
            transition = 0;
            current_state = 0;
            requested_state = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct linear_segment_t
    {
        uint8_t                 number;                     // 1 byte
        uint8_t                 modifications;              // 1 byte
        uint16_t                generic_appearance;         // 2 bytes
        uint32_t                specific_appearance;        // 4 bytes
        location24_t            location;                   // 24 bytes
        orientation_t           orientation;                // 12 bytes

// In the latest standard these are represented as 32-bit floating
// point numbers, not 16-bit unsigned integers.
//
#ifdef LINEAR_SEGMENTS_USE_32BIT_FLOAT
        float32_t               segment_length;             // 4 bytes
        float32_t               segment_width;              // 4 bytes
        float32_t               segment_height;             // 4 bytes
        float32_t               segment_depth;              // 4 bytes
#else
        uint16_t                segment_length;             // 2 bytes
        uint16_t                segment_width;              // 2 bytes
        uint16_t                segment_height;             // 2 bytes
        uint16_t                segment_depth;              // 2 bytes
#endif

        uint32_t                padding;                    // 4 bytes

        inline float32_t length(void) const { return segment_length; }
        inline float32_t width(void) const { return segment_width; }
        inline float32_t height(void) const { return segment_height; }
        inline float32_t depth(void) const { return segment_depth; }

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };
}

std::ostream &operator<<(std::ostream &, const vdis::simulation_id_t &);
std::ostream &operator<<(std::ostream &, const vdis::entity_id_t &);
std::ostream &operator<<(std::ostream &, const vdis::event_id_t &);
std::ostream &operator<<(std::ostream &, const vdis::object_id_t &);
std::ostream &operator<<(std::ostream &, const vdis::entity_type_t &);
std::ostream &operator<<(std::ostream &, const vdis::object_type_t &);
std::ostream &operator<<(std::ostream &, const vdis::marking_t &);
std::ostream &operator<<(std::ostream &, const vdis::location12_t &);
std::ostream &operator<<(std::ostream &, const vdis::location24_t &);
std::ostream &operator<<(std::ostream &, const vdis::orientation_t &);
std::ostream &operator<<(std::ostream &, const vdis::velocity_t &);
std::ostream &operator<<(std::ostream &, const vdis::timestamp_t &);
std::ostream &operator<<(std::ostream &, const vdis::clocktime_t &);

#endif
