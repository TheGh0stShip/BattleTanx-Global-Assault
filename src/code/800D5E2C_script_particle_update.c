/* Unit 0x800D5E2C..0x800D6508 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/r3/d5e2c_d6508/f5e2c.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800D6508 */
typedef struct { float x, y, z; } Vec3;
typedef struct { unsigned int unk0; int unk4; int pad[5]; } Info;
typedef struct { int pad0; int type; } Obj;
typedef struct { Obj *obj; int pad; Vec3 pos; int pad2[4]; } Hit;
typedef struct { Vec3 pos; short angle; int unk10; int unk14; int unk18; int pad1C; int unk20; int pad24; } Msg;
typedef struct { void (*fn)(Obj *, void *, int, Msg *, Info *); int pad[2]; } HitFn;
typedef struct {
    char pad0[0xA];
    unsigned short angle;
    union { struct { float x, y, z; } f; Vec3 v; } p;
    float vx, vy;
    float ax, ay;
    unsigned char unk28, unk29, type, unk2B;
    Obj *unk2C;
    int unk30;
    float timer;
    char pad38[8];
    unsigned char counter;
} Proj;
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
extern float D_80074F78, D_80074F7C, D_80074F80, D_80074F84, D_80074F88, D_80074F8C, D_80074F90;
extern Info D_80074F5C;
extern float D_80219488;
extern int D_80121D68[];
extern int D_80121D74[];
extern int D_80121D80[];
extern short D_80397650;
extern HitFn D_80224B5C[];
extern void func_800A5BD8(Vec3 *, int, int, float, int, Vec3 *);
extern int func_800B49E0(Vec3 *, Vec3 *, int, int, int, Obj *, Hit *);
extern float func_8009D4B0(int);
extern float func_8009D510(int);
extern int func_8009D914(void);
extern void func_80097FB4(int, float, float, float, int);
extern int func_800B02D4(float, float, int);

void func_800D5E2C(Proj *s, int *out)
{
    int kill = 0;
    int fx = 1;
    Hit hit;
    Vec3 v;
    Vec3 pos;
    Vec3 a;
    Vec3 b;
    Msg m;
    Info r;
    float spd;
    float t;
    float sc;

    if (s->timer < D_80074F78) {
    pos.x = s->p.f.x + s->vx * D_80219488;
    pos.y = s->p.f.y + s->vy * D_80219488;
    pos.z = s->p.f.z;
    s->vx += s->ax * D_80219488;
    s->timer += D_80219488;
    s->vy += s->ay * D_80219488;
    if (D_80121D68[s->type] != 0) {
        if ((s->counter++ % D_80121D74[s->type]) == 0) {
            v.x = pos.x + (s->vx + s->vx) * D_80219488;
            v.y = pos.y + (s->vy + s->vy) * D_80219488;
            v.z = pos.z;
            func_800A5BD8(&pos, 0, s->unk2B, 1.0f, D_80121D68[s->type], &v);
        }
    }
    a = s->p.v;
    b = pos;
    if (s->p.f.z > D_80074F7C) {
        a.z = D_80074F80; b.z = D_80074F80;
    }
    {
    int k = s->unk2B;
    Obj *o = s->unk2C;
    D_80397650 = 1;
    if ((unsigned short)func_800B49E0(&a, &b, 0x64940B, k, 0, o, &hit)) {
        hit.pos.z = s->p.f.z;
        if (hit.obj != 0) {
            r = D_80074F5C;
            m.pos = hit.pos;
            m.angle = s->angle;
            m.unk10 = s->unk28;
            m.unk14 = s->unk30;
            m.unk18 = s->unk29;
            m.unk20 = 0;
            if (D_80224B5C[hit.obj->type].fn != 0) {
                D_80224B5C[hit.obj->type].fn(hit.obj, s, 0, &m, &r);
            }
            switch (r.unk0) {
            case 1:
                kill = 1;
                break;
            case 3:
                kill = 1;
                fx = 0;
                break;
            case 4:
                kill = 1;
                break;
            case 5:
                s->angle = *(unsigned short *)&r.unk4;
                if (ABS(s->vx) > ABS(s->vy)) { spd = s->vx; if (!(spd > 0.0f)) spd = -spd; } else { spd = s->vy; if (!(spd > 0.0f)) spd = -spd; }
                if (ABS(s->vx) < ABS(s->vy)) { t = s->vx; if (t > 0.0f) goto pos; else goto neg; }
                else { t = s->vy; if (t > 0.0f) goto pos; else goto neg; }
            pos:
                sc = spd + t * D_80074F84 / D_80074F88; goto done5;
            neg:
                sc = spd + -t * D_80074F8C / D_80074F90;
            done5:
                s->vx = sc * func_8009D4B0(s->angle);
                s->vy = sc * func_8009D510(s->angle);
                s->ax = 0.0f;
                s->ay = 0.0f;
                s->unk30 = 0;
                pos.x = s->p.f.x;
                pos.y = s->p.f.y;
                s->unk2C = hit.obj;
                break;
            }
        } else {
            kill = 1;
        }
    }
    }
    if (kill) {
        *out = 1;
        if (fx) {
            if (s->type == 1) {
                switch ((unsigned int)func_8009D914() & 3) {
                case 0: func_80097FB4(25, s->p.f.x, s->p.f.y, 1.0f, s->unk2B); break;
                case 1: func_80097FB4(26, s->p.f.x, s->p.f.y, 1.0f, s->unk2B); break;
                case 2: func_80097FB4(27, s->p.f.x, s->p.f.y, 1.0f, s->unk2B); break;
                case 3: func_80097FB4(28, s->p.f.x, s->p.f.y, 1.0f, s->unk2B); break;
                }
            } else if (hit.obj != 0) {
                switch (hit.obj->type) {
                case 14:
                case 22:
                    break;
                case 4:
                    func_80097FB4(32, s->p.f.x, s->p.f.y, 1.0f, s->unk2B);
                    break;
                default:
                    func_80097FB4(10, s->p.f.x, s->p.f.y, 1.0f, s->unk2B);
                    break;
                }
            } else {
                func_80097FB4(42, s->p.f.x, s->p.f.y, 1.0f, s->unk2B);
            }
            func_800A5BD8(&hit.pos, 0, s->unk2B, 1.0f, D_80121D80[s->type], 0);
        }
    } else {
        s->p.f.x = pos.x;
        s->p.f.y = pos.y;
        if (!func_800B02D4(pos.x, pos.y, s->unk2B)) {
            *out = 1;
        }
    }
    } else {
        *out = fx;
    }
}
