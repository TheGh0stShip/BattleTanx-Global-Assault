typedef struct { int a; int b; unsigned short c; char ext[4]; char name[16]; char pad[2]; } Save;
typedef struct { char text[4]; char name[16]; char term; char pad[11]; } Disp;
extern Save D_803A6360[16];
extern Disp D_803A62D0[4];
extern unsigned short D_8011F236;
extern char D_8011F9EC[][16];
extern unsigned short D_8011F820[];
extern char D_80074034[];
extern unsigned short func_800CC0B0(void);
extern int sprintf(char *, const char *, ...);
extern void func_80099160(void *, char *, int);
extern void func_80097508(char *, int);

int func_800CC2F8(int base) {
    unsigned short i;
    unsigned char buf[16];
    unsigned char *p;
    int slot;
    char (*flags)[16] = D_8011F9EC;

    unsigned short r;
    if (D_8011F236 == 0 && (r = func_800CC0B0()) != 0) return r;
    {
    for (i = 0; i < 4; i++) {
        slot = base + i;
        if (slot >= 16) {
            D_803A62D0[i].text[0] = 0;
            flags[i][0] = 1;
            continue;
        }
        flags[i][0] = 8;
        sprintf(D_803A62D0[i].text, D_80074034, slot + 1);
        if (D_803A6360[slot].b == 0 && D_803A6360[slot].c == 0 && D_803A6360[slot].a == 0) {
            D_803A62D0[i].name[0] = 'e';
            D_803A62D0[i].name[1] = 'm';
            D_803A62D0[i].name[2] = 'p';
            D_803A62D0[i].name[3] = 't';
            D_803A62D0[i].name[4] = 'y';
            D_803A62D0[i].name[5] = 0;
            D_8011F820[i] = 0;
            continue;
        }
        func_80099160(D_803A62D0[i].name, D_803A6360[base + i].name, 16);
        D_803A62D0[i].term = 0;
        func_80099160(buf, D_803A6360[base + i].ext, 4);
        buf[4] = 0;
        if (buf[0] != 0) {
            func_80097508(D_803A62D0[i].name, '.');
            p = buf;
            while (*p != 0) func_80097508(D_803A62D0[i].name, *p++);
        }
        D_8011F820[i] = (unsigned int)D_803A6360[base + i].a >> 8;
    }
    }
    return 0;
}
