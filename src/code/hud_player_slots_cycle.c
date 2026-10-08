typedef struct Obj { char pad0[4]; unsigned char *p; char pad8[4]; void *c; void *d; unsigned short h14; } Obj;
typedef struct { int a; int b; int c; } E12;
extern signed char D_80117EB0;
extern unsigned short D_8011DC74;
extern int D_80117F04[];
extern int D_80117F34[];
extern signed char D_8011DC70[];
extern E12 D_8011D1E4[];
extern Obj D_8011CD88, D_8011CEF8, D_8011D068, D_8011D1C8;
void func_800C3460(Obj *, int);
void func_800BEBA8(Obj *, void *, int);
void func_800C3B10(void);
int func_800C402C(void *arg) {
    unsigned short m;
    unsigned short i;
    Obj *o;
    unsigned int v;
    unsigned char *p;
    D_8011DC74 = D_80117EB0;
    for (i = 0; i < 4; i++) {
        v = D_80117F04[i];
        switch (v) {
        case 2: m = 0; break;
        case 1: m = 3; break;
        case 0:
        default: m = 100; break;
        }
        switch (i) {
        case 0: o = &D_8011CD88; break;
        case 1: o = &D_8011CEF8; break;
        case 2: o = &D_8011D068; break;
        default: o = &D_8011D1C8; break;
        }
        func_800C3460(o, i);
        D_80117F34[i] = D_8011D1E4[D_8011DC70[i]].a;
        p = o->p;
        if (m == 0) {
            p[0x101] = 0x90;
            o->h14 = i;
        } else if (m == 3) {
            p[0x101] = 16;
            o->h14 = 0;
        } else {
            o->h14 = 255;
            continue;
        }
        func_800BEBA8(o, arg, m);
    }
    func_800C3B10();
    return 1;
}
