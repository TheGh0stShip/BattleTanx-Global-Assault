typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32;
typedef struct { s32 unk0; u8 pad[0x1C]; } Unk800DB6F0;
extern Unk800DB6F0 *D_80219498;

u16 func_800DB6F0(f32 x, u8 idx) {
    Unk800DB6F0 *p = D_80219498;
    f32 v = (f32)*(s32 *)((u8 *)p + (idx << 5) + 32) - 30.0f;
    f32 a = v - 4.5f;
    if (a + 10.0f < x) {
        return 0x5555;
    }
    return (u32)((x - 10.0f) / a * 5461.0f + 16384.0f);
}

u16 func_800DB7A4(f32 x, u8 idx) {
    Unk800DB6F0 *p = D_80219498;
    f32 v = (f32)*(s32 *)((u8 *)p + (idx << 5) + 32) - 30.0f;
    f32 a = v - 4.5f;
    if (x < v - a) {
        return 0x2AAB;
    }
    return (u32)((v - x) / a * -5461.0f + 16384.0f);
}
