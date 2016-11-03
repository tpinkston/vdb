#include "vdb_lasers.h"

#include "vdis_pdus.h"
#include "vdis_services.h"

// ----------------------------------------------------------------------------
void vdb::lasers::process_pdu(const pdu_data_t &data, const vdis::pdu_t &pdu)
{
    if (pdu.base())
    {
        switch(pdu.type())
        {
            case vdis::PDU_TYPE_DESIGNATOR:
            {
                const vdis::designator_pdu_t &designator = *SCAST(
                    vdis::designator_pdu_t,
                    pdu.base());

                process_designator(data, designator);
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vdb::lasers::process_designator(
    const pdu_data_t &data,
    const vdis::designator_pdu_t &pdu)
{
    // TODO process_designator
}
