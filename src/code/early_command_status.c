#include "types.h"

extern u32 func_80077C40(void);
extern void func_80077C78(u32 status);
extern u32 func_80078830(u32 command, u32 arg1, u32 arg2, u32 arg3);

u32 func_80078C68(u32 arg1, u32 arg2, u32 arg3) {
    u32 status = func_80077C40();
    u32 result = func_80078830(0x707, arg1, arg2, arg3);

    func_80077C78(status);
    return result;
}
