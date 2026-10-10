typedef struct {
    float score;
    int owner;
} ScoreEntry;

extern ScoreEntry D_80237180[][3][100];

void func_800AAC14(int x0, float y0, int x1, float y1,
                   int layers, int owner, int group) {
    int old[2][3];
    char unused[16];
    int unchanged = 1;
    int dx = x1 - x0;
    int layer;
    int scan0;
    int scan1;
    int compare;
    int x;
    float step;
    ScoreEntry *entry;

    if (dx == 0) {
        for (layer = layers - 1; layer >= 0; layer--) {
            entry = &D_80237180[group][layer][x0];
            if (!(y0 < entry->score))
                return;
            entry->score = y0;
            entry->owner = owner;
        }
        return;
    }

    scan0 = 0;
    if (layers > 0) {
        do {
            entry = &D_80237180[group][scan0][x0];
            if (y0 < entry->score) {
                unchanged = 0;
                entry->score = y0;
                entry->owner = owner;
            } else {
                old[0][scan0] = entry->owner;
            }
            scan0++;
        } while (scan0 < layers);
    }
    scan1 = 0;
    if (layers > 0) {
        do {
            entry = &D_80237180[group][scan1][x1];
            if (y1 < entry->score) {
                unchanged = 0;
                entry->score = y1;
                entry->owner = owner;
            } else {
                old[1][scan1] = entry->owner;
            }
            scan1++;
        } while (scan1 < layers);
    }
    if (unchanged) {
        for (compare = 0; compare < layers && unchanged; compare++)
            unchanged &= -(old[0][compare] == old[1][compare]);
        if (unchanged)
            return;
    }

    step = (y1 - y0) / (float)dx;
    for (x = x0 + 1; x <= x1 - 1; x++) {
        y0 += step;
        for (layer = layers - 1; layer >= 0; layer--) {
            entry = &D_80237180[group][layer][x];
            if (!(y0 < entry->score))
                break;
            entry->score = y0;
            entry->owner = owner;
        }
    }
}
