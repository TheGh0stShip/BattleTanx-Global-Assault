#include "types.h"

typedef void (*MusCommand)(void);

typedef struct MusSchedulerCallbacks {
    void (*install)(void);
    void (*waitFrame)(void);
    void (*doTask)(void);
} MusSchedulerCallbacks;

typedef struct NAudioRspData {
    u16 commandHandlers[16];
    u16 vectorConstants[72];
    u16 resampleLut[256];
} NAudioRspData;

typedef struct MusAudioRuntimeData {
    NAudioRspData rsp;
    MusCommand commandTable[45];
    u8 commandPadding[12];
    s32 smallRoomParams[26];
    s32 bigRoomParams[34];
    s32 echoParams[10];
    s32 chorusParams[34];
    s32 flangeParams[10];
    s32 noFxParams[10];
    s32 effectCount;
    s32 *effectList[7];
    MusSchedulerCallbacks defaultScheduler;
    MusSchedulerCallbacks *currentScheduler;
    void *lastAudioBuffer;
    u32 audioThreadState[3];
    s32 frameSizeToggle;
    u32 frameSizeState[3];
} MusAudioRuntimeData;

typedef char NAudioRspData_size_check[sizeof(NAudioRspData) == 0x2B0 ? 1 : -1];
typedef char MusAudioRuntimeData_size_check[sizeof(MusAudioRuntimeData) == 0x5B0 ? 1 : -1];

extern void Fstop(void);
extern void Fwave(void);
extern void Fport(void);
extern void Fportoff(void);
extern void Fdefa(void);
extern void Ftempo(void);
extern void Fcutoff(void);
extern void Fendit(void);
extern void Fvibup(void);
extern void Fvibdown(void);
extern void Fviboff(void);
extern void Flength(void);
extern void Fignore(void);
extern void Ftrans(void);
extern void Fignore_trans(void);
extern void Fdistort(void);
extern void func_800FAE70(void);
extern void Fenvoff(void);
extern void Fenvon(void);
extern void Ftroff(void);
extern void Ftron(void);
extern void Ffor(void);
extern void Fnext(void);
extern void Fwobble(void);
extern void Fwobbleoff(void);
extern void Fvelon(void);
extern void Fveloff(void);
extern void Fvelocity(void);
extern void Fpan(void);
extern void Fstereo(void);
extern void Fdrums(void);
extern void Fdrumsoff(void);
extern void Fprint(void);
extern void Fgoto(void);
extern void Freverb(void);
extern void FrandNote(void);
extern void FrandVolume(void);
extern void FrandPan(void);
extern void Fvolume(void);
extern void Fstartfx(void);
extern void Fbendrange(void);
extern void Fsweep(void);
extern void Fchangefx(void);
extern void Fmarker(void);
extern void Flength0(void);

extern void __OsSchedInstall(void);
extern void func_800FF420(void);
extern void func_800FF480(void);

extern MusAudioRuntimeData n_aspMainDataStart;

#define MS(value) ((value) * 40)

MusAudioRuntimeData n_aspMainDataStart = {
    {
        {
            0x10EC, 0x139C, 0x119C, 0x1A64, 0x11C8, 0x17EC, 0x1208, 0x0000,
            0x0000, 0x127C, 0x1348, 0x1248, 0x1C84, 0x12D4, 0x02B0, 0x1384,
        },
        {
            0xF000, 0x0F00, 0x00F0, 0x000F, 0x0001, 0x0010, 0x0100, 0x1000,
            0x0002, 0x0004, 0x0006, 0x0008, 0x000A, 0x000C, 0x000E, 0x0010,
            0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001,
            0x0000, 0x0020, 0x0002, 0x0800, 0x0008, 0x7FFF, 0x0100, 0x0200,
            0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000,
            0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000,
            0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000,
            0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001,
            0x2000, 0x4000, 0x6000, 0x8000, 0xA000, 0xC000, 0xE000, 0xFFFF,
        },
        {
            0x0C39, 0x66AD, 0x0D46, 0xFFDF, 0x0B39, 0x6696, 0x0E5F, 0xFFD8,
            0x0A44, 0x6669, 0x0F83, 0xFFD0, 0x095A, 0x6626, 0x10B4, 0xFFC8,
            0x087D, 0x65CD, 0x11F0, 0xFFBF, 0x07AB, 0x655E, 0x1338, 0xFFB6,
            0x06E4, 0x64D9, 0x148C, 0xFFAC, 0x0628, 0x643F, 0x15EB, 0xFFA1,
            0x0577, 0x638F, 0x1756, 0xFF96, 0x04D1, 0x62CB, 0x18CB, 0xFF8A,
            0x0435, 0x61F3, 0x1A4C, 0xFF7E, 0x03A4, 0x6106, 0x1BD7, 0xFF71,
            0x031C, 0x6007, 0x1D6C, 0xFF64, 0x029F, 0x5EF5, 0x1F0B, 0xFF56,
            0x022A, 0x5DD0, 0x20B3, 0xFF48, 0x01BE, 0x5C9A, 0x2264, 0xFF3A,
            0x015B, 0x5B53, 0x241E, 0xFF2C, 0x0101, 0x59FC, 0x25E0, 0xFF1E,
            0x00AE, 0x5896, 0x27A9, 0xFF10, 0x0063, 0x5720, 0x297A, 0xFF02,
            0x001F, 0x559D, 0x2B50, 0xFEF4, 0xFFE2, 0x540D, 0x2D2C, 0xFEE8,
            0xFFAC, 0x5270, 0x2F0D, 0xFEDB, 0xFF7C, 0x50C7, 0x30F3, 0xFED0,
            0xFF53, 0x4F14, 0x32DC, 0xFEC6, 0xFF2E, 0x4D57, 0x34C8, 0xFEBD,
            0xFF0F, 0x4B91, 0x36B6, 0xFEB6, 0xFEF5, 0x49C2, 0x38A5, 0xFEB0,
            0xFEDF, 0x47ED, 0x3A95, 0xFEAC, 0xFECE, 0x4611, 0x3C85, 0xFEAB,
            0xFEC0, 0x4430, 0x3E74, 0xFEAC, 0xFEB6, 0x424A, 0x4060, 0xFEAF,
            0xFEAF, 0x4060, 0x424A, 0xFEB6, 0xFEAC, 0x3E74, 0x4430, 0xFEC0,
            0xFEAB, 0x3C85, 0x4611, 0xFECE, 0xFEAC, 0x3A95, 0x47ED, 0xFEDF,
            0xFEB0, 0x38A5, 0x49C2, 0xFEF5, 0xFEB6, 0x36B6, 0x4B91, 0xFF0F,
            0xFEBD, 0x34C8, 0x4D57, 0xFF2E, 0xFEC6, 0x32DC, 0x4F14, 0xFF53,
            0xFED0, 0x30F3, 0x50C7, 0xFF7C, 0xFEDB, 0x2F0D, 0x5270, 0xFFAC,
            0xFEE8, 0x2D2C, 0x540D, 0xFFE2, 0xFEF4, 0x2B50, 0x559D, 0x001F,
            0xFF02, 0x297A, 0x5720, 0x0063, 0xFF10, 0x27A9, 0x5896, 0x00AE,
            0xFF1E, 0x25E0, 0x59FC, 0x0101, 0xFF2C, 0x241E, 0x5B53, 0x015B,
            0xFF3A, 0x2264, 0x5C9A, 0x01BE, 0xFF48, 0x20B3, 0x5DD0, 0x022A,
            0xFF56, 0x1F0B, 0x5EF5, 0x029F, 0xFF64, 0x1D6C, 0x6007, 0x031C,
            0xFF71, 0x1BD7, 0x6106, 0x03A4, 0xFF7E, 0x1A4C, 0x61F3, 0x0435,
            0xFF8A, 0x18CB, 0x62CB, 0x04D1, 0xFF96, 0x1756, 0x638F, 0x0577,
            0xFFA1, 0x15EB, 0x643F, 0x0628, 0xFFAC, 0x148C, 0x64D9, 0x06E4,
            0xFFB6, 0x1338, 0x655E, 0x07AB, 0xFFBF, 0x11F0, 0x65CD, 0x087D,
            0xFFC8, 0x10B4, 0x6626, 0x095A, 0xFFD0, 0x0F83, 0x6669, 0x0A44,
            0xFFD8, 0x0E5F, 0x6696, 0x0B39, 0xFFDF, 0x0D46, 0x66AD, 0x0C39,
        },
    },
    {
        Fstop, Fwave, Fport, Fportoff, Fdefa, Ftempo, Fcutoff, Fendit,
        Fvibup, Fvibdown, Fviboff, Flength, Fignore, Ftrans, Fignore_trans,
        Fdistort, func_800FAE70, Fenvoff, Fenvon, Ftroff, Ftron, Ffor, Fnext,
        Fwobble, Fwobbleoff, Fvelon, Fveloff, Fvelocity, Fpan, Fstereo,
        Fdrums, Fdrumsoff, Fprint, Fgoto, Freverb, FrandNote, FrandVolume,
        FrandPan, Fvolume, Fstartfx, Fbendrange, Fsweep, Fchangefx, Fmarker,
        Flength0,
    },
    { 0 },
    {
        3, MS(100),
        0, MS(54), 9830, -9830, 0, 0, 0, 0,
        MS(19), MS(38), 3276, -3276, 0x3FFF, 0, 0, 0,
        0, MS(60), 5000, 0, 0, 0, 0, 0x5000,
    },
    {
        4, MS(100),
        0, MS(66), 9830, -9830, 0, 0, 0, 0,
        MS(22), MS(54), 3276, -3276, 0x3FFF, 0, 0, 0,
        MS(66), MS(91), 3276, -3276, 0x3FFF, 0, 0, 0,
        0, MS(94), 8000, 0, 0, 0, 0, 0x5000,
    },
    { 1, MS(200), 0, MS(179), 12000, 0, 0x7FFF, 0, 0, 0 },
    { 1, MS(20), 0, MS(5), 0x4000, 0, 0x7FFF, 7600, 700, 0 },
    { 1, MS(20), 0, MS(5), 0, 0x5FFF, 0x7FFF, 380, 500, 0 },
    { 1, MS(20), 0, MS(5), 0, 0, 0, 0, 0, 0 },
    6,
    {
        n_aspMainDataStart.noFxParams,
        n_aspMainDataStart.smallRoomParams,
        n_aspMainDataStart.bigRoomParams,
        n_aspMainDataStart.chorusParams,
        n_aspMainDataStart.flangeParams,
        n_aspMainDataStart.echoParams,
        0,
    },
    { __OsSchedInstall, func_800FF420, func_800FF480 },
    &n_aspMainDataStart.defaultScheduler,
    0,
    { 0 },
    1,
    { 0 },
};
