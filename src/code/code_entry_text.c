#include "types.h"

typedef struct CodeEntryText {
    char erase[8];
    char enter[8];
    char invalid_code[20];
    char keys[32][4];
} CodeEntryText;

CodeEntryText gCodeEntryText = {
    "ERASE",
    "ENTER",
    "INVALID INPUT CODE",
    {
        "B", "C", "D", "F", "G", "H", "J", "K", "L", "M", "N",
        "P", "Q", "R", "S", "T", "V", "W", "X", "Y", "Z", "0",
        "1", "2", "3", "4", "5", "6", "7", "8", "9", "+",
    },
};
