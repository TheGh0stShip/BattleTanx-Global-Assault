typedef struct { int a, b; void *c; } S;
extern signed char D_80117EB0;
extern S D_8012074C;
extern S D_8012064C;
extern char D_8012019C;
extern int D_802195D8;
extern int D_802195C8;
extern int func_8009D144(void);
extern void func_8009C31C(void);
void func_800CF198(void) {
    S *s;
    if (func_8009D144() != 0) {
        s = &D_8012074C;
        if (D_80117EB0 == 1) s = &D_8012064C;
        if (s->c == &D_8012019C) { func_8009C31C(); return; }
    }
    D_802195D8 = 0;
    D_802195C8 = 2;
}
