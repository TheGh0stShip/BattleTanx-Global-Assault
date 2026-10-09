#include "types.h"

typedef struct Func80097CC8Entry {
    void *first;
    void *second;
} Func80097CC8Entry;

extern Func80097CC8Entry D_80114710[];
extern void *D_801B4548[];
extern void func_8009ED00(void *first, void *middle, void *second);

void func_80097CC8(s32 entryIndex, s32 middleIndex) {
    func_8009ED00(
        D_80114710[entryIndex].first,
        D_801B4548[middleIndex],
        D_80114710[entryIndex].second);
}
