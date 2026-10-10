/* RODATA_VRAM 0x80071344 */
typedef unsigned short u16;

typedef struct {
    float x;
    float y;
} Point;

typedef struct Unit {
    char pad0[8];
    float x;
    float y;
    char pad10[0x94];
    int bit;
    char padA8[0x424];
} Unit;

typedef struct {
    int state;
    Point target;
    char padC[0x18];
    float range;
} AiState;

typedef struct {
    char pad0[8];
    float x;
    float y;
    char pad10[0x80];
    Point *leader;
    char pad94[0xDC];
    AiState ai;
} Tank;

extern Unit D_801AD128[];
u16 func_80089D98(int mask);

#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

Unit *func_80084CC8(Tank *tank, int bits) {
    float best_distance = 64000.0f;
    Unit *best = 0;
    int mask = bits;
    AiState *ai = &tank->ai;
    Point *leader = tank->leader;
    Unit *target;
    float distance;

    while (mask != 0) {
        target = &D_801AD128[func_80089D98(mask)];
        mask ^= target->bit;
        distance = DIST(target->x - leader->x, target->y - leader->y) +
                   DIST(target->x - ai->target.x, target->y - ai->target.y);
        if (distance > ai->range) {
            continue;
        }
        distance = DIST(target->x - tank->x, target->y - tank->y);
        if (distance < best_distance) {
            best = target;
            best_distance = distance;
        }
    }
    return best;
}
