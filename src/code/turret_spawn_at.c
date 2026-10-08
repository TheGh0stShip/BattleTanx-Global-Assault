typedef struct { float x, y, z; } Vec3;
extern int D_80123BDC[][24];
extern short D_80123BDE[][48];
extern short D_80397650;
typedef struct {
    char pad0[0xA]; unsigned char bA; char pB; Vec3 pos; unsigned short h18; unsigned short h1A;
    unsigned char b1C; unsigned char b1D; unsigned char b1E; unsigned char b1F;
    unsigned char b20; unsigned char b21; unsigned char b22; char p23[0x28 - 0x23];
    unsigned short h28; char p2A[2]; int i2C; int i30; int i34; int i38; int i3C; int i40;
} Wreck;
extern float D_80123BD8[][24];
extern int D_80123BE0[][24];
extern int D_80123C04[][24];
extern float D_80075C64;
extern float D_80075C68;
extern int func_800E29A4(unsigned char, Vec3 *, unsigned short, unsigned char);
extern Wreck *func_800A18D0(int, int);
extern unsigned short func_800B1898(Wreck *, short, short, int, short, short, short, short, short, short, unsigned short, int, unsigned char);

Wreck *func_800E2AEC(unsigned char kind, Vec3 *pos, unsigned short a2, unsigned short a3,
                     unsigned char a4, unsigned char a5, int a6) {
    Wreck *o;

    if (a6 == 0) {
        if (!func_800E29A4(kind, pos, a2 + a3, a4)) return 0;
    }
    o = func_800A18D0(16, 68);
    if (o == 0) return 0;
    o->h18 = a2 + a3;
    o->b20 = kind;
    if (D_80123BE0[kind][0] != 0) {
        o->b1F = 1;
        o->h28 = 0xFFFF;
        o->b21 = 0;
    } else {
        o->b1F = 0;

        o->h28 = func_800B1898(o, pos->x, pos->y, 0, -D_80123BDC[kind][0], D_80123BDE[kind][0],
                               -D_80123BDC[kind][0], D_80123BDE[kind][0], pos->z - D_80075C64,
                               (pos->z + D_80123BD8[kind][0] > D_80075C68) ? (short)(pos->z + D_80123BD8[kind][0]) : 70, a3 + a2,
                               (kind == 3) ? 0x400000 : 4096, a4);
        o->b21 = 255;
    }
    o->b1E = 0;
    o->i30 = -1000;
    o->h1A = a2;
    o->i38 = 0;
    o->pos = *pos;
    o->i2C = -1000;
    o->i34 = -1000;
    o->b22 = a5;
    o->b1C = a4;
    o->b1D = 0;
    o->bA = 0;
    o->i3C = D_80123C04[kind][0];
    o->i40 = a6;
    return o;
}
typedef struct { int id; char p4[0x150 - 4]; float x; float y; } Target;
typedef struct { char pad[0x38]; Target *target; } Seeker;
typedef struct { int id; int pad[9]; } Hit;
