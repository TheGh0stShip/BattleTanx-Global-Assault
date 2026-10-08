#include "types.h"

typedef struct PakFileIdentity {
    u8 pad0[4];
    u32 game_code;
    u16 company_code;
    u8 extension[4];
    u8 name[1];
} PakFileIdentity;

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216E00[];

extern s32 osRecvMesg(void *queue, void *message, s32 flags);
extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern s32 osPfsDeleteFile(void *pfs, u16 company, u32 game,
                           void *name, void *extension);

s32 func_800993B0(s32 controller, PakFileIdentity *identity) {
    s32 result;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller--;
    result = osPfsDeleteFile(D_80216E00 + controller * 0x68,
                             identity->company_code, identity->game_code,
                             identity->name, identity->extension);
    osSendMesg(&D_80217010, &D_80219200, 0);
    return result;
}
