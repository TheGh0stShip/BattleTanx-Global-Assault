/* RODATA_VRAM 0x800716A0 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char pad0[0x10];
    int team;
    char pad14[0x250 - 0x14];
} Obj;

typedef struct Tank {
    char pad0[0x94];
    u8 field94;
    char pad95[0xB4 - 0x95];
    int fieldB4;
    int fieldB8;
    float fieldBC;
    int timer;
    u8 fieldC4;
    u8 mode;
    char padC6[2];
    struct Tank *tracked;
    char padCC[0x16C - 0xCC];
    u8 flags;
    char pad16D[0x1C0 - 0x16D];
    int field1C0;
    char pad1C4[0x1D0 - 0x1C4];
    struct {
        char pad0[0x10];
        int team;
    } *owner;
    char pad1D4[0x1E0 - 0x1D4];
    int state;
} Tank;

extern Obj D_80235F00[];
extern int D_8021945C;

void *func_800A9928(Obj *, int, int, int, Tank *);
void func_80088ABC(Tank *, void *, int, int);

static inline Obj *waypoint_at(int index) {
    return &D_80235F00[index];
}

static inline Obj *waypoint(int index) {
    if (index == 127) {
        return 0;
    }
    return waypoint_at(index);
}

static inline void *scan_targets(Tank *tank, int team) {
    void *result = 0;
    u16 i;

    for (i = 0; i < 5; i++) {
        Obj *candidate = waypoint(i);
        if (candidate->team == team) {
            continue;
        }
        if ((result = func_800A9928(candidate, 0, 3, 128, tank)) != 0) {
            break;
        }
        if ((result = func_800A9928(candidate, 0, 1, 128, tank)) != 0) {
            break;
        }
    }
    return result;
}

void func_80088854(Tank *tank) {
    Tank *tracked;
    void *result;

    if (tank->mode == 1 && (tracked = tank->tracked) != 0) {
        if (tracked->state & 1) {
            if (!(tracked->state & 0x80) && tracked->field94 == tank->field94 &&
                tracked->owner->team != tank->owner->team) {
                if (tracked->state & 2) {
                    return;
                }
                result = scan_targets(tank, tank->owner->team);
                if (result == 0) {
                    return;
                }
                func_80088ABC(tank, result, tank->field1C0, 1);
                return;
            }
        }
    }
    result = scan_targets(tank, tank->owner->team);
    if (result == 0) {
        tank->fieldB4 = 0;
        tank->fieldB8 = 0;
        tank->fieldBC = 64000.0f;
        tank->timer = D_8021945C + 300;
        tank->fieldC4 = 0;
        tank->mode = 0;
        tank->flags &= ~1;
        return;
    }
    func_80088ABC(tank, result, tank->field1C0, 1);
}
