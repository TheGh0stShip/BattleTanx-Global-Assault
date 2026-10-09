#include "types.h"

extern s32 D_8023A064;
extern void *D_8023A068;
extern u8 D_8023A070[];
extern void _bzero(void *destination, s32 size);

void func_800ACE70(void) {
    D_8023A068 = D_8023A070;
    D_8023A064 = 0x15D5D8;
    _bzero(D_8023A070, 0x15D5D8);
}
