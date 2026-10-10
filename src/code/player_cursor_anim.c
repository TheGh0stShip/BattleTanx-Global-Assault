typedef struct Menu {
    char pad0[0x10]; int count; int idx; unsigned int state; float t;
} Menu;

void func_800D7544(Menu *m, short *out) {
    int off = 0;
    int i;

    switch (m->state) {
    case 0:
        break;
    case 1:
        off = m->t * (65536.0f / m->count);
        break;
    case 2:
        off = -m->t * (65536.0f / m->count);
        break;
    }
    for (i = 0; i < m->count; i++) {
        out[i] = (0xFFFF / m->count) * (i + m->count - m->idx) + off;
    }
}
