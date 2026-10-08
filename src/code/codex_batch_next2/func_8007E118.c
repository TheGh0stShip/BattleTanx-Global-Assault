#include "types.h"

extern void *func_8007DA5C(u16 index);
extern void func_8007D4A0(void *source, void *destination);

/* Exact via the label-gated prologue reorder documented in
 * docs/NORMALIZER_ASSISTED.md; the C-only output differs in scheduling. */
void func_8007E118(u8 *source, void *destination) {
    if (source[0] == 2) {
        source = func_8007DA5C(*(u16 *)(source + 2));
    }
    func_8007D4A0(source + 0xC, destination);
}
