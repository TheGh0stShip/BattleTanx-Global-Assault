/* SPAN 0x800BDAF8 */
#include "types.h"

typedef struct ListNode800BDA30 {
    u8 active;
    u8 flags;
    u8 pad02[0x0E];
} ListNode800BDA30;

typedef struct ListOwner800BDA30 {
    u8 pad00[4];
    ListNode800BDA30 *nodes;
} ListOwner800BDA30;

static __inline__ ListNode800BDA30 *find_selected(ListOwner800BDA30 *owner) {
    ListNode800BDA30 *node = owner->nodes;

    while (node->active != 0) {
        if (node->flags & 0x80) {
            return node;
        }
        node++;
    }
    return 0;
}

static __inline__ ListNode800BDA30 *find_next(ListNode800BDA30 *node) {
    while (node->active != 0) {
        if (node->flags & 0x10) {
            return node;
        }
        node++;
    }
    return 0;
}

static __inline__ void select_node(ListNode800BDA30 *previous,
                                   ListNode800BDA30 *current) {
    previous->flags &= 0x7F;
    current->flags |= 0x80;
}

void func_800BDA30(ListOwner800BDA30 *owner) {
    ListNode800BDA30 *selected = find_selected(owner);

    if (selected != 0) {
        ListNode800BDA30 *next = find_next(selected + 1);

        if (next == 0) {
            next = find_next(owner->nodes);
        }
        select_node(selected, next);
    }
}
