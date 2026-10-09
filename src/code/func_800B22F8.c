#include "types.h"

typedef struct {
    u8 pad0[4];
    s32 value;
    u16 previous;
    u8 padA[0x1C];
    u16 marker;
} Item40;

extern Item40 D_803978E0[];
extern u16 D_803977E8;
void func_800B0F4C(u16 index);

void func_800B22F8(u16 index) {
    Item40 *item = &D_803978E0[index];
    u16 previous;

    func_800B0F4C(index);
    previous = D_803977E8;
    D_803977E8 = index;
    item->value = 0;
    item->marker = 0xFFFF;
    item->previous = previous;
}
