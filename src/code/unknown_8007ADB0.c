#include "types.h"

extern u8* D_80114500;

/* Address of the current display-list head in the graphics manager. */
void* func_8007ADB0(void) {
    return D_80114500 + 0xC8;
}
