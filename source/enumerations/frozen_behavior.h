// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef FROZEN_BEHAVIOR_H
#define FROZEN_BEHAVIOR_H

namespace vdb
{
    typedef enum
    {
        FROZEN_BEHAVIOR_RUN_INTERNAL_SIMULATION_CLOCK = 0,
        FROZEN_BEHAVIOR_TRANSMIT_PDUS = 1,
        FROZEN_BEHAVIOR_UPDATE_MODELS_OF_OTHER_ENTITIES = 2,
        FROZEN_BEHAVIOR_CONTINUE_TRANSMIT_PDUS = 3,
        FROZEN_BEHAVIOR_CEASE_MODELS_OF_OTHER_ENTITIES = 4,
        FROZEN_BEHAVIOR_CONTINUE_MODELS_OF_OTHER_ENTITIES = 5,
        FROZEN_BEHAVIOR_END
    } frozen_behavior_e;
}

#endif
