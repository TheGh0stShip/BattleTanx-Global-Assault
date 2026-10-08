/* ---- 0x800DE000/b/h.c ---- */
typedef struct { unsigned char kind; char pad[7]; unsigned char sub; } ObjH;
extern unsigned char D_802194A4;
int func_800DF33C(ObjH *o) {
    if (D_802194A4 < 2) return 1;
    switch (o->kind) {
    case 4:
    case 10:
    case 24:
        return 0;
    case 15:
        return o->sub == 1 || o->sub == 6;
    }
    return 1;
}

