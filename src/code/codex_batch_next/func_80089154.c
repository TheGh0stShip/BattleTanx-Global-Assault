#include "types.h"

void *func_80089154(void *object) {
    if (*(u8 *)((u8 *)object + 0xE8) != 0) {
        return (u8 *)object + 0xD0;
    }
    return 0;
}
