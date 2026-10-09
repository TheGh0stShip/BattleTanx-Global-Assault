#include "types.h"

typedef struct {
    u16 value;
    u8 pad_2[0xCE];
} SelectionDefinition80085DA8;

typedef struct {
    u8 pad_000[0x98];
    s32 selection_index;
    u8 pad_09C[0xD4];
    s32 state;
    s32 timestamp;
    s16 timer;
    u8 pad_17A[0x66];
    u32 flags;
    u8 pad_1E4[0x10];
    u16 parameter;
} Object80085DA8;

extern s32 D_8021945C;
extern SelectionDefinition80085DA8 D_80122EE0[];

extern s32 func_8008F3FC(u16 parameter, s32 mask, s32 arg2, u16 value);
extern void func_80088720(Object80085DA8 *object);
extern void func_80088FD4(Object80085DA8 *object);

void func_80085DA8(Object80085DA8 *object, s32 delay, s32 force_state) {
    register s32 state __asm__("$2");

    object->timer = 0x1800;
    object->flags |= 0x100;
    object->timestamp = D_8021945C + delay;

    if ((force_state != 0) ||
        (func_8008F3FC(object->parameter, 0x24700F, 0,
                       D_80122EE0[object->selection_index].value) != 0)) {
        state = 3;
    } else {
        state = 1;
    }
    object->state = state;

    func_80088720(object);
    func_80088FD4(object);
}
