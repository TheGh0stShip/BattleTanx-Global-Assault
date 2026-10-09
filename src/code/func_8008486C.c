#include "types.h"
#include "m2c_macros.h"

f32 func_8009D8A0(f32);
void func_8009DB0C(f32 *, f32);
void func_8009E044(void *, f32 *);

void func_8008486C(void *arg0) {
    f32 position[2];
    f32 random_value;
    void *object;

    object = M2C_FIELD(arg0, void **, 0x90);
    if (object != 0) {
        position[0] = M2C_FIELD(object, f32 *, 8);
        position[1] = M2C_FIELD(object, f32 *, 0xC);
        random_value = func_8009D8A0(M2C_FIELD(object, f32 *, 0x10));
        func_8009DB0C(position, random_value);
        M2C_FIELD(arg0, f32 *, 0x17C) = M2C_FIELD(object, f32 *, 0);
        M2C_FIELD(arg0, f32 *, 0x180) = M2C_FIELD(object, f32 *, 4);
        func_8009E044(arg0 + 0x17C, position);
    }
}
