typedef struct Info { int a; int b; unsigned short c; unsigned char d; } Info;
typedef struct Obj { char pad0[0xB]; unsigned char unkB; char padC[4]; char *unk10; char pad14[0x10C - 0x14]; void *owner; unsigned char unk110; char pad111[0x1E8 - 0x111]; Info *info; } Obj;
typedef struct Spr { char pad[0x40]; unsigned char unk40; } Spr;
typedef struct Mtx44 { float m[16]; int unk40; } Mtx44;
typedef struct Ent { int pad[0x90 / 4]; int unk90; char pad94[0xD0 - 0x94]; } Ent;
typedef struct Sel { int pad0[3]; Obj *obj; int count; int sel; unsigned int state; float t; int pad20[2]; int unk28; } Sel;
typedef struct Vec3 { float x, y, z; } Vec3;

static const Vec3 D_800750B8 = { 0.0f, -3.5e+02f, 0.0f };
extern int D_802195CC;
extern int D_8021945C;
extern Ent D_80122E38[];
extern Ent D_80122EC8[];
extern unsigned char D_80123AA0[], D_80123AA4[], D_80123AA8[];
extern int func_800A8F34(Obj *, int *, unsigned char *);
extern void func_800CAD9C(int, int);
extern int func_8009D144(void);
extern void func_800CAE10(char *, int *, int);
extern float func_8009D4B0(int);
extern float func_8009D510(int);
extern void func_8009EEE0(void *);
extern void func_8009F090(Mtx44 *, int, int, int);
extern void func_8009EF30(Mtx44 *, float *);
extern void func_8009F824(Mtx44 *, float, float, float);
extern unsigned short func_8009D81C(int, int);
extern void func_800947C4(int, Mtx44 *, int, int, int, int);
extern void func_8009EFD4(Spr *, int, int, int, int);

void func_800D7638(Sel *arg0) {
    int nz = 0;
    Vec3 base = D_800750B8;
    float a[3];
    float b[3];
    Mtx44 mtx;
    int ids[16];
    unsigned short ang[16];
    unsigned char flags[16];
    float pos[3];
    unsigned char col[3];
    int n, i, j, k, off;
    float scale, c, f, k0;
    unsigned short v;
    Sel *s;
    unsigned short *pa;

    s = arg0;
    if (D_802195CC == 7) return;
    if (s->obj->owner != s || !s->obj->unk110) return;
    n = func_800A8F34(s->obj, ids, flags);
    for (i = 0; i < n; i++) nz += flags[i] != 0;
    if (n != s->count || (s->state == 0 && !flags[s->sel])) {
        s->count = n;
        s->sel = 0;
        if (nz) {
            s->unk28 = D_8021945C;
            while (!flags[s->sel]) s->sel++;
        }
        s->state = 0;
        s->t = 0;
        func_800CAD9C(*(int *)&D_80122E38[ids[s->sel]], s->obj->unkB);
        if (func_8009D144()) {
            func_800CAE10(s->obj->unk10 + 0xC, (int *)&D_80122EC8[ids[s->sel]], s->obj->unkB);
        }
    }
    off = 0;
    pa = ang;
    switch (s->state) {
    case 0:
        break;
    case 1:
        off = s->t * (65536.0f / s->count);
        break;
    case 2:
        off = -s->t * (65536.0f / s->count);
        break;
    }
    for (k = 0; k < s->count; k++) {
        pa[k] = (0xFFFF / s->count) * (k + s->count - s->sel) + off;
    }
    b[0] = 0;
    f = func_8009D4B0(0x1000);
    c = 1e+02f;
    b[2] = -f * c;
    b[1] = func_8009D510(0x1000) * c;
    a[0] = c;
    a[2] = 0;
    a[1] = 0;
    i = 0;
    if (i < n) {
    k0 = 2.0f;
    for (; i < n; i++) {
        scale = 1.0f;
        switch (s->state) {
        case 0:
            if (i == s->sel) scale = 2.0f;
            break;
        case 1:
            if (i == s->sel) scale = -s->t + k0;
            else if (i == (s->sel + n - 1) % n) scale = s->t + 1.0f;
            break;
        case 2:
            if (i == s->sel) scale = -s->t + k0;
            else if (i == (s->sel + 1) % n) scale = s->t + 1.0f;
            break;
        }
        scale *= 0.5f;
        func_8009EEE0(&mtx);
        mtx.unk40 = 0;
        pos[0] = base.x + func_8009D510(ang[i]) * b[0] + func_8009D4B0(ang[i]) * a[0];
        pos[2] = base.z + func_8009D510(ang[i]) * b[2] + func_8009D4B0(ang[i]) * a[2];
        pos[1] = base.y + func_8009D510(ang[i]) * b[1] + func_8009D4B0(ang[i]) * a[1];
        func_8009F090(&mtx, 0, (D_8021945C * 200) & 0xFFF8, 0);
        func_8009EF30(&mtx, pos);
        func_8009F824(&mtx, scale, scale, scale);
        if (flags[i]) {
            v = func_8009D81C(0, ang[i]);
            if (v < (unsigned short)(0x10000 / n)) {
                for (j = 0; j < 3; j++)
                    col[j] = D_80123AA8[j] + (D_80123AA4[j] - D_80123AA8[j]) * v / (0x10000 / n);
            } else {
                for (j = 0; j < 3; j++) col[j] = D_80123AA4[j];
            }
        } else {
            for (j = 0; j < 3; j++) col[j] = D_80123AA0[j];
        }
        func_800947C4(ids[i], &mtx, col[0], col[1], col[2], s->obj->unkB);
    }
    }
}

