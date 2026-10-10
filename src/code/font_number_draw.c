typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

s16 func_80096F48(void *ctx, u8 *text, u16 font, s16 x, s16 y, float sx, float sy);
s16 func_800973E0(u8 *s, u16 font, float scale);

s16 func_80096BDC(void *ctx, int val, u16 font, s16 x, s16 y, float sx, float sy, u16 align) {
    u8 buf[20];
    u8 *p = buf;
    int d;
    s16 off;

    if (val < 0) {
        *p++ = '-';
        val = -val;
    }
    d = 1000000000;
    while (val / d == 0) {
        d /= 10;
        if (d <= 0) {
            break;
        }
    }
    if (d == 0) {
        *p++ = '0';
    } else {
        while (d > 0) {
            *p++ = val / d + '0';
            val -= (s16)(val / d) * d;
            d /= 10;
        }
    }
    *p = 0;
    switch (align) {
    case 0:
        off = 0;
        break;
    case 1:
        off = func_800973E0(buf, font, sx);
        break;
    case 2:
        off = func_800973E0(buf, font, sx) >> 1;
        break;
    default:
        off = 0;
        break;
    }
    return func_80096F48(ctx, buf, font, x - off, y, sx, sy);
}
