typedef struct Vec2 {
    float x;
    float y;
} Vec2;

typedef struct Node {
    float x;
    float y;
    float direction[2];
    float angle;
} Node;

typedef struct Object {
    char pad000[0x90];
    Node *node;
    char pad094[0x170 - 0x94];
    int state;
    float position[2];
    float target_position[2];
    char pad184[4];
    float speed;
    int timer;
    float angle_half;
    float angle_scaled;
    char pad198[0x1E0 - 0x198];
    int flags;
} Object;

extern int D_8021945C;
extern void func_80086208(Object *, int);
extern void func_8009DB0C(float *, float);
extern void func_8009E044(float *, float *);
extern float func_8009D8A0(float);
extern void func_800848E8(Object *);
extern void func_80088720(Object *);
extern void func_80089008(Object *);

void func_800852F8(Object *object) {
    Vec2 direction;
    Vec2 target_direction;
    Node *node = object->node;
    Node *target;

    object->flags |= 0x100;
    if (node == 0) {
        func_80086208(object, 4);
        return;
    }
    object->state = 1;
    direction.x = node->direction[0];
    direction.y = node->direction[1];
    func_8009DB0C(&direction.x, node->angle);
    object->position[0] = node->x;
    object->position[1] = node->y;
    func_8009E044(object->position, &direction.x);
    target = object->node;
    if (target != 0) {
        target_direction.x = target->direction[0];
        target_direction.y = target->direction[1];
        func_8009DB0C(&target_direction.x, func_8009D8A0(target->angle));
        object->target_position[0] = target->x;
        object->target_position[1] = target->y;
        func_8009E044(object->target_position, &target_direction.x);
    }
    func_800848E8(object);
    object->speed = 0.2f;
    object->timer = D_8021945C + 10;
    object->angle_half = node->angle * 0.5f;
    object->angle_scaled = node->angle * 1.5f;
    func_80088720(object);
    func_80089008(object);
}
