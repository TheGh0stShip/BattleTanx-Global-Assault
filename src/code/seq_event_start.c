typedef struct {
    int active;
    void *obj;
    float time;
    unsigned short type;
    unsigned char pade;
    unsigned char idx;
} Ev;
extern Ev D_803A66C0[];
extern float D_80219488;
extern unsigned char D_803A6A04;
extern unsigned char D_803A6A00;
extern unsigned short func_800D12B0(int, unsigned short);
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

unsigned short func_800D4B58(int arg0)
{
    int script;
    unsigned short i;
    unsigned short type;
    Ev *e;

    for (i = 0; i < 50; i++) {
        if (D_803A66C0[i].active == 0)
            break;
    }
    script = arg0;
    e = &D_803A66C0[i];
    e->type = 0xFFFF;
    e->obj = 0;
    e->active = script;
    D_803A6A04 = 255;
    D_803A6A00 = 0;
    e->time = D_80219488;
    do {
        script += func_800D12B0(script, e->type);
    } while (D_803A6A04 != 8 && D_803A6A04 != 0);
    if (D_803A6A04 == 0)
        return 0xFFFF;
    type = D_803A6A00;
    switch (type) {
    case 0: func_800D25E0(e); break;
    case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17:
    case 18: case 19: case 20: case 21: case 22: case 23: case 24:
        func_800D1C90(e); break;
    case 30: case 31: func_800D2B44(e); break;
    case 40: func_800D31DC(e); break;
    case 41: func_800D3554(e); break;
    case 1: case 2: func_800D37D0(e); break;
    case 32: func_800D39F8(e); break;
    case 33: func_800D3C64(e); break;
    case 34: func_800D404C(e); break;
    case 50: case 51: case 52: case 53: func_800D441C(e); break;
    }
    return type;
}
