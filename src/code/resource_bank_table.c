#include "types.h"

typedef struct CompressedResourceBank {
    u32 rom_address;
    u32 compressed_size;
} CompressedResourceBank;

/* Consecutive compressed banks consumed by the resource loader. */
CompressedResourceBank gCompressedResourceBanks[26] = {
    { 0xB058FAE0, 0x00003498 },
    { 0xB0592F78, 0x00000AAC },
    { 0xB0593A28, 0x00086C6E },
    { 0xB061A698, 0x00009970 },
    { 0xB0624008, 0x00118F84 },
    { 0xB073CF90, 0x00004950 },
    { 0xB07418E0, 0x00000ECC },
    { 0xB07427B0, 0x0000289E },
    { 0xB0745050, 0x000043D2 },
    { 0xB0749428, 0x00000606 },
    { 0xB0749A30, 0x00005576 },
    { 0xB074EFA8, 0x00005EA4 },
    { 0xB0754E50, 0x000065F8 },
    { 0xB075B448, 0x00006740 },
    { 0xB0761B88, 0x0000A3DE },
    { 0xB076BF68, 0x000062E2 },
    { 0xB0772250, 0x00007ABC },
    { 0xB0779D10, 0x00008A4C },
    { 0xB0782760, 0x00008896 },
    { 0xB078AFF8, 0x00007E88 },
    { 0xB0792E80, 0x000096DA },
    { 0xB079C560, 0x00009C0E },
    { 0xB07A6170, 0x000006E0 },
    { 0xB07A6850, 0x00007DF2 },
    { 0xB07AE648, 0x00008AAC },
    { 0xB07B70F8, 0x000078B6 },
};
