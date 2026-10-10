/* RODATA_VRAM 0x80072E80 */
typedef struct {
    float dist;
    int id;
} Nearest;

extern Nearest D_80237180[][3][100];

int func_800ABF38(int x0, float d0, int x1, float d1, int layer, int p) {
    int dx = x1 - x0;
    int l = layer - 1;
    float step;
    int i;
    int id0;

    if (dx == 0) {
        if (d0 < D_80237180[p][l][x0].dist) {
            return 1;
        }
        return 0;
    }
    if (d0 < D_80237180[p][l][x0].dist) {
        return 1;
    }
    id0 = D_80237180[p][l][x0].id;
    if (d1 < D_80237180[p][l][x1].dist) {
        return 1;
    }
    if (id0 == D_80237180[p][l][x1].id) {
        return 0;
    }
    if (dx < 10) {
        if ((d0 + d1) / 2.0f < D_80237180[p][l][x0 + dx / 2].dist) {
            return 1;
        }
        return 0;
    }
    step = (d1 - d0) / dx;
    for (i = x0 + 1; i <= x1 - 1; i++) {
        d0 += step;
        if (d0 < D_80237180[p][l][i].dist) {
            return 1;
        }
    }
    return 0;
}
