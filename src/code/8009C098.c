/* SPAN 0x8009C284 */
/* RODATA_VRAM 0x8007250C */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 score;          /* 0x00 */
    char p2[0x18 - 2];
    u8 team;            /* 0x18 */
} TeamInfo;

typedef struct {
    char p0[0x10];
    TeamInfo *info;     /* 0x10 */
    char p14[0x74 - 0x14];
    int state;          /* 0x74 */
    char p78[0x250 - 0x78];
} Unit;

typedef struct {
    void *world;        /* 0x80219498 */
    int track;          /* 0x8021949C */
    unsigned int mode;  /* 0x802194A0 */
    u8 nHuman;          /* 0x802194A4 */
    u8 nPlayers;        /* 0x802194A5 */
    u8 nTanks;          /* 0x802194A6 */
    u8 nSlots;          /* 0x802194A7 */
    u8 teams[5];        /* 0x802194A8 */
    char pad[3];
    int time;           /* 0x802194B0 */
} GameState;

extern GameState D_80219498;
extern Unit D_80235F00[];

void func_800A96B8(Unit *u);

static inline Unit *getUnit(int i) {
    if (i == 127) {
        return 0;
    }
    return &D_80235F00[i];
}

void func_8009C098(void) {
    Unit *u;
    int i;

    switch (D_80219498.mode) {
    case 0:
    case 6:
        for (i = 0; i < D_80219498.nTanks; i++) {
            u = getUnit(i);
            if (u->info->score >= D_80219498.time) {
                u->state = 1;
            } else {
                u->state = 2;
            }
        }
        break;
    default: {
        u8 alive[5] = { 0, 0, 0, 0, 0 };

        for (i = 0; i < D_80219498.nTanks; i++) {
            u = getUnit(i);
            func_800A96B8(u);
            if (u->state != 2) {
                alive[u->info->team] = 1;
            }
        }
        for (i = 0; i < D_80219498.nTanks; i++) {
            u = getUnit(i);
            if (alive[u->info->team] != 0) {
                u->state = 1;
            } else {
                u->state = 2;
            }
        }
        break;
    }
    }
}
