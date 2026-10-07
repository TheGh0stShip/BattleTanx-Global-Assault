#include "types.h"

typedef struct {
    u8 pad_0[8];
    s32 type;
    u8 pad_C[4];
} SelectionEntry;

extern void* D_8011DC50;
extern u16 D_8011DC54;
extern u16 D_8011DC56;
extern u16 D_803A5980;
extern SelectionEntry* D_803A5984;
extern s16 D_803A5972;

extern void* func_800BD880(void* owner);
extern void func_800C2528();

void func_800C27EC(void) {
    void* entry = func_800BD880(D_8011DC50);

    func_800C2528(D_8011DC50, entry);
    D_8011DC56 = 0;
}

s32 func_800C282C(void) {
    if ((D_8011DC56 == 0) && (D_8011DC54 < (D_803A5980 - 1))) {
        D_8011DC54++;
        func_800C2528();
    }
    return 0;
}

s32 func_800C2884(void) {
    if ((D_8011DC56 == 0) && (D_8011DC54 != 0)) {
        D_8011DC54--;
        func_800C2528();
    }
    return 0;
}

s32 func_800C28CC(void) {
    s32 type = D_803A5984[D_8011DC54].type;

    if ((u32)(type - 1) >= 2) {
        D_803A5972 = 3;
    }
    return 0;
}
