typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef int s32;
typedef float f32;

typedef struct Vec2 {
    f32 x;
    f32 y;
} Vec2;

typedef struct QueryResult {
    s32 *object;
    u8 pad04[0x1C];
    u16 type;
    u16 pad22;
} QueryResult;

typedef struct RuntimeFields {
    s32 value;
    s32 hit;
    f32 scale;
    s32 timer;
    u8 mode;
    u8 state;
    u8 pad12[2];
    f32 x;
    f32 y;
} RuntimeFields;

typedef struct ClearObject {
    u8 pad000[0xB4];
    void *value;
} ClearObject;

typedef struct Object {
    s32 handle;
    u8 pad04[4];
    Vec2 position;
    u8 pad10[0x84];
    u8 player;
    u8 pad95[0x1F];
    void *value_b4;
    s32 value_b8;
    f32 scale;
    s32 timer;
    u8 mode;
    u8 mode2;
    u8 padC6[0xA6];
    u8 flags;
} Object;

extern s32 D_8021945C;
extern s16 D_80397650;
extern s32 func_800B49E0(Vec2 *, Vec2 *, s32, u8, s32, s32, QueryResult *);

void func_80088B6C(Object *object, Vec2 *position, s32 value, s8 mode) {
    QueryResult result;
    RuntimeFields *fields = (RuntimeFields *)&object->value_b4;
    f32 scale = 64000.0f;
    s32 timer = D_8021945C + 0x12C;
    u8 flags = object->flags;
    u8 player = object->player;
    s32 handle = object->handle;

    ((ClearObject *)object)->value = 0;
    fields->value = value;
    object->value_b8 = 0;
    object->mode = 0;
    object->mode2 = 0;
    object->mode = mode;
    object->timer = timer;
    object->scale = scale;
    object->flags = flags & ~1;
    D_80397650 = 0;
    if ((func_800B49E0(position, &object->position, 0x87007, player,
                       0, handle, &result) & 0xFFFF) &&
        result.type == 7 && result.object != 0) {
        object->value_b8 = *result.object;
    }
    fields->x = position->x;
    fields->state = 2;
    fields->y = position->y;
}
