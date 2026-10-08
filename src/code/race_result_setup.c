typedef struct { char pad[8]; void *link; } Item;
extern int D_8021949C;
extern int D_80117EB4;
extern char D_8011E634[];
extern char D_8011E578[];
extern short D_8011F1F8, D_8011F1FA, D_8011F1FC, D_8011F1FE, D_8011F200;
extern short D_8011F202, D_8011F204, D_8011F206, D_8011F208;
extern void func_80098BF8(void);
extern int func_800E8C88(int);
extern int func_8009D144(void);
extern int func_800E8E34(int);
extern int func_800E8F90(int);
extern void func_800CAA48(int, int, float, int);
extern void func_800BEBA8(void *, int, int);
extern Item *func_800BD880(void *);
extern void func_800BDA30(void *);

void func_800C726C(void) {
    int a;
    int b;
    int *p = &D_8021949C;

    func_80098BF8();
    a = func_800E8C88(*p);
    if (func_8009D144() != 0) {
        b = func_800E8E34(*p);
    } else {
        b = func_800E8F90(D_80117EB4);
    }
    if (b != 0 || a != 0) {
        func_800CAA48(a, b, 1.0e10f, 1);
    }
    func_800BEBA8(D_8011E634, 0, 1);
    if (func_800BD880(D_8011E634)->link != D_8011E578) {
        do {
            func_800BDA30(D_8011E634);
        } while (func_800BD880(D_8011E634)->link != D_8011E578);
    }
    D_8011F1F8 = 1;
    D_8011F1FA = 1;
    D_8011F1FC = 1;
    D_8011F1FE = 1;
    D_8011F208 = 1;
    D_8011F200 = 1;
    D_8011F202 = 1;
    D_8011F204 = 1;
    D_8011F206 = 1;
}
