/* ---- 0x800DE000/c/j.c ---- */
typedef struct { unsigned short count; unsigned short start; char pad[12]; } Seg;
typedef struct { short x; short p; short y; unsigned short h; int key; } Item;
typedef struct { int *hdr; Seg *segs; Item *items; char pad[0x38-0xC]; } Elem;
typedef struct { int x; Elem *elems; } Tbl;
typedef struct { float x; float y; unsigned short h; unsigned char b; } Out;
int func_800DF7A0(Tbl *t, int idx, int key, Out *out, int max) {
    Elem *e = &t->elems[idx];
    unsigned char i;
    int n = 0;
    int j;
    int cnt = *e->hdr;
    for (i = 0; i < cnt; i++) {
        Seg *s = &e->segs[i];
        for (j = s->start; j < s->start + s->count; j++) {
            Item *it = &e->items[j];
            if (it->key == key) {
                out[n].x = it->x;
                out[n].y = it->y;
                out[n].h = it->h;
                out[n].b = i;
                n++;
                if (n == max) return n;
            }
        }
    }
    return n;
}

