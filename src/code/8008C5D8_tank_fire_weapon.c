/* func_8008C5D8 (0x8008C5D8-0x8008E620, 0x2048): tank weapon fire dispatcher.
 * Written from the ROM listing with the m2c transcription as a control-flow guide.
 * SPAN 0x8008E620
 * RODATA_VRAM 0x80071988
 */
typedef unsigned char u8; typedef signed char s8; typedef unsigned short u16; typedef short s16;
typedef int s32; typedef unsigned int u32; typedef float f32;

typedef struct { f32 x, y, z; } Vec3f;

typedef struct Driver {
    u8 pad00[0xA];
    u8 flagsA;          /* 0x0A */
    u8 id;              /* 0x0B */
    u8 pad0C[0x6C];
    u8 state[0x18];     /* 0x78 */
    s32 owner;          /* 0x90 */
    u8 pad94[8];
    Vec3f vecA;         /* 0x9C */
    Vec3f vecB;         /* 0xA8 */
    u8 padB4[0x1CC - 0xB4];
    f32 scale;          /* 0x1CC */
} Driver;

typedef struct Tank {
    s32 id;             /* 0x000 */
    s32 pad04;
    Vec3f pos;          /* 0x008 */
    u8 pad14[0xC];
    u16 yaw;            /* 0x020 */
    u16 pad22;
    u16 turret;         /* 0x024 */
    u16 pad26;
    f32 dir[2];         /* 0x028 */
    f32 speed;          /* 0x030 */
    u8 pad34[0x68 - 0x34];
    u16 pitch;          /* 0x068 */
    u8 pad6A[0x94 - 0x6A];
    u8 team;            /* 0x094 */
    u8 b95;             /* 0x095 */
    u8 pad96[2];
    s32 type;           /* 0x098 */
    u8 pad9C[0x1D0 - 0x9C];
    Driver *drv;        /* 0x1D0 */
    s32 ammoMax;        /* 0x1D4 */
    s32 ammo;           /* 0x1D8 */
    s32 pad1DC;
    u32 flags;          /* 0x1E0 */
    f32 recoilX;        /* 0x1E4 */
    f32 recoil1E8;      /* 0x1E8 */
    f32 recoilZ;        /* 0x1EC */
    f32 recoil1F0;      /* 0x1F0 */
    u8 pad1F4[8];
    u16 u1FC;           /* 0x1FC */
    u16 u1FE;           /* 0x1FE */
    u16 pad200;
    u16 u202;           /* 0x202 */
    u16 pad204;
    u16 u206;           /* 0x206 */
    u8 pad208[6];
    u16 u20E;           /* 0x20E */
    u16 u210;           /* 0x210 */
    u16 u212;           /* 0x212 */
    u16 pad214;
    u16 u216;           /* 0x216 */
    u8 pad218[0x230 - 0x218];
    s32 t230;           /* 0x230 */
    s32 t234;           /* 0x234 */
    void *ptr238;       /* 0x238 */
    void *ptr23C;       /* 0x23C */
    s32 t240;           /* 0x240 */
    s32 t244;           /* 0x244 */
    u8 pad248[0x260 - 0x248];
    s32 t260;           /* 0x260 */
    s32 t264;           /* 0x264 */
    s32 t268;           /* 0x268 */
    u8 pad26C[0x28C - 0x26C];
    s32 t28C;           /* 0x28C */
    void *em290;        /* 0x290 */
    void *em294;        /* 0x294 */
    void *em298;        /* 0x298 */
    u8 pad29C[0x4B8 - 0x29C];
    u16 u4B8;           /* 0x4B8 */
    u16 pad4BA;
    s32 i4BC;           /* 0x4BC */
    f32 f4C0;           /* 0x4C0 */
} Tank;

typedef struct TankStats {  /* stride 0xD0 */
    s16 range;          /* 0x00 */
    s16 pad02;
    s16 pad04;
    s16 height;         /* 0x06 */
    s32 pad08;
    s32 fxType;         /* 0x0C */
    f32 speed;          /* 0x10 */
    f32 fxScale;        /* 0x14 */
    s32 pad18;
    s32 damage;         /* 0x1C */
    s32 sound;          /* 0x20 */
    Vec3f muzzle;       /* 0x24 */
    Vec3f muzzle2;      /* 0x30 */
    u8 pad3C[0x10];
    s32 kind;           /* 0x4C */
    s32 kind2;          /* 0x50 */
    u8 pad54[0x24];
    u32 flags;          /* 0x78 */
    u8 pad7C[0xD0 - 0x7C];
} TankStats;

typedef struct { u8 pad[0x5C]; s32 val; } Race;

extern TankStats D_80122E40[];
extern s32 D_8021945C;          /* frame counter */
extern f32 D_80219488;          /* time scale */
extern Race *D_8021958C;
extern s32 D_802195D4;
extern u8 D_80122360[];
extern s32 D_80123A58[];
extern s32 D_80123A7C[];
extern u8 D_801155EC[];
extern u8 D_801151A4[];
extern const char gEdgePowerUsedMessage[]; /* owned by func_8008C5BC */

void func_8008C3B4(Tank *, Vec3f *, s32, Vec3f *, u16 *);
void func_80095B90(Tank *, s32);
void func_800966D4(Tank *, s32);
void func_800979F4(s32);
void func_80097FB4(s32, f32, f32, f32, u8);
f32 func_8009D4B0(u16);
f32 func_8009D510(u16);
f32 func_8009D8A0(f32);
u32 func_8009D914(void);
void func_800A5BD8(Vec3f *, s32, u8, f32, void *, s32);
void func_800A6B08(void *, void *);
void func_800A6B38(void *, void *);
void func_800A8B14(Driver *, void *);
f32 func_800B93A4(Vec3f *, u8);
void func_800CA620(u8, const char *, s32);
void func_800D5C80(Vec3f *, u8, f32 *, f32 *, u16, u8, f32, s32, s32, s32, Driver *, f32);
void func_800DAAE0(Vec3f *, u8, Vec3f *, u16, u8, u8, s32, Driver *);
void *func_800DC3A8(Vec3f *, u8, f32, u16, Driver *, u8, u8, Vec3f *, Vec3f *, s32, u8, u8);
void func_800DC968(Vec3f *, u8, Vec3f *, u16, u8, void *, u8, s32, u8, s32);
void func_800E4F3C(u8, Vec3f *, u16, u8, u8);
s32 func_800EC1F8(Vec3f *, u8, u8, s32, s32, s32);
void func_800EC790(Vec3f *, u8, u16, u8, s32, s32, u8, Driver *, f32, f32);
void *func_800EFC70(Vec3f *, u8, u16, u16, f32 *, f32, s32, s32, u8);
void func_800EFDC8(void *, Vec3f *, u16, u16, f32 *, f32);
void func_800F1B30(Vec3f *, u8, s32, u8);
void func_800F28AC(Vec3f *, f32 *, f32 *, u8, u16, u8, u8);
void func_800F8660(Vec3f *, u8, u8);

#define ST(t) D_80122E40[(t)->type]
#define NOW D_8021945C

/* recoil kick applied after a shot: k = strength, randomised by +-20% */
static inline void recoil(Tank *t, f32 k) {
    s32 ra = t->turret - 0x8000;
    f32 rk = k * (func_8009D8A0(0.4f) + 0.8f);

    t->recoilX += rk * func_8009D510(ra);
    t->recoilZ -= rk * func_8009D4B0(ra);
    t->recoil1E8 = 0.0f;
    t->recoil1F0 = 0.0f;
}

s32 func_8008C5D8(Tank *t, s32 weapon, s32 mode) {
    switch (weapon) {
    case 1:
        switch (t->type) {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
        case 11: case 12: case 13: case 14: {
            Vec3f pos;
            f32 acc[2];
            f32 vel[2];
            u16 ang;

            acc[0] = 0.0f;
            acc[1] = 0.0f;
            if (ST(t).flags & 2) {
                Vec3f m;

                m = ST(t).muzzle;
                if (t->flags & 4) {
                    m.x = -m.x;
                }
                t->flags ^= 4;
                func_8008C3B4(t, &m, 0, &pos, &ang);
            } else {
                func_8008C3B4(t, &ST(t).muzzle, 0, &pos, &ang);
            }
            vel[0] = func_8009D4B0(ang) * ST(t).speed;
            vel[1] = func_8009D510(ang) * ST(t).speed;
            if (ST(t).sound != -1) {
                func_80095B90(t, ST(t).sound);
            }
            func_800D5C80(&pos, t->team, vel, acc, ang, (f32)ST(t).damage * t->drv->scale, ST(t).fxScale,
                          ST(t).fxType, t->id, 1, t->drv, ST(t).speed);
            recoil(t, (f32)ST(t).damage * 6.0f / 40.0f);
            if (ST(t).kind != 0x107) {
                t->ptr238 = D_80123A58;
                t->t230 = NOW;
            }
            return 0;
        }
        case 10: {
            Vec3f pos1;
            Vec3f pos2;
            f32 acc2[2];
            f32 vel2[2];
            Vec3f m2;
            u16 ang2;

            m2 = ST(t).muzzle;
            m2.x = -m2.x;
            func_8008C3B4(t, &m2, 0, &pos1, &ang2);
            func_8008C3B4(t, &ST(t).muzzle, 0, &pos2, &ang2);
            vel2[0] = func_8009D4B0(ang2) * ST(t).speed;
            vel2[1] = func_8009D510(ang2) * ST(t).speed;
            switch (mode) {
            case 0:
                acc2[0] = vel2[1] / 700.0f;
                acc2[1] = -vel2[0] / 700.0f;
                break;
            case 1:
                acc2[0] = vel2[1] / 60.0f - vel2[0] / 30.0f;
                acc2[1] = -vel2[0] / 60.0f - vel2[1] / 30.0f;
                break;
            case 2:
                acc2[0] = -vel2[1] / 80.0f - vel2[0] / 30.0f;
                acc2[1] = vel2[0] / 80.0f - vel2[1] / 30.0f;
                break;
            }
            func_800D5C80(&pos1, t->team, vel2, acc2, ang2, (f32)ST(t).damage * t->drv->scale, ST(t).fxScale,
                          ST(t).fxType, t->id, 1, t->drv, ST(t).speed);
            switch (mode) {
            case 0:
                acc2[0] = -vel2[1] / 700.0f;
                acc2[1] = vel2[0] / 700.0f;
                break;
            case 1:
                acc2[0] = vel2[1] / 80.0f - vel2[0] / 30.0f;
                acc2[1] = -vel2[0] / 80.0f - vel2[1] / 30.0f;
                break;
            case 2:
                acc2[0] = -vel2[1] / 60.0f - vel2[0] / 30.0f;
                acc2[1] = vel2[0] / 60.0f - vel2[1] / 30.0f;
                break;
            }
            func_80095B90(t, 8);
            func_800D5C80(&pos2, t->team, vel2, acc2, ang2, (f32)ST(t).damage * t->drv->scale, ST(t).fxScale,
                          ST(t).fxType, t->id, 1, t->drv, ST(t).speed);
            if (ST(t).kind != 0x107) {
                t->ptr238 = D_80123A58;
                t->t230 = NOW;
            }
            return 0;
        }
        case 9: {
            Vec3f pos3;
            Vec3f vel3;
            u16 ang3;

            func_8008C3B4(t, &ST(t).muzzle, 0, &pos3, &ang3);
            func_80095B90(t, 0x12);
            vel3.x = func_8009D4B0(ang3);
            vel3.z = 0.0f;
            vel3.y = func_8009D510(ang3);
            func_800DC968(&pos3, t->team, &vel3, ang3, t->drv->id, D_801155EC, (f32)ST(t).damage * t->drv->scale,
                          t->id, (t->flags >> 1) & 1, 7);
            if (ST(t).kind != 0x107) {
                t->ptr238 = D_80123A7C;
                t->t230 = NOW;
            }
            return 0;
        }
        case 8: {
            Vec3f pos4;
            u16 ang4;
            s32 dt;
            s32 off;

            dt = NOW - t->t28C;
            off = 0;
            if (mode & 1) {
                off = 0x4000;
            } else if (mode & 2) {
                off = 0xC000;
            }
            func_8008C3B4(t, &ST(t).muzzle, off, &pos4, &ang4);
            if (D_80219488 * 6.0f < (f32)dt) {
                t->em290 = func_800EFC70(&pos4, t->team, ang4, 0xAA9 - t->pitch, t->dir, t->speed, t->id,
                                         t->flags & 2, t->b95);
                t->t28C = NOW;
            } else if (t->em290 != 0 && (f32)dt <= D_80219488 * 4.0f) {
                t->t28C = NOW;
                func_800EFDC8(t->em290, &pos4, ang4, 0xFFFF - t->pitch, t->dir, t->speed);
            }
            return 0;
        }
        }
        return 0;
    case 8: {
        Vec3f p;

        if (mode == 3) {
            p = t->pos;
            p.z += (f32)ST(t).height;
            func_800F8660(&p, t->team, t->b95);
            t->u206 -= 14;
        } else {
            func_800966D4(t, 0);
        }
        return 0;
    }
    case 2: {
        Vec3f pos;
        f32 acc[2];
        f32 vel[2];
        f32 v[2];
        u16 ang;
        u32 dmg;

        dmg = t->drv->scale * 30.0f;
        func_8008C3B4(t, &ST(t).muzzle2, 0, &pos, &ang);
        vel[0] = func_8009D4B0(ang) * 80.0f;
        vel[1] = func_8009D510(ang) * 80.0f;
        v[1] = -vel[0];
        v[0] = vel[1];
        if (mode == 3) {
            t->flags |= 0x800;
        }
        if (t->flags & 0x800) {
            f32 r;

            r = func_8009D8A0(6.0f) - 3.0f;
            vel[0] += (v[0] * r) / 10.0f;
            vel[1] += (v[1] * r) / 10.0f;
            acc[0] = (-v[0] * r) / 200.0f;
            acc[1] = (-v[1] * r) / 200.0f;
            func_800D5C80(&pos, t->team, vel, acc, ang, dmg, 1.0f, 2, t->id, 1, t->drv, ST(t).speed);
            if (func_8009D914() % 3 == 0) {
                func_80095B90(t, 0x14);
            }
        } else {
            acc[0] = 0.0f;
            acc[1] = 0.0f;
            func_800D5C80(&pos, t->team, vel, acc, ang, dmg, 1.0f, 2, t->id, 1, t->drv, ST(t).speed);
            vel[0] += v[0] / 10.0f;
            vel[1] += v[1] / 10.0f;
            acc[0] = -v[0] / 200.0f;
            acc[1] = -v[1] / 200.0f;
            func_800D5C80(&pos, t->team, vel, acc, ang, dmg, 1.0f, 2, t->id, 1, t->drv, ST(t).speed);
            vel[0] -= v[0] / 5.0f;
            vel[1] -= v[1] / 5.0f;
            acc[0] = v[0] / 200.0f;
            acc[1] = v[1] / 200.0f;
            func_800D5C80(&pos, t->team, vel, acc, ang, dmg, 1.0f, 2, t->id, 1, t->drv, ST(t).speed);
            func_80095B90(t, 0x14);
        }
        recoil(t, 6.0f);
        return 0;
    }
    case 9: {
        Vec3f pos;
        Vec3f vel;
        u16 ang;
        u8 dmg;

        dmg = t->drv->scale * 15.0f;
        func_8008C3B4(t, &ST(t).muzzle2, 0, &pos, &ang);
        vel.x = func_8009D4B0(ang) * 100.0f;
        vel.z = 0.0f;
        vel.y = func_8009D510(ang) * 100.0f;
        if (mode == 3) {
            t->flags |= 0x800;
        }
        func_800DAAE0(&pos, t->team, &vel, ang, dmg, 1, t->id, t->drv);
        func_80095B90(t, 0x3A);
        if (!(t->flags & 0x800)) {
            func_800DAAE0(&pos, t->team, &vel, ang, dmg, 1, t->id, t->drv);
            func_800DAAE0(&pos, t->team, &vel, ang, dmg, 1, t->id, t->drv);
        }
        return 0;
    }
    case 4: {
        Vec3f pos;
        f32 v[2];
        u16 ang;
        Driver *d;
        void *proj;

        d = t->drv;
        func_8008C3B4(t, &ST(t).muzzle2, 0, &pos, &ang);
        v[0] = func_8009D4B0(ang) * 25.0f;
        v[1] = func_8009D510(ang) * 25.0f;
        proj = func_800DC3A8(&pos, t->team, t->f4C0 * 25.0f, ang, d, t->drv->scale * 70.0f, (t->flags >> 1) & 1,
                             &d->vecB, &d->vecA, t->id, mode == 3 ? t->u1FE : 0, t->f4C0 < 1.0f);
        if (mode == 3) {
            t->u1FE = 1;
        }
        if (proj != 0) {
            func_80095B90(t, 0x14);
            if (d->owner == t->id) {
                func_800A8B14(d, proj);
                func_800A6B08(d->state, D_80122360);
                func_800A6B38(d->state, (u8 *)proj + 0x28);
            }
        }
        return 0;
    }
    case 3: {
        Vec3f pos;
        Vec3f vel;
        u16 ang;

        func_8008C3B4(t, &ST(t).muzzle2, 0, &pos, &ang);
        func_80095B90(t, 0x12);
        vel.x = func_8009D4B0(ang);
        vel.z = 0.0f;
        vel.y = func_8009D510(ang);
        func_800DC968(&pos, t->team, &vel, ang, t->drv->id, D_801155EC, t->drv->scale * 35.0f, t->id,
                      (t->flags >> 1) & 1, mode == 3 ? 0x16 : 7);
        if (mode == 3) {
            if (t->u1FC - 1 < 15) {
                t->u1FC = 1;
            } else {
                t->u1FC -= 14;
            }
        }
        if (ST(t).kind2 != 0x107) {
            t->ptr23C = D_80123A7C;
            t->t234 = NOW;
        }
        return 0;
    }
    case 7: {
        Vec3f pos;
        u16 ang;

        func_8008C3B4(t, &ST(t).muzzle2, 0, &pos, &ang);
        func_800EC790(&pos, t->team, ang, 100, 1, t->id, 1, t->drv, 0.0f, t->f4C0);
        func_800979F4(0x41);
        return 0;
    }
    case 14: {
        Vec3f pos;
        u16 ang;

        func_8008C3B4(t, &ST(t).muzzle2, 0, &pos, &ang);
        func_80095B90(t, 0x2E);
        func_800EC790(&pos, t->team, ang, t->drv->scale * 50.0f, 0, t->id, 1, t->drv,
                      mode == 3 ? (f32)t->u212 : 0.0f, t->f4C0);
        if (mode == 3) {
            t->u212 -= 9;
        }
        return 0;
    }
    case 10:
        if (t->t240 != 0) {
            t->t240 += 0x69;
        } else {
            func_80095B90(t, 0x2F);
            t->t240 = NOW + 0x69;
            t->t244 = NOW;
        }
        return 0;
    case 11: {
        f32 k;

        switch (t->type) {
        case 2:
            k = 2.0f;
            break;
        case 6:
            k = 0.66f;
            break;
        default:
            k = 1.0f;
            break;
        }
        t->ammo += (s32)(k * 15.0f);
        if (t->ammoMax < t->ammo) {
            t->ammo = t->ammoMax;
        }
        func_80095B90(t, 0x3B);
        return 0;
    }
    case 13:
        if (t->t260 != -1) {
            t->t268 += 0x1C2;
        } else {
            t->t264 = 1;
            t->t260 = NOW;
            func_80095B90(t, 0x37);
            if (mode == 3) {
                t->i4BC = 0;
                t->u210 = 1;
                t->t268 = NOW + 0x708;
                t->flags |= 0x1000;
                t->u4B8 = t->yaw + t->turret;
            } else {
                t->t268 = NOW + 0x1C2;
            }
        }
        return 0;
    case 6: {
        if (mode == 3) {
            f32 b[2];
            f32 c[2];
            Vec3f a;

            a = t->pos;
            a.z = 0.0f;
            b[0] = -func_8009D4B0(t->yaw) * 100.0f;
            b[1] = -func_8009D510(t->yaw) * 100.0f;
            c[0] = b[1];
            c[1] = -b[0];
            func_800F28AC(&a, b, c, t->b95, t->yaw, t->team, 0);
            t->u202 = 1;
        } else {
            Vec3f q;

            q.x = t->pos.x;
            q.y = t->pos.y;
            q.z = func_800B93A4(&q, t->team);
            func_800F1B30(&t->pos, t->team, 0, t->b95);
            func_80095B90(t, 0x13);
        }
        return 0;
    }
    case 15: {
        if (mode == 3) {
            t->flags |= 0x800;
        }
        if (t->flags & 0x800) {
            if (D_802195D4 % 6 == 0) {
                Vec3f p;

                p.x = t->pos.x;
                p.z = (f32)ST(t).height;
                p.y = t->pos.y;
                func_800EC790(&p, t->team, func_8009D914() % 0xFFFF, 0, 2, t->id, 1, t->drv, 0.0f, 1.0f);
                if (func_8009D914() % 3 == 0) {
                    func_80097FB4(0x34, t->pos.x, t->pos.y, 1.0f, t->team);
                }
            }
        } else {
            Vec3f p;

            p.x = t->pos.x;
            p.y = t->pos.y;
            p.z = func_800B93A4(&p, t->team);
            func_800F1B30(&t->pos, t->team, 1, t->b95);
        }
        return 0;
    }
    case 16:
        if (mode == 3) {
            f32 b[2];
            f32 c[2];
            Vec3f a;

            a = t->pos;
            a.z = 0.0f;
            b[0] = -func_8009D4B0(t->yaw) * 100.0f;
            b[1] = -func_8009D510(t->yaw) * 100.0f;
            c[0] = b[1];
            c[1] = -b[0];
            func_800F28AC(&a, b, c, t->b95, t->yaw, t->team, 1);
            t->u216 = 1;
        } else {
            Vec3f r;

            r.x = t->pos.x + func_8009D4B0(t->yaw) * (f32)(ST(t).range - 50);
            r.y = t->pos.y + func_8009D510(t->yaw) * (f32)(ST(t).range - 50);
            r.z = func_800B93A4(&r, t->team);
            func_800E4F3C(5, &r, t->yaw + 0x8000, t->team, t->b95);
            func_80095B90(t, 0x17);
        }
        return 0;
    case 12: {
        Vec3f pos;
        u16 ang;
        s32 dt;

        dt = NOW - t->t28C;
        func_8008C3B4(t, &ST(t).muzzle, 0, &pos, &ang);
        if (D_80219488 * 6.0f < (f32)dt) {
            t->em290 = func_800EFC70(&pos, t->team, ang, 0xAA9 - t->pitch, t->dir, t->speed, t->id,
                                     t->flags & 2, t->b95);
            if (mode == 3) {
                t->em294 = func_800EFC70(&pos, t->team, ang + 0x1556, 0xAA9 - t->pitch, t->dir, t->speed,
                                         t->id, t->flags & 2, t->b95);
                t->em298 = func_800EFC70(&pos, t->team, ang - 0x1556, 0xAA9 - t->pitch, t->dir, t->speed,
                                         t->id, t->flags & 2, t->b95);
                t->u20E -= 2;
            } else {
                t->em294 = 0;
                t->em298 = 0;
            }
            t->t28C = NOW;
        } else if (t->em290 != 0 && (f32)dt <= D_80219488 * 4.0f) {
            u16 spread;

            t->t28C = NOW;
            func_800EFDC8(t->em290, &pos, ang, 0xFFFF - t->pitch, t->dir, t->speed);
            spread = (s32)func_8009D8A0(2732.0f) + 0xAAA;
            if (t->em294 != 0) {
                func_800EFDC8(t->em294, &pos, ang + spread, 0xFFFF - t->pitch, t->dir, t->speed);
                if (t->u20E >= 2U) {
                    t->u20E -= 1;
                }
            }
            if (t->em298 != 0) {
                func_800EFDC8(t->em298, &pos, ang - spread, 0xFFFF - t->pitch, t->dir, t->speed);
                if (t->u20E >= 2U) {
                    t->u20E -= 1;
                }
            }
        } else {
            t->em294 = 0;
            t->em298 = 0;
        }
        return 0;
    }
    case 17: {
        Vec3f p;
        Vec3f unused;   /* frame slot 0x1E8: declared but never referenced in the ROM */

        p.x = t->pos.x;
        p.z = t->pos.z + 25.0f;
        p.y = t->pos.y;
        func_800A5BD8(&p, 0, t->team, (f32)D_8021958C->val / 2250.0f, D_801151A4,
                      func_800EC1F8(&p, t->team, t->b95, 100, 3, 1));
        t->drv->flagsA &= ~8;
        func_80095B90(t, 0x36);
        func_800CA620(t->drv->id, gEdgePowerUsedMessage, 0x2D);
        return 0;
    }
    }
    return 0;
}
