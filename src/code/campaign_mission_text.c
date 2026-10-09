#include "types.h"

typedef struct CampaignMissionText {
    char location_00[0x10];
    char location_01[0x10];
    char location_02[0x10];
    char location_03[0x18];
    char location_04[0xC];
    char location_05[0xC];
    char location_06[0x10];
    char location_07[0x18];
    char location_08[0x10];
    char location_09[0x14];
    char location_10[0xC];
    char location_11[0x10];
    char location_12[0x10];
    char location_13[0x14];
    char location_14[0x14];
    char location_15[0x18];
    char location_16[0x10];
    char location_17[0x10];
    char location_18[0xC];
    char location_19[0xC];
    char location_20[0xC];
    char location_21[0xC];
    char location_22[0x8];
    char location_23[0xC];
    char objective_00[0x1C];
    char objective_01[0x18];
    char objective_02[0x20];
    char objective_03[0x18];
    char objective_04[0x24];
    char objective_05[0x1C];
    char objective_06[0x14];
    char objective_07[0x24];
    char objective_08[0x18];
    char objective_09[0x1C];
    char objective_10[0x28];
    char objective_11[0x14];
    char objective_12[0x24];
    char objective_13[0x20];
    char objective_14[0x20];
    char objective_15[0x14];
    char objective_16[0x1C];
    char objective_17[0x1C];
    char objective_18[0x24];
    char objective_19[0x18];
    char objective_20[0x28];
    char objective_21[0x28];
    char objective_22[0x28];
    char objective_23[0x18];
    char objective_24[0x28];
    char objective_25[0x28];
    char briefing_00[0x70];
    char briefing_01[0x68];
    char briefing_02[0x68];
    char briefing_03[0x5C];
    char briefing_04[0x68];
    char briefing_05[0x74];
    char briefing_06[0x70];
    char briefing_07[0x74];
    char briefing_08[0x70];
    char briefing_09[0x74];
    char briefing_10[0x78];
    char briefing_11[0x74];
    char briefing_12[0x78];
    char briefing_13[0x7C];
    char briefing_14[0x74];
    char briefing_15[0x6C];
    char briefing_16[0x78];
    char briefing_17[0x68];
    char briefing_18[0x74];
} CampaignMissionText;

CampaignMissionText gCampaignMissionText = {
    "-SF AIRPORT-",
    "-SF BREAKOUT-",
    "-TRUCK STOP-",
    "-TEXAS SLAVE FORTRESS-",
    "-DRIVE IN-",
    "-DC MALL-",
    "-WHITE HOUSE-",
    "-HOUSES OF PARLIAMENT-",
    "-TOWER BRIDGE-",
    "-TOWER OF LONDON-",
    "-BISTRO-",
    "-CHAMPS ELYSEE-",
    "-EIFFEL TOWER-",
    "-BRANDENBURG GATE-",
    "-BERLIN WAR ZONE-",
    "-ESCAPE FROM BERLIN-",
    "-SHORE PATROL-",
    "-ASSAULT ON SF-",
    "-ALCATRAZ-",
    "-LAKEPARK-",
    "-PANHANDLE-",
    "-RAILYARD-",
    "-SFO-",
    "-CROSSFIRE-",
    "DESTROY ALL INVADING TANKS",
    "GET TO THE ESCAPE SHIP",
    "DESTROY ALL TANKS AND BUNKERS",
    "RESCUE ALL PRISONERS",
    "DESTROY BOTH PROJECTOR BUILDINGS",
    "CAPTURE ALL DATA AND ESCAPE",
    "DESTROY ALL ENEMIES",
    "RESCUE ALL IRON MAIDENS AND ESCAPE",
    "GET ACROSS THE BRIDGE",
    "DESTROY ALL TANKS AND BOATS",
    "GET ALL FOUR SCIENTISTS IN YOUR BASE",
    "DESTROY ALL TANKS",
    "DESTROY THE EIFFEL TOWER REACTOR",
    "ESCORT THE CONVOY INTO BERLIN",
    "RESCUE ALL PRISONERS AND ESCAPE",
    "PROTECT THE CONVOY",
    "DESTROY ALL TANKS AND BOATS",
    "DESTROY ALL TANKS AND BOATS",
    "FIND AND DESTROY THE ANNIHILATOR",
    "DEFEAT TEN ENEMY TANKS",
    "GET ALL QUEENLORDS BACK TO YOUR BASE",
    "BE THE FIRST TO RESCUE TEN QUEENLORDS",
    "KEEP THE QUEENLORD SAFE IN YOUR BASE",
    "DEFEAT 10 ENEMY TANKS",
    "GET MORE OF THE ENEMY BEFORE TIME IS UP",
    "PROTECT YOUR CONVOY OR DESTROY THEIRS",
    "Unknown enemy forces have invaded San Francisco Airport.  You and three of your allied tanks must stop them.",
    "You must get your family away from the traitorous masses.  Use the tunnel to get to the escape ship. ",
    "You must raid a camp guarded by the Skull Riders gang.  Be sure to destroy the tank emitting bunkers.",
    "Use the Slave Train as cover to avoid the gun emplacements, and locate all the prisoners.",
    "Cassandra is using movies to control local gangs. Destroy both movie projector buildings with grenades.",
    "By hitting the Capitol, you can capture data disks containing critical information about Cassandra's operations.",
    "You must deal with Cassandra's bodyguard of Shadow Ops, and destroy her mighty Annihilator class Goliath tank.",
    "You must form a new army in Europe.  Rescue the Iron Maidens gang- they have been imprisoned by the Crimson Guard.",
    "The only way to get to the Crimson Guard's base is to cross Tower Bridge. Use guided missiles to clear a path.",
    "Be sure to defeat all the Crimson Guard forces before taking on the Crimson Lord and his Annihilator class tank.",
    "Cassandra has four secret labs in Paris.  You have secured one, but must rescue scientists from the other three labs.",
    "Cassandra is hiding a secret army of at least 50 tanks in the town.  Sneak in quietly and destroy every last tank.",
    "Destroy the laser equipped Eiffel Tower to stop Cassandra's radio broadcasts.  Shoot the power generator at it's base.",
    "You must move heavy supplies and weapons through the Brandenburg Gate.  The transports must be defended from enemy attack.",
    "Cassandra has hidden your son amongst several look-alike children.  Rescue all three children from the War Zone.",
    "You have hidden the children amongst several transport vehicles.  They must make it out of Berlin intact.",
    "The Storm Ravens have taken over the docks, and must be cleared out before you can safely land your transport ships.",
    "The Storm Ravens have amassed over 70 tanks for their invasion. You must stop them from getting ashore.",
    "Cassandra has established a base on Alcatraz.  You must stop her before she can put a Doomsday plan into action.",
};
