/* ---- 0x800DC000/c/func_800DD1C0.c ---- */
typedef unsigned char u8; typedef unsigned short u16; typedef int s32; typedef float f32;
extern s32 D_8021945C;
typedef struct { f32 x, y, z; } Vec3;
typedef struct {
    char p0[12]; Vec3 pos; u8 a; char p1; u16 b; u8 c; char p2[3];
    s32 d, e, f; u8 g; char p3[3]; s32 h; s32 time;
} Ev;
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern f32 func_8009D8A0(f32);
extern void func_800DC968(Vec3 *, u8, Vec3 *, u16, u8, s32, s32, s32, u8, s32);
void func_800DD1C0(Ev *ev, s32 *done) {
    Vec3 v; s32 t = D_8021945C;
    if (ev->time < t) {
        v.x = func_8009D4B0(ev->b);
        v.z = func_8009D8A0(0.05f) - 0.025f;
        v.y = func_8009D510(ev->b);
        func_800DC968(&ev->pos, ev->a, &v, ev->b, ev->c, ev->d, ev->e, ev->f, ev->g, ev->h);
        *done = 1;
    }
}

