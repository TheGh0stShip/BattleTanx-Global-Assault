/* RODATA_VRAM 0x80072B88 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
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
    float x;
    float y;
    float z;
    char pad14[0x24 - 0x14];
} Hit;

typedef struct {
    float x;
    float y;
    int unk08;
    u16 angle;
    int unk10;
    char pad14[0x20 - 0x14];
    int unk20;
} HitQuery;

typedef struct {
    int status;
    int unk[6];
} Reply;

typedef struct {
    s8 x;
    s8 y;
} Stick;

typedef struct {
    int active;
    int type;
    char pad08[2];
    u8 state;
    char pad0B;
    u16 angle;
    Vec2 position;
    u8 unit;
    u8 grid;
    char pad1A[0x20 - 0x1A];
    int timer;
} Spotter;

typedef struct Unit {
    char data[0x250];
} Unit;

extern TaskClass D_80224B58[];
extern Unit D_80235F00[];
extern s16 D_80397650;
extern float D_80219488;
extern const Reply D_80072B34;

Stick *func_80098250(u8 pad);
float func_8009D4B0(u16 angle);
float func_8009D510(u16 angle);
int func_800D6E40(Unit *unit);
void func_800A8D54(Unit *unit, int value);
int func_800A94FC(Unit *unit);
u16 func_800B49E0(
    Vec2 *start, Vec2 *end, int mask, u8 grid, int value, void *self, Hit *hit
);

static inline Unit *get_unit(int index) {
    if (index == 127) {
        return 0;
    }
    return &D_80235F00[index];
}

void func_800A6C20(Spotter *spotter, int *done) {
    Stick *stick;
    Vec2 target;
    Hit hit;
    HitQuery query;
    Reply reply;
    float speed;
    int blocked;

    switch (spotter->state) {
    case 1:
        if (++spotter->timer >= 46) {
            int value = func_800D6E40(get_unit(spotter->unit));

            func_800A8D54(get_unit(spotter->unit), value);
            *done = 1;
        }
        break;
    case 2:
        if (++spotter->timer >= 46) {
            if (func_800A94FC(get_unit(spotter->unit)) >= 0) {
                *done = 1;
            }
        }
        break;
    case 0:
        blocked = 0;
        stick = func_80098250(spotter->unit);
        spotter->angle -= (unsigned int)(stick->x * 10 * D_80219488);
        speed = stick->y * 0.6f * D_80219488;
        target.x = spotter->position.x + speed * func_8009D4B0(spotter->angle);
        target.y = spotter->position.y + speed * func_8009D510(spotter->angle);
        D_80397650 = 0;
        if (func_800B49E0(
                &spotter->position, &target, 0xA00403, spotter->grid, 0,
                spotter, &hit
            ) != 0) {
            if (hit.data != 0) {
                reply = D_80072B34;
                query.unk08 = 0;
                query.x = hit.x;
                query.y = hit.y;
                query.angle = spotter->angle;
                query.unk10 = 0;
                query.unk20 = 0;
                if (D_80224B58[hit.data->type].message != 0) {
                    D_80224B58[hit.data->type].message(
                        hit.data, spotter, 0, &query, &reply
                    );
                }
                if (reply.status == 1) {
                    blocked = 1;
                }
            } else {
                blocked = 1;
            }
        }
        if (blocked) {
            float distance;

            distance = func_8009D4B0(spotter->angle);
            spotter->position.x =
                (speed > 0.0f ? -distance : distance) + hit.x;
            distance = func_8009D510(spotter->angle);
            spotter->position.y =
                (speed > 0.0f ? -distance : distance) + hit.y;
        } else {
            spotter->position.x = target.x;
            spotter->position.y = target.y;
        }
        break;
    }
}
