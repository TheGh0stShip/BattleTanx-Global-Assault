#include "types.h"

typedef struct MenuHelpText {
    char select_a_to_go_back[28];
    char move_highlight[28];
    char change_highlighted[24];
    char select_highlighted[24];
    char previous_menu[28];
    char exit_help[28];
} MenuHelpText;

MenuHelpText gMenuHelpText = {
    "Select A to go back also",
    "Move Highlight to New Item",
    "Change Highlighted Item",
    "Select Highlighted Item",
    "Go Back to Previous Menu",
    "Hit Any Button To Exit Help",
};
