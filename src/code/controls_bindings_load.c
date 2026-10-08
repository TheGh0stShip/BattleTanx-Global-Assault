typedef struct { unsigned char type; char pad[7]; void *ptr; int pad2; } Entry;
typedef struct { int a; Entry *entries; char pad[0x1C0 - 8]; } Slot;
extern Slot D_8011AB78[];
extern unsigned char D_803A61B4[];
extern int D_80117F24[];
extern char D_80116E9C[], D_80116EB8[], D_80116E80[], D_80116EF0[], D_80116ED4[];
extern char D_80116F0C[], D_80116F28[], D_80116F60[], D_80116F44[];
extern void func_800C1F08(int, int, Slot *);

#define BLOCK(k) \
    func_800C1F08(k, p[9], &D_8011AB78[k]); \
    e = D_8011AB78[k].entries; \
    while (e->type != 4) e++; \
    q = p; \
    for (i = 0; i < 9; i++, e++) { \
        e++; \
        switch (*q++) { \
        case 1: e->ptr = D_80116E9C; break; \
        case 2: e->ptr = D_80116EB8; break; \
        case 3: e->ptr = D_80116E80; break; \
        case 4: e->ptr = D_80116EF0; break; \
        case 5: e->ptr = D_80116ED4; break; \
        case 6: e->ptr = D_80116F0C; break; \
        case 7: e->ptr = D_80116F28; break; \
        case 8: e->ptr = D_80116F60; break; \
        case 9: e->ptr = D_80116F44; break; \
        default: e->ptr = 0; break; \
        } \
    } \
    p += 9; \
    D_80117F24[k] = *p++;

unsigned char *func_800CB934(void)
{
    unsigned char *p = D_803A61B4;
    Entry *e;
    unsigned char *q;
    unsigned short i;

    BLOCK(0)
    BLOCK(1)
    BLOCK(2)
    BLOCK(3)
    return p;
}
