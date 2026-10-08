#include "types.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Item4B84 {
    u8 pad[0x10];
    s32 id;
    u8 pad14[0x1C4];
    f32 armor;
    u8 pad1DC[8];
    u8 r, g, b;
    u8 pad1E7[0x69];
} Item4B84;
typedef struct Info4B84 { u8 pad[0xC]; s32 dmg; s32 slot; } Info4B84;
typedef struct Obj4B84 {
    u8 pad[0xA];
    u8 flags;
    u8 padB;
    Vec3f pos;
    u16 h18;
    u8 pad1A[2];
    u8 b1C;
    u8 pad1D[3];
    u8 type;
    u8 active;
    u8 slot;
    u8 pad23[0x19];
    s32 hp;
} Obj4B84;
typedef struct Type4B84 { s32 idx; u8 pad[0x40]; s32 bonus; u8 pad48[0x18]; } Type4B84;

extern Item4B84 D_80235F00[];
extern Type4B84 D_80123BC8[];
extern s32 D_803A53A0[];
extern void func_800A9A98(Item4B84*, s32);
extern s32 func_8009E9C8(Info4B84*, Vec3f*);
extern void func_800DA65C(s32, Vec3f*, u16, u8, u16, s32, u8, u8, u8);

static inline Item4B84* getItem(s32 s) {
    if (s == 0x7F) return 0;
    return &D_80235F00[s];
}

void func_800E4B84(Obj4B84* o, s32 unused, Info4B84* info) {
    Item4B84* attacker;
    Item4B84* victim;
    s32 dmg;
    u16 v;

    if (o->active) {
        {
            Item4B84* t;
            if (info->slot == 0x7F) t = 0; else t = &D_80235F00[info->slot];
            attacker = t;
        }
        dmg = info->dmg;
        if (o->hp > 0) {
            o->hp -= (s32)((f32)dmg / getItem(o->slot)->armor);
            if (o->hp < 1 && attacker != 0) {
                if (attacker->id != getItem(o->slot)->id) {
                    func_800A9A98(attacker, D_80123BC8[o->type].bonus);
                }
            }
        }
        if (o->hp <= 0) {
            victim = getItem(o->slot);
            v = func_8009E9C8(info, &o->pos);
            func_800DA65C(D_803A53A0[D_80123BC8[o->type].idx], &o->pos, o->h18, o->b1C, v, 0,
                          victim->r, victim->g, victim->b);
            o->flags |= 2;
        }
    }
}
