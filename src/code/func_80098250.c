/* SPAN 0x80098334 */
#include "types.h"

typedef struct MaskRecord80098250 {
    u16 mask;
    u8 first;
    u8 second;
    u16 unused;
} MaskRecord80098250;

extern s32 D_80219208[];
extern MaskRecord80098250 D_80219218[];
extern MaskRecord80098250 D_80219238[];
extern u8 D_80216DB1;
extern u32 D_80216DB4;
extern u32 D_80216DB8;
extern u32 D_80216DBC;

u8 *func_80098250(s32 index) {
    register MaskRecord80098250 *current __asm__("$6");
    register s32 mapped_index __asm__("$2");
    register u32 offset __asm__("$3");
    register u32 *state __asm__("$7");
    register u8 *result __asm__("$4");
    register u8 first __asm__("$2");
    register u8 second __asm__("$3");
    register u32 check __asm__("$2");

    mapped_index = D_80219208[index];
    current = D_80219238;
    __asm__ volatile("" : "=r"(current) : "0"(current));
    offset = mapped_index * sizeof(MaskRecord80098250);
    __asm__ volatile("" : "=r"(offset) : "0"(offset));
    current = (MaskRecord80098250 *)(offset + (u32)current);

    {
        register u32 raw_first __asm__("$2") = current->mask;
        register u32 expanded_first __asm__("$5");
        register u32 raw_second __asm__("$4");
        register u32 expanded_second __asm__("$3");
        register u32 difference __asm__("$2");

        state = &D_80216DB4;
        __asm__ volatile("" : "=r"(state) : "0"(state));
        raw_second = *(u16 *)(offset + (u32)D_80219218);

        expanded_first = raw_first | ((raw_first & 0xF00) >> 8);
        expanded_first |= (raw_first & 0xF) << 8;
        expanded_second = raw_second | ((raw_second & 0xF00) >> 8);
        expanded_second |= (raw_second & 0xF) << 8;

        difference = expanded_first & ~expanded_second;
        *state = expanded_first;
        D_80216DB8 = difference;
        D_80216DBC = expanded_second & ~expanded_first;
    }

    first = current->first;
    ((u8 *)state)[-4] = first;
    second = current->second;
    D_80216DB1 = second;
    result = (u8 *)state - 4;

    if ((u8)(first + 2) < 5) {
        ((u8 *)state)[-4] = 0;
    }
    check = (u8)(second + 2);
    if (check < 5) {
        D_80216DB1 = 0;
    }
    return result;
}
