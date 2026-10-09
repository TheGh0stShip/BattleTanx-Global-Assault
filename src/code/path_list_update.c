/* SPAN 0x80080AF4 */
typedef struct { unsigned char type; char p1; unsigned short next; char p4[4]; int w8; short c; unsigned short e; char p10[6]; unsigned short prev; } Node;
typedef struct { Node *cur; unsigned short head; unsigned short count; char p8[2]; short hA; char pC[8]; short h14; unsigned short h16; int h18; } List;
typedef struct { char pad[0xF0]; List list; char p10C[0x132 - 0x10C]; short h132; } Obj;
typedef struct { char pad[0xC008]; unsigned short limit; } Pool;
extern Pool *D_80114680;
extern Node *func_8007DA5C(unsigned short);
extern unsigned short func_8007DE3C(unsigned short *);
extern unsigned short func_8007DBE0(unsigned short *);

void func_80080920(Obj *o) {
    List *l = &o->list;
    Node *cur = l->cur;
    Node *pn;
    Node *n;
    Node *t;
    unsigned short key;
    unsigned short pi;
    unsigned short ni;
    unsigned short lim;
    unsigned short k;

    ni = 0;
    o->h132 = 0;
    if (cur == 0) {
        key = l->h16;
        if (l->h16 == 0) {
            pi = 0;
            pn = 0;
        } else {
            pi = func_8007DA5C(key)->next;
            pn = func_8007DA5C(pi);
            ni = pn->next;
            l->count -= func_8007DE3C(&pn->prev);
        }
    } else {
        pi = cur->next;
        pn = func_8007DA5C(pi);
        ni = pn->next;
        cur->w8 = 0;
    }
    l->hA = 0;
    l->h14 = 0;
    l->h16 = 0;
    l->h18 = 0;
    while (ni != 0) {
        n = func_8007DA5C(ni);
        if (n->type == 2) {
            key = ni;
            ni = n->next;
            t = n;
            n = func_8007DA5C(ni);
            pn->next = ni;
            n->prev = pi;
            if (t->e == pi) {
                t->e = 0;
            } else {
                t->c = 0;
            }
            l->count -= func_8007DBE0(&key);
        }
        pi = ni;
        pn = n;
        ni = pn->next;
    }
    lim = D_80114680->limit;
    if (l->count > lim) {
        Node *w;

        w = func_8007DA5C(l->head);
        for (k = 1; k < lim; k++) {
            w = func_8007DA5C(w->prev);
        }
        l->count -= func_8007DBE0(&w->prev);
    }
}
