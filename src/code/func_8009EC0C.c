/* SPAN 0x8009ECF4 */
#include "types.h"

extern f32 sqrtf(f32 value);

f32 func_8009EC0C(f32 *vector) {
    f32 length;
    register f32 root asm("$f2");

    root = sqrtf((vector[0] * vector[0]) +
                 (vector[2] * vector[2]) +
                 (vector[1] * vector[1]));
    length = root;

    if (length > 0.0f ? length < 0.0001f : -length < 0.0001f) {
        return 0.0f;
    }

    vector[0] /= length;
    vector[2] /= length;
    vector[1] /= length;
    return length;
}
