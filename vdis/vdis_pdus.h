#ifndef VDIS_PDUS_H
#define VDIS_PDUS_H

#include "vdis_data_types.h"

// Base lengths of PDUs as defined in specification (includes header but
// not supplemental records)
//
#define ENTITY_STATE_PDU_SIZE       144
#define FIRE_PDU_SIZE                96
#define DETONATION_PDU_SIZE         104
#define COLLISION_PDU_SIZE           60
#define CREATE_ENTITY_PDU_SIZE       28
#define REMOVE_ENTITY_PDU_SIZE       28
#define START_RESUME_PDU_SIZE        44
#define STOP_FREEZE_PDU_SIZE         40
#define ACKNOWLEDGE_PDU_SIZE         32
#define ACTION_REQUEST_PDU_SIZE      40
#define ACTION_RESPONSE_PDU_SIZE     40
#define DATA_QUERY_PDU_SIZE          40
#define SET_DATA_PDU_SIZE            40
#define DATA_PDU_SIZE                40
#define EVENT_REPORT_PDU_SIZE        40
#define COMMENT_PDU_SIZE             32
#define EM_EMISSION_PDU_SIZE         28
#define DESIGNATOR_PDU_SIZE          88

namespace vdis
{
    class byte_stream_t;

    struct base_pdu_t;
    struct fixed_datum_record_t;
    struct variable_datum_record_t;
    struct vp_record_t;

    // ------------------------------------------------------------------------
    class pdu_t
    {
      public:

        pdu_t(byte_stream_t &);
        ~pdu_t(void);

        void clear(void);

        inline const base_pdu_t *base(void) const { return base_ptr; }

        static bool is_ignoring_invalid_headers(void);
        static void set_ignoring_invalid_headers(bool value);

      protected:

        base_pdu_t
            *base_ptr;

      private:

        static bool validate_header(byte_buffer_t &);

        static bool ignore_invalid_headers;
    };

    // ------------------------------------------------------------------------
    struct base_pdu_t
    {
        pdu_header_t            header;             // 12 bytes

        virtual ~base_pdu_t(void) { }

        virtual void clear(void) = 0;
        virtual void print(std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;
    };

    // ------------------------------------------------------------------------
    struct default_pdu_t : base_pdu_t
    {
        byte_buffer_t           content;            // Variable length

        virtual ~default_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_state_pdu_t : base_pdu_t
    {
        entity_id_t             id;                 // 6 bytes
        uint8_t                 force;              // 1 byte
        uint8_t                 vp_record_count;    // 1 byte
        entity_type_t           type;               // 8 bytes
        entity_type_t           alternate_type;     // 8 bytes
        velocity_t              velocity;           // 12 bytes
        location24_t            location;           // 24 bytes
        orientation_t           orientation;        // 12 bytes
        uint32_t                appearance;         // 4 bytes
        dead_reckoning_t        dead_reckoning;     // 40 bytes
        entity_marking_t        marking;            // 12 bytes
        entity_capabilities_t   capabilities;       // 4 bytes
        vp_record_t           **vp_records;         // 16 bytes each

        entity_state_pdu_t(void);
        ~entity_state_pdu_t(void);

        inline const vp_record_t *vp_record(uint32_t i) const
        {
            return (vp_records and (i < vp_record_count)) ? vp_records[i] : 0;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct fire_pdu_t : base_pdu_t
    {
        entity_id_t             shooter;            // 6 bytes
        entity_id_t             target;             // 6 bytes
        munition_id_t           munition;           // 6 bytes
        event_id_t              event;              // 6 bytes
        uint32_t                fire_mission_index; // 4 bytes
        location24_t            world_location;     // 24 bytes
        burst_descriptor_t      burst_descriptor;   // 16 bytes
        velocity_t              velocity;           // 12 bytes
        float32_t               range;              // 4 bytes

        ~fire_pdu_t(void) { }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct detonation_pdu_t : base_pdu_t
    {
        entity_id_t             shooter;            // 6 bytes
        entity_id_t             target;             // 6 bytes
        munition_id_t           munition;           // 6 bytes
        event_id_t              event;              // 6 bytes
        velocity_t              velocity;           // 12 bytes
        location24_t            world_location;     // 24 bytes
        burst_descriptor_t      burst_descriptor;   // 16 bytes
        location12_t            entity_location;    // 12 bytes
        uint8_t                 result;             // 1 byte
        uint8_t                 vp_record_count;    // 1 byte
        uint16_t                padding;            // 2 bytes
        vp_record_t           **vp_records;         // 16 bytes each

        detonation_pdu_t(void);
        ~detonation_pdu_t(void);

        inline const vp_record_t *vp_record(uint32_t i) const
        {
            return (vp_records and (i < vp_record_count)) ? vp_records[i] : 0;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct collision_pdu_t : base_pdu_t
    {
        entity_id_t             issuing_entity;     // 6 bytes
        entity_id_t             colliding_entity;   // 6 bytes
        event_id_t              event;              // 6 bytes
        uint8_t                 collision_type;     // 1 byte
        uint8_t                 padding;            // 1 byte
        velocity_t              velocity;           // 12 bytes
        float32_t               mass;               // 4 bytes
        location12_t            location;           // 12 bytes

        collision_pdu_t(void);
        ~collision_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct create_entity_pdu_t : base_pdu_t
    {
        entity_id_t             originator;         // 6 bytes
        entity_id_t             recipient;          // 6 bytes
        uint32_t                request_id;         // 4 bytes

        create_entity_pdu_t(void);
        ~create_entity_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct remove_entity_pdu_t : base_pdu_t
    {
        entity_id_t             originator;         // 6 bytes
        entity_id_t             recipient;          // 6 bytes
        uint32_t                request_id;         // 4 bytes

        remove_entity_pdu_t(void);
        ~remove_entity_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct start_resume_pdu_t : base_pdu_t
    {
        entity_id_t             originator;         // 6 bytes
        entity_id_t             recipient;          // 6 bytes
        clocktime_t             real_time;          // 8 bytes
        clocktime_t             simulation_time;    // 8 bytes
        uint32_t                request_id;         // 4 bytes

        start_resume_pdu_t(void);
        ~start_resume_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct stop_freeze_pdu_t : base_pdu_t
    {
        entity_id_t             originator;         // 6 bytes
        entity_id_t             recipient;          // 6 bytes
        clocktime_t             real_time;          // 8 bytes
        uint8_t                 reason;             // 1 byte
        uint8_t                 behavior;           // 1 byte
        uint16_t                padding;            // 4 bytes
        uint32_t                request_id;         // 4 bytes

        stop_freeze_pdu_t(void);
        ~stop_freeze_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct acknowledge_pdu_t : base_pdu_t
    {
        entity_id_t             originator;         // 6 bytes
        entity_id_t             recipient;          // 6 bytes
        uint16_t                acknowledge_flag;   // 2 bytes
        uint16_t                response_flag;      // 2 bytes
        uint32_t                request_id;         // 4 bytes

        acknowledge_pdu_t(void);
        ~acknowledge_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct abstract_siman_pdu_t : base_pdu_t
    {
        entity_id_t             originator;         // 6 bytes
        entity_id_t             recipient;          // 6 bytes
        uint32_t                fixed_count;        // 4 bytes
        uint32_t                variable_count;     // 4 bytes
        fixed_datum_t         **fixed_records;      // Variable length
        variable_datum_t      **variable_records;   // Variable length

        abstract_siman_pdu_t(void);
        ~abstract_siman_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void print_fixed_datums(std::ostream &) const;
        void print_variable_datums(std::ostream &) const;
    };

    // ------------------------------------------------------------------------
    struct action_request_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                request_id;         // 4 bytes
        uint32_t                action_id;          // 4 bytes

        action_request_pdu_t(void);
        ~action_request_pdu_t(void);

        inline actreq_action_ids_e action_id_enum(void) const
        {
        	return (actreq_action_ids_e)action_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct action_response_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                request_id;         // 4 bytes
        uint32_t                request_status;     // 4 bytes

        action_response_pdu_t(void);
        ~action_response_pdu_t(void);

        inline actres_req_status_e request_status_enum(void) const
        {
        	return (actres_req_status_e)request_status;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct data_query_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                request_id;         // 4 bytes
        uint32_t                time_interval;      // 4 bytes

        data_query_pdu_t(void);
        ~data_query_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct set_data_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                request_id;         // 4 bytes
        uint32_t                padding;            // 4 bytes

        set_data_pdu_t(void);
        ~set_data_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct data_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                request_id;         // 4 bytes
        uint32_t                padding;            // 4 bytes

        data_pdu_t(void);
        ~data_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct event_report_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                event_type;         // 4 bytes
        uint32_t                padding;            // 4 bytes

        event_report_pdu_t(void);
        ~event_report_pdu_t(void);

        inline event_type_e event_type_enum(void) const
        {
        	return (event_type_e)event_type;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct comment_pdu_t : abstract_siman_pdu_t
    {
    	comment_pdu_t(void);
        ~comment_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct em_emission_pdu_t : base_pdu_t
    {
    	// TODO

    	em_emission_pdu_t(void);
        ~em_emission_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct designator_pdu_t : base_pdu_t
    {
        entity_id_t             designating_id;     // 6 bytes
        uint8_t                 spot_type;          // 1 byte
        uint8_t                 system_name;        // 1 byte
        entity_id_t             designated_id;      // 6 bytes
        uint16_t                code;               // 2 bytes
        float32_t               power;              // 4 bytes
        float32_t               wavelength;         // 4 bytes
        location12_t            spot_offset;        // 12 bytes
        location24_t            spot_location;      // 24 bytes
        uint8_t                 algorithm;          // 1 byte
        uint8_t                 flash_rate;         // 1 byte
        uint8_t                 system_number;      // 1 byte
        uint8_t                 function;           // 1 byte
        location12_t            beam_offset;        // 12 bytes

        designator_pdu_t(void);
        ~designator_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };
}

std::ostream &operator<<(std::ostream &, const vdis::pdu_t &);

#endif
