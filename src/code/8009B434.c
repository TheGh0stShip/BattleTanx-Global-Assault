/* SPAN 0x8009B62C */
typedef unsigned char u8;

typedef struct {
    char p0[0x18];
    u8 team;            /* 0x18 */
} TeamInfo;

typedef struct {
    char p0[0xA];
    u8 fA;              /* 0x0A */
    char pb[0x10 - 0xB];
    TeamInfo *info;     /* 0x10 */
    char p14[0x74 - 0x14];
    int state;          /* 0x74 */
    char p78[0x250 - 0x78];
} Unit;

extern u8 D_802194A6;
extern Unit D_80235F00[];

void func_80102F10(void *p, int n);
void func_800A96B8(Unit *u);

int func_8009B434(void) {
    int seen[5];
    Unit *u;
    int i;
    int n = 0;

    func_80102F10(seen, sizeof(seen));
    for (i = 0; i < D_802194A6; i++) {
        if (i == 127) {
            u = 0;
        } else {
            u = &D_80235F00[i];
        }
        func_800A96B8(u);
        if (u->state != 2 && seen[u->info->team] == 0) {
            seen[u->info->team] = 1;
            n++;
        }
    }
    return (n < 2) * 2;
}

int func_8009B534(int strict) {
    Unit *u;
    int i;
    int a = 0;
    int b = 0;

    for (i = 0; i < D_802194A6; i++) {
        if (i == 127) {
            u = 0;
        } else {
            u = &D_80235F00[i];
        }
        func_800A96B8(u);
        if (u->state != 2) {
            if (u->fA & 2) {
                b++;
            } else {
                a++;
            }
        }
    }
    if (b == 0 || ((a == 0) & (strict != 0))) {
        return 2;
    }
    return 0;
}
