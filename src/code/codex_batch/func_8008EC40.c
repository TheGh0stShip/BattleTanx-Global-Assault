#include "types.h"
#include "m2c_macros.h"

void func_800C924C(void *, u8, s32, void *);

void func_8008EC40(void *arg0, s32 arg1, s32 arg2) {
    s32 offset;
    void *entry;

    offset = arg1 * 2;
    {
        register void *fixed_entry __asm__("$2");

        /* Preserve the original operand order: addu v0, a0, a3. */
        __asm__(".word 0x00871021" : "=r"(fixed_entry) : "r"(arg0), "r"(offset));
        entry = fixed_entry;
    }
    M2C_FIELD(entry, u16 *, 0x1F6) =
        (u16)(M2C_FIELD(entry, u16 *, 0x1F6) + arg2);
    if ((arg1 != 1) && (M2C_FIELD(arg0, s32 *, 0x220) == 0)) {
        M2C_FIELD(arg0, s32 *, 0x220) = arg1;
        if (M2C_FIELD(arg0, s32 *, 0x1E0) & 2) {
            func_800C924C(arg0 + (offset + 0x1F6),
                          M2C_FIELD(M2C_FIELD(arg0, void **, 0x1D0), u8 *, 0xB),
                          arg1, arg0);
        }
    }
}
