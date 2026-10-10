/* SPAN 0x8009ACDC */
/* RODATA_VRAM 0x80072490 */
typedef unsigned char u8;

typedef struct {
    int unk0;
    int track;          /* 0x04 */
    unsigned int mode;  /* 0x08 */
    u8 nAI;             /* 0x0C */
    u8 teams[5];        /* 0x0D */
    char p12[0x50 - 0x12];
    int time;           /* 0x50 */
    char p54[0x6C - 0x54];
    int music;          /* 0x6C */
} LevelDef;

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
    char listeners[0xEA - 0x1C];
    u8 inv[5];          /* 0x80219582 */
    u8 order[5];        /* 0x80219587 */
    LevelDef *level;    /* 0x8021958C */
    char p590[0x108 - 0xF8];
    int slotVal[5];     /* 0x802195A0 */
    int f5B4;           /* 0x802195B4 */
    u8 retries;         /* 0x802195B8 */
    u8 retry;           /* 0x802195B9 */
} GameState;

extern GameState D_80219498;
extern u8 D_80117EB0;

LevelDef *func_800E9424(void);
void func_80097D14(int track, int fade);
void func_800A22CC(void);
void func_800A87BC(void);
void func_800AF43C(void);
void func_800BFCA4(void);
void func_800A80AC(void);
void func_800E7E24(void);
void func_800E7EE0(void);
void func_800DEDAC(void);

void func_8009A6F8(void) {
    LevelDef *lv;
    int i;
    int j;

    if (D_80219498.retry != 0) {
        if (D_80219498.retries < 100) {
            D_80219498.retries++;
        }
    } else {
        D_80219498.retries = 0;
    }
    D_80219498.retry = 0;
    lv = func_800E9424();
    D_80219498.level = lv;
    D_80219498.mode = lv->mode;
    D_80219498.time = lv->time;
    D_80219498.track = lv->track;
    D_80219498.nHuman = D_80117EB0;
    D_80219498.nPlayers = D_80117EB0;
    D_80219498.nTanks = D_80117EB0 + lv->nAI;
    D_80219498.nSlots = 4;
    if (D_80117EB0 == 1) {
        D_80219498.order[1] = 2;
        D_80219498.order[2] = 3;
        D_80219498.order[0] = 0;
        D_80219498.order[3] = 4;
        D_80219498.order[4] = 1;
    } else {
        D_80219498.order[1] = 1;
        D_80219498.order[2] = 2;
        D_80219498.order[0] = 0;
        D_80219498.order[3] = 3;
        D_80219498.order[4] = 4;
    }
    for (i = 0; i < 5; i++) {
        D_80219498.inv[D_80219498.order[i]] = i;
    }
    for (j = 0; j < 5; j++) {
        D_80219498.teams[j] = D_80219498.level->teams[D_80219498.order[j]];
    }
    if (D_80219498.mode == 12) {
        D_80219498.time = 0;
        D_80219498.f5B4 = 0;
    }
    func_80097D14(D_80219498.level->music, 10);
    func_800A22CC();
    func_800A87BC();
    func_800AF43C();
    func_800BFCA4();
    func_800A80AC();
    switch (D_80219498.mode) {
    case 11:
        func_800E7E24();
    case 7:
    case 13:
    case 14:
        func_800E7EE0();
        break;
    }
    func_800DEDAC();
}

extern int D_80117EB4;
extern int D_80117ED4[4];
extern unsigned int D_80117F04[4];
extern int D_80117F34[4];
extern int D_80117F44;

void func_8009A4C8(void);
void func_8009D168(void);
void func_800A7D70(void);
void func_800E713C(void);

void func_8009A95C(void) {
    int i;
    unsigned short n;
    u8 t;

    D_80219498.retries = 0;
    D_80219498.retry = 0;
    D_80219498.track = D_80117F44;
    switch (D_80117EB4) {
    case 6:
        D_80219498.mode = 6;
        D_80219498.time = 10;
        break;
    case 7:
        D_80219498.mode = 5;
        D_80219498.time = 5400;
        break;
    case 2:
        D_80219498.mode = 0;
        D_80219498.time = 10;
        break;
    case 4:
        D_80219498.mode = 2;
        D_80219498.time = 10;
        break;
    case 3:
        D_80219498.mode = 1;
        D_80219498.time = 0;
        break;
    case 5:
        D_80219498.mode = 3;
        break;
    case 8:
        D_80219498.mode = 4;
        break;
    }
    D_80219498.nHuman = D_80117EB0;
    D_80219498.nPlayers = D_80117EB0;
    D_80219498.nSlots = 4;
    D_80219498.nTanks = 0;
    D_80219498.teams[0] = 0;
    D_80219498.teams[1] = 0;
    D_80219498.teams[2] = 0;
    D_80219498.teams[3] = 0;
    if (D_80117EB4 == 6) {
        n = D_80117EB0;
        if (n == 1) {
            n++;
        }
        for (i = 0; i < n; i++) {
            t = 0;
            switch (i) {
            case 0:
                t = 2;
                break;
            case 2:
                t = 3;
                break;
            case 1:
                t = 1;
                break;
            }
            D_80219498.teams[D_80219498.nTanks] = t;
            D_80219498.slotVal[D_80219498.nTanks] = 0;
            D_80219498.nTanks++;
        }
    } else {
        for (i = 0; i < 4; i++) {
            t = 0;
            if (D_80117F04[i] < 3) {
                if (D_80117F04[i] == 0) {
                    continue;
                }
                switch (D_80117ED4[i]) {
                case 1:
                case 5:
                    t = 0;
                    break;
                case 2:
                case 6:
                    t = 1;
                    break;
                case 3:
                    t = 2;
                    break;
                case 4:
                    t = 3;
                    break;
                }
                D_80219498.teams[D_80219498.nTanks] = t;
                D_80219498.slotVal[D_80219498.nTanks] = D_80117F34[i];
                D_80219498.nTanks++;
            }
        }
    }
    func_8009A4C8();
    func_8009D168();
    func_800A22CC();
    func_800A87BC();
    func_800AF43C();
    func_800BFCA4();
    func_800A7D70();
    switch (D_80219498.mode) {
    case 1:
        func_800E7E24();
        func_800E7EE0();
        break;
    case 3:
        func_800E7EE0();
        D_80219498.time = 1200;
        break;
    case 2:
        func_800E713C();
        break;
    }
}
