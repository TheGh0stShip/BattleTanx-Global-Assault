/* SPAN 0x80083A80 */
/* RODATA_VRAM 0x800712BC */
typedef unsigned short u16;
typedef struct { float x, y; } Pt;
typedef struct { unsigned int state; char p4[0x188 - 0x174]; Pt target; float r; } Ai;
typedef struct { char p0[8]; Pt pos; char p10[0x170 - 0x10]; Ai ai; char p194[0x1C4 - 0x194]; float speed; } Tank;
typedef struct { float dist; float speed; u16 heading; char pa[6]; int arrived; } Move;
void func_8009DA44(Pt *, Pt *, Pt *);
u16 func_8009DDF0(Pt *);
void func_8009DAB0(Pt *, float, u16);
void func_8009E044(Pt *, Pt *);
void func_80087EF0(Tank *, Move *);
void func_80087A80(Tank *, Move *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

void func_8008380C(Tank *e, Move *m) {
    Pt d, v, c, w;

    switch (e->ai.state) {
    case 0:
    case 1:
        break;
    case 2:
    case 3:
    case 4:
    {
    u16 h;
    func_8009DA44(&d, &e->ai.target, &e->pos);
    h = func_8009DDF0(&d) + 0x1556;
    func_8009DAB0(&v, e->ai.r, h);
    c.x = e->ai.target.x;
    c.y = e->ai.target.y;
    func_8009E044(&c, &v);
    func_8009DA44(&w, &e->pos, &c);
    m->dist = DIST(e->pos.x - c.x, e->pos.y - c.y);
    m->heading = func_8009DDF0(&w);
    m->speed = e->speed;
    m->arrived = 0;
    func_80087EF0(e, m);
    func_80087A80(e, m);
    }
        break;
    }
}
