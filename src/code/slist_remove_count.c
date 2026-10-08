/* ---- 0x800E4800/src/func_800E8318.c ---- */
#include "types.h"
#define NULL 0

typedef struct Item { u8 pad[0x10]; s32 id; u8 pad14[0x23C]; } Item;
typedef struct Obj82AC { u8 pad[0x1D0]; Item* item; } Obj82AC;
typedef struct Arg82AC { u8 pad[0x1D]; u8 slot; } Arg82AC;
typedef struct Node { u8 pad[0x20]; struct Node* next; } Node;
typedef struct List { u8 pad[0xC]; Node* head; } List;

extern Item D_80235F00[];
extern void func_800E759C(Obj82AC*, Arg82AC*);

void func_800E8318(List* l, Node* n) {
    Node* p;

    if (l->head == n) {
        l->head = n->next;
    } else {
        for (p = l->head; p != NULL; p = p->next) {
            if (p->next == n) {
                break;
            }
        }
        p->next = n->next;
    }
    n->next = NULL;
}

u16 func_800E8358(List* l) {
    u16 c = 0;
    Node* p;

    for (p = l->head; p != NULL; p = p->next) {
        c++;
    }
    return c;
}

