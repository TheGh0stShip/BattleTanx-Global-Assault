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
extern s32 func_8009E9C8(void*, Vec3f*);
extern void func_800DA65C(s32, Vec3f*, u16, u8, u16, s32, u8, u8, u8);

static inline Item4B84* getItem(s32 s) {
    if (s == 0x7F) return 0;
    return &D_80235F00[s];
}

typedef struct Info48E0 { f32 x; f32 y; s32 a8; s32 slot; s32 mode; s32 a14; } Info48E0;
typedef struct Loc48E0 { f32 x, y, z; s32 pad; s16 h; u8 b; } Loc48E0;
extern void func_800E2E0C(Obj4B84*, Loc48E0*, s16*, u8*);
extern void func_800E462C(Obj4B84*, Loc48E0*, u8, s32, s32, s32, s32);
extern s32 func_800EC72C(s32, s32);

void func_800E48E0(Obj4B84* o, s32 unused, Info48E0* info) {
    Loc48E0 l;
    Item4B84* attacker;
    Item4B84* victim;
    s32 dmg;
    u16 v;

    if (info->mode == 1) {
        func_800E2E0C(o, &l, &l.h, &l.b);
        dmg = (info->x - l.x) * (info->x - l.x) + (info->y - l.y) * (info->y - l.y);
        func_800E462C(o, &l, l.b, info->a14, info->slot, dmg, info->a8);
    } else if (o->active) {
        dmg = func_800EC72C(info->a14, info->a8);
        {
            Item4B84* t;
            if (info->slot == 0x7F) t = 0; else t = &D_80235F00[info->slot];
            attacker = t;
        }
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
