// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

namespace vdb
{
    typedef enum
    {
        EVENT_TYPE_OTHER = 0,
        EVENT_TYPE_UNUSED = 1,
        EVENT_TYPE_RAN_OUT_OF_AMMO = 2,
        EVENT_TYPE_KILLED_IN_ACTION = 3,
        EVENT_TYPE_DAMAGE = 4,
        EVENT_TYPE_MOBILITY_DISABLED = 5,
        EVENT_TYPE_FIRE_DISABLED = 6,
        EVENT_TYPE_RAN_OUT_OF_FUEL = 7,
        EVENT_TYPE_ENTITY_INITIALIZATION = 8,
        EVENT_TYPE_REQ_INDIRECT_FIRE_OR_CAS_MISSION = 9,
        EVENT_TYPE_INDIRECT_FIRE_OR_CAS_FIRE = 10,
        EVENT_TYPE_MINEFIELD_ENTRY = 11,
        EVENT_TYPE_MINEFIELD_DETONATION = 12,
        EVENT_TYPE_VEHICLE_MASTER_POWER_ON = 13,
        EVENT_TYPE_VEHICLE_MASTER_POWER_OFF = 14,
        EVENT_TYPE_AGGREGATE_STATE_CHANGE_REQ = 15,
        EVENT_TYPE_PREVENT_COLLISION_DETONATION = 16,
        EVENT_TYPE_OWNERSHIP_REPORT = 17,
        EVENT_TYPE_RADAR_PERCEPTION = 18,
        EVENT_TYPE_END
    } event_type_e;
}

#endif
