#include "types.h"
#include "m2c_macros.h"

void func_80088B6C(void *, void *, s32, s32);
void func_80088FD4(void *);


void func_80082D98(void *object, s32 alternate) {
    f32 first_x;
    f32 first_y;
    f32 next_x;
    u16 count_a;
    u16 count_b;
    u16 count_c;
    u16 count_d;
    u16 call_count;
    void *point_a;
    void *point_b;
    void *point_c;
    void *point_d;
    void *call_target;
    u32 state;
    s32 path_kind;

    path_kind = 2;
    M2C_FIELD(object, s32 *, 0x170) = path_kind;
    M2C_FIELD(object, s16 *, 0x174) = 0;
    M2C_FIELD(object, u16 *, 0x176) = 0;
    state = M2C_FIELD(object, u32 *, 0x1E0);
    state &= ~0x100;
    M2C_FIELD(object, u32 *, 0x1E0) = state;
    state = (u32)object + 0x170;

    if (alternate == 0) {
        first_x = 1600.0f;
        first_y = -1050.0f;
        count_a = M2C_FIELD(object, u16 *, 0x176);
        next_x = 700.0f;
        M2C_FIELD((void *)state, s32 *, 0) = path_kind;
        M2C_FIELD(object, f32 *, 0x178) = first_x;
        M2C_FIELD(object, f32 *, 0x17C) = first_y;
        M2C_FIELD(object, s32 *, 0x170) = path_kind;
        count_a++;
        M2C_FIELD(object, u16 *, 0x176) = count_a;
        point_a = (void *)state + (((count_a & 0xFFFF) * 8) + 8);
        M2C_FIELD(point_a, f32 *, 0) = next_x;
        M2C_FIELD(point_a, f32 *, 4) = first_y;
        count_c = M2C_FIELD(object, u16 *, 0x176);
        first_y = -500.0f;
        M2C_FIELD(object, s32 *, 0x170) = path_kind;
    } else {
        first_x = 700.0f;
        first_y = -1050.0f;
        count_b = M2C_FIELD(object, u16 *, 0x176);
        next_x = 1600.0f;
        M2C_FIELD((void *)state, s32 *, 0) = path_kind;
        M2C_FIELD(object, f32 *, 0x178) = first_x;
        M2C_FIELD(object, f32 *, 0x17C) = first_y;
        M2C_FIELD(object, s32 *, 0x170) = path_kind;
        count_b++;
        M2C_FIELD(object, u16 *, 0x176) = count_b;
        point_b = (void *)state + (((count_b & 0xFFFF) * 8) + 8);
        M2C_FIELD(point_b, f32 *, 0) = next_x;
        M2C_FIELD(point_b, f32 *, 4) = first_y;
        count_c = M2C_FIELD(object, u16 *, 0x176);
        first_y = -1500.0f;
        M2C_FIELD(object, s32 *, 0x170) = path_kind;
    }

    count_c++;
    M2C_FIELD(object, u16 *, 0x176) = count_c;
    point_c = (void *)state + (((count_c & 0xFFFF) * 8) + 8);
    M2C_FIELD(point_c, f32 *, 0) = next_x;
    M2C_FIELD(point_c, f32 *, 4) = first_y;
    M2C_FIELD(object, s32 *, 0x170) = path_kind;
    count_d = M2C_FIELD(object, u16 *, 0x176) + 1;
    M2C_FIELD(object, u16 *, 0x176) = count_d;
    point_d = (void *)state + (((count_d & 0xFFFF) * 8) + 8);
    M2C_FIELD(point_d, f32 *, 0) = first_x;
    M2C_FIELD(point_d, f32 *, 4) = first_y;

    M2C_FIELD(object, u16 *, 0x176) =
        (u16)(M2C_FIELD(object, u16 *, 0x176) + 1);
    call_count = M2C_FIELD((void *)state, u16 *, 4);
    call_target = (void *)state + ((call_count * 8) + 8);
    func_80088B6C(object, call_target, 0x42C80000, 1);
    func_80088FD4(object);
}
