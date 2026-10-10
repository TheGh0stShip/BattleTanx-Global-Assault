/* SPAN 0x800ECEBC */
/* RODATA_VRAM 0x80076720 */
/* CFLAGS -O2 -G0 -mips3 -mgp32 -mfp32 -fno-cse-skip-blocks */
typedef struct { float x, y, z; } Vec3f;
typedef struct { char pad[0xB]; unsigned char id; } Owner;
typedef struct { int floor; char p4[0x1C]; } LanePlayer;
typedef struct { char pad[0x20]; LanePlayer player[1]; } Lane;
typedef struct { char pad[8]; Vec3f pos; Vec3f n; char p20[8]; } Hit;
typedef struct {
    char pad[0xC]; Vec3f pos; Vec3f vel; int kind; int t28; unsigned short ang; unsigned char b2E; char p2F;
    unsigned char owner; char p31[3]; int i34; Owner *src; float f3C; float scale;
} Shell;
extern int D_8021945C;
extern Lane *D_80219498;
extern short D_80397650;
extern void *D_801255A4[];
extern unsigned short D_801255B0[];
extern float D_801255B8[];
extern int D_801255C4[];
extern unsigned char D_80115444[];
extern unsigned char D_80115CD4[];
extern void func_800979F4(int);
extern void func_80097FB4(int, float, float, float, unsigned char);
extern float func_8009D8A0(float);
extern void func_800A2C6C(unsigned char, Vec3f *, short, unsigned char, unsigned char, int);
extern void func_800A5BD8(Vec3f *, int, unsigned char, float, void *, int);
extern void func_800A7090(Vec3f *, unsigned char, float, float, float);
extern unsigned short func_800B49E0(Vec3f *, Vec3f *, int, unsigned char, unsigned short, int, Hit *);
extern int func_800EC1F8(Vec3f *, unsigned char, unsigned char, unsigned char, int, int);
extern void func_800ED010(Vec3f *, unsigned char, unsigned short, float, Owner *);
extern void func_800F1CC8(Vec3f *, unsigned char, unsigned char);

static inline unsigned short collide(Vec3f *from, Vec3f *to, int mask, unsigned char owner, unsigned short r, int ignore, Hit *hit) {
    D_80397650 = 1;
    return func_800B49E0(from, to, mask, owner, r, ignore, hit);
}

void func_800EC8E8(Shell *s, int *done) {
    Vec3f np;
    Hit hit;
    Vec3f r;
    int kind = s->kind;

    if (D_801255C4[kind] < D_8021945C - s->t28) {
        int h;
        *done = 1;
        h = 0;
        if (s->f3C != 0.0f) {
            func_800ED010(&s->pos, s->owner, s->ang, s->f3C, s->src);
            return;
        }
        s->pos.z -= D_801255B8[kind];
        if (D_801255A4[kind] != 0) {
            if (D_801255A4[kind] == D_80115444) {
                func_800979F4(0x15);
                h = func_800EC1F8(&s->pos, s->owner, s->src->id, s->b2E, 2, 0);
            } else {
                func_80097FB4(10, s->pos.x, s->pos.y, 1.0f, s->owner);
                func_800A7090(&s->pos, s->owner, 20.0f, 300.0f, 1000.0f);
            }
            func_800A5BD8(&s->pos, 0, s->owner, s->scale, D_801255A4[kind], h);
        }
        if (kind == 0) {
            func_800A2C6C(s->owner, &s->pos, s->scale * 172.0f, s->b2E, s->src->id, 0xE49D0A);
        }
    } else {
        float floor;
        s->vel.z -= 0.6f;
        np.x = s->pos.x + s->vel.x * s->scale;
        np.z = s->pos.z + s->vel.z * s->scale;
        np.y = s->pos.y + s->vel.y * s->scale;
        floor = s->scale * D_801255B8[s->kind];
        if (np.z < floor) {
            np.z = floor;
            s->vel.z *= -0.4f;
            s->vel.x *= (func_8009D8A0(0.4f) + 0.8f) * 0.5f;
            s->vel.y *= (func_8009D8A0(0.4f) + 0.8f) * 0.5f;
            if (s->vel.z > 1.2f) {
                func_80097FB4(0x16, np.x, np.y, 1.0f, s->owner);
            }
            if (s->vel.z < 0.6f && s->kind != 1) {
                s->t28 = -1000;
            }
        }
        if (s->owner != 0) {
            float top = (float)D_80219498->player[s->owner].floor - D_801255B8[s->kind];
            if (top < np.z) {
                np.z = top - 0.1f;
                if (s->vel.z > 0.0f) {
                    s->vel.z = -s->vel.z;
                }
            }
        }
        if (collide(&s->pos, &np, 0x6494EB, s->owner, D_801255B0[kind], s->i34, &hit)) {
            float d = s->vel.x * hit.n.x + s->vel.z * hit.n.z + s->vel.y * hit.n.y;
            r.x = s->vel.x - 2.0f * hit.n.x * d;
            r.z = s->vel.z - 2.0f * hit.n.z * d;
            r.y = s->vel.y - 2.0f * hit.n.y * d;
            if (r.z * s->vel.z < 0.0f && r.z < 0.6f && s->kind != 1) {
                s->t28 = -1000;
            }
            s->pos.x = hit.pos.x + r.x / 100.0f;
            s->pos.z = hit.pos.z + r.z / 100.0f;
            s->pos.y = hit.pos.y + r.y / 100.0f;
            s->vel.x = r.x * 0.6f;
            s->vel.z = r.z * 0.6f;
            s->vel.y = r.y * 0.6f;
            s->i34 = 0;
        } else {
            s->pos.x = np.x;
            s->pos.z = np.z;
            s->pos.y = np.y;
        }
        if (s->kind == 2) {
            r.x = s->pos.x;
            r.z = s->pos.z - 30.0f;
            r.y = s->pos.y;
            func_800A5BD8(&r, 0, s->owner, 1.0f, D_80115CD4, 0);
            if (s->vel.z < 0.0f) {
                *done = 1;
                func_800F1CC8(&s->pos, s->owner, s->src->id);
            }
        }
        s->ang += 0x200;
    }
}
