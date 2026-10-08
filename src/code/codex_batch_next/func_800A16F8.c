#include "types.h"

extern s32 func_8007AF84(s32 value);
extern void func_8007ADF0(s32 index, void *value);
extern void func_800A17F0(void *object);
extern void func_800A1858(void *object);

void func_800A16F8(void *object) {
    u8 *data = object;

    if (func_8007AF84(1) == 0) {
        func_8007ADF0(1, *(void **)(*(u8 **)(data + 0x204) + 0x40));
        func_8007ADF0(2, 0);
        if (*(u16 *)(data + 0x1F4) == 0 && *(s32 *)(data + 0x1FC) == 1) {
            func_800A17F0(data);
        } else if (*(u16 *)(data + 0x1F4) & 4) {
            *(u16 *)(data + 0x1F4) |= 8;
        } else {
            *(s32 *)(data + 0x204) = 0;
        }
    } else {
        *(s32 *)(data + 0x1F8) = 1;
    }
    func_800A1858(data);
}
