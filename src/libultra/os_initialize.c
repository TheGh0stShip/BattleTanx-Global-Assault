/* IDOFLAGS: -O1 -mips2
   DATA_VRAM 0x80126E50: osClockRate, osViClock, __osShutdown, __OSGlobalIntMask (0x20 incl. padding).
   LDSYM __osFinalrom=0x803ADDB0 LDSYM osTvType=0x80000300 LDSYM osResetType=0x8000030C
   LDSYM osAppNMIBuffer=0x8000031C LDSYM __osExceptionPreamble=0x80104FA0 LDSYM __osGetSR=0x80105DA0 */
#include "ultra.h"

#define OS_CLOCK_RATE 62500000LL
#define VI_NTSC_CLOCK 48681812
#define VI_PAL_CLOCK 49656530
#define VI_MPAL_CLOCK 48628316
#define OS_TV_PAL 0
#define OS_TV_NTSC 1
#define OS_TV_MPAL 2
#define OS_IM_ALL 0x003FFF01
#define SR_CU1 0x20000000
#define FPCSR_FS 0x01000000
#define FPCSR_EV 0x00000800
#define PIF_RAM_END 0x1FC007FF
#define UT_VEC 0x80000000
#define XUT_VEC 0x80000080
#define ECC_VEC 0x80000100
#define E_VEC 0x80000180
#define OS_APP_NMI_BUFSIZE 64

typedef struct {
    u32 inst1;
    u32 inst2;
    u32 inst3;
    u32 inst4;
} __osExceptionVector;

extern __osExceptionVector __osExceptionPreamble_80104FA0;
extern s32 __osFinalrom;
extern s32 osTvType;
extern s32 osResetType;
extern u8 osAppNMIBuffer[];
extern u32 __osGetSR_80105DA0(void);
extern void __osSetSR(u32 value);
extern void __osSetFpcCsr(u32 value);
extern s32 __osSiRawReadIo(u32 devAddr, u32 *data);
extern s32 __osSiRawWriteIo(u32 devAddr, u32 data);
extern void osWritebackDCache(void *vaddr, s32 nbytes);
extern void osInvalICache(void *vaddr, s32 nbytes);
extern void osMapTLBRdb(void);
extern s32 osPiRawReadIo(u32 devAddr, u32 *data);

OSTime osClockRate = OS_CLOCK_RATE;
s32 osViClock = VI_NTSC_CLOCK;
u32 __osShutdown = 0;
u32 __OSGlobalIntMask = OS_IM_ALL;

void osInitialize(void)
{
    u32 pifdata;
    u32 clock = 0;

    __osFinalrom = 1;
    __osSetSR(__osGetSR_80105DA0() | SR_CU1);
    __osSetFpcCsr(FPCSR_FS | FPCSR_EV);
    while (__osSiRawReadIo(PIF_RAM_END - 3, &pifdata))
        ;
    while (__osSiRawWriteIo(PIF_RAM_END - 3, pifdata | 8))
        ;
    *(__osExceptionVector *)UT_VEC = __osExceptionPreamble_80104FA0;
    *(__osExceptionVector *)XUT_VEC = __osExceptionPreamble_80104FA0;
    *(__osExceptionVector *)ECC_VEC = __osExceptionPreamble_80104FA0;
    *(__osExceptionVector *)E_VEC = __osExceptionPreamble_80104FA0;
    osWritebackDCache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osInvalICache((void *)UT_VEC, E_VEC - UT_VEC + sizeof(__osExceptionVector));
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xF;
    if (clock != 0)
        osClockRate = clock;
    osClockRate = osClockRate * 3 / 4;
    if (osResetType == 0)
        _bzero(osAppNMIBuffer, OS_APP_NMI_BUFSIZE);
    if (osTvType == OS_TV_PAL)
        osViClock = VI_PAL_CLOCK;
    else if (osTvType == OS_TV_MPAL)
        osViClock = VI_MPAL_CLOCK;
    else
        osViClock = VI_NTSC_CLOCK;
}
