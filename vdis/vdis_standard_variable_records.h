#ifndef VDIS_STANDARD_VARIABLE_RECORDS_H
#define VDIS_STANDARD_VARIABLE_RECORDS_H

#include "vdis_byte_stream.h"
#include "vdis_data_types.h"
#include "vdis_enums.h"

namespace vdis
{
    // Standard Variable (SV) records are used by the following:
    //
    //    Entity Damage Status PDU
    //    Application Control PDU
    //
    struct standard_variable_record_t;

    // Returns array with length equal to 'count'
    //
    standard_variable_record_t **read_standard_variable_records(
        byte_stream_t &stream,
        uint16_t count
    );

    // ------------------------------------------------------------------------
    struct standard_variable_content_t
    {
        virtual ~standard_variable_content_t(void) { }

        // Returns length in bytes of entire record (with type and length)
        //
        virtual uint16_t length(void) const = 0;

        virtual void clear(void) = 0;
        virtual void print(const string_t &, std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;

        uint16_t read_length(byte_stream_t &stream);
    };

    // ------------------------------------------------------------------------
    struct standard_variable_record_t
    {
        uint32_t                     type;               // 4 bytes
        standard_variable_content_t *content_ptr;        // Variable length

        standard_variable_record_t(void) : type(0), content_ptr(0) { }
        ~standard_variable_record_t(void) { clear(); }

        inline datum_ids_e datum_id_enum(void) const
        {
            return (vdis::datum_ids_e)type;
        }

        inline uint16_t length(void) const
        {
            return (content_ptr ? content_ptr->length() : 0);
        }

        inline void clear(void)
        {
            if (content_ptr)
            {
                delete content_ptr;
            }

            type = 0;
            content_ptr = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct default_standard_variable_content_t : standard_variable_content_t
    {
        byte_buffer_t               buffer;

        ~default_standard_variable_content_t(void) { clear(); }

        inline uint16_t length(void) const { return (buffer.length() + 6); }

        inline void clear(void)
        {
            buffer.clear();
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct application_state_t : public standard_variable_content_t
    {
        uint8_t                     padding;                // 1 byte
        uint8_t                     transition;             // 1 byte
        uint8_t                     current_state;          // 1 byte
        uint8_t                     requested_state;        // 1 byte
        uint8_t                     current_configuration;  // 1 byte
        uint8_t                     exercise_count;         // 1 byte
        exercise_state_t          **exercises;              // 4 bytes each

        ~application_state_t(void) { clear(); }

        inline transition_e transition_enum(void) const
        {
            return (transition_e)transition;
        }

        inline app_state_e current_state_enum(void) const
        {
            return (app_state_e)current_state;
        }

        inline app_state_e requested_state_enum(void) const
        {
            return (app_state_e)requested_state;
        }

        uint16_t length(void) const;

        void clear(void);
        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            BASE_LENGTH_BYTES = 6;
    };

    // ------------------------------------------------------------------------
    struct application_health_status_t : public standard_variable_content_t
    {
        uint16_t                    padding;                // 2 bytes
        uint16_t                    status_type;            // 2 bytes
        uint8_t                     general_status;         // 1 byte
        uint8_t                     specific_status;        // 1 byte
        int32_t                     value_integer;          // 4 bytes
        float64_t                   value_float;            // 8 bytes

        ~application_health_status_t(void) { clear(); }

        inline app_general_status_e general_status_enum(void) const
        {
            return (app_general_status_e)general_status;
        }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            padding = 0;
            status_type = 0;
            general_status = 0;
            specific_status = 0;
            value_integer = 0;
            value_float = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 24;
    };

    // ------------------------------------------------------------------------
    struct stealth_spectrum_t : public standard_variable_content_t
    {
        uint8_t                     spectrum_type;          // 1 byte
        uint8_t                     invert;                 // 1 byte
        float32_t                   magnification;          // 4 bytes
        uint16_t                    sensor_mode;            // 2 bytes
        uint16_t                    padding;                // 2 bytes

        ~stealth_spectrum_t(void) { clear(); }

        inline stealth_spectrum_type_e spectrum_type_enum(void) const
        {
            return (stealth_spectrum_type_e)spectrum_type;
        }

        inline yes_no_e invert_enum(void) const
        {
            return (yes_no_e)invert;
        }

        inline stealthstate_sensor_mode_e sensor_mode_enum(void) const
        {
            return (stealthstate_sensor_mode_e)sensor_mode;
        }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            spectrum_type = 0;
            invert = 0;
            magnification = 0;
            sensor_mode = 0;
            padding = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 16;
    };

    // ------------------------------------------------------------------------
    struct stealth_location_t : public standard_variable_content_t
    {
        uint16_t                    padding16;              // 2 bytes
        location24_t                location;               // 24 bytes
        orientation_t               orientation;            // 12 bytes
        uint32_t                    padding32;              // 4 bytes

        ~stealth_location_t(void) { clear(); }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            padding16 = 0;
            location.clear();
            orientation.clear();
            padding32 = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 48;
    };

    // ------------------------------------------------------------------------
    struct stealth_attachment_t : public standard_variable_content_t
    {
        uint16_t                    padding16;              // 2 bytes
        id_t                        attached_entity;        // 6 bytes
        uint8_t                     attach_command;         // 1 byte
        uint8_t                     attach_mode;            // 1 byte
        location12_t                relative_location;      // 12 bytes
        orientation_t               relative_orientation;   // 12 bytes

        ~stealth_attachment_t(void) { clear(); }

        inline stealth_attach_command_e attach_command_enum(void) const
        {
            return (stealth_attach_command_e)attach_command;
        }

        inline stealth_attach_mode_e attach_mode_enum(void) const
        {
            return (stealth_attach_mode_e)attach_mode;
        }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            padding16 = 0;
            attached_entity.clear();
            attach_command = 0;
            attach_mode = 0;
            relative_location.clear();
            relative_orientation.clear();
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 40;
    };

    // ------------------------------------------------------------------------
    struct stealth_velocity_t : public standard_variable_content_t
    {
        uint8_t                     relative;               // 1 byte
        uint8_t                     algorithm;              // 1 byte
        velocity_t                  linear_velocity;        // 12 bytes
        velocity_t                  angular_velocity;       // 12 bytes
        velocity_t                  linear_acceleration;    // 12 bytes
        uint32_t                    padding32;              // 4 bytes

        ~stealth_velocity_t(void) { clear(); }

        inline yes_no_e relative_enum(void) const
        {
            return (yes_no_e)relative;
        }

        inline dead_reckoning_e algorithm_enum(void) const
        {
            return (dead_reckoning_e)algorithm;
        }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            relative = 0;
            algorithm = 0;
            linear_velocity.clear();
            angular_velocity.clear();
            linear_acceleration.clear();
            padding32 = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 48;
    };

    // ------------------------------------------------------------------------
    struct stealth_field_of_view_t : public standard_variable_content_t
    {
        uint16_t                    padding16;              // 2 bytes
        float32_t                   horizontal;             // 4 bytes
        float32_t                   vertical;               // 4 bytes

        ~stealth_field_of_view_t(void) { clear(); }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            padding16 = 0;
            horizontal = 0.0f;
            vertical = 0.0f;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 16;
    };

    // ------------------------------------------------------------------------
    struct stealth_marking_id_t : public standard_variable_content_t
    {
        uint16_t                    id;                     // 2 bytes
        marking_t                   marking;                // 12 bytes
        uint32_t                    padding32;              // 4 bytes

        ~stealth_marking_id_t(void) { clear(); }

        inline uint16_t length(void) const { return LENGTH_BYTES; }

        inline void clear(void)
        {
            id = 0;
            marking.clear();
            padding32 = 0;
        }

        void print(const string_t &, std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint16_t
            LENGTH_BYTES = 24;
    };
}

#endif
