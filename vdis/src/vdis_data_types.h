#ifndef VDIS_DATA_TYPES_H
#define VDIS_DATA_TYPES_H

#include "vdis_enums.h"

namespace vdis
{
    class byte_stream_t;

    // ------------------------------------------------------------------------
    struct simulation_id_t
    {
        uint16_t                    site;                       // 2 bytes
        uint16_t                    application;                // 2 bytes

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
    struct id_t
    {
        uint16_t                    site;                       // 2 bytes
        uint16_t                    application;                // 2 bytes
        uint16_t                    identity;                   // 2 bytes

        bool is_none(void) const;
        bool is_all(void) const;

        inline void clear(void)
        {
            site = 0;
            application = 0;
            identity = 0;
        }

        void set(uint64_t value);
        uint64_t get(void) const;

        bool matches(const id_t &) const;

        bool operator==(const id_t &) const;
        bool operator!=(const id_t &) const;
        bool operator<(const id_t &) const;
        bool operator>(const id_t &) const;

        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t   NONE = 0;
        static const uint16_t   ALL = 65535;
    };

    // ------------------------------------------------------------------------
    struct entity_type_t
    {
        uint8_t                     kind;                       // 1 byte
        uint8_t                     domain;                     // 1 byte
        uint16_t                    country;                    // 2 bytes
        uint8_t                     category;                   // 1 byte
        uint8_t                     subcategory;                // 1 byte
        uint8_t                     specific;                   // 1 byte
        uint8_t                     extra;                      // 1 byte

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
        uint8_t                     domain;                     // 1 byte
        uint8_t                     kind;                       // 1 byte
        uint8_t                     category;                   // 1 byte
        uint8_t                     subcategory;                // 1 byte

        inline domain_e domain_enum(void) const
        {
            return (domain_e)domain;
        }

        inline object_kind_e kind_enum(void) const
        {
            return (object_kind_e)kind;
        }

        inline void clear(void)
        {
            domain = 0;
            kind = 0;
            category = 0;
            subcategory = 0;
        }

        void set(uint32_t value);
        uint32_t get(void) const;

        std::string description(object_geometry_e geometry) const;

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
        uint8_t                     character_set;              // 1 Byte
        char                        characters[11];             // 11 Bytes

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
            uint32_t                ammunition_supply:1;        // Bit 0
            uint32_t                fuel_supply:1;              // Bit 1
            uint32_t                recovery:1;                 // Bit 2
            uint32_t                repair:1;                   // Bit 3
            uint32_t                ads_broadcast:1;            // Bit 4
            uint32_t                sling_load_carrier:1;       // Bit 5
            uint32_t                sling_loadable:1;           // Bit 6
            uint32_t                ied_presence:1;             // Bit 7
            uint32_t                task_organizable:1;         // Bit 8
            uint32_t                padding:23;                 // Bits 9-31
        };

        uint32_t                    value;
        bits_t                      bits;

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct offset12_t
    {
        float32_t                   x;                          // 4 bytes
        float32_t                   y;                          // 4 bytes
        float32_t                   z;                          // 4 bytes

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
    struct location12_t
    {
        float32_t                   x;                          // 4 bytes
        float32_t                   y;                          // 4 bytes
        float32_t                   z;                          // 4 bytes

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
        float64_t                   x;                          // 8 bytes
        float64_t                   y;                          // 8 bytes
        float64_t                   z;                          // 8 bytes

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
    struct geodetic_location_t
    {
        float64_t                   latitude;                   // 8 bytes
        float64_t                   longitude;                  // 8 bytes
        float64_t                   elevation;                  // 8 bytes

        inline void clear(void)
        {
            latitude = 0;
            longitude = 0;
            elevation = 0;
        }

        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct orientation_t
    {
        float32_t                   psi;                        // 4 bytes
        float32_t                   theta;                      // 4 bytes
        float32_t                   phi;                        // 4 bytes

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
        float32_t                   x;                          // 4 bytes
        float32_t                   y;                          // 4 bytes
        float32_t                   z;                          // 4 bytes

        inline float32_t length(void) const
        {
            return std::sqrt(length_squared());
        }

        inline float32_t length_squared(void) const
        {
            return (x * x) + (y * y) + (z * z);
        }

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
        uint8_t                     algorithm;                  // 1 byte
        uint8_t                     parameters[15];             // 15 bytes
        velocity_t                  linear_acceleration;        // 12 bytes
        velocity_t                  angular_velocity;           // 12 bytes

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
        uint32_t                    value;                      // 4 bytes

        inline void clear(void)
        {
            value = 0;
        }

        uint64_t get_milliseconds(void) const;
        void set_milliseconds(uint64_t millis);
        void add_milliseconds(uint64_t millis);

        bool is_absolute(void) const;
        void set_absolute(bool absolute);

        void set(uint32_t minutes, float32_t seconds, bool absolute);
        void get(uint32_t &minutes, float32_t &seconds, bool &absolute) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct clocktime_t
    {
        uint32_t                    hours;                      // 4 bytes
        timestamp_t                 offset;                     // 4 bytes

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
        uint8_t                     protocol;                   // 1 byte
        uint8_t                     exercise;                   // 1 byte
        uint8_t                     type;                       // 1 byte
        uint8_t                     family;                     // 1 byte
        timestamp_t                 timestamp;                  // 4 bytes
        uint16_t                    length;                     // 2 bytes
        uint8_t                     status;                     // 1 byte
        uint8_t                     padding;                    // 1 byte

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

        bool is_valid(void) const;
        void reset(pdu_type_e);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct sling_line_t
    {
        float32_t                   line_length;                // 4 bytes
        float32_t                   line_offset;                // 4 bytes

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
        munition_type_t             munition;                   // 8 bytes
        uint16_t                    warhead;                    // 2 bytes
        uint16_t                    fuse;                       // 2 bytes
        uint16_t                    quantity;                   // 2 bytes
        uint16_t                    rate;                       // 2 bytes

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
    union spread_spectrum_t
    {
        struct bits_t
        {
            uint16_t                frequency_hopping:1;        // Bit 0
            uint16_t                pseudo_noise:1;             // Bit 1
            uint16_t                time_hopping:1;             // Bit 2
            uint16_t                padding:13;                 // Bits 3-15
        };

        uint16_t                    value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union encoding_scheme_t
    {
        struct bits_t
        {
            uint16_t                encoding_type:14;           // Bit 0-13
            uint16_t                encoding_class:2;           // Bits 14-15
        };

        uint16_t                    value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct modulation_type_t
    {
        spread_spectrum_t           spread_spectrum;            // 2 bytes
        uint16_t                    major_modulation;           // 2 bytes
        uint16_t                    modulation_detail;          // 2 bytes
        uint16_t                    radio_system;               // 2 bytes

        inline void clear(void)
        {
            spread_spectrum.clear();
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
        id_t                        entity;                     // 6 bytes
        uint8_t                     emitter;                    // 1 byte
        uint8_t                     beam;                       // 1 byte

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
        uint8_t                     data_length;                // 1 byte
        uint8_t                     id_number;                  // 1 byte
        uint16_t                    parameter_index;            // 2 bytes

        // Fundamental parameter data:
        float32_t                   frequency;                  // 4 bytes
        float32_t                   frequency_range;            // 4 bytes
        float32_t                   effective_radiated_power;   // 4 bytes
        float32_t                   pulse_repetition_frequency; // 4 bytes
        float32_t                   pulse_width;                // 4 bytes

        // Beam data:
        float32_t                   azimuth_center;             // 4 bytes
        float32_t                   azimuth_sweep;              // 4 bytes
        float32_t                   elevation_center;           // 4 bytes
        float32_t                   elevation_sweep;            // 4 bytes
        float32_t                   sweep_sync;                 // 4 bytes

        uint8_t                     function;                   // 1 byte
        uint8_t                     target_count;               // 1 byte
        uint8_t                     high_density_track_jam;     // 1 byte
        uint8_t                     status;                     // 1 byte
        uint32_t                    jamming_technique;          // 4 bytes
        emitter_target_t          **targets;                    // Variable

        emitter_beam_t(void);
        ~emitter_beam_t(void);

        inline const emitter_target_t *target(uint32_t i) const
        {
            return (targets and (i < target_count)) ? targets[i] : 0;
        }

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct emitter_system_t
    {
        uint8_t                     data_length;                // 1 byte
        uint8_t                     beam_count;                 // 1 byte
        uint16_t                    padding;                    // 2 bytes
        uint16_t                    name;                       // 2 bytes
        uint8_t                     function;                   // 1 byte
        uint8_t                     number;                     // 1 byte
        location12_t                location;                   // 12 bytes
        emitter_beam_t            **beams;                      // Variable

        emitter_system_t(void);
        ~emitter_system_t(void);

        inline const emitter_beam_t *beam(uint32_t i) const
        {
            return (beams and (i < beam_count)) ? beams[i] : 0;
        }

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union iff_change_options_t
    {
        // change           0=no initial report/no change
        //                  1=initial report/change
        // heartbeat        0=no, 1=yes
        // iff_mode         0=transponder, 1=interrogator
        // simulation_mode  0=regeneration, 1=interactive
        // interactive      0=incapable, 1=capable
        // test_mode        0=off, 1=on
        struct bits_t
        {
            uint8_t                 change:1;                   // Bit 0
            uint8_t                 specific1:1;                // Bit 1
            uint8_t                 specific2:1;                // Bit 2
            uint8_t                 heartbeat:1;                // Bit 3
            uint8_t                 iff_mode:1;                 // Bit 4
            uint8_t                 simulation_mode:1;          // Bit 5
            uint8_t                 interactive:1;              // Bit 6
            uint8_t                 test_mode:1;                // Bit 7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_system_id_t
    {
        uint16_t                    type;                       // 2 bytes
        uint16_t                    name;                       // 2 bytes
        uint8_t                     mode;                       // 1 byte
        iff_change_options_t        options;                    // 1 byte

        inline void clear(void)
        {
            type = 0;
            name = 0;
            mode = 0;
            options.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_operational_data_t
    {
        uint8_t                     status;                      // 1 byte
        uint8_t                     data_field_1;                // 1 byte
        uint8_t                     information_layers;          // 1 byte
        uint8_t                     data_field_2;                // 1 byte
        uint16_t                    parameter_1;                 // 2 bytes
        uint16_t                    parameter_2;                 // 2 bytes
        uint16_t                    parameter_3;                 // 2 bytes
        uint16_t                    parameter_4;                 // 2 bytes
        uint16_t                    parameter_5;                 // 2 bytes
        uint16_t                    parameter_6;                 // 2 bytes

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
        float32_t                   radiated_power;              // 4 bytes
        float32_t                   frequency;                   // 4 bytes
        float32_t                   interrogation_frequency;     // 4 bytes
        float32_t                   pulse_width;                 // 4 bytes
        float32_t                   burst_length;                // 4 bytes
        uint8_t                     applicable_modes;            // 1 byte
        uint8_t                     system_specific_data[3];     // 3 bytes

        inline void clear(void)
        {
            radiated_power = 0;
            frequency = 0;
            interrogation_frequency = 0;
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
    struct iff_layer1_data_t
    {
        id_t                        emitter;                    // 6 bytes
        id_t                        event;                      // 6 bytes
        location12_t                antenna_location;           // 12 bytes
        iff_system_id_t             system_id;                  // 6 bytes
        uint8_t                     system_designator;          // 1 byte
        uint8_t                     system_specific;            // 1 byte
        iff_operational_data_t      operational_data;           // 16 bytes

        inline void clear(void)
        {
            emitter.clear();
            event.clear();
            antenna_location.clear();
            system_id.clear();
            system_designator = 0;
            system_specific = 0;
            operational_data.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct iff_layer2_data_t
    {
        // Layer header
        uint8_t                     number;                     // 1 byte
        uint8_t                     specific_info;              // 1 byte
        uint16_t                    layer_length;               // 2 bytes

        // Beam data
        float32_t                   azimuth_center;             // 4 bytes
        float32_t                   azimuth_sweep;              // 4 bytes
        float32_t                   elevation_center;           // 4 bytes
        float32_t                   elevation_sweep;            // 4 bytes
        float32_t                   sweep_sync;                 // 4 bytes

        // Secondary operational data
        uint8_t                     parameter_1;                // 1 byte
        uint8_t                     parameter_2;                // 1 byte
        uint16_t                    parameter_data_count;       // 2 bytes
        iff_parameter_data_t      **parameter_data;             // Variable

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
        uint32_t                    type;                       // 4 bytes
        uint16_t                    data_length;                // 2 bytes
        uint8_t                     index;                      // 1 byte
        uint8_t                     padding;                    // 1 byte
        uint8_t                    *data;                       // Variable

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
        uint8_t                     id;                         // 1 byte
        uint8_t                     transition;                 // 1 byte
        uint8_t                     current_state;              // 1 byte
        uint8_t                     requested_state;            // 1 byte

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
    union object_modification_t
    {
        struct bits_t
        {
            uint8_t                 modified_location:1;        // Bit 0
            uint8_t                 modified_orientation:1;     // Bit 1
            uint8_t                 padding:6;                  // Bits 2-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union generic_object_appearance_t
    {
        struct bits_t
        {
            uint16_t                percent_complete:8;         // Bits 0-7
            uint16_t                damage:2;                   // Bits 8-9
            uint16_t                predistributed:1;           // Bits 10
            uint16_t                deactivated:1;              // Bits 11
            uint16_t                smoking:1;                  // Bits 12
            uint16_t                flaming:1;                  // Bits 13
            uint16_t                padding:2;                  // Bits 14-15
        };

        uint16_t                    value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct linear_segment_t
    {
        uint8_t                     number;                     // 1 byte
        object_modification_t       modifications;              // 1 byte
        generic_object_appearance_t generic_appearance;         // 2 bytes
        uint32_t                    specific_appearance;        // 4 bytes
        location24_t                location;                   // 24 bytes
        orientation_t               orientation;                // 12 bytes

// In the latest standard these are represented as 32-bit floating point
// numbers, not the 16-bit unsigned integers that AVCATT/OneSAF uses.
//
#ifdef LINEAR_SEGMENTS_USE_32BIT_FLOAT
        float32_t                   segment_length;             // 4 bytes
        float32_t                   segment_width;              // 4 bytes
        float32_t                   segment_height;             // 4 bytes
        float32_t                   segment_depth;              // 4 bytes
#else
        uint16_t                    segment_length;             // 2 bytes
        uint16_t                    segment_width;              // 2 bytes
        uint16_t                    segment_height;             // 2 bytes
        uint16_t                    segment_depth;              // 2 bytes
#endif

        uint32_t                    padding;                    // 4 bytes

        inline float32_t length(void) const { return segment_length; }
        inline float32_t width(void) const { return segment_width; }
        inline float32_t height(void) const { return segment_height; }
        inline float32_t depth(void) const { return segment_depth; }

        inline void clear(void)
        {
            number = 0;
            modifications.clear();
            generic_appearance.clear();
            specific_appearance = 0;
            location.clear();
            orientation.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static void using_type(const object_type_t *type_ptr);
    };

    // ------------------------------------------------------------------------
    union hifi_light_flags_t
    {
        struct bits_t
        {
            uint8_t                 powered_on:1;               // Bit 0
            uint8_t                 attached:1;                 // Bit 1
            uint8_t                 directional:1;              // Bit 2
            uint8_t                 feather_enabled:1;          // Bit 3
            uint8_t                 padding:4;                  // Bits 4-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_engine_rates_t
    {
        float32_t                   generator;                  // 4 bytes (%)
        float32_t                   turbine;                    // 4 bytes (%)
        float32_t                   fuel_flow;                  // 4 bytes

        inline void clear(void)
        {
            generator = 0;
            turbine = 0;
            fuel_flow = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_engine_transmission_t
    {
        struct bits_t
        {
            uint8_t                 high_temperature:1;         // Bit 0
            uint8_t                 low_pressure:1;             // Bit 1
            uint8_t                 debris:1;                   // Bit 2
            uint8_t                 chips:1;                    // Bit 3
            uint8_t                 engine_chips:1;             // Bit 4
            uint8_t                 padding:3;                  // Bits 5-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_transmission_t
    {
        struct bits_t
        {
            uint8_t                 high_temperature:1;         // Bit 0
            uint8_t                 low_pressure_auxiliary:1;   // Bit 1
            uint8_t                 low_pressure_main:1;        // Bit 2
            uint8_t                 debris:1;                   // Bit 3
            uint8_t                 chips:1;                    // Bit 4
            uint8_t                 padding:3;                  // Bits 5-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_comb_transmission_t
    {
        struct bits_t
        {
            uint8_t                 high_temperature:1;         // Bit 0
            uint8_t                 low_pressure_auxiliary:1;   // Bit 1
            uint8_t                 low_pressure_main:1;        // Bit 2
            uint8_t                 debris_right:1;             // Bit 3
            uint8_t                 debris_left:1;              // Bit 3
            uint8_t                 chips:1;                    // Bit 5
            uint8_t                 padding:2;                  // Bits 6-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_shaft_t
    {
        struct bits_t
        {
            uint8_t                 low_pressure:1;             // Bit 0
            uint8_t                 chips:1;                    // Bit 1
            uint8_t                 padding:6;                  // Bits 2-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_ground_contact_t
    {
        struct bits_t
        {
            uint8_t                 right_switch:1;             // Bit 0
            uint8_t                 left_switch:1;              // Bit 1
            uint8_t                 padding:6;                  // Bits 2-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_control_malfunctions_t
    {
        struct bits_t
        {
            uint8_t                 pump_fail:1;                // Bit 0
            uint8_t                 return_line_filter:1;       // Bit 1
            uint8_t                 pressure_line_filter:1;     // Bit 2
            uint8_t                 padding:5;                  // Bits 3-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_utility_malfunctions_t
    {
        struct bits_t
        {
            uint8_t                 utility_pump_fail:1;        // Bit 0
            uint8_t                 apu_pump_fail:1;            // Bit 1
            uint8_t                 return_line_filter:1;       // Bit 2
            uint8_t                 pressure_line_filter:1;     // Bit 3
            uint8_t                 padding:4;                  // Bits 4-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_uh72_controls_t
    {
        struct bits_t
        {
            uint8_t                 hoist_cable_cut:1;          // Bit 0
            uint8_t                 water_bucket_valve_open:1;  // Bit 1
            uint8_t                 over_torque_warning:1;      // Bit 2
            uint8_t                 padding:5;                  // Bits 3-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_hook_safety_t
    {
        struct bits_t
        {
            uint8_t                 uh60_emergency_release:1;   // Bit 0
            uint8_t                 hook_safe:1;                // Bit 1
            uint8_t                 padding:6;                  // Bits 2-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_flare_state_t
    {
        struct bits_t
        {
            uint16_t                safe:1;                     // Bit 0
            uint16_t                padding:15;                 // Bits 1-15
        };

        uint16_t                    value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_cargo_hook_t
    {
        struct bits_t
        {
            uint16_t                locked_open:1;              // Bit 0
            uint16_t                padding:15;                 // Bits 1-15
        };

        uint16_t                    value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct ncm3_water_bucket_t
    {
        float32_t                   fill_level;                 // 4 bytes
        uint16_t                    size;                       // 2 bytes
        uint8_t                     padding;                    // 1 byte
        uint8_t                     releasing;                  // 1 byte

        inline void clear(void)
        {
            fill_level = 0;
            size = 0;
            padding = 0;
            releasing = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    union ncm3_hoist_status_t
    {
        struct bits_t
        {
            uint8_t                 cable_reset:1;              // Bit 0
            uint8_t                 cable_cut:1;                // Bit 1
            uint8_t                 device_open:1;              // Bit 2
            uint8_t                 device_type:3;              // Bits 3-5
            uint8_t                 padding:2;                  // Bits 6-7
        };

        uint8_t                     value;
        bits_t                      bits;

        inline void clear(void)
        {
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct ncm3_hoist_t
    {
        location24_t                rescue_device_location;     // 24 bytes
        orientation_t               rescue_device_orientation;  // 12 bytes
        float32_t                   boom_angle;                 // 4 bytes
        location24_t                hook_location;              // 24 bytes
        orientation_t               hook_orientation;           // 12 bytes
        ncm3_hoist_status_t         status;                     // 1 byte
        uint8_t                     hoist_device_id;            // 1 byte
        uint8_t                     padding[2];                 // 2 bytes

        inline void clear(void)
        {
            rescue_device_location.clear();
            rescue_device_orientation.clear();
            boom_angle = 0;
            hook_location.clear();
            hook_orientation.clear();
            status.clear();
            padding[0] = 0;
            padding[1] = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct ncm3_hydraulic_system_t
    {
        uint16_t                     padding;                   // 2 bytes
        uint16_t                     temperature;               // 2 bytes
        uint16_t                     pressure;                  // 2 bytes
        uint16_t                     reservoir_level;           // 2 bytes

        inline void clear(void)
        {
            padding = 0;
            temperature = 0;
            pressure = 0;
            reservoir_level = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct ncm3_maintenance_cautions_t
    {
        uint8_t                     padding;                    // 1 byte
        ncm3_engine_transmission_t  engine_transmission1;       // 1 byte
        ncm3_engine_transmission_t  engine_transmission2;       // 1 byte
        ncm3_transmission_t         forward_transmission;       // 1 byte
        ncm3_comb_transmission_t    combined_transmission;      // 1 byte
        ncm3_transmission_t         aft_transmission;           // 1 byte
        ncm3_shaft_t                aft_shaft;                  // 1 byte
        ncm3_ground_contact_t       ground_contact;             // 1 byte

        inline void clear(void)
        {
            padding = 0;
            engine_transmission1.clear();
            engine_transmission2.clear();
            forward_transmission.clear();
            combined_transmission.clear();
            aft_transmission.clear();
            aft_shaft.clear();
            ground_contact.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct ncm3_hydraulic_cautions_t
    {
        uint8_t                     padding;                    // 1 byte
        ncm3_control_malfunctions_t flight_control1;            // 1 byte
        ncm3_control_malfunctions_t flight_control2;            // 1 byte
        ncm3_utility_malfunctions_t utility;                    // 1 byte

        inline void clear(void)
        {
            padding = 0;
            flight_control1.clear();
            flight_control2.clear();
            utility.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct ncm3_hydraulic_gauges_t
    {
        ncm3_hydraulic_system_t     flight_control1;            // 8 bytes
        ncm3_hydraulic_system_t     flight_control2;            // 8 bytes
        ncm3_hydraulic_system_t     utility;                    // 8 bytes

        inline void clear(void)
        {
            flight_control1.clear();
            flight_control2.clear();
            utility.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };
}

// Stream out operators for structures that get printed on a single line
// with the need for prefixes on multiple lines
//
std::ostream &operator<<(std::ostream &, const vdis::simulation_id_t &);
std::ostream &operator<<(std::ostream &, const vdis::id_t &);
std::ostream &operator<<(std::ostream &, const vdis::entity_type_t &);
std::ostream &operator<<(std::ostream &, const vdis::object_type_t &);
std::ostream &operator<<(std::ostream &, const vdis::marking_t &);
std::ostream &operator<<(std::ostream &, const vdis::offset12_t &);
std::ostream &operator<<(std::ostream &, const vdis::location12_t &);
std::ostream &operator<<(std::ostream &, const vdis::location24_t &);
std::ostream &operator<<(std::ostream &, const vdis::geodetic_location_t &);
std::ostream &operator<<(std::ostream &, const vdis::orientation_t &);
std::ostream &operator<<(std::ostream &, const vdis::velocity_t &);
std::ostream &operator<<(std::ostream &, const vdis::timestamp_t &);
std::ostream &operator<<(std::ostream &, const vdis::clocktime_t &);

#endif
