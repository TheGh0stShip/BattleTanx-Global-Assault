#include "types.h"

typedef struct {
    u16 flags;
    u8 pad2[14];
} Entry800BFCA4;

extern Entry800BFCA4 D_803A57F0[];
extern u64 D_803A5938;
extern u16 D_80116840;
extern u16 D_803A5972;
extern u16 D_803A5970;
extern void *D_803A5954;
extern void *D_803A5958;

u64 osGetTime(void);
void *func_800ACEB4(s32 size);
void func_800C6920(void);

void func_800BFCA4(void) {
    u16 i;

    D_803A5938 = osGetTime();
    for (i = 0; i < 20; i++) {
        D_803A57F0[i].flags = 0;
    }
    D_80116840 = 0;
    D_803A5972 = 0;
    D_803A5970 = 0;
    D_803A5954 = func_800ACEB4(0x5EB0);
    D_803A5958 = func_800ACEB4(0x5EB0);
    func_800C6920();
}
