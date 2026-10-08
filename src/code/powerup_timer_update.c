typedef struct { char pad[77]; unsigned char b77; } Sub;
typedef struct {
    char pad0[10];
    short h10;
    unsigned short id;
    char pad14[2];
    float y;
    unsigned int state;
    int timer;
    Sub *sub;
    char pad32[12];
    int i44;
    void *p48;
    float f52, f56;
} Obj;
typedef struct { short h; char pad[38]; } Ent;
extern Ent D_803978F4[];
extern int D_8021945C;
extern void func_800E4F00(void *, Obj *);
extern void func_800B129C(int, short, short);
extern unsigned int func_8009D914(void);
extern void func_800B22F8(int);
void func_800F2DB4(Obj *o, int *done) {
    o->sub->b77 |= 0xF0;
    if (o->i44 <= 0) {
        o->state = 4;
        if (o->p48 != 0) {
            func_800E4F00(o->p48, o);
            o->p48 = 0;
        }
    }
    switch (o->state) {
    case 0: {
        int id = o->id; int t;
        o->y -= 2.0f;
        t = o->y;
        { short a = o->f52; short b = o->f56; func_800B129C(id, a, b); }
        D_803978F4[id].h = t;
        if (o->y < (float)(-o->h10 - 50)) {
            o->state = 1;
            o->timer = D_8021945C + func_8009D914() % 60;
        }
        break; }
    case 1:
        if (D_8021945C - o->timer >= 121) o->state = 2;
        break;
    case 2: {
        int id = o->id; int t;
        o->y += 2.0f;
        t = o->y;
        { short a = o->f52; short b = o->f56; func_800B129C(id, a, b); }
        D_803978F4[id].h = t;
        if (0.0f < o->y) {
            o->state = 3;
            o->timer = D_8021945C + func_8009D914() % 60;
        }
        break; }
    case 3:
        if (D_8021945C - o->timer >= 121) o->state = 0;
        break;
    case 4: {
        int id = o->id; int t;
        o->y -= 2.0f;
        t = o->y;
        { short a = o->f52; short b = o->f56; func_800B129C(id, a, b); }
        D_803978F4[id].h = t;
        if (o->y < (float)(-o->h10 - 150)) {
            o->sub->b77 = 0;
            *done = 1;
            func_800B22F8(o->id);
        }
        break; }
    }
}
