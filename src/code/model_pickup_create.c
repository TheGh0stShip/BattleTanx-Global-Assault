typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[10];
    unsigned char a;
    char pad1;
    short h;
    char pad2[2];
    Vec3 pos;
    unsigned char b;
    char pad3;
    unsigned short c;
    char rest[1];
} Obj;
extern int D_802194A0;
extern Obj *func_800A18D0(int, int);
extern short func_800B1898(Obj *, short, short, int, int, int, int, int, int, int, int, int, int);
extern void func_800C98D8(void *);
void func_800E9B50(unsigned char *p, int unused, Vec3 *v, unsigned short s, unsigned char t) {
    Obj *o;
    switch (D_802194A0) {
    case 0: case 1: case 2: case 3: case 6: case 8: case 11: case 12: case 13:
        return;
    }
    o = func_800A18D0(19, 36);
    if (o == 0) return;
    o->h = func_800B1898(o, (short)v->x, (short)v->y, 0, -p[1], p[1], -p[2], p[2], 0, 100, s, 0x10000, t);
    o->a = p[3];
    o->c = s;
    o->pos = *v;
    o->b = t;
    if (D_802194A0 == 14 && o->a == 0) {
        func_800C98D8((char *)o + 0x20);
    }
}
