/* RODATA_VRAM 0x800713A0 */
#include "types.h"

typedef struct {
    char pad0[0xBC];
    void *target;
    char padC0[0x168 - 0xC0];
    s32 state;
    char pad16C[8];
    s32 nextState;
} TransitionObject;

void func_800859A8(void *object, s32 state, void *target, void *context);
void func_80086208(void *object, s32 state);

void func_80086190(TransitionObject *object, void *context) {
    s32 state;

    switch (object->state) {
        case 6:
        case 17:
        case 18:
        case 19:
        case 21:
            object->state = 24;
            func_800859A8(object, 6, object->target, context);
            break;
        case 9:
        case 11:
        case 12:
        case 13:
            state = object->state;
            object->state = 24;
            func_800859A8(object, state, object->target, context);
            break;
        case 24:
            func_80086208(object, object->nextState);
            return;
        default:
            return;
    }
}
