#include "types.h"

extern u8 func_80077D64(u32 address);
extern void func_80077FD0(u32 address, u32 value);

void func_80078200(u32 destination, u32 source, u32 length) {
    while (length-- != 0) {
        func_80077FD0(destination++, func_80077D64(source++));
    }
}
