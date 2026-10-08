typedef struct { char pad[0x1C]; unsigned char b28; unsigned char b29; char p[2]; float x; float z; } Obj;
typedef struct { int p0; int type; } Msg;
typedef struct { unsigned char b0; char p[3]; int w4; char pad[4]; unsigned short h12; } Arg;
typedef struct { unsigned char f; char p[3]; float x; float z; } Out;
extern void func_800E16D8(Obj *, int);
extern unsigned short func_8009E9C8(Arg *, float *);
void func_800E1A4C(Obj *p, Msg *m, unsigned int mode, Arg *a, void *out) {
    Obj *o = p;
    switch (mode) {
    case 0:
        switch (m->type) {
        case 11: case 37: case 38: case 50:
            if (o->b29 == 0) { func_800E16D8(o, a->h12); *(int *)out = 1; }
            break;
        case 4:
            if (o->b29 != 0) { *(int *)out = 9; break; }
            *(int *)out = 10; func_800E16D8(o, a->h12);
            break;
        case 28:
            if (o->b29 == 0) *(int *)out = 1;
            break;
        case 66:
            *(int *)out = 1;
            break;
        }
        break;
    case 4:
        if (a->w4 == 0 && p->b29 == 0 && p->b28 == a->b0) {
            ((Out *)out)->f = 1;
            ((Out *)out)->x = p->x;
            ((Out *)out)->z = p->z;
        }
        break;
    case 3: case 5:
        if (p->b29 == 0) func_800E16D8(p, func_8009E9C8(a, &p->x));
        break;
    }
}
