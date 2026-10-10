/* SPAN 0x800B0D70 */
/* RODATA_VRAM 0x80072F90 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    int unk0;
    int unk4;           /* 0x04 */
    u16 link[4];        /* 0x08 */
    char pad10[0x16];
    u16 flags;          /* 0x26 */
} GridNode;

typedef struct {
    s16 x0, z0;         /* 0x00 */
    u16 w, h;           /* 0x04 */
    u16 cw, ch;         /* 0x08 */
    u16 *cells;         /* 0x0C */
    float scale;        /* 0x10 */
    void *tbl;          /* 0x14 */
} Grid;

typedef struct {
    int unk0;
    s16 x0, z0, x1, z1; /* 0x04 */
    char padC[0x14];
} Area;

typedef struct {
    Area areas[6];      /* 0x00 */
    char padC0[4];
    u8 nAreas;          /* 0xC4 */
} World;

extern World *D_80219498;
extern GridNode D_803978E0[];
extern u16 D_803977E8;
extern Grid D_803977F0[];
extern u16 D_80397658[];
extern u16 D_801166F0;
extern u16 D_803A5380;
extern u16 D_803A539E;
extern u32 D_8021949C;

void func_80102F10(void *p, int n);
void func_800A2DFC(void);
int func_8009D144(void);
u16 *func_800ACEB4(int size);
void func_800B07D4(u16 id, s16 x0, s16 z0, s16 x1, s16 z1);
extern char D_801179FC[];
extern char D_80117A18[];
extern char D_80117A34[];
extern char D_80117A50[];
extern char D_80117A6C[];
extern char D_80117A88[];
extern char D_80117AA4[];
extern char D_80117AC0[];
extern char D_80117ADC[];
extern char D_80117AF8[];
extern char D_80117B14[];
extern char D_80117B30[];
extern char D_80117B4C[];
extern char D_80117B68[];
extern char D_80117B84[];
extern char D_80117BA0[];
extern char D_80117BBC[];
extern char D_80117BD8[];
extern char D_80117BF4[];
extern char D_80117C10[];
extern char D_80117C2C[];
extern char D_80117C48[];
extern char D_80117C64[];
extern char D_80117C80[];
extern char D_80117C9C[];
extern char D_80117CB8[];
extern char D_80117CD4[];
extern char D_80117CF0[];
extern char D_80117D0C[];
extern char D_80117D28[];
extern char D_80117D44[];
extern char D_80117D60[];
extern char D_80117D7C[];
extern char D_80117D98[];
extern char D_80117DB4[];

void func_800B06E0(void) {
    u16 i;

    func_80102F10(D_803978E0, 0xDAC0);
    for (i = 0; i < 1399; i++) {
        D_803978E0[i].link[0] = i + 1;
        D_803978E0[i].flags = 0xFFFF;
    }
    D_803A5380 = 0xFFFF;
    D_803A539E = 0xFFFF;
    D_803977E8 = 0;
    for (i = 0; i < D_80219498->nAreas; i++) {
        func_800B07D4(i, D_80219498->areas[i].x0, D_80219498->areas[i].z0, D_80219498->areas[i].x1,
                      D_80219498->areas[i].z1);
    }
    D_801166F0 = 0;
}

void func_800B07D4(u16 id, s16 x0, s16 z0, s16 x1, s16 z1) {
    Grid *g = &D_803977F0[id];
    u16 n;
    u16 i;
    int ch;
    u16 *c;

    g->w = x1 - x0;
    g->h = z1 - z0;
    g->cw = (g->w >> 10) + 3;
    ch = (g->h >> 10) + 3;
    g->x0 = x0;
    g->z0 = z0;
    g->ch = ch;
    n = (g->cw * (ch << 2)) | 1;
    c = func_800ACEB4(n * 2);
    g->cells = c;
    for (i = 0; i < n; i++) {
        *c++ = 0xFFFF;
    }
    if (id == 0) {
        g->scale = 70.0f / g->w;
        if (70.0f / g->h < g->scale) {
            g->scale = 70.0f / g->h;
        }
        if (func_8009D144()) {
            switch (D_8021949C) {
            case 0:
            case 24:
                g->tbl = D_80117C9C;
                break;
            case 1:
                g->tbl = D_80117D98;
                break;
            case 2:
                g->tbl = D_80117CB8;
                break;
            case 3:
                g->tbl = D_80117B84;
                break;
            case 4:
                g->tbl = D_80117B4C;
                break;
            case 5:
                g->tbl = D_80117DB4;
                break;
            case 6:
                g->tbl = D_80117C10;
                break;
            case 7:
                g->tbl = D_80117D7C;
                break;
            case 8:
                g->tbl = D_80117D44;
                break;
            case 9:
                g->tbl = D_80117AA4;
                break;
            case 10:
                g->tbl = D_80117B14;
                break;
            case 11:
                g->tbl = D_80117BA0;
                break;
            case 12:
                g->tbl = D_80117A6C;
                break;
            case 13:
                g->tbl = D_80117ADC;
                break;
            case 14:
                g->tbl = D_80117BD8;
                break;
            case 15:
                g->tbl = D_80117C64;
                break;
            case 16:
                g->tbl = D_801179FC;
                break;
            case 25:
                g->tbl = 0;
                break;
            case 26:
                g->tbl = D_80117C48;
                break;
            }
        } else {
            switch (D_8021949C) {
            case 1:
                g->tbl = D_80117D98;
                break;
            case 2:
                g->tbl = D_80117CD4;
                break;
            case 3:
                g->tbl = D_80117B84;
                break;
            case 4:
                g->tbl = D_80117B68;
                break;
            case 5:
                g->tbl = D_80117DB4;
                break;
            case 6:
                g->tbl = D_80117C2C;
                break;
            case 7:
                g->tbl = D_80117D7C;
                break;
            case 8:
                g->tbl = D_80117D60;
                break;
            case 9:
                g->tbl = D_80117AC0;
                break;
            case 10:
                g->tbl = D_80117B30;
                break;
            case 11:
                g->tbl = D_80117BBC;
                break;
            case 12:
                g->tbl = D_80117A88;
                break;
            case 13:
                g->tbl = D_80117AF8;
                break;
            case 14:
                g->tbl = D_80117BF4;
                break;
            case 15:
                g->tbl = D_80117C80;
                break;
            case 16:
                g->tbl = D_801179FC;
                break;
            case 17:
                g->tbl = D_80117A18;
                break;
            case 18:
                g->tbl = D_80117A34;
                break;
            case 19:
                g->tbl = D_80117A50;
                break;
            case 20:
            case 26:
                g->tbl = D_80117CF0;
                break;
            case 21:
                g->tbl = D_80117D0C;
                break;
            case 22:
            case 23:
                g->tbl = D_80117D28;
                break;
            case 0:
            case 24:
                g->tbl = D_80117C9C;
                break;
            case 25:
                g->tbl = 0;
                break;
            }
        }
    }
}

void func_800B0B8C(u16 id) {
    GridNode *n = &D_803978E0[id];

    n->flags |= 0x1000;
    D_80397658[D_801166F0] = id;
    D_801166F0++;
}

void func_800B0BE0(void) {
    u16 i;

    for (i = 0; i < D_801166F0; i++) {
        (&D_803978E0[(&D_80397658[i])[0]])->flags &= ~0x1000;
    }
    D_801166F0 = 0;
}

u16 func_800B0C54(void) {
    u16 head = D_803977E8;
    GridNode *n;
    u16 i;

    if (head == 0xFFFF) {
        func_800A2DFC();
        if (D_803977E8 == head) {
            return 0xFFFF;
        }
    }
    head = D_803977E8;
    n = &D_803978E0[head];
    D_803977E8 = n->link[0];
    for (i = 0; i < 4; i++) {
        n->link[i] = 0xFFFF;
    }
    n->unk4 = 0;
    return head;
}

int func_800B0CFC(u16 id, u16 dir) {
    u16 n = 0;

    u16 next;

    if (id == 0xFFFF) return 1;
    while (n < 150) {
        next = (&D_803978E0[id])->link[dir];
        if (next == 0xFFFF) break;
        n++;
        id = next;
    }
    return n < 100;
}
