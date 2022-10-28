#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>

//#include "FleetInfo.h"
#include "flags.h"
#include "structs.h"
#include "cbe_lib.h"
#include "cbe.h"

using namespace std;

// Global Variables
namespace BE {
    string AttFleetStr = "", AttRaceName = "", AttFleetName = "";
    string AttBattleStr = "", AttDamageStr = "", AttRetreatStr = "";
    string DefFleetStr = "", DefRaceName = "", DefFleetName = "";
    string DefBattleStr = "", DefDamageStr = "", DefRetreatStr = "";

    long AttShipsTotal = 0, DefShipsTotal = 0;
    long AttBreakOff = 0, AttTargetBonus = 0;
    long DefBreakOff = 0, DefTargetBonus = 0;
    long AttShipsLeft = 0, AttFleetStrength = 0;
    long AttTargetPriority = 0, AttIsCloaked = 0; 
    long AttIsMixed = 0;
    long AttSurprised = 0, AttReserve = 0;
    long DefShipsLeft = 0, DefFleetStrength = 0;
    long DefTargetPriority = 0, DefIsCloaked = 0;
    long DefIsMixed = 0;
    long DefSurprised = 0, DefReserve = 0;

    long CombatRound = 0, RetreatFlag = 0, AttFledFlag = 0, DefFledFlag = 0, ReactorBreachFlag = 0, CritCount = 0, firepower = 0;

    double DM_ToHitA = 0.0, DM_ToHitB = 0.0;

    long A = 0, B = 0, C = 0, D = 0, E = 0, L = 0, X = 0, Y = 0, aa = 0, bb = 0;

    long Target1 = 0, Target2 = 0;
    long dice1 = 0, dice2 = 0, dice3 = 0;
    long Damage1 = 0, Damage2 = 0, Damage3 = 0, Scope = 0;
    long Crits = 0, Shields = 0, Hull = 0, DamageLevel = 0;
    long AttHasFighters = 0, DefHasFighters = 0;
    long CritDamageFlag = 0, CritSpecialFlag = 0;

    long BO_AttackTotal = 0, BO_AttackPercent = 0, BO_Att = 0;
    long BO_DefenseTotal = 0, BO_DefensePercent = 0, BO_Def = 0;

    long MissileB = 0, MissileS = 0, MissileT = 0, MissileH = 0;

    double ShieldsPercent = 0.0, HullPercent = 0.0;

    string AttFile = "", DefFile = "", TempAFile = "", TempBFile = "";
    string ActiveFile = "", TempStr = "", ShipCritStr = "";
    string CriticalStr = "", RaceStr = "";

    long MaximumDamage = 0, AttacksMax = 0, AttacksIndex = 0;
    AttackInfo Attacks[20000];
    SalvoInfo Salvos[200];

    string AttShipStr[9999];
    string DefShipStr[9999];
    string AttCritStr[9999];
    string DefCritStr[9999];

    long CurBeamA[9999];
    long CurShieldA[9999];
    long CurTorpA[9999];
    long CurHullA[9999];
    long HitsA[9999];
    long PenHitsA[9999];
    long StatusA[9999];
    long AmmoA[9999];
    long MaxBeamA[9999];
    long MaxShieldA[9999];
    long MaxTorpA[9999];
    long MaxHullA[9999];
    long CurDamA[9999];
    string SpecialA[9999];
    long BPAttackCritA[9999];
    
    long CurBeamB[9999];
    long CurShieldB[9999];
    long CurTorpB[9999];
    long CurHullB[9999];
    long HitsB[9999];
    long PenHitsB[9999];
    long StatusB[9999];
    long AmmoB[9999];
    long MaxBeamB[9999];
    long MaxShieldB[9999];
    long MaxTorpB[9999];
    long MaxHullB[9999];
    long CurDamB[9999];
    string SpecialB[9999];
    long BPAttackCritB[9999];

    string TempAttCritStr[9999];
    string TempDefCritStr[9999];

    long TempCurBeamA[9999];
    long TempCurShieldA[9999];
    long TempCurTorpA[9999];
    long TempCurHullA[9999];
    long TempHitsA[9999];
    long TempPenHitsA[9999];
    long TempStatusA[9999];
    long TempCurDamA[9999];
    string TempSpecialA[9999];

    long TempCurBeamB[9999];
    long TempCurShieldB[9999];
    long TempCurTorpB[9999];
    long TempCurHullB[9999];
    long TempHitsB[9999];
    long TempPenHitsB[9999];
    long TempStatusB[9999];
    long TempCurDamB[9999];
    string TempSpecialB[9999];

    string terrain[7];
    long terrain_def[7];
    string terrain_special[7];

    long LocationA[9999];
    long LocationB[9999];
    long OrdersA[9999];
    long OrdersB[9999];

    long CRIT_DIS = 0, CRIT_HEAT = 0, CRIT_MESON = 0;
    long CRIT_VIBRO = 0, CRIT_BP = 0, CRIT_SPECIAL = 0;

    long dice = 0;
    long ret = 0;

    bool LoadA_ready = false, LoadD_ready = false;
    bool AttHasLongRange = false, DefHasLongRange = false;
    string GroupName = "Fleets", UnitName = "Ships";
}

void loadAttackingFleet(string fname) {
    cout << "Loading attacking fleet from: " << fname << endl;
    BE::AttFleetStr = fname;

    fstream attFile;
    attFile.open(BE::AttFleetStr,ios::in | ios::binary);
    if(attFile.is_open()) {
        string line = "";
        getline(attFile,line,'\n');
        BE::FleetInfo fleet = parseFleetHeader(line);
        BE::AttRaceName = fleet.RaceName;
        BE::AttFleetName = fleet.FleetName;
        BE::AttBreakOff = fleet.BreakOff;
        BE::AttShipsTotal = fleet.ShipsTotal;
        BE::AttFleetStrength = fleet.FleetStrength;
        BE::AttShipsLeft = fleet.ShipsLeft;
        BE::AttTargetBonus = fleet.TargetBonus;
        BE::AttTargetPriority = fleet.TargetPriority;
        BE::AttReserve = fleet.Reserve;

        long numUnits = 0;
        while(getline(attFile,line,'\n')) {
            BE::UnitInfo unit = parseUnit(line);

            BE::AttShipStr[numUnits] = unit.UnitName;
            BE::MaxBeamA[numUnits] = unit.MaxBeam;
            BE::CurBeamA[numUnits] = unit.CurBeam;
            BE::MaxShieldA[numUnits] = unit.MaxShield;
            BE::CurShieldA[numUnits] = unit.CurShield;
            BE::MaxTorpA[numUnits] = unit.MaxTorp;
            BE::CurTorpA[numUnits] = unit.CurTorp;
            BE::MaxHullA[numUnits] = unit.MaxHull;
            BE::CurHullA[numUnits] = unit.CurHull;
            BE::CurDamA[numUnits] = unit.CurDam;
            BE::StatusA[numUnits] = unit.Status;
            BE::AmmoA[numUnits] = unit.Ammo;
            BE::SpecialA[numUnits] = unit.Special;

            numUnits++;
        }

        BE::AttShipsLeft = numUnits;

        attFile.close();

        CBE::attackerLoaded = true;
    }
}

void loadDefendingFleet(string fname) {
    cout << "Loading defending fleet from: " << fname << endl;
    BE::DefFleetStr = fname;

    fstream defFile;
    defFile.open(BE::DefFleetStr,ios::in | ios::binary);
    if(defFile.is_open()) {
        string line = "";
        getline(defFile,line,'\n');
        BE::FleetInfo fleet = parseFleetHeader(line);
        BE::AttRaceName = fleet.RaceName;
        BE::AttFleetName = fleet.FleetName;
        BE::AttBreakOff = fleet.BreakOff;
        BE::AttShipsTotal = fleet.ShipsTotal;
        BE::AttFleetStrength = fleet.FleetStrength;
        BE::AttShipsLeft = fleet.ShipsLeft;
        BE::AttTargetBonus = fleet.TargetBonus;
        BE::AttTargetPriority = fleet.TargetPriority;
        BE::AttReserve = fleet.Reserve;

        long numUnits = 0;
        while(getline(defFile,line,'\n')) {
            BE::UnitInfo unit = parseUnit(line);

            BE::DefShipStr[numUnits] = unit.UnitName;
            BE::MaxBeamB[numUnits] = unit.MaxBeam;
            BE::CurBeamB[numUnits] = unit.CurBeam;
            BE::MaxShieldB[numUnits] = unit.MaxShield;
            BE::CurShieldB[numUnits] = unit.CurShield;
            BE::MaxTorpB[numUnits] = unit.MaxTorp;
            BE::CurTorpB[numUnits] = unit.CurTorp;
            BE::MaxHullB[numUnits] = unit.MaxHull;
            BE::CurHullB[numUnits] = unit.CurHull;
            BE::CurDamB[numUnits] = unit.CurDam;
            BE::StatusB[numUnits] = unit.Status;
            BE::AmmoB[numUnits] = unit.Ammo;
            BE::SpecialB[numUnits] = unit.Special;

            numUnits++;
        }

        BE::DefShipsLeft = numUnits;

        defFile.close();

        CBE::defenderLoaded = true;
    }
}

bool IsMissile(const string & special) {
    bool res = false;

    // If the position of "MSL" is NOT npos
    if(special.find("MSL") != string::npos) {
        // Then it must be a missile
        res = true;
    }

    return res;
}

bool IsCaptured(const string & special) {
    bool res = false;

    // IF the position of "CAPTURED" is NOT npos (no position)
    if(special.find("CAPTURED") != string::npos) {
        // Then it must be captured
        res = true;
    }

    return res;
}

bool IsCrippled(const string & special) {
    bool res = false;

    // IF the position of "CRIPPLE" is NOT npos (no position)
    if(special.find("CRIPPLE") != string::npos) {
        // Then it must be crippled
        res = true;
    }

    return res;
}

bool IsFled(const string & special) {
    bool res = false;

    // IF the position of "FLED" is NOT npos (no position)
    if(special.find("FLED") != string::npos) {
        // Then it must be fled
        res = true;
    }

    return res;
}

void writeTempFiles() {
    long old_AttShipsLeft = 0;
    long old_DefShipsLeft = 0;

    old_AttShipsLeft = BE::AttShipsLeft; // TODO: Move to variable declaration.
    old_DefShipsLeft = BE::DefShipsLeft; // TODO: Move to variable declaration.

    // Write the Working and Temp files

    // Find Dead and Fled Ships
    ofstream attFledFile;
    attFledFile.open("fled_att.csv",ios::out | ios::binary | ios::trunc);
    for(int i = 0;i < old_AttShipsLeft;i++) {
        // IF the current ships hull is less than 1 OR (the current ship is a missile AND the combat round greater than 0)
        // This clears out dead ships and missiles from the ships list/array
        if(BE::CurHullA[i] < 1 || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0)) {
            BE::AttShipsLeft = BE::AttShipsLeft - 1;
            // TODO: Add writing to a att_dead.csv file
        }
        // ELSE non-dead ships that are not missiles
        else {
            // IF NOT a captured unit AND NOT a crippled unit
            if(!IsCaptured(BE::SpecialA[i]) && !IsCrippled(BE::SpecialA[i])) {
                // IF the unit has fled
                // Units that have been marked as fled but are captured or cripple never get considered fled.
                if(IsFled(BE::SpecialA[i])) {
                    BE::AttShipsLeft = BE::AttShipsLeft - 1;
                    attFledFile << BE::AttShipStr[i] << "," << BE::MaxBeamA[i] << "," << BE::CurBeamA[i] << "," << BE::MaxShieldA[i] << "," << BE::CurShieldA[i] << "," << BE::MaxTorpA[i] << "," << BE::CurTorpA[i] << "," << BE::MaxHullA[i] << "," << BE::CurHullA[i] << "," << BE::CurDamA[i] << "," << BE::StatusA[i] << "," << BE::AmmoA[i] << "," << BE::SpecialA[i] << "\n";
                }
            }
        }
    }
    // Close the fled_att.csv file here
    attFledFile.flush();
    attFledFile.close();

    // Open TempAFile here
    ofstream tempAFile;
    tempAFile.open(BE::TempAFile,ios::out | ios::binary | ios::trunc);
    // Write the header to the fleet file here (TempAFile)
    tempAFile << BE::AttRaceName << "," << BE::AttFleetName << "," << BE::AttBreakOff << "," << BE::AttShipsTotal << "," << BE::AttFleetStrength << "," << BE::AttShipsLeft << "," << BE::AttTargetBonus << "," << BE::AttTargetPriority << "," << BE::AttReserve << "\n";
    if(BE::AttShipsLeft > 0) {
        for(int i = 0;i < old_AttShipsLeft;i++) {
            // IF the current ships hull is less than 1 OR the ship is fled OR (the ships is a missile AND the combat round is greater than 0)
            if(BE::CurHullA[i] < 1 || IsFled(BE::SpecialA[i]) || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0)) {
                // Do nothing as this ship is either dead, fled, or a missile
            }
            else {
                // Write this ship to the TempAFile csv
                tempAFile << BE::AttShipStr[i] << "," << BE::MaxBeamA[i] << "," << BE::CurBeamA[i] << "," << BE::MaxShieldA[i] << "," << BE::CurShieldA[i] << "," << BE::MaxTorpA[i] << "," << BE::CurTorpA[i] << "," << BE::MaxHullA[i] << "," << BE::CurHullA[i] << "," << BE::CurDamA[i] << "," << BE::StatusA[i] << "," << BE::AmmoA[i] << "," << BE::SpecialA[i] << "\n";
            }
        }
    }
    tempAFile.flush();
    tempAFile.close();

    // Now do it all again for the defenders
    // TODO: Write a function that takes CurHullA,SpecialA, etc so that this doesn't have to be coded twice...
    //      This may need to wait until I have restructed the fleet data into objects
    // Find Dead and Fled Ships
    ofstream defFledFile;
    defFledFile.open("fled_def.csv",ios::out | ios::binary | ios::trunc);
    for(int i = 0;i < old_DefShipsLeft;i++) {
        // IF the current ships hull is less than 1 OR (the current ship is a missile AND the combat round greater than 0)
        // This clears out dead ships and missiles from the ships list/array
        if(BE::CurHullB[i] < 1 || (IsMissile(BE::SpecialB[i]) && BE::CombatRound > 0)) {
            BE::DefShipsLeft = BE::DefShipsLeft - 1;
            // TODO: Add writing to a att_dead.csv file
        }
        // ELSE non-dead ships that are not missiles
        else {
            // IF NOT a captured unit AND NOT a crippled unit
            if(!IsCaptured(BE::SpecialB[i]) && !IsCrippled(BE::SpecialB[i])) {
                // IF the unit has fled
                // Units that have been marked as fled but are captured or cripple never get considered fled.
                if(IsFled(BE::SpecialB[i])) {
                    BE::DefShipsLeft = BE::DefShipsLeft - 1;
                    defFledFile << BE::DefShipStr[i] << "," << BE::MaxBeamB[i] << "," << BE::CurBeamB[i] << "," << BE::MaxShieldB[i] << "," << BE::CurShieldB[i] << "," << BE::MaxTorpB[i] << "," << BE::CurTorpB[i] << "," << BE::MaxHullB[i] << "," << BE::CurHullB[i] << "," << BE::CurDamB[i] << "," << BE::StatusB[i] << "," << BE::AmmoB[i] << "," << BE::SpecialB[i] << "\n";
                }
            }
        }
    }
    // Close the fled_def.csv file here
    defFledFile.flush();
    defFledFile.close();

    // Open TempAFile here
    ofstream tempBFile;
    tempBFile.open(BE::TempBFile,ios::out | ios::binary | ios::trunc);
    // Write the header to the fleet file here (TempAFile)
    tempBFile << BE::DefRaceName << "," << BE::DefFleetName << "," << BE::DefBreakOff << "," << BE::DefShipsTotal << "," << BE::DefFleetStrength << "," << BE::DefShipsLeft << "," << BE::DefTargetBonus << "," << BE::DefTargetPriority << "," << BE::DefReserve << "\n";
    if(BE::DefShipsLeft > 0) {
        for(int i = 0;i < old_DefShipsLeft;i++) {
            // IF the current ships hull is less than 1 OR the ship is fled OR (the ships is a missile AND the combat round is greater than 0)
            if(BE::CurHullB[i] < 1 || IsFled(BE::SpecialB[i]) || (IsMissile(BE::SpecialB[i]) && BE::CombatRound > 0)) {
                // Do nothing as this ship is either dead, fled, or a missile
            }
            else {
                // Write this ship to the TempAFile csv
                tempBFile << BE::DefShipStr[i] << "," << BE::MaxBeamB[i] << "," << BE::CurBeamB[i] << "," << BE::MaxShieldB[i] << "," << BE::CurShieldB[i] << "," << BE::MaxTorpB[i] << "," << BE::CurTorpB[i] << "," << BE::MaxHullB[i] << "," << BE::CurHullB[i] << "," << BE::CurDamB[i] << "," << BE::StatusB[i] << "," << BE::AmmoB[i] << "," << BE::SpecialB[i] << "\n";
            }
        }
    }
    tempBFile.flush();
    tempBFile.close();
}

void readTempA() {
    // Open tempA file for reading
    ifstream tempA;
    cout << "Opening " << BE::TempAFile << " as tempA";
    tempA.open(BE::TempAFile,ios::binary);
    // Read in the fleet header line
    if(tempA.is_open()) {
        // Read the first line
        string header = "";
        getline(tempA,header,'\n');
        BE::FleetInfo info = parseFleetHeader(header);
        cout << info.RaceName << " - " << info.FleetName << endl;
        BE::AttRaceName = info.RaceName;
        BE::AttFleetName = info.FleetName;

        // Read the unit lines

        // Close the file
        tempA.close();
    }
    else {
        cout << "Unable to open: " << BE::TempAFile << endl;
    }
}

void readTempB() {}

/*
* Main BattleEngine Loop
*/
void be_main() {
    long AttackLoop = 0, AbortCounter = 0;
    long AttShipsRead = 0, DefShipsRead = 0;

    string status = "", NewTag = "", temp_str = "";

    bool AttHasScreen = false, DefHasScreen = false;
    bool AttHasReserveUnits = false, DefHasReserveUnits = false;

    long HullTarget = 0, CombatBonus = 0, ForceID = 0;
    long SeekTarget = 0, HasReaded = 0;
    long AttVal = 0, DefVal = 0, BPDice = 0, SuicideBonus = 0;
    long FirePower1 = 0, FirePower2 = 0;
    long DataLinkA[52], DataLinkB[52];
    long AttGone = 0, DefGone = 0, Special1 = 0, Special2 = 0;
    long number_of_attacks = 0;

    BE::CombatInfo Hits[200];

    string CombatStr = "";

    long tmp = 0, TempAttShipsLeft = 0, TempDefShipsLeft = 0;
    long missile_counter = 0, BeamsOut = 0, TorpsOut = 0;
    long beam_count = 0, torp_count = 0;
    long AttNumValidTargets = 0, DefNumValidTargets = 0;
    long YieldBonus = 0, ShipHit = 0, offset = 0;

    double BaseAccuracy = 0.0;

    long start = 0, start1 = 0, old_start = 0, sc = 0, packet_size = 0;

    string weapon_str = "";
    string missile_str = "";
    long swt = 0;
    bool Strip = false;
    string new_str = "";
    long AutoMiss = 0;
    long AutoHit = 0;

    // Seed the random number generator
    srand(time(NULL));

    // Set counts and flags to their default values
    BE::CombatRound = 0;
    BE::RetreatFlag = 0;
    BE::AttShipsLeft = 0;
    BE::DefShipsLeft = 0;
    AttShipsRead = 0;
    DefShipsRead = 0;
    AttackLoop = 1; // set to zero to end combat | TODO: This can be moved to variable declaration
    BE::AttacksMax = 20000; // TODO: This can be moved to the variable declaration
    BE::AttacksIndex = 0;

    BE::DM_ToHitA = 0;
    BE::DM_ToHitB = 0;

    // Original code has a lot of Form code here for the GUI.  Will pick up after that.

    // Basic init stuff
    BE::BO_Att = 0; // TODO: This can be moved to the variable declaration.
    BE::BO_Def = 0; // TODO: This can be moved to the variable declaration.
    BaseAccuracy = CBE::baseAccuracy; // TODO: Need to setup a command line arg and a interface for this.  | TODO: Move to variable declaration. | TODO: Move to BE namespace

    // TODO: Open the battle_report and damage_report files here for output

    // NEW: Set TempAFile and TempBFile to tempa.csv and tempb.csv for initial temp file writes.

    writeTempFiles();

    // NEW: Set back to original values

    // NOTE: TempAFile and TempBFile are set in the menu code when the user selects the attacker and defender files

    // This is where the program comes back in subsequent rounds
    do
    {
        // Load in the attackers
        readTempA();
        // Load in the defenders
        readTempB();

        // Trip the Round counter
        BE::CombatRound = BE::CombatRound + 1;
    } while (true/* condition | TODO: replace true with variable.  Perhaps AttackLoop? */);
    
}

/*
* valid command line arguments are:
* -a <string> - the filename for the attacking fleet csv
* -d <string> - the filename for the defending fleet csv
* -g <string> - the group name for this simulation (default is Fleets)
* -u <string> - the unit name for this simulation (default is Ships)
*/
int main(int argc, char *argv[]) {
    int i = 1;
    string fname = "";
    while(i < argc) {
        string cmd = argv[i];
        if(cmd == "-a") {
            // Move to the next argument
            i++;
            // Load the attacking fleet
            fname = argv[i];
            loadAttackingFleet(fname);
        }
        else if (cmd == "-d") {
            // Move to the next argument
            i++;
            // Load the defending fleet
            fname = argv[i];
            loadDefendingFleet(fname);
        }
        else if (cmd == "-hit") {
            // Move to the next argument
            i++;
            // Set the to hit
            CBE::baseAccuracy = stof(argv[i]);
        }
        else {
            cout << "Unknown command: " << cmd << endl;
        }
        i++;
    }

    // Present the menu to the user
    bool done = false;
    int choice = 0;
    while(!done) {
        // Display the menu
        cout << "BattleEngine 1.1" << endl;
        if(CBE::attackerLoaded) {
            cout << "\t1: Attacking Fleet: " << BE::AttFleetStr << endl;
        }
        else {
            cout << "\t1: Load Attacking Fleet" << endl;
        }
        if(CBE::defenderLoaded) {
            cout << "\t2: Defending Fleet: " << BE::DefFleetStr << endl;
        }
        else {
            cout << "\t2: Load Defending Fleet" << endl;
        }
        cout << "\t3: Update Settings" << endl;
        cout << "\t4: One Turn" << endl;
        cout << "\t5: Fight!" << endl;
        cout << "\t0: Exit" << endl;
        cout << "Selection: ";

        // Wait for user input
        cin >> choice;

        // What did the user choose?
        switch(choice) {
            case 1:
                // Load the attacking fleet
                cout << "Attackers File Name: ";
                cin >> fname;
                loadAttackingFleet(fname);
                break;
            case 2:
                // Load the defending fleet
                cout << "Defenders File Name: ";
                cin >> fname;
                loadDefendingFleet(fname);
                break;
            case 4:
                cout << "Doing one round of combat" << endl;
                break;
            case 5:
                // Do the thing!
                cout << "Starting combat" << endl;
                be_main();
                break;
            default:
                done = true;
        }
    }

    cout << "Goodbye!" << endl;

    // Return a clean run
    return 0;
}