typedef unsigned char u8;

typedef struct {
    char pad0[0xC];
    int score;
} TeamInfo;

typedef struct {
    char pad0[0x10];
    TeamInfo *info;
    char pad14[0x19C];
    int kills;
    char pad1B4[0x9C];
} Unit;

typedef struct {
    int type;
    char pad4[8];
    u8 done_b;
    char padD[0x57];
    u8 done_a;
} LevelDef;

typedef struct {
    unsigned int level;
    int total;
    int kills[4];
} Progress;

typedef struct {
    void *world;
    int track;
    unsigned int mode;
    u8 human_count;
    u8 player_count;
} GameState;

extern Progress D_803A8310;
extern Unit D_80235F00[];
extern GameState D_80219498;
extern u8 D_80125AB2;
extern int D_80117EB4;
extern int D_802195C8;
extern int D_802195D4;
extern int D_802195D8;

LevelDef *func_800E9424(void);
int func_800A977C(Unit *unit);
void func_80097B20(int fade);

static inline Unit *get_unit(int index) {
    if (index == 127) {
        return 0;
    }
    return &D_80235F00[index];
}

void func_8009C31C(void) {
    Unit *unit;
    int index;
    u8 done;

    if (func_800E9424()->type != 1) {
        D_803A8310.total = D_80235F00[0].info->score;
        for (index = 0; index < D_80219498.human_count; index++) {
            unit = get_unit(index);
            D_803A8310.total += func_800A977C(unit);
            D_803A8310.kills[index] = unit->kills;
        }
    }
    D_803A8310.level++;
    if (D_80125AB2 == 0 && D_803A8310.level < 34) {
        while (1) {
            if (func_800E9424()->type == 0) {
                done = func_800E9424()->done_a;
            } else if (func_800E9424()->type == 1) {
                done = func_800E9424()->done_b;
            } else {
                continue;
            }
            if (!done) {
                break;
            }
            D_803A8310.level++;
        }
    }
    if (D_803A8310.level == 34) {
        func_80097B20(10);
        D_802195D8 = 0;
        D_802195C8 = 8;
    } else if (func_800E9424()->type == 0) {
        D_80117EB4 = 1;
        D_802195D4 = -1;
        D_802195C8 = 7;
    } else if (func_800E9424()->type == 1) {
        D_802195D4 = -1;
        D_80117EB4 = 1;
        D_802195C8 = 3;
    }
}
