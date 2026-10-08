#include "types.h"

extern void func_800FB688(s32 channel, void *value);

void func_80097B68(void *value) {
    func_800FB688(2, value);
    func_800FB688(1, value);
}
