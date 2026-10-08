typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 pos;
    unsigned char b24;
    char pad25[3];
    int i28;
    int state;
    float f36;
    int time;
    int i44;
    unsigned short u48;
} Obj;
typedef struct { char pad[152]; int i152; } Ctx;
typedef struct { int i0; int type; char pad8[4]; Ctx *ctx; } Msg;
typedef struct { int i0; int i4; int i8; int i12; Vec3 pos; } Out;
extern int D_8021945C;
extern char D_8011551C[];
extern void func_800B22F8(int);
extern float func_8009D8A0(float);
extern void func_800A5BD8(Vec3 *, int, int, float, void *, int);
extern void func_80097FB4(int, float, float, float, int);
void func_800F2288(Obj *o, Msg *m, int a2, Out *out) {
    switch (m->type) {
    case 4:
        if (o->state == 0) break;
        if (o->i28 == 0 && m->ctx->i152 == 4) break;
        if (o->state == 1) {
            switch (o->i28) {
            case 0:
                func_800B22F8(o->u48);
                o->u48 = 0xFFFF;
                o->state = 2;
                o->time = D_8021945C;
                func_800A5BD8(&o->pos, 0, o->b24, 1.0f, D_8011551C, 0);
                break;
            case 1:
                func_800B22F8(o->u48);
                o->u48 = 0xFFFF;
                o->state = 3;
                o->f36 = (func_8009D8A0(0.2f) + 0.9f) * 12.0f;
                func_80097FB4(52, o->pos.x, o->pos.y, 1.0f, o->b24);
                break;
            }
        }
        if (o->i28 == 0) {
            out->i0 = 13;
            out->i8 = 20;
            out->i12 = o->i44;
            out->pos = o->pos;
        }
        break;
    case 28:
        if (o->state == 0) break;
        if (o->state == 1) {
            switch (o->i28) {
            case 0:
                func_800B22F8(o->u48);
                o->u48 = 0xFFFF;
                o->state = 2;
                o->time = D_8021945C;
                func_800A5BD8(&o->pos, 0, o->b24, 1.0f, D_8011551C, 0);
                break;
            case 1:
                func_800B22F8(o->u48);
                o->u48 = 0xFFFF;
                o->state = 3;
                o->f36 = (func_8009D8A0(0.2f) + 0.9f) * 12.0f;
                func_80097FB4(52, o->pos.x, o->pos.y, 1.0f, o->b24);
                break;
            }
        }
        break;
    }
}
