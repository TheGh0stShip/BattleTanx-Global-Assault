/* RODATA_VRAM 0x80072B34 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

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
    int active;
    int type;
    int unk08;
    void *current;
    void *previous;
    u8 blend;
    Task *target;
    int id;
    float zoom;
    Vec3 at;
    Vec3 eye;
    Vec3 up;
    float matrix[4][4];
    int unk88;
    u8 grid;
    float shake;
} Camera;

typedef struct {
    int status;
    int unk[6];
} Reply;

typedef struct {
    char pad00[0x20];
    int unk20;
} HitQuery;

extern TaskClass D_80224B58[];
extern short D_80397650;

u16 func_800B49E0(
    Vec3 *start, Vec3 *end, int mask, u8 grid, int c, int d, Hit *hit
);

#define FABS(value) ((value) > 0.0f ? (value) : -(value))

void func_800A6320(
    Camera *camera, Vec3 *from, Vec3 *out, Vec3 *base, float *factor_arg
) {
    float *factor;
    Hit hit;
    HitQuery query;
    float base_z;
    float from_z;
    u16 count;
    int blocked = 0;

    base_z = base->z;
    from_z = from->z;
    base->z = 40.0f;
    from->z = 40.0f;
    factor = factor_arg;
    D_80397650 = 1;
    count = func_800B49E0(
        base, from, 0xA00403, camera->grid, 0, 0, &hit
    );
    base->z = base_z;
    from->z = from_z;
    if (count != 0) {
        if (hit.data != 0) {
            Reply reply = { 2 };

            query.unk20 = 0;
            if (D_80224B58[hit.data->type].message != 0) {
                D_80224B58[hit.data->type].message(
                    hit.data, camera, 0, &query, &reply
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
        out->x = hit.x;
        out->y = hit.y;
        if (FABS(from->x - base->x) > FABS(from->y - base->y)) {
            *factor = (hit.x - base->x) / (from->x - base->x);
        } else {
            *factor = (hit.y - base->y) / (from->y - base->y);
        }
        out->z = (from->z - 50.0f) * *factor + 50.0f;
        return;
    }
    *out = *from;
    *factor = 1.0f;
}
