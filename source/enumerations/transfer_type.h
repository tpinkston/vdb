// -----------------------------------------------
// This file is script-generated...
// -----------------------------------------------

#ifndef TRANSFER_TYPE_H
#define TRANSFER_TYPE_H

namespace vdb
{
    typedef enum
    {
        TRANSFER_TYPE_OTHER = 0,
        TRANSFER_TYPE_PUSH_TRANSFER_ENTITY = 1,
        TRANSFER_TYPE_AUTO_PULL_TRANSFER_ENTITY = 2,
        TRANSFER_TYPE_MUTUAL_XCH_ENTITY = 3,
        TRANSFER_TYPE_PUSH_TRANSFER_ENV_PROC = 4,
        TRANSFER_TYPE_AUTO_PULL_TRANSFER_ENV_PROC = 5,
        TRANSFER_TYPE_MUTUAL_XCH_ENV_PROC = 6,
        TRANSFER_TYPE_CANCEL_TRANSFER = 7,
        TRANSFER_TYPE_MAN_PULL_TRANSFER_ENTITY = 8,
        TRANSFER_TYPE_MAN_PULL_TRANSFER_ENV_PROC = 9,
        TRANSFER_TYPE_REMOVE_ENTITY = 10,
        TRANSFER_TYPE_END
    } transfer_type_e;
}

#endif
