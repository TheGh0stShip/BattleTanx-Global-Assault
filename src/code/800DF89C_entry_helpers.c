/* Compiler-constrained source match: p7 must occupy $a0 at the call site. */
typedef struct { short a; short b; short c; unsigned short d; short e; short f; unsigned short g; short h; } Ent;
typedef struct { char pad[0x14]; Ent *ents; char p2[0x38-0x18]; } Elem;
typedef struct { int x; Elem *elems; } Tbl;
extern unsigned short func_800B1898(void *, short, short, int, int, int, int, int, short, short, unsigned short, int, unsigned char);
unsigned short func_800DF89C(Tbl *t, float *pos, unsigned short p2, unsigned char p3, int idx, unsigned short sub, int p6, void *p7) {
    register void *q asm("$4") = p7;
    Ent *e = &t->elems[idx].ents[sub];
    return func_800B1898(q, pos[0], pos[1], 0, e->c, e->f, e->e, e->h, e->d + (int)pos[2], e->g + (int)pos[2], p2, p6, p3);}

unsigned short func_800DF988(Tbl *t, float *pos, unsigned short p2, unsigned char p3, int idx, unsigned short sub, int p6, void *p7) {
    register void *q asm("$4") = p7;
    Ent *e = &t->elems[idx].ents[sub];
    return func_800B1898(q, pos[0], pos[1], 0, e->c, e->f, e->e, e->h, e->d + (int)pos[2], 5000, p2, p6, p3);}
