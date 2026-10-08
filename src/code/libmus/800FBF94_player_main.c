/* RODATA_VRAM 0x80077608: literal pool linked at its retail address.
   RODATA_TRIM 0x8 8: PROPOSED - retail .rodata for this unit is 8-byte aligned. */
#include "mus_channel.h"

typedef struct {
    unsigned char type;
    int value;
} mus_msg_t;

extern int D_803AD9B4;
extern int D_803AD9B8;
extern int D_803AD9BC;
extern mus_msg_t *D_803AD9C0;
extern char *D_803AD978;
extern int D_803AD988;
extern void func_800FFA1C(void *dst, void *src, int size);
extern void func_800FBF14(mus_msg_t *msg);
extern void func_800FC864(channel_t *cp, int i);
extern void func_800FC31C(channel_t *cp, int i);
extern void func_800FCF34(channel_t *cp);
extern void func_800FCFF8(channel_t *cp);
extern void func_801022D0(void *voice);
extern void func_800FCBB0(channel_t *cp);
extern void func_800FCDC0(channel_t *cp);
extern float func_800FCED0(channel_t *cp);
extern float func_800FCE78(channel_t *cp);
extern void func_800FCA34(channel_t *cp, int i, float freq);
extern void func_800FC8E0(channel_t *cp, int i);

int player_text_1AE0(mus_msg_t *msg)
{
    int next;

    next = (D_803AD9B8 + 1) % D_803AD9BC;
    if (next == D_803AD9B4)
        return 0;
    func_800FFA1C(&D_803AD9C0[D_803AD9B8], msg, sizeof(mus_msg_t));
    D_803AD9B8 = next;
    return 1;
}

int func_800FC02C(void)
{
    int i;
    channel_t *cp;
    float freq;

    while (D_803AD9B4 != D_803AD9B8) {
        func_800FBF14(&D_803AD9C0[D_803AD9B4]);
        D_803AD9B4++;
        if (D_803AD9B4 == D_803AD9BC)
            D_803AD9B4 = 0;
    }
    for (i = -4, cp = D_803AD97C; i < D_803AD974 - 4; i++, cp++) {
        if (cp->pdata == 0 || (cp->flags & 1))
            continue;
        if (cp->f08)
            func_800FC864(cp, i);
        cp->f0C += cp->volume;
        if (cp->f9A != 0x7FFF) {
            while (cp->f3C - cp->f0C < 0) {
                if (cp->pdata == 0)
                    goto next;
                func_800FC31C(cp, i);
            }
            if (cp->pdata == 0)
                continue;
        }
        if (cp->f38 && cp->f14 - cp->f0C < 0)
            func_800FCF34(cp);
        if (cp->f34 && cp->f18 - cp->f0C < 0)
            func_800FCFF8(cp);
        if (cp->stopping != -1 && --cp->stopping == -1) {
            cp->f38 = 0;
            cp->f34 = 0;
            cp->song_bank = 0;
            cp->sample_bank = 0;
            cp->handle = 0;
            cp->f08 = 0;
            cp->pdata = 0;
            if (cp->fC9) {
                cp->fC9 = 0;
                func_801022D0(D_803AD978 + i * 0x1C);
            }
        }
        if (cp->fC9) {
            if (cp->fC3)
                func_800FCBB0(cp);
            if (cp->fD4 && cp->f94 - cp->f0C < 0)
                func_800FCDC0(cp);
            freq = cp->freqoffset;
            if (cp->fD5)
                freq += func_800FCED0(cp);
            if (cp->fCE)
                freq += func_800FCE78(cp);
            if (cp->f08 == 0) {
                func_800FCA34(cp, i, freq);
                func_800FC8E0(cp, i);
            }
        }
        cp->fAA = (unsigned int)(cp->f0C - cp->f40) >> 8;
    next:;
    }
    return D_803AD988;
}

typedef unsigned char *(*mus_command_t)(channel_t *cp, unsigned char *p);

typedef struct {
    unsigned short wave;       /* 0x00 */
    unsigned short env;        /* 0x02 */
    unsigned char pan;         /* 0x04 */
    unsigned char note;        /* 0x05 */
} drum_t;

typedef struct {
    unsigned char pad00[0x18];
    unsigned char *env;        /* 0x18: 7-byte envelope records */
    unsigned char pad1C[4];
    unsigned short *wave_map;  /* 0x20 */
} song_t;

typedef struct {
    unsigned char pad00[0x14];
    unsigned short *wave_map;  /* 0x14 */
} sample_bank_t;

typedef struct {
    unsigned char pad00[0x28];
    float *detune;             /* 0x28 */
    void **waves;              /* 0x2C */
} ptr_bank_t;

extern mus_command_t D_80126590[];
extern void func_800FCB40(channel_t *cp);
extern void __MusIntInitSweep(channel_t *cp);
extern void func_80102190(void *voice, short volume, int time);
extern void func_80101FD0(void *voice, unsigned char pan);

#define SIGNED8(x) (((x) & 0x80) ? ((x) & 0xFF) - 0x100 : ((x) & 0xFF))

static inline unsigned char *func_envelope(channel_t *cp, unsigned char *p)
{
    int a;
    int b;

    a = *p++;
    if (a == 0)
        a = 1;
    cp->fBF = a;
    cp->f64 = 0x400 / a;
    cp->fC0 = *p++;
    a = *p++;
    cp->fC6 = a;
    b = *p++;
    cp->fC1 = b;
    cp->f58 = 1.0 / (float)a * (float)(b - cp->fC0);
    a = *p++;
    cp->fC7 = a;
    b = *p++;
    cp->fC2 = b;
    cp->f5C = 1.0 / (float)a * (float)(b - cp->fC1);
    a = *p++;
    cp->fC8 = a;
    cp->f60 = 1.0 / (float)a;
    return p;
}

void func_800FC31C(channel_t *cp, int x)
{
    unsigned char *p;
    unsigned char c;
    unsigned char vel;
    int note;
    int wave;
    ptr_bank_t *pbank;

    p = cp->pdata;
    if (p) {
    loop:
        c = *p;
        if (c >= 0x80) {
            p = D_80126590[c & 0x7F](cp, p + 1);
            if (p)
                goto loop;
        }
    }
    cp->pdata = p;
    if (p) {

    cp->f4C = cp->f50;
    note = *cp->pdata++;
    if (cp->fD2) {
        vel = *cp->pdata++;
        cp->fBB = vel;
        if (vel >= 0x80) {
            cp->fBB = vel & 0x7F;
            cp->fD2 = 0;
            cp->fD3 = cp->fBB;
        }
    } else {
        cp->fBB = cp->fD3;
    }

    if (cp->fAC) {
        if (cp->fB7) {
            cp->fB7 = 0;
            c = *cp->pdata++;
            if (c < 0x80)
                cp->f9A = c;
            else
                cp->f9A = ((c & 0x7F) << 8) + *cp->pdata++;
        } else {
            cp->f9A = cp->fAC;
        }
    } else {
        c = *cp->pdata++;
        if (c < 0x80)
            cp->f9A = c;
        else
            cp->f9A = ((c & 0x7F) << 8) + *cp->pdata++;
    }

    cp->f40 = cp->f3C;
    cp->f3C += cp->f9A << 8;
    cp->fAA = 0;
    cp->fD1 = 0;
    cp->fD0 = cp->fCF;
    if (cp->song_bank && !cp->f84) {
        if (((song_t *)cp->song_bank)->wave_map[cp->fAE] == 0xFFFF)
            note = 0x60;
    }

    if (note != 0x60) {
    pbank = cp->f7C;
    if (cp->f84) {
        cp->fAE = ((drum_t *)cp->f84)[note].wave;
        cp->fBD = ((drum_t *)cp->f84)[note].pan >> 1;
        func_envelope(cp, &((song_t *)cp->song_bank)->env[((drum_t *)cp->f84)[note].env * 7]);
        note = ((drum_t *)cp->f84)[note].note;
    }
    if (!cp->fD6)
        func_800FCB40(cp);
    if (cp->fD4)
        __MusIntInitSweep(cp);
    wave = cp->fAE;
    if (cp->song_bank)
        wave = ((song_t *)cp->song_bank)->wave_map[wave];
    else
        wave = ((sample_bank_t *)cp->sample_bank)->wave_map[wave];
    if (!cp->fD7) {
        cp->f08 = pbank->waves[wave];
        if (cp->fC9 && cp->fA0) {
            cp->fA0 = 0;
            func_80102190(D_803AD978 + x * 0x1C, 0, D_803AD988);
        } else {
            func_800FC864(cp, x);
        }
    }
    cp->f2C = (float)note + pbank->detune[wave];
    c = cp->fB9 * (1 - cp->fBA);
    cp->f2C += SIGNED8(c);
    c = cp->fCA;
    if (c != cp->pan_dirty) {
        cp->pan_dirty = c;
        func_80101FD0(D_803AD978 + x * 0x1C, cp->pan + ((0x80 - cp->pan) * c >> 7));
    }
    } else {
        if (cp->fC3 < 4) {
            cp->fC3 = 4;
            cp->fC5 = 1;
            cp->f54 = cp->f0C;
            cp->fCD = cp->fC4;
        }
    }
    } else {
        if (cp->fC9) {
            cp->fC9 = 0;
            func_80102190(D_803AD978 + x * 0x1C, 0, D_803AD988);
            func_801022D0(D_803AD978 + x * 0x1C);
        }
    }
}
