#include "types.h"

extern u8 D_801147E0;
extern u8 D_801F3198[];
extern u8 D_801FE598[];
extern void func_800FBB70(void *arg);
extern void func_800FBD94(void *arg);
extern void *func_800FB4F0(void *arg);
extern void func_8009813C(void *arg);

void *func_800979F4(void *arg) {
    void *result;

    D_801147E0++;
    func_800FBB70(D_801FE598);
    func_800FBD94(D_801F3198);
    result = func_800FB4F0(arg);
    if (result != 0) {
        func_8009813C(result);
    }
    return result;
}
