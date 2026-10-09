#include "types.h"

extern void func_80086560(void *object);
extern void func_80088030(void *object);
extern void func_8007E64C(void *object);
extern void func_8008268C(void *object);
extern void func_80082BD4(void *object);
extern void func_80088CA8(void *object);

void func_80087C38(void *owner) {
    void *object = *(void **)((u8 *)owner + 0xC);

    func_80086560(object);
    func_80088030(object);
    func_8007E64C(object);
    func_8008268C(object);
    func_80082BD4(object);
    func_80088CA8(object);
}
