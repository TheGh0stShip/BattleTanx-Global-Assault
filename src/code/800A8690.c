/* SPAN 0x800A87BC */
typedef unsigned char u8;

typedef struct {
    char p0[8];
    unsigned int pressed;   /* 0x08 */
} Pad;

typedef struct {
    char p0[0xA];
    u8 flags;           /* 0x0A */
    u8 pad;             /* 0x0B */
    char pC[0x78 - 0xC];
    char cam[0x178 - 0x78]; /* 0x78 */
    unsigned int fireMask;  /* 0x178 */
    unsigned int altMask;   /* 0x17C */
    char p180[0x250 - 0x180];
} Unit;

extern Unit D_80235F00[];
extern int D_80117EB4;
extern int D_802195C8;
extern int D_802195CC;

Pad *func_80098250();
void func_800A8E84(Unit *u);
void func_800A9660(Unit *u);
void func_80097B20(int fade);
void func_800C726C(void);
void func_800A6588(void *cam);

static inline Unit *getUnit(int i) {
    if (i == 127) {
        return 0;
    }
    return &D_80235F00[i];
}

void func_800A8690(int id) {
    Unit *u = getUnit(id);
    Pad *p;

    if ((u->flags & 2) && D_80117EB4 != 10) {
        p = func_80098250();
        if (p->pressed & u->fireMask) {
            func_800A8E84(u);
        }
        if ((p->pressed & u->altMask) && D_802195CC != 5) {
            func_800A9660(u);
        }
        if ((p->pressed & 0x1000) && D_802195C8 != 5 && D_80117EB4 != 10) {
            func_80097B20(2);
            func_800C726C();
            D_802195C8 = 5;
        }
    }
    if (u->flags & 1) {
        func_800A6588(u->cam);
    }
}
