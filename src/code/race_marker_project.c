/* SPAN 0x800C8B74 */
/* RODATA_VRAM 0x80073D80 */
/* Owns rodata 0x80073D80-0x80073DA8 (3.0f, 8.0f, 3.0f, 8.0f, 5000.0f,
 * 7000.0f, 1000.0f, 512.0f, 2^31, 2^31). func_800C87EC, func_800C8854,
 * func_800C8870, func_800C88E8, func_800C8930, func_800C8970, func_800C89E4,
 * func_800C8ABC, func_800C8AFC, func_800C8B14 and func_800C8B48 are interior
 * labels. */
#include "types.h"

#define ABS(x) ((x) > 0.0f ? (x) : -(x))

typedef struct {
    u8 pad0[0x30];
    f32 x;
    f32 z;
} MarkerPos;

typedef struct {
    u8 pad0[0x78];
    MarkerPos pos;
    u8 padB0[0x1A0];
} RacePlayer;

typedef struct {
    s16 x;
    s16 y;
    u16 icon;
    u8 alpha;
    u8 pad7;
} RaceMarker;

extern RacePlayer D_80235F00[];
extern RaceMarker D_803A5990[][32];
extern s32 func_800ACCB4(f32* pos, f32* out, u8 arg2, u16 player);
extern u16 func_800C7AA0(u16 type);

static inline RacePlayer* race_player_get(s32 id) {
    if (id == 0x7F) {
        return 0;
    }
    return &D_80235F00[id];
}

s32 func_800C877C(f32* pos, s32 arg1, u16 player, u16 type, u16 slot, u16 flags) {
    RacePlayer* p;
    MarkerPos* m;
    f32 major;
    f32 minor;
    f32 dist;
    f32 range;
    f32 out[3];

    p = race_player_get(player);
    m = &p->pos;
    if (ABS(m->x - pos[0]) > ABS(m->z - pos[1])) {
        major = ABS(m->x - pos[0]);
    } else {
        major = ABS(m->z - pos[1]);
    }
    if (ABS(m->x - pos[0]) < ABS(m->z - pos[1])) {
        minor = m->x - pos[0];
        if (minor > 0.0f) {
            dist = major + minor * 3.0f / 8.0f;
        } else {
            dist = major + -minor * 3.0f / 8.0f;
        }
    } else {
        minor = m->z - pos[1];
        if (minor > 0.0f) {
            dist = major + minor * 3.0f / 8.0f;
        } else {
            dist = major + -minor * 3.0f / 8.0f;
        }
    }
    if (!(flags & 0xF0)) {
        if (dist > 5000.0f) {
            return 0;
        }
        range = 5000.0f;
    } else {
        if (dist > 7000.0f || dist < 1000.0f) {
            return 0;
        }
        range = 7000.0f;
    }
    if (func_800ACCB4(pos, out, arg1, player)) {
        D_803A5990[player][slot].x = (s32)out[0] - 4;
        D_803A5990[player][slot].y = (s32)out[2] - 7;
        D_803A5990[player][slot].icon = func_800C7AA0(type);
        if (dist < range - 512.0f) {
            D_803A5990[player][slot].alpha = 0xFF;
        } else {
            D_803A5990[player][slot].alpha = ((u16)(u32)range - (u16)(u32)dist) >> 1;
        }
        D_803A5990[player][slot].icon |= flags;
        return 1;
    }
    return 0;
}
