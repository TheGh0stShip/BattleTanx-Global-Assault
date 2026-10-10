/* SPAN 0x800AE8EC */
typedef unsigned char u8;

typedef struct {
    void *a;
    void *b;
    void *c;
} Part;

typedef struct {
    void *world;        /* 0x80219498 */
    int track;          /* 0x8021949C */
    unsigned int mode;  /* 0x802194A0 */
    u8 nHuman;          /* 0x802194A4 */
    u8 nPlayers;        /* 0x802194A5 */
} GameState;

extern GameState D_80219498;

void *func_8007B0E4(void *cmds, int n, void *c);
void func_8007B1F0(void *a, void *c, void *b, void *arg1, int a5, void *o, void *t, int view, int a4, int z);

void func_800AE710(Part *pt, void *arg1, void *o, int a3, int a4, u8 mask) {
    int n = D_80219498.nPlayers;
    int v;

    if (pt != 0) {
        if (mask == 0) {
            return;
        }
        for (v = 0; v < n; v++) {
            if ((mask >> v) & 1) {
                func_8007B1F0(pt->a, pt->c, pt->b, arg1, a4, o, 0, v, a3, 0);
            }
        }
    }
}

void func_800AE7F0(Part *pt, void *arg1, void *o, int a3, int a4, u8 mask, void *cmds, int ncmds) {
    int n = D_80219498.nPlayers;
    void *c;
    int v;

    if (pt != 0) {
        if (mask == 0) {
            return;
        }
        c = func_8007B0E4(cmds, ncmds, pt->c);
        for (v = 0; v < n; v++) {
            if ((mask >> v) & 1) {
                func_8007B1F0(pt->a, c, pt->b, arg1, a4, o, 0, v, a3, 1);
            }
        }
    }
}
