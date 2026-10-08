typedef struct {
    int active;
    void *obj;
    int pad8;
    unsigned short type;
    unsigned char pade;
    unsigned char idx;
} Ev;
typedef struct {
    int pad0;
    int pad4;
    unsigned int buttons;
} Pad;
extern Ev D_803A66C0[];
extern int D_8021949C;
extern int D_803A6664;
extern int D_80117EB4;
extern void func_800D25E0(Ev *);
extern void func_800D1C90(Ev *);
extern void func_800D2B44(Ev *);
extern void func_800D31DC(Ev *);
extern void func_800D3554(Ev *);
extern void func_800D37D0(Ev *);
extern void func_800D39F8(Ev *);
extern void func_800D3C64(Ev *);
extern void func_800D404C(Ev *);
extern void func_800D441C(Ev *);
extern void func_800D53D4(Ev *);
extern Pad *func_80098250(int);

int func_800D5508(void)
{
    unsigned short i;
    unsigned short done;
    Ev *e;
    Pad *p;

    done = 1;
    for (i = 0; i < 50; i++) {
        e = &D_803A66C0[i];
        if (e->active != 0) {
        switch (e->type) {
        case 0:
            func_800D25E0(e);
            done &= (e->active != 0) ? 0xFFFF : 0;
            break;
        case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
        case 18: case 19: case 20: case 21: case 22: case 23: case 24:
            func_800D1C90(e);
            break;
        case 30: case 31:
            func_800D2B44(e);
            break;
        case 40:
            func_800D31DC(e);
            break;
        case 41:
            func_800D3554(e);
            break;
        case 1: case 2:
            func_800D37D0(e);
            break;
        case 32:
            func_800D39F8(e);
            break;
        case 33:
            func_800D3C64(e);
            break;
        case 34:
            func_800D404C(e);
            break;
        case 50: case 51: case 52: case 53:
            func_800D441C(e);
            if (D_8021949C == 25)
                done &= (e->active != 0) ? 0xFFFF : 0;
            break;
        }
        }
    }
    if (done == 0) {
        D_80117EB4 = D_803A6664;
        for (i = 0; i < 50; i++)
            func_800D53D4(&D_803A66C0[i]);
        return 1;
    }
    p = func_80098250(0);
    if (p->buttons & 0x8000) {
        D_80117EB4 = D_803A6664;
        return 2;
    }
    if (p->buttons & 0x1000) {
        D_80117EB4 = D_803A6664;
        return 3;
    }
    return 0;
}
