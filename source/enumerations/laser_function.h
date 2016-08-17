// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef LASER_FUNCTION_H
#define LASER_FUNCTION_H

namespace vdb
{
    typedef enum
    {
        LASER_FUNCTION_DESIGNATING = 0,
        LASER_FUNCTION_RANGING = 1,
        LASER_FUNCTION_IR_DESIGNATING = 2,
        LASER_FUNCTION_BEAM_RIDING = 3,
        LASER_FUNCTION_LASER_POINTING = 4,
        LASER_FUNCTION_IR_MARKING = 5,
        LASER_FUNCTION_NVG_MARKING = 6,

        // Supplemental laser functions
        LASER_FUNCTION_IR_LOW = 7,
        LASER_FUNCTION_IR_HIGH = 8,
        LASER_FUNCTION_IR_PULSE = 9,
        LASER_FUNCTION_LOW = 10,
        LASER_FUNCTION_HIGH = 11,
        LASER_FUNCTION_PULSE = 12,
        LASER_FUNCTION_JAMMING = 13,
        LASER_FUNCTION_OFF = 255,
        LASER_FUNCTION_END
    } laser_function_e;
}

#endif
