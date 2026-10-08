/* ---- 0x800DC000/func_800DD0D8.c ---- */
typedef unsigned char u8; typedef unsigned short u16; typedef int s32; typedef float f32;
typedef struct { f32 x, y, z; } Vec3;
typedef struct {
    char p0[12]; Vec3 pos; u8 a; char p1; u16 b; u8 c; char p2[3];
    s32 d, e, f; u8 g; char p3[3]; s32 h; s32 time;
} Ev;
extern void *func_800A18D0(s32, s32);
extern s32 D_8021945C;
void func_800DD0D8(Vec3 *pos, u8 a, u16 b, u8 c, s32 d, s32 e, s32 f, u8 g, s32 h, s32 delay) {
    Ev *ev = func_800A18D0(39, 56);
    if (ev != 0) {
        ev->e = e;
        ev->g = g;
        ev->d = d;
        ev->f = f;
        ev->h = h;
        ev->a = a;
        ev->pos = *pos;
        ev->c = c;
        ev->b = b;
        ev->time = D_8021945C + delay;
    }
}

