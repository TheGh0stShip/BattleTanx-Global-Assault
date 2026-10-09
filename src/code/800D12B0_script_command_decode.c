/* Normalizer-assisted: retail assigns the opcode and pair accumulator registers oppositely. */
/* RODATA_VRAM 0x80074208 */
typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef float f32;
extern u8 D_803A69E2, D_803A6A04, D_803A66BC, D_803A7030, D_803A7032, D_803A7031;
extern u8 D_803A7022, D_803A7024, D_803A7023, D_803A6A00, D_803A6FC8, D_803A6672, D_803A7025, D_803A666C;
extern s16 D_803A6A06, D_803A6A02, D_803A69E0, D_803A666E, D_803A6670, D_803A66BE;
extern f32 D_803A69E8, D_803A69F4, D_803A69EC, D_803A69F8, D_803A7028, D_803A6FD4;
extern u8 *D_803A7018;

#define DECODE(v, sel, k1, k2, k3) \
    switch (sel) { \
    case k1: v = *p++; if (v & 0x80) v |= 0xFFFFFF00; break; \
    case k2: v = *p++ << 8; v += *p++; if (v & 0x8000) v |= 0xFFFF0000; break; \
    case k3: v = *p++ << 16; v += *p++ << 8; v += *p++; if (v & 0x800000) v |= 0xFF000000; break; \
    default: v = 0; break; \
    }

#define S24(v) v = *p++ << 16; v += *p++ << 8; v += *p++; if (v & 0x800000) v |= 0xFF000000;

u16 func_800D12B0(u8 *start, s32 arg) {
    u8 *p = start;
    u8 op;
    s32 mode;
    s32 v;
    s32 hi;
    s32 w;

    D_803A69E2 = 0;
    op = *p;
    D_803A6A04 = op;
    switch (op) {
    case 0:
        break;
    case 1:
        p++;
        v = *p++ << 8;
        v += *p++;
        D_803A6A06 = v;
        break;
    case 2:
        p++;
        D_803A6A06 = 1;
        break;
    case 3: case 4:
        p++;
        mode = *p++;
        DECODE(v, mode & 3, 1, 2, 3)
        if (v != 0) D_803A69E8 = (f32)v / 256.0f; else D_803A69E8 = 0;
        DECODE(v, mode & 0xC, 4, 8, 12)
        if (v != 0) D_803A69F4 = (f32)v / 256.0f; else D_803A69F4 = 0;
        DECODE(v, mode & 0x30, 16, 32, 48)
        if (v != 0) D_803A69EC = (f32)v / 256.0f; else D_803A69EC = 0;
        if (mode & 0x40) {
            v = *p++; hi = *p++; D_803A69E2 = 1;
            v <<= 8; v += hi; D_803A6A02 = v;
        } else {
            D_803A6A02 = 0;
        }
        if (mode & 0x80) {
            v = *p++; hi = *p++; D_803A66BC = 1;
            v <<= 8; v += hi; D_803A69E0 = v;
        } else {
            D_803A66BC = 0;
        }
        break;
    case 5:
        p++;
        mode = *p++;
        if (mode & 3) {
            { s32 v = *p++ << 8; v += *p++;
            D_803A7030 = 1;
            D_803A69E8 = (f32)(s16)v / 16384.0f; }
        } else D_803A7030 = 0;
        if (mode & 0xC) {
            { s32 v = *p++ << 8; v += *p++;
            D_803A7032 = 1;
            D_803A69F4 = (f32)(s16)v / 16384.0f; }
        } else D_803A7032 = 0;
        if (mode & 0x30) {
            { s32 v = *p++ << 8; v += *p++;
            D_803A7031 = 1;
            D_803A69EC = (f32)(s16)v / 16384.0f; }
        } else D_803A7031 = 0;
        break;
    case 7:
        p++;
        mode = *p++;
        if (mode & 1) { s32 hi = *p++; D_803A7030 = 1; D_803A7022 = hi; }
        else { D_803A7022 = 0; D_803A7030 = 0; }
        if (mode & 4) { s32 hi = *p++; D_803A7032 = 1; D_803A7024 = hi; }
        else { D_803A7024 = 0; D_803A7032 = 0; }
        if (mode & 0x10) { s32 hi = *p++; D_803A7031 = 1; D_803A7023 = hi; }
        else { D_803A7023 = 0; D_803A7031 = 0; }
        break;
    case 6: case 35: case 43:
        p++;
        { s32 hi = *p++; s32 v = *p++; D_803A69F8 = (f32)(v | (hi << 8)) / 256.0f; }
        break;
    case 42:
        p++;
        { s32 hi = *p++; s32 v = *p++; D_803A69F8 = (f32)(v | (hi << 8)) / 16384.0f; }
        break;
    case 19:
        p++;
        S24(v)
        D_803A7028 = (f32)v / 256.0f;
        break;
    case 20:
        p++;
        S24(v)
        D_803A69E8 = (f32)v / 256.0f;
        S24(v)
        D_803A69F4 = (f32)v / 256.0f;
        break;
    case 8: case 12: case 16: case 26: case 28: case 31: case 32: case 37: case 41: case 47:
        p++;
        D_803A6A00 = *p++;
        break;
    case 9: case 10: case 11:
        p++;
        if ((arg & 0xFFFF) < 32) {
            if ((arg & 0xFFFF) >= 30) {
                D_803A6A00 = 0;
                break;
            }
        }
        D_803A6A00 = *p++;
        break;
    case 13: case 14: case 15: case 48: case 49:
        p++;
        D_803A6FC8 = *p++;
        D_803A6672 = *p++;
        D_803A7025 = *p++;
        break;
    case 25:
        p++;
        D_803A6FC8 = *p++;
        D_803A6672 = *p++;
        D_803A7025 = *p++;
        D_803A666C = *p++;
        break;
    case 17: case 18:
        p++;
        break;
    case 21:
        p++;
        D_803A7018 = p;
        while (*p++ != 0);
        break;
    case 22: case 23: case 33:
        p++;
        { s32 a; a = *p++; w = *p++; w |= a << 8; D_803A666E = w; }
        { s32 a; a = *p++; w = *p++; w |= a << 8; D_803A6670 = w; }
        if (D_803A6A04 == 23) {
            { s32 hi = *p++; s32 v = *p++; D_803A6FD4 = v | (hi << 8); }
        }
        break;
    case 24:
        p++;
        D_803A6FC8 = *p++;
        D_803A6672 = *p++;
        D_803A7025 = *p++;
        goto block27;
    case 27:
        p++;
    block27:
        D_803A666C = *p++;
        { s32 hi = *p++; s32 v = *p++; D_803A6FD4 = v | (hi << 8); }
        break;
    case 34: case 38: case 44:
        p++;
        { s32 a; a = *p++; hi = *p++; hi |= a << 8; D_803A66BE = hi; }
        break;
    case 40:
        p++;
        { s32 v = *p++ << 8; v += *p++;
        D_803A69E8 = (f32)(s16)v / 256.0f; }
        { s32 v = *p++ << 8; v += *p++;
        D_803A69EC = (f32)(s16)v / 256.0f; }
        break;
    case 29: case 30: case 36: case 39: case 45: case 46:
        p++;
        D_803A6A00 = 0;
        break;
    default:
        p = start + 1;
        break;
    }
    return p - start;
}
