#include "types.h"

typedef struct {
    u8 pad00[0xC0];
    u16 buffer_index;
} DisplayState;

extern DisplayState* D_80114500;

void* func_8007AD94(void) {
    return (u8*)D_80114500 + 0x90 + D_80114500->buffer_index * 0x10;
}
