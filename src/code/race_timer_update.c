typedef struct { unsigned int w0, w1; } Gfx;
typedef struct { short x; short y; unsigned short flags; unsigned char a; unsigned char pad; } Ent;
extern float D_8011F214;
extern float D_803A5948;
extern float D_80073D70, D_80073D74, D_80073D78, D_80073D7C;
extern signed char D_80117EB0;
extern char D_8011E0E4[], D_8011E384[], D_8011E454[], D_8011E2B4[];
extern unsigned char D_8011F210[];
extern unsigned char D_8011F21C[][3];
extern Ent D_803A5990[][32];
extern char D_80117698[], D_801176B4[], D_801176D0[];
extern void func_8007BDFC(Gfx **, int);
extern void func_8007C9B8(Gfx **, void *, int, int, float, float);

void func_800C8484(Gfx **gp, void *a1) {
    Gfx *g;
    void *tex = 0;
    unsigned short idx;
    unsigned short s;
    unsigned char i;
    float t;

    t = D_8011F214 - D_803A5948;
    D_8011F214 = t;
    if (t < D_80073D70) {
        D_8011F214 = t + D_80073D74;
    }
    switch (D_80117EB0) {
    case 2:
        idx = a1 != D_8011E0E4;
        break;
    case 3:
        if (a1 == D_8011E384) idx = 0;
        else if (a1 == D_8011E454) idx = 1;
        else idx = 2;
        break;
    case 4:
        if (a1 == D_8011E2B4) idx = 0;
        else if (a1 == D_8011E384) idx = 1;
        else if (a1 == D_8011E454) idx = 2;
        else idx = 3;
        break;
    case 1:
    default:
        idx = 0;
        break;
    }
    s = idx;
    if (D_8011F210[s] != 0) {
        g = *gp;
        func_8007BDFC(&g, 2);
        for (i = 0; i < D_8011F210[s]; i++) {
            unsigned short f = D_803A5990[s][i].flags;
            unsigned char r, gg, bb;
            if ((f & 0x100) && D_8011F214 > 0.0f) continue;
            r = D_8011F21C[f & 0xF][0];
            gg = D_8011F21C[f & 0xF][1];
            bb = D_8011F21C[f & 0xF][2];
            switch (f & 0xF0) {
            case 0: tex = D_80117698; break;
            case 0x10: tex = D_801176B4; break;
            case 0x20: tex = D_801176D0; break;
            }
            {
                Gfx *p = g++;
                p->w0 = 0xFA000000;
                p->w1 = (r << 24) | (gg << 16) | (bb << 8) | D_803A5990[s][i].a;
            }
            func_8007C9B8(&g, tex, D_803A5990[s][i].x, D_803A5990[s][i].y, D_80073D78, D_80073D7C);
        }
        *gp = g;
    }
}
