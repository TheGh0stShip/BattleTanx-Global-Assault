/* SPAN 0x800B05F4 */
typedef unsigned char u8;

typedef struct {
    char p0[0x1C];
    u8 col[3];          /* 0x1C */
    char p1F;
} Area;

typedef struct {
    Area areas[6];      /* 0x000 */
    char pC0[4];
    u8 nAreas;          /* 0x0C4 */
    char pC5[0x26C - 0xC5];
    u8 env[12];         /* 0x26C */
    u8 fog[3];          /* 0x278 */
    u8 sky[3];          /* 0x27B */
    u8 skyc[3];         /* 0x27E */
} World;

typedef struct {
    World *world;       /* 0x80219498 */
} GameState;

extern GameState D_80219498;

void func_800A9C24(int mode, u8 a, u8 b, u8 c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
void func_80079C00(u8 r, u8 g, u8 b);
void func_800F7150(u8 a, u8 b, u8 c, u8 d, int e, int f);
void func_800A9BF0(u8 i, u8 r, u8 g, u8 b);

void func_800B04E0(void) {
    World *w = D_80219498.world;
    int i;

    func_800A9C24(2, w->env[0], w->env[1], w->env[2], w->env[3], w->env[4], w->env[5], w->env[6], w->env[7],
                  w->env[8], w->env[9], w->env[10], w->env[11]);
    func_80079C00(w->fog[0], w->fog[1], w->fog[2]);
    func_800F7150(D_80219498.world->skyc[0], D_80219498.world->skyc[1], D_80219498.world->skyc[2],
                  D_80219498.world->sky[0], D_80219498.world->sky[1], D_80219498.world->sky[2]);
    for (i = 0; i < D_80219498.world->nAreas; i++) {
        func_800A9BF0(i, D_80219498.world->areas[i].col[0], D_80219498.world->areas[i].col[1],
                      D_80219498.world->areas[i].col[2]);
    }
}
