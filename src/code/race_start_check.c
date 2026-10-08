extern int func_8009D144(void);
extern signed char D_80117EB0;
extern int D_80117EB4;
extern unsigned int D_80117ED4[];

int func_800C7AA0(unsigned short arg0) {
    unsigned short i;
    unsigned short count;

    if (func_8009D144() != 0) {
        if (D_80117EB0 == 1 && arg0 != 0) {
            arg0++;
        }
        switch (arg0) {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 3: return 4;
        case 4: return 5;
        }
        return 0;
    }
    if (arg0 >= 4) {
        return 0;
    }
    count = 0;
    i = 0;
    while (1) {
        if (D_80117ED4[i] != 0) {
            count++;
        }
        i++;
        if (arg0 < count) break;
    }
    i--;
    if (D_80117EB4 == 6) {
        switch (i) {
        case 0: return 1;
        case 1: return 3;
        case 2: return 4;
        default: return 5;
        }
    }
    switch (D_80117ED4[i]) {
    case 0: break;
    case 2: case 6: return 3;
    case 3: return 1;
    case 4: return 4;
    case 1: case 5: return 5;
    }
    return 0;
}
