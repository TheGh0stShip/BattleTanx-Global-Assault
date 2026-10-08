#include "types.h"

extern void func_8009ED00(u32 value);

typedef struct Entry80097BC4 {
    u32 argument;
    u32 limit;
} Entry80097BC4;

extern Entry80097BC4 D_80114710[];

u32 func_80097BC4(s32 index, s32 unused, u32 value) {
    if (value <= D_80114710[index].limit) {
        func_8009ED00(D_80114710[index].argument);
        return value;
    }
    return 0;
}
