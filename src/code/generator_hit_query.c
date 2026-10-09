#include "types.h"

typedef struct {
    u8 pad00[0x0C];
    f32 x;
    f32 y;
    u8 pad14[6];
    u8 kind;
    u8 pad1B;
    s16 active;
} GeneratorHitObject;

typedef struct {
    u8 kind;
    u8 pad01[3];
    void *owner;
} GeneratorHitMessage;

typedef struct {
    u8 kind;
    u8 pad01[3];
    f32 x;
    f32 y;
} GeneratorHitResult;

void func_800F6100(GeneratorHitObject *object, void *unused,
                   GeneratorHitMessage *message, GeneratorHitResult *result) {
    if (message->owner == 0 && object->active != 0 &&
        message->kind == object->kind) {
        result->kind = 2;
        result->x = object->x;
        result->y = object->y;
    }
}
