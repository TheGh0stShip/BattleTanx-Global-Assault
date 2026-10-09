#include "types.h"

typedef struct {
    u32 type;
    u32 flags;
    void *ucode_boot;
    u32 ucode_boot_size;
    void *ucode;
    u32 ucode_size;
    void *ucode_data;
    u32 ucode_data_size;
    void *dram_stack;
    u32 dram_stack_size;
    void *output_buffer;
    void *output_buffer_size;
    void *data;
    u32 data_size;
    void *yield_data;
    u32 yield_data_size;
} Task97844;

typedef struct {
    void *data;
    u32 size;
    void *ucode;
    void *ucode_data;
} TaskInput97844;

extern void *D_80224B40;
extern Task97844 *D_801147E8;
extern u8 rspbootTextStart[];
extern u8 D_800F8E80[];
extern u8 D_801B4590[];
extern s32 osRecvMesg(void *queue, void *message, s32 flags);

void func_80097844(TaskInput97844 *input) {
    Task97844 task;
    Task97844 *pending;
    Task97844 *current;

    task.data = input->data;
    task.data_size = input->size;
    task.ucode = input->ucode;
    task.ucode_data = input->ucode_data;
    task.type = 2;
    task.ucode_boot = rspbootTextStart;
    task.flags = 0;
    task.ucode_boot_size = D_800F8E80 - rspbootTextStart;
    task.ucode_size = 0x1000;
    task.ucode_data_size = 0x800;
    task.dram_stack = 0;
    task.dram_stack_size = 0;
    task.output_buffer = 0;
    task.output_buffer_size = 0;
    task.yield_data = 0;
    task.yield_data_size = 0;

    if (D_80224B40 == 0) {
        pending = D_801147E8;
        current = &task;
        if (pending != 0) {
            do {
            } while (pending != 0);
        }
        D_801147E8 = current;
    }
    osRecvMesg(D_801B4590, 0, 1);
}
