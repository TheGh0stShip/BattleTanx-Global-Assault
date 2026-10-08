#include "types.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Off { f32 x, y; } Off;
typedef struct PNode {
    u8 pad[0x20];
    struct PNode* next;
    f32 x, y, z;
} PNode;
typedef struct Obj7F60 {
    u8 pad[0xC];
    PNode* head;
    f32 x, y, z;
    u8 id;
} Obj7F60;

extern Off D_80123E10[];

void func_800E7F60(Obj7F60* o, Vec3f* out, u8* idOut) {
    s32 i;
    Vec3f v;
    PNode* n;
    u8 found;

    *idOut = o->id;
    for (i = 0; i < 5; i++) {
        v.x = o->x + D_80123E10[i].x;
        v.z = o->z;
        v.y = o->y + D_80123E10[i].y;
        found = 0;
        n = o->head;
        while (n != 0) {
            if (v.x == n->x && v.z == n->z && v.y == n->y) {
                found = 1;
                n = 0;
            } else {
                n = n->next;
            }
        }
        if (!found) {
            *out = v;
            return;
        }
    }
}
