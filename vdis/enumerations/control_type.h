// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef CONTROL_TYPE_H
#define CONTROL_TYPE_H

namespace vdis
{
    typedef enum
    {
        CONTROL_TYPE_RESERVED = 0,
        CONTROL_TYPE_STATUS = 1,
        CONTROL_TYPE_REQUEST_ACKNOWLEDGE_REQUIRED = 2,
        CONTROL_TYPE_REQUEST_NO_ACKNOWLEDGE = 3,
        CONTROL_TYPE_ACK_REQUEST_GRANTED = 4,
        CONTROL_TYPE_NACK_REQUEST_DENIED = 5,
        CONTROL_TYPE_END
    } control_type_e;
}

#endif
