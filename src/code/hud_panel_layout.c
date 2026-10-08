extern char D_8011DAE4[], D_8011DB54[], D_8011DBC4[];
extern char D_8011DA1C[], D_8011DA28[], D_8011DA34[], D_8011DA40[], D_8011DA4C[], D_8011DA58[], D_8011DA64[];
extern int D_80117EB4;
extern int D_80117ED4[];
extern int D_80117F04[];
void func_800C48F0(void);

typedef struct { int pad0; int pad4; char *str; } Obj;

int func_800C5D80(void *a0, Obj *a1) {
    unsigned short i, j, found;

    if (a0 == D_8011DAE4) {
        i = 0;
    } else if (a0 == D_8011DB54) {
        i = 1;
    } else {
        i = (a0 == D_8011DBC4) ? 2 : 3;
    }
    if (D_80117EB4 == 8) {
        if (D_80117ED4[i] == 5) D_80117ED4[i] = 6; else D_80117ED4[i] = 5;
    } else {
        if (--D_80117ED4[i] == 0) D_80117ED4[i] = 4;
    }
    found = 0;
    for (j = 0; j < 4; j++) {
        if (j != i && (unsigned)(D_80117F04[j] - 1) < 2 && D_80117ED4[j] != D_80117ED4[i]) {
            found = 1;
            break;
        }
    }
    if (!found) {
        if (D_80117EB4 == 8) {
            if (D_80117ED4[i] == 5) D_80117ED4[i] = 6; else D_80117ED4[i] = 5;
        } else {
            if (--D_80117ED4[i] == 0) D_80117ED4[i] = 4;
        }
    }
    switch (D_80117ED4[i]) {
    case 1: a1->str = D_8011DA28; break;
    case 2: a1->str = D_8011DA34; break;
    case 3: a1->str = D_8011DA1C; break;
    case 4: a1->str = D_8011DA40; break;
    case 6: a1->str = D_8011DA4C; break;
    case 5: a1->str = D_8011DA58; break;
    case 0:
    default: a1->str = D_8011DA64; break;
    }
    func_800C48F0();
    return 0;
}
