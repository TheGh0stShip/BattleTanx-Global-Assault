/* ---- 0x800E0000/f1980.c ---- */
typedef struct {
    char pad[0x1C]; unsigned char b1C; unsigned char b1D; char p1E[2]; float x; float y;
} Ent3;
typedef struct { unsigned char b0; char p1[3]; int i4; } Data3;
typedef struct { unsigned char b; char p[3]; float x; float y; } Out3;
extern unsigned short func_8009E9C8(Data3 *, float *);
extern void func_800E16D8(Ent3 *, unsigned short);

void func_800E1980(Ent3 *e, void *m, Data3 *d, Out3 *o) {
    if (d->i4 == 0 && e->b1D == 0 && e->b1C == d->b0) {
        o->b = 1;
        o->x = e->x;
        o->y = e->y;
    }
}

void func_800E19C4(Ent3 *e, void *m, Data3 *d) {
    if (e->b1D == 0) {
        func_800E16D8(e, func_8009E9C8(d, &e->x));
    }
}

void func_800E1A08(Ent3 *e, void *m, Data3 *d) {
    if (e->b1D == 0) {
        func_800E16D8(e, func_8009E9C8(d, &e->x));
    }
}

