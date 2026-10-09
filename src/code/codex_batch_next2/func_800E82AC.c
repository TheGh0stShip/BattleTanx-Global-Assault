#include "types.h"

typedef struct MatchRecord800E82AC {
    u8 pad00[0x10];
    void *target;
} MatchRecord800E82AC;

typedef struct MatchOwner800E82AC {
    u8 pad00[0x1D0];
    MatchRecord800E82AC *record;
} MatchOwner800E82AC;

typedef struct MatchMessage800E82AC {
    u8 pad00[0x1D];
    u8 record_index;
} MatchMessage800E82AC;

extern u8 D_80235F00[];
extern void func_800E759C(MatchOwner800E82AC *owner,
                          MatchMessage800E82AC *message);

void func_800E82AC(MatchOwner800E82AC *owner,
                   MatchMessage800E82AC *message) {
    register MatchRecord800E82AC *selected __asm__("$2");
    register MatchRecord800E82AC *record __asm__("$7");
    register u32 index __asm__("$6");
    u8 raw_index;

    raw_index = message->record_index;
    record = owner->record;
    index = raw_index;
    if (raw_index == 0x7F) {
        selected = 0;
    } else {
        selected = (MatchRecord800E82AC *)(D_80235F00 + index * 0x250);
    }

    if (record->target == selected->target) {
        func_800E759C(owner, message);
    }
}
