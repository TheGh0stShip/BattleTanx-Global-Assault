/* SPAN 0x800A0A54 */
#include "types.h"

extern s32 D_8021E410;
extern s32 D_8021DA40[];
extern s32 D_8021D550[];
extern s32 D_8021DF28[];
extern s32 D_80222418;
extern s32 D_80222428[];

void func_800A0968(void) {
    register s32 *anchor asm("$2") = &D_8021E410;
    register s32 i asm("$5");

    *anchor = 0;
    i = 0x13A;
    {
        register s32 previous asm("$4");
        register s32 *forward asm("$11") = D_8021DA40;
        register s32 *backward asm("$10") = D_8021D550;
        register s32 *flags asm("$9") = D_80222428;
        register s32 one asm("$8") = 1;
        register s32 *destination_base asm("$7") = anchor - 315;
        register s32 *source_base asm("$6") = anchor - 314;

        do {
            previous = i - 1;
            forward[previous] = i;
            backward[i] = previous;
            flags[i] = one;
            destination_base[i] = source_base[i] + 1;
            i = previous;
        } while (i > 0);
    }

    i = 0x1000;
    {
        register s32 numerator asm("$7") = 10000;
        register s32 *second_anchor asm("$2") = &D_80222418;
        register s32 *destination asm("$6") = second_anchor - 1;
        register s32 *source asm("$4") = second_anchor;

        D_80222428[0] = 0;
        *source = 0;
        do {
            register s32 quotient asm("$2");
            register s32 value asm("$3");

            quotient = numerator / (i + 200);
            value = *source;
            source--;
            i--;
            value += quotient;
            *destination = value;
            destination--;
        } while (i > 0);
    }
}
