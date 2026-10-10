/* SPAN 0x800C12B4 */
/* RODATA_VRAM 0x80073748 */
typedef unsigned char u8;

extern void *func_800BD880(void);
extern void func_800BD8B8(void *current, void *target);

void func_800C123C(void) {
    u8 *record = func_800BD880();
    u8 *selected = 0;

    switch (**(u8 **)(record + 8)) {
    case 'E':
        selected = record - 0x80;
        break;
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
        selected = record - 0xB0;
        break;
    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        selected = record - 0xA0;
        break;
    }
    if (selected != 0) {
        func_800BD8B8(record, selected);
    }
}
