#include "types.h"

extern void *D_80125EA0;
extern void *D_80125EA4;
extern void *D_80125EA8;
extern void *D_80125EAC;
extern void *D_80125EB0;
extern void *D_80125EB4;
extern void *D_80125EB8;

void func_800F80E8(u32 variant, void **first, void **second) {
    switch (variant) {
    case 6:
        *second = D_80125EB8;
        *first = D_80125EAC;
        break;
    case 9:
        *second = D_80125EB8;
        *first = D_80125EA8;
        break;
    case 1:
        *second = D_80125EB8;
        *first = D_80125EA4;
        break;
    case 0:
        *second = D_80125EB8;
        *first = D_80125EA0;
        break;
    case 11:
        *second = D_80125EB0;
        *first = D_80125EAC;
        break;
    case 3:
        *second = D_80125EB0;
        *first = D_80125EA8;
        break;
    case 8:
        *second = D_80125EB0;
        *first = D_80125EA4;
        break;
    case 5:
        *second = D_80125EB0;
        *first = D_80125EA0;
        break;
    case 4:
        *second = D_80125EB4;
        *first = D_80125EAC;
        break;
    case 7:
        *second = D_80125EB4;
        *first = D_80125EA8;
        break;
    case 2:
        *second = D_80125EB4;
        *first = D_80125EA4;
        break;
    case 10:
        *second = D_80125EB4;
        *first = D_80125EA0;
        break;
    default:
        *second = 0;
        *first = 0;
        break;
    }
}
