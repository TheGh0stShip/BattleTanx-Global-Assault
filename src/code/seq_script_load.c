typedef struct {
    int active;
    void *obj;
    float time;
    unsigned short type;
    unsigned char pade;
    unsigned char idx;
} Ev;
typedef struct {
    int used;
    char pad[0x18];
} Slot;
typedef struct {
    char pad0[6];
    unsigned short v;
    char pad8[8];
} Rec16;
typedef struct {
    unsigned short v;
    char pad[86];
} Rec88;
extern unsigned char D_80121CE4;
extern unsigned char D_80121CE3;
extern unsigned char D_80121CD0;
extern unsigned char D_80121CD1;
extern float D_80121CD4;
extern float D_80121CD8;
extern void *D_80121CDC;
extern char D_01000138[], D_01000150[], D_01000168[], D_01000180[], D_01000198[], D_010001B0[], D_010001C8[], D_010001E0[];
extern void *D_803A6FCC;
extern void *D_803A6FD0;
extern Slot D_8011686C[4];
extern Slot D_80117880[14];
extern Ev D_803A66C0[50];
extern Rec16 D_803A6FE0[4];
extern Rec88 D_803A6A58[16];
extern int D_803A69FC;
extern int D_803A702C;
extern unsigned char *D_803A6660;
extern unsigned char *D_803A6674;
extern unsigned char D_803A69F0;
extern void *D_803A66B8;
extern char D_80235F78[];
extern float D_80236068;
extern unsigned char D_803A6A04;
extern float D_80074D00, D_80074D04, D_80074D08, D_80074D0C, D_80074D10, D_80074D14,
    D_80074D18, D_80074D1C, D_80074D20, D_80074D24;
extern double D_80074D28;
extern void *func_800ACEB4(int);
extern void func_8009ED00(int, unsigned char *, int);
extern unsigned short func_800D4B58(unsigned char *);
extern unsigned short func_800D12B0(unsigned char *, unsigned short);

void func_800D4DE0(void)
{
    unsigned short i;
    int size;
    unsigned char *p;
    unsigned char *s;
    unsigned short n;
    unsigned short type;
    float f;
    float g;

    if (D_80121CE4 == 0) {
        D_803A6FCC = func_800ACEB4(18400);
        D_803A6FD0 = func_800ACEB4(18400);
        D_8011686C[0].used = 0;
        D_8011686C[1].used = 0;
        D_8011686C[2].used = 0;
        D_8011686C[3].used = 0;
        D_80117880[3].used = 0;
        D_80117880[0].used = 0;
        D_80117880[1].used = 0;
        D_80117880[2].used = 0;
        D_80117880[4].used = 0;
        D_80117880[5].used = 0;
        D_80117880[6].used = 0;
        D_80117880[7].used = 0;
        D_80117880[8].used = 0;
        D_80117880[9].used = 0;
        D_80117880[10].used = 0;
        D_80117880[11].used = 0;
        D_80117880[12].used = 0;
        D_80117880[13].used = 0;
        D_80121CE4 = 1;
    }
    for (i = 0; i < 50; i++) {
        D_803A66C0[i].active = 0;
        D_803A66C0[i].obj = 0;
    }
    for (i = 0; i < 4; i++)
        D_803A6FE0[i].v = 0;
    for (i = 0; i < 16; i++)
        D_803A6A58[i].v = 0;
    D_80121CD0 = 255;
    D_80121CD1 = 0;
    if (D_80121CE3 != 0) {
        size = D_803A69FC - D_803A702C;
        size += size & 1;
        D_803A6660 = func_800ACEB4(size);
        func_8009ED00(D_803A702C, D_803A6660, size);
        D_80121CE3 = 0;
        D_803A6674 = D_803A6660;
    } else {
        D_803A6660 = D_803A6674;
    }
    D_803A69F0 = 0;
    D_803A66B8 = D_80235F78;
    switch (D_803A6660[1]) {
    case 1: D_80121CDC = D_010001E0; D_80121CD8 = D_80074D00; break;
    case 2: D_80121CDC = D_01000150; D_80121CD8 = D_80074D04; break;
    case 3: D_80121CDC = D_01000168; D_80121CD8 = D_80074D08; break;
    case 4: D_80121CDC = D_01000180; D_80121CD8 = D_80074D0C; break;
    case 5: D_80121CDC = D_01000198; D_80121CD8 = D_80074D10; break;
    case 6: D_80121CDC = D_010001B0; D_80121CD8 = D_80074D14; break;
    case 7: D_80121CDC = D_010001C8; D_80121CD8 = D_80074D18; break;
    case 0:
    default: D_80121CDC = D_01000138; D_80121CD8 = D_80074D1C; break;
    }
    g = D_80121CD4 * D_80074D20 + D_80074D24;
    if (D_80121CDC == D_01000150 || D_80121CDC == D_01000168 || D_80121CDC == D_010001E0)
        g = g * D_80074D28;
    D_80236068 = g;
    p = D_803A6660;
    n = (p[2] << 8) | p[3];
    s = p + 4;
    for (i = 0; i < n; i++) {
        type = func_800D4B58(s);
        D_803A6A04 = 255;
        do {
            s += func_800D12B0(s, type);
        } while (D_803A6A04 != 0);
        s++;
    }
}
