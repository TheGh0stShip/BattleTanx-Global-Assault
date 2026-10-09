#include "types.h"

typedef struct SchedulerRecord {
    s32 sequence;
    s32 kind;
    u16 next;
    u8 pad_A[0x3A];
} SchedulerRecord;

extern s16 D_80224B50;
extern s16 D_80224EE8[];
extern SchedulerRecord D_80224EF0[];
extern u16 D_80235EF0;
extern s32 D_80114CB0;

extern void func_800A1BE0(SchedulerRecord *record);

SchedulerRecord *func_800A18D0(s32 kind) {
    register s32 index __asm__("$4");
    register u32 old_index __asm__("$5");
    register s32 sequence __asm__("$6");
    register u32 next __asm__("$7");

    if (D_80224B50 == -1) {
        if (kind < 8) {
            register s16 *scan __asm__("$3");
            s32 cursor;
            s16 found;
            register s32 sentinel __asm__("$5");

            cursor = 0x40;
            sentinel = -1;
            __asm__ volatile("" : : "r"(sentinel));
            scan = D_80224EE8;
            do {
                found = *scan;
                if (found != sentinel) {
                    func_800A1BE0(&D_80224EF0[found]);
                    break;
                }
                cursor--;
                scan--;
            } while (cursor >= 0x2D);
        } else {
            return 0;
        }
    }

    index = D_80224B50;
    old_index = D_80235EF0;
    sequence = D_80114CB0;
    next = D_80224EF0[index].next;
    D_80235EF0 = index;
    D_80224EF0[index].next = old_index;
    D_80224EF0[index].kind = kind;
    D_80114CB0 = sequence + 1;
    D_80224EF0[index].sequence = sequence;
    D_80224B50 = next;
    return &D_80224EF0[index];
}
