int func_800F80A0(float *a, float *b) {
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    if (dx * dx + dy * dy > 1e+06f) return 0;
    return 1;
}
