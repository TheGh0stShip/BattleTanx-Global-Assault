typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct { f32 x, y, z; } Vec3f;
typedef struct {
    u8 pad0[0xC];
    Vec3f end;      /* 0x0C */
    Vec3f dir;      /* 0x18 */
    f32 dist;       /* 0x24 */
    u8 color;       /* 0x28 */
    u8 owner;       /* 0x29 */
    u8 pad2A[2];
    s32 unk2C;      /* 0x2C */
} Shot;
typedef struct { u8 pad[4]; s32 kind; } HitObj;
typedef struct {
    HitObj *obj;    /* 0x00 */
    s32 unk4;
    Vec3f pos;      /* 0x08 */
    f32 nx, ny;     /* 0x14, 0x18 */
    s32 pad1C[3];
} Hit;
typedef struct { u32 type; u16 ang; u16 pad; s32 rest[5]; } HitResp;
typedef struct {
    Vec3f pos;      /* 0x00 */
    u16 ang;        /* 0x0C */
    s32 arg6;       /* 0x10 */
    void *color;    /* 0x14 */
    s32 arg8;       /* 0x18 */
    s32 unk1C;
    s32 unk20;      /* 0x20 */
} HitInfo;
typedef struct { u8 pad[0x1E4]; u8 r, g, b; u8 pad2[0x250 - 0x1E7]; } ColorEnt;
typedef struct { void (*fn)(HitObj *, Shot *, s32, HitInfo *, HitResp *); s32 pad[2]; } HitHandler;

extern ColorEnt D_80235F00[];
extern HitHandler D_80224B5C[];
extern s16 D_80397650;

extern Shot *func_800A18D0(s32, s32);
extern s32 func_800B49E0(Vec3f *, Vec3f *, s32, s32, s32, s32, Hit *);
extern f32 func_8009D4B0(u16);
extern f32 func_8009D510(u16);
extern void func_800A5BD8(Vec3f *, s32, s32, f32, s32, s32);
extern s32 func_8009D5B4(f32);
extern u32 func_8009D914(void);
extern void func_800DD0D8(Vec3f *, s32, u16, s32, s32, s32, s32, s32, s32, s32);
extern float sqrtf(float);

s32 func_800DC968(Vec3f *pos, u8 owner, Vec3f *vel, u16 ang, u8 color, s32 arg5, s32 arg6,
                  s32 arg7, u8 arg8, s32 depth)
{
    Vec3f end;
    Hit hit;
    Hit *hp;
    HitInfo info;
    HitResp resp;
    Vec3f start2;
    Vec3f vel2;
    Shot *shot;
    f32 ground;
    f32 t;
    s32 done;
    s32 found;
    s32 base;
    u16 a;

    done = 0;
    shot = func_800A18D0(50, 48);
    ground = 0.0f;
    if (shot == 0) {
        return -1;
    }
    if (depth <= 0) {
        return -1;
    }
    if (pos->z <= ground) {
        return 0;
    }
    shot->owner = owner;
    shot->dir.x = -vel->x;
    shot->dir.z = -vel->z;
    shot->dir.y = -vel->y;
    shot->color = color;
    shot->unk2C = 8;
    end.z = pos->z + vel->z * 12000.0f;
    if (end.z < ground) {
        t = pos->z / (pos->z - end.z);
        end.x = pos->x + vel->x * 12000.0f * t;
        end.y = pos->y + vel->y * 12000.0f * t;
        end.z = ground;
    } else {
        end.x = pos->x + vel->x * 12000.0f;
        end.y = pos->y + vel->y * 12000.0f;
    }
    if (pos->z == end.z && 20.0f < pos->z) {
        ground = pos->z;
        pos->z = end.z = 37.0f;
    }
    hp = &hit;
    do {
        found = (u16)func_800B49E0(pos, &end, 0x64940B, shot->owner, (D_80397650 = 1, 0), arg7, hp);
        if (found == 0 || hit.obj == 0) {
            done = 1;
            continue;
        }
        resp = (HitResp){ 2 };
        info.ang = ang;
        info.pos.x = hit.pos.x;
        info.pos.z = hit.pos.z;
        info.pos.y = hit.pos.y;
        if (color != 127) {
            info.color = &D_80235F00[color];
        } else {
            info.color = 0;
        }
        info.arg8 = arg8;
        info.unk20 = 0;
        info.arg6 = arg6;
        if (D_80224B5C[hit.obj->kind].fn != 0) {
            D_80224B5C[hit.obj->kind].fn(hit.obj, shot, 0, &info, &resp);
        }
        switch (resp.type) {
        case 1:
            done = 1;
            break;
        case 2:
            pos->x = hit.pos.x + vel->x;
            pos->z = hit.pos.z + vel->z;
            pos->y = hit.pos.y + vel->y;
            found = 0;
            if ((pos->x - end.x) * (pos->x - end.x) + (pos->y - end.y) * (pos->y - end.y)
                + (pos->z - end.z) * (pos->z - end.z) < 100.0f) {
                done = 1;
            }
            break;
        case 5:
            found = 0;
            vel2.x = func_8009D4B0(resp.ang);
            vel2.z = vel->z;
            vel2.y = func_8009D510(resp.ang);
            start2.x = hit.pos.x + vel2.x;
            start2.z = hit.pos.z + vel2.z;
            start2.y = hit.pos.y + vel2.y;
            func_800DC968(&start2, owner, &vel2, resp.ang, 127, arg5, arg6, (s32)hit.obj, arg8,
                          depth - 1);
            done = 1;
            end.x = hit.pos.x;
            end.z = hit.pos.z;
            end.y = hit.pos.y;
            break;
        }
    } while (!done);

    if (found) {
        if (ground != 0.0f) {
            hit.pos.z = ground;
            pos->z = ground;
        }
        func_800A5BD8(&hit.pos, 0, owner, 1.0f, arg5, 0);
        end.x = hit.pos.x;
        end.z = hit.pos.z;
        end.y = hit.pos.y;
        if (depth >= 17) {
            if (hit.nx < 0.0f) {
                base = -func_8009D5B4(hit.ny);
            } else {
                base = func_8009D5B4(hit.ny);
            }
            a = base + (func_8009D914() & 0x1FFF) - 0x1000;
            info.pos.z = pos->z;
            info.pos.x = hit.pos.x + func_8009D4B0(a);
            info.pos.y = hit.pos.y + func_8009D510(a);
            func_800DD0D8(&info.pos, owner, a, color, arg5, arg6, arg7, arg8, depth - 1,
                          func_8009D914() % 3 + 3);
            a = base + (func_8009D914() & 0x1FFF) - 0x1000;
            info.pos.z = pos->z;
            info.pos.x = hit.pos.x + func_8009D4B0(a);
            info.pos.y = hit.pos.y + func_8009D510(a);
            func_800DD0D8(&info.pos, owner, a, color, arg5, arg6, arg7, arg8, depth - 1,
                          func_8009D914() % 3 + 3);
        }
    } else if (end.z == 0.0f) {
        func_800A5BD8(&end, 0, owner, 1.0f, arg5, 0);
    }
    shot->dist = sqrtf((end.x - pos->x) * (end.x - pos->x) + (end.z - pos->z) * (end.z - pos->z)
                       + (end.y - pos->y) * (end.y - pos->y));
    shot->end.x = end.x;
    shot->end.z = end.z;
    shot->end.y = end.y;
    if (ground != 0.0f) {
        shot->end.z = ground;
    }
    return 0;
}
