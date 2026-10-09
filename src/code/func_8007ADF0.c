#include "types.h"

typedef struct DisplayContext8007ADF0 {
    u8 pad00[0xB0];
    s16 slots[4];
    s32 stride;
    u16 *records;
} DisplayContext8007ADF0;

extern DisplayContext8007ADF0 *D_80114500;

void func_8007ADF0(s32 slot, u16 *record) {
    DisplayContext8007ADF0 *context;
    u16 *current;
    s16 index;
    s16 count;

    if (slot == 0) {
        if (record == 0) {
            index = -1;
        } else {
            current = D_80114500->records;
            context = D_80114500;
            for (count = 0; count < 3; count++) {
                if (current == record) {
                    break;
                }
                current += context->stride;
            }
            index = count;
        }
        D_80114500->slots[3] = index;
    } else if (slot == 1) {
        if (record == 0) {
            index = -1;
        } else {
            current = D_80114500->records;
            context = D_80114500;
            for (count = 0; count < 3; count++) {
                if (current == record) {
                    break;
                }
                current += context->stride;
            }
            index = count;
        }
        D_80114500->slots[2] = index;
    } else if (slot == 2) {
        if (record == 0) {
            index = -1;
        } else {
            current = D_80114500->records;
            context = D_80114500;
            for (count = 0; count < 3; count++) {
                if (current == record) {
                    break;
                }
                current += context->stride;
            }
            index = count;
        }
        D_80114500->slots[1] = index;
    } else if (slot == 3) {
        if (record == 0) {
            index = -1;
        } else {
            current = D_80114500->records;
            context = D_80114500;
            for (count = 0; count < 3; count++) {
                if (current == record) {
                    break;
                }
                current += context->stride;
            }
            index = count;
        }
        D_80114500->slots[0] = index;
    }
}
