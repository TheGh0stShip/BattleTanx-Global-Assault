#include "types.h"

typedef struct HudKey {
    u8 pad0[8];
    u8* label;
    u8 pad1[4];
} HudKey;

extern void func_800BD8B8(HudKey* from, HudKey* to);

s32 func_800C12B4(s32 arg0, HudKey* key) {
    HudKey* next;

    switch (key->label[0]) {
    case 'P':
        next = key + 9;
        break;
    case 'B':
    case '0':
        next = key + 10;
        break;
    case 'E':
        if (key->label[1] == 'R') {
            next = key + 1;
        } else {
            next = key - 1;
        }
        break;
    default:
        next = key - 1;
        break;
    }
    func_800BD8B8(key, next);
    return 0;
}

s32 func_800C1338(s32 arg0, HudKey* key) {
    HudKey* next;

    switch (key->label[0]) {
    case 'Z':
        next = key - 9;
        break;
    case 'N':
    case '+':
        next = key - 10;
        break;
    case 'E':
        if (key->label[1] == 'N') {
            next = key - 1;
        } else {
            next = key + 1;
        }
        break;
    default:
        next = key + 1;
        break;
    }
    func_800BD8B8(key, next);
    return 0;
}
