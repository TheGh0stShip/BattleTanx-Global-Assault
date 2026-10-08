#include "types.h"

extern s32 D_8021945C;
extern void func_80086208(void *object, s32 state);

void func_80085EB0(void *object) {
    u8 *data = object;

    switch (*(u32 *)(data + 0x170)) {
        case 1:
            if (D_8021945C >= *(s32 *)(data + 0x174)) {
                *(s32 *)(data + 0x170) = 2;
            }
            break;
        case 2:
            if (*(u16 *)(data + 0x178) > 0x200) {
                *(u16 *)(data + 0x178) -= 0x200;
            } else {
                s32 time = D_8021945C + 0x5A;

                *(u16 *)(data + 0x178) = 0;
                *(s32 *)(data + 0x170) = 3;
                *(s32 *)(data + 0x174) = time;
            }
            break;
        case 3:
            if (D_8021945C >= *(s32 *)(data + 0x174)) {
                func_80086208(object, 6);
            }
            break;
    }
}
