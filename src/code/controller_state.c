#include "types.h"

typedef struct {
    u8 pad00[0xC0];
    u16 flag;
    u16 padC2;
    u32 pending;
    u8 data[1];
} ControllerState;

extern ControllerState* D_80114500;

u32 func_8007ADC0(void) {
    u32 pending = D_80114500->pending;
    D_80114500->pending = 0;
    D_80114500->flag ^= 1;
    return pending;
}

void* func_8007ADE0(void) {
    return D_80114500->data;
}
