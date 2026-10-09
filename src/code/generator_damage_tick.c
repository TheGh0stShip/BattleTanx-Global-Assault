#include "types.h"

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct {
    u8 pad00[0x1E4];
    u8 red;
    u8 green;
    u8 blue;
    u8 pad1E7[0x69];
} Player;

typedef struct {
    u8 pad00[0x0C];
    Vec3f position;
    u16 effect;
    u8 kind;
    u8 owner;
    s16 health;
    u8 pad1E[6];
    s32 handle;
} Generator;

typedef struct {
    u8 pad00[0x0C];
    s32 damage;
    s32 attacker;
} DamageMessage;

extern Player D_80235F00[];
extern u8 D_80115BC8[];
extern void func_800DA7D0(s32 handle, Vec3f *position, u16 effect, u8 kind,
                         s32 arg4, s32 arg5, u8 red, u8 green, u8 blue);
extern void func_800A5BD8(Vec3f *position, u16 effect, u8 kind, f32 scale,
                         void *asset, s32 arg5);
extern void func_800A9A98(Player *player, s32 amount);
extern void func_800A9B64(Player *player, s32 event);

static inline Player *get_player(s32 index) {
    Player *player;

    if (index == 127) {
        player = 0;
    } else {
        player = &D_80235F00[index];
    }
    return player;
}

void func_800F6344(Generator *generator, void *unused,
                   DamageMessage *message) {
    Player *attacker;
    Player *owner;
    Player *attacker_temp;
    s32 damage;

    attacker_temp = get_player(message->attacker);
    damage = message->damage;
    attacker = attacker_temp;
    if (generator->health > 0) {
        generator->health -= damage;
        if (generator->health <= 0) {
            owner = get_player(generator->owner);
            func_800DA7D0(generator->handle, &generator->position,
                          generator->effect, generator->kind, 0, 0,
                          owner->red, owner->green, owner->blue);
            func_800A5BD8(&generator->position, generator->effect,
                          generator->kind, 1.0f, D_80115BC8, 0);
            if (attacker != 0) {
                func_800A9A98(attacker, 1000);
            }
        }
    }
    func_800A9B64(get_player(generator->owner), 10);
}
