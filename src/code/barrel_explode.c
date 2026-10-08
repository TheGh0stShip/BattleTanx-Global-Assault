typedef struct { float m[4][4]; int pad; } Mtx;
typedef struct {
    char pad0[12];
    float x, y, s;
    unsigned char b24;
    char pad25;
    unsigned short u26;
    int i28;
    unsigned int n32;
    char pad36[8];
    unsigned short u44, u46;
    short s48;
} Obj;
extern unsigned char func_800AD14C(float, float, float, int);
extern void func_8009EF30(Mtx *, float *);
extern void func_8009F090(Mtx *, unsigned short, unsigned short, int);
extern int func_800AA598(int);
extern void func_800AE4D0(int, int, Mtx *, int, int, int, int);
void func_800F352C(Obj *o) {
    unsigned char r;
    if (o->n32 >= 2) {
    Mtx m = { { {1.0f, 0, 0, 0}, {0, 1.0f, 0, 0}, {0, 0, 1.0f, 0}, {0, 0, 0, 1.0f} }, 0 };
    r = func_800AD14C(o->x, o->y, o->s48, o->b24);
    if (r) {
        func_8009EF30(&m, &o->x);
        func_8009F090(&m, o->s * o->u44, o->u26 + o->s * o->u46, 0);
        func_800AE4D0(o->i28, func_800AA598(o->b24), &m, 0, 0, 0, r);
    }
    }
}
