#include "types.h"

typedef struct {
    u8 pad00[72];
    s32 value;
    u8 pad4C;
    u8 enabled;
    u8 destroyed;
} DestructibleModel;

typedef struct {
    s32 flags;
    u8 pad04[28];
    s16 state;
    u8 pad22[6];
} DestructibleEntry;

typedef struct {
    u8 pad00[12];
    DestructibleModel *model;
    s32 handle;
    s32 value;
    f32 position[4];
    u16 entry_id;
    u16 effect;
    u8 variant;
    u8 destroyed;
    u8 linked;
} DestructibleProp;

extern DestructibleEntry D_803978E0[];
extern void func_800DA4F0(s32, f32 *, s32, s32, s32, s32);
extern void func_800DA7D0(s32, f32 *, s32, s32, s32, s32, s32, s32, s32);
extern void func_800B22F8(s32);
extern void func_800A1BE0(DestructibleProp *);

void func_800ED4F4(DestructibleProp *prop, u16 effect_id, u8 use_effect) {
    if (prop->destroyed == 0) {
        prop->destroyed = 1;
        prop->model->destroyed = 1;
        if (prop->value != 0) {
            prop->model->value = prop->value;
        } else {
            prop->model->enabled = 0;
        }
        if (use_effect) {
            func_800DA4F0(prop->handle, prop->position, prop->effect,
                          prop->variant, effect_id, 0);
        } else {
            func_800DA7D0(prop->handle, prop->position, prop->effect,
                          prop->variant, 0, 0, 0, 0, 0);
        }
        if (prop->entry_id != 0xFFFF) {
            if (prop->linked) {
                DestructibleEntry *entry;
                s32 state = 2;
                s32 offset = prop->entry_id * sizeof(DestructibleEntry);

                entry = D_803978E0;
                ((DestructibleEntry *)((u8 *)entry + offset))->flags |= 0x200;
                entry[prop->entry_id].flags &= ~0x100002;
                entry[prop->entry_id].state = state;
            } else {
                func_800B22F8(prop->entry_id);
                func_800A1BE0(prop);
            }
        }
    }
}
