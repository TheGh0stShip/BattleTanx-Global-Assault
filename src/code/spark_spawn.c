/* ---- 0x800F6800/c/src/f799c.c ---- */
typedef struct { int x, y, z; } V3;
typedef struct Obj { char pad0[12]; V3 p; unsigned char k; char p19; short ang; float spd; float r; unsigned char c24; unsigned char c25; } Obj;
extern float func_8009D8A0(float);
extern unsigned int func_8009D914(void);
extern Obj *func_800A18D0(int, int);
void func_800F799C(V3 *p, unsigned char k) {
    Obj *o = func_800A18D0(58, 40);
    if (o == 0) return;
    o->p = *p;
    o->ang = func_8009D914() % 0xFFFF;
    o->spd = (func_8009D8A0(0.2f) + 0.9f) * 1e+01f;
    o->r = 1.0f;
    o->c24 = func_8009D914() & 0x1f;
    o->c25 = func_8009D914() & 1;
    o->k = k;
}

