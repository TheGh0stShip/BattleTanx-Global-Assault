/* SPAN 0x80083C70 */
/* RODATA_VRAM 0x800712CC */
typedef unsigned char u8; typedef unsigned short u16;
typedef struct { float x, y, z; } Vec;
typedef struct { unsigned int state; int timer; u16 heading; Vec home; Vec target; } Ai;
typedef struct {
    char p0[8]; float x; float y; char p10[0x10]; u16 heading; char p22[0x16C - 0x22]; u8 flags; char p16d[3]; Ai ai;
} Tank;
extern int D_8021945C;
int func_8009E068(float *, Vec *, float);
void func_80088FD4(Tank *);
void func_80089008(Tank *);
u16 func_8009D81C(int, int);

void func_80083A80(Tank *e) {
    Ai *ai = &e->ai;
    float p[2];
    float r;
    u8 f;

    switch (e->ai.state) {
    case 2:
        f = e->flags & 0x10;
        e->flags &= ~0x10;
        if (f) {
            if (300.0f < e->ai.target.z) {
                p[0] = e->x;
                p[1] = e->y;
                if (func_8009E068(p, &e->ai.target, 300.0f) != 0) {
                    e->ai.target.x = p[0];
                    e->ai.target.y = p[1];
                    e->ai.target.z = 300.0f;
                    e->ai.timer = D_8021945C + 90;
                    e->ai.heading = e->heading;
                    e->ai.state = 3;
                } else {
                    e->ai.state = 5;
                    e->ai.timer = D_8021945C + 60;
                }
                func_80088FD4(e);
                break;
            }
        }
        f = e->flags & 0x20;
        e->flags &= ~0x20;
        if (f) {
            ai->state = 5;
            ai->timer = D_8021945C + 60;
            func_80088FD4(e);
        }
        break;
    case 3:
        if (D_8021945C >= e->ai.timer) e->ai.state = 4;
        break;
    case 4:
        if (func_8009D81C(e->heading, e->ai.heading) < 0x2000) {
            e->ai.state = 2;
            e->ai.target.x = e->ai.home.x;
            e->ai.target.y = e->ai.home.y;
            e->ai.target.z = e->ai.home.z;
            func_80089008(e);
        }
        break;
    case 5:
        if (D_8021945C >= e->ai.timer) {
            e->ai.state = 2;
            func_80089008(e);
        }
        break;
    }
}
