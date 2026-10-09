/* SPAN 0x80083DA8 */
#include "types.h"

typedef struct Record80083CC0 {
    s32 id;
    u8 pad04[0x20];
    u8 data[1];
} Record80083CC0;

typedef struct State80083CC0 {
    u8 pad000[0xA0];
    s32 override;
    u8 pad0A4[0xCC];
    s32 mode;
    Record80083CC0 *record;
    s32 record_id;
} State80083CC0;

extern void func_80086208(State80083CC0 *state, s32 reason);
extern Record80083CC0 *func_800A1A28(s32 unused, s32 kind);
extern void func_80088B6C(State80083CC0 *state, void *data,
                          s32 unused, s32 enabled);
extern void func_80088720(State80083CC0 *state);

void func_80083CC0(State80083CC0 *state) {
    Record80083CC0 *record;
    s32 *mode;

    if (state->override != 0) {
        func_80086208(state, 0x13);
        return;
    }

    record = func_800A1A28(0, 7);
    mode = &state->mode;
    switch (*mode) {
        case 1:
            if (record == 0) {
                state->record = 0;
                state->record_id = 0;
            } else {
                state->record = record;
                state->record_id = record->id;
                func_80088B6C(state, record->data, 0, 1);
                *mode = 2;
            }
            break;
        case 2:
            if (record == 0 || record != state->record ||
                record->id != state->record_id) {
                func_80088720(state);
                *mode = 1;
            }
            break;
    }
}
