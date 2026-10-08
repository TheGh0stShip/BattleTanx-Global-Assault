extern unsigned int D_802194A0, D_8021949C; extern int D_802194B0;
extern char D_801177B0[], D_80117794[], D_80117708[], D_801176EC[], D_80117724[];
void *func_800C9680(void) {
    switch (D_802194A0) {
    case 0: case 8: return D_801177B0;
    case 2: return D_80117724;
    case 7:
        if (D_802194B0 == 0) return 0;
    case 13: case 14:
        switch (D_8021949C) {
        case 2: return D_80117794;
        case 4: return D_80117708;
        case 12: return D_801176EC;
        case 6: return D_80117724;
        }
        return 0;
    }
    return 0;
}
