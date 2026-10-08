#include "types.h"

u32 func_80097CB8(void *state) {
    return (*(u16 *)((u8 *)state + 2) + 1) & 1;
}
