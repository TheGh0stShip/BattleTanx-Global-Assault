/* ---- 0x800ED800/tu/unknown_800F1770.c ---- */
#include "types.h"
#define NULL ((void *)0)
extern void ***volatile D_803A57A4;
void func_800F757C(void *);
void func_800F1770(void) { if (D_803A57A4 != NULL) func_800F757C(**D_803A57A4); }

