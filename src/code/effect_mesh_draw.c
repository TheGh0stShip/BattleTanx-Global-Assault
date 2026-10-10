/* SPAN 0x800F8DA4 */
/* RODATA_VRAM 0x800774BC */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;

typedef struct {
    u8 pad0[0xC];
    f32 pos[3];
    u8 pad18[0x24 - 0x18];
    u8 unk24;
    u8 pad25[0x2C - 0x25];
    s32 unk2C;
} Obj8AAC;

extern s32 D_8021945C;
extern void *D_803A56D4;
extern u8 func_800AD14C(f32, f32, f32, u8);
extern f32 func_8009D4B0(u16);
extern void func_800AD6A8(void *, f32 *, f32, f32, s32, u16, s32, s32, u8, u32 *, s32, s32);

void func_800F8AAC(Obj8AAC *obj) {
    u8 vis;
    s32 d;
    u8 alpha;
    f32 fd;
    f32 a, b, c;
    s32 t;
    u16 r0, r1, r2;
    u32 col[4];
    f32 *pos;

    vis = func_800AD14C(obj->pos[0], obj->pos[1], 19.0f, obj->unk24);
    if (vis) {
        d = D_8021945C - obj->unk2C;
        if (d < 30) {
            alpha = (d * 255) / 30;
        } else {
            alpha = 255;
        }
        fd = d;
        a = func_8009D4B0((s32)(fd / 50.0f * 65535.0f)) * 4.0f + 15.0f;
        b = func_8009D4B0((s32)(fd / 59.0f * 65535.0f)) * 4.0f + 15.0f;
        c = func_8009D4B0((s32)(fd / 40.0f * 65535.0f)) * 4.0f + 15.0f;
        pos = obj->pos;
        t = D_8021945C;
        r0 = -(t * 728);
        r1 = t << 10;
        r2 = t << 9;
        col[0] = 0xFA000000;
        col[1] = alpha | 0x78000000;
        col[2] = 0xFB000000;
        col[3] = 0xE6787800;
        func_800AD6A8(D_803A56D4, pos, a, a, 0, r0, 0, 0, vis, col, 2, 0);
        col[0] = 0xFA000000;
        col[1] = alpha | 0x780000;
        col[2] = 0xFB000000;
        col[3] = 0x78E67800;
        func_800AD6A8(D_803A56D4, pos, b, b, 0, r1, 0, 0, vis, col, 2, 0);
        col[0] = 0xFA000000;
        col[1] = alpha | 0x7800;
        col[2] = 0xFB000000;
        col[3] = 0x7878E600;
        func_800AD6A8(D_803A56D4, pos, c, c, 0, r2, 0, 0, vis, col, 2, 0);
    }
}
