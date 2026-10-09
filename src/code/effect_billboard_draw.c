typedef struct { float x, y, z; } Vec3;
typedef struct { float m[17]; } M44;
typedef struct { unsigned int w0, w1; } Gfx;
typedef struct { char pad[0x46]; short h; char rest[0xD0 - 0x48]; } Ent;
typedef struct {
    int *p; int p4; Vec3 pos; char pad[148 - 20]; unsigned char b94; char p95[3]; int w98;
} Owner;
typedef struct {
    char pad[12]; Vec3 pos; unsigned char b18; char p19[3];
    Owner *owner; int w20; unsigned short r24, r26, r28, s2a, s2c, s2e; int w30; float f34;
} Obj;
extern M44 D_800772A0;
extern int D_8021945C;
extern short D_80122E46[];
extern int D_803A561C;
extern unsigned char func_800AD14C(float, float, float, int);
extern void func_8009EF30(M44 *, Vec3 *);
extern void func_8009F090(M44 *, int, int, int);
extern void func_8009F824(M44 *, float, float, float);
extern void func_800AD9A8(int, int, M44 *, int, int, int, Gfx *, int);
void func_800F6C70(void *arg) {
    M44 m = D_800772A0;
    Obj *o = arg;
    Gfx g[2];
    unsigned char r;
    unsigned char a;
    int d;
    if (o->owner != 0) {
        if (*o->owner->p != o->w20) return;
        r = func_800AD14C(o->owner->pos.x, o->owner->pos.y, o->f34, o->owner->b94);
    } else {
        r = func_800AD14C(o->pos.x, o->pos.y, o->f34, o->b18);
    }
    if (r) {
        d = D_8021945C - o->w30;
        if (d < 6) a = (d << 8) / 6;
        else a = ~((d - 6) * 255 / 45);
        if (o->owner != 0) {
            Vec3 t;
            t = o->owner->pos;
            t.z += (float)(D_80122E46[o->owner->w98 * 104] / 2);
            func_8009EF30(&m, &o->owner->pos);
        } else {
            func_8009EF30(&m, &o->pos);
        }
        func_8009F090(&m, o->r24, o->r26, o->r28);
        func_8009F824(&m, o->f34, o->f34, o->f34);
        {
        register int h asm("a0") = D_803A561C;
        g[0].w0 = 0xFA000000;
        g[0].w1 = a | 0xFF00;
        g[1].w0 = 0xFB000000;
        g[1].w1 = (a << 24) | (a << 16) | 0xFF00;
        func_800AD9A8(h, 0, &m, 1, 0, r, g, 2);
        }
    }
}
