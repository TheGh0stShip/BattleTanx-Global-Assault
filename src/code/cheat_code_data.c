#include "types.h"

typedef struct CheatCodeData {
    char teddy_breaks[12];
    char eighty_days[8];
    char happy_happy[12];
    char rockets_red_glare[12];
    char war_drb[8];
    char persistence_of_time[12];
    char bronze_sights[12];
    char dream_weaver[8];
    char nuke_n_hacks[12];
    char drink_me[8];
    char shrimp[8];
    char tom_thumb[8];
    char mini_me[8];
    char pygmy[8];
    char unknown_88[4];
    char deceptive_b[8];
    char deceptive_c[8];
    char deceptive_d[8];
    char deceptive_f[8];
    char deceptive_g[8];
    char deceptive_h[8];
    char deceptive_j[8];
    char deceptive_k[8];
    char deceptive_l[8];
    char deceptive_m[8];
    char deceptive_p[8];
    char deceptive_q[8];
    char deceptive_r[8];
    char deceptive_s[8];
    char deceptive_t[8];
    char deceptive_zero[8];
    char unknown_10c[4];
    char unknown_110[4];
    char custom_gang[12];
    char level_select[16];
    char invulnerability[16];
    char all_weapons[12];
    char super_happy_code[20];
    char cinematics_loop[16];
    char cinematic_test[16];
    char no_cinematics[16];
    char secret_level[16];
    char game_password[16];
    char scramble_models[16];
    char what_the[12];
    char cassandra_gang[16];
    char brandon_gang[16];
    char mini_tanks_1[16];
    char mini_tanks_2[16];
    char mini_tanks_3[16];
    char mini_tanks_4[16];
    char mini_tanks_5[16];
    char *codes[33];
} CheatCodeData;

CheatCodeData gCheatCodeData = {
    "TRDDYBRRKS", "80DYS", "HPPYHPPY", "RCKTSRDGLR", "WRDRB",
    "PRSSTNCFTM", "BRNCSSTS", "DRMWVR", "NNKNHCKS", "DR1NKM3",
    "SHR1MP", "T0MTHMB", "M1N1M3", "PYGMY", "?",
    "DCPTCB", "DCPTCC", "DCPTCD", "DCPTCF", "DCPTCG", "DCPTCH",
    "DCPTCJ", "DCPTCK", "DCPTCL", "DCPTCM", "DCPTCP", "DCPTCQ",
    "DCPTCR", "DCPTCS", "DCPTCT", "DCPTC0", "?", "?",
    "CUSTOM GANG", "LEVEL SELECT", "INVULNERABILITY", "ALL WEAPONS",
    "SUPER HAPPY CODE", "CINEMATICS LOOP", "CINEMATIC TEST",
    "NO CINEMATICS", "SECRET LEVEL", "GAME PASSWORD", "SCRAMBLE MODELS",
    "WHAT THE", "CASSANDRA GANG", "BRANDON GANG", "MINI TANKS 1",
    "MINI TANKS 2", "MINI TANKS 3", "MINI TANKS 4", "MINI TANKS 5",
    {
        gCheatCodeData.teddy_breaks,
        gCheatCodeData.eighty_days,
        gCheatCodeData.happy_happy,
        gCheatCodeData.rockets_red_glare,
        gCheatCodeData.unknown_110,
        gCheatCodeData.unknown_88,
        gCheatCodeData.deceptive_b,
        gCheatCodeData.deceptive_c,
        gCheatCodeData.deceptive_d,
        gCheatCodeData.deceptive_f,
        gCheatCodeData.deceptive_g,
        gCheatCodeData.deceptive_h,
        gCheatCodeData.deceptive_j,
        gCheatCodeData.deceptive_k,
        gCheatCodeData.deceptive_l,
        gCheatCodeData.deceptive_m,
        gCheatCodeData.deceptive_p,
        gCheatCodeData.deceptive_q,
        gCheatCodeData.deceptive_r,
        gCheatCodeData.deceptive_s,
        gCheatCodeData.deceptive_t,
        gCheatCodeData.deceptive_zero,
        gCheatCodeData.unknown_10c,
        gCheatCodeData.war_drb,
        gCheatCodeData.persistence_of_time,
        gCheatCodeData.bronze_sights,
        gCheatCodeData.dream_weaver,
        gCheatCodeData.nuke_n_hacks,
        gCheatCodeData.drink_me,
        gCheatCodeData.shrimp,
        gCheatCodeData.tom_thumb,
        gCheatCodeData.mini_me,
        gCheatCodeData.pygmy,
    },
};
