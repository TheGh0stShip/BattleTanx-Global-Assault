typedef struct {
    char pad0[0x1E0];
    int flags;
    char pad1[0x224 - 0x1E4];
    int f224;
    int f228;
} Obj;
typedef struct {
    char pad0[8];
    short next;
    short pad;
    Obj *obj;
    char pad1[0x44 - 0x10];
} Node;
extern int D_802195C8;
extern short D_80224E70;
extern int D_8021945C;
extern Node D_80224EF0[];
extern short D_8011F1F8, D_8011F1FA, D_8011F1FC, D_8011F1FE, D_8011F200;
extern short D_8011F202, D_8011F204, D_8011F206, D_8011F208;
extern void func_80098534(void);

int func_800C73A8(void) {
    short *p;
    Obj *obj;
    Node *n;

    D_802195C8 = 3;
    func_80098534();
    p = &D_80224E70;
    while (*p != -1) {
        n = &D_80224EF0[*p];
        obj = n->obj;
        if (obj != 0 && (obj->flags & 2)) {
            obj->f224 = D_8021945C;
            obj->f228 = D_8021945C;
        }
        p = &D_80224EF0[*p].next;
    }
    D_8011F1F8 = 0;
    D_8011F1FA = 0;
    D_8011F1FC = 0;
    D_8011F1FE = 0;
    D_8011F208 = 0;
    D_8011F200 = 0;
    D_8011F202 = 0;
    D_8011F204 = 0;
    D_8011F206 = 0;
    return 3;
}
