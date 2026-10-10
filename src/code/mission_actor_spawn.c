/* RODATA_VRAM 0x800761C0 */
typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct {
    char pad0[10];
    unsigned char status;
    char pad1;
    short handle;
    char pad2[2];
    Vec3 position;
    unsigned char type;
    char pad3;
    unsigned short value;
    int state;
} MissionActor;

extern int D_802194A0;
extern MissionActor *func_800A18D0(int, int);
extern short func_800B1898(MissionActor *, short, short, short, int, int,
                           int, int, int, int, int, int, int);
extern void func_800C98D8(void *);

void func_800E9990(short x, short z, short y, short heading, short arg4,
                   short arg5, short arg6, short arg7, short arg8,
                   unsigned short value, unsigned char type, int status) {
    MissionActor *actor;

    switch (D_802194A0) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 8:
    case 11:
    case 12:
    case 13:
        return;
    }

    actor = func_800A18D0(19, 36);
    if (actor == 0) {
        return;
    }

    actor->handle = func_800B1898(actor, x, y, z, heading, arg4, arg7, arg8,
                                  arg5, arg6, value, 0x10000, type);
    actor->position.z = z;
    actor->value = value;
    actor->position.y = y;
    actor->position.x = x;
    actor->type = type;
    actor->state = 0;
    actor->status = status;

    if (D_802194A0 == 14 && actor->status == 0) {
        func_800C98D8((char *)actor + 0x20);
    }
}
