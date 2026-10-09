#include "types.h"

typedef struct {
    u8 pad00[0xC];
    f32 x;
    f32 y;
    u8 pad14[4];
    u8 kind;
    u8 pad19[7];
    s32 state;
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
