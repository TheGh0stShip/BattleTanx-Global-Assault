typedef struct { char pad[0x18]; void *f18; } Inner;
typedef struct { char pad[4]; Inner *f4; } ArgA;
typedef struct { unsigned char b0; unsigned char f1; char pad[6]; void *f8; } Sub;
typedef struct {
    char pad0[8];
    void *f8;
    char pad1[0x10 - 0xC];
    Sub sub;
} ArgB;
extern signed char D_80117EB0;
extern unsigned int D_80117ED4[];
extern unsigned int D_80117EF4[];
extern int D_80117F04[];
extern char D_8011DAE4[], D_8011DB54[], D_8011DBC4[], D_8011D9F0[];
extern char D_80118EE0[], D_80118EDC[];
extern char D_8011DA28[], D_8011DA34[], D_8011DA1C[], D_8011DA40[];
extern char D_8011DA4C[], D_8011DA58[], D_8011DA64[];
extern char D_8011DA10[], D_8011DA04[], D_8011D9FC[];
extern void func_800C48F0();

int func_800C6280(ArgA *a0, ArgB *a1) {
    Inner *in = a0->f4;
    int idx;
    unsigned short count;
    unsigned short i;
    unsigned int *p;
    unsigned int t;
    Sub *sp;
    int v;

    if (a0 == (ArgA *)D_8011DAE4) {
        idx = 0;
    } else if (a0 == (ArgA *)D_8011DB54) {
        idx = 1;
    } else {
        idx = (a0 != (ArgA *)D_8011DBC4) ? 3 : 2;
    }
    if (in->f18 == D_8011D9F0) {
        if (a1->f8 == D_80118EE0) {
            a1->f8 = D_80118EDC;
            D_80117F04[idx] = 1;
            t = D_80117ED4[0];
            D_80117ED4[idx] = t;
            sp = &a1->sub;
            switch (t) {
            case 1: sp->f8 = D_8011DA28; break;
            case 2: sp->f8 = D_8011DA34; break;
            case 3: sp->f8 = D_8011DA1C; break;
            case 4: sp->f8 = D_8011DA40; break;
            case 6: sp->f8 = D_8011DA4C; break;
            case 5: sp->f8 = D_8011DA58; break;
            case 0: goto def;
            default: def: sp->f8 = D_8011DA64; break;
            }
            a1->sub.f1 = 16;
            func_800C48F0();
        } else {
            if (D_80117EB0 == 1) {
                count = 0;
                for (i = 1; i < 4; i++) {
                    if (D_80117F04[i] == 1 && D_80117ED4[i] != D_80117ED4[0]) {
                        count++;
                    }
                }
            } else {
                count = 4;
            }
            if (count < 2 && D_80117ED4[idx] != D_80117ED4[0]) {
                return 0;
            }
            a1->f8 = D_80118EE0;
            D_80117F04[idx] = 0;
            D_80117ED4[idx] = 0;
            a1->sub.f8 = D_8011DA64;
            a1->sub.f1 = 1;
            func_800C48F0();
        }
    } else {
        if (--D_80117EF4[idx] == 0) {
            D_80117EF4[idx] = 3;
        }
        p = &D_80117EF4[idx];
        switch (*p) {
        case 2: a1->f8 = D_8011D9FC; break;
        case 1: a1->f8 = D_8011DA10; break;
        case 3: a1->f8 = D_8011DA04; break;
        default: a1->f8 = D_8011D9FC; break;
        }
    }
    return 0;
}
