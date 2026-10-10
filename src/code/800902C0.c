/* RODATA_VRAM 0x80071DCC */
/* SPAN 0x800904C8 */
typedef unsigned short u16;
typedef int s32;
typedef float f32;

typedef struct {
    f32 x;
    f32 y;
} Vec2;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

typedef struct {
    char pad0[8];
    f32 x;
    f32 y;
    char pad10[4];
    f32 previousX;
    f32 previousY;
    char pad1C[4];
    u16 angle;
    u16 previousAngle;
    char pad24[0x98 - 0x24];
    s32 kind;
    char pad9C[0x258 - 0x9C];
    f32 leftTreadScroll;
    f32 rightTreadScroll;
    char pad260[0x272 - 0x260];
    u16 tiltAngle;
} TreadState;

typedef struct {
    f32 scrollScale;
    f32 halfWidth;
    char pad8[0xD0 - 8];
} TreadDefinition;

extern TreadDefinition D_80122ED8[];

f32 func_8009D4B0(u16 angle);
s32 func_8009D6DC(u16 first, u16 second);
void func_8009FF1C(Vec2 *input, Vec3 *output, u16 angle);

void func_800902C0(TreadState *tread) {
    Vec2 displacement;
    Vec3 values[2];
    s32 turn;

    turn = tread->angle - tread->previousAngle;
    displacement.x = tread->x - tread->previousX;
    displacement.y = tread->y - tread->previousY;
    func_8009FF1C(&displacement, &values[0], ~tread->angle);
    values[1].x = values[0].y - D_80122ED8[tread->kind].halfWidth * func_8009D4B0(turn);
    values[1].z = values[0].y + D_80122ED8[tread->kind].halfWidth * func_8009D4B0(turn);
    if (tread->tiltAngle != 0 && (u16)func_8009D6DC(tread->tiltAngle, 0x8000) < 0x4000) {
        values[1].x = -values[1].x;
        values[1].z = -values[1].z;
    }
    tread->leftTreadScroll -= values[1].x * D_80122ED8[tread->kind].scrollScale;
    if (tread->leftTreadScroll >= 256.0f) {
        tread->leftTreadScroll -= 256.0f;
    } else if (tread->leftTreadScroll < 0.0f) {
        tread->leftTreadScroll += 256.0f;
    }
    tread->rightTreadScroll -= values[1].z * D_80122ED8[tread->kind].scrollScale;
    if (tread->rightTreadScroll >= 256.0f) {
        tread->rightTreadScroll -= 256.0f;
    } else if (tread->rightTreadScroll < 0.0f) {
        tread->rightTreadScroll += 256.0f;
    }
}
