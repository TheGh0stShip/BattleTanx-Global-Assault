#include "types.h"

void func_80098CC8(void);
extern s32 D_80216FA0[];
extern s32 D_80216FC0[];
extern u8 D_802194A4;

void func_80098FBC(void) {
    volatile s32 frame_pad[2];
    register s32 *src __asm__("$4");
    register s32 *dst __asm__("$5");
    register u8 *count __asm__("$6");
    register s32 index __asm__("$3");
    s32 initial_count;
    s32 value;

    func_80098CC8();
    count = &D_802194A4;
    initial_count = *count;
    __asm__("" : "=r"(initial_count) : "0"(initial_count));
    if (initial_count > 0) {
        index = 0;
        dst = D_80216FC0;
        src = D_80216FA0;
        do {
            value = *src;
            src++;
            *dst = (~value) != 0;
            index++;
            dst++;
        } while (index < (s32)*count);
    }
    (void)frame_pad;
}
