#include "types.h"

typedef struct {
    u16 mask;
    u8 first;
    u8 second;
    u16 unused;
} InputRecord8009836C;

extern s32 D_80219208[];
extern InputRecord8009836C D_80219218[];
extern InputRecord8009836C D_80219238[];
extern u8 D_80216DB0;
extern u8 D_80216DB1;
extern s32 D_80216DB4;
extern s32 D_80216DB8;
extern s32 D_80216DBC;

u8 *func_8009836C(s32 index) {
    register InputRecord8009836C *current __asm__("$5");
    register InputRecord8009836C *previous __asm__("$4");
    register s32 mapped_index __asm__("$2");
    register u32 offset __asm__("$4");
    register s32 *state __asm__("$6");
    register u8 *result __asm__("$4");
    register u8 first __asm__("$2");
    register u8 second __asm__("$3");
    register u32 check __asm__("$2");

    mapped_index = D_80219208[index];
    current = D_80219238;
    __asm__ volatile("" : "=r"(current) : "0"(current));
    offset = mapped_index * sizeof(InputRecord8009836C);
    __asm__ volatile("" : "=r"(offset) : "0"(offset));
    current = (InputRecord8009836C *)(offset + (u32)current);
    {
        u16 mask = current->mask;
        state = &D_80216DB4;
        __asm__ volatile("" : "=r"(state) : "0"(state));
        *state = mask;
    }
    previous = (InputRecord8009836C *)(offset + (u32)D_80219218);

    {
        register u32 inverted __asm__("$2");
        register u32 mask __asm__("$3");
        inverted = previous->mask;
        mask = current->mask;
        inverted = ~inverted;
        mask &= inverted;
        D_80216DB8 = mask;
        __asm__ volatile("" : : : "memory");
    }
    {
        register u32 inverted __asm__("$2");
        register u32 mask __asm__("$3");
        inverted = current->mask;
        mask = previous->mask;
        inverted = ~inverted;
        mask &= inverted;
        D_80216DBC = mask;
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
