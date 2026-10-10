typedef unsigned short u16;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    float x;
    float y;
    float dx;
    float dy;
    float len;
} Seg;

typedef struct {
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
    Seg *seg;
} Mover;

float func_8009E948(float *value);

extern inline void func_800B5D1C(Mover *mover, Vec2 *force) {
    float magnitude = mover->unk8 * mover->unk18;

    mover->unk0 *= magnitude;
    mover->unk4 *= magnitude;
    mover->unk0 += force->x;
    mover->unk4 += force->y;
    mover->unk8 = func_8009E948(&mover->unk0);
    mover->unk8 /= mover->unk18;
}

void func_800B5F70(Mover *mover, Vec2 *from, float scale, Vec2 *velocity,
                    Vec2 *to) {
    Vec2 delta;
    float dot;
    float projection;

    delta.x = from->x - to->x;
    delta.y = from->y - to->y;
    if (delta.x == 0.0f && delta.y == 0.0f) {
        velocity->x *= scale;
        velocity->y *= scale;
        func_800B5D1C(mover, velocity);
        return;
    }

    func_8009E948(&delta.x);
    dot = velocity->x * delta.x + velocity->y * delta.y;
    if (dot > 0.0f) {
        projection = scale * dot;
        delta.x *= projection;
        delta.y *= projection;
        func_800B5D1C(mover, &delta);
    }

    velocity->x *= scale;
    velocity->y *= scale;
    velocity->x -= delta.x;
    velocity->y -= delta.y;

    scale = func_8009E948(&velocity->x) / mover->unk18;
    if (delta.x * velocity->y - delta.y * velocity->x < 0.0f) {
        scale = -scale;
    }
    mover->unk1C += scale;
}
