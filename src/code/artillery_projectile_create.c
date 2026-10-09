#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct {
    u8 pad00[0x250];
} Player;

typedef struct {
    u8 pad00[0x0C];
    Vec3f position;
    f32 velocity_x;
    f32 velocity_z;
    f32 velocity_y;
    u8 kind;
    u8 pad25[3];
    Player *owner;
    s32 spawn_time;
    void *target;
    s32 target_id;
} ArtilleryProjectile;

extern Player D_80235F00[];
extern s32 D_8021945C;
extern ArtilleryProjectile *func_800A18D0(s32 kind, s32 size);

static inline Player *get_player(s32 index) {
    Player *player;

    if (index == 127) {
        player = 0;
    } else {
        player = &D_80235F00[index];
    }
    return player;
}

void func_800F8660(Vec3f *position, u8 kind, u8 owner) {
    ArtilleryProjectile *projectile;

    projectile = func_800A18D0(44, sizeof(ArtilleryProjectile));
    if (projectile != 0) {
        projectile->position = *position;
        projectile->velocity_x = 0.0f;
        projectile->velocity_y = 0.0f;
        projectile->velocity_z = 0.0f;
        projectile->target = 0;
        projectile->kind = kind;
        projectile->target_id = 0;
        projectile->owner = get_player(owner);
        projectile->spawn_time = D_8021945C;
    }
}
