/* func_800FAFBC = libmus MusInitialize (player_api.c), n_audio variant.
 * Adapted from AngheloAlf/drmario64 lib/libmus/src/player_api.c @ b5526094 (MIT repo; libmus is Software
 * Creations' N64 SDK library). Static libmus variables are referenced through their BattleTanx addresses.
 * MODE raw
 * SPAN 0x800FB244
 */
typedef struct { short priority; short fxBus; unsigned char unityPitch; } ALVoiceConfig;
typedef struct { unsigned char pad[0x1C]; } ALVoice;
typedef struct { unsigned char pad[0xC9]; unsigned char playing; unsigned char padCA[0x13C - 0xCA]; } channel_t;
typedef struct ALPlayer_s { struct ALPlayer_s *next; void *clientData; void *handler; } ALPlayer;
typedef struct {
    unsigned long control_flag;   /* 0x00 */
    int channels;                 /* 0x04 */
    void *sched;                  /* 0x08 */
    int thread_priority;          /* 0x0C */
    unsigned char *heap;          /* 0x10 */
    int heap_length;              /* 0x14 */
    unsigned char *ptr;           /* 0x18 */
    unsigned char *wbk;           /* 0x1C */
    void *default_fxbank;         /* 0x20 */
    int fifo_length;              /* 0x24 */
    unsigned char pad28[0x40 - 0x28];
    void *diskrom;                /* 0x40: disk ROM PI handle */
} musConfig;
#define MAX_SONGS 4
#define MusInitialize func_800FAFBC
/* libmus names -> BattleTanx symbols (no new symbols needed) */
#define diskrom_handle D_803ADA50
#define __muscontrol_flag D_803AD9D0
#define max_channels D_803AD974
#define mus_voices D_803AD978
#define mus_channels D_803AD97C
#define mus_channels2 D_803AD980
#define mus_vsyncs_per_second D_803AD984
#define mus_next_frame_time D_803AD988
#define mus_current_handle D_803AD990
#define mus_random_seed D_803AD994
#define mus_init_bank D_803AD998
#define mus_default_bank D_803AD99C
#define mus_last_fxtype D_803AD9A4
#define libmus_fxheader_single D_803AD9AC
#define libmus_fxheader_current D_803AD9A8
#define marker_callback D_803AD9B0
#define plr_player D_803AD960
#define __MusIntMain func_800FC02C
#define __MusIntMemInit func_800FF950
#define __MusIntSchedInit func_800FF3B0
#define __MusIntFifoOpen func_800FBEB0
#define MusPtrBankInitialize func_800FBB34
#define MusFxBankInitialize func_800FBCFC
#define __MusIntAudManInit func_800FF560
#define n_alSynAddPlayer func_80101D90
#define __MusIntInitialiseChannel func_800FD4CC
#define n_alSynAllocVoice func_80101DE0
#define __MusIntMemRemaining func_800FF9CC

extern int osTvType;
extern void *diskrom_handle;               /* D_803ADA50 */
extern unsigned long __muscontrol_flag;    /* D_803AD9D0 */
extern int max_channels;                   /* D_803AD974 */
extern ALVoice *mus_voices;                /* D_803AD978 */
extern channel_t *mus_channels;            /* D_803AD97C */
extern channel_t *mus_channels2;           /* D_803AD980 */
extern int mus_vsyncs_per_second;          /* D_803AD984 */
extern int mus_next_frame_time;            /* D_803AD988 */
extern unsigned long mus_current_handle;   /* D_803AD990 */
extern long mus_random_seed;               /* D_803AD994 */
extern void *mus_init_bank;                /* D_803AD998 */
extern void *mus_default_bank;             /* D_803AD99C */
extern int mus_last_fxtype;                /* D_803AD9A4 */
extern void *libmus_fxheader_single;       /* D_803AD9AC */
extern void *libmus_fxheader_current;      /* D_803AD9A8 */
extern void *marker_callback;              /* D_803AD9B0 */
extern ALPlayer plr_player;                /* D_803AD960 */
extern int __MusIntMain(void *);           /* func_800FC02C */
extern void __MusIntMemInit(void *, int);  /* func_800FF950 */
extern void __MusIntSchedInit(void *);     /* func_800FF3B0 */
extern void *__MusIntMemMalloc(int);
extern void __MusIntFifoOpen(int);         /* func_800FBEB0 */
extern void MusPtrBankInitialize(void *, void *);  /* func_800FBB34 */
extern void MusFxBankInitialize(void *);   /* func_800FBCFC */
extern void __MusIntAudManInit(musConfig *, int, int);  /* func_800FF560 */
extern void MusSetMasterVolume(unsigned long, int);
extern void n_alSynAddPlayer(ALPlayer *);  /* func_80101D90 */
extern void __MusIntInitialiseChannel(channel_t *);    /* func_800FD4CC */
extern void n_alSynAllocVoice(ALVoice *, ALVoiceConfig *);  /* func_80101DE0 */
extern int __MusIntMemRemaining(void);     /* func_800FF9CC */

int MusInitialize(musConfig *config)
{
	ALVoiceConfig vc;
	int i;

	diskrom_handle = config->diskrom;
	__muscontrol_flag = config->control_flag;
	max_channels = config->channels+MAX_SONGS;
	if (osTvType==0)
		mus_vsyncs_per_second = 50;
	else
		mus_vsyncs_per_second = 60;
	mus_next_frame_time = 1000000/mus_vsyncs_per_second;
	__MusIntMemInit(config->heap, config->heap_length);
	__MusIntSchedInit(config->sched);
	mus_voices = __MusIntMemMalloc((max_channels-MAX_SONGS)*sizeof(ALVoice));
	mus_channels = __MusIntMemMalloc(max_channels*sizeof(channel_t));
	mus_channels2 = mus_channels+MAX_SONGS;
	__MusIntFifoOpen(config->fifo_length);
	mus_default_bank = mus_init_bank = 0;
	if (config->ptr && config->wbk)
		MusPtrBankInitialize(config->ptr, config->wbk);
	libmus_fxheader_current = libmus_fxheader_single = 0;
	if (config->default_fxbank)
		MusFxBankInitialize(config->default_fxbank);
	marker_callback=0;
	mus_last_fxtype = 2;
	__MusIntAudManInit(config, mus_vsyncs_per_second, mus_last_fxtype);
	MusSetMasterVolume(3, 0x7fff);
	mus_current_handle = 1;
	mus_random_seed = 0x12345678;
	plr_player.next       = 0;
	plr_player.handler    = __MusIntMain;
	plr_player.clientData = &plr_player;
	n_alSynAddPlayer(&plr_player);
	for(i=0; i<max_channels; i++)
	{
		mus_channels[i].playing = 0;
		__MusIntInitialiseChannel(&mus_channels[i]);
		vc.unityPitch = 0;
		vc.priority = config->thread_priority;
		vc.fxBus = 0;
		if (i>=MAX_SONGS)
			n_alSynAllocVoice(&mus_voices[i-MAX_SONGS], &vc);
	}
	return (__MusIntMemRemaining());
}
