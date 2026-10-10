typedef unsigned char u8;
typedef unsigned int u32;
typedef int s32;

typedef struct SearchObject SearchObject;

struct SearchObject {
    u8 pad000[4];
    SearchObject *next;
    u8 pad008[0x94 - 8];
    u8 owner;
    u8 pad095[0x9C - 0x95];
    u32 list_index;
    u8 pad0A0[0x1E0 - 0xA0];
    s32 flags;
};

typedef struct SearchRoot {
    u8 pad000[0x1B8];
    SearchObject *heads[4];
} SearchRoot;

static inline SearchObject *next_search_object(
    SearchRoot *root, SearchObject *object) {
    SearchObject *result;
    u32 index;

    if (object == 0 || !(object->flags & 1)) {
        result = root->heads[0];
        index = 0;
    } else {
        index = object->list_index;
        result = object->next;
    }
    while (result == 0 && index < 3) {
        index++;
        result = root->heads[index];
    }
    return result;
}

SearchObject *func_800A9928(SearchRoot *root, SearchObject *object,
                            s32 required, s32 excluded,
                            SearchObject *owner_filter) {
    object = next_search_object(root, object);
    if (object != 0) {
search:
        if ((object->flags & required) != required ||
            (object->flags & excluded) ||
            (owner_filter != 0 && object->owner != owner_filter->owner)) {
            object = next_search_object(root, object);
            if (object != 0) {
                goto search;
            }
        }
    }
    return object;
}
