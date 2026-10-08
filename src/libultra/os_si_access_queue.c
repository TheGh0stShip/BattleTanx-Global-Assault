#include "ultra.h"

extern s32 D_80126EC0;
extern OSMesg D_803B04F0;
extern OSMesgQueue D_803B04F8;
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);

void __osSiCreateAccessQueue(void)
{
    D_80126EC0 = 1;
    osCreateMesgQueue(&D_803B04F8, &D_803B04F0, 1);
    osSendMesg(&D_803B04F8, 0, OS_MESG_NOBLOCK);
}

void __osSiGetAccess_8010FBB0(void)
{
    OSMesg msg;

    if (!D_80126EC0) {
        __osSiCreateAccessQueue();
    }
    osRecvMesg(&D_803B04F8, &msg, OS_MESG_BLOCK);
}

void __osSiRelAccess_8010FBF4(void)
{
    osSendMesg(&D_803B04F8, 0, OS_MESG_NOBLOCK);
}
