#include "types.h"

typedef struct {
    u8 pad_0[0xC];
    s32 value;
} ValueObject;

typedef struct {
    u8 pad_0[0xC];
    u8 flags;
    u8 pad_D[3];
    ValueObject *value_object;
    u8 pad_14[0x5C];
    s32 source;
} SelectionObject;

typedef struct {
    u8 pad_0[0x54];
    u32 enabled_mask;
} GlobalSelection;

typedef struct {
    s32 threshold;
    u8 pad_4[0xCC];
} SelectionEntry;

extern GlobalSelection *D_8021958C;
extern SelectionEntry D_80122EC8[];

extern s32 func_8009D144(SelectionObject *object);
extern s32 *func_800D69E0(s32 source);

s32 func_800A8F34(SelectionObject *object, s32 *indices, u8 *states) {
    register SelectionObject *saved_object __asm__("$17");
    register s32 *saved_indices __asm__("$18");
    register u8 *saved_states __asm__("$16");
    saved_object = object;
    saved_indices = indices;
    saved_states = states;
    if (func_8009D144(saved_object)) {
        register s32 count __asm__("$8");
        register s32 index __asm__("$4");
        register s32 one __asm__("$9");
        register s32 table_offset __asm__("$7");
        register s32 *index_out __asm__("$5");
        register u8 *state_out __asm__("$6");

        count = 0;
        index = 0;
        one = 1;
        table_offset = 0;
        state_out = saved_states;
        index_out = saved_indices;
        while (index < 15) {
            if (D_8021958C->enabled_mask & (one << index)) {
                *index_out = index;
                if (saved_object->value_object->value >=
                    *(s32 *)((u8 *)D_80122EC8 + table_offset)) {
                    *state_out = one;
                } else {
                    *state_out = 0;
                }
                state_out++;
                index_out++;
                count++;
            }
            index++;
            table_offset += 0xD0;
        }
        return count;
    } else {
        register s32 *source __asm__("$4");
        register s32 index __asm__("$3");
        register s32 one __asm__("$7");
        register s32 *index_out __asm__("$5");
        register u8 *state_out __asm__("$6");
        s32 *source_result;

        source_result = func_800D69E0(saved_object->source);
        index = 0;
        one = 1;
        state_out = saved_states;
        index_out = saved_indices;
        source = source_result;
        while (index < 5) {
            *index_out = *source;
            if (((s32)saved_object->flags >> index) & 1) {
                *state_out = one;
            } else {
                *state_out = 0;
            }
            state_out++;
            index_out++;
            source++;
            index++;
        }
        return 5;
    }
}
