#include "types.h"

void func_80097508(u8 *text, u8 value) {
    while (*text != 0) {
        text++;
    }
    *text++ = value;
    *text = 0;
}
