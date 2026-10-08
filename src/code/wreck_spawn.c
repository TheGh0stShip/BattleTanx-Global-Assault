typedef struct { float x, y, z; } Vec3;
extern int D_80123BDC[][24];
extern short D_80123BDE[][48];
extern short D_80397650;
extern int func_800B02D4(float, float, int);
extern unsigned short func_800B205C(int, short, short, int, short, short, short, short, int, int, unsigned short, int, unsigned char);
extern unsigned short func_800B3748(unsigned short, int, void *, int, int);

int func_800E29A4(unsigned char id, Vec3 *pos, unsigned short arg2, unsigned char arg3) {
    char buf[1152];
    unsigned short h;
    if (func_800B02D4(pos->x, pos->y, arg3) != 0) {
    h = func_800B205C(0, (short)pos->x, (short)pos->y, 0,
                      -D_80123BDC[id][0], D_80123BDE[id][0],
                      -D_80123BDC[id][0], D_80123BDE[id][0],
                      0, 0, arg2, 0, arg3);
    D_80397650 = 0;
    return func_800B3748(h, 0xE4550F, buf, 0, 1) == 0;
    }
    return 0;
}
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
