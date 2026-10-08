typedef struct { int p0; int type; } Msg;
typedef struct { char pad[0x10]; int a; int b; } Arg;
extern void func_800E4530(void *, int, int);
void func_800E44C8(void *o, Msg *m, Arg *a, int *res) {
    switch (m->type) {
    case 11: case 35: case 37: case 38: case 50:
        *res = 1;
        func_800E4530(o, a->a, a->b);
        break;
    case 4: case 28:
        *res = 1;
        break;
    }
}
