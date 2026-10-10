typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { float m[4][4]; } Mtx;
typedef struct { Mtx m; int flag; } Mtx17;

typedef struct {
    char p0[0xC0];
    Mtx m0;
    char p100[0x118 - 0x100];
    int f118;
    char p11c[4];
    u16 h120;
    char p122[2];
    Mtx m1;
    char p164[0x250 - 0x164];
} Unit;

extern int D_80117EB4;
extern u8 D_802194A5;
extern Unit D_80235F00[];

void func_8007B03C(void);
void func_800AC9E8(void);
void func_800B0268(void);
void func_800A1FDC(void);
void func_800D250C(Mtx17 *m, u16 *ang, int *vals);
void func_8009F4B4(Mtx *a, Mtx *b, Mtx17 *out);
int func_8007B65C(int kind, Mtx17 *m, int *vals, u16 *ang);
int func_8007B8EC(int kind, Mtx17 *m, int *vals, u16 *ang);

typedef struct {
    void *world;
    int track;
    unsigned int mode;
    u8 nHuman;
    u8 nPlayers;
} GameState;
extern GameState D_80219498;

void func_8009B0F0(void) {
    Mtx17 mats[5];
    u16 ang[5];
    int vals[5];
    Unit *u;
    Unit *v;
    int i;

    func_8007B03C();
    func_800AC9E8();
    func_800B0268();
    func_800A1FDC();
    if (D_80117EB4 == 10) {
        func_800D250C(mats, ang, vals);
        mats[0].flag = 0;
    } else {
        for (i = 0; i < D_80219498.nPlayers; i++) {
            if (i == 127) {
                u = 0;
            } else {
                u = &D_80235F00[i];
            }
            func_8009F4B4(&u->m0, &u->m1, &mats[i]);
            mats[i].flag = 0;
            ang[i] = u->h120;
            vals[i] = u->f118;
        }
    }
    if (func_8007B65C(4, mats, vals, ang) < 0) return;
    if (func_8007B65C(5, mats, vals, ang) < 0) return;
    if (func_8007B65C(0, mats, vals, ang) < 0) return;
    if (func_8007B65C(2, mats, vals, ang) < 0) return;
    if (func_8007B65C(1, mats, vals, ang) < 0) return;
    for (i = 0; i < D_80219498.nPlayers; i++) {
        if (i == 127) {
            v = 0;
        } else {
            v = &D_80235F00[i];
        }
        (&mats[i])->m = v->m1;
        mats[i].flag = 0;
    }
    if (func_8007B8EC(0, mats, vals, ang) < 0) return;
    func_8007B65C(3, mats, vals, ang);
}
