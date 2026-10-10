/* RODATA_VRAM 0x80071598 */
typedef unsigned char u8;
typedef struct { float x, y; } Pt;
typedef struct { char p0[0x18]; unsigned int state; char p1c[0x68 - 0x1C]; char *track; } Owner;
typedef struct {
    char p0[8]; float x; float y; char p10[0x94 - 0x10]; u8 busy; char p95[0x168 - 0x95]; unsigned int mode;
    char p16c[0x1D0 - 0x16C]; Owner *owner;
} Tank;
int func_80095B68(Tank *);
#define ABS(x) ((x) > 0.0f ? (x) : -(x))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define DIST(dx, dy) (MAX(ABS(dx), ABS(dy)) + MIN(ABS(dx), ABS(dy)) * 3.0f / 8.0f)

static inline float closeness(Tank *t, Pt *p) {
    float f = 1.0f;
    if (p != 0) f = (64000.0f - DIST(t->x - p->x, t->y - p->y)) / 64000.0f;
    return f;
}

float func_80086CEC(Tank *arg0, unsigned int arg1) {
    float r = 0.0f;
    Tank *t = arg0;
    unsigned int mode = arg1;
    int ok;
    Pt *p;
    float f;

    ok = 0;
    if (func_80095B68(t) != 0 && t->busy == 0) {
        switch (t->mode) {
        case 6: case 9: case 17: case 18: case 19: case 20: case 21:
            ok = 1;
            break;
        default:
            ok = 0;
            break;
        }
    }
    if (ok) {
        if (t->mode == mode) {
            r = 1.0f;
        } else {
            Owner *o = t->owner;
            p = 0;
            switch (o->state) {
            case 4:
                p = (Pt *)(o->track + 0x10);
                break;
            case 5:
                p = (Pt *)(o->track + 0x24);
                break;
            case 3:
            case 6:
                p = (Pt *)(o->track + 8);
                break;
            }
            {
                Pt *q = p;
                f = 1.0f;
                if (q != 0) f = (64000.0f - DIST(t->x - q->x, t->y - q->y)) / 64000.0f;
            }
            switch (t->mode) {
            case 17: case 18: case 19: case 20:
                r = f * 0.9f;
                break;
            default:
                r = f * 0.1f;
                break;
            }
        }
    }
    return r;
}
