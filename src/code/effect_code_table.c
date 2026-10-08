typedef unsigned long long u64;
typedef struct Ent36 { int w[9]; } Ent36;
extern Ent36 D_80122384[];
extern int D_80123AC8[];

Ent36 *func_800D69E0(int i) {
    return &D_80122384[i];
}

void func_800D69FC(u64 code, int *out) {
    int a, b;
    int i;
    int *tbl = D_80123AC8;

    a = D_80123AC8[((code >> 20) & 0xF) % 15];
    b = 0;
    if (((code >> 28) & 0x1F) == 0) {
        b = tbl[((code >> 24) & 0xF) % 15];
    }
    for (i = 0; i < 5; i++) {
        out[i] = ((code >> (i * 4)) & 0xF) % 11;
    }
    out[6] = a;
    out[7] = b;
}
