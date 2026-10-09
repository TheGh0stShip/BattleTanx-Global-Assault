#include "types.h"

void func_80097530(u8 *text) {
    u8 *start = text;

    while (*text != 0) {
        text++;
    }
    if (text != start) {
        text[-1] = 0;
    }
}
