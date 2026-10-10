#include "types.h"

typedef struct {
    u8 pad00[8];
    s32 x;
    s32 y;
    u8 pad10[0x84];
    u8 kind;
} EffectState9660C;

typedef struct {
    u8 pad00[4];
    s32 type;
} EffectSource9660C;

extern u32 func_8009D914(void);
extern void func_80097FB4(s32 effect, s32 x, s32 y, f32 scale, u8 kind);

void func_8009660C(EffectState9660C *state, f32 amount,
                   EffectSource9660C *source) {
    f32 scale;
    s32 effect;
    u32 choice;

    if (0.4f < amount) {
        scale = (amount - 0.4f) / 0.4f;
        if (source != 0 && source->type == 4) {
            func_80097FB4(0x23, state->x, state->y, scale, state->kind);
        } else {
            choice = func_8009D914() & 3;
            switch (choice) {
                case 0:
                    effect = 0x21;
                    break;
                case 1:
                    effect = 0x22;
                    break;
                case 2:
                    effect = 0x23;
                    break;
                default:
                    effect = 0x25;
                    break;
            }
            func_80097FB4(effect, state->x, state->y, scale, state->kind);
        }
    }
}
