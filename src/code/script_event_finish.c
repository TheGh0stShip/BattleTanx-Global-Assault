typedef struct { short x, y; } Pt;
extern Pt D_803A6F88[];
typedef struct {
    char pad0[0xC];
    float c;
    float d;
    float e;
    char pad18[0x21 - 0x18];
    unsigned char g;
} Obj;
typedef struct {
    int active;
    Obj *obj;
    int pad8;
    unsigned short type;
    unsigned char pade;
    unsigned char idx;
} Ev;
extern void func_8008B82C(Obj *, int, int);
extern void func_800E4530(Obj *, int, int);

void func_800D53D4(Ev *e)
{
    Obj *o;
    unsigned char i;
    if (e->active == 0 || e->obj == 0)
        return;
    switch (e->type) {
    case 0: case 53:
        break;
    case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
    case 18: case 19: case 20: case 21: case 22: case 23: case 24:
        func_8008B82C(e->obj, 0, 255);
        e->active = 0;
        return;
    case 30: case 31:
        func_800E4530(e->obj, 10000, 0);
        e->active = 0;
        return;
    case 40:
        *(unsigned short *)((char *)e->obj + 0xE) &= 0x7FFF;
        break;
    case 41:
        e->obj->g = 1;
        break;
    case 33:
        i = e->idx;
        o = e->obj;
        o->c = D_803A6F88[i].x;
        o->d = D_803A6F88[i].y;
        break;
    case 34:
        i = e->idx;
        o = e->obj;
        o->d = D_803A6F88[i].x;
        o->e = D_803A6F88[i].y;
        break;
    }
    e->active = 0;
}
