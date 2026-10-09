#include "types.h"

typedef struct CampaignMapHelpText {
    char move[28];
    char cross_ocean[36];
} CampaignMapHelpText;

CampaignMapHelpText gCampaignMapHelpText = {
    "Move to Next Map Location",
    "Select the Arrow to Cross the Ocean",
};
