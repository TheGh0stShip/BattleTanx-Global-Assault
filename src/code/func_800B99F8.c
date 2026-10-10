/* SPAN 0x800B9A4C */
typedef int s32;

typedef struct {
    s32 value;
    s32 key;
} DisplayRegistryEntry;

extern DisplayRegistryEntry D_803A57C0[];
extern s32 D_803A57E0;

/* Looks up key in the interleaved value/key registry. */
s32 func_800B99F8(s32 key) {
    s32 i;

    for (i = 0; i < D_803A57E0; i++) {
        if (key == D_803A57C0[i].key) {
            return D_803A57C0[i].value;
        }
    }
    return 0;
}
