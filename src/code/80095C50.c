/* SPAN 0x80095E7C */
/* RODATA_VRAM 0x80072268 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct {
    char p0[0x30];
    float speed;        /* 0x30 */
    float maxSpeed;     /* 0x34 */
    char p38[0x98 - 0x38];
    int zone;           /* 0x98 */
    char p9c[0x1E0 - 0x9C];
    int flags;          /* 0x1E0 */
    char p1e4[0x280 - 0x1E4];
    int sndEngine;      /* 0x280 */
    int sndSkid;        /* 0x284 */
    int sndBoost;       /* 0x288 */
    int boostTime;      /* 0x28C */
} Tank;

extern int D_8021945C;
extern float D_80114698[];

int func_800979F4(int id);
void func_80097BA4(int h, int a);
int func_800981E0(int h);
void func_800FB8E0(int h, s16 v);
void func_800FB9B0(int h, float f);

void func_80095C50(Tank *t) {
    int dt;
    float r;
    int snd;

    if (!(t->flags & 2)) return;
    dt = D_8021945C - t->boostTime;
    r = t->speed / t->maxSpeed;
    if (dt < 8) {
        if (t->sndBoost == 0 || func_800981E0(t->sndBoost == 0)) {
            t->sndBoost = func_800979F4(71);
        }
        if (dt >= 5) {
            func_800FB8E0(t->sndBoost, (dt - 4) * 255 / 4);
        }
    } else if (t->sndBoost != 0) {
        func_80097BA4(t->sndBoost, 0);
        t->sndBoost = 0;
    }
    snd = 36;
    if (t->zone == 4) {
        snd = 49;
    }
    if (t->sndEngine == 0 || !func_800981E0(t->sndEngine)) {
        t->sndEngine = func_800979F4(snd);
    }
    func_800FB9B0(t->sndEngine, (r + 0.5f) / 1.5f * D_80114698[t->zone]);
    func_800FB8E0(t->sndEngine, 127);
    if (t->zone == 4) return;
    if (r > 0.5) {
        if (t->sndSkid == 0 || !func_800981E0(t->sndSkid)) {
            t->sndSkid = func_800979F4(38);
        }
        func_800FB8E0(t->sndSkid, 2.0f * (r - 0.5f) * 255.0f);
        func_800FB9B0(t->sndSkid, D_80114698[t->zone]);
    } else if (t->sndSkid != 0) {
        func_80097BA4(t->sndSkid, 0);
        t->sndSkid = 0;
    }
}
