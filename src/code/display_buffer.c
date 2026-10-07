#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} DisplayCommand;

typedef struct {
    u8 pad00[0xC0];
    u16 buffer_index;
    u16 padC2;
    u32 pending;
    DisplayCommand* primary;
    DisplayCommand* secondary;
} DisplayState;

extern DisplayState* D_80114500;

void func_8007AC34(u32 address, u8 flag) {
    DisplayCommand* command = D_80114500->primary++;
    command->w0 = 0xDA380000 | ((flag ^ 1) & 0xFF);
    command->w1 = address - 0x80000000;
}

void func_8007AC6C(u32 address, u8 flag) {
    DisplayCommand* command = D_80114500->primary++;
    command->w0 = 0xDA380000 | ((flag ^ 1) & 0xFF);
    command->w1 = address - 0x80000000;
}

void func_8007ACA4(u32 address, u8 flag, u16 value) {
    DisplayCommand* command = D_80114500->primary++;
    command->w0 = 0xDB0E0000;
    command->w1 = value;
    command = D_80114500->primary++;
    command->w0 = 0xDA380000 | ((flag ^ 1) & 0xFF);
    command->w1 = address - 0x80000000;
}

void func_8007ACF8(u32 address) {
    DisplayCommand* command = D_80114500->primary++;
    command->w0 = 0xDE000000;
    command->w1 = address;
}

void func_8007AD1C(u32 address) {
    DisplayCommand* command = D_80114500->secondary++;
    command->w0 = 0xDE000000;
    command->w1 = address;
}
