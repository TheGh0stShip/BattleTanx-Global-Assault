typedef struct { unsigned char type; char pad[7]; void *ptr; int pad2; } Entry;
typedef struct { int a; Entry *entries; char pad[0x1C0 - 8]; } Slot;
extern Slot D_8011AB78[];
extern unsigned char D_803A61B4[];
extern int D_80117F24[];
extern char D_80116E9C[], D_80116EB8[], D_80116E80[], D_80116EF0[], D_80116ED4[];
extern char D_80116F0C[], D_80116F28[], D_80116F60[], D_80116F44[];
extern unsigned int func_800C17C8(void *);

#define BLOCK(k) { unsigned char *q; unsigned char i; \
    e = D_8011AB78[k].entries; \
    while (e->type != 4) e++; \
    q = p; \
    for (i = 0; i < 9; i++, e++) { \
        e++; \
        switch (func_800C17C8(e->ptr)) { \
        case 0x8000: v = 1; break; \
        case 0x4000: v = 2; break; \
        case 0x2000: v = 3; break; \
        case 0x20: v = 4; break; \
        case 0x10: v = 5; break; \
        case 0x8: v = 6; break; \
        case 0x4: v = 7; break; \
        case 0x2: v = 8; break; \
        case 0x1: v = 9; break; \
        default: v = 0; break; \
        } \
        *q++ = v; \
    } \
    p += 9; \
    *p++ = D_80117F24[k]; }

unsigned char *func_800CB334(void)
{
    unsigned char *p = D_803A61B4;
    Entry *e;
    unsigned char v;

    BLOCK(0)
    BLOCK(1)
    BLOCK(2)
    BLOCK(3)
    return p;
}

void func_800CB824(Slot *s, unsigned char *q)
{
    Entry *e;
    unsigned short i;

    e = s->entries;
    while (e->type != 4) e++;
    for (i = 0; i < 9; i++, e++) {
        e++;
        switch (*q++) {
        case 1: e->ptr = D_80116E9C; break;
        case 2: e->ptr = D_80116EB8; break;
        case 3: e->ptr = D_80116E80; break;
        case 4: e->ptr = D_80116EF0; break;
        case 5: e->ptr = D_80116ED4; break;
        case 6: e->ptr = D_80116F0C; break;
        case 7: e->ptr = D_80116F28; break;
        case 8: e->ptr = D_80116F60; break;
        case 9: e->ptr = D_80116F44; break;
        default: e->ptr = 0; break;
        }
    }
}
