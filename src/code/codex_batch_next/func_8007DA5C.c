#include "types.h"

extern u8 *D_80114680;

void *func_8007DA5C(u16 index) {
    void *result;

    if (index != 0) {
        result = D_80114680 + index * 24;
    } else {
        result = 0;
    }
    return result;
}
