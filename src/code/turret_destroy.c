/* ---- 0x800E0000/f2018.c ---- */
typedef struct { char pad[0x4C]; unsigned char b4C; unsigned char b4D; } Def;
typedef struct {
    char pad0[0xA]; unsigned short hA; Def *def; void *obj10; char pad14[0x18-0x14]; void *obj18;
    char pad1C[0x24-0x1C]; float x; float y; char pad2C[0x30-0x2C];
    unsigned short h30; unsigned short h32; unsigned short h34; unsigned char id; char p37; unsigned char mode;
} Ent;
extern void func_800DA7D0(void *, float *, unsigned short, unsigned char, int, int, int, int, int);
extern void func_80097FB4(int, float, float, float, unsigned char);
extern void func_800B22F8(unsigned short);

void func_800E2018(Ent *e, unsigned short arg1, unsigned char arg2) {
    if (e->mode == 2) return;
    if (e->mode == 0 && e->obj18 != 0 && arg2 != 0) {
        func_800DA7D0(e->obj18, &e->x, e->h30, e->id, 0, 8, 0, 0, 0);
        func_800DA7D0(e->obj18, &e->x, e->h30 + 0x2000, e->id, 0, 8, 0, 0, 0);
    }
    if (e->mode == 1 && e->obj18 != 0 && arg2 != 0) {
        func_800DA7D0(e->obj18, &e->x, e->h30, e->id, 0, 0x40, 0, 0, 0);
    }
    func_80097FB4(39, e->x, e->y, 1.0f, e->id);
    e->mode = 2;
    e->h32 = arg1;
    e->h34 = 1;
    e->def->b4C = 2;
    e->def->b4D |= 0xF0;
    if (e->hA != 0xFFFF) func_800B22F8(e->hA);
}

