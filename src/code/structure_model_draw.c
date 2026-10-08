typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { f32 m[16]; s32 pad; } Mtx44;
typedef struct { u8 pad[0x48]; void *dl; u8 pad4C; u8 flags; } Model;
typedef struct {
    u8 pad0[0xA];
    u8 flags;   /* 0xA */
    u8 unkB;    /* 0xB */
    s32 x;      /* 0xC */
    s32 y;      /* 0x10 */
    s32 z;      /* 0x14 */
    u16 rot;    /* 0x18 */
    u8 idx;     /* 0x1A */
    u8 pad1B;
    void *p1C, *p20, *p24, *p28, *p2C; /* 0x1C-0x2C */
    u8 pad30[8];
    Model *model; /* 0x38 */
    u8 unk3C;   /* 0x3C */
    u8 type;    /* 0x3D */
    u8 pad3E[4];
    s16 team;   /* 0x42 */
} Obj;
typedef struct { u8 pad[0xD]; u8 color; u8 pad2[0x44 - 0xE]; } TeamEnt;
typedef struct { u8 pad[0x1E4]; u8 r, g, b; u8 pad2[0x250 - 0x1E7]; } ColorEnt;

extern u8 D_8021957C[];
extern TeamEnt D_80224F00[];
extern ColorEnt D_80235F00[];
extern void *D_803A56C4[];
extern void *D_803A53A0[];

extern s32 func_800AA058(s32, void *);
extern void func_8009EFD4(Mtx44 *, s32, s32, s32, s32);
extern void func_800ADCA0(void *, s32, Mtx44 *, s32, s32, s32, u32 *, s32, s32);
extern void func_800AE4D0(void *, s32, Mtx44 *, s32, s32, s32, s32);
extern void func_80079968(void *, void *, s32, s32, Mtx44 *, s32, s32, s32, s32, s32);
extern void func_800AD9A8(void *, s32, Mtx44 *, s32, s32, s32, u32 *, s32);

static inline ColorEnt *getColor(s32 i)
{
    if (i == 127) {
        return 0;
    }
    return &D_80235F00[i];
}

void func_800DDF04(Obj *arg0)
{
    Mtx44 mtx = { { 1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f }, 0 };
    u32 dl[6];
    u32 dl2[2];
    u32 dl3[2];
    Obj *obj;
    s32 vis;
    s32 h;
    ColorEnt *c;
    void **pp;

    obj = arg0;
    vis = D_8021957C[obj->idx] & (obj->model->flags >> 4);
    if (vis == 0) {
        return;
    }
    h = func_800AA058(obj->idx, &obj->x);
    func_8009EFD4(&mtx, obj->x, obj->z, obj->y, obj->rot);
    switch (obj->type) {
    case 1:
    case 3:
        dl[0] = 0xFB000000;
        dl[1] = 0xFAFA00FF;
        dl[2] = 0xFC147E04;
        dl[3] = 0x5FFEFDFE;
        dl[4] = 0xE200001C;
        dl[5] = 0xC8112230;
        func_800ADCA0(obj->model->dl, h, &mtx, 0, 0, vis, dl, 3, obj->unkB);
        break;
    case 0:
    case 2:
        func_800AE4D0(obj->model->dl, h, &mtx, 0, 0, 0, vis);
        break;
    case 4:
        func_80079968(obj->p1C, obj->p20, obj->unk3C, h, &mtx, 0, 0, vis, 0, 0);
        break;
    case 6: {

        func_80079968(obj->p24, obj->p28, obj->unk3C, h, &mtx, 0, 0, (u8)vis, 0, 0);
        func_800AE4D0(obj->p2C, h, &mtx, 0, 0, 0, (u8)vis);
        if (obj->team != -1) {
            c = getColor(D_80224F00[obj->team].color);
            dl2[0] = 0xFB000000;
            dl2[1] = (c->r << 24) | (c->g << 16) | (c->b << 8) | 0xFF;
            func_800AD9A8(D_803A56C4[0], h, &mtx, 0, 0, vis, dl2, 1);
        }
        break;
    }
    case 5:
        func_800AE4D0(obj->p20, h, &mtx, 0, 0, 0, vis);
        break;
    case 7:
        if (obj->team != -1) {
            c = getColor(D_80224F00[obj->team].color);
            dl3[0] = 0xFB000000;
            dl3[1] = (c->r << 24) | (c->g << 16) | (c->b << 8) | 0xFF;
            func_800AD9A8(D_803A56C4[0], h, &mtx, 0, 0, vis, dl3, 1);
        }
        break;
    }
    if (obj->flags & 1) {
        void **base = D_803A53A0;
        if (obj->flags & 2) {
            pp = &base[254];
        } else {
            pp = &base[255];
        }
        func_800AE4D0(*pp, 0, &mtx, 0, 1, 0, vis);
    }
}
