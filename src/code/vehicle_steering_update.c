/* SPAN 0x80087C30 */
/* RODATA_VRAM 0x8007160C */
typedef struct { int rate; char p4[0x42 - 4]; unsigned short turn; char p44[0xD0 - 0x44]; } VDef;
typedef struct { float dist; float speed; unsigned short ang; } Tgt;
typedef struct {
    char pad[0x20]; unsigned short ang; char p22[6]; float vel[3]; char p34[0x48 - 0x34]; short h48;
    char p4A[0x98 - 0x4A]; int kind; char p9C[0x14C - 0x9C]; int t14C;
} Obj;
extern VDef D_80122E58[];
extern float D_80219488;
extern int D_8021945C;
extern int func_8009D75C(unsigned short *, unsigned short, unsigned short);
extern unsigned short func_800B6934(float *);
extern int func_800909CC(Obj *, float, unsigned short, unsigned short);
extern short func_8009DFAC(float *);

void func_80087A80(Obj *o, Tgt *t) {
    float spd;
    float rate;
    unsigned short old;
    unsigned short ang;
    unsigned short step;

    rate = D_80122E58[o->kind].turn * D_80219488;
    old = o->ang;
    step = (unsigned int)rate;
    if ((unsigned short)func_8009D75C(&o->ang, t->ang, step) > 182 && t->dist < 100.0f) {
        spd = 0.0f;
    } else {
        spd = t->speed;
    }
    ang = o->ang + func_800B6934(o->vel);
    o->ang = ang;
    if (D_8021945C < o->t14C) {
        if (D_8021945C + 4 < o->t14C) {
            func_800909CC(o, -0.3f, old, old);
        } else {
            func_800909CC(o, spd * 0.6f, ang, old);
        }
    } else if (func_800909CC(o, spd, ang, old) & 0x10) {
        o->t14C = D_8021945C + 8;
    }
    o->h48 = func_8009DFAC(o->vel);
}
