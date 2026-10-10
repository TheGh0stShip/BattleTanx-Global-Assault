typedef unsigned long long u64;
typedef long long s64;
extern unsigned int D_80117EB8;
extern unsigned char D_80125558[];
extern int sprintf(char *, const char *, ...);
extern u64 func_8009E668(u64);

void func_800E96B4(unsigned int a0, int a1, int a2, int a3, char *out)
{
    int mode = 0;
    u64 v;
    int i;

    if (a0 >= 32) {
        sprintf(out, "TRDDYBRRKS");
        return;
    }
    if (a1 > 819100) a1 = 819100;
    if (a2 > 819100) a2 = 819100;
    if (a3 > 1275) a3 = 1275;
    a1 /= 100;
    a2 /= 100;
    a3 /= 5;
    switch (D_80117EB8) {
    case 1: mode = 0; break;
    case 2: mode = 1; break;
    case 3: mode = 2; break;
    }
    v = func_8009E668(((s64)a3 << 8) + 20 + ((u64)a0 << 16) + ((s64)a2 << 22) + ((s64)a1 << 35) + ((s64)mode << 48));
    for (i = 0; i < 10; i++) {
        out[i] = D_80125558[(int)(v >> ((9 - i) * 5)) & 0x1F];
    }
    out[10] = 0;
}
