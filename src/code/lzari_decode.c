#include "types.h"

#define N 4096
#define F 60
#define THRESHOLD 2
#define N_CHAR (256 - THRESHOLD + F)
#define Q1 (1U << 15)
#define Q2 (2 * Q1)
#define Q3 (3 * Q1)
#define Q4 (4 * Q1)
#define MAX_CUM (Q1 - 1)

extern u8 D_8021C508[];
extern u32 D_8021C500;
extern u32 D_8021D544;
extern s32 D_8021D548;
extern s32 D_8021D550[];
extern s32 D_8021DA40[];
extern u32 D_8021DF28[];
extern s32 D_8021E414;
extern u32 D_8021E418[];
extern u32 D_8022241C;
extern u8 *D_80222420;
extern u32 D_80222428[];
extern u32 D_80222914;
extern u32 D_80222918;
extern u32 D_8022291C;
extern u8 *D_80222920;

s32 func_800A0BA8(void);
s32 func_800A0E00(void);
void func_800A0918(void);
void func_800A0968(void);
void func_800A0A54(s32 sym);
s32 func_800A1040(u32 x);
s32 func_800A1090(u32 x);
s32 func_800A10E0(void);

s32 func_800A0750(s32 input_size, u8 *input, s32 output_size, u8 *output) {
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    s32 c;
    u32 count;

    D_8022241C = 0;
    D_80222918 = Q4;
    D_8021C500 = 0;
    D_8021D544 = 0;
    D_8022291C = 0;
    D_8021D548 = input_size;
    D_80222420 = input;
    D_8021E414 = output_size;
    D_80222920 = output;
    D_80222914 = *(u32 *)input;
    D_80222420 += 4;
    if (D_80222914 == 0 || D_80222914 > (u32)output_size)
        return -1;
    func_800A0918();
    func_800A0968();
    for (i = 0; i < N - F; i++)
        D_8021C508[i] = ' ';
    r = N - F;
    for (count = 0; count < D_80222914;) {
        c = func_800A0BA8();
        if (c < 256) {
            *D_80222920++ = c;
            D_8021C508[r++] = c;
            r &= N - 1;
            count++;
        } else {
            i = (r - func_800A0E00() - 1) & (N - 1);
            j = c - 255 + THRESHOLD;
            for (k = 0; k < j; k++) {
                c = D_8021C508[(i + k) & (N - 1)];
                *D_80222920++ = c;
                D_8021C508[r++] = c;
                r &= N - 1;
                count++;
            }
        }
    }
    return 0;
}
void func_800A0918(void) {
    s32 i;

    for (i = 0; i < 17; i++)
        D_8021C500 = 2 * D_8021C500 + func_800A10E0();
}

void func_800A0968(void) {
    s32 ch;
    s32 sym;
    s32 i;

    D_8021DF28[N_CHAR] = 0;
    for (sym = N_CHAR; sym >= 1; sym--) {
        ch = sym - 1;
        D_8021DA40[ch] = sym;
        D_8021D550[sym] = ch;
        D_80222428[sym] = 1;
        D_8021DF28[sym - 1] = D_8021DF28[sym] + D_80222428[sym];
    }
    D_80222428[0] = 0;
    D_8021E418[N] = 0;
    for (i = N; i >= 1; i--)
        D_8021E418[i - 1] = D_8021E418[i] + 10000 / (i + 200);
}

void func_800A0A54(s32 sym) {
    s32 i;
    s32 c;
    s32 ch_i;
    s32 ch_sym;

    if (D_8021DF28[0] >= MAX_CUM) {
        c = 0;
        for (i = N_CHAR; i > 0; i--) {
            D_8021DF28[i] = c;
            D_80222428[i] = (D_80222428[i] + 1) >> 1;
            c += D_80222428[i];
        }
        D_8021DF28[0] = c;
    }
    for (i = sym; D_80222428[i] == D_80222428[i - 1]; i--)
        ;
    if (i < sym) {
        ch_i = D_8021D550[i];
        ch_sym = D_8021D550[sym];
        D_8021D550[i] = ch_sym;
        D_8021D550[sym] = ch_i;
        D_8021DA40[ch_i] = sym;
        D_8021DA40[ch_sym] = i;
    }
    D_80222428[i]++;
    while (--i >= 0)
        D_8021DF28[i]++;
}

s32 func_800A0BA8(void) {
    s32 sym;
    s32 ch;
    u32 range;

    range = D_80222918 - D_8022241C;
    sym = func_800A1040(
        ((D_8021C500 - D_8022241C + 1) * D_8021DF28[0] - 1) / range);
    D_80222918 = D_8022241C +
        (range * D_8021DF28[sym - 1]) / D_8021DF28[0];
    D_8022241C += (range * D_8021DF28[sym]) / D_8021DF28[0];
    for (;;) {
        if (D_8022241C >= Q2) {
            D_8021C500 -= Q2;
            D_8022241C -= Q2;
            D_80222918 -= Q2;
        } else if (D_8022241C >= Q1 && D_80222918 <= Q3) {
            D_8021C500 -= Q1;
            D_8022241C -= Q1;
            D_80222918 -= Q1;
        } else if (D_80222918 > Q2) {
            break;
        }
        D_8022241C += D_8022241C;
        D_80222918 += D_80222918;
        D_8021C500 = 2 * D_8021C500 + func_800A10E0();
    }
    ch = D_8021D550[sym];
    func_800A0A54(sym);
    return ch;
}

s32 func_800A0E00(void) {
    s32 position;
    u32 range;

    range = D_80222918 - D_8022241C;
    position = func_800A1090(
        ((D_8021C500 - D_8022241C + 1) * D_8021E418[0] - 1) / range);
    D_80222918 = D_8022241C +
        (range * D_8021E418[position]) / D_8021E418[0];
    D_8022241C += (range * D_8021E418[position + 1]) / D_8021E418[0];
    for (;;) {
        if (D_8022241C >= Q2) {
            D_8021C500 -= Q2;
            D_8022241C -= Q2;
            D_80222918 -= Q2;
        } else if (D_8022241C >= Q1 && D_80222918 <= Q3) {
            D_8021C500 -= Q1;
            D_8022241C -= Q1;
            D_80222918 -= Q1;
        } else if (D_80222918 > Q2) {
            break;
        }
        D_8022241C += D_8022241C;
        D_80222918 += D_80222918;
        D_8021C500 = 2 * D_8021C500 + func_800A10E0();
    }
    return position;
}

s32 func_800A1040(u32 x) {
    s32 i;
    s32 j;
    s32 k;

    i = 1;
    j = N_CHAR;
    while (i < j) {
        k = (i + j) / 2;
        if (D_8021DF28[k] > x)
            i = k + 1;
        else
            j = k;
    }
    return i;
}

s32 func_800A1090(u32 x) {
    s32 i;
    s32 j;
    s32 k;

    i = 1;
    j = N;
    while (i < j) {
        k = (i + j) / 2;
        if (D_8021E418[k] > x)
            i = k + 1;
        else
            j = k;
    }
    return i - 1;
}

s32 func_800A10E0(void) {
    D_8022291C >>= 1;
    if (D_8022291C == 0) {
        D_8021D544 = *D_80222420++;
        D_8022291C = 0x80;
    }
    return (D_8021D544 & D_8022291C) != 0;
}
