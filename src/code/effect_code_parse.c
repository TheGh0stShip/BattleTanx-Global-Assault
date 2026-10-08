typedef unsigned long long u64;
typedef struct Ent36 { int w[9]; } Ent36;
extern unsigned char D_80122380;
extern unsigned char D_80122381;
extern Ent36 D_80122534[];
extern unsigned char D_801225C4[];
extern int D_80123AC8[];
extern unsigned int strlen(unsigned char *);
extern u64 func_8009E250(u64);

static __inline__ void decode(u64 code, int *out) {
    int a, b;
    int i;

    a = D_80123AC8[((code >> 20) & 0xF) % 15];
    b = 0;
    if (((code >> 28) & 0x1F) == 0) {
        b = D_80123AC8[((code >> 24) & 0xF) % 15];
    }
    for (i = 0; i < 5; i++) {
        out[i] = ((code >> (i * 4)) & 0xF) % 11;
    }
    out[6] = a;
    out[7] = b;
}

void func_800D6B80(unsigned char *str) {
    u64 acc = 0;
    unsigned int i;
    int j;

    for (i = 0; i < strlen(str); i++) {
        for (j = 0; j < 21; j++) {
            if (str[i] == D_801225C4[j]) {
                break;
            }
        }
        acc *= 21;
        acc += j;
    }
    decode(func_8009E250(acc), D_80122534[D_80122381].w);
    D_80122381 = (D_80122381 + 1) % 4;
    D_80122380++;
    if (D_80122380 >= 5) {
        D_80122380 = 4;
    }
}
