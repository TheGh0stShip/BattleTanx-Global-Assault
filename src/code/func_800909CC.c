/* RODATA_VRAM 0x80071E54 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef int s32;
typedef float f32;

typedef struct { f32 x, y; } Vec2;
typedef struct { f32 x, y, z; } Vec3;

typedef struct {
    s32 id;
    s32 type;
    char pad8[4];
    s32 state;
} Entity;

typedef struct {
    void (*callback)(Entity *, Entity *, s32, void *, void *);
    s32 pad[2];
} EntityType;

typedef struct {
    Entity *entity;
    u16 id;
    Vec3 pos;
    char pad14[0x24 - 0x14];
} Contact;

typedef struct { char pad[0x38]; } Mover;

typedef struct {
    Entity *entity;
    char pad4[4];
    Vec2 pos;
    f32 height;
    Vec2 previous;
    char pad1C[4];
    u16 angle;
    char pad22[6];
    Mover mover;
    f32 scale;
    char pad64[0x90 - 0x64];
    s32 mode;
    char pad94[0x1D0 - 0x94];
    void *owner;
    char pad1D4[0xC];
    s32 flags;
    char pad1E4[0x10];
    u16 index;
} Tank;

typedef struct {
    Vec3 pos;
    u16 angle;
    s32 value10;
    void *owner;
    s32 flags;
    s32 value1C;
    s32 value20;
    s32 value24;
} HitInfo;

typedef struct {
    s32 type;
    s32 pad4;
    s32 amount;
    s32 source;
    Vec2 pos;
    s32 pad18;
    s32 pad1C;
} Event;

extern EntityType D_80224B5C[];
extern s16 D_80397650;

u16 func_800B74B4(Mover *mover, f32 speed, u16 angle, Vec2 *pos,
                   Vec2 *previous, u16 index, Entity *entity, u16 *hit);
void func_800B69F4(Mover *mover, f32 speed, u16 angle, u16 tankAngle);
void func_800B88DC(Mover *mover, u16 index, f32 *height);
u16 func_800B3748(u16 id, s32 mask, Contact *out, u16 grow, u16 single);
void func_8009060C(Tank *tank, Event *event);

s32 func_800909CC(Tank *tank, f32 speed, u16 angle, u16 angle2) {
    Contact contacts[32];
    Vec2 old;
    u16 hit;
    u16 result;
    u16 count;
    u16 i;
    Contact *contact;
    Contact *list;

    result = 0;
    old.x = tank->pos.x;
    old.y = tank->pos.y;
    hit = 0;
    if (func_800B74B4(&tank->mover, speed, angle, &tank->pos,
                      &tank->previous, tank->index, tank->entity, &hit)) {
        if (tank->mode == 0) {
            tank->angle = angle2;
        }
        result = 3;
    } else if (tank->mode == 0) {
        tank->angle = angle;
    }
    result |= hit;
    if (tank->pos.x == old.x && tank->pos.y == old.y) {
        result |= 5;
    }
    if (tank->entity->state == 0) {
        return 9;
    }
    func_800B69F4(&tank->mover, speed, angle2, tank->angle);
    func_800B88DC(&tank->mover, tank->index, &tank->height);
    D_80397650 = 1;
    tank->scale = 1.0f;
    list = contacts;
    count = func_800B3748(tank->index, 0x0203AB10, list, 0, 0);
    for (i = 0; i < count; i++) {
        contact = &contacts[i];
        if (contact->entity != 0) {
            HitInfo info;
            Event event;

            event.type = 2;
            info.angle = tank->angle;
            info.pos = contact->pos;
            info.flags = tank->flags & 2;
            info.owner = tank->owner;
            info.value10 = 0;
            info.value20 = 0;
            if (D_80224B5C[contact->entity->type].callback != 0) {
                D_80224B5C[contact->entity->type].callback(
                    contact->entity, tank->entity, 0, &info, &event);
            }
            func_8009060C(tank, &event);
            if (!(tank->flags & 1)) {
                return 9;
            }
        }
    }
    return result & 0xFFFF;
}
