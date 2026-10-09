/* SPAN 0x80082834 */
typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { char p0[8]; float pos[3]; char p14[0xF4 - 0x14]; u16 node; char pf6[0x128 - 0xF6]; Pt goal; u16 at; short step; } Ent;
Pt *func_80082834(Ent *);
void func_8009DA44(Pt *, float *, Pt *);
int func_8007D628(float *, Pt *, Pt *);
int func_80082870(Ent *, Pt *);
void func_8007E7A8(Ent *);

inline Pt *func_800825A4(Ent *e) {
    Pt *p = func_80082834(e);
    Pt d;
    Pt t;

    if (p == 0) {
    e->step = 0;
    e->at = e->node;
    p = func_80082834(e);
    if (p != 0) {
        func_8009DA44(&d, e->pos, p);
        t.x = p->x + d.y;
        t.y = p->y - d.x;
        if (func_8007D628(e->pos, &t, p) == 1) {
            e->goal.x = t.x;
            e->goal.y = t.y;
        } else {
            e->goal.x = p->x - d.y;
            e->goal.y = p->y + d.x;
        }
    }
    }
    return p;
}
void func_8008268C(Ent *e) {
    Pt *p = func_800825A4(e);

    if (func_80082870(e, p) != 0) {
        func_8007E7A8(e);
        func_800825A4(e);
    }
}
