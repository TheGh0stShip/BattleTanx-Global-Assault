/* Normalizer-assisted: retail allocates the fifth parameter to $a3 after saving arg4. */
/* RODATA_VRAM 0x80075DB0 */
typedef struct { char pad[0xA]; unsigned char flags; char p2[1]; float x; float y; char p3[8]; unsigned char id; char p4[0x23]; int busy; } Obj;
typedef struct { int a; int type; } Msg;
typedef struct { unsigned char id; char p[0xF]; int a; int b; } Arg;
typedef struct { unsigned char hit; char p[3]; float x; float y; } Out;
void func_800E4530(Obj *, int, int);
void func_800E48E0(Obj *, Msg *, Arg *);
void func_800E4B84(Obj *, Msg *, Arg *);
void func_800E4DA0(Obj *o0, Msg *m, unsigned int kind, Arg *arg, int *out) {
    Obj *o = o0; Arg *a2 = arg; Msg *m2 = m;
    if (o->flags & 2) return;
    switch (kind) {
    case 0:
        switch (m2->type) {
        case 11: case 35: case 37: case 38: case 50:
            *out = 1;
            func_800E4530(o, a2->a, a2->b);
            break;
        case 4: case 28:
            *out = 1;
            break;
        }
        break;
    case 4:
        if (o->id == a2->id && o->busy == 0) {
            ((Out *)out)->hit = 1;
            ((Out *)out)->x = o->x;
            ((Out *)out)->y = o->y;
        }
        break;
    case 5:
        func_800E48E0(o, m2, a2);
    case 3:
        func_800E4B84(o, m2, a2);
        break;
    }
}
