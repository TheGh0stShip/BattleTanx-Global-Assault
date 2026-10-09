#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} DebrisPosition;

typedef struct {
    u8 pad00[12];
    DebrisPosition position;
    u8 variant;
    u8 pad19[3];
    s32 spawn_time;
    s8 velocity[4][2];
} DebrisParticle;

extern s32 D_8021945C;
extern void *func_800A18D0(s32, s32);
extern u32 func_8009D914(void);

void func_800EB838(DebrisPosition *position, u8 variant) {
    DebrisParticle *particle = func_800A18D0(47, 40);
    s32 i = 0;

    if (particle != 0) {
        particle->position = *position;
        particle->variant = variant;
        particle->spawn_time = D_8021945C;
        for (; i < 4; i++) {
            particle->velocity[i][0] = func_8009D914() % 100 - 50;
            particle->velocity[i][1] = func_8009D914() % 20 - 10;
        }
    }
}
