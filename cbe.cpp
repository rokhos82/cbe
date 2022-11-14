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

ofstream CBE::debugFile;
CBE::debugFile.open("debug.txt",ios::write);

string BE::AttFleetStr = "", BE::AttRaceName = "", BE::AttFleetName = "";
string BE::AttBattleStr = "", BE::AttDamageStr = "", BE::AttRetreatStr = "";
string BE::DefFleetStr = "", BE::DefRaceName = "", BE::DefFleetName = "";
string BE::DefBattleStr = "", BE::DefDamageStr = "", BE::DefRetreatStr = "";

long BE::AttShipsTotal = 0, BE::DefShipsTotal = 0;
long BE::AttBreakOff = 0, BE::AttTargetBonus = 0;
long BE::DefBreakOff = 0, BE::DefTargetBonus = 0;
long BE::AttShipsLeft = 0, BE::AttFleetStrength = 0;
long BE::AttTargetPriority = 0, BE::AttIsCloaked = 0; 
long BE::AttIsMixed = 0;
long BE::AttSurprised = 0, BE::AttReserve = 0;
long BE::DefShipsLeft = 0, BE::DefFleetStrength = 0;
long BE::DefTargetPriority = 0, BE::DefIsCloaked = 0;
long BE::DefIsMixed = 0;
long BE::DefSurprised = 0, BE::DefReserve = 0;

long BE::CombatRound = 0, BE::RetreatFlag = 0, BE::AttFledFlag = 0, BE::DefFledFlag = 0, BE::ReactorBreachFlag = 0, BE::CritCount = 0, BE::firepower = 0;

double BE::DM_ToHitA = 0.0, BE::DM_ToHitB = 0.0;

long BE::A = 0, BE::B = 0, BE::C = 0, BE::D = 0, BE::E = 0, BE::L = 0, BE::X = 0, BE::Y = 0, BE::aa = 0, BE::bb = 0;

long BE::Target1 = 0, BE::Target2 = 0;
long BE::dice1 = 0, BE::dice2 = 0, BE::dice3 = 0;
long BE::Damage1 = 0, BE::Damage2 = 0, BE::Damage3 = 0, BE::Scope = 0;
long BE::Crits = 0, BE::Shields = 0, BE::Hull = 0, BE::DamageLevel = 0;
long BE::AttHasFighters = 0, BE::DefHasFighters = 0;
long BE::CritDamageFlag = 0, BE::CritSpecialFlag = 0;

long BE::BO_AttackTotal = 0, BE::BO_AttackPercent = 0, BE::BO_Att = 0;
long BE::BO_DefenseTotal = 0, BE::BO_DefensePercent = 0, BE::BO_Def = 0;

long BE::MissileB = 0, BE::MissileS = 0, BE::MissileT = 0, BE::MissileH = 0;

double BE::ShieldsPercent = 0.0, BE::HullPercent = 0.0;

string BE::AttFile = "", BE::DefFile = "", BE::TempAFile = "", BE::TempBFile = "";
string BE::ActiveFile = "", BE::TempStr = "", BE::ShipCritStr = "";
string BE::CriticalStr = "", BE::RaceStr = "";

long BE::MaximumDamage = 0, BE::AttacksMax = 0, BE::AttacksIndex = 0;

BE::AttackInfo BE::Attacks[20000];
BE::SalvoInfo BE::Salvos[200];

string BE::AttShipStr[9999];
string BE::DefShipStr[9999];
string BE::AttCritStr[9999];
string BE::DefCritStr[9999];

long BE::CurBeamA[9999];
long BE::CurShieldA[9999];
long BE::CurTorpA[9999];
long BE::CurHullA[9999];
long BE::HitsA[9999];
long BE::PenHitsA[9999];
long BE::StatusA[9999];
long BE::AmmoA[9999];
long BE::MaxBeamA[9999];
long BE::MaxShieldA[9999];
long BE::MaxTorpA[9999];
long BE::MaxHullA[9999];
long BE::CurDamA[9999];
string BE::SpecialA[9999];
long BE::BPAttackCritA[9999];

long BE::CurBeamB[9999];
long BE::CurShieldB[9999];
long BE::CurTorpB[9999];
long BE::CurHullB[9999];
long BE::HitsB[9999];
long BE::PenHitsB[9999];
long BE::StatusB[9999];
long BE::AmmoB[9999];
long BE::MaxBeamB[9999];
long BE::MaxShieldB[9999];
long BE::MaxTorpB[9999];
long BE::MaxHullB[9999];
long BE::CurDamB[9999];
string BE::SpecialB[9999];
long BE::BPAttackCritB[9999];

string BE::TempAttCritStr[9999];
string BE::TempDefCritStr[9999];

long BE::TempCurBeamA[9999];
long BE::TempCurShieldA[9999];
long BE::TempCurTorpA[9999];
long BE::TempCurHullA[9999];
long BE::TempHitsA[9999];
long BE::TempPenHitsA[9999];
long BE::TempStatusA[9999];
long BE::TempCurDamA[9999];
string BE::TempSpecialA[9999];

long BE::TempCurBeamB[9999];
long BE::TempCurShieldB[9999];
long BE::TempCurTorpB[9999];
long BE::TempCurHullB[9999];
long BE::TempHitsB[9999];
long BE::TempPenHitsB[9999];
long BE::TempStatusB[9999];
long BE::TempCurDamB[9999];
string BE::TempSpecialB[9999];

string BE::terrain[7];
long BE::terrain_def[7];
string BE::terrain_special[7];

long BE::LocationA[9999];
long BE::LocationB[9999];
long BE::OrdersA[9999];
long BE::OrdersB[9999];

long BE::CRIT_DIS = 0, BE::CRIT_HEAT = 0, BE::CRIT_MESON = 0;
long BE::CRIT_VIBRO = 0, BE::CRIT_BP = 0, BE::CRIT_SPECIAL = 0;

long BE::dice = 0;
long BE::ret = 0;

bool BE::LoadA_ready = false, BE::LoadD_ready = false;
bool BE::AttHasLongRange = false, BE::DefHasLongRange = false;
string BE::GroupName = "Fleets", BE::UnitName = "Ships";

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

        cout << BE::AttShipsLeft << " units loeaded." << endl;
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
        BE::DefRaceName = fleet.RaceName;
        BE::DefFleetName = fleet.FleetName;
        BE::DefBreakOff = fleet.BreakOff;
        BE::DefShipsTotal = fleet.ShipsTotal;
        BE::DefFleetStrength = fleet.FleetStrength;
        BE::DefShipsLeft = fleet.ShipsLeft;
        BE::DefTargetBonus = fleet.TargetBonus;
        BE::DefTargetPriority = fleet.TargetPriority;
        BE::DefReserve = fleet.Reserve;

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
        cout << BE::DefShipsLeft << " units loaded." << endl;
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
            CBE::debugFile << BE::AttShipStr[i] << "," 
                          << BE::MaxBeamA[i] << "," 
                          << BE::CurBeamA[i] << "," 
                          << BE::MaxShieldA[i] << "," 
                          << BE::CurShieldA[i] << "," 
                          << BE::MaxTorpA[i] << "," 
                          << BE::CurTorpA[i] << "," 
                          << BE::MaxHullA[i] << "," 
                          << BE::CurHullA[i] << "," 
                          << BE::CurDamA[i] << "," 
                          << BE::StatusA[i] << "," 
                          << BE::AmmoA[i] << "," 
                          << BE::SpecialA[i] << "\n";
            // IF the current ships hull is less than 1 OR the ship is fled OR (the ships is a missile AND the combat round is greater than 0)
            if(BE::CurHullA[i] < 1 || IsFled(BE::SpecialA[i]) || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0)) {
                // Do nothing as this ship is either dead, fled, or a missile
                cerr << BE::AttShipStr[i] << " is dead/fled/msl" << endl;
            }
            else {
                // Write this ship to the TempAFile csv
                tempAFile << BE::AttShipStr[i] << "," 
                          << BE::MaxBeamA[i] << "," 
                          << BE::CurBeamA[i] << "," 
                          << BE::MaxShieldA[i] << "," 
                          << BE::CurShieldA[i] << "," 
                          << BE::MaxTorpA[i] << "," 
                          << BE::CurTorpA[i] << "," 
                          << BE::MaxHullA[i] << "," 
                          << BE::CurHullA[i] << "," 
                          << BE::CurDamA[i] << "," 
                          << BE::StatusA[i] << "," 
                          << BE::AmmoA[i] << "," 
                          << BE::SpecialA[i] << "\n";
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
    cout << "Opening " << BE::TempAFile << " as tempA" << endl;
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

    BE::TempAFile = "tempa.csv";
    BE::TempBFile = "tempb.csv";

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
        cout << "\t6: Debug Output" << endl;
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
            case 6:
                debugPrintUnits();
                break;
            default:
                done = true;
        }
    }

    cout << "Goodbye!" << endl;

    // Return a clean run
    return 0;
}