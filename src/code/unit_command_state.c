/* SPAN 0x800873C8 */
/* RODATA_VRAM 0x800715C0 */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { int op; void *fn; short n; } Cmd;
typedef struct {
    char p0[0x18]; unsigned int state; Cmd q[4]; u16 count; char p4e[0x68 - 0x4E]; int timer; u16 val; u8 blink;
} Unit;
extern char *D_80114680;
extern struct { int v; } D_802194A0;
extern int D_8021945C;
void func_80086CEC();

static inline void push(Unit *o, int op, void *fn, int n) {
    Cmd *c = &o->q[o->count];
    c->op = op;
    c->n = n;
    c->fn = fn;
    o->count++;
}
#define PUSH push

int func_8008723C(Unit *o, unsigned int state, int *arg, int narg) {
    int changed = o->state != state;
    o->state = state;
    o->count = 0;
    switch (state) {
    case 0: case 1:
        break;
    case 2:
        PUSH(o, 6, 0, 24);
        break;
    case 3:
        if (narg) o->timer = *arg;
        PUSH(o, 18, 0, 24);
        break;
    case 4:
        if (narg) o->timer = *arg;
        PUSH(o, 19, func_80086CEC, 1);
        PUSH(o, 6, 0, 24);
        break;
    case 5:
        if (narg) o->timer = *arg;
        PUSH(o, 17, 0, 24);
        break;
    case 6:
        if (narg) o->timer = *arg;
        break;
    case 7:
        o->timer = D_8021945C + 150;
        o->val = *(u16 *)(D_80114680 + 0xC06E);
        o->blink = 0;
        if (D_802194A0.v != 10) {
            PUSH(o, 9, 0, 1);
            PUSH(o, 6, 0, 24);
        }
        break;
    }
    return changed;
}
