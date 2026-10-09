#include "types.h"

s32 func_8008A350(void *state) {
    void *item;
    s32 type;
    s32 result;

    item = *(void **)state;
    if (item == 0 || *(s32 *)item != *(s32 *)((u8 *)state + 4)) {
        return 0;
    }
    type = *(s32 *)((u8 *)item + 4);
    switch (type) {
        case 4:
            result = *(s32 *)((u8 *)item + 0xC) != 0;
            break;
        case 0x43:
            result = 1;
            break;
        default:
            result = 0;
            break;
    }
    return result;
}
