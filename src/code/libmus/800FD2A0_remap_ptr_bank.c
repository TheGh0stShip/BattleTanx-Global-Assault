/* Source shape adapted from libmus __MusIntRemapPtrBank in drmario64 (MIT).
 * RODATA_VRAM 0x80077698
 * RODATA_TRIM 0x8 8
 */

extern void osWritebackDCacheAll(void);
extern void func_800FD7D0(void *ptrs, void *base, int count);

typedef unsigned char u8;
typedef unsigned int u32;

typedef struct {
    u8 *base;
    int pad04;
    u8 type;
    u8 flags;
    u8 pad0A[2];
    void *loop;
    void *book;
} wave_t;

typedef struct {
    int pad00[4];
    int flags;
    int pad14[3];
    int count;
    u8 *basenote;
    float *detune;
    wave_t **wave_list;
} ptr_bank_t;

#define PTRFLAG_REMAPPED 0x80000000
#define BASEOFFSET 48
#define U8_TO_FLOAT(c) ((c)&128) ? -(256-(c)) : (c)
#define AL_ADPCM_WAVE 0

void func_800FD2A0(char *pptr, char *wptr)
{
    int i;
    ptr_bank_t *ptrfile_addr;
    unsigned char *chardetune, charwork;
    float *floatdetune, floatwork;
    unsigned long base;

    ptrfile_addr = (ptr_bank_t *)pptr;
    if (ptrfile_addr->flags & PTRFLAG_REMAPPED)
        return;
    ptrfile_addr->flags |= PTRFLAG_REMAPPED;

    func_800FD7D0(&ptrfile_addr->basenote, pptr, 3);
    func_800FD7D0(&ptrfile_addr->wave_list[0], pptr, ptrfile_addr->count);

    for (i = 0; i < ptrfile_addr->count; i++) {
        floatdetune = &ptrfile_addr->detune[i];
        chardetune = (unsigned char *)floatdetune;
        charwork = *chardetune;

        floatwork = U8_TO_FLOAT(charwork);
        *floatdetune = floatwork / 100.0;

        charwork = ptrfile_addr->basenote[i] - BASEOFFSET;
        floatwork = U8_TO_FLOAT(charwork);
        *floatdetune += floatwork;

        if (!ptrfile_addr->wave_list[i]->flags) {
            base = (unsigned long)ptrfile_addr->wave_list[i]->base;
            if ((base & 0xff000000) != 0xff000000) {
                base += (unsigned long)wptr;
                ptrfile_addr->wave_list[i]->base = (u8 *)base;
            }
            ptrfile_addr->wave_list[i]->flags = 1;

            if (ptrfile_addr->wave_list[i]->loop)
                ptrfile_addr->wave_list[i]->loop =
                    (void *)((u32)ptrfile_addr->wave_list[i]->loop + (u32)pptr);
            if (ptrfile_addr->wave_list[i]->type == AL_ADPCM_WAVE)
                ptrfile_addr->wave_list[i]->book =
                    (void *)((u32)ptrfile_addr->wave_list[i]->book + (u32)pptr);
        }
    }
    osWritebackDCacheAll();
}
