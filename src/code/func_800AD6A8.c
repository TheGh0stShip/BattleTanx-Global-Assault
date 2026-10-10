typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { float x, y; } Vec2;

typedef struct {
    Vec2 pos;
    char pad08[0x20];
} View;

typedef struct {
    void *a;
    void *b;
    void *c;
} Part;

typedef struct {
    Part *parts;
    u8 count;
} Lod;

typedef struct {
    Lod *lods;
    char pad04[8];
    u8 lodMode;
} Model;

typedef struct {
    void *world;
    int track;
    unsigned int mode;
    u8 nHuman;
    u8 nPlayers;
    u8 nTanks;
    u8 nSlots;
    u8 teams[5];
    char pad13[3];
    int time;
    View views[4];
} GameState;

extern GameState D_80219498;

void *func_8007B0E4(void *commands, int count, void *part);
void func_8007B498(void *a, void *c, void *b, int arg7, Vec2 *position,
                   float x, float y, float z, u16 angle, int view, int arg6,
                   u8 flag);

static inline int select_lod(Vec2 *a, Vec2 *b, int kind) {
    float dx;
    float dy;
    float distance;

    switch (kind) {
    case 2:
        dx = a->x - b->x;
        dy = a->y - b->y;
        if (90000.0f < dx * dx + dy * dy) {
            return 1;
        }
        return 0;
    case 3:
        dx = a->x - b->x;
        dy = a->y - b->y;
        distance = dx * dx + dy * dy;
        if (2890000.0f < distance) {
            return 2;
        }
        if (1000000.0f < distance) {
            return 1;
        }
        return 0;
    }
    return 0;
}

static inline void draw_model_for_view(Model *model, Vec2 *position, float x,
                                       float y, float z, u16 angle, int arg6,
                                       int arg7, int view, u8 flag) {
    Lod *lod;
    int i;

    if (model != 0) {
        lod = &model->lods[select_lod(&D_80219498.views[view].pos, position,
                                     model->lodMode)];
        for (i = 0; i < lod->count; i++) {
            Part *part = &lod->parts[i];

            func_8007B498(part->a, part->c, part->b, arg7, position, x, y, z,
                          angle, view, arg6, flag);
        }
    }
}

void func_800AD6A8(Model *model, Vec2 *position, float x, float y, float z,
                   u16 angle, int arg6, int arg7, u8 mask, void *commands,
                   int commandCount, u8 flag) {
    int playerCount = D_80219498.nPlayers;

    if (model != 0) {
        if (mask == 0) {
            return;
        }
        if (commands == 0) {
            int view;

            if ((int)commands < playerCount) {
                view = 0;
                do {
                    if ((mask >> view) & 1) {
                        draw_model_for_view(model, position, x, y, z, angle,
                                            arg6, arg7, view, flag);
                    }
                    view++;
                } while (view < playerCount);
            }
        } else {
            Lod *lod;
            Part *part;
            void *relocatedPart;
            int partIndex;
            int view;

            lod = model->lods;
            for (partIndex = 0; partIndex < lod->count; partIndex++) {
                part = &lod->parts[partIndex];
                relocatedPart = func_8007B0E4(commands, commandCount, part->c);
                for (view = 0; view < playerCount; view++) {
                    if ((mask >> view) & 1) {
                        func_8007B498(part->a, relocatedPart, part->b, arg7,
                                      position, x, y, z, angle, view, arg6,
                                      flag);
                    }
                }
            }
        }
    }
}
