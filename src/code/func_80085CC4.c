#include "types.h"
#include "m2c_macros.h"

extern M2C_UNK func_800B88DC(void *, u16, void *);
extern M2C_UNK func_80087EF0();
extern s32 func_8008F3FC(u16, M2C_UNK, M2C_UNK, M2C_UNK);

void func_80085CC4(void *arg0, void *arg1) {
    u16 temp_v0;
    f32 temp_f0;

    M2C_FIELD(arg1, s32 *, 0) = 0;
    temp_v0 = M2C_FIELD(arg0, u16 *, 0x20);
    temp_f0 = 1.0f;
    M2C_FIELD(arg1, s32 *, 0x10) = 0;
    M2C_FIELD(arg1, u16 *, 8) = temp_v0;
    M2C_FIELD(arg1, f32 *, 4) = temp_f0;
    func_80087EF0();
    if (func_8008F3FC(M2C_FIELD(arg0, u16 *, 0x1F4), 0x201008, 0, 0) == 0) {
        M2C_FIELD(arg0, f32 *, 8) = M2C_FIELD(arg0, f32 *, 8) + M2C_FIELD(arg0, f32 *, 0x184);
        M2C_FIELD(arg0, f32 *, 0xC) = M2C_FIELD(arg0, f32 *, 0xC) + M2C_FIELD(arg0, f32 *, 0x188);
    }
    func_800B88DC(arg0 + 0x28, M2C_FIELD(arg0, u16 *, 0x1F4), arg0 + 0x10);
}
