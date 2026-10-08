extern void *D_803AD9AC;
extern void *D_803AD9A8;
extern int D_803AD998;
extern int func_800FD8B8(void *bank, int number, int volume, int pan, int priority);

int func_800FB4F0(int number)
{
    void *bank;
    int handle;

    if (D_803AD9AC != 0) {
        bank = D_803AD9AC;
        D_803AD9AC = 0;
    } else {
        bank = D_803AD9A8;
        if (bank == 0) {
            D_803AD998 = 0;
            return 0;
        }
    }
    if (D_803AD998 == 0)
        D_803AD998 = ((int *)bank)[4];
    handle = func_800FD8B8(bank, number, 0x80, 0x80, -1);
    D_803AD998 = 0;
    return handle;
}
