typedef struct { char pad[52]; int hp; } Obj;
typedef struct { int pad; int kind; } Msg;
typedef struct { char pad[16]; int dmg; } Arg;
void func_800EFB30(Obj *o, Msg *m, Arg *p, int *out) {
    switch (m->kind) {
    case 4:
        *out = 9;
        break;
    case 11: case 37: case 38: case 50:
        if (o->hp > 0) {
            o->hp = o->hp - p->dmg;
            if (o->hp < 0) o->hp = 0;
            *out = 1;
            break;
        }
        *out = 1;
        break;
    case 28: case 66:
        *out = 1;
        break;
    }
}
