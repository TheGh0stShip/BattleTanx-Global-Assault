typedef struct { int pad[13]; int hp; } Obj;
typedef struct { int pad; int type; } Msg;
typedef struct { int pad[4]; int dmg; } Hit;
void func_800EFB90(Obj *obj, Msg *msg, int skip, Hit *hit, int *out) {
    int v;
    if (skip == 0) {
        switch (msg->type) {
        case 4: *out = 9; break;
        case 11: case 37: case 38: case 50:
            if (obj->hp > 0) {
                v = obj->hp - hit->dmg;
                obj->hp = v;
                if (v < 0) obj->hp = 0;
            }
            *out = 1; break;
        case 28: case 66: *out = 1; break;
        }
    }
}
