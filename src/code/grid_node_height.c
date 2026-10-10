typedef short s16;
typedef unsigned short u16;

typedef struct {
    int unused0;
    int unused4;
    u16 links[4];
    s16 x;
    s16 z;
    s16 y;
    s16 dx0;
    s16 dx1;
    s16 dz0;
    s16 dz1;
    s16 height0;
    s16 height1;
    s16 radius;
    u16 field_24;
    u16 grid;
} GridNode;

extern GridNode D_803978E0[];
int abs(int value);

float func_800B8870(u16 id) {
    GridNode *node = &D_803978E0[id];
    GridNode *copy = node;
    int first = node->height0;
    int second = node->height1;

    if (abs(first) > abs(second)) {
        return first + copy->y;
    }
    return second + node->y;
}
