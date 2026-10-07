#include "types.h"

typedef struct {
    u8 pad_0[0xA];
    u8 flags_A;
    u8 pad_B[0x1BE];
    u8 limit_A;
    u8 limit_B;
    u8 pad_1CB[0x32];
    u8 value;
} LimitObject;

s32 func_800A974C(LimitObject* object) {
    u8 limit;
    u8 value;

    if (!(object->flags_A & 2)) {
        value = object->value;
        limit = object->limit_A;
    } else {
        value = object->value;
        limit = object->limit_B;
    }
    return limit < value;
}
