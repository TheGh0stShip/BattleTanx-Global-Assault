#include "types.h"

extern s32 func_8008E620(void);

s32 func_80087DD4(void) {
    return func_8008E620() < 10;
}
