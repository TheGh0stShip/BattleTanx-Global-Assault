typedef unsigned short u16;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct Task {
    int active;
    int type;
} Task;

typedef struct {
    void (*update)(Task *task, int *result);
    void (*message)(Task *task, void *from, int kind, void *data, void *reply);
    void (*draw)(Task *task);
} TaskClass;

typedef struct {
    Task *data;
    u16 id;
    Vec3 position;
    char pad14[0x24 - 0x14];
} Contact;

typedef struct {
    Vec3 position;
    u16 angle;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
} HitMessage;

typedef struct {
    int status;
    int unk[6];
} Reply;

typedef struct {
    void *owner;
    char pad04[0x10 - 4];
    Vec3 unk10;
    char pad1C[0x20 - 0x1C];
    u16 angle;
    char pad22[0x28 - 0x22];
    Vec3 unk28;
    char pad34[0x1D0 - 0x34];
    int unk1D0;
    char pad1D4[0x1E0 - 0x1D4];
    int flags;
    char pad1E4[0x1F4 - 0x1E4];
    u16 grid;
    char pad1F6[0x4A4 - 0x1F6];
    u16 state;
} Tank;

extern TaskClass D_80224B58[];
extern short D_80397650;

void func_800B88DC(Vec3 *a, u16 grid, Vec3 *b);
u16 func_800B3748(u16 id, int mask, Contact *out, u16 grow, u16 single);
void func_8009060C(Tank *tank, Reply *reply);

/* Normalizer-assisted: the retail allocator cycles three tied saved registers. */
void func_80090C4C(Tank *tank) {
    Contact hits[32];
    HitMessage message;
    Reply reply;
    u16 count;
    u16 index;
    Contact *list;
    Contact *scan;

    if (!(tank->state & 0x2000)) {
        func_800B88DC(&tank->unk28, tank->grid, &tank->unk10);
    }
    D_80397650 = 1;
    list = hits;
    count = func_800B3748(tank->grid, 0xC08F00, list, 0, 0);
    scan = list;
    for (index = 0; index < count; index++) {
        Contact *contact = &scan[index];

        if (contact->data != 0) {
            reply.status = 2;
            message.angle = tank->angle;
            message.position = contact->position;
            message.unk18 = tank->flags & 2;
            message.unk14 = tank->unk1D0;
            message.unk10 = 0;
            message.unk20 = 0;
            if (D_80224B58[contact->data->type].message != 0) {
                D_80224B58[contact->data->type].message(
                    contact->data, tank->owner, 0, &message, &reply
                );
            }
            func_8009060C(tank, &reply);
        }
    }
}
