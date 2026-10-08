#include "types.h"

typedef struct {
    u8 pad[8];
    u8 *data;
} EventData;

extern u16 D_803A5970;
extern u16 D_8011DC58;
extern u8 D_80119C20[];
s32 func_80097508(u8 *state, u8 value);

s32 func_800C13BC(void *unused, EventData *event) {
    if (D_803A5970 == 0 && D_8011DC58 < 10) {
        func_80097508(D_80119C20, event->data[0]);
        D_8011DC58++;
    }
    return 0;
}
