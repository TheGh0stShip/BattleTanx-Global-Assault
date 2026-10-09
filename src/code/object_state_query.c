#include "types.h"

typedef struct {
    u8 pad00[0xC];
    f32 x;
    f32 y;
    u8 pad14[4];
    u8 kind;
    u8 pad19[3];
    s32 mode;
    s32 state;
    f32 timer;
    s32 timestamp;
    u8 pad2C[4];
    u16 effect_handle;
} ObjectState;

typedef struct {
    u8 kind;
    u8 pad01[3];
    s32 blocked;
} QueryMessage;

typedef struct {
    u8 matched;
    u8 pad01[3];
    f32 x;
    f32 y;
} QueryResult;

extern f32 D_80077028;
extern f32 D_8007702C;
extern f32 D_80077030;
extern u8 D_8011551C[];
extern s32 D_8021945C;

extern void func_800B22F8(u16 handle);
extern void func_800A5BD8(void *position, u16 effect, u8 kind, f32 scale,
                         void *asset, s32 arg5);
extern f32 func_8009D8A0(f32 value);
extern void func_80097FB4(s32 type, s32 x, s32 y, f32 scale, u8 kind);

void func_800F24F8(ObjectState *object, void *unused, QueryMessage *message,
                   QueryResult *result) {
    if (message->blocked == 0 && object->state == 1 &&
        object->kind == message->kind) {
        result->matched = 1;
        result->x = object->x;
        result->y = object->y;
    }
}

void func_800F253C(ObjectState *object) {
    if (object->state == 1) {
        object->state = 5;
    }
}

void func_800F2558(ObjectState *object) {
    if (object->state == 1) {
        switch (object->mode) {
            case 0:
                func_800B22F8(object->effect_handle);
                object->effect_handle = 0xFFFF;
                object->state = 2;
                object->timestamp = D_8021945C;
                func_800A5BD8(&object->x, 0, object->kind, 1.0f,
                              D_8011551C, 0);
                break;
            case 1:
                func_800B22F8(object->effect_handle);
                object->effect_handle = 0xFFFF;
                object->state = 3;
                object->timer =
                    (func_8009D8A0(D_80077028) + D_8007702C) * D_80077030;
                func_80097FB4(0x34, *(s32 *)&object->x, *(s32 *)&object->y,
                              1.0f, object->kind);
                break;
        }
    }
}
