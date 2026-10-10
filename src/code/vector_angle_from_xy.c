typedef unsigned short u16;

float sqrtf(float x);
u16 func_8009D578(float x);

u16 func_800B8310(float x, float y) {
    u16 neg;
    unsigned int a;

    if (x < 0.0f) {
        neg = 1;
        x = -x;
    } else {
        neg = 0;
    }
    a = func_8009D578(x / sqrtf(x * x + y * y)) >> 1;
    return neg ? a ^ 0xFFFF : a;
}
