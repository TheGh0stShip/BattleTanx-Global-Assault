#include "types.h"

typedef struct {
    u8 pad_000[0x170];
    s32 state;
    s32 timestamp;
    u8 pad_178[0x68];
    u32 flags;
} Object80085F7C;

extern s32 D_8021945C;
extern void func_80088854(Object80085F7C *object);
extern void func_80089008(Object80085F7C *object);

void func_80085F7C(Object80085F7C *object, u32 state) {
    object->flags &= ~0x100;

    switch (state) {
    case 1:
        object->state = state;
        object->timestamp = D_8021945C;
        break;
    case 2:
        object->state = state;
        object->timestamp = D_8021945C + 0x258;
        break;
    case 4:
        object->state = state;
        object->timestamp = D_8021945C + 0x12C;
        break;
    }

    func_80088854(object);
    func_80089008(object);
}
