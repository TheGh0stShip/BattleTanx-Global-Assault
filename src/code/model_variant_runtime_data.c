#include "types.h"

/*
 * Four display-list tokens followed by the initialized model-cache state.
 * These are N64 addresses and runtime words, not native host pointers.
 */
u32 gModelVariantRuntimeWords[56] = {
    0x80125D40, 0x80125D98, 0x80125DF0, 0x80125E48,
};
