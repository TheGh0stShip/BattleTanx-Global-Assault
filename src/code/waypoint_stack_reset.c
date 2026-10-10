/* SPAN 0x8008A890 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { void *node; int value; } StackEntry;
typedef struct { u8 active, done; u16 depth; StackEntry entries[5]; } Stack;
typedef struct {
    char pad0[0xC00C]; Stack first, second; int current, previous;
    u8 flag, padC075; u16 count; int last;
} World;

extern void func_8007D998(World *);

static inline void reset(Stack *stack) {
    u16 i;
    stack->active = 1;
    stack->done = 0;
    stack->depth = 0;
    for (i = 0; i < 5; i++) {
        StackEntry *entry = (StackEntry *)((StackEntry *)stack + i) + 0;
        entry = (StackEntry *)((char *)entry + 4);
        entry->node = 0;
        entry->value = 0;
    }
}

void func_8008A764(World *world, int start) {
    func_8007D998(world);
    reset(&world->first);
    reset(&world->second);
    world->current = 0;
    world->previous = 0;
    world->current = start;
    world->flag = 0;
    world->count = 0;
    world->last = 0;
}
