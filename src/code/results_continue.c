typedef struct { int a, b; void *c; } S;
extern signed char D_80117EB0;
extern S D_8012072C, D_8012062C;
extern char D_801201A8;
extern int D_8021949C;
extern char D_802195B9;
extern int D_802195D4, D_802195C8;
extern int func_8009D144(void);
extern void func_8009C31C(void);
extern int *func_800E9424(void);
extern void func_800CD730(int, int, int);
void func_800CF0CC(void) {
    S *s;
    if (func_8009D144() == 0) goto end;
    s = &D_8012072C;
    if (D_80117EB0 == 1) s = &D_8012062C;
    if (s->c != &D_801201A8) goto end;
    func_8009C31C();
    if (D_8021949C != 16) {
        func_800CD730(2, 1, (*func_800E9424() == 0 ? -1 : 0) & 7 | 3);
    }
    return;
end:
    D_802195B9 = 1;
    D_802195D4 = -1;
    D_802195C8 = 3;
}
