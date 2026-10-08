/* ---- 0x800E9000/r2c/ebca8.c ---- */
typedef unsigned short u16;
typedef struct { char pad[0x250]; } Ent;
extern Ent D_80235F00[];
extern u16 func_8009E9C8(void *, int *);
extern void func_8009EEE0(void *);
extern void func_8009EFD4(void *, int, int, int, int);
extern void func_8007B1F0(int, int, int, int, int, void *, int, int, int, int);

void func_800EBCA8(int idx, int a1, int a2, int a3, int a4, int *p) {
    struct { float m[16]; int z; } m;
    char *e;
    u16 r;

    if (idx == 127) {
        e = 0;
    } else {
        e = (char *)&D_80235F00[idx];
    }
    r = func_8009E9C8(e + 0xA8, p);
    func_8009EEE0(&m);
    func_8009EFD4(&m, p[0], p[2], p[1], r);
    m.z = 0;
    func_8007B1F0(a2, a1, a3, 0, 0, &m, 0, idx, 1, 1);
}

