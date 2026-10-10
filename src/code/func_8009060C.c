typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { f32 x, y; } Vec2;

typedef struct {
    char p0[0x10];
    s32 team;
    char p14[0x250 - 0x14];
} Owner;

typedef struct {
    char p0[0x0B];
    u8 id;
    char p0c[0x10 - 0x0C];
    s32 team;
    char p14[0x250 - 0x14];
} Player;

typedef struct {
    char p0[8];
    Vec2 pos;
    char p10[0x10];
    u16 ang;
    char p22[0x60 - 0x22];
    f32 f60;
    char p64[0x7C - 0x64];
    f32 heat[4];
    char p8c[0x1D0 - 0x8C];
    Owner *owner;
    char p1d4[4];
    s32 health;
    char p1dc[4];
    s32 flags;
    f32 shakeX;
    s32 shakeVX;
    f32 shakeY;
    s32 shakeVY;
    u16 index;
    char p1f6[0x250 - 0x1F6];
    s32 f250;
    s32 f254;
    char p258[8];
    s32 f260;
} Tank;

typedef struct {
    s32 type;
    s32 p4;
    s32 amount;
    s32 source;
    Vec2 pos;
} Event;

typedef struct { char p[0x28]; } Slot;

extern Player D_80235F00[];
extern Slot D_803978E0[];
extern u8 D_80125AB0;
extern u16 D_80114694;
extern s32 D_8021945C;

void func_8008B82C(Tank *t, u8 by, s32 how);
void func_8008ECB4(Tank *t, s32 amount, s32 c);
void func_80095B90(Tank *t, s32 a);
s32 func_8009D144(void);
f32 func_8009D4B0();
f32 func_8009D510();
f32 func_8009D8A0(f32 a);
s32 func_8009E9C8(Vec2 *a, Vec2 *b);
void func_800A9A98(Owner *o, s32 a);
void func_800A9B64(Player *p, s32 a);
void func_800B2BE4(Slot *s, Vec2 *out);

#define SHAKE(t, mag, angle) \
    { \
        s32 ang = (angle); \
        (t)->shakeX += (mag) * func_8009D510(ang); \
        (mag) *= func_8009D4B0(ang); \
        (t)->shakeY -= (mag); \
        (t)->shakeVX = 0; \
        (t)->shakeVY = 0; \
    }

static inline void heatCorners(Tank *t, Event *ev, Vec2 *q) {
    u16 i;

    func_800B2BE4(&D_803978E0[t->index], q);
    for (i = 0; i < 4; i++) {
        Vec2 *c = &q[i];
        f32 dx = ev->pos.x - c->x;
        f32 dy = ev->pos.y - c->y;
        if (dx * dx + dy * dy < 3600.0f) {
            t->heat[i] += 4.0f;
            if (t->heat[i] > 4.0f) {
                t->heat[i] = 4.0f;
            }
        }
    }
}

void func_8009060C(Tank *t, Event *ev) {
    u16 hitAng;
    Player *p;
    Player *src;
    s32 amount;
    Vec2 pts[7];
    f32 mag;
    u16 back = 0x8000;

    switch (ev->type) {
    case 12:
    case 13:
        hitAng = func_8009E9C8(&ev->pos, &t->pos) - t->ang;
        mag = func_8009D8A0(0.4f) + 0.8f;
        mag *= 6.0f;
        SHAKE(t, mag, hitAng);
        src = (ev->source != 0x7F) ? &D_80235F00[ev->source] : 0;
        amount = ev->amount;
        p = src;
        if (p != 0 && p->team == t->owner->team) {
            func_800A9B64(p, 7);
        }
        if (D_80125AB0 == 0 || D_80114694 != 0 || !(t->flags & 2)) {
            t->health -= amount;
            if (t->f260 != -1) {
                t->f260 = D_8021945C;
            }
            if (t->health <= 0) {
                t->health = 1;
                func_8008B82C(t, p != 0 ? p->id : 0x7F, (amount > 500) * 2);
            }
        }
        if (ev->type == 13) {
            heatCorners(t, ev, pts);
        }
        break;
    case 6:
        func_8008ECB4(t, ev->amount, 1);
        func_80095B90(t, 0x18);
        if (func_8009D144() != 0) {
            func_800A9A98(t->owner, 0x19);
        }
        break;
    case 11:
        t->f60 = 0.0f;
        mag = func_8009D8A0(0.4f) + 0.8f;
        mag *= 6.0f;
        goto shake;
    case 7:
    case 10:
        t->f60 = 0.25f;
        mag = func_8009D8A0(0.4f) + 0.8f;
        mag *= 6.0f;
        goto shake;
    case 9:
        mag = func_8009D8A0(0.4f) + 0.8f;
        mag *= 3.0f;
    shake:
        SHAKE(t, mag, back);
        break;
    case 8:
        if (!(t->flags & 0x30)) {
            t->flags |= 0x10;
            t->f250 = 0;
            t->f254 = ev->amount;
        }
        break;
    }
}
