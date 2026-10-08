typedef struct Menu {
    char pad0[0x10]; int count; int idx; unsigned int state; float t;
} Menu;
extern float D_800750B0, D_800750B4;

void func_800D7544(Menu *m, short *out) {
    int off = 0;
    int i;

    switch (m->state) {
    case 0:
        break;
    case 1:
        off = m->t * (D_800750B0 / m->count);
        break;
    case 2:
        off = -m->t * (D_800750B4 / m->count);
        break;
    }
    for (i = 0; i < m->count; i++) {
        out[i] = (0xFFFF / m->count) * (i + m->count - m->idx) + off;
    }
}
