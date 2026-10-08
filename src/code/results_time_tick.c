typedef struct { char pad[0x14]; unsigned short idx; } Obj;
extern float D_803A5948; extern int D_803A6640;
extern int D_803A65A0[]; extern int D_803A6628[];
extern char D_803A6600[][10];
extern unsigned char D_803A665C; extern unsigned char D_803A65C2; extern short D_803A663E;
extern char D_800740E0[]; extern char D_800740EC[];
void func_800979F4(int); int sprintf(char *, const char *, ...);
int func_800CF7A8(Obj *o) {
    unsigned short i = o->idx;
    D_803A65A0[i] += (unsigned short)(unsigned int)(D_803A6640 * D_803A5948);
    if (D_803A65A0[i] >= D_803A6628[i]) {
        int t, h, m, s, rem; char *buf;
        t = D_803A6628[i]; D_803A65A0[i] = t;
        h = t / 108000; rem = t % 108000;
        m = rem / 1800; rem = rem % 1800;
        buf = D_803A6600[i]; s = rem / 30;
        if (h == 0) sprintf(buf, D_800740E0, m, s);
        else sprintf(buf, D_800740EC, h, m, s);
        if (D_803A665C) { D_803A65C2 = 1; D_803A663E = 0; func_800979F4(45); }
        return 1;
    } else {
        int t, h, m, s, rem; char *buf;
        t = D_803A65A0[i];
        h = t / 108000; rem = t % 108000;
        m = rem / 1800; rem = rem % 1800;
        buf = D_803A6600[i]; s = rem / 30;
        if (h == 0) sprintf(buf, D_800740E0, m, s);
        else sprintf(buf, D_800740EC, h, m, s);
        return 0;
    }
}
