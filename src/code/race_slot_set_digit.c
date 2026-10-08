typedef struct { void **p; int pad; } S8;
typedef struct { int a; struct { char pad[0x1C]; void **q; } *b; char pad[0x38]; } S40;
extern S8 D_8011E6B8[];
extern S40 D_8011E75C[];
extern char D_8011E678[4][0x10];
extern void func_800BEBA8(void *, int, int);

void func_800CA4A0(char *arg0, int arg1, unsigned short arg2)
{
    S8 *v;
    S40 *o;
    void *b;

    switch (arg1) {
    case 1: v = &D_8011E6B8[0]; o = &D_8011E75C[1]; break;
    case 2: v = &D_8011E6B8[1]; o = &D_8011E75C[2]; break;
    case 3: v = &D_8011E6B8[2]; o = &D_8011E75C[3]; break;
    case 4: v = &D_8011E6B8[3]; o = &D_8011E75C[4]; break;
    case 5: v = &D_8011E6B8[4]; o = &D_8011E75C[5]; break;
    case 6: v = &D_8011E6B8[5]; o = &D_8011E75C[6]; break;
    case 0: default: v = &D_8011E6B8[0]; o = &D_8011E75C[0]; break;
    }
    v->p = (void *)(arg0 + 20);
    b = o->b;
    switch (arg2) {
    case 0: *((void ***)b)[7] = D_8011E678[0]; break;
    case 1: *((void ***)b)[7] = D_8011E678[1]; break;
    case 2: *((void ***)b)[7] = D_8011E678[2]; break;
    case 3: *((void ***)b)[7] = D_8011E678[3]; break;
    }
    func_800BEBA8(o, 0, 1);
}
