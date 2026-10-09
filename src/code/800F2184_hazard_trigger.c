/* func_800F2184 (0x800F2184-0x800F2288, 0x104): hazard state trigger (written in this lane).
 * Owns the literals 0.2f/0.9f/12.0f at 0x80077004.
 */
/* SPAN 0x800F2288 */
/* RODATA_VRAM 0x80077004 */
typedef struct { float x, y, z; } Vec3f;
typedef struct {
    char pad[0xC]; Vec3f pos; unsigned char owner; char p19[3]; int kind; int state; float vel; int t28; char p2C[4];
    unsigned short snd;
} Obj;
extern int D_8021945C;
extern unsigned char D_8011551C[];
extern void func_800B22F8(unsigned short);
extern float func_8009D8A0(float);
extern void func_800A5BD8(Vec3f *, int, unsigned char, float, void *, int);
extern void func_80097FB4(int, float, float, float, unsigned char);

void func_800F2184(Obj *o) {
    if (o->state == 1) {
        switch (o->kind) {
        case 0:
            func_800B22F8(o->snd);
            o->snd = 0xFFFF;
            o->state = 2;
            o->t28 = D_8021945C;
            func_800A5BD8(&o->pos, 0, o->owner, 1.0f, D_8011551C, 0);
            break;
        case 1:
            func_800B22F8(o->snd);
            o->snd = 0xFFFF;
            o->state = 3;
            o->vel = (func_8009D8A0(0.2f) + 0.9f) * 12.0f;
            func_80097FB4(0x34, o->pos.x, o->pos.y, 1.0f, o->owner);
            break;
        }
    }
}
