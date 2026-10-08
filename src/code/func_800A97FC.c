#include "types.h"

typedef struct {
    u8 pad0[4];
    s32 type;
    s16 next;
    u8 padA[0xD];
    u8 player;
    u8 pad18[0x2C];
} Entry800A97FC;

extern s16 D_80235EF0;
extern Entry800A97FC D_80224EF0[];
extern u8 D_802194A4[];
extern u8 D_80235F00[];

void func_800A97FC(void) {
    s16 index = D_80235EF0;

    while (index != -1) {
        Entry800A97FC *entry = &D_80224EF0[index];
        if (entry->type == 13 && entry->player >= D_802194A4[0]) {
            u8 *player;
            if (entry->player == 0x7F) {
                player = 0;
            } else {
                player = D_80235F00 + entry->player * 592;
            }
            player[0x1FC]++;
        }
        index = entry->next;
    }
}
