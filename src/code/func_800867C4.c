/* RODATA_VRAM 0x80071508 */
#include "types.h"

typedef struct {
    char pad0[0x10];
    s32 owner;
} InteractionObject;

extern s32 D_802194A0;
extern u8 *D_80114680;

void func_8008723C(void *object, s32 mode, s32 arg2, s32 arg3);

void func_800867C4(InteractionObject *object) {
    s32 mode;

    switch (D_802194A0) {
        case 1:
        case 2:
        case 3:
        case 11:
            mode = 2;
            break;
        case 4:
        case 10:
            if (*(s32 *)(D_80114680 + 0xC070) != object->owner) {
                mode = 7;
                break;
            }
        case 5:
        case 6:
        case 7:
        case 8:
        case 12:
        case 13:
        case 14:
        default:
            mode = 1;
            break;
        case 9:
            return;
    }
    func_8008723C(object, mode, 0, 0);
}
