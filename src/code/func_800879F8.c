/* RODATA_VRAM 0x80071608 */
#include "types.h"

typedef struct {
    u16 angle;
    char pad2[2];
    f32 rate;
    f32 amplitude;
    f32 offset;
} WaveSample;

f32 func_8009D4B0(u16 angle);

f32 func_800879F8(WaveSample *sample, f32 phase) {
    u32 step = phase * sample->rate;

    return sample->offset + sample->amplitude * func_8009D4B0(sample->angle + step);
}
