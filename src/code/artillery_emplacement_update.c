/* ---- 0x800F6800/b/src/f83c0.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad[48]; float f30; float f34; char p38[148 - 56]; unsigned char b94; char p95[336 - 149];
    float f150; float f154;
} Tank;
typedef struct { char pad[440]; Tank *tank; char rest[0x250 - 444]; } Player;
typedef struct {
    char pad[12]; Vec3 pos; unsigned char b18; unsigned char b19; char p1a[2]; int w1c;
} Obj;
extern int D_8021945C;
extern unsigned char D_802194A4;
extern Player D_80235F00[];
extern int D_801155EC;
extern unsigned int func_8009D914(void);
extern float func_8009D8A0(float);
extern int func_80096250(Tank *);
extern void func_8009EC0C(Vec3 *);
extern int func_8009E9C8(Vec3 *, Vec3 *);
extern void func_800DC968(Vec3 *, int, Vec3 *, unsigned short, int, int *, int, int, int, int);
void func_800F83C0(Obj *o, int *done) {
    int i;
    Tank *t;
    float range, lo;
    Vec3 tgt, d;
    if (o->b19) {
        *done = 1;
        return;
    }
    if (D_8021945C > o->w1c) {
        i = (D_802194A4 == 1) ? 0 : (func_8009D914() & 1);
        {
            Player *p;
            if (i == 127) p = 0;
            else p = &D_80235F00[i];
            t = p->tank;
        }
        if (t != 0 && func_80096250(t) && t->b94 == o->b18) {
            range = t->f30 / t->f34 * 2e+02f + 1e+02f;
            lo = -range;
            range = range - lo;
            tgt.x = t->f150 + (lo + func_8009D8A0(range));
            tgt.z = -1.0f;
            tgt.y = t->f154 + (lo + func_8009D8A0(range));
            d.x = tgt.x - o->pos.x;
            d.z = tgt.z - o->pos.z;
            d.y = tgt.y - o->pos.y;
            func_8009EC0C(&d);
            func_800DC968(&o->pos, o->b18, &d, func_8009E9C8(&o->pos, &tgt), 2, &D_801155EC, 30, 0, 0, 7);
            o->w1c = D_8021945C + 40 + func_8009D914() % 40;
        }
    }
}

