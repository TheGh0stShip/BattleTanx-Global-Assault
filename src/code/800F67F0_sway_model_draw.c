/* func_800F67F0 (0x800F67F0-0x800F6C64, 0x474): swaying model render (written in this lane from the ROM
 * listing; owns the rodata vector {0,30,-10} and 512.0f/5.0f at 0x80077280-0x80077294). */
/* SPAN 0x800F6C64 */
/* RODATA_VRAM 0x80077280 */
typedef struct { float x, y, z; } Vec3f;
typedef struct { float m[4][4]; } Mtx;
typedef struct { float m[4][4]; int flag; } Mtx17;
typedef struct {
    char pad[0xC]; Vec3f pos; unsigned short ang; unsigned char owner; char p1B; int model; int t20;
    int p24; int p28; int p2C; int p30; int radius;
} Obj;
extern int D_8021945C;
static const Vec3f D_80077280 = { 0.0f, 30.0f, -10.0f };
extern unsigned char func_800AD14C(float, float, float, unsigned char);
extern int func_800AA058(unsigned char, Vec3f *);
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
extern void func_8009EEE0(Mtx *);
extern void func_8009F8A0(Mtx *, Mtx *, unsigned short);
extern void func_8009FB68(Mtx *, Mtx *, unsigned short);
extern void func_8009FA04(Mtx *, Mtx *, unsigned short);
extern void func_8009F288(Mtx *, Vec3f *, Vec3f *);
extern void func_8009F4B4(Mtx *, Mtx *, Mtx *);
extern void func_8009EF30(Mtx17 *, Vec3f *);
extern void func_800AE4D0(int, int, Mtx17 *, int, int, int, unsigned char);

void func_800F67F0(Obj *o) {
    Mtx m1;
    Mtx m2;
    Mtx m3;
    Mtx m4;
    Vec3f base;
    Vec3f rot;
    Vec3f drift;
    Vec3f wob;
    Vec3f world;
    Vec3f local;
    Mtx17 out;
    unsigned char vis;
    int t;
    int pitch;
    int yaw;

    vis = func_800AD14C(o->pos.x, o->pos.y, (float)o->radius, o->owner);
    if (vis) {
        t = D_8021945C - o->t20;
        base = D_80077280;
        if (t > 60) pitch = 0x1556; else pitch = t * 0x1556 / 60;
        if (t > 90) yaw = 0xAAA; else yaw = t * 0xAAA / 90;
        yaw = (unsigned short)yaw + func_8009D4B0(D_8021945C * 0xFFFF / o->p30) * 512.0f;
        func_8009EEE0(&m3);
        func_8009F8A0(&m3, &m1, yaw);
        func_8009FB68(&m3, &m2, pitch);
        func_8009FA04(&m3, &m4, o->ang);
        func_8009F288(&m2, &base, &rot);
        if (t > 60) {
            drift.x = 0.0f;
            drift.z = -func_8009D4B0(0x1556) * (float)(t - 60) * 5.0f;
            drift.y = func_8009D510(0x1556) * (float)(t - 60) * 5.0f;
        } else {
            drift.x = 0.0f;
            drift.z = 0.0f;
            drift.y = 0.0f;
        }
        wob.x = 2.0f * func_8009D4B0(D_8021945C * 0xFFFF / o->p24);
        wob.z = 2.0f * func_8009D4B0(D_8021945C * 0xFFFF / o->p28);
        wob.y = 2.0f * func_8009D4B0(D_8021945C * 0xFFFF / o->p2C);
        local.x = (base.x - rot.x) + drift.x + wob.x;
        local.z = (base.z - rot.z) + drift.z + wob.z;
        local.y = (base.y - rot.y) + drift.y + wob.y;
        func_8009F288(&m4, &local, &world);
        world.x += o->pos.x;
        world.z += o->pos.z;
        world.y += o->pos.y;
        func_8009F4B4(&m1, &m2, &m3);
        func_8009F4B4(&m3, &m4, (Mtx *)&out);
        func_8009EF30(&out, &world);
        out.flag = 0;
        func_800AE4D0(o->model, func_800AA058(o->owner, &o->pos), &out, 0, 0, 0, vis);
    }
}
