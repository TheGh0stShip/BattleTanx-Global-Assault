typedef struct { int a; int pad[2]; int c; } Ref;
typedef struct { int pad[3]; Ref *ref; int id; int pad2[4]; float t; } Obj;
extern float D_80219488;
void func_800F11D0(Obj *obj, int *out) {
    Ref *r = obj->ref;
    if ((r->a != obj->id) | (r->c == 0)) { *out = 1; return; }
    if (obj->t <= D_80219488) { *out = 1; return; }
    obj->t -= (unsigned short)(unsigned int)D_80219488;
}
