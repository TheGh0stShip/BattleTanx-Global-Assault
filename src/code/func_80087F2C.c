/* SPAN 0x80088000 */
#include "types.h"

typedef struct Vec2_80087F2C {
    f32 x;
    f32 y;
} Vec2_80087F2C;

typedef struct Hit80087F2C {
    u8 pad00[8];
    f32 x;
    f32 y;
    u8 pad10[0x18];
} Hit80087F2C;

typedef struct State80087F2C {
    s32 owner;
    u8 pad004[4];
    Vec2_80087F2C position;
    u8 pad010[0x10];
    u16 angle;
    u8 pad022[0x0E];
    f32 distance;
    u8 pad034[0x60];
    u8 player;
    u8 pad095[0xBB];
    Vec2_80087F2C output;
} State80087F2C;

extern f32 D_80071664;
extern volatile s16 D_80397650;
extern void func_8009DAB0(Vec2_80087F2C *out, f32 distance, u16 angle);
extern void func_8009E044(Vec2_80087F2C *position, Vec2_80087F2C *offset);
extern u16 func_800B49E0(Vec2_80087F2C *start, Vec2_80087F2C *end,
                         s32 mask, s32 player, s32 limit, s32 owner,
                         Hit80087F2C *hit);

void func_80087F2C(State80087F2C *state) {
    Vec2_80087F2C offset;
    Vec2_80087F2C position;
    Hit80087F2C hit;
    register f32 scaled_distance asm("$f4");
    register f32 position_x asm("$f2");
    register f32 position_y asm("$f0");
    register s32 player asm("$7");
    register s32 owner asm("$3");
    register s32 mask asm("$6");

    scaled_distance = state->distance;
    position_x = state->position.x;
    scaled_distance *= D_80071664;
    position.x = position_x;
    position_y = state->position.y;
    position.y = position_y;
    func_8009DAB0(&offset, scaled_distance, state->angle);
    func_8009E044(&position, &offset);

    player = state->player;
    owner = state->owner;
    mask = 0x2C700F;
    D_80397650 = 0;
    if (func_800B49E0(&state->position, &position, mask,
                      player, 100, owner, &hit) != 0) {
        state->output.x = hit.x;
        state->output.y = hit.y;
    } else {
        state->output.x = position.x;
        state->output.y = position.y;
    }
}
