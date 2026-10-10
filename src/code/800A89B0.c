/* SPAN 0x800A8A78 */
/* RODATA_VRAM 0x80072D00 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u16 kills;
} TeamInfo;

typedef struct {
    char p0[0x18];
} CamView;

typedef struct {
    char p0[0xB];
    u8 hud;             /* 0x0B */
    char pC[0x10 - 0xC];
    TeamInfo *info;     /* 0x10 */
    char p14[0x78 - 0x14];
    char cam[0x114 - 0x78]; /* 0x78 */
    int view;           /* 0x114 */
    char p118[0x210 - 0x118];
    u16 kills;          /* 0x210 */
    char p212[0x250 - 0x212];
} Unit;

typedef struct {
    char p0[0x98];
    int row;            /* 0x98 */
} ViewSel;

extern int D_80117EB4;
extern u8 D_802194A5;
extern Unit D_80235F00[];
extern CamView D_80121D90[][2];

int sprintf(char *buf, const char *fmt, ...);
void func_800CA620(u8 hud, char *msg, int time);
void func_800A6ADC(void *cam, CamView *v);
void func_800A6B5C(void *cam, int mode);

static inline Unit *getUnit(int i) {
    if (i == 127) {
        return 0;
    }
    return &D_80235F00[i];
}


void func_800A8A78(Unit *u, ViewSel *s);


void func_800A89B0(int id, int n) {
    char buf[128];
    Unit *u = getUnit(id);

    u->info->kills += n;
    u->kills += n;
    if (D_80117EB4 != 10) {
        if (u->info->kills == 1) {
            func_800CA620(u->hud, "1 ENEMY KILLED", 45);
        } else {
            sprintf(buf, "%i ENEMIES KILLED", u->info->kills);
            func_800CA620(u->hud, buf, 45);
        }
    }
}
