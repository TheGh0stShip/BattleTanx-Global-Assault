typedef struct { char pad[36]; int hp; char p2[5]; unsigned char b45; } Obj;
typedef struct { char pad[4]; int type; } Ent;
typedef struct { char pad[16]; int dmg; } Arg;
void func_800ED4F4(Obj *, int, int);
void func_800ED698(Obj *o, Ent *e, Arg *a, int *out) {
    switch (e->type) {
    case 11: case 37: case 38: case 50:
        if (o->b45 == 0) {
            *out = 1;
            o->hp -= a->dmg;
            if (o->hp < 0) func_800ED4F4(o, 0, 0);
        }
        break;
    case 4:
        if (o->b45 != 0) *out = 9;
        else *out = 1;
        break;
    case 28: case 66:
        *out = 1;
        break;
    case 12: case 21:
        func_800ED4F4(o, 0, 0);
        break;
    }
}
