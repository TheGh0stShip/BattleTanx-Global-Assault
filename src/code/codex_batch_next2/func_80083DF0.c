#include "types.h"

typedef struct Object80083DF0 Object80083DF0;

typedef struct {
    u8 pad_00[0x18];
    s32 kind;
    u8 pad_1C[0x4C];
    Object80083DF0 *owner;
} Entity80083DF0;

struct Object80083DF0 {
    u8 pad_000[0xA0];
    s32 active;
    u8 pad_0A4[0x21];
    u8 effect_state;
    u8 pad_0C6[2];
    Object80083DF0 *effect_owner;
    u8 pad_0CC[0xF4];
    s32 effect_value;
    u8 pad_1C4[0xC];
    Entity80083DF0 *entity;
};

typedef struct {
    u8 pad_00[0x10];
    s32 state;
} SearchRecord80083DF0;

extern SearchRecord80083DF0 *func_800A1A28(SearchRecord80083DF0 *previous, s32 kind);
extern void func_80088ABC(Object80083DF0 *object, Object80083DF0 *owner,
                          s32 value, u8 kind);
extern void func_80086208(Object80083DF0 *object, s32 state);

void func_80083DF0(Object80083DF0 *object) {
    register Object80083DF0 *saved_object __asm__("$16");
    Object80083DF0 *owner;
    register Entity80083DF0 *entity __asm__("$4");
    SearchRecord80083DF0 *record;

    saved_object = object;
    owner = 0;
    if (saved_object->active != 0) {
        func_80086208(saved_object, 0x13);
        return;
    }
    if (saved_object->effect_state == 1) {
        owner = saved_object->effect_owner;
    }
    if (owner == 0) {
        entity = saved_object->entity;
        if (entity->kind == 3) {
            owner = entity->owner;
            func_80088ABC(saved_object, owner, saved_object->effect_value, 1);
        }
    }
    if (owner == 0) {
        func_80086208(saved_object, 6);
        return;
    }
    if (owner->active != 0) {
        return;
    }
    record = func_800A1A28(0, 7);
    if (record == 0) {
        func_80086208(saved_object, 6);
    } else if (record->state == 1) {
        func_80086208(saved_object, 0x11);
    } else {
        func_80086208(saved_object, 6);
    }
}
