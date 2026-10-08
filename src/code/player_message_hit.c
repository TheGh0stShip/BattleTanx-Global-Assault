typedef struct { char pad[0x1D]; unsigned char b29; } Obj;
typedef struct { int p0; int type; } Msg;
typedef struct { char pad[0xC]; unsigned short h12; } Arg;
extern void func_800E16D8(Obj *, int);
void func_800E18D8(Obj *o, Msg *m, Arg *a, int *res) {
    switch (m->type) {
    case 11: case 37: case 38: case 50:
        if (o->b29 == 0) { func_800E16D8(o, a->h12); *res = 1; }
        break;
    case 4:
        if (o->b29 != 0) { *res = 9; }
        else { *res = 10; func_800E16D8(o, a->h12); }
        break;
    case 28:
        if (o->b29 == 0) *res = 1;
        break;
    case 66:
        *res = 1;
        break;
    }
}
