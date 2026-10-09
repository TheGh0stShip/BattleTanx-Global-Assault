#include "types.h"

typedef struct CodeEntryHelpText {
    char move_highlight[32];
    char select_character[32];
    char erase_character[36];
    char enter_code[40];
} CodeEntryHelpText;

CodeEntryHelpText gCodeEntryHelpText = {
    "Move Highlight to New Character",
    "Select Highlighted Character",
    "Select ERASE to remove a character",
    "Select ENTER to use the inputted code",
};
