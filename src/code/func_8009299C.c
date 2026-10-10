typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 primFrom[4];
    u8 envFrom[4];
    s32 duration;
    u8 primTo[4];
    u8 envTo[4];
} ColorFade;

typedef struct { u32 w0, w1; } Gfx;

typedef struct {
    char pad00[0x98];
    s32 kind;
    char pad9C[0x194];
    s32 fadeStart[2];
    ColorFade *fade[2];
} Tank;

typedef struct {
    s32 texture[2];
    char pad08[0xC8];
} TankTextureDefinition;

extern TankTextureDefinition D_80122E8C[];
extern s32 D_8021945C;
extern void *D_803A53A0[];

void func_800AD9A8(void *texture, s32 arg1, s32 position, s32 arg3,
                   s32 arg4, s32 alpha, Gfx *commands, s32 commandCount);

static inline void interpolate_color(u8 *out, u8 *from, u8 *to, f32 fraction) {
    s32 i;

    for (i = 0; i < 4; i++) {
        out[i] = from[i] + (f32)(to[i] - from[i]) * fraction;
    }
}

void func_8009299C(Tank *tank, s32 position, u8 alpha) {
    u32 index;

    for (index = 0; index < 2; index++) {
        u8 primitive[4];
        u8 environment[4];
        Gfx commands[2];
        f32 fraction;
        void **texture;
        void **textureTable;

        textureTable = D_803A53A0;
        if (tank->fadeStart[index] < 0) {
            continue;
        }
        fraction = (f32)(D_8021945C - tank->fadeStart[index]) /
                   (f32)tank->fade[index]->duration;
        interpolate_color(environment, tank->fade[index]->envFrom,
                          tank->fade[index]->envTo, fraction);
        interpolate_color(primitive, tank->fade[index]->primFrom,
                          tank->fade[index]->primTo, fraction);
        commands[0].w0 = 0xFA000000;
        commands[1].w0 = 0xFB000000;
        commands[0].w1 = (primitive[0] << 24) | (primitive[1] << 16) |
                         (primitive[2] << 8) | primitive[3];
        commands[1].w1 = (environment[0] << 24) |
                         (environment[1] << 16) |
                         (environment[2] << 8) | environment[3];
        if (index == 0) {
            texture = &textureTable[D_80122E8C[tank->kind].texture[0]];
        } else {
            texture = &textureTable[D_80122E8C[tank->kind].texture[1]];
        }
        func_800AD9A8(*texture, 0, position, 1, 0, alpha, commands, 2);
    }
}
