typedef struct { char pad0[12]; int pos[3]; unsigned char b18; char pad19; short s1a; float f1c; float f20; unsigned char b24; unsigned char b25; char pad26[6]; unsigned char b20x; } Dummy;
typedef struct { int x, y, z; } V3;
typedef struct Obj { char pad0[12]; V3 p; unsigned char k; char p19; short ang; float spd; float r; unsigned char c24; unsigned char c25; } Obj;
typedef struct Src { char pad0[12]; V3 p; unsigned char k; char p19[3]; float lim; unsigned char dead; unsigned char done; } Src;
extern float func_8009D8A0(float);
extern unsigned int func_8009D914(void);
extern Obj *func_800A18D0(int, int);
void func_800F7A80(Src *s, int *out) {
    float r;
    unsigned char k;
    Obj *o;
    if (s->done) { *out = 1; return; }
    if (s->dead) return;
    r = 1.0f;
    if (!(func_8009D8A0(r) < s->lim)) return;
    k = s->k;
    o = func_800A18D0(58, 40);
    if (o == 0) return;
    o->p = s->p;
    o->ang = func_8009D914() % 0xFFFF;
    o->spd = (func_8009D8A0(0.2f) + 0.9f) * 1e+01f;
    o->r = r;
    o->c24 = func_8009D914() & 0x1f;
    o->c25 = func_8009D914() & 1;
    o->k = k;
}
