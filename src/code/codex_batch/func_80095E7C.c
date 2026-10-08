#include "types.h"

extern u8 D_801146E8[];
extern u8 D_801146F0[];

void func_80095E7C(void *arg0) {
    register u8 *current __asm__("$4") = arg0;
    register u8 *source __asm__("$5");
    register u8 *replacement __asm__("$6");
    register u8 *entry __asm__("$7");
    register s32 index __asm__("$8");
    register u8 *end __asm__("$9");
    register u8 *replacement_base __asm__("$10");
    register u8 *source_base __asm__("$11");
    register u32 terminator __asm__("$12");
    s32 first;
    s32 second;

    terminator = 0xDF000000;
    source_base = D_801146E8;
    replacement_base = D_801146F0;
    end = current + 0x1F40;
outer:
    if (*(u32 *)current == terminator) {
        return;
    }
    index = 0;
    entry = current;
    replacement = replacement_base;
    source = source_base;
inner:
    if ((*(s32 *)source == *(s32 *)entry) &&
        (*(s32 *)(source + 4) == *(s32 *)(entry + 4))) {
        first = *(s32 *)replacement;
        second = *(s32 *)(replacement + 4);
        *(s32 *)entry = first;
        *(s32 *)(entry + 4) = second;
    }
    replacement += 8;
    index++;
    source += 8;
    if (index <= 0) {
        goto inner;
    }
    current += 8;
    if ((s32)current < (s32)end) {
        goto outer;
    }
}
