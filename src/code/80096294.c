/* SPAN 0x8009660C */
/* RODATA_VRAM 0x80072280 */
typedef unsigned char u8;

typedef struct {
    int unk0;
    int model;          /* 0x04 */
} ZoneObj;

typedef struct {
    int n0;             /* 0x00 */
    ZoneObj *list0;     /* 0x04 */
    int n1;             /* 0x08 */
    ZoneObj *list1;     /* 0x0C */
    char p10[0x54 - 0x10];
    int models[7];      /* 0x54 */
    char p70[0xD0 - 0x70];
} Zone;

extern Zone D_80122E28[];
extern void *D_803A53A0[];
extern int D_803A57BC;

float func_8009D8A0(float max);
unsigned int func_8009D914(void);

int func_80096294(u8 chance, u8 base, int hp, int maxhp, int *out) {
    float f;
    unsigned int r;

    if (hp < maxhp * 0.9f) {
        f = 0.0f;
    } else {
        f = (hp - maxhp * 0.9f) / maxhp * 0.1f;
        chance = chance + (100 - chance) * f;
    }
    r = func_8009D914();
    if ((u8)(r % 100) < chance) {
        *out = (base * 30) * (func_8009D8A0(0.2f) + 0.9f) * (1.0f - f / 2.0f);
        return 0;
    }
    return 1;
}

void func_80096424(int z) {
    int i;

    D_803A53A0[D_80122E28[z].models[0]] = 0;
    D_803A53A0[D_80122E28[z].models[1]] = 0;
    D_803A53A0[D_80122E28[z].models[2]] = 0;
    D_803A53A0[D_80122E28[z].models[3]] = 0;
    D_803A53A0[D_80122E28[z].models[4]] = 0;
    D_803A53A0[D_80122E28[z].models[5]] = 0;
    D_803A53A0[D_80122E28[z].models[6]] = 0;
    for (i = 0; i < D_80122E28[z].n0; i++) {
        D_803A53A0[D_80122E28[z].list0[i].model] = 0;
    }
    for (i = 0; i < D_80122E28[z].n1; i++) {
        D_803A53A0[D_80122E28[z].list1[i].model] = 0;
    }
    D_803A57BC = -1;
}
