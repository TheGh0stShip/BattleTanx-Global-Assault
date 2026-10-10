/* RODATA_VRAM 0x8007121C */
#include "types.h"

typedef struct {
    char pad0[8];
    f32 x;
    f32 y;
    char pad10[0x20 - 0x10];
    u16 idleState;
    char pad22[0x30 - 0x22];
    f32 speed;
    char pad34[0x48 - 0x34];
    u16 movingState;
    char pad4A[0x13C - 0x4A];
    u16 state;
    char pad13E[2];
    f32 timer;
    f32 previousX;
    f32 previousY;
} MovingObject;

void func_80082004(MovingObject *object) {
    f32 *position = (f32 *)((char *)object + 0x134);

    if (object->speed <= 0.0f) {
        object->timer = 0.0f;
        object->state = object->idleState;
    } else {
        object->timer = object->speed * 600.0f / 60.0f + 100.0f;
        object->state = object->movingState;
    }
    position[4] = object->x;
    position[5] = object->y;
}
