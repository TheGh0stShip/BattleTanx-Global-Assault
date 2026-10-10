/* SPAN 0x800B129C */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    int unk0;
    int unk4;           /* 0x04 */
    u16 link[4];        /* 0x08 */
    s16 x, z, y;        /* 0x10 */
    s16 dx0, dx1;       /* 0x16 */
    s16 dz0, dz1;       /* 0x1A */
    u16 unk1E, unk20;   /* 0x1E */
    s16 radius;         /* 0x22 */
    u16 unk24;          /* 0x24 */
    u16 grid;          /* 0x26 */
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
int func_8009D6DC(u16 a, u16 b);
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

void func_800B06E0(void);

void func_800B07D4(u16 id, s16 x0, s16 z0, s16 x1, s16 z1);

extern inline void func_800B0B8C(u16 id) {
    GridNode *n = &D_803978E0[id];

    n->grid |= 0x1000;
    D_80397658[D_801166F0] = id;
    D_801166F0++;
}

void func_800B0BE0(void);

extern inline u16 func_800B0C54(void) {
    u16 head = D_803977E8;
    u16 id;
    GridNode *n;
    u16 i;

    if (head == 0xFFFF) {
        func_800A2DFC();
        if (D_803977E8 == head) {
            return 0xFFFF;
        }
    }
    id = D_803977E8;
    n = &D_803978E0[id];
    D_803977E8 = n->link[0];
    for (i = 0; i < 4; i++) {
        n->link[i] = 0xFFFF;
    }
    n->unk4 = 0;
    return id;
}

int func_800B0CFC(u16 id, u16 dir);

extern u16 D_801166E0[4];
extern u16 D_801166E8[4];

#define ABS(x) ((x) > 0 ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

u16 func_800B0D70(u16 target, u16 *head, short dir);

void func_800B0E38(u16 id);

void func_800B0F4C(u16 id);


void func_800B129C(u16 id, s16 x, s16 z);

void func_800B14A8(u16 id, s16 x, s16 z);

void func_800B1610(u16 id, int grid);

void func_800B1668(u16 id, s16 x, s16 z, s16 grid);

void func_800B16E4(u16 id, s16 dx0, s16 dx1, s16 dz0, s16 dz1, u16 a5, u16 a6);

u16 func_800B1DA8(int data, s16 x, s16 z, s16 y, s16 dx0, s16 dx1, s16 dz0, s16 dz1, u16 a8, u16 a9, u16 a10, int kind, u16 grid);

u16 func_800B205C(int data, s16 x, s16 z, s16 y, s16 dx0, s16 dx1, s16 dz0, s16 dz1, u16 a8, u16 a9, u16 a10, int kind, u16 grid);

u16 func_800B1898(int data, s16 x, s16 z, s16 y, s16 dx0, s16 dx1, s16 dz0, s16 dz1, u16 a8, u16 a9, u16 ang, int kind, u16 grid);

void func_800B22F8(u16 id);


void func_800B109C(u16 id, s16 x, s16 z, s16 y, s16 dx0, s16 dx1, s16 dz0, s16 dz1, u16 a8, u16 a9, u16 a10,
                   u16 grid) {
    GridNode *n = &D_803978E0[id];
    u16 dx, dz;

    func_800B0F4C(id);
    n->x = x;
    n->z = z;
    n->y = y;
    n->dx0 = dx0;
    n->dx1 = dx1;
    n->dz0 = dz0;
    n->dz1 = dz1;
    n->unk1E = a8;
    n->unk20 = a9;
    n->unk24 = a10;
    n->grid = grid;
    dx = MAX(ABS(dx0), ABS(dx1));
    dz = MAX(ABS(dz0), ABS(dz1));
    n->radius = MAX(ABS(dx), ABS(dz)) + MIN(ABS(dx), ABS(dz)) * 3 / 8;
    func_800B0E38(id);
}
