/* RODATA_VRAM 0x80071DD4 */
typedef int s32;
typedef float f32;

typedef struct {
    f32 x;
    f32 y;
} Vec2;

f32 func_8009D4B0(unsigned short angle);
f32 func_8009D510(unsigned short angle);

f32 func_800904C8(s32 angle, f32 *out) {
    Vec2 corners[4] = {
        {26.5f, 24.5f},
        {37.25f, 13.75f},
        {37.25f, -13.75f},
        {26.5f, -24.5f},
    };
    f32 maxX;
    f32 maxY;
    f32 sine;
    f32 cosine;
    f32 value;
    s32 i;

    maxX = 0.0f;
    maxY = 0.0f;
    sine = func_8009D4B0(angle);
    cosine = func_8009D510(angle);
    for (i = 0; i < 4; i++) {
        value = cosine * corners[i].y - sine * corners[i].x;
        if (!(value > 0.0f)) {
            value = -value;
        }
        if (maxX < value) {
            maxX = value;
        }
        value = sine * corners[i].y + cosine * corners[i].x;
        if (!(value > 0.0f)) {
            value = -value;
        }
        if (maxY < value) {
            maxY = value;
        }
    }
    *out = maxY / 37.25f;
    return maxX;
}
