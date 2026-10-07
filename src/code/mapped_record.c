#include "types.h"

typedef struct {
    u32 value;
    u16 type;
    u8 index;
} MappedRecord;

extern u8 D_80114670[];
extern u8 D_80114678[];

void func_8007D500(MappedRecord* source, s32 mode, MappedRecord* output) {
    u32 value = source->value;
    u16 type = source->type;
    u8 index;

    output->value = value;
    output->type = type;
    index = source->index;
    if (mode == 2 || mode == 0x12) {
        output->index = D_80114670[index];
    } else {
        output->index = D_80114678[index];
    }
}
