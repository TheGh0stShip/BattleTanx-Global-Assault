/* LDSYM D_80121940=0x80121940 */
#include "types.h"

typedef struct {
    u8 pad0[8];
    void* label;
    u8 pad1[0x34];
} ResultsColumn;

extern ResultsColumn D_80121940[];
extern s32 D_803A8318[];
extern u8 D_80121810[];
extern u8 D_80121820[];
extern u8 D_80121830[];
extern u8 D_8012183C[];
extern u8 D_8012184C[];

void func_800CFD18(u16 player) {
    ResultsColumn* column = &D_80121940[1];
    s32 time = D_803A8318[player];

    if (player == 0) column = &D_80121940[0];
    if (time < 70000) column->label = D_80121810;
    else if (time < 90000) column->label = D_80121820;
    else if (time < 120000) column->label = D_80121830;
    else if (time < 150000) column->label = D_8012183C;
    else column->label = D_8012184C;
}
