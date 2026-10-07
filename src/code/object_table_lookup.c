#include "types.h"

typedef struct {
    u8 pad_0[0x104];
    s32 field_104;
    u8 pad_108[4];
} ObjectTableEntry;

extern ObjectTableEntry D_80236B30[];

s32 func_800AA598(u8 index) {
    return D_80236B30[index].field_104;
}
