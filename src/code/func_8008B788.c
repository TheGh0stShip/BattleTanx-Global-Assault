#include "types.h"

typedef struct Func8008B788Node {
    u8 pad0[4];
    struct Func8008B788Node *next;
    u8 pad8[0x94];
    s32 group;
    u8 padA0[0x130];
    u8 *manager;
} Func8008B788Node;

extern void func_80089F40(
    Func8008B788Node *node, s32 oldGroup, s32 newGroup, s32 zero);

void func_8008B788(Func8008B788Node *node, s32 newGroup) {
    s32 oldGroup = node->group;
    u8 *manager = node->manager;
    Func8008B788Node **link =
        (Func8008B788Node **)(manager + 0x1B8 + oldGroup * 4);
    Func8008B788Node **newBase;
    register u8 *countManager asm("$3");

    while (*link != 0 && *link != node) {
        link = &(*link)->next;
    }
    *link = node->next;
    countManager = *(u8 * volatile *)&node->manager;
    countManager = (u8 *)((u32)countManager + oldGroup);
    countManager[0x1C8]--;
    newBase = (Func8008B788Node **)(newGroup * 4 + (u32)manager);
    node->next = *(Func8008B788Node **)((u8 *)newBase + 0x1B8);
    *(Func8008B788Node **)((u8 *)newBase + 0x1B8) = node;
    node->group = newGroup;
    countManager = *(u8 * volatile *)&node->manager;
    countManager = (u8 *)((u32)countManager + newGroup);
    countManager[0x1C8]++;
    func_80089F40(node, oldGroup, newGroup, 0);
}
