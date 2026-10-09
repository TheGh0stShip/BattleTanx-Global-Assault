#include "types.h"

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216E00[];
extern u16 D_8011481C;
extern u32 D_80114820;
extern u8 D_80114808[];
extern u8 D_80114818[];

extern s32 osRecvMesg(void *queue, void *message, s32 flags);
extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern s32 osPfsAllocateFile(void *pfs, u16 company, u32 game,
                             void *name, void *extension, s32 size,
                             s32 *file_number);

s32 func_800992E0(s32 controller, s32 *file_number) {
    s32 result;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller--;
    result = osPfsAllocateFile(D_80216E00 + controller * 0x68,
                               D_8011481C, D_80114820, D_80114808,
                               D_80114818, 0x100, file_number);
    osSendMesg(&D_80217010, &D_80219200, 0);
    return result;
}
