#ifndef VDIS_DATUM_RECORDS_H
#define VDIS_DATUM_RECORDS_H

#include "vdis_byte_buffer.h"
#include "vdis_data_types.h"

namespace vdis
{
    class byte_stream_t;

    struct fixed_datum_record_t;
    struct variable_datum_record_t;

    fixed_datum_record_t **read_fixed_datum_records(
        byte_stream_t &stream,
        uint32_t fixed_count
    );

    variable_datum_record_t **read_variable_datum_records(
        byte_stream_t &stream,
        uint32_t variable_count
    );

    bool write_fixed_datum_records(
        byte_stream_t &stream,
        fixed_datum_record_t **fixed_records,
        uint32_t fixed_count
    );

    bool write_variable_datum_records(
        byte_stream_t &stream,
        variable_datum_record_t **variable_records,
        uint32_t variable_count
    );

    // ------------------------------------------------------------------------
    struct fixed_datum_record_t
    {
        uint32_t                    id;                     // 4 bytes
        uint32_t                    value;                  // 4 bytes

        inline datum_ids_e datum_id_enum(void) const
        {
            return (datum_ids_e)id;
        }

        inline void clear(void)
        {
            id = 0;
            value = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct variable_datum_content_t
    {
        virtual ~variable_datum_content_t(void) { }

        uint32_t read_length(byte_stream_t &stream);

        // Returns datum length in bits
        //
        virtual uint32_t length(void) const = 0;

        virtual void clear(void) = 0;
        virtual void print(const std::string &, std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;
    };

    // ------------------------------------------------------------------------
    struct variable_datum_record_t
    {
        uint32_t                    id;                     // 4 bytes
        variable_datum_content_t   *content_ptr;            // Variable length

        variable_datum_record_t(void);
        ~variable_datum_record_t(void);

        inline datum_ids_e datum_id_enum(void) const
        {
            return (datum_ids_e)id;
        }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const
        {
            return (content_ptr ? content_ptr->length() : 0);
        }

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct default_variable_datum_content_t : variable_datum_content_t
    {
        byte_buffer_t               buffer;

        // Returns datum length in bits
        //
        inline uint32_t length(void) const
        {
            return (buffer.length() * 8);
        }

        inline void clear(void)
        {
            buffer.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct damage_status_t : variable_datum_content_t
    {
        uint16_t                    casualties;             // 2 bytes
        uint8_t                     cause;                  // 1 byte
        uint8_t                     padding1;               // 1 byte
        id_t                        event_id;               // 6 bytes
        uint16_t                    extent;                 // 2 bytes
        uint32_t                    padding2;               // 4 bytes

        inline damage_cause_e cause_enum(void) const
        {
            return (damage_cause_e)cause;
        }

        inline damage_extent_e extent_enum(void) const
        {
            return (damage_extent_e)extent;
        }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            casualties = 0;
            cause = 0;
            padding1 = 0;
            event_id.clear();
            extent = 0;
            padding2 = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 128; // Bits
    };

    // ------------------------------------------------------------------------
    struct sling_load_capability_t : variable_datum_content_t
    {
        id_t                        payload;                // 6 bytes
        id_t                        carrier;                // 6 bytes
        float32_t                   drag_coeffficient;      // 4 bytes
        float32_t                   current_mass;           // 4 bytes
        uint16_t                    padding;                // 2 bytes
        uint8_t                     hook_type;              // 1 byte
        uint8_t                     lines_needed;           // 1 byte
        sling_line_t              **lines;                  // N bytes (array)

        sling_load_capability_t(void);
        virtual ~sling_load_capability_t(void);

        inline hook_type_e hook_type_enum(void) const
        {
            return (hook_type_e)hook_type;
        }

        // Returns datum length in bits
        //
        uint32_t length(void) const;

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            BASE_LENGTH_BITS = 192; // Bits
    };

    // ------------------------------------------------------------------------
    struct force_id_affiliation_t : variable_datum_content_t
    {
        uint16_t                    force;                  // 2 bytes
        uint16_t                    padding;                // 2 bytes
        byte_buffer_t               name;                   // N bytes (array)

        virtual ~force_id_affiliation_t(void)
        {
            clear();
        }

        inline force_id_e force_enum(void) const
        {
            return (force_id_e)force;
        }

        std::string force_name(void) const;

        // Returns datum length in bits
        //
        uint32_t length(void) const;

        void clear(void);
        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            BASE_LENGTH_BITS = 32; // Bits
    };

    // ------------------------------------------------------------------------
    struct avcatt_to_ncm3_01_t : variable_datum_content_t
    {
        location24_t                ownship_gcc;                // 24 bytes
        geodetic_location_t         ownship_gdc;                // 24 bytes
        orientation_t               ownship_orientation;        // 12 bytes
        velocity_t                  ownship_velocity;           // 4 bytes
        id_t                        load_id;                    // 4 bytes
        uint16_t                    frozen;                     // 4 bytes
        location24_t                load_gcc;                   // 24 bytes
        uint32_t                    load_attached;              // 4 bytes
        orientation_t               load_orientation;           // 12 bytes
        uint32_t                    search_light;               // 4 bytes
        float32_t                   search_light_azimuth;       // 4 bytes
        float32_t                   search_light_elevation;     // 4 bytes
        uint32_t                    water_bucket_full;          // 4 bytes
        uint16_t                    place_survivor_on_hoist;    // 2 bytes
        id_t                        survivor_id;                // 6 bytes
        uint32_t                    hook_type;                  // 4 bytes
        uint32_t                    malfunction;                // 4 bytes
        uint32_t                    collision;                  // 4 bytes
        uint32_t                    ownship_appearance;         // 4 bytes
        uint32_t                    ownship_update;             // 4 bytes
        float32_t                   sling_tightness_center;     // 4 bytes
        float32_t                   sling_tightness_forward;    // 4 bytes
        float32_t                   sling_tightness_aft;        // 4 bytes
        float32_t                   rotor_roll;                 // 4 bytes
        float32_t                   rotor_pitch;                // 4 bytes
        float32_t                   rotor_coning_forward;       // 4 bytes
        float32_t                   rotor_coning_aft;           // 4 bytes
        float32_t                   rotor_speed;                // 4 bytes
        float32_t                   lift_vibration_frequency;   // 4 bytes
        float32_t                   lift_vibration_amplitude;   // 4 bytes
        ncm3_engine_rates_t         engine_rates1;              // 12 bytes
        ncm3_engine_rates_t         engine_rates2;              // 12 bytes
        float32_t                   total_airspeed;             // 4 bytes
        uint32_t                    weight_on_wheel_left;       // 4 bytes
        uint32_t                    weight_on_wheel_right;      // 4 bytes
        uint32_t                    weight_on_wheel_aft;        // 4 bytes
        float32_t                   groundspeed_x;              // 4 bytes
        float32_t                   groundspeed_y;              // 4 bytes
        uint32_t                    gear_squeal_left;           // 4 bytes
        uint32_t                    gear_squeal_right;          // 4 bytes
        uint32_t                    gear_squeal_aft;            // 4 bytes

        virtual ~avcatt_to_ncm3_01_t(void) { }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            ownship_gcc.clear();
            ownship_gdc.clear();
            ownship_orientation.clear();
            ownship_velocity.clear();
            load_id.clear();
            frozen = 0;
            load_gcc.clear();
            load_attached = 0;
            load_orientation.clear();
            search_light = 0;
            search_light_azimuth = 0;
            search_light_elevation = 0;
            water_bucket_full = 0;
            place_survivor_on_hoist = 0;
            survivor_id.clear();
            hook_type = 0;
            malfunction = 0;
            collision = 0;
            ownship_appearance = 0;
            ownship_update = 0;
            sling_tightness_center = 0;
            sling_tightness_forward = 0;
            sling_tightness_aft = 0;
            rotor_roll = 0;
            rotor_pitch = 0;
            rotor_coning_forward = 0;
            rotor_coning_aft = 0;
            rotor_speed = 0;
            lift_vibration_frequency = 0;
            lift_vibration_amplitude = 0;
            engine_rates1.clear();
            engine_rates2.clear();
            total_airspeed = 0;
            weight_on_wheel_left = 0;
            weight_on_wheel_right = 0;
            weight_on_wheel_aft = 0;
            groundspeed_x = 0;
            groundspeed_y = 0;
            gear_squeal_left = 0;
            gear_squeal_right = 0;
            gear_squeal_aft = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 2112; // Bits (264 bytes)
    };

    // ------------------------------------------------------------------------
    struct ncm3_to_avcatt_01_t : variable_datum_content_t
    {
        uint32_t                    configuration;              // 4 bytes
        uint32_t                    gun_trigger_left;           // 4 bytes
        uint32_t                    gun_trigger_right;          // 4 bytes
        uint32_t                    gun_trigger_aft;            // 4 bytes
        uint32_t                    hook_animation_state;       // 4 bytes
        uint32_t                    released_water;             // 4 bytes
        uint32_t                    released_cargo;             // 4 bytes
        uint32_t                    released_flare;             // 4 bytes
        uint32_t                    hoist_medic_state;           // 4 bytes
        uint32_t                    hoist_survivor_state;       // 4 bytes
        uint32_t                    hoist_available;            // 4 bytes
        orientation_t               medic_orientation;          // 12 bytes
        location24_t                medic_location;             // 24 bytes
        location24_t                survivor_location;          // 24 bytes
        orientation_t               survivor_orientation;       // 12 bytes
        offset12_t                  sling_load_offset;          // 12 bytes
        orientation_t               sling_load_orientation;     // 12 bytes
        uint32_t                    padding;

        virtual ~ncm3_to_avcatt_01_t(void) { }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            configuration = 0;
            gun_trigger_left = 0;
            gun_trigger_right = 0;
            gun_trigger_aft = 0;
            hook_animation_state = 0;
            released_water = 0;
            released_cargo = 0;
            released_flare = 0;
            hoist_medic_state = 0;
            hoist_survivor_state = 0;
            hoist_available = 0;
            medic_orientation.clear();
            medic_location.clear();
            survivor_location.clear();
            survivor_orientation.clear();
            sling_load_offset.clear();
            sling_load_orientation.clear();
            padding = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 1152; // Bits (144 bytes)
    };

    // ------------------------------------------------------------------------
    struct avcatt_to_ncm3_02_t : variable_datum_content_t
    {
        ncm3_maintenance_cautions_t maintenance_cautions;   // 8 bytes
        ncm3_hydraulic_cautions_t   hydraulic_cautions;     // 4 bytes
        ncm3_hydraulic_gauges_t     hydraulic_gauges;       // 24 bytes
        ncm3_hook_safety_t          hook_safety;            // 1 byte
        ncm3_uh72_controls_t        uh72_controls;          // 1 byte
        uint16_t                    padding;                // 2 bytes

        virtual ~avcatt_to_ncm3_02_t(void) { }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            maintenance_cautions.clear();
            hydraulic_cautions.clear();
            hydraulic_gauges.clear();
            hook_safety.clear();
            uh72_controls.clear();
            padding = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 320; // Bits (40 bytes)
    };

    // ------------------------------------------------------------------------
    struct ncm3_to_avcatt_02_t : variable_datum_content_t
    {
        ncm3_flare_state_t          flare_state;            // 2 bytes
        ncm3_cargo_hook_t           cargo_hook_state;       // 2 bytes
        uint32_t                    padding;                // 4 bytes
        ncm3_water_bucket_t         water_bucket_state;     // 8 bytes
        ncm3_hoist_t                hoist_data;             // 80 bytes

        virtual ~ncm3_to_avcatt_02_t(void) { }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            flare_state.clear();
            cargo_hook_state.clear();
            padding = 0;
            water_bucket_state.clear();
            hoist_data.clear();
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 768; // Bits (96 bytes)
    };

    // ------------------------------------------------------------------------
    struct avcatt_to_ncm3_03_t : variable_datum_content_t
    {
        uint16_t                    flare_count;            // 2 bytes
        uint8_t                     padding[5];             // 5 bytes
        uint8_t                     missile_audio_warning;  // 1 byte

        virtual ~avcatt_to_ncm3_03_t(void) { }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            flare_count = 0;
            padding[0] = 0;
            padding[1] = 0;
            padding[2] = 0;
            padding[3] = 0;
            padding[4] = 0;
            missile_audio_warning = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 64; // Bits (8 bytes)
    };

    // ------------------------------------------------------------------------
    struct ncm3_to_avcatt_03_t : variable_datum_content_t
    {
        uint64_t                    padding;                // 8 bytes

        virtual ~ncm3_to_avcatt_03_t(void) { }

        // Returns datum length in bits
        //
        inline uint32_t length(void) const { return LENGTH_BITS; }

        inline void clear(void)
        {
            padding = 0;
        }

        void print(const std::string &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            LENGTH_BITS = 64; // Bits (8 bytes)
    };
}

#endif
