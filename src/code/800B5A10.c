/* SPAN 0x800B5B8C */
/* RODATA_VRAM 0x800730D0 */
typedef unsigned short u16;
typedef struct { float x, y; } Vec2;
typedef struct {
    float x, y;     /* 0x00 start */
    float dx, dy;   /* 0x08 unit direction */
    float len;      /* 0x10 */
} Seg;
typedef struct {
    u16 kind;
    float a, b, c, d, e, f;
} SegDef;
typedef struct {
    float unk0, unk4, unk8;
    float unkC, unk10, unk14;
    u16 unk18;
    float unk1C;
    short unk20;
    float unk24, unk28, unk2C, unk30;
    int unk34;
    float unk38;
    short unk3C, unk3E;
    char pad40[4];
    int unk44[4];
    int unk54[4];
    short unk64;
    Seg *seg;
} Mover;

float func_8009E948(float *v);
int func_8009DFAC(float *v);
float func_8009D4B0(u16 a);
float func_8009D510(u16 a);
void func_80097FB4(int kind, float x, float y, float s, int z);
extern float D_80219488;
extern u16 D_80116702;

void func_800B5A10(Mover *m, SegDef *d) {
    u16 i;

    m->unk0 = 0;
    m->unk4 = 0;
    m->unk8 = 0;
    m->unk1C = 0;
    m->unk20 = 0;
    m->unk3C = 0;
    m->unk3E = 0;
    m->seg = 0;
    m->unk38 = 1.0f;
    for (i = 0; i < 4; i++) {
        m->unk44[i] = 0;
        m->unk54[i] = 0;
    }
    m->unk18 = d->kind;
    m->unkC = d->a;
    m->unk10 = d->b;
    m->unk14 = d->c;
    m->unk24 = d->d;
    m->unk28 = d->e;
    m->unk2C = d->f;
    m->unk64 = 0;
    m->unk34 = 0;
    m->unk30 = m->unk18 * m->unkC * 1.5f;
}

void func_800B5AD0(Seg *s, float x0, float y0, float x1, float y1) {
    s->x = x0;
    s->y = y0;
    s->dx = x1 - x0;
    s->dy = y1 - y0;
    s->len = func_8009E948(&s->dx);
}

u16 func_800B5B28(Mover *m, Seg *s, Vec2 *mid) {
    m->seg = s;
    mid->x = s->x + s->dx * (s->len / 2.0f);
    mid->y = s->y + s->dy * (s->len / 2.0f);
    return func_8009DFAC(&s->dx);
}

extern inline void func_800B5B8C(Mover *m) {
    float dot;

    if (m->unk0 == m->seg->dx && m->unk4 == m->seg->dy) return;
    dot = m->unk0 * m->seg->dx + m->unk4 * m->seg->dy;
    if (dot < 0.0f) {
        m->unk8 = m->unk8 * -dot;
        m->unk0 = -m->seg->dx;
        m->unk4 = -m->seg->dy;
    } else {
        m->unk8 = m->unk8 * dot;
        m->unk0 = m->seg->dx;
        m->unk4 = m->seg->dy;
    }
}

extern inline u16 func_800B5C48(Mover *m, Vec2 *p) {
    Vec2 e[2];
    float r2, d0, d1, dx, dy;
    e[0].x = m->seg->x;
    e[0].y = m->seg->y;
    e[1].x = e[0].x + m->seg->dx * m->seg->len;
    e[1].y = e[0].y + m->seg->dy * m->seg->len;
    r2 = m->seg->len * m->seg->len;
    dx = p->x - e[0].x; dy = p->y - e[0].y;
    d0 = dx * dx + dy * dy;
    dx = p->x - e[1].x; dy = p->y - e[1].y;
    d1 = dx * dx + dy * dy;
    if (r2 < d0) { p->x = e[1].x; p->y = e[1].y; return 1; }
    if (r2 < d1) { p->x = e[0].x; p->y = e[0].y; return 1; }
    return 0;
}

extern inline void func_800B5D1C(Mover *m, Vec2 *v) {
    float k = m->unk8 * m->unk18;
    m->unk0 *= k;
    m->unk4 *= k;
    m->unk0 += v->x;
    m->unk4 += v->y;
    m->unk8 = func_8009E948(&m->unk0);
    m->unk8 /= m->unk18;
}

void func_800B5DA8(Mover *m, float f, u16 ang);

void func_800B5E70(Mover *m, Mover *o, float f);

extern inline void func_800B5F30(Mover *m, Vec2 *a, Vec2 *b, float amount) {
    float cross = a->x * b->y - a->y * b->x;

    if (cross < 0.0f) {
        amount = -amount;
    }
    m->unk1C += amount;
}

void func_800B5F70(Mover *m, Vec2 *a, float f, Vec2 *v, Vec2 *b);

void func_800B61B8(Mover *m, Vec2 *a, float f, u16 ang, Vec2 *b);

void func_800B623C(Mover *m, Vec2 *a, Vec2 *v, Vec2 *b);

void func_800B62A4(Mover *m, Mover *o, float f, Vec2 *a, Vec2 *b, u16 *d1, u16 *d2);

extern inline void func_800B641C(Mover *m, float f) {
    m->unk8 *= f;
    if (m->unk8 < 0.0f) {
        m->unk0 = -m->unk0;
        m->unk4 = -m->unk4;
        m->unk8 = -m->unk8;
    }
}

void func_800B6468(Mover *m, float f);

extern inline void func_800B64D4(Mover *m, float f) {
    float t;

    if (m->unk1C == 0.0f) return;
    t = (m->unk14 + f) * D_80219488;
    if (t <= 0.0f) return;
    if (m->unk1C > 0.0f) {
        m->unk1C -= t;
        if (m->unk1C < 0.0f) {
            m->unk1C = 0.0f;
        }
    } else {
        m->unk1C += t;
        if (m->unk1C > 0.0f) {
            m->unk1C = 0.0f;
        }
    }
}

void func_800B654C(Mover *m, float f, u16 ang);

void func_800B66E0(Mover *m, Vec2 *p);

u16 func_800B6934(Mover *m);
