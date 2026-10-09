/* SPAN 0x8008C5BC */
/* RODATA_VRAM 0x80071908 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { char p0[0xB]; u8 id; char pc[4]; int team; } Unit;
typedef struct { char p0[0x90]; int flags; float ox; float oy; float oz; char pa0[0xD0 - 0xA0]; } Zone;
typedef struct Tank {
    int b0; char p4[4]; float x; float y; char p10[0x20 - 0x10]; u16 heading; char p22[6]; char fx[0x98 - 0x28];
    int zone; char p9c[0x1D0 - 0x9C];
    Unit *owner; char p1d4[4]; int hp; int shield; int flags; float vx; float f1e8; float vz; float f1f0;
    char p1f4[0x260 - 0x1F4]; int hitTime;
} Tank;
extern u8 D_80125AB0;
extern u16 D_80114694;
extern int D_8021945C;
extern Zone D_80122E28[];
void func_800A9B64(Unit *, int);
void func_8008B82C(Tank *, int, int);
void func_800F0ED0(int, float *, int, int);
void func_80095B90(Tank *, int);
float func_8009D4B0(u16);
float func_8009D510(u16);
float func_8009D8A0(float);
void func_80098B58(int, int, int, int);
void func_800B61B8(void *, float *, float, int, float *);

inline void func_8008BD5C(Tank *t, int dmg, Unit *src, u16 big) {
    if (src != 0 && src->team == t->owner->team) {
        func_800A9B64(src, 7);
    }
    if (D_80125AB0 != 0 && D_80114694 == 0 && (t->flags & 2)) return;
    t->hp -= dmg;
    if (t->hitTime != -1) t->hitTime = D_8021945C;
    if (t->hp > 0) return;
    t->hp = 1;
    if (big) {
        func_8008B82C(t, src ? src->id : 127, 1);
    } else {
        func_8008B82C(t, src ? src->id : 127, (dmg > 500) * 2);
    }
}
float func_8008BE74(Tank *t) {
    switch (t->zone) {
    case 2:
        return 2.0f;
    case 6:
        return 0.66f;
    default:
        return 1.0f;
    }
}

inline void func_8008BEC4(Tank *t, u16 angle, float magnitude) {
    u16 rel;
    float f;

    rel = angle - t->heading;
    f = magnitude * (func_8009D8A0(0.4f) + 0.8f);
    t->vx += f * func_8009D510(rel);
    t->vz -= f * func_8009D4B0(rel);
    t->f1e8 = 0.0f;
    t->f1f0 = 0.0f;
}

void func_8008BF5C(Tank *t, int dmg, Unit *src, int unused, float *hit, u16 ang, int w, int kind) {
    if (kind != 1) {
        if (t->shield > 0) {
            func_800F0ED0(t->b0, hit, dmg, w);
            func_80095B90(t, 29);
            t->shield -= dmg;
            if (t->shield > 0) return;
            dmg += t->shield;
        }
        if (D_80122E28[t->zone].flags & 0x20) {
            float c = func_8009D4B0(t->heading);
            float s = func_8009D510(t->heading);
            if ((hit[0] - t->x) * c + (hit[1] - t->y) * s > 0.0f) {
                if (t->flags & 2) {
                    func_8008BD5C(t, dmg / 4, src, 0);
                    func_80098B58(t->owner->id, dmg / 4, 100, 0);
                } else {
                    func_8008BD5C(t, dmg / 8, src, 0);
                }
                return;
            }
        }
    }
    func_8008BD5C(t, dmg, src, 0);
    func_8008BEC4(t, ang, dmg * 6.0f / 30.0f);
    if (t->flags & 2) func_80098B58(t->owner->id, dmg, 100, 0);
    func_800B61B8(t->fx, &t->x, dmg * 80, ang, hit);
}

typedef float Mtx[4][4];
typedef struct {
    char p0[8]; float x; float y; float z; char p14[0x20 - 0x14]; u16 heading; char p22[2]; u16 turret;
    char p26[0x98 - 0x26]; int zone; char p9c[0x4C0 - 0x9C]; float scale;
} Tank2;

void func_8009EFD4(Mtx, float, float, float, int);
void func_8009EEE0(Mtx);
void func_8009F824(Mtx, float, float, float);
void func_8009F4B4(Mtx, Mtx, Mtx);
void func_8009F288(Mtx, void *, int);

void func_8008C3B4(Tank2 *e, void *out, u16 base, int mode, u16 *angle) {
    Mtx m1 = {
        { 1.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f, 1.0f },
    };
    Mtx m2;
    Mtx m3;

    if (D_80122E28[e->zone].flags & 4) {
        func_8009EFD4(m1, e->x, e->z, e->y, e->heading);
        func_8009EEE0(m2);
        func_8009EFD4(m2, D_80122E28[e->zone].ox, D_80122E28[e->zone].oz, D_80122E28[e->zone].oy, e->turret);
        if (e->scale != 1.0f) func_8009F824(m2, e->scale, e->scale, e->scale);
        func_8009F4B4(m2, m1, m3);
        func_8009F288(m3, out, mode);
    } else {
        func_8009EFD4(m1, e->x, e->z, e->y, (u16)(e->heading + e->turret));
        if (e->scale != 1.0f) func_8009F824(m1, e->scale, e->scale, e->scale);
        func_8009F288(m1, out, mode);
    }
    *angle = base + (e->heading + e->turret);
}
