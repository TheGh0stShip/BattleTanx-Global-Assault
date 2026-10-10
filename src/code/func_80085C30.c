#include "types.h"
#include "m2c_macros.h"

void func_8009DAB0(void *, u32, u16, s32);
void func_80088720(void *);
void func_80089008(void *);
extern s32 D_8021945C;

void func_80085C30(void *arg0, s32 arg1) {
    s32 flags = M2C_FIELD(arg0, s32 *, 0x1E0);
    s32 expires = D_8021945C;
    register f32 base_height __asm__("$f0") = 250.0f;
    register f32 x __asm__("$f2") = M2C_FIELD(arg0, f32 *, 8);
    register f32 y __asm__("$f4") = M2C_FIELD(arg0, f32 *, 0xC);
    register u32 five_bits __asm__("$5");
    u16 id;

    __asm__ volatile("lui %0,0x40a0" : "=r"(five_bits));
    id = *(volatile u16 *)((u8 *)arg0 + 0x20);
    M2C_FIELD(arg0, s32 *, 0x170) = 1;
    flags |= 0x100;
    expires += 0x3C;
    M2C_FIELD(arg0, s32 *, 0x1E0) = flags;
    M2C_FIELD(arg0, s32 *, 0x174) = expires;
    M2C_FIELD(arg0, f32 *, 0x178) = base_height;
    M2C_FIELD(arg0, f32 *, 0x17C) = x;
    M2C_FIELD(arg0, f32 *, 0x180) = y;
    func_8009DAB0(arg0 + 0x184, five_bits, id, expires);
    M2C_FIELD(arg0, s32 *, 0x18C) = arg1;
    func_80088720(arg0);
    func_80089008(arg0);
}
