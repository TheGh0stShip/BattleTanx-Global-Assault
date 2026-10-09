/* SPAN 0x800866A4 */
/* RODATA_VRAM 0x800713F0 */
typedef unsigned char u8;
typedef struct { float x, y; } Pt;
typedef struct { char p0[0x50]; Pt pts[3]; char p68[0x1C9 - 0x68]; u8 a; u8 b; char p1cb[0x250 - 0x1CB]; } Obj;
typedef struct { char p0[0x95]; u8 wp; char p96[0x168 - 0x96]; unsigned int mode; } Tank;
extern Obj D_80235F00[];
void func_80088720(Tank *);
void func_80082D10(Tank *);
void func_80082D98(Tank *, int);
void func_80082F98(Tank *);
void func_80082FE8(Tank *);
void func_800830B4(Tank *);
void func_8008341C(Tank *);
void func_80084704(Tank *);
void func_80083188(Tank *, int, int);
void func_800852F8(Tank *);
void func_8008355C(Tank *);
void func_80085F7C(Tank *, int);
void func_80083C70(Tank *);
void func_80083DA8(Tank *);
void func_80084224(Tank *);
void func_80088B6C(Tank *, Pt *, float, int);
void func_80088FD4(Tank *);
void func_80085DA8(Tank *, int, int);

static inline Obj *wp_at(int i) { return &D_80235F00[i]; }
static inline Obj *waypoint(int i) {
    if (i == 127) return 0;
    return wp_at(i);
}
void func_80086208(Tank *e, int mode) {
    e->mode = mode;
    switch (mode) {
    case 0:
        func_80088720(e);
        break;
    case 1:
        func_80082D10(e);
        break;
    case 23: {
        Obj *w = waypoint(e->wp);
        if ((w->a + w->b) & 1) {
            func_80086208(e, 2);
        } else {
            func_80086208(e, 3);
        }
        break;
    }
    case 2:
        func_80082D98(e, 0);
        break;
    case 3:
        func_80082D98(e, 1);
        break;
    case 4:
        func_80082F98(e);
        break;
    case 5:
        func_80082FE8(e);
        break;
    case 6:
        func_800830B4(e);
        break;
    case 9:
        func_8008341C(e);
        break;
    case 21:
        func_80084704(e);
        break;
    case 7:
        func_80083188(e, 1, 6);
        break;
    case 8:
        func_80083188(e, 3, 6);
        break;
    case 22:
        func_800852F8(e);
        break;
    case 10:
        func_8008355C(e);
        break;
    case 11:
        func_80085F7C(e, 2);
        break;
    case 12:
        func_80085F7C(e, 1);
        break;
    case 13:
        func_80085F7C(e, 4);
        break;
    case 14:
        func_80083188(e, 3, 11);
        break;
    case 15:
        func_80083188(e, 3, 12);
        break;
    case 16:
        func_80083188(e, 3, 13);
        break;
    case 17:
        func_80083C70(e);
        break;
    case 18:
        func_80083DA8(e);
        break;
    case 19:
        func_80084224(e);
        break;
    case 28:
        func_80088B6C(e, &waypoint(e->wp)->pts[0], 400.0f, 1);
        func_80088FD4(e);
        break;
    case 29:
        func_80088B6C(e, &waypoint(e->wp)->pts[1], 400.0f, 1);
        func_80088FD4(e);
        break;
    case 30:
        func_80088B6C(e, &waypoint(e->wp)->pts[2], 400.0f, 1);
        func_80088FD4(e);
        break;
    case 26:
        func_80085DA8(e, 120, 0);
        break;
    case 27:
        func_80085DA8(e, 0, 1);
        break;
    }
}

void func_80082D60(Tank *);
void func_80082F1C(Tank *);
void func_80082FE0(Tank *);
void func_80083028(Tank *);
void func_800830F8(Tank *);
void func_80083464(Tank *);
void func_8008474C(Tank *);
void func_800831D0(Tank *);
void func_800854C4(Tank *);
void func_80083A80(Tank *);
void func_80086034(Tank *);
void func_80083CC0(Tank *);
void func_80083DF0(Tank *);
void func_80084278(Tank *);
void func_800859E4(Tank *);
void func_80085D58(Tank *);
void func_80085EB0(Tank *);

void func_80086560(Tank *e) {
    switch (e->mode) {
    case 0:
        break;
    case 1:
        func_80082D60(e);
        break;
    case 2: case 3:
        func_80082F1C(e);
        break;
    case 4:
        func_80082FE0(e);
        break;
    case 5:
        func_80083028(e);
        break;
    case 6:
        func_800830F8(e);
        break;
    case 9:
        func_80083464(e);
        break;
    case 21:
        func_8008474C(e);
        break;
    case 7: case 8: case 14: case 15: case 16:
        func_800831D0(e);
        break;
    case 22:
        func_800854C4(e);
        break;
    case 10:
        func_80083A80(e);
        break;
    case 11: case 12: case 13:
        func_80086034(e);
        break;
    case 17:
        func_80083CC0(e);
        break;
    case 18:
        func_80083DF0(e);
        break;
    case 19:
        func_80084278(e);
        break;
    case 24:
        func_800859E4(e);
        break;
    case 25:
        func_80085D58(e);
        break;
    case 26: case 27:
        func_80085EB0(e);
        break;
    case 30:
        break;
    }
}
