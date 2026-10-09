/* Unit 0x800DB6F0..0x800DC214 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Three functions: func_800DB6F0 (0xB4), func_800DB7A4 (0xAC) and func_800DB850 (0x9C4). The first two are the
 * pitch-clamp helpers that func_800DB850 inlines, so they must be `inline` (not static) in the same translation unit.
 * Replaces the former model_height_to_angle.c unit and owns
 * 0x800DB6F0..0x800DBBE4 plus its complete rodata pool.
 * integrating this unit means replacing that file (its two functions are reproduced here byte-identically) and
 * extending the unit_rodata.tsv row to 0x80075490 0xFC (float literals plus the 23-entry jump table at 0x80075520).
 * Origin: claude-work/output/workers/0x800DA000/r2b/tu_db6f0.c; re-verified in this lane with tools/kmc_cmp.py.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DC214 */
/* RODATA_VRAM 0x80075490 */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32;
typedef struct { s32 unk0; u8 pad[0x1C]; } Unk800DB6F0;
extern Unk800DB6F0 *D_80219498;

inline u16 func_800DB6F0(f32 x, u8 idx) {
    Unk800DB6F0 *p = D_80219498;
    f32 v = (f32)*(s32 *)((u8 *)p + (idx << 5) + 32) - 30.0f;
    f32 a = v - 4.5f;
    if (a + 10.0f < x) {
        return 0x5555;
    }
    return (u32)((x - 10.0f) / a * 5461.0f + 16384.0f);
}

inline u16 func_800DB7A4(f32 x, u8 idx) {
    Unk800DB6F0 *p = D_80219498;
    f32 v = (f32)*(s32 *)((u8 *)p + (idx << 5) + 32) - 30.0f;
    f32 a = v - 4.5f;
    if (x < v - a) {
        return 0x2AAB;
    }
    return (u32)((v - x) / a * -5461.0f + 16384.0f);
}
typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[0xA]; unsigned char b10; unsigned char b11; Vec3 pos; float speed;
    unsigned short yaw, pitch; short s32; short timer;
    unsigned char b36, flags, team, owner; char pad28[0x18]; int i64;
} P;
typedef struct {
    char pad0[0xA]; unsigned char b10; unsigned char ctrl; char padC[0x84]; P *p144;
    char pad94[0xD8]; int i364; int i368; char pad174[0xDC];
} O;
typedef struct { signed char sx, sy; char pad2[6]; int b8; int b12; } C;
typedef struct { char pad0[4]; int type; char pad8[0x35]; unsigned char b61; } Obj;
typedef struct { Obj *obj; int pad4; Vec3 pos; float n[3]; unsigned short kind; } Hit;
typedef struct { Vec3 pos; short angle; int b36; O *owner; int b37; int pad; int zero; } Ev;
typedef struct { unsigned int result; int a, b, c, d, e, f; } Info;
typedef struct { void (*fn)(Obj *, P *, int, Ev *, Info *); int pad[2]; } Handler;
extern O D_80235F00[]; extern float D_80219488;
extern short D_80397650; extern Handler D_80224B5C[];
extern char D_80115E0C[], D_801159F4[], D_801155EC[];
extern C *func_80098250(int); extern void func_800A8E3C(O *);
extern float func_8009D4B0(int); extern float func_8009D510(int);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);
extern unsigned short func_800B49E0(Vec3 *, Vec3 *, int, int, int, int, Hit *);
extern int func_800B02D4(float, float, int);
extern void func_80097FB4(int, float, float, float, int);
extern void func_800DC968(Vec3 *, int, Vec3 *, int, int, void *, int, int, int, int);

static inline O *get_owner(int i) { if (i == 127) return 0; return &D_80235F00[i]; }

void func_800DB850(P *arg, int *done) {
    P *s = arg; O *o; C *c; Hit hit; Vec3 a; Vec3 np; Ev ev;
    int hitflag = 0; int fx = 1; unsigned short r; unsigned short lim; float f;

    o = get_owner(s->owner);
    if (--s->timer > 0) {
      if (s->flags & 1) {
        c = 0;
        if (s->flags & 4) {
            c = func_80098250(o->ctrl);
            if (c->b12 & o->i368) {
                s->flags &= ~4;
                if ((o->b10 & 2) && o->p144 == s) func_800A8E3C(o);
            } else {
                s->yaw -= (unsigned short)(c->sx * 10 * D_80219488);
                s->s32 = (unsigned short)(-c->sx * 10 * D_80219488);
                s->pitch += (unsigned short)(c->sy * 5 * D_80219488);
                lim = func_800DB7A4(s->pos.z, s->team);
                if (s->pitch < lim) s->pitch = lim;
                else {
                    lim = func_800DB6F0(s->pos.z, s->team);
                    if (lim < s->pitch) s->pitch = lim;
                }
            }
        }
        np.x = s->pos.x + s->speed * func_8009D4B0(s->yaw) * func_8009D4B0(s->pitch) * D_80219488;
        np.z = s->pos.z + s->speed * func_8009D510(s->pitch) * D_80219488;
        np.y = s->pos.y + s->speed * func_8009D510(s->yaw) * func_8009D4B0(s->pitch) * D_80219488;
        if (np.z > 190.0f) np.z = 190.0f;
        else if (np.z < 10.0f) np.z = 10.0f;
        a.x = s->pos.x; a.z = s->pos.z; a.y = s->pos.y;
        func_800A5BD8(&a, 0, s->team, s->b11 ? 0.25f : 1.0f, D_80115E0C, 0);
        {
            int team = s->team; int ign = s->i64;
            D_80397650 = 1;
            r = func_800B49E0(&s->pos, &np, 0x6494EB, team, 0, ign, &hit);
        }
        if (r) {
            if (hit.obj == 0) {
                hitflag = 1;
            } else {
                Info info = { 2, 0, 0, 0, 0, 0, 0 };
                ev.pos.x = hit.pos.x; ev.pos.z = hit.pos.z; ev.pos.y = hit.pos.y;
                ev.angle = s->yaw;
                ev.b36 = s->b36;
                ev.owner = get_owner(s->owner);
                ev.b37 = s->flags & 4;
                ev.zero = 0;
                if (D_80224B5C[hit.obj->type].fn)
                    D_80224B5C[hit.obj->type].fn(hit.obj, s, 0, &ev, &info);
                switch (info.result) {
                case 1: case 5: hitflag = 1; break;
                case 3: hitflag = 1; fx = 0; break;
                }
            }
        }
        if (hitflag) {
            int ok = 1;
            if ((s->flags & 8) && hit.obj) {
                switch (hit.obj->type) {
                case 10: case 14: case 16: case 21: case 22: case 24: case 25:
                case 26: case 28: case 32:
                    ok = 0;
                    break;
                case 12:
                    if (hit.obj->b61 != 7) ok = 0;
                    break;
                }
            }
            if (ok) {
                s->timer = 45;
                s->flags &= ~1;
            }
            if (fx) {
                                func_80097FB4(9, s->pos.x, s->pos.y, 1.0f, s->team);
                func_800A5BD8(&a, 0, s->team, s->b11 ? 0.25f : 1.0f, D_801159F4, 0);
            }
        } else if (!func_800B02D4(np.x, np.y, s->team) || np.z <= 0.0f) {
                        func_80097FB4(9, s->pos.x, s->pos.y, 1.0f, s->team);
            func_800A5BD8(&a, 0, s->team, s->b11 ? 0.25f : 1.0f, D_801159F4, 0);
            *done = 1;
        } else {
            s->pos.x = np.x; s->pos.y = np.y; s->pos.z = np.z;
        }
        if ((s->flags & 4) && (c->b8 & o->i364) && s->b10) {
            ev.pos.x = func_8009D4B0(s->yaw) * func_8009D4B0(s->pitch);
            ev.pos.z = func_8009D510(s->pitch);
            ev.pos.y = func_8009D510(s->yaw) * func_8009D4B0(s->pitch);
            func_800DC968(&s->pos, s->team, &ev.pos, s->yaw, s->owner, D_801155EC, s->b36 >> 1, 0, 1, 7);
            func_80097FB4(18, s->pos.x, s->pos.y, 1.0f, s->team);
            s->b10--;
        }
    } else if (s->flags & 4) {
        c = func_80098250(o->ctrl);
        if (c->b12 & o->i368) {
            s->flags &= ~4;
            if ((o->b10 & 2) && o->p144 == s) func_800A8E3C(o);
        }
      }
    } else {
        *done = 1;
    }
    if (*done == 1 && (o->b10 & 2) && o->p144 == s) func_800A8E3C(o);
}
