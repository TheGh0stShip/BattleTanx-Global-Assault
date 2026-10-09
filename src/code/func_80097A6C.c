#include "types.h"

extern u8 D_801147E0;
extern u8 D_801FE598;
extern u8 D_801F3198;

extern void func_800FBB70(void *bank);
extern void func_800FBD94(void *bank);
extern void *func_800FB570(s32, s32, s32, s32, s32);
extern void func_8009813C(void *value);

void *func_80097A6C(s32 a, s32 b, s32 c, s32 d, s32 e) {
    void *result;

    D_801147E0++;
    func_800FBB70(&D_801FE598);
    func_800FBD94(&D_801F3198);
    result = func_800FB570(a, b, c, d, e);
    if (result != 0) {
        func_8009813C(result);
    }
    return result;
}
