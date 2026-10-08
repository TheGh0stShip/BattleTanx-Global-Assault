/* IDOFLAGS: -O1 -mips2
   LDSYM __osContPifRam=0x803ADD20 LDSYM __osContLastCmd=0x803ADD60 LDSYM __osMaxControllers=0x803ADD61
   LDSYM __osSiGetAccess=0x8010FBB0 LDSYM __osSiRelAccess=0x8010FBF4 LDSYM __osPackRequestData=0x801036B8 */
#include "controller.h"

s32 osContStartQuery(OSMesgQueue *mq)
{
    s32 ret = 0;

    __osSiGetAccess();
    if (__osContLastCmd != CONT_CMD_REQUEST_STATUS) {
        __osPackRequestData(CONT_CMD_REQUEST_STATUS);
        ret = __osSiRawStartDma(OS_WRITE, &__osContPifRam);
        osRecvMesg(mq, 0, OS_MESG_BLOCK);
    }
    ret = __osSiRawStartDma(OS_READ, &__osContPifRam);
    __osContLastCmd = CONT_CMD_REQUEST_STATUS;
    __osSiRelAccess();
    return ret;
}

void osContGetQuery(OSContStatus *data)
{
    u8 pattern;

    __osContGetInitData(&pattern, data);
}
