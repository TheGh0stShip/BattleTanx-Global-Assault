typedef struct { void *pad; unsigned char *child; void *p08; void *p0c; void *p10; unsigned short f14; char rest[0x70 - 0x16]; } Obj;
extern Obj D_8011DAE4, D_8011DB54, D_8011DBC4, D_8011DC34;
extern char D_8011D408[], D_8011D424[], D_8011D9F0[], D_80118EE0[];
void func_800BF130(Obj *);
void func_800BF1A4(Obj *);

void func_800C4E24(Obj *o) {
    Obj *p;
    unsigned short n;
    unsigned char *c;
    if (o->f14 != 0) return;
    p = o;
    n = 0;
    do {
        n++;
        if (p == &D_8011DAE4) p = &D_8011DB54;
        else if (p == &D_8011DB54) p = &D_8011DBC4;
        else if (p == &D_8011DBC4) p = &D_8011DC34;
        else p = &D_8011DAE4;
    } while (n < 3 && p->f14 != 0);
    if (n < 4) {
        c = o->child;
        o->p0c = D_8011D408;
        o->p10 = D_8011D424;
        c[49] = 1;
        c[65] = 1;
        func_800BF130(o);
        c = p->child;
        c[49] = 144;
        if (*(void **)(c + 24) == D_8011D9F0 && *(void **)(c + 56) == D_80118EE0) c[65] = 1;
        else c[65] = 16;
        func_800BF1A4(p);
    }
}
