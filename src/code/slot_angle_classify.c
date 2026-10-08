typedef struct { short a; short b; short c; unsigned short d; short e; short f; short g; short h; } Ent;
typedef struct { char pad[0x14]; Ent *ents; char p2[0x38-0x18]; } Elem;
typedef struct { int x; Elem *elems; } Tbl;
extern double D_80075948;
#define ABS(x) ((x) > 0 ? (x) : -(x))
int func_800DF63C(Tbl *t, int idx, unsigned short sub, unsigned char mode, float f, unsigned char kind) {
    int a = (int)f + t->elems[idx].ents[sub].g;
    if (mode != 0) {
        if (ABS(a - 105) < 10) return 129;
        if (kind == 1 && ABS(a - 144) < 10) return 130;
        if (ABS(a - 210) < 10) return 130;
        if (a > D_80075948) return 131;
    }
    if (a < 106) return 1;
    if (a < 211) return 2;
    return 3;
}
