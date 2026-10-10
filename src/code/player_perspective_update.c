/* RODATA_VRAM 0x80072CC8 */
typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    char pad0[0xA];
    u8 flags;
    char padB[0x10D];
    void *display_list;
    char pad11C[4];
    u16 perspective_norm;
    char pad122[2];
    float projection[4][4];
    char pad164[0xEC];
} Unit;

typedef struct {
    void *world;
    int track;
    unsigned int mode;
    u8 human_count;
    u8 player_count;
} GameState;

extern GameState D_80219498;
extern Unit D_80235F00[];
extern int D_8023A060;
extern char D_1000150[];
extern char D_1000168[];

void guPerspectiveF(float matrix[4][4], u16 *perspective_norm, float fovy,
                    float aspect, float near, float far, float scale);

static inline Unit *get_unit(int index) {
    if (index == 127) {
        return 0;
    }
    return &D_80235F00[index];
}

void func_800A7664(void) {
    Unit *unit;
    float aspect;
    int index;
    float far;

    for (index = 0; index < D_80219498.player_count; index++) {
        unit = get_unit(index);
        if (unit->display_list == D_1000150 ||
            unit->display_list == D_1000168) {
            aspect = 320.0f / 120.0f;
        } else {
            aspect = 320.0f / 240.0f;
        }
        far = D_8023A060;
        guPerspectiveF(unit->projection, &unit->perspective_norm,
                       (unit->flags & 0x10) ? 30.0f : 37.0f,
                       aspect, 16.0f, far, 1.0f);
    }
}
