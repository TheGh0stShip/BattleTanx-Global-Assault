#include "types.h"

void func_80094F40(void *object, void *unused, u8 *type, u8 *result) {
    if (object == 0) {
        *result = 0;
    } else if (*(u8 *)((u8 *)object + 0x94) == *type) {
        *result = 2;
        *(u16 *)((u8 *)object + 0x8C) &= 0x0FFF;
    }
}
