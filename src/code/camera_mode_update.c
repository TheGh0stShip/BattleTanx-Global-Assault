/* RODATA_VRAM 0x80072D28 */
#include "types.h"
#include "m2c_macros.h"

#ifndef NULL
#define NULL 0
#endif

M2C_UNK func_800A6ADC(void *, M2C_UNK *, void *);
M2C_UNK func_800A6B38(void *, M2C_UNK *);
M2C_UNK func_800A6B5C(void *, M2C_UNK);
M2C_UNK func_800C8F00(void *, u8, s32);
M2C_UNK func_800C924C(void *, u8, s32);
M2C_UNK func_800C95A0(void *, u8);
M2C_UNK func_800C962C(void *, void *, u8);
M2C_UNK func_800C975C(void *, u8);

typedef struct {
    u8 bytes[24];
} CamView;

extern CamView D_80121D90[2][15][2];
extern M2C_UNK D_80122330;
extern M2C_UNK D_80122348;
extern s32 D_8021945C;
extern u32 D_802194A0;
extern u8 D_802194A5;

void func_800A8B38(void *arg0) {
    s32 temp_a0;
    s32 temp_a2_2;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x10C);
    if (temp_v1 != NULL) {
        temp_a0 = M2C_FIELD(temp_v1, s32 *, 4);
        switch (temp_a0) {
        case 4: {
            M2C_UNK *var_a1;
            s32 temp_a0_2;
            s32 temp_a1;
            void *temp_a2;

            temp_a2 = arg0 + 0x78;
            temp_a1 = M2C_FIELD(M2C_FIELD(temp_v1, void **, 0xC), s32 *, 0x98);
            temp_a0_2 = M2C_FIELD(arg0, s32 *, 0x114);
            var_a1 = (M2C_UNK *)&D_80121D90[(u8)D_802194A5 >= 2U][temp_a1][temp_a0_2];
            func_800A6ADC(temp_a2, var_a1, temp_a2);
            temp_s0 = arg0 + 0x78;
            func_800A6B5C(temp_s0, 2);
            if (D_8021945C != 0) {
                func_800A6B38(temp_s0, &D_80122348);
            }
            temp_s0_2 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x10C), void **, 0xC);
            if (M2C_FIELD(temp_s0_2, s32 *, 0x1E0) & 2) {
                func_800C8F00(temp_s0_2 + ((M2C_FIELD(temp_s0_2, s32 *, 0x21C) * 2) + 0x1F6),
                              M2C_FIELD(arg0, u8 *, 0xB), M2C_FIELD(temp_s0_2, s32 *, 0x98));
                temp_a2_2 = M2C_FIELD(temp_s0_2, s32 *, 0x220);
                func_800C924C(temp_s0_2 + ((temp_a2_2 * 2) + 0x1F6),
                              M2C_FIELD(arg0, u8 *, 0xB), temp_a2_2);
                func_800C962C(temp_s0_2 + 0x1D4, temp_s0_2 + 0x1D8,
                              M2C_FIELD(arg0, u8 *, 0xB));
                func_800C95A0(arg0 + 0x1B0, M2C_FIELD(arg0, u8 *, 0xB));
            }
            switch (D_802194A0) {
            case 0:
            case 8:
                if (M2C_FIELD(temp_s0_2, s32 *, 0x1E0) & 2) {
                    func_800C975C(M2C_FIELD(arg0, void **, 0x10), M2C_FIELD(arg0, u8 *, 0xB));
                    return;
                }
                break;
            case 2:
            case 7:
            case 13:
                if (M2C_FIELD(temp_s0_2, s32 *, 0x1E0) & 2) {
                    func_800C975C(M2C_FIELD(arg0, void **, 0x10) + 4,
                                  M2C_FIELD(arg0, u8 *, 0xB));
                    return;
                }
                break;
            case 14:
                if (M2C_FIELD(temp_s0_2, s32 *, 0x1E0) & 2) {
                    func_800C975C(arg0 + 0x214, M2C_FIELD(arg0, u8 *, 0xB));
                    return;
                }
                break;
            }
            break;
        }
        case 6:
        case 3:
            ((void (*)())func_800A6ADC)(arg0 + 0x78, &D_80122330);
            break;
        }
    }
}
