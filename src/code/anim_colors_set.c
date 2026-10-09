#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    Gfx prim;
    Gfx env;
    u8 pad10[0x68];
} AnimColorEntry;

extern AnimColorEntry D_80125B28[3];
extern void *D_80125AC0;
extern void func_800F7230(void *manager, void *unused);

void func_800F7150(u8 r, u8 g, u8 b, u8 er, u8 eg, u8 eb) {
    s32 i;
    u32 prim_op;
    u32 prim;
    u32 env;
    Gfx *commands;
    Gfx *prim_cmd;
    Gfx *env_cmd;

    i = 0;
    prim_op = 0xFA000000;
    prim = (r << 24) | ((g & 0xFF) << 16) | ((b & 0xFF) << 8) | 0xFF;
    commands = &D_80125B28[0].prim;
    env_cmd = commands + 1;
    prim_cmd = commands;
    env = (er << 24) | (eg << 16) | (eb << 8);
    do {
        prim_cmd->w0 = prim_op;
        prim_cmd->w1 = prim;
        env_cmd->w0 = 0xFB000000;
        env_cmd->w1 = env;
        env_cmd = (Gfx *)((u8 *)env_cmd + sizeof(AnimColorEntry));
        i++;
        prim_cmd = (Gfx *)((u8 *)prim_cmd + sizeof(AnimColorEntry));
    } while (i < 3);
    if (D_80125AC0 != 0) {
        u32 unused;
        func_800F7230(D_80125AC0, &unused);
    }
}
