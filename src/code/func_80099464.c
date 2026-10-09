#include "types.h"

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216E00[];

extern s32 osRecvMesg(void *queue, void *message, s32 flags);
extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern s32 osPfsReadWriteFile(void *pfs, s32 file_number, s32 mode,
                              s32 offset, s32 size, void *buffer);
extern u32 func_80099758(void *buffer);

s32 func_80099464(s32 controller, s32 file_number, void *buffer) {
    s32 result;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller--;
    result = osPfsReadWriteFile(D_80216E00 + controller * 0x68,
                                file_number, 0, 0, 0x100, buffer);
    if (result == 0 && func_80099758(buffer) != *(u32 *)buffer) {
        result = -0x45;
    }
    osSendMesg(&D_80217010, &D_80219200, 0);
    return result;
}
