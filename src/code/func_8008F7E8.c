#include "types.h"
#include "m2c_macros.h"

void func_800B129C(u16, s16, s16);
extern u8 D_803978E0[];

void func_8008F7E8(void *arg0) {
    register u8 *record __asm__("$16");
    register s32 height __asm__("$17");
    register f32 temp __asm__("$f0");
    register f32 x __asm__("$f2");
    u16 index;
    u16 id;
    s16 x_value;
    s16 y_value;

    temp = M2C_FIELD(arg0, f32 *, 0x10);
    height = (s32)temp;
    index = M2C_FIELD(arg0, u16 *, 0x1F4);
    x = *(volatile f32 *)((u8 *)arg0 + 8);
    temp = *(volatile f32 *)((u8 *)arg0 + 0xC);
    id = M2C_FIELD(arg0, u16 *, 0x20);
    record = D_803978E0 + index * 0x28;
    x_value = (s16)(s32)x;
    y_value = (s16)(s32)temp;
    *(u16 *)(record + 0x24) = id;
    func_800B129C(index, x_value, y_value);
    *(s16 *)(record + 0x14) = (s16)height;
}
