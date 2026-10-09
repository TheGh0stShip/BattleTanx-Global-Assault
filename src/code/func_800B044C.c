#include "types.h"

extern void func_800A9C24(s32 mode, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8, u8);
extern void func_80079C00(u8, u8, u8);

void func_800B044C(u8 *values) {
    func_800A9C24(2, values[0], values[1], values[2], values[3], values[4],
                  values[5], values[6], values[7], values[8], values[9],
                  values[10], values[11]);
    func_80079C00(values[12], values[13], values[14]);
}
