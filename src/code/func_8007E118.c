#include "types.h"

extern void *Steps_InitStep_Free(u16 index);
extern void func_8007D4A0(void *source, void *destination);

/* Exact via the label-gated prologue reorder documented in
 * docs/NORMALIZER_ASSISTED.md; the C-only output differs in scheduling. */
void Steps_FreeBranch(u8 *source, void *destination) {
    if (source[0] == 2) {
        source = Steps_InitStep_Free(*(u16 *)(source + 2));
    }
    func_8007D4A0(source + 0xC, destination);
}
