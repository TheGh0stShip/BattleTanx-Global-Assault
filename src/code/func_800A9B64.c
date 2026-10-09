#include "types.h"
#include "m2c_macros.h"

void func_800979F4(s32);
void func_800CA620(u8, s32, s32);
extern s32 D_8021945C;

void func_800A9B64(void *arg0, s32 arg1) {
    register s32 offset __asm__("$16");
    register void *entry __asm__("$5");
    register s32 sound __asm__("$5");
    register s32 effect __asm__("$4");
    u8 owner;

    if (arg0 != 0) {
        offset = arg1 * 4;
        if (M2C_FIELD(arg0, u8 *, 0xA) & 2) {
            __asm__("addu %0,%1,%2" : "=r"(entry) : "r"(offset), "r"(arg0));
            if ((D_8021945C - M2C_FIELD(entry, s32 *, 0x21C)) >= 0x3D) {
                M2C_FIELD(entry, s32 *, 0x21C) = D_8021945C;
                owner = M2C_FIELD(arg0, u8 *, 0xB);
                __asm__ volatile("lui $1,%%hi(D_8011666C)\n"
                                 "addu $1,$1,%1\n"
                                 "lw %0,%%lo(D_8011666C)($1)"
                                 : "=r"(sound) : "r"(offset));
                func_800CA620(owner, sound, 0x2D);
                __asm__ volatile("lui $1,%%hi(D_801166A0)\n"
                                 "addu $1,$1,%1\n"
                                 "lw %0,%%lo(D_801166A0)($1)"
                                 : "=r"(effect) : "r"(offset));
                func_800979F4(effect);
            }
        }
    }
}
