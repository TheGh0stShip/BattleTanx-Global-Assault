#include "types.h"

extern s8 D_80117EB0;
extern u8 D_8011E0E4[];
extern u8 D_8011E2B4[];
extern u8 D_8011E384[];
extern u8 D_8011E454[];

s32 func_800C7594(void* arg0) {
    u8 ret;

    switch (D_80117EB0) {
    case 1:
        ret = 0;
        break;
    case 2:
        ret = arg0 != D_8011E0E4;
        break;
    case 3:
        if (arg0 == D_8011E384) {
            ret = 0;
        } else if (arg0 == D_8011E454) {
            ret = 1;
        } else {
            ret = 2;
        }
        break;
    case 4:
        if (arg0 == D_8011E2B4) {
            ret = 0;
        } else if (arg0 == D_8011E384) {
            ret = 1;
        } else if (arg0 == D_8011E454) {
            ret = 2;
        } else {
            ret = 3;
        }
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}
