#include "types.h"

typedef struct {
    u8 pad[0x38];
    u8 *display;
} MenuState;

extern u8 D_80117EB0;
extern s32 D_80114804;
extern s32 D_80117EB4;
extern u8 D_801185A8[];
void func_80098CC8(void *arg);

s32 func_800C04C8(void *arg, MenuState *state) {
    func_80098CC8(arg);
    D_80117EB0--;
    if ((s8)D_80117EB0 <= 0) {
        D_80117EB0 = D_80114804;
    }
    if ((s8)D_80117EB0 >= 3 && D_80117EB4 == 1) {
        D_80117EB4 = 2;
        state->display = D_801185A8;
    }
    return 0;
}
