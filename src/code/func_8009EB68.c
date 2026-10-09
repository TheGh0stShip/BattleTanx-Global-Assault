#include "types.h"

typedef struct Vec3f {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct Plane4f {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} Plane4f;

void func_8009EB68(Plane4f *plane, Vec3f *start, Vec3f *end, Vec3f *out) {
    register f32 px __asm__("$f4") = plane->x;
    register f32 sx __asm__("$f14") = start->x;
    register f32 start_distance __asm__("$f0") = px * sx;
    register f32 pz __asm__("$f10") = plane->z;
    register f32 work8 __asm__("$f8") = start->z;
    register f32 ex __asm__("$f12") = end->x;
    register f32 end_distance __asm__("$f4") = px;
    register f32 work2 __asm__("$f2") = end->z;
    register f32 py __asm__("$f6") = plane->y;

    work8 = pz * work8;
    end_distance *= ex;
    pz *= work2;
    work2 = py * start->y;
    start_distance += work8;
    work8 = end->y;
    end_distance += pz;
    end_distance += py * work8;
    start_distance += work2;
    work2 = plane->w;
    start_distance += work2;
    end_distance += work2;
    end_distance = start_distance - end_distance;
    start_distance /= end_distance;

    ex -= sx;
    ex = start_distance * ex;
    sx += ex;
    out->x = sx;
    work2 = end->z;
    end_distance = start->z;
    work2 -= end_distance;
    work2 = start_distance * work2;
    end_distance += work2;
    out->z = end_distance;
    work2 = end->y;
    end_distance = start->y;
    work2 -= end_distance;
    work2 = start_distance * work2;
    end_distance += work2;
    out->y = end_distance;
}
