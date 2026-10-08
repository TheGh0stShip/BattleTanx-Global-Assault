#include "types.h"

extern s16 D_80397906[];
extern void func_800B0F4C(u16 index);
extern void func_800B0E38(u16 index);

void func_800B1610(u16 index, s32 value) {
    func_800B0F4C(index);
    D_80397906[index * 20] = value;
    func_800B0E38(index);
}
