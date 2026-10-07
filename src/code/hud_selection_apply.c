#include "types.h"

extern u16 D_8011DC56;
extern s16 D_803A5972;

s32 func_800C290C(void) {
    D_803A5972 = D_8011DC56;
    return 1;
}
