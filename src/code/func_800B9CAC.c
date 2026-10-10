/* SPAN 0x800B9CFC */
typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;

typedef struct {
    u32 opcode;
    u32 address;
} DisplayCommand;

void func_800B9CAC(DisplayCommand *commands, s32 count, s32 offset) {
    s32 index;

    for (index = 0; index < count; index++) {
        if (commands[index].opcode == 0xDF000000) {
            break;
        }
        if (*(u8 *)&commands[index] == 1) {
            commands[index].address += offset;
        }
    }
}
