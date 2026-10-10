/* SPAN 0x800A6ABC */
/* RODATA_VRAM 0x80072B5C */
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

void func_800A61A0(Camera *c, CamView *view, Task *target, int id);

typedef struct {
    char p0[0x20];
    int unk20;
} HitQuery;

void func_800A6320(Camera *c, Vec3 *from, Vec3 *out, Vec3 *base, float *t);



void func_800A6588(Camera *c) {
    CamQuery q;
    Frame f;
    Vec3 ofs;
    Vec3 tgt;
    Vec3 a;
    Vec3 pa;
    Vec3 b;
    Vec3 pb;
    Vec3 eye;
    Vec3 base;
    Vec3 up = { 0.0f, 1.0f, 0.0f };
    Vec3 pofs;
    Vec3 ptgt;
    float t;
    Vec3 *peye = &c->eye;
    Vec3 *pat = &c->at;
    Vec3 *pup = &c->up;
    float k;

    if (D_80117EB4 == 10) {
        return;
    }
    c->zoom += 0.05f;
    if (1.0f < c->zoom) {
        c->zoom = 1.0f;
    }
    ofs.x = c->cur->ofs.x * c->zoom;
    k = 50.0f;
    ofs.z = (c->cur->ofs.z - k) * c->zoom + k;
    ofs.y = c->cur->ofs.y * c->zoom;
    tgt = c->cur->tgt;
    func_8009EEE0(&f);
    q.id = c->id;
    q.unk4 = 0;
    if (D_80224B58[c->target->type].msg != 0) {
        D_80224B58[c->target->type].msg(c->target, c, 1, &q, &f);
    }
    c->grid = f.grid;
    if (c->blend == 64) {
        func_8009F288(&f, &ofs, &eye);
        func_8009F288(&f, &tgt, pat);
    } else {
        pofs.x = c->prev->ofs.x * c->zoom;
        pofs.z = (c->prev->ofs.z - k) * c->zoom + k;
        pofs.y = c->prev->ofs.y * c->zoom;
        ptgt = c->prev->tgt;
        func_8009F288(&f, &ofs, &a);
        func_8009F288(&f, &tgt, &b);
        func_8009F288(&f, &pofs, &pa);
        func_8009F288(&f, &ptgt, &pb);
        eye.x = pa.x + (a.x - pa.x) * c->blend / 64.0f;
        eye.z = pa.z + (a.z - pa.z) * c->blend / 64.0f;
        eye.y = pa.y + (a.y - pa.y) * c->blend / 64.0f;
        c->at.x = pb.x + (b.x - pb.x) * c->blend / 64.0f;
        c->at.z = pb.z + (b.z - pb.z) * c->blend / 64.0f;
        c->at.y = pb.y + (b.y - pb.y) * c->blend / 64.0f;
        c->blend += 4;
        if (c->blend > 64) {
            c->blend = 64;
        }
    }
    base.x = f.m.m[3][0];
    base.y = f.m.m[3][2];
    base.z = eye.z;
    if (c->target->type == 11) {
        *peye = eye;
        t = 1.0f;
    } else {
        func_800A6320(c, &eye, peye, &base, &t);
    }
    c->zoom *= t;
    peye->x = base.x + (peye->x - base.x) * 0.8f;
    peye->y = base.y + (peye->y - base.y) * 0.8f;
    func_8009F1F4(&f, &up, pup);
    if (c->shake != 0.0f) {
        peye->x += func_8009D8A0(c->shake) - c->shake / 2.0f;
        peye->z += func_8009D8A0(c->shake) - c->shake / 2.0f;
        peye->y += func_8009D8A0(c->shake) - c->shake / 2.0f;
        c->shake -= 0.5f;
        if (c->shake < 0.0f) {
            c->shake = 0.0f;
        }
    }
    guLookAtF(c->mf, peye->x, peye->z, peye->y, pat->x, pat->z, pat->y, pup->x, pup->z, pup->y);
}
