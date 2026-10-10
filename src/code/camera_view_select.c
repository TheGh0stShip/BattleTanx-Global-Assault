typedef unsigned char u8;

typedef struct {
    char pad[0x18];
} CamView;

typedef struct {
    char pad0[0x78];
    char camera[0x9C];
    int view;
} Unit;

typedef struct {
    char pad[0x98];
    int row;
} ViewSelection;

extern u8 D_802194A5;
extern CamView D_80121D90[2][15][2];

void func_800A6ADC(void *camera, CamView *view);
void func_800A6B5C(void *camera, int mode);

void func_800A8A78(Unit *unit, ViewSelection *selection) {
    int row = selection->row;

    func_800A6ADC(unit->camera,
                  &D_80121D90[D_802194A5 >= 2][row][unit->view]);
    func_800A6B5C(unit->camera, 2);
}
