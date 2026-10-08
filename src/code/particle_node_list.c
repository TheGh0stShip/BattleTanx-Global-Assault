/* ---- 0x800ED800/tu/dlist_800F0930.c ---- */
#include "types.h"
#define NULL ((void *)0)
typedef struct N { struct N *next, *prev; } N;
void func_800F0930(N **head, N *n, N *at) {
    if (at != NULL) {
        n->next = at; n->prev = at->prev; at->prev = n;
        if (n->prev != NULL) n->prev->next = n;
        if (*head == at) *head = n;
    } else { n->prev = NULL; n->next = NULL; *head = n; }
}
void func_800F0978(N **head, N *n) {
    if (*head == n) *head = n->next;
    else if (n->prev != NULL) n->prev->next = n->next;
    if (n->next != NULL) n->next->prev = n->prev;
}

/* ---- 0x800ED800/c/f_800F09C0.c ---- */
extern void func_8009ED00(unsigned int rom, void *ram, int size);
extern char D_B0102068[], D_B0102468[], D_B0102868[];
extern char D_803AA750[], D_803AAB50[], D_803AAF50[];

void func_800F09C0(void) {
    func_8009ED00((unsigned int)D_B0102068, D_803AA750, 0x400);
    func_8009ED00((unsigned int)D_B0102468, D_803AAB50, 0x400);
    func_8009ED00((unsigned int)D_B0102868, D_803AAF50, 0x400);
}

