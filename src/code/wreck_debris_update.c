/* ---- 0x800F6800/src/f708c.c ---- */
typedef struct { char pad[28]; int **p1c; int i20; unsigned short a, b, c, da, db, dc; int t30; } Obj;
extern int D_8021945C;
extern float D_80219488;
void func_800F708C(Obj *o, int *out) {
    if (D_8021945C - o->t30 >= 51 || (o->p1c != 0 && **o->p1c != o->i20)) {
        *out = 1;
    } else {
        o->a += (int)(D_80219488 * o->da);
        o->b += (int)(D_80219488 * o->db);
        o->c += (int)(D_80219488 * o->dc);
    }
}

