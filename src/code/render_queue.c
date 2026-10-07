#include "types.h"

typedef struct RenderEntry {
    s32 field_0;
    s32 sort_key;
    s32 field_8;
    s32 matrix;
    u8 enabled;
    s16 field_12;
    struct RenderEntry* next;
} RenderEntry;

typedef struct RenderNode {
    u32 key;
    RenderEntry* lists[5];
    struct RenderNode* next;
} RenderNode;

typedef struct {
    s32 pad[3];
    u32 limit;
} RenderLimit;

typedef struct {
    s32 pad[2];
    u32 next_matrix;
} RenderAllocator;

typedef struct {
    f32 values[16];
    s32 converted;
} MatrixWithCache;

typedef struct {
    RenderNode nodes[300];
    RenderNode* hash[1][32];
} RenderNodePool;

extern s32 D_80168080;
extern s32 D_80168084;
extern RenderNode* D_801777E0[][32];
extern RenderNodePool D_80175710;
extern RenderEntry D_80168090[];
extern RenderLimit* func_8007AD94(void);
extern RenderAllocator* func_8007ADB0(void);
extern void guMtxF2L(void* source, void* destination);

s32 func_8007B1F0(u32 key, s32 field_0, s32 sort_key, s32 field_8,
                  s32 field_12, MatrixWithCache* matrix, s32 matrix_address,
                  s32 list_index, s32 table_index, u8 enabled) {
    RenderLimit* limits;
    RenderAllocator* allocator;
    RenderNode* node;
    u32 hash;
    RenderEntry* entry;
    RenderEntry** link;

    limits = func_8007AD94();
    allocator = func_8007ADB0();
    if (D_80168080 == 0x8F0) {
        return -1;
    }
    if (matrix != 0) {
        if (matrix->converted == 0) {
            if (allocator->next_matrix >= limits->limit + 0xC000) {
                return -1;
            }
            guMtxF2L(matrix, (void*)allocator->next_matrix);
            matrix->converted = allocator->next_matrix;
            allocator->next_matrix += 0x40;
        }
        matrix_address = matrix->converted;
    }
    hash = ((key >> 2) * 17) & 0x1F;
    for (node = D_801777E0[table_index][hash]; node != 0;
         node = node->next) {
        if (node->key == key) {
            break;
        }
    }
    if (node == 0) {
        if (D_80168084 == 300) {
            return -1;
        }
        node = &D_80175710.nodes[D_80168084++];
        node->next = D_80175710.hash[table_index][hash];
        D_80175710.hash[table_index][hash] = node;
        node->key = key;
        node->lists[0] = 0;
        node->lists[1] = 0;
        node->lists[2] = 0;
        node->lists[3] = 0;
    }
    entry = &D_80168090[D_80168080++];
    if (node->lists[list_index] == 0) {
        entry->next = 0;
        node->lists[list_index] = entry;
    } else {
        link = &node->lists[list_index];
        while (*link != 0 && (*link)->sort_key != sort_key) {
            link = &(*link)->next;
        }
        if (enabled) {
            while (*link != 0 && (*link)->sort_key == sort_key &&
                   !((*link)->enabled & 1)) {
                link = &(*link)->next;
            }
        }
        entry->next = *link;
        *link = entry;
    }
    entry->field_0 = field_0;
    entry->matrix = matrix_address;
    entry->sort_key = sort_key;
    entry->field_12 = field_12;
    entry->field_8 = field_8;
    entry->enabled = enabled != 0;
    return 0;
}

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3f;

typedef struct RenderEntry2 {
    s32 field_0;
    s32 sort_key;
    u8 field_8;
    u8 flags;
    s16 field_A;
    Vec3f vector;
    f32 x;
    f32 y;
    f32 z;
    struct RenderEntry2* next;
} RenderEntry2;

typedef struct RenderNode2 {
    u32 key;
    RenderEntry2* lists[5];
    struct RenderNode2* next;
} RenderNode2;

typedef struct {
    RenderNode2 nodes[32];
    RenderNode2* hash[1][32];
} RenderNodePool2;

extern s32 D_80168088;
extern s32 D_8016808C;
extern RenderNode2* D_8017CE60[][32];
extern RenderNodePool2 D_8017CAE0;
extern RenderEntry2 D_80177AE0[];

s32 func_8007B498(u32 key, s32 field_0, s32 sort_key, u8 field_8,
                  Vec3f* vector, f32 x, f32 y, f32 z, s16 field_A,
                  s32 list_index, s32 table_index, u8 flags) {
    RenderNode2* node;
    u32 hash;
    RenderEntry2* entry;
    RenderEntry2* current;

    if (D_80168088 == 0x200) {
        return -1;
    }
    hash = ((key >> 2) * 17) & 0x1F;
    for (node = D_8017CE60[table_index][hash]; node != 0;
         node = node->next) {
        if (node->key == key) {
            break;
        }
    }
    if (node == 0) {
        if (D_8016808C == 32) {
            return -1;
        }
        node = &D_8017CAE0.nodes[D_8016808C++];
        node->next = D_8017CAE0.hash[table_index][hash];
        D_8017CAE0.hash[table_index][hash] = node;
        node->key = key;
        node->lists[0] = 0;
        node->lists[1] = 0;
        node->lists[2] = 0;
        node->lists[3] = 0;
    }
    entry = &D_80177AE0[D_80168088++];
    if (node->lists[list_index] == 0) {
        entry->next = 0;
        node->lists[list_index] = entry;
    } else {
        current = node->lists[list_index];
        while (current->sort_key != sort_key) {
            if (current->next == 0) {
                break;
            }
            current = current->next;
        }
        entry->next = current->next;
        current->next = entry;
    }
    entry->vector = *vector;
    entry->x = x;
    entry->y = y;
    entry->field_A = field_A;
    entry->field_0 = field_0;
    entry->sort_key = sort_key;
    entry->field_8 = field_8;
    entry->z = z;
    entry->flags = flags;
    return 0;
}
