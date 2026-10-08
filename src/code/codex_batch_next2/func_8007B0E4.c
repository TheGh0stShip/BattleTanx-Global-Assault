#include "types.h"

typedef struct DisplayCommand8007B0E4 {
    u32 word0;
    u32 word1;
} DisplayCommand8007B0E4;

extern s16 D_80114510;
extern u8 D_80114512;
extern u8 D_8017CEE0[];
extern void _bcopy(const void *source, void *destination, s32 size);

void *func_8007B0E4(const void *source, s32 count, void *fallback) {
    DisplayCommand8007B0E4 *command;
    register s32 copy_size __asm__("$17");
    u32 base_address;
    register u32 destination_address __asm__("$16");
    s16 next_cursor;
    s16 cursor;

    cursor = D_80114510;
    next_cursor = cursor;
    if (cursor + count + 1 >= 0x2001) {
        return fallback;
    }
    copy_size = count * sizeof(DisplayCommand8007B0E4);
    destination_address = D_80114512;
    D_80114510 = ++next_cursor + count;
    base_address = (u32)(D_8017CEE0
        + cursor * sizeof(DisplayCommand8007B0E4));
    destination_address = (destination_address << 16) + base_address;
    _bcopy(source, (void *)destination_address, copy_size);
    command = (DisplayCommand8007B0E4 *)destination_address + count;
    command->word0 = 0xDE010000;
    command->word1 = (u32)fallback;
    return (void *)destination_address;
}
