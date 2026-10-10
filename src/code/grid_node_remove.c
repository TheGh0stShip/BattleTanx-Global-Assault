typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    int unk00;
    int unk04;
    u16 links[4];
    s16 x;
    s16 z;
    s16 y;
    s16 dx0;
    s16 dx1;
    s16 dz0;
    s16 dz1;
    u16 unk1E;
    u16 unk20;
    s16 radius;
    u16 unk24;
    u16 grid;
} GridNode;

typedef struct {
    s16 x;
    s16 z;
    u16 width;
    u16 height;
    u16 cell_width;
    u16 cell_height;
    u16 *cells;
    float scale;
    void *table;
} Grid;

extern GridNode D_803978E0[];
extern Grid D_803977F0[];
extern u16 D_801166E0[4];
extern u16 D_801166E8[4];

u16 func_800B0D70(u16 target, u16 *head, short direction);

void func_800B0F4C(u16 id) {
    GridNode *node = &D_803978E0[id];
    Grid *grid;
    int dx;
    int dz;
    u8 i;

    if (node->grid == 0xFFFF) {
        return;
    }
    grid = &D_803977F0[node->grid];
    dx = node->x - grid->x;
    dz = node->z - grid->z;
    if (func_800B0D70(id, grid->cells, 0)) {
        return;
    }
    for (i = 0; i < 4; i++) {
        func_800B0D70(
            id,
            &grid->cells[
                ((((dz + D_801166E8[i]) >> 10) + i * grid->cell_height) *
                     grid->cell_width +
                 ((dx + D_801166E0[i]) >> 10)) &
                0xFFFF
            ] + 1,
            i
        );
    }
}
