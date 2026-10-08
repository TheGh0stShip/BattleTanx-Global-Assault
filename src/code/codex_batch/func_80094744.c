#include "types.h"
#include "m2c_macros.h"

void func_80092C38(void *, f32 *, s32);

void func_80094744(void *arg0) {
    f32 position[3];
    f32 *source;
    s32 mode;
    void *object;
    register void *call_object __asm__("$4");

    object = M2C_FIELD(arg0, void **, 0xC);
    if (object != 0) {
        if (M2C_FIELD(object, s32 *, 0x1E0) & 0x1000) {
            func_80092C38(object, object + 8, 0);
            position[0] = M2C_FIELD(object, f32 *, 0x150);
            __asm__("addu %0,%1,$0" : "=r"(call_object) : "r"(object));
            position[2] = M2C_FIELD(object, f32 *, 0x10);
            source = position;
            mode = 1;
            position[1] = M2C_FIELD(call_object, f32 *, 0x154);
        } else {
            call_object = object;
            source = object + 8;
            mode = 0;
        }
        func_80092C38(call_object, source, mode);
    }
}
