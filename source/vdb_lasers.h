#ifndef VDB_LASERS_H
#define VDB_LASERS_H

#include "vdb_system.h"

#include "vdis_data_types.h"

namespace vdis
{
    class pdu_t;
    struct designator_pdu_t;
}

namespace vdb
{
    class pdu_data_t;

    // ------------------------------------------------------------------------
    class lasers
    {
      public:

        static void process_pdu(const pdu_data_t &data, const vdis::pdu_t &);

      private:

        static void process_designator(
            const pdu_data_t &data,
            const vdis::designator_pdu_t &pdu
        );

        static void print_data(const pdu_data_t &data);

        struct laser_t
        {
            vdis::id_t                      target;
            uint16_t                        code;
            float32_t                       power;
            vdis::desig_spot_type_e         spot_type;
            vdis::desig_system_name_e       system_name;
            vdis::laser_function_e          function;

            inline bool operator==(const laser_t &other) const
            {
                if ((target == other.target) and
                    (code == other.code) and
                    (spot_type == other.spot_type) and
                    (system_name == other.system_name) and
                    (function == other.function))
                {
                    return true;
                }

                return false;
            }

            inline bool operator!=(const laser_t &other) const
            {
                return not (*this == other);
            }
        };

        typedef std::map<uint8_t, laser_t>
            laser_systems_t;
        typedef std::map<vdis::id_t, laser_systems_t>
            entity_lasers_t;

        static entity_lasers_t              current_lasers;

        static std::ostream                &err;
        static std::ostream                &out;
    };
}

#endif
