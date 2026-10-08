/* ---- 0x800D6000/f6e40.c ---- */
typedef struct Obj {
    char pad0[0x1E8];
    struct Info *info;
    struct Node *node;
    char pad1[0x218 - 0x1F0];
    int time;
} Obj;
typedef struct Node {
    char pad0[0xC];
    Obj *owner;
    int unk10;
    int unk14;
    char pad18[0x20 - 0x18];
    unsigned char unk20;
    char pad21[3];
    int unk24;
    int unk28;
    int unk2C;
} Node;
extern Node *func_800A18D0(int, int);
extern int func_8009D144(void);
extern int D_8021945C;
#define gFrame D_8021945C

Node *func_800D6E40(Obj *obj) {
    Node *n = func_800A18D0(6, 0x30);
    if (n == 0) {
        return 0;
    }
    n->owner = obj;
    obj->node = n;
    n->unk20 = 0;
    n->unk24 = 15;
    n->unk10 = -1;
    n->unk14 = 0;
    n->unk28 = gFrame;
    if (func_8009D144() == 0 && gFrame - obj->time < 120) {
        n->unk2C = 1;
    } else {
        n->unk2C = 0;
    }
    return n;
}

