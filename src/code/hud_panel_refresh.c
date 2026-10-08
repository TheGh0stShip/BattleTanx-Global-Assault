extern char D_8011DAE4[], D_8011DB54[], D_8011DBC4[];
extern char D_8011DA1C[], D_8011DA28[], D_8011DA34[], D_8011DA40[], D_8011DA4C[], D_8011DA58[], D_8011DA64[];
extern char D_8011D9F0[], D_80118EE0[], D_80118EDC[], D_8011DA10[], D_8011DA04[], D_8011D9FC[];
extern signed char D_80117EB0;
extern unsigned int D_80117ED4[];
extern unsigned int D_80117EF4[];
extern unsigned int D_80117F04[];
void func_800C48F0(void);

typedef struct { char p0; char flag; char p2[6]; char *str; } Sub;
typedef struct { int pad0; int pad4; char *str; int padC; Sub sub; } Obj;
typedef struct { char pad[0x18]; char *p18; } Inner;
typedef struct { int pad0; Inner *p4; } Src;

int func_800C5FB4(Src *a0, Obj *a1) {
    unsigned short i, j, cnt;
    Sub *sub;

    if ((char *)a0 == D_8011DAE4) {
        i = 0;
    } else if ((char *)a0 == D_8011DB54) {
        i = 1;
    } else {
        i = ((char *)a0 == D_8011DBC4) ? 2 : 3;
    }
    if (a0->p4->p18 == D_8011D9F0) {
        if (a1->str == D_80118EE0) {
            a1->str = D_80118EDC;
            D_80117F04[i] = 1;
            D_80117ED4[i] = D_80117ED4[0];
            sub = &a1->sub;
            switch (D_80117ED4[i]) {
            case 1: sub->str = D_8011DA28; break;
            case 2: sub->str = D_8011DA34; break;
            case 3: sub->str = D_8011DA1C; break;
            case 4: sub->str = D_8011DA40; break;
            case 6: sub->str = D_8011DA4C; break;
            case 5: sub->str = D_8011DA58; break;
            case 0:
            default: sub->str = D_8011DA64; break;
            }
            a1->sub.flag = 16;
            func_800C48F0();
        } else {
            if (D_80117EB0 == 1) {
                cnt = 0;
                for (j = 1; j < 4; j++) {
                    if (D_80117F04[j] == 1 && D_80117ED4[j] != D_80117ED4[0]) {
                        cnt++;
                    }
                }
            } else {
                cnt = 4;
            }
            if (cnt >= 2 || D_80117ED4[i] == D_80117ED4[0]) {
                a1->str = D_80118EE0;
                D_80117F04[i] = 0;
                D_80117ED4[i] = 0;
                a1->sub.str = D_8011DA64;
                a1->sub.flag = 1;
                func_800C48F0();
            }
        }
    } else {
        if (++D_80117EF4[i] >= 4) D_80117EF4[i] = 1;
        switch (D_80117EF4[i]) {
        case 1: a1->str = D_8011DA10; break;
        case 3: a1->str = D_8011DA04; break;
        case 2:
        default: a1->str = D_8011D9FC; break;
        }
    }
    return 0;
}
