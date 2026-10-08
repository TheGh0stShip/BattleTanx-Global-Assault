/* ---- 0x800ED800/tu/dlist_800F0618.c ---- */
#include "types.h"
#define NULL ((void *)0)
typedef struct N { struct N *next, *prev; } N;
N *func_800F0618(N **head) {
    N *n;
    if (*head != NULL) {
        n = *head;
        *head = n->next; n->prev = NULL; n->next = NULL;
        return n;
    }
    return NULL;
}
void func_800F0644(N **head, N *n) { N *h = *head; n->prev = NULL; n->next = h; *head = n; }

