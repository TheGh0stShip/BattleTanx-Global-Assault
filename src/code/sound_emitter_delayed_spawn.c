/* ---- 0x800E4800/src/func_800E5164.c ---- */
#include "types.h"

typedef struct Snd5164 {
    u8 pad00[0x0A];
    u16 id;
    u8 pos[0x0C];
    u16 h18;
    u8 b1A;
    u8 b1B;
    s32 time;
    u8 b20;
} Snd5164;

extern s32 D_8021945C;
extern void func_800B22F8(u16);
extern void func_800E2AEC(s32, void*, s32, s32, s32, s32, s32);

void func_800E5164(Snd5164* s, s32* out) {
    if (D_8021945C - s->time > 32) {
        func_800B22F8(s->id);
        func_800E2AEC(s->b1A, s->pos, s->h18, 0, s->b1B, s->b20, 0);
        *out = 1;
    }
}

