#include "types.h"

extern u32 D_80122EB8;

void func_80085E4C(void *object, void *result) {
    u8 *data = object;
    s32 index;

    *(s32 *)((u8 *)result + 0xC) = 0;
    *(u16 *)(data + 0x20) -= *(u16 *)(data + 0x178);
    index = *(s32 *)(data + 0x98);
    if (*(u32 *)((u8 *)&D_80122EB8 + index * 0xD0) & 1) {
        *(u16 *)(data + 0x24) += *(u16 *)(data + 0x178) * 2;
    }
}
