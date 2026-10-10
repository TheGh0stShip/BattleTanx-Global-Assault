#include "types.h"

u16 func_8009D6F8(int angle, int amount, int direction) {
    u16 result;
    u16 short_angle = angle;
    u16 short_amount = amount;

    if (direction != 1) {
        result = angle + amount;
    } else {
        int temp = short_angle;

        temp += 0x10000;
        temp -= short_amount;
        result = temp;
    }
    return result;
}
