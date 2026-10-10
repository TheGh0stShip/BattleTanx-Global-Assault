#include "types.h"
#include "m2c_macros.h"
#ifndef NULL
#define NULL ((void *)0)
#endif
u16 Steps_CopyObstacleRef(void *arg0) {
    u8 temp_v1;

    if (arg0 != NULL) {
        temp_v1 = M2C_FIELD(arg0, u8 *, 0);
        if (temp_v1 != 2) {
            if (temp_v1 == 3) {
                return M2C_FIELD(arg0, u16 *, 0x16);
            }
            /* Duplicate return node #6. Try simplifying control flow for better match */
            return 0U;
        }
        return M2C_FIELD(arg0, u16 *, 0xE);
    }
    return 0U;
}
