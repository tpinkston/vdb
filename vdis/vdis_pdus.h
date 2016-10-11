#ifndef VDIS_PDUS_H
#define VDIS_PDUS_H

#include "vdis_byte_buffer.h"
#include "vdis_data_types.h"

// Base lengths of PDUs as defined in specification (includes header but
// not supplemental records)
//
#define ENTITY_STATE_PDU_SIZE              144
#define FIRE_PDU_SIZE                       96
#define DETONATION_PDU_SIZE                104
#define COLLISION_PDU_SIZE                  60
#define CREATE_ENTITY_PDU_SIZE              28
#define REMOVE_ENTITY_PDU_SIZE              28
#define START_RESUME_PDU_SIZE               44
#define STOP_FREEZE_PDU_SIZE                40
#define ACKNOWLEDGE_PDU_SIZE                32
#define ACTION_REQUEST_PDU_SIZE             40
#define ACTION_RESPONSE_PDU_SIZE            40
#define DATA_QUERY_PDU_SIZE                 40
#define SET_DATA_PDU_SIZE                   40
#define DATA_PDU_SIZE                       40
#define EVENT_REPORT_PDU_SIZE               40
#define COMMENT_PDU_SIZE                    32
#define EM_EMISSION_PDU_SIZE                28
#define DESIGNATOR_PDU_SIZE                 88
#define TRANSMITTER_PDU_SIZE               104
#define SIGNAL_PDU_SIZE                     32
#define RECEIVER_PDU_SIZE                   32
#define IFF_PDU_SIZE                        60
#define ENVIRONMENTAL_PROCESS_PDU_SIZE      20
#define POINT_OBJECT_STATE_PDU_SIZE         88
#define LINEAR_OBJECT_STATE_PDU_SIZE        40
#define AREAL_OBJECT_STATE_PDU_SIZE         48
#define APPLICATION_CONTROL_PDU_SIZE        40

namespace vdis
{
    class byte_buffer_t;
    class byte_stream_t;

    struct fixed_datum_record_t;
    struct standard_variable_record_t;
    struct variable_datum_record_t;
    struct variable_parameter_record_t;

    // ------------------------------------------------------------------------
    struct pdu_base_t
    {
        pdu_header_t                    header;                     // 12 bytes

        virtual ~pdu_base_t(void) { }

        virtual inline const id_t *get_initator(void) const
        {
            return 0;
        }

        virtual inline const uint32_t *get_request_id(void) const
        {
            return 0;
        }

        virtual inline bool contains_id(const id_t &) const
        {
            return false;
        }

        virtual void clear(void) = 0;
        virtual void print(std::ostream &) const = 0;
        virtual void read(byte_stream_t &) = 0;
        virtual void write(byte_stream_t &) = 0;
    };

    // ------------------------------------------------------------------------
    class pdu_t
    {
      public:

        pdu_t(void) : base_type(PDU_TYPE_OTHER), base_ptr(0) { }

        pdu_t(byte_stream_t &);

        inline virtual ~pdu_t(void)
        {
            clear();
        }

        void clear(void);

        inline pdu_type_e type(void) const { return base_type; }

        inline const pdu_base_t *base(void) const { return base_ptr; }

        inline const id_t *get_initator(void) const
        {
            return (base() ? base()->get_initator() : 0);
        }

        inline const uint32_t *get_request_id(void) const
        {
            return (base() ? base()->get_request_id() : 0);
        }

        inline bool contains_id(const id_t &id) const
        {
            return (base() ? base()->contains_id(id) : false);
        }

        static bool validate_header(byte_buffer_t &);

        // When set to true, the 'base' PDU will only get generated if the
        // header is valid (i.e. call to 'valid_header' method returns true.
        // Default is true.
        //
        static bool                     validate_headers;

      protected:

        pdu_type_e                      base_type;
        pdu_base_t                     *base_ptr;
    };

    // ------------------------------------------------------------------------
    struct default_pdu_t : pdu_base_t
    {
        byte_buffer_t                   content;                    // Variable

        virtual ~default_pdu_t(void)
        {
            clear();
        }

        inline void clear(void)
        {
            header.clear();
            content.clear();
        }

        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct entity_state_pdu_t : pdu_base_t
    {
        id_t                            id;                         // 6 bytes
        uint8_t                         force;                      // 1 byte
        uint8_t                         record_count;               // 1 byte
        entity_type_t                   type;                       // 8 bytes
        entity_type_t                   alternate_type;             // 8 bytes
        velocity_t                      velocity;                   // 12 bytes
        location24_t                    location;                   // 24 bytes
        orientation_t                   orientation;                // 12 bytes
        uint32_t                        appearance;                 // 4 bytes
        dead_reckoning_t                dead_reckoning;             // 40 bytes
        marking_t                       marking;                    // 12 bytes
        entity_capabilities_t           capabilities;               // 4 bytes
        variable_parameter_record_t   **records;                    // Variable

        entity_state_pdu_t(void);
        ~entity_state_pdu_t(void);

        inline const variable_parameter_record_t *record(uint32_t i) const
        {
            return (records and (i < record_count)) ? records[i] : 0;
        }

        inline const id_t *get_initator(void) const
        {
            return &id;
        }

        inline bool contains_id(const id_t &i) const
        {
            return id.matches(i);
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        // When set to true, entity markings get automatically mapped to the
        // entity ID via the 'entity_markings' service class.  This gets
        // done in this structure's 'read' method.  Default is true.
        //
        static bool                     set_entity_markings;
    };

    // ------------------------------------------------------------------------
    struct fire_pdu_t : pdu_base_t
    {
        id_t                            shooter;                    // 6 bytes
        id_t                            target;                     // 6 bytes
        id_t                            munition;                   // 6 bytes
        id_t                            event;                      // 6 bytes
        uint32_t                        fire_mission_index;         // 4 bytes
        location24_t                    world_location;             // 24 bytes
        burst_descriptor_t              burst_descriptor;           // 16 bytes
        velocity_t                      velocity;                   // 12 bytes
        float32_t                       range;                      // 4 bytes

        ~fire_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &shooter;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct detonation_pdu_t : pdu_base_t
    {
        id_t                            shooter;                    // 6 bytes
        id_t                            target;                     // 6 bytes
        id_t                            munition;                   // 6 bytes
        id_t                            event;                      // 6 bytes
        velocity_t                      velocity;                   // 12 bytes
        location24_t                    world_location;             // 24 bytes
        burst_descriptor_t              burst_descriptor;           // 16 bytes
        location12_t                    entity_location;            // 12 bytes
        uint8_t                         result;                     // 1 byte
        uint8_t                         record_count;               // 1 byte
        uint16_t                        padding;                    // 2 bytes
        variable_parameter_record_t   **records;                    // Variable

        detonation_pdu_t(void);
        ~detonation_pdu_t(void);

        inline const variable_parameter_record_t *record(uint32_t i) const
        {
            return (records and (i < record_count)) ? records[i] : 0;
        }

        inline const id_t *get_initator(void) const
        {
            return &shooter;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct collision_pdu_t : pdu_base_t
    {
        id_t                            issuing_entity;             // 6 bytes
        id_t                            colliding_entity;           // 6 bytes
        id_t                            event;                      // 6 bytes
        uint8_t                         collision_type;             // 1 byte
        uint8_t                         padding;                    // 1 byte
        velocity_t                      velocity;                   // 12 bytes
        float32_t                       mass;                       // 4 bytes
        location12_t                    location;                   // 12 bytes

        collision_pdu_t(void);
        ~collision_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &issuing_entity;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct create_entity_pdu_t : pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        uint32_t                        request_id;                 // 4 bytes

        create_entity_pdu_t(void);
        ~create_entity_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct remove_entity_pdu_t : pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        uint32_t                        request_id;                 // 4 bytes

        remove_entity_pdu_t(void);
        ~remove_entity_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct start_resume_pdu_t : pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        clocktime_t                     real_time;                  // 8 bytes
        clocktime_t                     simulation_time;            // 8 bytes
        uint32_t                        request_id;                 // 4 bytes

        start_resume_pdu_t(void);
        ~start_resume_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct stop_freeze_pdu_t : pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        clocktime_t                     real_time;                  // 8 bytes
        uint8_t                         reason;                     // 1 byte
        uint8_t                         behavior;                   // 1 byte
        uint16_t                        padding;                    // 4 bytes
        uint32_t                        request_id;                 // 4 bytes

        stop_freeze_pdu_t(void);
        ~stop_freeze_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct acknowledge_pdu_t : pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        uint16_t                        acknowledge_flag;           // 2 bytes
        uint16_t                        response_flag;              // 2 bytes
        uint32_t                        request_id;                 // 4 bytes

        acknowledge_pdu_t(void);
        ~acknowledge_pdu_t(void) { }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct abstract_siman_pdu_t : pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        uint32_t                        fixed_count;                // 4 bytes
        uint32_t                        variable_count;             // 4 bytes
        fixed_datum_record_t          **fixed_records;              // Variable
        variable_datum_record_t       **variable_records;           // Variable

        abstract_siman_pdu_t(void);
        ~abstract_siman_pdu_t(void);

        inline const fixed_datum_record_t *fixed_datum(uint32_t i) const
        {
            return (fixed_records and (i < fixed_count)) ?
                fixed_records[i] :
                0;
        }

        inline const variable_datum_record_t *variable_record(uint32_t i) const
        {
            return (variable_records and (i < variable_count)) ?
                variable_records[i] :
                0;
        }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        void clear(void);
        void print_records(const std::string &, std::ostream &) const;
        void read_records(byte_stream_t &);
        void write_records(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct action_request_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                        request_id;                 // 4 bytes
        uint32_t                        action_id;                  // 4 bytes

        action_request_pdu_t(void);
        ~action_request_pdu_t(void);

        inline actreq_action_ids_e action_id_enum(void) const
        {
            return (actreq_action_ids_e)action_id;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct action_response_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                        request_id;                 // 4 bytes
        uint32_t                        request_status;             // 4 bytes

        action_response_pdu_t(void);
        ~action_response_pdu_t(void);

        inline actres_req_status_e request_status_enum(void) const
        {
            return (actres_req_status_e)request_status;
        }

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct data_query_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                        request_id;                 // 4 bytes
        uint32_t                        time_interval;              // 4 bytes

        data_query_pdu_t(void);
        ~data_query_pdu_t(void);

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct set_data_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                        request_id;                 // 4 bytes
        uint32_t                        padding;                    // 4 bytes

        set_data_pdu_t(void);
        ~set_data_pdu_t(void);

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct data_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                        request_id;                 // 4 bytes
        uint32_t                        padding;                    // 4 bytes

        data_pdu_t(void);
        ~data_pdu_t(void);

        inline const uint32_t *get_request_id(void) const
        {
            return &request_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct event_report_pdu_t : abstract_siman_pdu_t
    {
        uint32_t                        event_type;                 // 4 bytes
        uint32_t                        padding;                    // 4 bytes

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
    struct em_emission_pdu_t : pdu_base_t
    {
        // TODO: em_emission_pdu_t

        em_emission_pdu_t(void);
        ~em_emission_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct designator_pdu_t : pdu_base_t
    {
        id_t                            designating_id;             // 6 bytes
        uint8_t                         spot_type;                  // 1 byte
        uint8_t                         system_name;                // 1 byte
        id_t                            designated_id;              // 6 bytes
        uint16_t                        code;                       // 2 bytes
        float32_t                       power;                      // 4 bytes
        float32_t                       wavelength;                 // 4 bytes
        location12_t                    spot_offset;                // 12 bytes
        location24_t                    spot_location;              // 24 bytes
        uint8_t                         algorithm;                  // 1 byte
        uint8_t                         flash_rate;                 // 1 byte
        uint8_t                         system_number;              // 1 byte
        uint8_t                         function;                   // 1 byte
        location12_t                    beam_offset;                // 12 bytes

        designator_pdu_t(void);
        ~designator_pdu_t(void);

        inline const id_t *get_initator(void) const
        {
            return &designating_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct transmitter_pdu_t : pdu_base_t
    {
        id_t                            entity_id;                  // 6 bytes
        uint16_t                        radio_id;                   // 2 bytes
        entity_type_t                   radio_type;                 // 8 bytes
        uint8_t                         transmit_state;             // 1 byte
        uint8_t                         input_source;               // 1 byte
        uint16_t                        transmitter_parameters;     // 2 bytes
        location24_t                    antenna_location;           // 24 bytes
        location12_t                    relative_antenna_location;  // 12 bytes
        uint16_t                        antenna_pattern_type;       // 2 bytes
        uint16_t                        antenna_pattern_length;     // 2 bytes
        uint64_t                        frequency;                  // 8 bytes
        float32_t                       bandwidth;                  // 4 bytes
        float32_t                       power;                      // 4 bytes
        modulation_type_t               modulation_type;            // 8 bytes
        uint16_t                        crypto_system;              // 2 bytes
        uint16_t                        crypto_key;                 // 2 bytes
        uint8_t                         modulation_parameter_length;// 1 byte
        uint8_t                         padding8;                   // 1 byte
        uint16_t                        padding24;                  // 2 bytes
        uint8_t                        *antenna_patterns;           // Variable
        uint8_t                        *modulation_parameters;      // Variable

        transmitter_pdu_t(void);
        ~transmitter_pdu_t(void);

        inline const id_t *get_initator(void) const
        {
            return &entity_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);

        static const uint32_t
            BASE_LENGTH = 92; // bytes, minus header
    };

    // ------------------------------------------------------------------------
    struct signal_pdu_t : pdu_base_t
    {
        id_t                            entity_id;                  // 6 bytes
        uint16_t                        radio_id;                   // 2 bytes
        uint16_t                        encoding;                   // 2 bytes
        uint16_t                        tdl_type;                   // 2 bytes
        int32_t                         sample_rate;                // 4 bytes
        uint16_t                        data_length;                // 2 bytes
        uint16_t                        samples;                    // 2 bytes
        uint8_t                        *data;                       // Variable

        signal_pdu_t(void);
        ~signal_pdu_t(void);

        inline const id_t *get_initator(void) const
        {
            return &entity_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct receiver_pdu_t : pdu_base_t
    {
        id_t                            entity_id;                  // 6 bytes
        uint16_t                        radio_id;                   // 2 bytes
        uint16_t                        receiver_state;             // 2 bytes
        uint16_t                        padding;                    // 2 bytes
        float32_t                       power;                      // 4 bytes
        id_t                            transmitter_entity;         // 2 bytes
        uint16_t                        transmitter_radio;          // 2 bytes

        receiver_pdu_t(void);
        ~receiver_pdu_t(void);

        inline const id_t *get_initator(void) const
        {
            return &entity_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
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
    struct iff_pdu_t : pdu_base_t
    {
        id_t                            emitter;                    // 6 bytes
        id_t                            event;                      // 6 bytes
        location12_t                    antenna_location;           // 12 bytes
        iff_system_id_t                 system_id;                  // 6 bytes
        uint8_t                         designator;                 // 1 byte
        uint8_t                         specific;                   // 1 byte
        iff_operational_data_t          operational_data;           // 16 bytes
        byte_buffer_t                   extra_layers;               // Variable

        iff_pdu_t(void);
        ~iff_pdu_t(void);

        inline const id_t *get_initator(void) const
        {
            return &emitter;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct minefield_state_pdu_t : pdu_base_t
    {
        // TODO: minefield_state_pdu_t

        minefield_state_pdu_t(void);
        ~minefield_state_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct minefield_query_pdu_t : pdu_base_t
    {
        // TODO: minefield_query_pdu_t

        minefield_query_pdu_t(void);
        ~minefield_query_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct minefield_data_pdu_t : pdu_base_t
    {
        // TODO: minefield_data_pdu_t

        minefield_data_pdu_t(void);
        ~minefield_data_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct minefield_response_nack_pdu_t : pdu_base_t
    {
        // TODO: minefield_response_nack_pdu_t

        minefield_response_nack_pdu_t(void);
        ~minefield_response_nack_pdu_t(void);

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct environmental_process_pdu_t : pdu_base_t
    {
        id_t                            process_id;                 // 6 bytes
        entity_type_t                   environment_type;           // 8 bytes
        uint8_t                         model_type;                 // 1 byte
        uint8_t                         status;                     // 1 byte
        uint16_t                        record_count;               // 2 bytes
        uint16_t                        sequence_number;            // 2 bytes
        environment_record_t          **records;                    // Variable

        environmental_process_pdu_t(void);
        ~environmental_process_pdu_t(void);

        inline const environment_record_t *record(uint32_t i) const
        {
            return (records and (i < record_count)) ? records[i] : 0;
        }

        inline const id_t *get_initator(void) const
        {
            return &process_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct point_object_state_pdu_t : pdu_base_t
    {
        id_t                            object_id;                  // 6 bytes
        id_t                            referenced_object_id;       // 6 bytes
        uint16_t                        update;                     // 2 bytes
        uint8_t                         force_id;                   // 1 byte
        uint8_t                         modifications;              // 1 byte
        object_type_t                   object_type;                // 4 bytes
        location24_t                    location;                   // 24 bytes
        orientation_t                   orientation;                // 12 bytes
        uint32_t                        specific_appearance;        // 4 bytes
        uint16_t                        generic_appearance;         // 2 bytes
        uint16_t                        padding16;                  // 2 bytes
        simulation_id_t                 requestor_id;               // 4 bytes
        simulation_id_t                 receiver_id;                // 4 bytes
        uint32_t                        padding32;                  // 4 bytes

        point_object_state_pdu_t(void);
        ~point_object_state_pdu_t(void);

        inline const id_t *get_initator(void) const
        {
            return &object_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct linear_object_state_pdu_t : pdu_base_t
    {
        id_t                            object_id;                  // 6 bytes
        id_t                            referenced_object_id;       // 6 bytes
        uint16_t                        update;                     // 2 bytes
        uint8_t                         force_id;                   // 1 byte
        uint8_t                         segment_count;              // 1 byte
        simulation_id_t                 requestor_id;               // 4 bytes
        simulation_id_t                 receiver_id;                // 4 bytes
        object_type_t                   object_type;                // 4 bytes
        linear_segment_t              **segments;                   // Variable

        linear_object_state_pdu_t(void);
        ~linear_object_state_pdu_t(void);

        inline const linear_segment_t *segment(uint32_t i) const
        {
            return (segments and (i < segment_count)) ? segments[i] : 0;
        }

        inline const id_t *get_initator(void) const
        {
            return &object_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct areal_object_state_pdu_t : pdu_base_t
    {
        id_t                            object_id;                  // 6 bytes
        id_t                            referenced_object_id;       // 6 bytes
        uint16_t                        update;                     // 2 bytes
        uint8_t                         force_id;                   // 1 byte
        uint8_t                         modifications;              // 1 byte
        object_type_t                   object_type;                // 4 bytes
        uint32_t                        specific_appearance;        // 4 bytes
        uint16_t                        generic_appearance;         // 2 bytes
        uint16_t                        point_count;                // 2 bytes
        simulation_id_t                 requestor_id;               // 4 bytes
        simulation_id_t                 receiver_id;                // 4 bytes
        location24_t                  **points;                     // Variable

        areal_object_state_pdu_t(void);
        ~areal_object_state_pdu_t(void);

        inline const location24_t *point(uint32_t i) const
        {
            return (points and (i < point_count)) ? points[i] : 0;
        }

        inline const id_t *get_initator(void) const
        {
            return &object_id;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };

    // ------------------------------------------------------------------------
    struct application_control_pdu_t : public pdu_base_t
    {
        id_t                            originator;                 // 6 bytes
        id_t                            recipient;                  // 6 bytes
        uint8_t                         reliability_service;        // 1 byte
        uint8_t                         time_interval;              // 1 byte
        uint8_t                         control_type;               // 1 byte
        uint8_t                         padding;                    // 1 byte
        uint16_t                        originator_type;            // 2 bytes
        uint16_t                        recipient_type;             // 2 bytes
        uint32_t                        request_id;                 // 4 bytes
        uint8_t                         total_parts;                // 1 byte
        uint8_t                         current_part;               // 1 byte
        uint16_t                        record_count;               // 2 bytes
        standard_variable_record_t    **records;                    // Variable

        application_control_pdu_t(void);
        ~application_control_pdu_t(void);

        inline app_ctrl_control_type_e control_type_enum(void) const
        {
            return (app_ctrl_control_type_e)control_type;
        }

        inline app_ctrl_application_type_e originator_type_enum(void) const
        {
            return (app_ctrl_application_type_e)originator_type;
        }

        inline app_ctrl_application_type_e recipient_type_enum(void) const
        {
            return (app_ctrl_application_type_e)recipient_type;
        }

        inline const standard_variable_record_t *record(uint32_t i) const
        {
            return (records and (i < record_count)) ? records[i] : 0;
        }

        inline const id_t *get_initator(void) const
        {
            return &originator;
        }

        void clear(void);
        void print(std::ostream &) const;
        void read(byte_stream_t &);
        void write(byte_stream_t &);
    };
}

std::ostream &operator<<(std::ostream &, const vdis::pdu_t &);

#endif
