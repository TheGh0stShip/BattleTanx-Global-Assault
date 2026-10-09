/* Source shape adapted from the MIT-licensed Dr. Mario 64 libultra 2.0I. */
#include "ultra.h"

#define OS_TASK_YIELDED 0x1
#define OS_TASK_LOADABLE 0x4
#define OS_YIELD_DATA_SIZE 0xC00
#define SP_IMEM_START 0x04001000
#define SP_CLR_HALT 0x1
#define SP_CLR_BROKE 0x4
#define SP_CLR_SSTEP 0x20
#define SP_SET_INTR_BREAK 0x100
#define SP_CLR_YIELD 0x200
#define SP_CLR_YIELDED 0x800
#define SP_CLR_TASKDONE 0x2000
#define SP_IO_READ(addr) (*(volatile u32 *)PHYS_TO_K1(addr))

typedef struct {
    u32 type;
    u32 flags;
    void *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
} OSTaskData;

typedef union {
    OSTaskData t;
    long long force_structure_alignment;
} OSTask;

extern u32 osVirtualToPhysical(void *);
extern void osWritebackDCache(void *, s32);
extern void __osSpSetStatus(u32);
extern s32 __osSpSetPc(u32);
extern s32 __osSpRawStartDma(s32, u32, void *, u32);
extern s32 __osSpDeviceBusy(void);

#define TO_PHYSICAL(ptr) \
    if (ptr != 0) { \
        ptr = (void *)osVirtualToPhysical(ptr); \
    } \
    (void)0

static OSTask tmp_task;

static OSTask *_VirtualToPhysicalTask(OSTask *input)
{
    OSTask *task;

    task = &tmp_task;
    _bcopy(input, task, sizeof(OSTask));
    TO_PHYSICAL(task->t.ucode);
    TO_PHYSICAL(task->t.ucode_data);
    TO_PHYSICAL(task->t.dram_stack);
    TO_PHYSICAL(task->t.output_buff);
    TO_PHYSICAL(task->t.output_buff_size);
    TO_PHYSICAL(task->t.data_ptr);
    TO_PHYSICAL(task->t.yield_data_ptr);
    return task;
}

void osSpTaskLoad(OSTask *input)
{
    OSTask *task;

    task = _VirtualToPhysicalTask(input);
    if (task->t.flags & OS_TASK_YIELDED) {
        task->t.ucode_data = task->t.yield_data_ptr;
        task->t.ucode_data_size = task->t.yield_data_size;
        input->t.flags &= ~OS_TASK_YIELDED;
        if (task->t.flags & OS_TASK_LOADABLE) {
            task->t.ucode = (u64 *)SP_IO_READ(
                (u32)input->t.yield_data_ptr + OS_YIELD_DATA_SIZE - 4);
        }
    }

    osWritebackDCache(task, sizeof(OSTask));
    __osSpSetStatus(
        SP_CLR_YIELD | SP_CLR_YIELDED | SP_CLR_TASKDONE | SP_SET_INTR_BREAK);
    while (__osSpSetPc(SP_IMEM_START) == -1) {
    }
    while (__osSpRawStartDma(
               1, SP_IMEM_START - sizeof(*task), task, sizeof(OSTask)) == -1) {
    }
    while (__osSpDeviceBusy()) {
    }
    while (__osSpRawStartDma(
               1, SP_IMEM_START, task->t.ucode_boot,
               task->t.ucode_boot_size) == -1) {
    }
}

void osSpTaskStartGo(OSTask *task)
{
    while (__osSpDeviceBusy()) {
    }
    __osSpSetStatus(
        SP_SET_INTR_BREAK | SP_CLR_SSTEP | SP_CLR_BROKE | SP_CLR_HALT);
}
