/* ---- 0x800E9000/a.c ---- */
extern int D_8021945C; extern int D_8021949C[];
int func_800E918C(int);
int func_800E98F8(void) {
    if (D_8021945C < func_800E918C(D_8021949C[0])) {
        return ((func_800E918C(D_8021949C[0]) - D_8021945C + 29) / 30) * 100;
    }
    return 0;
}

