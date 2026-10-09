#include "types.h"

typedef struct {
    u8 pad00[0xC5];
    u8 kind;
    u8 padC6[2];
    void *target;
} LookupState8865C;

typedef struct {
    u8 pad00[4];
    s32 type;
    u8 pad08[0x14];
    s16 count;
} LookupTarget8865C;

extern s32 func_80095B68(void *target);

void *func_8008865C(LookupState8865C *state) {
    void *result = 0;
    void *target;

    switch (state->kind) {
        case 2:
            result = &state->target;
            break;
        case 1:
            target = state->target;
            if (func_80095B68(target) &&
                ((*(u32 *)((u8 *)target + 0x1E0) & 0x80) == 0)) {
                result = (u8 *)target + 0x150;
            }
            break;
        case 3:
        {
            LookupTarget8865C *entry = state->target;
            if (entry != 0 && entry->type == 0x1C && entry->count > 0) {
                result = (u8 *)entry + 0xC;
            }
            break;
        }
    }
    return result;
}
