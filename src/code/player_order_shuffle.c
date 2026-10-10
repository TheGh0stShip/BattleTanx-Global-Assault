typedef unsigned char u8;

typedef struct {
    int unused0;
    int track;
    unsigned int mode;
    u8 ai_count;
    u8 teams[5];
    char pad12[0x3E];
    int time;
    char pad54[0x18];
    int music;
} LevelDef;

typedef struct {
    void *world;
    int track;
    unsigned int mode;
    u8 human_count;
    u8 player_count;
    u8 tank_count;
    u8 slot_count;
    u8 teams[5];
    char pad15[3];
    int time;
    char listeners[0xCE];
    u8 inverse_order[5];
    u8 order[5];
    LevelDef *level;
    char padF0[0x10];
    int slot_values[5];
    int field_11C;
    u8 retries;
    u8 retry;
} GameState;

extern GameState D_80219498;
int func_8009D914(void);

#define SHUFFLE_ORDER()                                                     \
    {                                                                       \
        int random_index;                                                   \
        u8 temporary;                                                       \
        u8 *entry;                                                          \
                                                                            \
        for (i = 0, entry = D_80219498.order; i < 4; i++, entry++) {        \
            random_index = func_8009D914() & 3;                             \
            if (i != random_index) {                                        \
                temporary = *entry;                                         \
                *entry = D_80219498.order[random_index];                    \
                D_80219498.order[random_index] = temporary;                 \
            }                                                               \
        }                                                                   \
        for (i = 0; i < 4; i++) {                                           \
            D_80219498.inverse_order[D_80219498.order[i]] = i;              \
        }                                                                   \
    }

void func_8009A4C8(void) {
    int i;

    for (i = 0; i < 5; i++) {
        D_80219498.inverse_order[i] = i;
        D_80219498.order[i] = i;
    }
    if (D_80219498.mode != 4) {
        SHUFFLE_ORDER();
    } else {
        do {
            SHUFFLE_ORDER();
        } while (D_80219498.inverse_order[0] >= D_80219498.tank_count ||
                 D_80219498.teams[D_80219498.inverse_order[0]] != 0);
    }
}
