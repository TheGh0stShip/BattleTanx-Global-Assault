typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 pos;
    Vec3 vel;
    int z36;
    int time;
    unsigned char b44;
} Obj50;
extern int D_8021945C;
extern Obj50 *func_800A18D0(int, int);
extern unsigned int func_8009D914(void);
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
void func_800F50B8(Vec3 *pos, unsigned char b, int count, unsigned short lo, unsigned short hi, float scale) {
    Vec3 v;
    int i;
    unsigned short a;
    unsigned short c;
    Obj50 *o;
    for (i = 0; i < count; i++) {
        a = lo + func_8009D914() % (unsigned)(hi - lo);
        c = func_8009D914();
        v.x = func_8009D4B0(a) * func_8009D4B0(c) * scale;
        v.z = func_8009D510(a) * scale;
        v.y = func_8009D4B0(a) * func_8009D510(c) * scale;
        o = func_800A18D0(62, 48);
        if (o != 0) {
            o->pos = *pos;
            o->vel = v;
            o->z36 = 0;
            o->b44 = b;
            o->time = D_8021945C + func_8009D914() % 5;
        }
    }
}
