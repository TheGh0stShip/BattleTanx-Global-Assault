typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 pos;
    Vec3 vel;
    int state;
    int time;
    unsigned char b44;
} Obj4D80;
extern float D_80219488;
extern int D_8021945C;
extern char D_80115C38[];
extern float func_8009D8A0(float);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, Vec3 *);

void func_800F4D80(Obj4D80 *arg0, int *done) {
    Obj4D80 *o = arg0;
    Vec3 old;
    Vec3 nv;
    old = o->pos;
    switch (o->state) {
    case 0:
        o->pos.x += o->vel.x * D_80219488;
        o->pos.z += o->vel.z * D_80219488;
        o->pos.y += o->vel.y * D_80219488;
        o->vel.z += -0.1f;
        if (D_8021945C - o->time >= 5) {
            o->state = 1;
        }
        break;
    case 1:
        nv.x = o->vel.x * 0.85f + func_8009D8A0(0.5f) - 0.25f;
        if (o->vel.z > 0.0f) {
            nv.z = o->vel.z * 0.85f + -0.1f;
        } else {
            nv.z = o->vel.z + -0.1f;
        }
        if (nv.z < -4.0f) {
            nv.z = -4.0f;
        }
        nv.y = o->vel.y * 0.85f + func_8009D8A0(0.5f) - 0.25f;
        o->pos.x += (nv.x + o->vel.x) / 2.0f * D_80219488;
        o->pos.z += (nv.z + o->vel.z) / 2.0f * D_80219488;
        o->pos.y += (nv.y + o->vel.y) / 2.0f * D_80219488;
        if (o->pos.z < 0.0f) {
            *done = 1;
            return;
        }
        o->vel = nv;
        break;
    }
    func_800A5BD8(&o->pos, 0, o->b44, 1.0f, D_80115C38, &old);
}
