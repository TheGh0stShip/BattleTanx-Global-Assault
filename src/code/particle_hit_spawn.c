typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { f32 x, y, z; } Vec3;
typedef struct { s32 pad0; s32 type; } Hit;
typedef struct { Hit *hit; s32 pad4; Vec3 pos; f32 f14; f32 f18; s32 pad1c[3]; } Info;
typedef struct { Vec3 pos; u16 h0c; s32 i10; void *p14; s32 i18; s32 pad1c; s32 i20; s32 pad24; } Ctx;
typedef struct { u32 v[7]; } Msg;
typedef struct Ent { u8 pad0[0xC]; Vec3 pos; } Ent;
typedef struct { u8 pad0[0xC]; s32 i0c; u8 pad10[4]; Ent *e14; u8 pad18[0x24 - 0x18]; Vec3 pos; u8 pad30[0x30 - 0x30]; u16 h30; u8 pad32[4]; s16 h36; u8 b38; u8 b39; u8 b3a; } A;
struct B { u8 pad0[8]; A *a; Vec3 pos; u16 h18; u8 pad1a[0x21 - 0x1A]; u8 b21; };
typedef struct B B;
typedef struct { void (*fn)(Hit *, A *, s32, Ctx *, Msg *); s32 pad[2]; } Handler;
extern s32 func_800B49E0(Vec3 *, Vec3 *, s32, s32, s32, s32, Info *);
extern u32 func_8009D914(void);
extern s32 func_8009D5B4(f32);
extern void func_800F38C0(s32, Vec3 *, s32, u16);
extern s16 D_80397650;
extern f32 D_801255D8, D_801255DC, D_801255E0;
extern u8 D_80235F00[][0x250];
extern Handler D_80224B5C[];

static inline s32 trace(Vec3 *a, B *b, s32 m, s32 x, Info *i) {
    s32 id = b->a->b3a;
    s32 y = b->a->i0c;
    D_80397650 = 1;
    return func_800B49E0(a, &b->pos, m, id, x, y, i);
}

static const Msg sMsgInit = { { 2 } };

void func_800F0B08(B *b) {
    A *a;
    Vec3 *p;
    Info info;
    Ctx c;
    u16 ang;
    Msg m;
    Msg m2;

    a = b->a;
    if (a->h30 != 0) {
        p = &a->pos;
    } else {
        if (a->e14 == (Ent *)b) {
            return;
        }
        p = &a->e14->pos;
    }
    if ((u16)trace(p, b, 0x64940B, 0, &info)) {
        D_801255D8 = info.pos.x;
        D_801255E0 = info.pos.z;
        D_801255DC = info.pos.y;
        if (info.hit != 0) {
            m = sMsgInit;
            c.pos.x = info.pos.x;
            c.pos.z = info.pos.z;
            c.pos.y = info.pos.y;
            c.h0c = b->h18;
            c.p14 = (a->b38 == 127) ? 0 : D_80235F00[a->b38];
            c.i10 = 4;
            c.i18 = a->h36;
            c.i20 = 0;
            if (D_80224B5C[info.hit->type].fn != 0) {
                D_80224B5C[info.hit->type].fn(info.hit, a, 0, &c, &m);
            }
            switch (m.v[0]) {
            case 2:
                break;
            case 1:
                a->b39 = b->b21;
                break;
            case 4:
                a->b39 = b->b21;
                if (func_8009D914() % 5 == 0) {
                    func_800F38C0(0, &info.pos, b->a->b3a, (info.f14 < 0.0f) ? -func_8009D5B4(info.f18) : func_8009D5B4(info.f18));
                }
                break;
            }
        } else {
            a->b39 = b->b21;
            if (func_8009D914() % 5 == 0) {
                func_800F38C0(0, &info.pos, b->a->b3a, (info.f14 < 0.0f) ? -func_8009D5B4(info.f18) : func_8009D5B4(info.f18));
            }
        }
    }
    if ((u16)trace(p, b, 2048, 50, &info) && info.hit != 0) {
        m2 = sMsgInit;
        if (D_80224B5C[info.hit->type].fn != 0) {
            D_80224B5C[info.hit->type].fn(info.hit, a, 0, &c, &m2);
        }
    }
}
