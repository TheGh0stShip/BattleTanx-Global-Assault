/* SPAN 0x8008A1A4 */
/* RODATA_VRAM 0x800718C0 */
typedef unsigned char u8;
typedef struct { char p0[0x18]; int mode; } Spawn;
typedef struct { char p0[0x10]; int team; char p14[0x1F0 - 0x14]; int slot; } Owner;
typedef struct {
    char p0[8]; float x; float y; char p10[0x98 - 0x10]; int zone; char p9c[0xA4 - 0x9C]; int bit;
    char pa8[0xB0 - 0xA8]; int bb0; char pb4[0xD0 - 0xB4]; char pd0[0x150 - 0xD0]; float lx; float ly; char p158[0x168 - 0x158];
    char p168[4]; u8 flags; char p16d[0x1D0 - 0x16D]; Owner *owner; char p1d4[0x1E0 - 0x1D4]; int ff;
} Tank;
extern char D_80125C30[];
extern int D_801ACEF0;
extern void *D_80114680;
void func_80088000(void *);
void func_80088C70(void *);
void func_80082B40(void *);
void func_80086208(Tank *, int);
void func_80086160(void *);
void func_80082A90(Tank *, void *);
void func_8007E170(Tank *);
void func_80080BC0(Tank *);
void func_8008616C(Tank *, int);
void func_80089C90(Tank *);
void func_8007E988(Tank *);
void func_80082560(Tank *);
void func_80082070(Tank *);

void func_80089F40(Tank *t, unsigned int from, unsigned int to, Spawn *sp) {
    int wasA = 0;
    int isA = 0;
    int wasB = 0;
    int isB = 0;
    int m;

    if (from == to) return;
    switch (from) {
    case 4:
        t->bb0 = 0;
        func_80088000(t->p0 + 0xB4);
        func_80088C70(t->p0 + 0xD0);
        func_80082B40(t->p0 + 0x158);
        t->flags = 0;
        if (sp == 0) {
            if (t->zone == 11) {
                t->flags = 0x20;
            } else {
                func_80086208(t, 27);
            }
        } else {
            func_80086160(t->p168);
            func_80082A90(t, D_80125C30 + t->owner->slot * 0x18);
            func_8007E170(t);
            func_80080BC0(t);
            t->lx = t->x;
            t->ly = t->y;
            if (sp->mode == 7 || sp->mode == 8 || sp->mode == 14 || sp->mode == 15 || sp->mode == 16) {
                func_80086208(t, sp->mode);
            } else {
                func_8008616C(t, sp->mode);
            }
        }
        break;
    case 1:
    case 2:
        wasA = 1;
        break;
    case 0:
        wasB = 1;
        break;
    case 3:
        wasB = t->ff & 2;
        break;
    }
    switch (to) {
    case 4:
        func_80089C90(t);
        if (D_80114680 != 0) func_8007E988(t);
        break;
    case 1:
    case 2:
        isA = 1;
        break;
    case 0:
        isB = 1;
        break;
    case 3:
        isB = t->ff & 2;
        break;
    }
    if (wasA != isA) {
        if (wasA) func_80082560(t);
        else func_80082070(t);
    }
    if (wasB != isB) {
        if (wasB) D_801ACEF0 &= ~t->bit;
        else D_801ACEF0 |= t->bit;
    }
}
