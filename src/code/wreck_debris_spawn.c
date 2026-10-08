/* ---- 0x800F6800/b/src/f6ed0.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { int *p; } Owner;
typedef struct {
    char pad[12]; Vec3 pos; unsigned char b18; char p19[3];
    Owner *owner; int w20; short r24, r26, r28, s2a, s2c, s2e; int w30; float f34;
} Obj;
extern int D_8021945C;
extern Obj *func_800A18D0(int, int);
extern unsigned int func_8009D914(void);
void func_800F6ED0(Owner *ow, float f, Vec3 *v, unsigned char b) {
    Obj *o = func_800A18D0(60, 56);
    if (o != 0) {
        if (ow != 0) {
            o->owner = ow;
            o->w20 = *ow->p;
        } else {
            o->owner = 0;
            o->pos = *v;
            o->b18 = b;
        }
        o->r24 = func_8009D914();
        o->r26 = func_8009D914();
        o->r28 = func_8009D914();
        o->s2a = func_8009D914() % 660 + 364;
        if (func_8009D914() & 1) o->s2a = -o->s2a;
        o->s2c = func_8009D914() % 660 + 364;
        if (func_8009D914() & 1) o->s2c = -o->s2c;
        o->s2e = func_8009D914() % 660 + 364;
        if (func_8009D914() & 1) o->s2e = -o->s2e;
        o->w30 = D_8021945C;
        o->f34 = f;
    }
}

