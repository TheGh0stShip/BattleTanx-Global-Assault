typedef struct Obj { char pad0[4]; unsigned char *p; char pad8[4]; void *c; void *d; unsigned short h14; } Obj;
typedef struct { char pad[8]; int flags; } Info;
extern unsigned short D_8011DC74;
extern Obj D_8011CD88, D_8011CEF8, D_8011D068, D_8011D1C8;
extern char D_8011C388[], D_8011C3A4[], D_8011C3B8[], D_8011C3D4[];
Info *func_8009836C(int);
void func_800BF130(Obj *);
void func_800BF1A4(Obj *);
inline void func_800C434C(Obj *o) {
    Obj *n;
    unsigned short i;
    if (o->h14 != 0) return;
    n = o;
    i = 0;
    do {
        i++;
        if (n == &D_8011CD88) n = &D_8011CEF8;
        else if (n == &D_8011CEF8) n = &D_8011D068;
        else if (n == &D_8011D068) n = &D_8011D1C8;
        else n = &D_8011CD88;
        if (i >= 4) return;
    } while (n->h14 != 0);
    o->p[0x101] = 16;
    func_800BF130(o);
    n->p[0x101] = 0x90;
    func_800BF1A4(n);
}
int func_800C4418(Obj *a) {
    unsigned char *p = a->p;
    a->c = D_8011C388;
    a->d = D_8011C3A4;
    p[0x101] = 0x90;
    D_8011DC74++;
    return 0;
}
int func_800C4458(Obj *o) {
    if (func_8009836C(o->h14)->flags & 0x20) {
        func_800C434C(o);
        return 0;
    }
    o->c = D_8011C3B8;
    o->d = D_8011C3D4;
    D_8011DC74--;
    o->p[0x101] = 16;
    return 0;
}
