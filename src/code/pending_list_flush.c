/* ---- 0x800E4800/src/func_800E759C.c ---- */
#include "types.h"
#define NULL 0

typedef struct Node { u8 pad[0x20]; struct Node* next; } Node;
typedef struct Owner759C { u8 pad[0xA0]; Node* pending; } Owner759C;

extern void func_800E66A8(Node*, s32);

void func_800E759C(Owner759C* o, s32 arg) {
    Node* n;
    while (o->pending != NULL) {
        n = o->pending;
        o->pending = n->next;
        func_800E66A8(n, arg);
    }
}

