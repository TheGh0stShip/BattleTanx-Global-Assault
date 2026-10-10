/* RODATA_VRAM 0x800710B0 */
#include "types.h"

typedef struct {
    char pad0[4];
    u16 target;
    u8 mode;
} PackedHeadingTarget;

void func_800B5728(u16 target, u8 mode, u16 heading, s32 arg3, s32 arg4);

void func_8007D5B0(PackedHeadingTarget *target, f32 heading, s32 arg4, s32 arg3) {
    f32 adjusted = heading + 20.0f;
    u16 targetId = target->target;
    u8 mode = target->mode;
    s32 savedArg = arg4;

    func_800B5728(targetId, mode, (u16)(u32)adjusted, arg3, savedArg);
}
