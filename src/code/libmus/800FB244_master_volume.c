#include "types.h"

extern s16 D_803AD98C;
extern s16 D_803AD98E;

void MusSetMasterVolume(s32 type, s32 volume) {
    if (type & 1) {
        D_803AD98C = volume;
    }
    if (type & 2) {
        D_803AD98E = volume;
    }
}
