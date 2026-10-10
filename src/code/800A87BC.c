/* SPAN 0x800A89B0 */
typedef unsigned char u8;
typedef short s16;
typedef int s32;

typedef struct {
    char p0[0x18];
    s32 model[5];       /* 0x18 */
} LevelDef;

typedef struct {
    void *world;        /* 0x80219498 */
    s32 track;          /* 0x8021949C */
    unsigned int mode;  /* 0x802194A0 */
    u8 nHuman;          /* 0x802194A4 */
    u8 nPlayers;        /* 0x802194A5 */
    u8 nTanks;          /* 0x802194A6 */
    u8 nSlots;          /* 0x802194A7 */
    u8 teams[5];        /* 0x802194A8 */
    char pad[3];
    s32 time;           /* 0x802194B0 */
    char listeners[0xEA - 0x1C];
    u8 inv[5];          /* 0x80219582 */
    u8 order[5];        /* 0x80219587 */
    LevelDef *level;    /* 0x8021958C */
    u8 colors[5][3];    /* 0x80219590 */
    char p59f[0x108 - 0x107];
    s32 slotVal[5];     /* 0x802195A0 */
} GameState;

typedef struct {
    char p0[0x14];
    u8 r, g, b;         /* 0x14 */
} ModelInfo;

typedef struct {
    s16 x, y, z;        /* 0x00 */
    char p6[2];
    s32 f8;             /* 0x08 */
    s32 fC;             /* 0x0C */
    char p10[8];
    u8 id;              /* 0x18 */
    u8 color[3];        /* 0x19 */
} PlayerColor;

extern GameState D_80219498;
extern PlayerColor D_80236A90[];
extern u8 D_80116580[][3];

void _bzero(void *p, s32 n);
s32 func_8009D144(void);
ModelInfo *func_800D69E0(s32 model);

void func_800A87BC(void) {
    s32 i;

    _bzero(D_80219498.colors, 15);
    if (func_8009D144()) {
        for (i = 0; i < D_80219498.nTanks; i++) {
            ModelInfo *info = func_800D69E0(D_80219498.level->model[D_80219498.order[i]]);

            D_80219498.colors[i][0] = info->r;
            D_80219498.colors[i][1] = info->g;
            D_80219498.colors[i][2] = info->b;
            D_80219498.slotVal[i] = D_80219498.level->model[D_80219498.order[i]];
        }
    } else {
        for (i = 0; i < 4; i++) {
            u8 r = D_80116580[i][0];
            u8 g = D_80116580[i][1];
            u8 b = D_80116580[i][2];
            PlayerColor *p = &D_80236A90[i];

            p->id = i;
            p->x = 0;
            p->y = 0;
            p->z = 0;
            p->f8 = 0;
            p->fC = 0;
            p->color[0] = r;
            p->color[1] = g;
            p->color[2] = b;
        }
        for (i = 0; i < D_80219498.nTanks; i++) {
            PlayerColor *src = &D_80236A90[D_80219498.teams[i]];

            D_80219498.colors[i][0] = src->color[0];
            D_80219498.colors[i][1] = src->color[1];
            D_80219498.colors[i][2] = src->color[2];
        }
    }
}

