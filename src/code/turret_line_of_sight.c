/* ---- 0x800E0000/h/func_800E2D74.c ---- */
typedef struct { float x, y, z; } Vec3;
extern short D_80397650;
typedef struct { int id; char p4[0x150 - 4]; float x; float y; } Target;
typedef struct { char pad[0x38]; Target *target; } Seeker;
typedef struct { int id; int pad[9]; } Hit;
extern unsigned short func_800B49E0(Vec3 *, Vec3 *, int, unsigned char, int, Seeker *, Hit *);

int func_800E2D74(Seeker *s, Vec3 *pos, unsigned char mask) {
    Hit hit;
    Vec3 v;
    v.x = s->target->x;
    v.y = s->target->y;
    v.z = pos->z;
    D_80397650 = 1;
    if (func_800B49E0(pos, &v, 0x64940B, mask, 0, s, &hit) == 0) {
        return 1;
    }
    return hit.id == s->target->id;
}

