#include "types.h"

/*
 * Address-validity map consumed by early_memory_dispatch.c (D_801144A0[]): the
 * search walks entries until limit >= addr and uses flags for that span. Limits are
 * N64 physical/virtual address tokens. D_801144A8 / D_801144B8 (entries 1 and 3)
 * are patched at boot with the detected RDRAM size (early_memory_dispatch.c:97-98).
 */
typedef struct MemRegion {
    u32 limit;
    u32 flags;
} MemRegion;

MemRegion gMemRegionTable[9] = {   /* D_801144A0 */
    { 0x7FFFFFFF, 0x0 },   /* KUSEG: invalid */
    { 0x803FFFFF, 0x3 },   /* KSEG0 RDRAM (limit patched to RAM size) */
    { 0x9FFFFFFF, 0x0 },
    { 0xA03FFFFF, 0x3 },   /* KSEG1 RDRAM (limit patched to RAM size) */
    { 0xA3FFFFFF, 0x0 },
    { 0xA4001FFF, 0xB },   /* SP DMEM/IMEM */
    { 0xAFFFFFFF, 0x0 },
    { 0xB1FDFFFF, 0x7 },   /* cartridge domain 1 */
    { 0xFFFFFFFF, 0x0 },   /* terminator */
};
