#include "types.h"

typedef struct {
    s32 value;
    s32 key;
} DisplayRegistryEntry;

typedef struct {
    u32 w0;
    u32 w1;
} DisplayCommand;

extern DisplayRegistryEntry D_803A57C0[];
extern s32 D_803A57E0;

void func_800B99C0(s32 value, s32 key) {
    s32 index = D_803A57E0;

    D_803A57C0[index].value = value;
    D_803A57C0[index].key = key;
    D_803A57E0 = index + 1;
}
