/* RODATA_VRAM 0x800712A0 */
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

typedef struct {
    char pad0[0xC];
    float x;
    float y;
    char pad14[8];
    short health;
} Entity;

typedef struct {
    char pad0[8];
    float x;
    float y;
} Object;

extern Entity *func_800A1A28(Entity *previous, int type);

Entity *func_80083230(Object *object) {
    Entity *best = 0;
    Entity *entity;
    float best_distance = 0.0f;
    float distance;

    for (entity = func_800A1A28(0, 0x1C); entity != 0;
         entity = func_800A1A28(entity, 0x1C)) {
        if (entity->health > 0) {
            distance = DIST(object->x - entity->x, object->y - entity->y);
            if ((best == 0) | (distance < best_distance)) {
                best = entity;
                best_distance = distance;
            }
        }
    }
    return best;
}
