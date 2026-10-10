/* ---- 0x800E4800/b/src/func_800E61E8.c ---- */
#include "types.h"

typedef struct Obj61E8 {
    u8 pad[0xC];
    f32 x, y, z;
    f32 a[3];
    f32 b[3];
    f32 tx, ty, tz;
} Obj61E8;

extern f32 func_8009D8A0(f32);

void func_800E61E8(Obj61E8* o) {

    o->a[0] = o->x + (o->tx - o->x) / 2.0f + func_8009D8A0(2e+02f) - 1e+02f;
    o->a[2] = o->z + (o->tz - o->z) / 2.0f + func_8009D8A0(1e+02f);
    o->a[1] = o->y + (o->ty - o->y) / 2.0f + func_8009D8A0(2e+02f) - 1e+02f;
    o->b[0] = o->x + (o->tx - o->x) / 2.0f + func_8009D8A0(2e+02f) - 1e+02f;
    o->b[2] = o->z + (o->tz - o->z) / 2.0f + func_8009D8A0(1e+02f);
    o->b[1] = o->y + (o->ty - o->y) / 2.0f + func_8009D8A0(2e+02f) - 1e+02f;
}

/* ---- 0x800E4800/b/src/func_800E6324.c ---- */
#include "types.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Ctrl6324 { Vec3f p[4]; } Ctrl6324;
typedef struct Mtx6324 { f32 m[17]; } Mtx6324;
typedef struct Obj6324 {
    u8 pad[0xC];
    Ctrl6324 ctrl;
    u8 id;
} Obj6324;

extern u8 func_800AD14C(f32, f32, f32, u8);
extern void func_800E5BB8(Vec3f*, s32);
extern s32 func_800AA598(u8);
extern void func_8009EFD4(Mtx6324*, f32, f32, f32, s32);
extern void func_8009F824(Mtx6324*, f32, f32, f32);
extern void func_800AE4D0(s32, s32, Mtx6324*, s32, s32, s32, u8);
extern s32 D_803A53D8[];
extern s32 D_80123DF0[];
static const Mtx6324 D_80075ED8 = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };

void func_800E6324(Obj6324* o) {
    Vec3f pts[97];
    Mtx6324 m;
    u8 vis;
    s32 n;
    s32 i;
    s32 model;

    vis = func_800AD14C(o->ctrl.p[0].x, o->ctrl.p[0].y, 300.0f, o->id);
    if (vis) {
        *(Ctrl6324*)pts = o->ctrl;
        n = 4;
        for (i = 0; i < n; i++) {
            func_800E5BB8(pts, i);
        }
        model = func_800AA598(o->id);
        for (i = 0; i < D_80123DF0[n]; i++) {
            m = D_80075ED8;
            func_8009EFD4(&m, pts[i].x, pts[i].z, pts[i].y, 0);
            func_8009F824(&m, 0.3f, 0.3f, 0.3f);
            func_800AE4D0(D_803A53D8[0], model, &m, 0, 0, 0, vis);
        }
    }
}

