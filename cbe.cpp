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

fstream CBE::debugFile = fstream("debug.txt",ios::out | ios::binary);

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
    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] Loading attacking fleet from: " << fname << endl;
    #endif
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

            #ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Loading unit: "
                           << unit.UnitName << ","
                           << unit.MaxBeam << ","
                           << unit.CurBeam << ","
                           << unit.MaxShield << ","
                           << unit.CurShield << ","
                           << unit.MaxTorp << ","
                           << unit.CurTorp << ","
                           << unit.MaxHull << ","
                           << unit.CurHull << ","
                           << unit.CurDam << ","
                           << unit.Status << ","
                           << unit.Ammo << ","
                           << unit.Special << endl;
            #endif

            numUnits++;
        }

        BE::AttShipsLeft = numUnits;

        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] Attacking Units Loaded: " << BE::AttShipsLeft << endl;
        #endif

        attFile.close();

        cout << BE::AttShipsLeft << " units loaded." << endl;
        CBE::attackerLoaded = true;
    }
}

void loadDefendingFleet(string fname) {
    cout << "Loading defending fleet from: " << fname << endl;
    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] Loading defending fleet from: " << fname << endl;
    #endif
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

            #ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Loaded unit: "
                           << unit.UnitName << ","
                           << unit.MaxBeam << ","
                           << unit.CurBeam << ","
                           << unit.MaxShield << ","
                           << unit.CurShield << ","
                           << unit.MaxTorp << ","
                           << unit.CurTorp << ","
                           << unit.MaxHull << ","
                           << unit.CurHull << ","
                           << unit.CurDam << ","
                           << unit.Status << ","
                           << unit.Ammo << ","
                           << unit.Special << endl;
            #endif

            numUnits++;
        }

        BE::DefShipsLeft = numUnits;

        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] Defending Units Loaded: " << BE::DefShipsLeft << endl;
        #endif

        defFile.close();

        CBE::defenderLoaded = true;
        cout << BE::DefShipsLeft << " units loaded." << endl;
    }
}

string AddTag(const string & source,const string & target) {
    // Add the target tag to the source string and return the modified string.
    int start = 0;
    string res = "";

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] AddTag(source:\"" << source << "\",target:\"" << target << "\")" << endl;
    #endif

    // Find the target tag in the source string first...not sure what this is doing.  Maybe grouping similar tags?
    // Nope!  See the else block below.  Looks like targets are intended to be singltons.  Multiple OFFLINE tags was never implemented...
    start = source.find(target);
    if(start == string::npos) {
        // The target does not exist in the source.
        // Check if the source starts with a bracket
        if(*(source.begin()) == '[' && *(source.end()) == ']') {
            // Yep, this is a weapon bracket
            // Now check if the target tag is uppercase
            if(isupper(target[0])) {
                // The target tag is upper case.  Make sure it goes OUTSIDE any brackets.
                // This leads to an intersting case of being able to add a tag to a salvo string...
                // Test for an ' ' at the end of the string first
                if(*(source.end()) != ' ') {
                    // Add the target tag to the end of the source string with a ' '
                    res = source + " " + target;
                }
                else {
                    // Just add the target tag to the end of the source string
                    res = source + target;
                }
            }
            else {
                // The target tag is lower case.  It can go inside the bracket.
                // We know that the ending character is ']'
                res = source.substr(0,source.size()-1) + " " + target + "]";
            }
        }
        else {
            // Not a bracket so add to the end...working with strings is so dumb.
            // Check for a space and add if necessary.
            if(*(source.end()) != ' ') {
                res = source + " " + target;
            }
            else {
                res = source + target;
            }
        }
    }
    else {
        // The tag already exists.  Just return the original string.
        res = source; // Create a new string   Memory allocation is going to be a mess...
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] AddTag(res:\"" << res << "\")" << endl;
    #endif

    return res;
}

vector<string> GetBrackets(const string & special) {
    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetBrackets(\"" << special << "\")" << endl;
    #endif
    // Setup the dynamic array for the return
    vector<string> brackets;

    // Is there an openning bracket in special?
    int old_start = special.find("[");
    while(old_start != string::npos) {
        // Yes, save that location
        int start = old_start;
        // Get the next closing bracket
        int start1 = special.find("]",start);
        string bracket = special.substr(start,start1-start+1);
        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] GetBrackets(start:" << start << ";start1:" << start1 << ";bracket:\"" << bracket << "\")" << endl;
        #endif
        brackets.push_back(bracket);
        // Get the next openning bracket if it exists
        old_start = special.find("[",start1);
    }

    // Return the bracket strings array
    return brackets;
}

int GetTriHex(const string & digit) {
    int val = stoi(digit,0,36);

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetTriHex(\"" << digit << "\")" << endl;
    #endif

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetTriHex(" << val << ")" << endl;
    #endif

    return val;
}

int HasAmmoWT(const string & special) {
    // Possible return values
    // >0: Indicates that the special includes an ammo tag and returns the value of that tag
    // 0: Indicates that the special includes an ammo tag and that the value is 0
    // <0: Indicates that the special does not include an ammo tag
    // TODO: Setup a set of constants to test against for clarity and consistancy
    int res = CBE::AMMO_EMPTY;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasAmmoWT(\"" << special << "\")" << endl;
    #endif

    // Find the ammo tag and return it's starting position in the special string
    int start = special.find("ammo");
    if(start != string::npos) {
        int middle = special.find(" ",start);
        int end = special.find(" ",middle+1);
        int len = end - middle;
        string ammo = special.substr(middle,len);
        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasAmmoWT(ammo:" << ammo << ")" << endl;
        #endif
        res = stoi(ammo);
    }
    else {
        res = CBE::AMMO_INFINITE;
    }

    // Return the result
    return res;
}

bool HasArtilleryWT(const string & special) {
    bool res = false;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasArtilleryWT(\"" << special << "\")" << endl;
    #endif

    // Look for `artillery` in the special string
    int start = special.find("artillery");
    if(start != string::npos) {
        // Found it!  Set the result to TRUE
        res = true;
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasArtilleryWT(" << res << ")" << endl;
    #endif

    return res;
}

int HasBatteries(const string & special) {
    int res = 0;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasBatteries(\"" << special << "\")" << endl;
    #endif

    for(int i = 0;i < special.length();i++) {
        // Count the opening square brackers to determine number of batteries
        if(special.at(i) == '[') {
            res++;
        }
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "HasBatteries(" << res << ")" << endl;
    #endif

    return res;
}

int HasDelay(const string & special) {
    int start = 0;
    int res = -1;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDelay(special:\"" << special << "\")" << endl;
    #endif

    start = special.find("DELAY");
    if(start != string::npos) {
        int middle = special.find(" ",start);
        int end = special.find(" ",middle+1);
        int len = end - middle;
        string delay = special.substr(middle,len);
        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasDelay(delay:" << delay << ")" << endl;
        #endif
        res = stoi(delay);
    }

    return res;
}

bool HasLong(const string & special) {
    bool res = false;

    // IF the position of "LONG" is NOT npos (no position)
    if(special.find("LONG") != string::npos) {
        // The it must be a long range weapons
        res = true;
    }

    return res;
}

bool HasLongWT(const string & special) {
    bool res = false;

    // IF the position of "long" is NOT npos (no position)
    if(special.find("long") != string::npos) {
        // Then it must have a long range weapon bracket
        res = true;
    }

    return res;
}

bool HasMissileWT(const string & special) {
    bool res = false;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMissileWT(\"" << special << "\")" << endl;
    #endif

    int start = special.find("mis");

    // IF the position of "mis" is NOT npos (no position)
    if(start != string::npos) {
        // Then it must have a mis tag in a bracket
        res = true;

        // This part is ridiculous...
        string B = special.substr(start + 3,1);
        string S = special.substr(start + 4,1);
        string T = special.substr(start + 5,1);
        string H = special.substr(start + 6,1);
        BE::MissileB = GetTriHex(B);
        BE::MissileS = GetTriHex(S);
        BE::MissileT = GetTriHex(T);
        BE::MissileH = GetTriHex(H);
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMissileWT(" << res << ")" << endl;
    #endif

    return res;
}

int HasReserve(const string & special) {
    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasReserve(special:\"" << special << "\")" << endl;
    #endif
    int res = -1;

    // Look for the starting position of "RESERVE"
    int start = special.find("RESERVE");
    if(start != string::npos) {
        int middle = special.find(" ",start);
        int end = special.find(" ",middle+1);
        int len = end - middle;
        string reserve = special.substr(middle,len);
        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasReserve(reserve:" << reserve << ")" << endl;
        #endif
        res = stoi(reserve);
    }

    return res;
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

bool IsCloak(const string & special) {
    bool res = false;

    // IF the position of "CLOAK" is NOT npos (no position)
    if(special.find("CLOAK") != string::npos) {
        // The it must be cloaked
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

bool IsDrifting(const string & special) {
    bool res = false;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsDrifting(\"" << special << "\")" << endl;
    #endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if(special.find("DRIFTING") != string::npos) {
        // Then it must be drifting
        res = true;
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsDrifting(" << res << ")" << endl;
    #endif

    return res;
}

bool IsFighter(const string & special) {
    bool res = false;
    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFighter(" << special << ")" << endl;
    #endif

    // IF the position of "FIGHTER" is NOT npos (no position)
    if(special.find("FIGHTER") != string::npos) {
        // Then it must be a fighter
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

bool IsFlee(const string & special) {
    bool res = false;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFlee(\"" << special << "\")" << endl;
    #endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if(special.find("FLEE") != string::npos) {
        // Then it must be drifting
        res = true;
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFlee(" << res << ")" << endl;
    #endif

    return res;
}

bool IsNoMove(const string & special) {
    bool res = false;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsNoMove(\"" << special << "\")" << endl;
    #endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if(special.find("NOMOVE") != string::npos) {
        // Then it must be drifting
        res = true;
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsNoMove(" << res << ")" << endl;
    #endif

    return res;
}

bool IsSurprise(const string & special) {
    bool res = false;

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsSurprise(special:\"" << special << "\")" << endl;
    #endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if(special.find("SURPRISE") != string::npos) {
        res = true;
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsSurprise(" << res << ")" << endl;
    #endif

    return res;
}

string RemoveTag(const string & source,const string & target,int num_fields) {
    // Remove the `target` tag from the `source` string and return a new string

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] RemoveTag(source:\"" << source << "\";target:\"" << target << "\";num_fields:" << num_fields << ")" << endl;
    #endif

    int A = 0, start = 0, end_tag = 0, tag1 = 0, tag2 = 0;
    string res = "";

    start = source.find(target);
    if(start != string::npos) {
        end_tag = source.find(" ",start + 1);
        if(end_tag == string::npos) {
            end_tag = source.length();
        }
        // If the tag has at least 1 field, search for the next " " character
        if(num_fields > 0) {
            end_tag = source.find(" ",end_tag + 1);
            if(end_tag == string::npos) {
                end_tag = source.length();
            }
        }
        // If the tag has at least 2 fields, search for the next " " character again
        if(num_fields > 1) {
            end_tag = source.find(" ",end_tag + 1);
            if(end_tag == string::npos) {
                end_tag = source.length();
            }
        }

        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] RemoveTag(start:" << start << ";end_tag:" << end_tag << ")" << endl;
        #endif
        // Get the "left" portion of the source string
        string left = source.substr(0,start-1);
        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] RemoveTag(left:\"" << left << "\")" << endl;
        #endif
        // Get the "right" portion of the source string
        string right = "";
        if(end_tag < source.length()) {
            right = source.substr(end_tag);
            #ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] RemoveTag(right:\"" << right << "\")" << endl;
            #endif
        }
        // Splice left and right together
        res = left + right;
        
        // Fix weapon tags that have lost their closing bracket because of inserted spaces
        // we have to count the number of opening and closing brackets
        // Don't do the check if the first character is NOT '['
        if(res.at(0) == '[') {
            for(int i = 0;i < res.length();i++) {
                // Check if the character is a '['
                if(res.at(i) == '[') {
                    tag1++;
                }
                // Check if the character is a ']'
                else if(res.at(i) == ']') {
                    tag2++;
                }
            }
            if(tag1 > tag2) {
                res = res + "]";
            }
        }
    }
    else {
        // The target tag does not exist, just return the original source string
        res = source;
    }

    #ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] RemoveTag(res:\"" << res << "\")" << endl;
    #endif

    return res;
}

void writeTempFiles() {
    long old_AttShipsLeft = 0;
    long old_DefShipsLeft = 0;

    old_AttShipsLeft = BE::AttShipsLeft; // TODO: Move to variable declaration.
    old_DefShipsLeft = BE::DefShipsLeft; // TODO: Move to variable declaration.

    #ifdef CBE_DEBUG
    CBE::debugFile << "############################## WRITE TEMP FILES ##############################" << endl;
    CBE::debugFile << "[INFO] CombatRound: " << BE::CombatRound << "; Attackers: " << old_AttShipsLeft << "; Defenders: " << old_DefShipsLeft << endl;
    #endif

    // Write the Working and Temp files

    // Find Dead and Fled Ships
    ofstream attFledFile;
    attFledFile.open("fled_att.csv",ios::out | ios::binary | ios::app);
    #ifdef CBE_DEBUG
    CBE::debugFile << "##### PROCESSING FLED ATTACKERS " << BE::AttShipsLeft << " #####" << endl;
    #endif
    for(int i = 0;i < old_AttShipsLeft;i++) {
        // IF the current ships hull is less than 1 OR (the current ship is a missile AND the combat round greater than 0)
        // This clears out dead ships and missiles from the ships list/array
        if(BE::CurHullA[i] < 1 || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0)) {
            BE::AttShipsLeft = BE::AttShipsLeft - 1;
            // TODO: Add writing to a att_dead.csv file
            #ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] DEAD/MISSILE: " << BE::AttShipStr[i] << endl;
            #endif
        }
        // ELSE non-dead ships that are not missiles
        else {
            // IF NOT a captured unit AND NOT a crippled unit
            if(!IsCaptured(BE::SpecialA[i]) && !IsCrippled(BE::SpecialA[i])) {
                // IF the unit has fled
                // Units that have been marked as fled but are captured or cripple never get considered fled.
                if(IsFled(BE::SpecialA[i])) {
                    BE::AttShipsLeft = BE::AttShipsLeft - 1;
                    attFledFile << BE::AttShipStr[i] << "," 
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
            else {
                #ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] CAPTURE/CRIPPLE: " << BE::AttShipStr[i] << endl;
                #endif
            }
        }
    }
    // Close the fled_att.csv file here
    attFledFile.flush();
    attFledFile.close();

    // Open TempAFile here
    ofstream tempAFile;
    tempAFile.open(BE::TempAFile,ios::out | ios::binary | ios::trunc);
    #ifdef CBE_DEBUG
    CBE::debugFile << "##### PROCESSING OTHER ATTACKERS " << BE::AttShipsLeft << " #####" << endl;
    #endif
    // Write the header to the fleet file here (TempAFile)
    tempAFile << BE::AttRaceName << "," << BE::AttFleetName << "," << BE::AttBreakOff << "," << BE::AttShipsTotal << "," << BE::AttFleetStrength << "," << BE::AttShipsLeft << "," << BE::AttTargetBonus << "," << BE::AttTargetPriority << "," << BE::AttReserve << "\n";
    if(BE::AttShipsLeft > 0) {
        for(int i = 0;i < old_AttShipsLeft;i++) {
            #ifdef CBE_DEBUG
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
            #endif
            // IF the current ships hull is less than 1 OR the ship is fled OR (the ships is a missile AND the combat round is greater than 0)
            if(BE::CurHullA[i] < 1 || IsFled(BE::SpecialA[i]) || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0)) {
                // Do nothing as this ship is either dead, fled, or a missile
                #ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] DEAD/FLED/MSL: " << BE::AttShipStr[i] << endl;
                #endif
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
    defFledFile.open("fled_def.csv",ios::out | ios::binary | ios::app);
    #ifdef CBE_DEBUG
    CBE::debugFile << "##### PROCESSING FLED DEFENDERS " << BE::DefShipsLeft << " #####" << endl;
    #endif
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
                    defFledFile << BE::DefShipStr[i] << "," 
                                << BE::MaxBeamB[i] << "," 
                                << BE::CurBeamB[i] << "," 
                                << BE::MaxShieldB[i] << "," 
                                << BE::CurShieldB[i] << "," 
                                << BE::MaxTorpB[i] << "," 
                                << BE::CurTorpB[i] << "," 
                                << BE::MaxHullB[i] << "," 
                                << BE::CurHullB[i] << "," 
                                << BE::CurDamB[i] << "," 
                                << BE::StatusB[i] << "," 
                                << BE::AmmoB[i] << "," 
                                << BE::SpecialB[i] << "\n";
                }
            }
            else {
                #ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] CAPTURE/CRIPPLE: " << BE::DefShipStr[i] << endl;
                #endif
            }
        }
    }
    // Close the fled_def.csv file here
    defFledFile.flush();
    defFledFile.close();

    // Open TempAFile here
    ofstream tempBFile;
    tempBFile.open(BE::TempBFile,ios::out | ios::binary | ios::trunc);
     #ifdef CBE_DEBUG
    CBE::debugFile << "##### PROCESSING OTHER DEFENDERS " << BE::DefShipsLeft << " #####" << endl;
    #endif
    // Write the header to the fleet file here (TempAFile)
    tempBFile << BE::DefRaceName << "," << BE::DefFleetName << "," << BE::DefBreakOff << "," << BE::DefShipsTotal << "," << BE::DefFleetStrength << "," << BE::DefShipsLeft << "," << BE::DefTargetBonus << "," << BE::DefTargetPriority << "," << BE::DefReserve << "\n";
    if(BE::DefShipsLeft > 0) {
        for(int i = 0;i < old_DefShipsLeft;i++) {
             #ifdef CBE_DEBUG
            CBE::debugFile << BE::DefShipStr[i] << "," 
                          << BE::MaxBeamB[i] << "," 
                          << BE::CurBeamB[i] << "," 
                          << BE::MaxShieldB[i] << "," 
                          << BE::CurShieldB[i] << "," 
                          << BE::MaxTorpB[i] << "," 
                          << BE::CurTorpB[i] << "," 
                          << BE::MaxHullB[i] << "," 
                          << BE::CurHullB[i] << "," 
                          << BE::CurDamB[i] << "," 
                          << BE::StatusB[i] << "," 
                          << BE::AmmoB[i] << "," 
                          << BE::SpecialB[i] << "\n";
            #endif
            // IF the current ships hull is less than 1 OR the ship is fled OR (the ships is a missile AND the combat round is greater than 0)
            if(BE::CurHullB[i] < 1 || IsFled(BE::SpecialB[i]) || (IsMissile(BE::SpecialB[i]) && BE::CombatRound > 0)) {
                // Do nothing as this ship is either dead, fled, or a missile
                #ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] DEAD/FLED/MSL: " << BE::DefShipStr[i] << endl;
                #endif
            }
            else {
                // Write this ship to the TempAFile csv
                tempBFile << BE::DefShipStr[i] << "," 
                          << BE::MaxBeamB[i] << "," 
                          << BE::CurBeamB[i] << "," 
                          << BE::MaxShieldB[i] << "," 
                          << BE::CurShieldB[i] << "," 
                          << BE::MaxTorpB[i] << "," 
                          << BE::CurTorpB[i] << "," 
                          << BE::MaxHullB[i] << "," 
                          << BE::CurHullB[i] << "," 
                          << BE::CurDamB[i] << "," 
                          << BE::StatusB[i] << "," 
                          << BE::AmmoB[i] << "," 
                          << BE::SpecialB[i] << "\n";
            }
        }
    }
    tempBFile.flush();
    tempBFile.close();
}

void readTempA() {
    // Open tempA file for reading
    ifstream tempA;
    #ifdef CBE_DEBUG
        cout << "Opening " << BE::TempAFile << " as tempA" << endl;
        CBE::debugFile << "[INFO] Opening " << BE::TempAFile << " as tempA" << endl;
    #endif
    tempA.open(BE::TempAFile,ios::binary | ios::in);
    // Read in the fleet header line
    if(tempA.is_open()) {
        // Read the first line
        string header = "";
        getline(tempA,header,'\n');
        BE::FleetInfo info = parseFleetHeader(header);
        #ifdef CBE_DEBUG
        cout << info.RaceName << " - " << info.FleetName << endl;
        CBE::debugFile << info.RaceName << " - " << info.FleetName << endl;
        #endif
        BE::AttRaceName = info.RaceName;
        BE::AttFleetName = info.FleetName;

        // Read the unit lines
        long numUnits = 0;
        string line = "";
        while(getline(tempA,line,'\n')) {
            // Now break it back out to the individual arrays...because...BASIC
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

            #ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Loaded tempA unit: " << unit.UnitName << ","
                           << unit.MaxBeam << ","
                           << unit.CurBeam << ","
                           << unit.MaxShield << ","
                           << unit.CurShield << ","
                           << unit.MaxTorp << ","
                           << unit.CurTorp << ","
                           << unit.MaxHull << ","
                           << unit.CurHull << ","
                           << unit.CurDam << ","
                           << unit.Status << ","
                           << unit.Ammo << ","
                           << unit.Special << endl;
            #endif

            numUnits++;
        }

        BE::AttShipsLeft = numUnits;

        #ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] Attacking Units Loaded: " << BE::AttShipsLeft << endl;
        #endif

        // Close the file
        tempA.close();
    }
    else {
        cout << "Unable to open: " << BE::TempAFile << endl;
    }
}

void readTempB() {
    // Open tempA file for reading
    ifstream tempB;
    #ifdef CBE_DEBUG
        cout << "Opening " << BE::TempBFile << " as tempB" << endl;
        CBE::debugFile << "Opening " << BE::TempBFile << " as tempB" << endl;
    #endif
    tempB.open(BE::TempBFile,ios::binary | ios::in);
    // Read in the fleet header line
    if(tempB.is_open()) {
        // Read the first line
        string header = "";
        getline(tempB,header,'\n');
        BE::FleetInfo info = parseFleetHeader(header);
        #ifdef CBE_DEBUG
        cout << info.RaceName << " - " << info.FleetName << endl;
        CBE::debugFile << info.RaceName << " - " << info.FleetName << endl;
        #endif
        BE::DefRaceName = info.RaceName;
        BE::DefFleetName = info.FleetName;

        // Read the unit lines
        long numUnits = 0;
        string line = "";
        while(getline(tempB,line,'\n')) {
            // Now break it back out to the individual arrays...because...BASIC
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

            #ifdef CBE_DEBUG
            CBE::debugFile << unit.UnitName << ","
                           << unit.MaxBeam << ","
                           << unit.CurBeam << ","
                           << unit.MaxShield << ","
                           << unit.CurShield << ","
                           << unit.MaxTorp << ","
                           << unit.CurTorp << ","
                           << unit.MaxHull << ","
                           << unit.CurHull << ","
                           << unit.CurDam << ","
                           << unit.Status << ","
                           << unit.Ammo << ","
                           << unit.Special << endl;
            #endif

            numUnits++;
        }

        BE::DefShipsLeft = numUnits;

        #ifdef CBE_DEBUG
        CBE::debugFile << "Defending Units Loaded: " << BE::DefShipsLeft << endl;
        #endif

        // Close the file
        tempB.close();
    }
    else {
        cout << "Unable to open: " << BE::TempBFile << endl;
    }
}

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
    //BE::AttShipsLeft = 0;
    //BE::DefShipsLeft = 0;
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

    // Open the battle_report and damage_report files here for output
    ofstream reportFile("battle_r.txt",ios::out | ios::trunc | ios::binary); // This is #3 in basic
    ofstream damageFile("damage_r.txt",ios::out | ios::trunc | ios::binary); // This is #4 in basic

    // TODO: Do file checks to see if they opened

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
        
        // Battle finally commences!
        cout << "Battle Round: " << BE::CombatRound << endl;
        reportFile << "Battle Round: " << BE::CombatRound << "\n";

        // Write the attackers to the report file
        reportFile << "Attackers are the " << BE::AttRaceName << ", " << BE::AttFleetName << " " << BE::GroupName << ".\n";
        reportFile << "Current group Break-off level is " << BE::AttBreakOff << "%\n";
        reportFile << "The " << BE::UnitName << " are currently listed as:\n";

        for(int x = 0;x < BE::AttShipsLeft;x++) {
            reportFile << BE::AttShipStr[x] << " Bm=" << BE::CurBeamA[x] << " Sh=" << BE::CurShieldA[x] << " Tp=" << BE::CurTorpA[x] << " Hl=" << BE::CurHullA[x] << " \"" << BE::SpecialA[x] << "\"\n";
        }
        

        // Write the defenders to the report file
        reportFile << "Defenders are the " << BE::DefRaceName << ", " << BE::DefFleetName << " " << BE::GroupName << ".\n";
        reportFile << "Current group Break-off level is " << BE::DefBreakOff << "%\n";
        reportFile << "The " << BE::UnitName << " are currently listed as:\n";

        for(int x = 0;x < BE::DefShipsLeft;x++) {
            reportFile << BE::DefShipStr[x] << " Bm=" << BE::CurBeamB[x] << " Sh=" << BE::CurShieldB[x] << " Tp=" << BE::CurTorpB[x] << " Hl=" << BE::CurHullB[x] << " \"" << BE::SpecialB[x] << "\"\n";
        }

        // Ok, battle actually commences NOW
        reportFile << "\nBattle Results Commence:\n";

        // Clear the old damage array values
        // TODO: Change this to max array size not just ships left
        for(int a = 0;a < BE::AttShipsLeft;a++) {
            BE::HitsA[a] = 0; BE::PenHitsA[a] = 0; BE::BPAttackCritA[a] = 0;
        }
        // TODO: Change this to max array size not just ships left
        for(int b = 0;b < BE::DefShipsLeft;b++) {
            BE::HitsB[b] = 0; BE::PenHitsB[b] = 0; BE::BPAttackCritB[b] = 0;
        }

        // Cloak and missiles affect the battle order
        // Fighters, suprise and reserve affect individual ships

        BE::AttIsCloaked = 0;
        BE::AttIsMixed = 0;
        BE::AttHasLongRange = 0;
        BE::AttHasFighters = 0;

        BE::DefIsCloaked = 0;
        BE::DefIsMixed = 0;
        BE::DefHasLongRange = 0;
        BE::DefHasFighters = 0;

        if(BE::CombatRound == 1) {
            // Several special things can happen in turn one.
            // cloaked ships can get a first strike
            // long range weapons can get a first strike if the targets are not cloaked
            // And, we need to determine if FLAK equiped ships have targets

            #ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Doing Combat Round 0 Special Checks" << endl;
            #endif

            // Check the attacking fleet
            for(int a = 0;a < BE::AttShipsLeft;a++) {
                // Count the number of units that are cloaked
                if(IsCloak(BE::SpecialA[a])) {
                    BE::AttIsCloaked = BE::AttIsCloaked + 1;
                }
                // Count if the unit has a long range tag
                // TODO: Add a check for DefHasLongRange so that once it is true the checking can stop
                if(HasLong(BE::SpecialA[a])) {
                    BE::AttHasLongRange = true;
                }
                // This checks all weapon tags since we pass it all weapon tags
                if(HasLongWT(BE::SpecialA[a])) {
                    BE::AttHasLongRange = true;
                }
                // Check if a fighter and not in reserve
                // TODO: Add a check for DefHasFighters to stop the checking early
                if(IsFighter(BE::SpecialA[a]) && HasReserve(BE::SpecialA[a]) < 0) {
                    BE::AttHasFighters = 1;
                }
            }

            // Determine if all the attackers are cloaked, if none are, or if some are cloked.
            // TODO: Seperate AttIsCloaked from the counting above
            if(BE::AttIsCloaked > 0) {
                if(BE::AttIsCloaked == BE::AttShipsLeft) {
                    BE::AttIsCloaked = 1;
                }
                else {
                    BE::AttIsCloaked = 0;
                    BE::AttIsMixed = 1;
                }
            }

            // Check the defending fleet
            for(int a = 0;a < BE::DefShipsLeft;a++) {
                // Count the numbe of units that are cloaked
                if(IsCloak(BE::SpecialB[a])) {
                    BE::DefIsCloaked = BE::DefIsCloaked + 1;
                }
                // Count if the unit has a long rangew tag
                // TODO: Add a check for DefHasLongRange so that once it is true the checking can stop
                if(HasLong(BE::SpecialB[a])) {
                    BE::DefHasLongRange = true;
                }
                // This checks all weapon tags since we pass it all weapon tags
                if(HasLongWT(BE::SpecialB[a])) {
                    BE::DefHasLongRange = true;
                }
                // Check if a fighter and not in reserve
                // TODO: Add a check for DefHasFighters to stop the checking early
                if(IsFighter(BE::SpecialB[a]) && HasReserve(BE::SpecialB[a]) < 0) {
                    BE::DefHasFighters = 1;
                }
            }

            // Determine if all the defenders are cloaked, if none are, or if some are cloked.
            // TODO: Seperate DefIsCloaked from the counting above
            if(BE::DefIsCloaked > 0) {
                if(BE::DefIsCloaked == BE::DefShipsLeft) {
                    BE::DefIsCloaked = 1;
                }
                else {
                    BE::DefIsCloaked = 0;
                    BE::DefIsMixed = 1;
                }
            }
        }
        else {
            // Remove RESERVE tags from delayed units
            for(int a = 0;a < BE::AttShipsLeft;a++) {
                // TODO: Split Has* and Get* functions so that boolean returns are separated from other types.
                // TODO: Generalize HasTag and GetTag functions?
                int delay = HasDelay(BE::SpecialA[a]);
                if(HasReserve(BE::SpecialA[a]) && delay > 0) {
                    if(delay < BE::CombatRound) {
                        BE::SpecialA[a] = RemoveTag(BE::SpecialA[a],"RESERVE",1);
                    }
                }
            }

            // Remove RESERVE tags from delayed units
            for(int b = 0;b < BE::DefShipsLeft;b++) {
                int delay = HasDelay(BE::SpecialB[b]);
                if(HasReserve(BE::SpecialB[b]) && delay > 0) {
                    if(delay < BE::CombatRound) {
                        BE::SpecialB[b] = RemoveTag(BE::SpecialB[b],"RESERVE",1);
                    }
                }
            }

            // Check attackers and defenders fleets for fighters
            // TODO: Need to turn this into a function: CheckFighters(special[],shipsLeft)
            for(int a = 0;a < BE::AttShipsLeft;a++) {
                if(IsFighter(BE::SpecialA[a]) && !HasReserve(BE::SpecialA[a])) {
                    BE::AttHasFighters = 1;
                    break; // Leave the loop early as we have found at least 1 fighter not in reserve.
                }
            }
            for(int b = 0;b < BE::DefShipsLeft;b++) {
                if(IsFighter(BE::SpecialB[b]) && !HasReserve(BE::SpecialB[b])) {
                    BE::DefHasFighters = 1;
                    break; // Leave the loop early as we have found at least 1 fighter not in reserve.
                }
            }
        }

        // Check for attacker reserve & screen
        AttHasScreen = false; // TODO: Move to BE namespace
        AttHasReserveUnits = false; // TODO: Move to BE namespace
        for(int a = 0;a < BE::AttShipsLeft;a++) {
            // TODO: What kind of mechanic should there be for not enough screen?v
            // The unit is either in the reserve or is part of the screen
            if(HasReserve(BE::SpecialA[a])) {
                AttHasReserveUnits = true;
            }
            else {
                AttHasScreen = true;
            }
            // Check if both there is both a screen and a reserve already.
            if(AttHasScreen && AttHasReserveUnits) {
                // Leave the loop early as we found a unit in the screen and a unit in the reserve.
                break;
            }
        }

        // Check for the defender reserve & screen
        // TODO: Turn this into a function: CheckFleetReserveAndScreen(special[],shipsLeft)
        DefHasScreen = false; // TODO: Move to BE namespace
        DefHasReserveUnits = false; // TODO: Move to BE namespace
        for(int b = 0;b < BE::DefShipsLeft;b++) {
            // The unit is either in the reserve or is part of the screen
            if(HasReserve(BE::SpecialB[b])) {
                DefHasReserveUnits = true;
            }
            else {
                DefHasScreen = true;
            }
            // Check if both there is both a screen and a reserve already.
            if(DefHasScreen && DefHasReserveUnits) {
                // Leave the loop early as we found a unit in the screen and a unit in the reserve.
                break;
            }
        }

        // If there is no screen, the reserve automatically fails
        // NOTE: if Reserve > BreakOff the reserves will NEVER engage.  Handy when you're trying to protect
        // something like a badly damage capitol ship or freighters and the GM says they're vulnerable to attack.
        // TODO: Turn this into a function:  BreakOffAndScreenCheck(special[],shipsLeft,hasScreen,hasReserve)
        // TODO:  I should be able to handle this with the earlier checks?  Maybe...
        // Check to see if the attackers have a reserve.
        if(AttHasReserveUnits) {
            // Check those reserve units for BreakOff and for screen
            for(int a = 0;a < BE::AttShipsLeft;a++) {
                // Get the reserve tag value of the ship
                int reserve = HasReserve(BE::SpecialA[a]);
                // Do the attackers have a screen?
                if(!AttHasScreen) {
                    // Does the ship have a reserve value?
                    if(reserve >= 0) {
                        // Remove the reserve tag
                        BE::SpecialA[a] = RemoveTag(BE::SpecialA[a],"RESERVE",1);
                        AttHasReserveUnits = false;
                    }
                }
                else {
                    // Is the reserve tag equal to or less than the attackers BreakOff?
                    if(BE::BO_Att >= reserve && reserve >= 0) {
                        // Remove the reserve tag
                        BE::SpecialA[a] = RemoveTag(BE::SpecialA[a],"RESERVE",1);
                    }
                }
            }
        }

        if(DefHasReserveUnits) {
            // Check those reserve units for BreakOff and for screen
            for(int b = 0;b < BE::AttShipsLeft;b++) {
                // Get the reserve tag from the ship
                int reserve = HasReserve(BE::SpecialB[b]);
                // Do the defenders have a screen?
                if(!DefHasScreen) {
                    // Does the ship have a reserve value?
                    if(reserve >= 0) {
                        // Remove the reserve tag
                        BE::SpecialB[b] = RemoveTag(BE::SpecialB[b],"RESERVE",1);
                    }
                }
                else {
                    // Is the reserve tag equal to or less than the defenders BreakOff?
                    if(BE::BO_Def >= reserve && reserve >= 0) {
                        // Remove the reserve tag
                        BE::SpecialB[b] = RemoveTag(BE::SpecialB[b],"RESERVE",1);
                    }
                }
            }
        }

        // UNKNOWN: Resetting AttackIndexes?
        if(BE::AttacksIndex > 0) {
            for(int i = 0;i < BE::AttacksIndex;i++) {
                BE::Attacks[i].AttackID = 0;
                BE::Attacks[i].TargetID = 0;
                BE::Attacks[i].Damage = 0;
                BE::Attacks[i].Weapon = 0;
                BE::Attacks[i].Special = "";
            }
        }

        // Spawn missiles routine
        TempAttShipsLeft = BE::AttShipsLeft;
        TempDefShipsLeft = BE::DefShipsLeft;
        missile_counter = 0;
        // Loop through a combined total of all remaining attacking and defending ships
        // This is horribley convoluted
        // TODO: make a funciton for this...not sure what it needs to look like
        for(int A = 0;A < (BE::AttShipsLeft + BE::DefShipsLeft);A++) {
            // Determine ForceID
            // 0 => attackers
            // 1 => defenders
            int B = A;
            ForceID = 0; // TODO: Make this a local scope variable
            // Are we done with attackers?
            if(A > BE::AttShipsLeft) {
                B = A - BE::AttShipsLeft;
                ForceID = 1; // Defenders
            }

            // Get unit values depending on ForceID
            if(ForceID == 0) {
                temp_str = BE::SpecialA[B]; // TODO: Make this local to the loop scope.  May not be necessary if a function is build.
                tmp = BE::CurTorpA[B]; // TODO: Make this local to the loop scope.  May not be necessary if a function is build.
            }
            else {
                temp_str = BE::SpecialB[B];
                tmp = BE::CurTorpB[B];
            }

            // Check if the unit is crippled or is suprised.  If either is true, then skip the unit
            if(IsCrippled(temp_str) || IsSurprise(temp_str)) {
                continue;
            }

            // Check for batteries in the special string
            if(HasBatteries(temp_str) > 0) {
                // Check if any of the batteries has a `misXXXX` tag
                if(!HasMissileWT(temp_str)) {
                    // Skip the unit since there is no missile tag in a bracket
                    continue;
                }

                tmp = 0; // TODO: Make this a local variable
                int SalvoCount = 0; // FIXME: This might not be local?
                int sc = 0; // TODO: Make this local to the loop below
                // Reset the salvo array
                for(sc = 0;sc < 200;sc++) {
                    BE::Salvos[sc].MissileS = 0;
                    BE::Salvos[sc].DataStr = "";
                }

                // Get the first openning weapons bracket
                old_start = temp_str.find("[");
                sc = 0; // TODO: Not sure what to do with this yet.
                // Get the brackets for this unit
                vector<string> brackets = GetBrackets(temp_str);
                for(int i = 0;i < brackets.size();i++) {
                    // Setup the salvo object
                    BE::Salvos[sc].DataStr = brackets[i];
                    BE::Salvos[sc].MissileS = 0;

                    // Does the bracket have a salvo tag?
                    if(HasMissileWT(BE::Salvos[sc].DataStr)) {
                        // Is the unit in reserve and without an artillery tag?
                        if(HasReserve(temp_str) && HasArtilleryWT(BE::Salvos[sc].DataStr)) {
                            // This unit is in reserve and doesn't have an artillery tag
                        }
                        else {
                            // This unit is either NOT in the reserve or HAS an artillery tag
                            // Do the long range checks.
                            if((BE::AttHasLongRange || BE::DefHasLongRange) && !HasLongWT(BE::Salvos[sc].DataStr)) {
                                // We are at long range BUT this unit does NOT have a long tag.
                                // Do nothing.
                            }
                            else {
                                // Either we are at standard range or this unit has a long tag.
                                // Does the unit have ammo?
                                int ammo = HasAmmoWT(BE::Salvos[sc].DataStr);
                                if(ammo > CBE::AMMO_EMPTY) {
                                    // The bracket has ammo OR doesn't use ammo
                                    // ORIGINAL is VAL(MID$(Salvo(sc).DataStr,2)) which is the numeric value of the substring from position 2 to end of string...I think
                                    // VAL(MID$("[2 mis0041 ammo 1 target 15]")) ==> VAL("2 mis0041 ammo 1 target 15]") ==> 2 (Wow...I really dislike QBASIC)
                                    // Essentially it grabs the number at the beginning of the string.  VAL might only return the first number it finds?
                                    // So, we get to do this the hard way for now because I'm not going to learn C++ regex right now.  See TODO below.
                                    // TODO: Convert this mess to a regex extraction of the first number in the string
                                    int start = BE::Salvos[sc].DataStr.find("[");
                                    // If start is npos then there is no "[" in the string.  So the first item in the string should be the size of the salvo.
                                    if(start == string::npos) {
                                        start = 0;
                                    }
                                    else {
                                        start++;
                                    }
                                    string sizeStr = BE::Salvos[sc].DataStr.substr(start); // Grab everything past the openning bracket or the start of string
                                    int size = stoi(sizeStr); // This will only grab the first number in the string and it must start with a string
                                    #ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Salvo Size: " << size << endl;
                                    #endif
                                    BE::Salvos[sc].MissileS = size; // This is simultaneously the number of missiles to launch and the volley size for non-missile salvos
                                }
                                else if(ammo == CBE::AMMO_INFINITE) {
                                    // The bracket doesn't need ammo
                                    // Do the same things as above....which is a mess
                                    // TODO: Redo logic to collapse these two paths
                                    int start = BE::Salvos[sc].DataStr.find("[");
                                    // If start is npos then there is no "[" in the string.  So the first item in the string should be the size of the salvo.
                                    if(start == string::npos) {
                                        start = 0;
                                    }
                                    else {
                                        start++;
                                    }
                                    string sizeStr = BE::Salvos[sc].DataStr.substr(start); // Grab everything past the openning bracket or the start of string
                                    int size = stoi(sizeStr); // This will only grab the first number in the string and it must start with a string
                                    #ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Salvo Size: " << size << endl;
                                    #endif
                                    BE::Salvos[sc].MissileS = size; // This is simultaneously the number of missiles to launch and the volley size for non-missile salvos
                                }
                            }
                        }
                    }

                    // Increment the salvo count
                    sc = sc + 1;
                }

                // Loop through the salvo count and spawn a missile 'unit' for each entity
                SalvoCount = sc;
                // TODO: Clean up the reused sc variable.  Poor thing, mistreated like that.
                for(sc = 0;sc < SalvoCount;sc++) {
                    // Does the salvo have a size greater than 0?  TODO: Move this check to the loop above?  Might mess with the [0] tags on units that shouldn't flee.  Maybe BRAVE tag.
                    int size = BE::Salvos[sc].MissileS;
                    if(size > 0) {
                        // The salvo has a size.  Do all the things!!!
                        // But first, check to make sure the salvo size won't overflow the 9999 max unit limit
                        // TODO: Setup a preprocessor variable for maximum number of units
                        if(size + TempAttShipsLeft + TempDefShipsLeft > 9999) {
                            // There will be a problem.  We should throw an exception here rather than just exiting
                            // TODO: Throw an exception
                            cerr << "The 9999 ship limit has been exceeded while spawning missiles.  Simulation aborted." << endl;
                            exit(1); // TODO: Define error codes...and more constants for consistancy and clarity.
                        }

                        // Check for a missile tag and get the BSTH values from the tag
                        // TODO: Add a flags member to the SalvoInfo struct so these tests don't need to be constantly done over and over and over and over again...
                        // This also sets BE::MissileB, BE::MissileS, BE::MissileT, and BE:MissileH
                        const string & special = BE::Salvos[sc].DataStr; // I got tired of accessing this variable in every function call....
                        HasMissileWT(special); // FIXME: I can't describe how much I hate this line.
                        // Check to see if the salvo need ammo.  See todo above...damn.
                        int ammo = HasAmmoWT(special);
                        if(ammo > CBE::AMMO_EMPTY) {
                            NewTag = "ammo " + (ammo - 1); // Decrement the ammo counter.  TODO: Make this a local variable
                            NewTag += " "; // Had to make this a new line do to type converstion of the (ammo -1) expression.
                            temp_str = RemoveTag(BE::Salvos[sc].DataStr,"ammo",1); // FIXME: This is so clunky to update the string in the middle of the round
                            temp_str = AddTag(BE::Salvos[sc].DataStr,NewTag);
                            BE::Salvos[sc].DataStr = temp_str; // FIXME: And this breaks my pointer above?  Maybe not.  We'll see!!!
                            // Remove the ammo string from missiles so there are no 'duds'
                            missile_str = RemoveTag(BE::Salvos[sc].DataStr,"ammo",1); // Remove the ammo tag and save as missile_str.  TODO: Local?  I'm not sure where this is used next.
                            missile_str = RemoveTag(missile_str,"shots",1); // Remove the shots tag because redundancy
                        }
                        else if(ammo == CBE::AMMO_INFINITE) {
                            missile_str = BE::Salvos[sc].DataStr; // Just use the salve string since there is no ammo
                        }
                        
                        // Spawn those babies...
                        for(int i = 0;i < size;i++) {
                            missile_counter = missile_counter + 1; // This is a global variable that was zeroed at the start of this section.  This tracks all missiles launched from both sides?
                            // Check the ForceID...TODO: Get rid of ForceID so that multiple fleets can be added.  Not to mention factions
                            if(ForceID == 0) {
                                // VB sucks.  Where do MissileB, MissileS, MissileT, and Missile H come from?  From the call to HasMissileWT() about 20 lines up!
                                // [JLL] This block of code makes me want to vomit.  No wonder tags jump between units.  Argh!!
                                TempAttShipsLeft = TempAttShipsLeft + 1;
                                BE::AttShipStr[TempAttShipsLeft] = "missile " + missile_counter; // Global missile names too...
                                BE::MaxBeamA[TempAttShipsLeft] = BE::MissileB;
                                BE::CurBeamA[TempAttShipsLeft] = BE::MissileB;
                                BE::MaxShieldA[TempAttShipsLeft] = BE::MissileS;
                                BE::CurShieldA[TempAttShipsLeft] = BE::MissileS;
                                BE::MaxTorpA[TempAttShipsLeft] = BE::MissileT;
                                BE::CurTorpA[TempAttShipsLeft] = BE::MissileT;
                                BE::MaxHullA[TempAttShipsLeft] = BE::MissileH;
                                BE::CurHullA[TempAttShipsLeft] = BE::MissileH;
                                // Inherit tags from the launcher and convert to a missile
                                BE::SpecialA[TempAttShipsLeft] = missile_str; // Use the missile string the the salvos array ~20-25 lines up.
                                // Change the beginning number to match the missile strength
                                // [JLL] This converts the B and T portions of the missile to a single salvo so that it is processed correctly.
                                // [JLL] For example, [4 mis0021] becomes 4 units that have [2] as their single salvo bracket
                                /* [JLL] The original code looks for the first space in the special string.
                                        Then creates a salvo bracket and appends the original special string from the " " onward.
                                        This looses information from the missile_str...maybe the salvo size?
                                    1) Remove the "[\d+" from the missile special
                                    2) Add a "[" + (Beam+Torp) to the missile special
                                    3) Remove the "mis" tag from the missile special
                                    4) Add "MSL" to denote the unit is a missile
                                    5) Add "SUICIDE" to denote the unit should be destroyed at end of turn
                                */
                                int start = BE::SpecialA[TempAttShipsLeft].find(" ");
                                BE::SpecialA[TempAttShipsLeft] = "[" + to_string(BE::MissileB + BE::MissileT) + BE::SpecialA[TempAttShipsLeft].substr(start); // Combine Beam and Torp rating for full salvo strength.  FIXME: This assumes that there is a closing ']'
                                BE::SpecialA[TempAttShipsLeft] = RemoveTag(BE::SpecialA[TempAttShipsLeft],"mis",0); // Remove the missile tag or missiles will spawn missiles.  Could be used for submunitions...
                                BE::SpecialA[TempAttShipsLeft] = AddTag(BE::SpecialA[TempAttShipsLeft],"MSL"); // Denotes the unit as a missile
                                BE::SpecialA[TempAttShipsLeft] = AddTag(BE::SpecialA[TempAttShipsLeft],"SUICIDE"); // Suicide denotes that the unit should be removed at end of combat
                                // [JLL] TODO: Remove all of the above and use bitwise flags
                            }
                            else {
                                // Do the other foce
                                // [JLL] TODO: Make this a function
                                TempDefShipsLeft = TempDefShipsLeft + 1;
                                BE::DefShipStr[TempDefShipsLeft] = "missile " + missile_counter; // Global missile names too...
                                BE::MaxBeamB[TempDefShipsLeft] = BE::MissileB;
                                BE::CurBeamB[TempDefShipsLeft] = BE::MissileB;
                                BE::MaxShieldB[TempDefShipsLeft] = BE::MissileS;
                                BE::CurShieldB[TempDefShipsLeft] = BE::MissileS;
                                BE::MaxTorpB[TempDefShipsLeft] = BE::MissileT;
                                BE::CurTorpB[TempDefShipsLeft] = BE::MissileT;
                                BE::MaxHullB[TempDefShipsLeft] = BE::MissileH;
                                BE::CurHullB[TempDefShipsLeft] = BE::MissileH;
                                // Inherit tags from the launcher and convert to a missile
                                BE::SpecialB[TempDefShipsLeft] = missile_str; // Use the missile string the the salvos array ~20-25 lines up.
                                int start = BE::SpecialB[TempDefShipsLeft].find(" ");
                                BE::SpecialB[TempDefShipsLeft] = "[" + to_string(BE::MissileB + BE::MissileT) + BE::SpecialB[TempDefShipsLeft].substr(start); // Combine Beam and Torp rating for full salvo strength.  FIXME: This assumes that there is a closing ']'
                                BE::SpecialB[TempDefShipsLeft] = RemoveTag(BE::SpecialB[TempDefShipsLeft],"mis",0); // Remove the missile tag or missiles will spawn missiles.  Could be used for submunitions...
                                BE::SpecialB[TempDefShipsLeft] = AddTag(BE::SpecialB[TempDefShipsLeft],"MSL"); // Denotes the unit as a missile
                                BE::SpecialB[TempDefShipsLeft] = AddTag(BE::SpecialB[TempDefShipsLeft],"SUICIDE"); // Suicide denotes that the unit should be removed at end of combat
                            }
                        }
                    }
                }

                // Reassemble weapon tags back into a unit's appropriate Special tag
                // Get the Specail tag from the special array.  Again with the ForceID junk.  Functions are your friend...
                // [JLL] This is why string manipulation is bad.  A well built class or struct will render a lot of this code useless.
                // Check ForceID
                if(ForceID == 0) {
                    temp_str = BE::SpecialA[B]; // [JLL] TODO: Stop global variable abuse!
                }
                else {
                    temp_str = BE::SpecialB[B];
                }

                Strip = false;
                new_str = "";

                // Not entirely sure what this is for.  Might be for removing the [] from the string?
                // Or does it remove the first salvo from the string?
                // [JLL] This removes all salvos from the string!  Because they will be rebuilt from the Salvos array below.
                // [JLL] This builds a new string for the unit, sans salvos, character by character.  Vomit inducing...
                for(swt = 0;swt < temp_str.size();swt++) {
                    if(temp_str[swt] == '[') {
                        Strip = true;
                    }
                    if(Strip == false) {
                        new_str = new_str + temp_str[swt];
                    }
                    if(temp_str[swt] == ']') {
                        Strip = false;
                    }
                }

                // This builds a new string of Salvos from the Salvos array?  But why?
                // Also, 200 salvos for the entire simulation...that isn't enough for one fleet!
                // See the code below this block.  The salvos array is per unit, I think.
                // [JLL] The Salvos array is per unit.
                // [JLL] FIXME: This is why salvos jump units some times.  This loops through all 200 Salvos.
                for(sc = 0;sc < 200;sc++) {
                    if(BE::Salvos[sc].DataStr == "") {
                        break;
                    }
                    new_str = new_str + BE::Salvos[sc].DataStr;
                }

                // Assign the new specials string to the SpecialA or SpecialB array for the unit in question depending on ForceID
                // [JLL] FIXME: This explains why the salvo string jumps from attackers to defenders and vice versa.
                if(ForceID == 0) {
                    BE::SpecialA[B] = new_str;
                }
                else {
                    BE::SpecialB[B] = new_str;
                }
            } // End of HasBatteries If Block
            else { // This unit has no batteries.
                // [JLL] TODO: Discontinue use of non-battery weapons
                // Check unit torpedo rating and for Missile
                // [JLL] I am skipping this for now pending response from the FOTS group - 2023-01-11
            }
        }// End of spawn missile loop....

        // [JLL] Since missiles are added at the end of the units array the code below keeps other units from targeting them.
        AttNumValidTargets = BE::AttShipsLeft;
        DefNumValidTargets = BE::DefShipsLeft;
        BE::AttShipsLeft = TempAttShipsLeft; // [JLL] Need to keep track of missiles
        BE::DefShipsLeft = TempDefShipsLeft; // [JLL] Need to keep track of missiles

        // Copy real values to temp fields so that combat will be fair
        for(int i = 0;i < BE::AttShipsLeft;i++) {
            BE::TempAttCritStr[i] = BE::AttCritStr[i];
            BE::TempCurBeamA[i] = BE::CurBeamA[i];
            BE::TempCurShieldA[i] = BE::CurShieldA[i];
            BE::TempCurTorpA[i] = BE::CurTorpA[i];
            BE::TempCurHullA[i] = BE::CurHullA[i];
            BE::TempHitsA[i] = BE::HitsA[i];
            BE::TempPenHitsA[i] = BE::HitsA[i];
            BE::TempCurDamA[i] = BE::CurDamA[i];
            BE::TempSpecialA[i] = BE::SpecialA[i];
        }
        for(int i = 0;i < BE::DefShipsLeft;i++) {
            BE::TempDefCritStr[i] = BE::DefCritStr[i];
            BE::TempCurBeamB[i] = BE::CurBeamB[i];
            BE::TempCurShieldB[i] = BE::CurShieldB[i];
            BE::TempCurTorpB[i] = BE::CurTorpB[i];
            BE::TempCurHullB[i] = BE::CurHullB[i];
            BE::TempHitsB[i] = BE::HitsB[i];
            BE::TempPenHitsB[i] = BE::HitsB[i];
            BE::TempCurDamB[i] = BE::CurDamB[i];
            BE::TempSpecialB[i] = BE::SpecialB[i];
        }

        // ------------------------------------------------------------------------------------------
        // Attack Routine
        for(BE::A = 0;BE::A < (BE::AttShipsLeft + BE::DefShipsLeft);BE::A++) {
            // The same BS about ForceID...
            int B = BE::A;
            ForceID = 0;
            if(BE::A > BE::AttShipsLeft) {
                B = BE::A - BE::AttShipsLeft;
                ForceID = 1;
            }
            if(BE::A = BE::AttShipsLeft + 1) {
                // Print a space in the report file
                reportFile << endl;
            }

            Special1 = 0;
            Special2 = 0;
            FirePower1 = 0;
            FirePower2 = 0;
            
            /* 
                OK, here's the problem in a nut shell.  Each ship can have multiple
                attacks.  In the past, the firepower was added up and the damage
                spread out.  But that won't work with the new multi's, Special
                effect weapons, and resistant defenses.  The firepower needs to be
                divided before combat and each shot rolled and applied separately.

                Determine the number of attacks:  Split, Multi, Flak, Special
                Effect weapons that are unsplit are lumped together into a
                single attack.  [ie. A 4 pt heat beam plus a 6 pt pen torp
                becomes a 10 pt heat+pen attack.]  I'll assume that 200
                individual attacks is enough to drive most individual units.
            */

           // Clear the firepower and TempSpecial attributes arrays.
           for(int i = 0;i < 200;i++) {
            Hits[i].firepower = 0;
            Hits[i].special = 0;
            Hits[i].tag = "";
           }
           number_of_attacks = 0; // Assume the attacker has no weapons

           // Determine eligibility and the number of attacks (if any)
           switch(ForceID) {
            case 0: // Attacking fleet
            if(BE::AttShipStr[B].find("missile") != string::npos) {
                // [JLL] Why the hell do we care if the name of the unit has "missile" in it?
                BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " is a drone or decoy."; // Check the name of the ship for missile?  Assume it is a drone or decoy.
            }
            else {
                BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " did not fire beams or torps."; // If missile is not in the name then the unit did not fire?
            }
            // [JLL] I am so confused by the if/else blocks above.  TODO: Can I remove these?

            // Check if the unit has a drifting tag AND NOT having a nomove tag.
            if(IsDrifting(BE::SpecialA[B]) && !IsNoMove(BE::SpecialA[B])) {
                // The unit is drifting and does not have a nomove tag
                // Check if the unit is fleeing
                if(IsFlee(BE::SpecialA[B])) {
                    // The unit is fleeing.  Remove flee and add fled
                    BE::TempSpecialA[B] = RemoveTag(BE::TempSpecialA[B],"FLEE",0);
                }
            }
            break;
            case 1: // Defending fleet
            break;
           }
        }

        // END OF ROUND!!!!
        writeTempFiles();

        //FIXME: Memory cleanup?
        
        reportFile << "\n";
        reportFile.flush();
        damageFile.flush();
    } while (BE::CombatRound < 100/* condition | TODO: replace true with variable.  Perhaps AttackLoop? */);
    
}

/*
* valid command line arguments are:
* -a <string> - the filename for the attacking fleet csv
* -d <string> - the filename for the defending fleet csv
* -g <string> - the group name for this simulation (default is Fleets)
* -u <string> - the unit name for this simulation (default is Ships)
*/
int main(int argc, char *argv[]) {
    #ifdef CBE_DEBUG
    CBE::debugFile << "############################## DEBUG ##############################" << endl;
    #endif
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

    // Close the debug file
    CBE::debugFile.close();

    // Return a clean run
    return 0;
}