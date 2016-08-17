// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef NATURE_H
#define NATURE_H

namespace vdb
{
    typedef enum
    {
        NATURE_OTHER = 0,
        NATURE_HOST_FIREABLE_MUNITION = 1,
        NATURE_MUNITION_CARRIED_AS_CARGO = 2,
        NATURE_FUEL_CARRIED_AS_CARGO = 3,
        NATURE_GUNMOUNT_ATTACHED_TO_HOST = 4,
        NATURE_COMPUTER_GENERATED_FORCES_CARRIED_AS_CARGO = 5,
        NATURE_VEHICLE_CARRIED_AS_CARGO = 6,
        NATURE_EMITTER_MOUNTED_ON_HOST = 7,
        NATURE_MOBILE_COMMAND_AND_CONTROL_ENTITY_CARRIED_ABOARD_HOST = 8,
        NATURE_ENTITY_STATIONED_AT_WITH_RESPECT_TO_HOST = 9,
        NATURE_TEAM_MEMBER_IN_FORMATION_WITH = 10,
        NATURE_END
    } nature_e;
}

#endif
