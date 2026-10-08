/* LDSYM __osPfsPifRam=0x803AF230 */
#include "pfs.h"

#define CONT_CARD_ON 0x01
#define CONT_CARD_PULL 0x02
#define CONT_ADDR_CRC_ER 0x04

extern void __osPfsRequestData(u8 cmd);
extern void __osPfsGetInitData(u8 *pattern, OSContStatus *data);

s32 __osPfsGetStatus(OSMesgQueue *queue, int channel)
{
    s32 ret = 0;
    OSMesg dummy;
    u8 pattern;
    OSContStatus data[MAXCONTROLLERS];

    __osPfsRequestData(CONT_CMD_REQUEST_STATUS);
    ret = __osSiRawStartDma(OS_WRITE, &__osPfsPifRam);
    osRecvMesg(queue, &dummy, OS_MESG_BLOCK);
    ret = __osSiRawStartDma(OS_READ, &__osPfsPifRam);
    osRecvMesg(queue, &dummy, OS_MESG_BLOCK);
    __osPfsGetInitData(&pattern, data);
    if (((data[channel].status & CONT_CARD_ON) != 0) &&
        ((data[channel].status & CONT_CARD_PULL) != 0))
        return PFS_ERR_NEW_PACK;
    if ((data[channel].errno != 0) ||
        ((data[channel].status & CONT_CARD_ON) == 0))
        return PFS_ERR_NOPACK;
    if ((data[channel].status & CONT_ADDR_CRC_ER) != 0)
        return PFS_ERR_CONTRFAIL;
    return ret;
}
