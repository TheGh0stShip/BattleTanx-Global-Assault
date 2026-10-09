/* SPAN 0x8008F78C */
typedef struct { int value; char pad[0x20]; } Rec;
typedef struct { short v[6]; char pad[0xD0 - 12]; } VSpawn;
extern unsigned short D_80397650;
extern VSpawn D_80122E3C[];
extern unsigned short func_800B3748(unsigned short, int, Rec *, int, int);
extern int func_800B0364(float, float, unsigned char);
extern int func_800B02D4(float, float, unsigned char);
extern unsigned short func_800B205C(int, short, short, int, int, int, int, int, int, int, unsigned short, int, unsigned char);
extern void func_800A2B9C(unsigned short);

int func_8008F3FC(unsigned short id, int mask, int value) {
    Rec recs[32];
    Rec *p;
    int n;
    int i;

    if (value != 0) {
        p = recs;
        D_80397650 = 0;
        n = func_800B3748(id, mask, p, 0, 0);
        for (i = 0; i < n; i++) {
            if (p[i].value != value) {
                return 1;
            }
        }
        return 0;
    }
    D_80397650 = 0;
    return func_800B3748(id, mask, recs, 0, 1) != 0;
}
static inline int occupied(unsigned short id, int mask, int value) {
    Rec recs[32];
    int n;
    int i;

    if (value != 0) {
        D_80397650 = 0;
        n = func_800B3748(id, mask, recs, 0, 0);
        for (i = 0; i < n; i++) {
            if (recs[i].value != value) {
                return 1;
            }
        }
        return 0;
    }
    D_80397650 = 0;
    return func_800B3748(id, mask, recs, 0, 1) != 0;
}

int func_8008F4AC(int kind, short x, short y, unsigned short ang, unsigned char team, unsigned char single, unsigned char check, int value) {
    unsigned short id;

    if (check && !(func_800B0364(x, y, team) && func_800B02D4(x, y, team))) {
        return 0;
    }
    id = func_800B205C(0, x, y, 0, D_80122E3C[kind].v[0], D_80122E3C[kind].v[1], D_80122E3C[kind].v[2],
                       D_80122E3C[kind].v[3], D_80122E3C[kind].v[4], D_80122E3C[kind].v[5], ang, 8, team);
    if (single) {
        if (occupied(id, 0x240008, value)) {
            return 0;
        }
        func_800A2B9C(id);
        return 1;
    }
    return !occupied(id, check ? 0x164510B : 0x64510B, value);
}
