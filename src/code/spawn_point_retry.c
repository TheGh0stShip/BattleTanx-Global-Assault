/* SPAN 0x80090218 */
/* RODATA_VRAM 0x80071DB0 */
typedef struct { float x, y; unsigned short ang; unsigned char team; } Spawn;
typedef struct { char pad[0x24]; unsigned short h24; char p26[2]; } Slot40;
typedef struct {
    char pad[8]; float x; float y; char p10[0x20 - 0x10]; unsigned short ang; char p22[6]; float vel[3]; char p34[0x94 - 0x34];
    unsigned char team; char p95[3]; int kind; char p9C[0x1E0 - 0x9C]; int flags; char p1E4[0x1F4 - 0x1E4]; unsigned short id;
    char p1F6[0x240 - 0x1F6]; int w240; char p244[0x250 - 0x244]; float timer; Spawn *spawns;
} Obj;
extern float D_80219488;
extern Slot40 D_803978E0[];
extern int func_8008F4AC(int, short, short, unsigned short, unsigned char, unsigned char, unsigned char, int);
extern void func_800B1668(unsigned short, short, short, unsigned char);
extern void func_800B9094(float *, unsigned short);

void func_80090040(Obj *o) {
    int id;
    unsigned short ang;
    int i;

    o->timer += o->w240 != 0 ? D_80219488 * 0.2f : D_80219488 * 0.1f;
    if (o->timer >= 1.0) {
        for (i = 0; i < 4; i++) {
            if (func_8008F4AC(o->kind, o->spawns[i].x, o->spawns[i].y, o->spawns[i].ang, o->spawns[i].team, 1, 0, 0)) {
                o->x = o->spawns[i].x;
                o->y = o->spawns[i].y;
                id = o->id;
                o->ang = o->spawns[i].ang;
                o->team = o->spawns[i].team;
                ang = o->spawns[i].ang;
                func_800B1668(id, o->spawns[i].x, o->spawns[i].y, o->spawns[i].team);
                D_803978E0[id].h24 = ang;
                func_800B9094(o->vel, o->id);
                o->flags = (o->flags & ~0x10) | 0x20;
                o->timer = 1.0f;
                break;
            }
        }
    }
}
