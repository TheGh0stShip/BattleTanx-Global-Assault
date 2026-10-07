#include "types.h"

extern u8 D_80118124[];
extern void func_800BEBA8(void* object, s32 value, s32 mode);

void func_800C03F0(s32 value) {
    func_800BEBA8(D_80118124, value, 0);
}
