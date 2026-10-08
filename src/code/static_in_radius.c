extern float D_80077450;
int func_800F80A0(float *a, float *b) {
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    if (dx * dx + dy * dy > D_80077450) return 0;
    return 1;
}
