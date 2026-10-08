typedef float f32; typedef double f64; typedef int s32;
extern f64 D_80075620;
typedef struct { char pad[0x24]; f32 t; char p2[4]; s32 n; } A;
void func_800DC7B0(A *a, s32 *out) {
    a->t -= D_80075620;
    if (--a->n <= 0 || a->t <= 0.0f) *out = 1;
}
