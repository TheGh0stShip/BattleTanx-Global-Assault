/* SPAN 0x80097FB4 */
#include "types.h"

typedef struct InfluenceRecord80097EE4 {
    f32 x;
    f32 y;
    u8 pad08[0x1D];
    u8 type;
    u8 pad26[2];
} InfluenceRecord80097EE4;

extern u8 D_802194A5;
extern InfluenceRecord80097EE4 D_802194B4[];
extern f64 D_80072430;
extern f64 D_80072438;
extern f64 D_80072440;

f32 func_80097EE4(f32 x, f32 y, u8 type) {
    register f32 strongest asm("$f4");
    register s32 count asm("$5");
    s32 i;

    count = D_802194A5;
    strongest = 0.0f;
    for (i = 0; i < count; i++) {
        f32 dx;
        f32 dy;
        f32 influence;

        if (D_802194B4[i].type == type) {
            dx = x - D_802194B4[i].x;
            dy = y - D_802194B4[i].y;
            influence = (f32)(D_80072438 /
                (((f64)((dx * dx) + (dy * dy)) * D_80072430) +
                 D_80072438));
            if (strongest < influence) {
                strongest = influence;
            }
        }
    }

    if (D_80072440 < (f64)strongest) {
        return strongest;
    }
    return 0.0f;
}
