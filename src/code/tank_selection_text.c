#include "types.h"

typedef struct TankSelectionText {
    char tank_bucks[12];
    char change_tank[12];
    char choose_tank[12];
} TankSelectionText;

TankSelectionText gTankSelectionText = {
    "TANK BUCKS", "Change Tank", "Choose Tank",
};
