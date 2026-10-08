/* ---- 0x800DC000/b/func_800DD290.c ---- */
typedef unsigned char u8; typedef unsigned int u32;
typedef struct { u32 w0, w1; } G;
typedef struct { int a; G *dl; int c; } E;
typedef struct { E *list; u8 count; } C;
extern G D_80123B98; extern u32 D_80123B9C;
int func_800DD290(C **pp) {
    C *c = *pp;
    int n = 0;
    int i;
    int found;
    for (i = c->count - 1; i >= n; ) {
        E *e = &c->list[n];
        int j = 0;
        found = 0;
        do {
            if (e->dl[j].w0 == D_80123B98.w0 && e->dl[j].w1 == D_80123B9C) found = 1;
            j++;
        } while (e->dl[j].w0 != 0xDF000000);
        if (found) {
            E t = *e;
            *e = c->list[i];
            c->list[i] = t;
            i--;
        } else {
            n++;
        }
    }
    return n;
}

