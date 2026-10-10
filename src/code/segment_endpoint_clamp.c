typedef unsigned short u16;

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

typedef struct Segment {
    float x;
    float y;
    float dx;
    float dy;
    float length;
} Segment;

typedef struct Mover {
    float unk0;
    float unk4;
    float unk8;
    float unkC;
    float unk10;
    float unk14;
    u16 unk18;
    float unk1C;
    short unk20;
    float unk24;
    float unk28;
    float unk2C;
    float unk30;
    int unk34;
    float unk38;
    short unk3C;
    short unk3E;
    char pad40[4];
    int unk44[4];
    int unk54[4];
    short unk64;
    Segment *segment;
} Mover;

u16 func_800B5C48(Mover *mover, Vec2 *point) {
    Vec2 endpoints[2];
    float radius_squared;
    float distance0;
    float distance1;
    float dx;
    float dy;

    endpoints[0].x = mover->segment->x;
    endpoints[0].y = mover->segment->y;
    endpoints[1].x = endpoints[0].x +
                     mover->segment->dx * mover->segment->length;
    endpoints[1].y = endpoints[0].y +
                     mover->segment->dy * mover->segment->length;
    radius_squared = mover->segment->length * mover->segment->length;
    dx = point->x - endpoints[0].x;
    dy = point->y - endpoints[0].y;
    distance0 = dx * dx + dy * dy;
    dx = point->x - endpoints[1].x;
    dy = point->y - endpoints[1].y;
    distance1 = dx * dx + dy * dy;
    if (radius_squared < distance0) {
        point->x = endpoints[1].x;
        point->y = endpoints[1].y;
        return 1;
    }
    if (radius_squared < distance1) {
        point->x = endpoints[0].x;
        point->y = endpoints[0].y;
        return 1;
    }
    return 0;
}
