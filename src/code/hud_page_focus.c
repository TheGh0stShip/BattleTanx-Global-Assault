typedef struct { char p0[0x18]; void *a18; char p1c[0x31-0x1C]; unsigned char b31; char p32[0x38-0x32]; void *a38; char p3c[0x41-0x3C]; unsigned char b41; } Obj;
typedef struct { int p0; Obj *obj; int p8; void *cbC; void *cb10; } Task;
extern char D_8011D408[], D_8011D424[], D_8011D9F0[], D_80118EE0[];
extern unsigned short D_8011DC80;
int func_800C6544(Task *t) {
    Obj *o = t->obj;
    t->cbC = D_8011D408;
    t->cb10 = D_8011D424;
    o->b31 = 0x90;
    if (o->a18 == D_8011D9F0 && o->a38 == D_80118EE0) o->b41 = 1;
    else o->b41 = 16;
    D_8011DC80++;
    return 0;
}
