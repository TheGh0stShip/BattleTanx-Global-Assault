#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} DisplayCommand;

s32 func_800B9C68(DisplayCommand* command) {
    s32 count = 0;

    if (command->w0 != 0xDF000000) {
        do {
            if ((command->w0 >> 24) == 0xFD) {
                count++;
            }
            command++;
        } while (command->w0 != 0xDF000000);
    }
    if (count != 0) {
        return count - 1;
    }
    return 0;
}
