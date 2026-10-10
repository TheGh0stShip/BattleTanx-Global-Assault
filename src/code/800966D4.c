/* SPAN 0x800967F0 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char p0[0xA];
    u8 fA;              /* 0x0A */
    char pb[0x78 - 0xB];
    char cam[0x90 - 0x78]; /* 0x78 */
    int target;         /* 0x90 */
    char p94[0x114 - 0x94];
    int mode;           /* 0x114 */
} Unit;

typedef struct {
    int id;             /* 0x00 */
    char p4[0x90 - 4];
    int f90;            /* 0x90 */
    char p94[4];
    int zone;           /* 0x98 */
    char p9c[0x1D0 - 0x9C];
    Unit *owner;        /* 0x1D0 */
    char p1d4[0x1E0 - 0x1D4];
    int flags;          /* 0x1E0 */
    char p1e4[0x1F4 - 0x1E4];
    u16 node;           /* 0x1F4 */
    char p1f6[0x248 - 0x1F6];
    int deathTime;      /* 0x248 */
    int f24C;           /* 0x24C */
    char p250[0x280 - 0x250];
    int sndEngine;      /* 0x280 */
    int sndSkid;        /* 0x284 */
    int sndBoost;       /* 0x288 */
    char p28c[0x4C4 - 0x28C];
    int killer;         /* 0x4C4 */
} Tank;

typedef struct { char p[0x30]; } CamPreset;

extern int D_8021945C;
extern u8 D_802194A5;
extern CamPreset D_80121D90[];
extern CamPreset D_80122060[];

void func_80097BA4(int h, int a);
void func_800B22F8(u16 id);
void func_800A6ADC(void *dst, CamPreset *p, void *src);

void func_800966D4(Tank *t, int killer) {
    Unit *o;

    t->f24C = 0;
    t->flags |= 8;
    t->deathTime = D_8021945C;
    if (t->sndBoost != 0) {
        func_80097BA4(t->sndBoost, 0);
        t->sndBoost = 0;
    }
    if (t->sndEngine != 0) {
        func_80097BA4(t->sndEngine, 0);
        t->sndEngine = 0;
    }
    if (t->sndSkid != 0) {
        func_80097BA4(t->sndSkid, 0);
        t->sndSkid = 0;
    }
    func_800B22F8(t->node);
    t->node = 0xFFFF;
    t->f90 = 0;
    o = t->owner;
    if ((o->fA & 1) && o->mode == 1 && o->target == t->id) {
        char *cam = o->cam;

        func_800A6ADC(cam, &(D_802194A5 >= 2 ? D_80122060 : D_80121D90)[t->zone], cam);
    }
    t->killer = killer;
}
