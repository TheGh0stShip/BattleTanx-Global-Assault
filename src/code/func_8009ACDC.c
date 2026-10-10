#include "types.h"

extern s32 D_80117EE4[];
extern s32 D_80117F04[];

s32 func_8009ACDC(s32 selection) {
    s32 eligible = 0;
    s32 index;

    for (index = 0; index < 4; index++) {
        s32 mode = D_80117F04[index];

        if ((u32)mode < 3) {
            if (mode != 0) {
                if (eligible == selection) {
                    s32 state = D_80117EE4[index];

                    switch ((u32)state) {
                        case 2:
                            return 1;
                        case 1:
                            return 0;
                        case 3:
                            return 2;
                    }
                }
                eligible++;
            }
        }
    }
    return 0;
}
