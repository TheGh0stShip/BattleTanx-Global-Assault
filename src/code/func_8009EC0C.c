/* SPAN 0x8009ECF4 */
#include "types.h"

extern f32 sqrtf(f32 value);
extern f32 D_8007262C;
extern f32 D_80072630;

f32 func_8009EC0C(f32 *vector) {
    f32 length;
    register f32 root asm("$f2");

    root = sqrtf((vector[0] * vector[0]) +
                 (vector[2] * vector[2]) +
                 (vector[1] * vector[1]));
    length = root;

    if (length > 0.0f ? length < D_8007262C : -length < D_80072630) {
        return 0.0f;
    }

    vector[0] /= length;
    vector[2] /= length;
    vector[1] /= length;
    return length;
}
