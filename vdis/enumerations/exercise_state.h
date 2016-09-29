// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef EXERCISE_STATE_H
#define EXERCISE_STATE_H

namespace vdis
{
    typedef enum
    {
        EXERCISE_STATE_UNKNOWN = 0,
        EXERCISE_STATE_UNALLOCATED = 1,
        EXERCISE_STATE_ALLOCATED = 2,
        EXERCISE_STATE_INITIALIZED = 3,
        EXERCISE_STATE_PAUSED = 4,
        EXERCISE_STATE_EXECUTING = 5,
        EXERCISE_STATE_ERROR = 6,
        EXERCISE_STATE_END
    } exercise_state_e;
}

#endif
