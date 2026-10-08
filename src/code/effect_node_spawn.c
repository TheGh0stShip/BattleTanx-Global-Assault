typedef struct Vec3 { float x, y, z; } Vec3;
typedef struct TypeInfo {
    short s[6];
    char pad[0x5C - 0xC];
    unsigned short u5C;
    char pad2[0xD0 - 0x5E];
} TypeInfo;
typedef struct Slot {
    unsigned int flags;
    void *node;
    char pad[0x20 - 8];
    short h20;
    char pad2[0x28 - 0x22];
} Slot;
typedef struct Node {
    char pad0[0xC];
    int type;
    Vec3 pos;
    unsigned short h1C;
    int i20;
    int i24;
    int t28;
    unsigned short h2C;
    unsigned char b2E;
    unsigned char b2F;
    char pad30[2];
    short h32;
    short h34;
} Node;
extern int D_8021945C;
extern TypeInfo D_80122E3C[];
extern Slot D_803978E0[];
extern Node *func_800A18D0(int, int);
extern unsigned short func_800B1898(Node *, short, short, short, int, int, int, int, int, int, int, int, int);
extern float func_800B95C8(int, short *, short *);

Node *func_800D7DE0(int type, Vec3 *pos, unsigned char b, unsigned short h, int i,
                    unsigned short id, unsigned char flag) {
    Node *n = func_800A18D0(10, 56);
    if (n == 0) {
        return 0;
    }
    n->type = type;
    n->pos = *pos;
    n->b2E = b;
    n->b2F = flag;
    n->h1C = h;
    n->i20 = i;
    n->t28 = D_8021945C;
    if (D_8021945C == 0) {
        n->t28 = -1000;
    }
    if (id == 0xFFFF) {
        n->h2C = func_800B1898(n, pos->x, pos->y, pos->z,
                               D_80122E3C[type].s[0], D_80122E3C[type].s[1], D_80122E3C[type].s[2],
                               D_80122E3C[type].s[3], D_80122E3C[type].s[4], D_80122E3C[type].s[5],
                               h, 0x400000, b);
    } else {
        Slot *e = &D_803978E0[id];
        n->h2C = id;
        e->node = n;
        e->flags = (e->flags & ~8) | 0x400000;
    }
    if (flag) {
        Slot *e = &D_803978E0[id];
        e->flags = (e->flags & 0x051A2280) | 0x40;
        e->h20 = 10;
    } else {
        n->i24 = D_80122E3C[type].u5C >> 2;
    }
    n->pos.z = func_800B95C8(n->h2C, &n->h32, &n->h34);
    return n;
}
