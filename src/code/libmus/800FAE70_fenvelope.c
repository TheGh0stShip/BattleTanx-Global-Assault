/* RODATA_VRAM 0x80077600 */
/* func_800FAE70 = libmus Fenvelope (player_commands.c) with Fdefa inlined.
 * Adapted from AngheloAlf/drmario64 lib/libmus/src/player_commands.c @ b5526094 (MIT repo; libmus is
 * Software Creations' N64 SDK library). Channel offsets from BattleTanx's layout (src/code/libmus/mus_channel.h).
 * MODE raw
 * SPAN 0x800FAFBC
 */
typedef struct { unsigned char pad[0x18]; unsigned char *env_table; } song_t;
typedef struct channel_s {
    unsigned char pad00[0x58];
    float env_attack_calc;     /* 0x58 */
    float env_decay_calc;      /* 0x5C */
    float env_release_calc;    /* 0x60 */
    int env_speed_calc;        /* 0x64 */
    unsigned char pad68[0x74 - 0x68];
    song_t *song_addr;         /* 0x74 */
    unsigned char pad78[0xBF - 0x78];
    unsigned char env_speed;          /* 0xBF */
    unsigned char env_init_vol;       /* 0xC0 */
    unsigned char env_max_vol;        /* 0xC1 */
    unsigned char env_sustain_vol;    /* 0xC2 */
    unsigned char padC3[3];
    unsigned char env_attack_speed;   /* 0xC6 */
    unsigned char env_decay_speed;    /* 0xC7 */
    unsigned char env_release_speed;  /* 0xC8 */
} channel_t;

static inline unsigned char *Fdefa(channel_t *cp, unsigned char *ptr)
{
  unsigned char value;

  value = *ptr++;
  if (value==0)
    value=1;
  cp->env_speed = value;
  cp->env_speed_calc = 1024/value;
  cp->env_init_vol = *ptr++;
  value = *ptr++;
  cp->env_attack_speed = value;
  cp->env_max_vol = *ptr++;
  cp->env_attack_calc  = (1.0 / ((float)value)) * ((float)(cp->env_max_vol-cp->env_init_vol));
  value = *ptr++;
  cp->env_decay_speed = value;
  cp->env_sustain_vol = *ptr++;
  cp->env_decay_calc = (1.0 / ((float)value)) * ((float)(cp->env_sustain_vol-cp->env_max_vol));
  value = *ptr++;
  cp->env_release_speed = value;
  cp->env_release_calc = 1.0 / ((float)value);
  return (ptr);
}

#define Fenvelope func_800FAE70
unsigned char *Fenvelope(channel_t *cp, unsigned char *ptr)
{
  int tmp;

  tmp = *ptr++;
  if(tmp&0x80)
  {
    tmp &= 0x7f;
    tmp <<= 8;
    tmp |= *ptr++;
  }
  (void)Fdefa(cp, &cp->song_addr->env_table[tmp*7]);
  return (ptr);
}
