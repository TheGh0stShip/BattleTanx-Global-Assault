float func_80079AFC(float x, int mode) {
    float r;

    switch (mode) {
    case 2:
        r = x * x;
        break;
    case 1:
        r = (2.0f - x) * x;
        break;
    default:
        r = x;
        break;
    }
    return r;
}

static inline float curveModeApply(float x, int mode) {
    float r;

    switch (mode) {
    case 2:
        r = x * x;
        break;
    case 1:
        r = (2.0f - x) * x;
        break;
    default:
        r = x;
        break;
    }
    return r;
}

void func_80079B34(int unused, int n, float x, int mode, int *a, int *b, int *frac) {
    float f;
    int i;

    if (1.0f <= x) {
        *a = n - 2;
        *b = n - 1;
        *frac = 255;
        return;
    }
    x = curveModeApply(x, mode);
    f = n - 1;
    i = x * f;
    *frac = (x - i / f) * f * 256.0f;
    *a = i;
    *b = i + 1;
}
