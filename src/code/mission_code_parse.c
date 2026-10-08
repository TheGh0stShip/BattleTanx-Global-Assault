typedef unsigned long long u64;
typedef long long s64;
extern unsigned int D_80117EB8;
extern unsigned char D_80125558[];
extern int D_80119320;
extern unsigned char D_80125AB2;
extern int D_80125548;
extern int D_8012554C;
extern int D_80125550;
extern int D_80125554;
extern int strlen(const char *);
extern u64 func_8009E7F0(u64);
extern void func_800C0A6C(int);

int func_800E944C(char *str)
{
    u64 v = 0;
    u64 r;
    int i;
    int j;
    int found;
    int a, b, c, d;

    if (strlen(str) != 10) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        found = 0;
        for (j = 0; j < 32; j++) {
            if (D_80125558[j] == (unsigned char)str[i]) {
                found = 1;
                v = (v << 5) + j;
            }
        }
        if (!found) {
            return 0;
        }
    }
    r = func_8009E7F0(v);
    if ((r >> 50) != 0) {
        return 0;
    }
    if ((r & 0xFF) != 20) {
        return 0;
    }
    a = (int)(r >> 16) & 0x3F;
    d = ((int)(r >> 8) & 0xFF) * 5;
    if (a >= 32) {
        return 0;
    }
    c = ((int)(r >> 22) & 0x1FFF) * 100;
    b = ((int)(r >> 35) & 0x1FFF) * 100;
    switch ((int)(r >> 48) & 3) {
    case 0: D_80117EB8 = 1; break;
    case 1: D_80117EB8 = 2; break;
    case 2: D_80117EB8 = 3; break;
    default: return 0;
    }
    func_800C0A6C(D_80119320 + 560);
    if (a == 27) {
        a += (D_80125AB2 == 0);
    }
    D_80125548 = a;
    D_80125550 = b;
    D_80125554 = c;
    D_8012554C = d;
    return 1;
}
