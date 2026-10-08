typedef struct { float x, y, z; } Vec3;
typedef struct { int w[17]; } M44;
typedef struct {
    char pad0[12];
    Vec3 pos;
    unsigned char b24;
    char pad25;
    unsigned short u26;
    int i28;
    char pad32[20];
    unsigned int time;
} Obj;
extern unsigned int D_8021945C;
extern M44 D_80076FC0;
extern int D_80125AA4[];
extern int D_803A53A0[];
extern unsigned char func_800AD14C(float, float, float, int);
extern int func_800AA058(int, Vec3 *);
extern void func_8009EFD4(M44 *, float, float, float, int);
extern void func_800AE4D0(int, int, M44 *, int, int, int, int);
void func_800F2050(Obj *o) {
    M44 m = D_80076FC0;
    int blink;
    int vis;
    int h;
    vis = func_800AD14C(o->pos.x, o->pos.y, 10.0f, o->b24);
    blink = 0;
    if (vis) {
        if (o->i28 == 0) {
            blink = ((D_8021945C + o->time) >> 3) & 1;
        }
        h = func_800AA058(o->b24, &o->pos);
        func_8009EFD4(&m, o->pos.x, o->pos.z, o->pos.y, o->u26);
        func_800AE4D0(D_803A53A0[D_80125AA4[o->i28]], h, &m, 0, 0, blink, vis);
    }
}
