/* SPAN 0x800AAC14 */
/* RODATA_VRAM 0x80072DC0 */
typedef unsigned char u8;

typedef struct {
    float dist;
    int id;
} Nearest;

typedef struct {
    void *world;        /* 0x80219498 */
    int track;          /* 0x8021949C */
    unsigned int mode;  /* 0x802194A0 */
    u8 nHuman;          /* 0x802194A4 */
    u8 nPlayers;        /* 0x802194A5 */
} GameState;

extern GameState D_80219498;
extern Nearest D_80237180[][3][100];

void func_800AAAE0(void) {
    int p;
    int l;
    int i;

    for (p = 0; p < D_80219498.nPlayers; p++) {
        for (l = 0; l < 3; l++) {
            for (i = 0; i < 100; i++) {
                D_80237180[p][l][i].dist = 100000000.0f;
            }
        }
    }
}

void func_800AAB68(int id, float d, Nearest *n) {
    if (d < n->dist) {
        n->dist = d;
        n->id = id;
    }
}

void func_800AAB90(int id, float d, int p, int i, int layer) {
    int l;

    for (l = layer - 1; l >= 0; l--) {
        Nearest *n = &D_80237180[p][l][i];

        if (!(d < n->dist)) {
            break;
        }
        n->dist = d;
        n->id = id;
    }
}
