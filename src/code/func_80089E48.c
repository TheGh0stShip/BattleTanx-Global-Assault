#include "types.h"

typedef struct PointerList {
    u32 count;
    void *items[1];
} PointerList;

void *func_80089E48(PointerList *list, u16 index) {
    void *result = list->items[index];

    list->count--;
    if (index < list->count) {
        list->items[index] = list->items[list->count];
    }
    return result;
}
