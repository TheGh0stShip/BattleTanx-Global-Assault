#include "types.h"

typedef struct TitleLegalText {
    char title[12];
    char copyright[252];
    char licensed_by_nintendo[24];
} TitleLegalText;

TitleLegalText gTitleLegalText = {
    "BattleTanx^",
    "@ 1999 The 3DO Company.  All rights reserved.\n"
    "3DO, BattleTanx, Global Assault, and their respective logos, are trademarks "
    "and/or service marks of The 3DO Company in the U.S. and other countries.  "
    "All other trademarks belong to their respective owners.",
    "LICENSED BY NINTENDO",
};
