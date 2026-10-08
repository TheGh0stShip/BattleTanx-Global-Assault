#include "types.h"

extern u8 *D_80219498;

void func_800B05F4(u8 *values) {
    u8 *data = D_80219498;

    data[0x26C] = values[4];
    data[0x26D] = values[5];
    data[0x26E] = values[6];
    data[0x272] = values[10];
    data[0x273] = values[11];
    data[0x274] = values[12];
    data[0x26F] = values[7];
    data[0x270] = values[8];
    data[0x271] = values[9];
    data[0x275] = values[13];
    data[0x276] = values[14];
    data[0x277] = values[15];
    data[0x278] = values[1];
    data[0x279] = values[2];
    data[0x27A] = values[3];
    data[0x27B] = values[19];
    data[0x27C] = values[20];
    data[0x27D] = values[21];
    data[0x27E] = values[16];
    data[0x27F] = values[17];
    data[0x280] = values[18];
}
