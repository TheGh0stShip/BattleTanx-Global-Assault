typedef unsigned char u8;
typedef unsigned short u16;
extern unsigned int D_802195CC;
extern int D_802194A0;
extern int D_802194B0;
extern u8 *D_80235F10;
extern u8 D_8011F210[];
extern u8 *func_800A1A28(u8 *prev, int type);
extern int func_80096250(u8 *obj);
extern u16 func_800C877C(float *pos, int a1, u16 a2, int a3, u16 n, int flags);

void func_800C8B74(u8 *self, u16 player, u16 team) {
    float pos[3];
    u16 n = 0;
    u8 *o;
    u8 *p;
    u8 *t;
    int k; int f;

    switch (D_802195CC) {
    case 5: case 6: case 7:
        D_8011F210[(u16)player] = 0;
        return;
    }
    p = func_800A1A28(0, 4);
    while (p != 0) {
        t = *(u8 **)(p + 12);
        if (t != 0 && t != self) {
            k = t[148];
            if (k == (u16)team && func_80096250(t)) {
                pos[0] = *(float *)(t + 8);
                pos[2] = *(float *)(t + 16) + 70.0f;
                pos[1] = *(float *)(t + 12);
                f = *(int *)(t + 160) != 0; f = -f;
                if (func_800C877C(pos, k, player, t[149], n, f & 0x120)) n++;
            }
        }
        p = func_800A1A28(p, 4);
    }
    for (o = func_800A1A28(0, 20); o != 0; o = func_800A1A28(o, 20)) {
        int kk = o[28];
        if (kk == (u16)team) {
            pos[0] = *(float *)(o + 16);
            pos[2] = *(float *)(o + 24) + 70.0f;
            pos[1] = *(float *)(o + 20);
            if (func_800C877C(pos, kk, player, o[29], n, 16)) n++;
        }
    }
    for (o = func_800A1A28(0, 7); o != 0; o = func_800A1A28(o, 7)) {
        int kk;
        if (*(int *)(o + 16) != 2 && (kk = o[52]) == (u16)team) {
            pos[0] = *(float *)(o + 36);
            pos[2] = *(float *)(o + 44) + 70.0f;
            pos[1] = *(float *)(o + 40);
            if (func_800C877C(pos, kk, player, o[53], n, 32)) n++;
        }
    }
    if ((D_802194A0 == 7 && !(*(u16 *)(D_80235F10 + 4) < D_802194B0)) || D_802194A0 == 14) {
        for (o = func_800A1A28(0, 19); o != 0; o = func_800A1A28(o, 19)) {
            int kk = o[28];
        if (kk == (u16)team) {
                pos[0] = *(float *)(o + 16);
                pos[2] = *(float *)(o + 24) + 70.0f;
                pos[1] = *(float *)(o + 20);
                if (func_800C877C(pos, kk, player, 0, n, 16)) n++;
            }
        }
    }
    D_8011F210[(u16)player] = n;
}
