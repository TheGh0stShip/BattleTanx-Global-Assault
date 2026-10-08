typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad0[12];
    Vec3 pos;
    short s24;
    unsigned char b26;
    char pad27;
    int i28;
    int time;
    int r[4];
    int i52;
} Obj66;
extern int D_8021945C;
extern Obj66 *func_800A18D0(int, int);
extern unsigned int func_8009D914(void);
extern void func_80097FB4(int, float, float, float, int);
void func_800F6650(Vec3 *pos, short a1, unsigned char a2, int a3, int a4, int a5) {
    Obj66 *o = func_800A18D0(49, 56);
    if (o != 0) {
        o->r[0] = func_8009D914() % 30 + 15;
        o->r[1] = func_8009D914() % 30 + 15;
        o->r[2] = func_8009D914() % 30 + 15;
        o->r[3] = func_8009D914() % 30 + 15;
        o->s24 = a1;
        o->i28 = a3;
        o->pos = *pos;
        o->b26 = a2;
        o->i52 = a5;
        o->time = D_8021945C;
        func_80097FB4(43, pos->x, pos->y, 1.0f, a2);
    }
}
