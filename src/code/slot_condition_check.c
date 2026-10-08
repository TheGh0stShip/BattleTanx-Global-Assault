typedef struct { unsigned char type; unsigned char b1; union { unsigned short h; unsigned char b; } u2; union { unsigned short h; int w; } u4; unsigned char b8; } CondG;
typedef struct { char p[0x10]; int base; char q[0x38-0x14]; } EntryG;
typedef struct { int a; EntryG *list; } CtxG;
extern unsigned char D_802194A4;
extern unsigned char D_802194A6;
extern unsigned char D_80219582[];
extern int func_800E8090(int);
int func_800DF3B8(CondG *arg, CtxG *ctx, int idx) {
    CondG *c;
    int ok;
loop:
    c = arg;
    if (D_802194A4 >= 2) {
        switch (c->type) {
        case 4:
            ok = 0;
            break;
        default:
            ok = 1;
            break;
        case 10:
        case 24:
            ok = 0;
            break;
        case 15:
            ok = (c->b8 == 1) | (c->b8 == 6);
            break;
        }
    } else {
        ok = 1;
    }
    if (!ok) return 0;
    switch (c->type) {
    case 39:
        switch (c->b1) {
        case 0:
            if (!func_800E8090(c->u2.h)) return 0;
            break;
        case 1:
            if (func_800E8090(c->u2.h)) return 0;
            break;
        default:
            return 0;
        }
        arg = (CondG *)(ctx->list[idx].base + c->u4.w);
        goto loop;
    case 0: case 1: case 2: case 3: case 4: case 5: case 10: case 11: case 14: case 15:
    case 21: case 22: case 24: case 28: case 34: case 35: case 43: case 44:
        return 1;
    case 12:
        if ((short)D_80219582[c->u2.b] < (short)D_802194A6) return c->u4.h != 0xFFFF;
        return 0;
    case 31:
        return func_800E8090(c->b1) != 0;
    }
    return 0;
}
