/* SPAN 0x800A6320 */
/* RODATA_VRAM 0x80072B30 */
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

typedef struct { float x, y, z; } Vec3;
typedef struct { float m[4][4]; } Matrix4f;

typedef struct {
    int active;
    int type;
} Task;

typedef struct {
    void (*update)(Task *t, int *result);
    void (*msg)(Task *t, void *from, int kind, void *data, void *reply);
    void (*draw)(Task *t);
} TaskClass;

typedef struct {
    Matrix4f m;
    u8 grid;
    char pad[7];
} Frame;

typedef struct {
    Task *data;
    u16 id;
    float x, y, z;
    char p14[0x24 - 0x14];
} Hit;

typedef struct {
    Vec3 ofs;
    Vec3 tgt;
} CamView;

typedef struct {
    int active;         /* 0x00 */
    int type;           /* 0x04 */
    int unk8;
    CamView *cur;       /* 0x0C */
    CamView *prev;      /* 0x10 */
    u8 blend;           /* 0x14 */
    Task *target;       /* 0x18 */
    int id;             /* 0x1C */
    float zoom;         /* 0x20 */
    Vec3 at;            /* 0x24 */
    Vec3 eye;           /* 0x30 */
    Vec3 up;            /* 0x3C */
    float mf[4][4];     /* 0x48 */
    int unk88;
    u8 grid;            /* 0x8C */
    float shake;        /* 0x90 */
} Camera;

typedef struct {
    int id;
    int unk4;
} CamQuery;

typedef struct {
    int status;
    int unk[6];
} Reply;

extern TaskClass D_80224B58[];
extern int D_80117EB4;
extern s16 D_80397650;

void func_8009EEE0(Frame *f);
void func_8009F1F4(Frame *f, Vec3 *in, Vec3 *out);
void func_8009F288(Frame *f, Vec3 *in, Vec3 *out);
float func_8009D8A0(float range);
u16 func_800B49E0(Vec3 *a, Vec3 *b, int mask, u8 grid, int c, int d, Hit *out);
void guLookAtF(float mf[4][4], float xEye, float yEye, float zEye, float xAt, float yAt, float zAt,
               float xUp, float yUp, float zUp);

#define FABS(a) ((a) > 0.0f ? (a) : -(a))


typedef struct {
    char p0[0x20];
    int unk20;
} HitQuery;

void func_800A6320(Camera *c, Vec3 *from, Vec3 *out, Vec3 *base, float *t);

void func_800A6588(Camera *c);


void func_800A61A0(Camera *c, CamView *view, Task *target, int id) {
    CamQuery q;
    Frame f;
    Vec3 *eye;
    Vec3 *at;
    float one = 1.0f;

    c->type = 66;
    c->cur = view;
    c->prev = view;
    c->blend = 64;
    c->target = target;
    c->id = id;
    c->zoom = one;
    func_8009EEE0(&f);
    q.id = id;
    eye = &c->eye;
    at = &c->at;
    q.unk4 = 0;
    if (D_80117EB4 == 10) {
        c->grid = 0;
        return;
    }
    if (D_80224B58[target->type].msg != 0) {
        D_80224B58[target->type].msg(target, c, 1, &q, &f);
    }
    c->grid = f.grid;
    func_8009F288(&f, &view->ofs, eye);
    func_8009F288(&f, &view->tgt, at);
    c->up.x = 0.0f;
    c->up.z = one;
    c->up.y = 0.0f;
    c->shake = 0.0f;
    guLookAtF(c->mf, c->eye.x, c->eye.z, c->eye.y, c->at.x, c->at.z, c->at.y, c->up.x, c->up.z, c->up.y);
}
