/* SPAN 0x800A8E3C */
#include "types.h"

typedef struct State800A8D54 {
    u8 pad000[0x0B];
    u8 player;
    u8 pad00C[0x6C];
    u8 transform[0x94];
    void *entry;
    u8 active;
} State800A8D54;

typedef struct Entry800A8D54 {
    u8 pad00[4];
    s32 type;
    u8 pad08[4];
    void *object;
    u8 pad10[4];
    s32 selection;
} Entry800A8D54;

typedef struct TableRecord800A8D54 {
    void *value;
    u8 pad04[0xCC];
} TableRecord800A8D54;

extern TableRecord800A8D54 D_80122E38[];
extern void func_800CAED0(s32 player);
extern void func_800CB0C0(s32 player);
extern void func_800A6ABC(void *transform, Entry800A8D54 *entry);
extern void func_800A8B38(State800A8D54 *state);
extern void func_800A8F34(void *object, s32 *choices, void *scratch);
extern void func_800CAFA8(s32 player);
extern void func_800CAD9C(void *value, s32 player);

void func_800A8D54(State800A8D54 *state, Entry800A8D54 *entry) {
    s32 choices[16];
    u8 scratch[16];
    s32 type;

    type = 6;
    if (((Entry800A8D54 *)state->entry)->type == type) {
        func_800CAED0(state->player);
        func_800CB0C0(state->player);
    }

    state->entry = entry;
    if (state->active != 0) {
        func_800A6ABC(state->transform, entry);
        func_800A8B38(state);
        if (entry->type == type) {
            s32 choice;

            func_800A8F34(entry->object, choices, scratch);
            func_800CAFA8(((State800A8D54 *)entry->object)->player);
            choice = choices[entry->selection];
            func_800CAD9C(D_80122E38[choice].value,
                         ((State800A8D54 *)entry->object)->player);
        }
    }
}
