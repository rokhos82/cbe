#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <limits>
#include <cmath>
#include <utility>
#include <unordered_map>
#include <regex>

// #include "FleetInfo.h"
#include "flags.h"
#include "structs.h"
#include "cbe_lib.h"
#include "cbe.h"
#include "CriticalHitTable.h"

using namespace std;

bool oneStep = false;

std::unordered_map<int, CriticalHitTable> critTables;

fstream CBE::debugFile = fstream("debug.txt", ios::out | ios::binary);

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
long BE::ValidTargets[9999];

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
string BE::GroupName = "Fleet", BE::UnitName = "Ship";

void loadAttackingFleet(string fname)
{
    std::cout << "Loading attacking fleet from: " << fname << endl;
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] Loading attacking fleet from: " << fname << endl;
#endif
    BE::AttFleetStr = fname;

    fstream attFile;
    attFile.open(BE::AttFleetStr, ios::in | ios::binary);
    if (attFile.is_open())
    {
        string line = "";
        getline(attFile, line, '\n');
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
        while (getline(attFile, line, '\n'))
        {
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

        std::cout << BE::AttShipsLeft << " units loaded." << endl;
        CBE::attackerLoaded = true;
    }
}

void loadDefendingFleet(string fname)
{
    std::cout << "Loading defending fleet from: " << fname << endl;
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] Loading defending fleet from: " << fname << endl;
#endif
    BE::DefFleetStr = fname;

    fstream defFile;
    defFile.open(BE::DefFleetStr, ios::in | ios::binary);
    if (defFile.is_open())
    {
        string line = "";
        getline(defFile, line, '\n');
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
        while (getline(defFile, line, '\n'))
        {
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
        std::cout << BE::DefShipsLeft << " units loaded." << endl;
    }
}

string AddTag(const string &source, const string &target)
{
    // Add the target tag to the source string and return the modified string.
    int start = 0;
    string res = "";

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] AddTag(source:\"" << source << "\",target:\"" << target << "\")" << endl;
#endif

    // Find the target tag in the source string first...not sure what this is doing.  Maybe grouping similar tags?
    // Nope!  See the else block below.  Looks like targets are intended to be singltons.  Multiple OFFLINE tags was never implemented...
    start = source.find(target);
    if (start == string::npos)
    {
        // The target does not exist in the source.
        // Check if the source starts with a bracket
        if (source[0] == '[' && source[source.size() - 1] == ']')
        {
#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] AddTag: This is a weapon bracket" << endl;
#endif
            // Yep, this is a weapon bracket
            // Now check if the target tag is uppercase
            if (isupper(target[0]))
            {
                // The target tag is upper case.  Make sure it goes OUTSIDE any brackets.
                // This leads to an intersting case of being able to add a tag to a salvo string...
                // Test for an ' ' at the end of the string first
                if (source[source.size() - 1] != ' ')
                {
                    // Add the target tag to the end of the source string with a ' '
                    res = source + " " + target;
                }
                else
                {
                    // Just add the target tag to the end of the source string
                    res = source + target;
                }
            }
            else
            {
                // The target tag is lower case.  It can go inside the bracket.
                // We know that the ending character is ']'
                res = source.substr(0, source.size() - 1) + " " + target + "]";
            }
        }
        else
        {
            // Not a bracket so add to the end...working with strings is so dumb.
            // Check for a space and add if necessary.
            if (source[source.size() - 1] != ' ')
            {
                res = source + " " + target;
            }
            else
            {
                res = source + target;
            }
        }
    }
    else
    {
        // The tag already exists.  Just return the original string.
        res = source; // Create a new string   Memory allocation is going to be a mess...
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] AddTag(res:\"" << res << "\")" << endl;
#endif

    return res;
}

long GetBPAttVal(const string &special)
{
    int start = 0;
    long attVal = -1;
    int end = 0;

    start = special.find("BP");
    if (start != string::npos)
    {
        start = special.find(" ", start);
        end = special.find(" ", start + 1);
    }

    return attVal;
}

long GetBPDefVal(const string &special, long hull)
{
    int start = 0;
    int end = 0;
    long defVal = hull;

    start = special.find("BP");
    if (start != string::npos)
    {
        start = special.find(" ", start + 1);
        start = special.find(" ", start + 1);
        end = special.find(" ", start);
        defVal += stol(special.substr(start, end));
    }

    return defVal;
}

vector<string> GetBrackets(const string &special)
{
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetBrackets(\"" << special << "\")" << endl;
#endif
    // Setup the dynamic array for the return
    vector<string> brackets;

    // Is there an openning bracket in special?
    int old_start = special.find("[");
    while (old_start != string::npos)
    {
        // Yes, save that location
        int start = old_start;
        // Get the next closing bracket
        int start1 = special.find("]", start);
        string bracket = special.substr(start, start1 - start + 1);
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] GetBrackets(start:" << start << ";start1:" << start1 << ";bracket:\"" << bracket << "\")" << endl;
#endif
        brackets.push_back(bracket);
        // Get the next openning bracket if it exists
        old_start = special.find("[", start1);
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetBrackets => " << brackets.size() << " brackets" << endl;
#endif

    // Return the bracket strings array
    return brackets;
}

long GetCrit(const std::string &data_str)
{
    // Default to 5 (normal)
    long crit = 5;

    size_t start = data_str.find("CRIT ");
    if (start != std::string::npos)
    {
        // Extract the crit value from the string
        std::string crit_str = data_str.substr(start + 5, 2);
        crit = std::strtol(crit_str.c_str(), nullptr, 10);
    }

    return crit;
}

int GetDatalinkIndex(char label)
{
    int index = -1;

    // Check if the label is A-Z
    if (label >= 65 && label <= 90)
    {
    }

    return index;
}

int GetROFDelayWT(const string &special)
{
    int res = -1;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetROFDelayWT(\"" << special << "\")" << endl;
#endif

    // Check for ROF tag first
    int start = special.find("rof");
    if (start != string::npos)
    {
        // Now get the delay value of the tag
        start = special.find(" ", start + 4);
        int end = special.find(" ", start + 1);
        res = stoi(special.substr(start, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetROFDelayWT(" << res << ")" << endl;
#endif

    return res;
}

int GetROFRateWT(const string &special)
{
    int res = -1;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetROFRateWT(\"" << special << "\")" << endl;
#endif

    // Check for ROF tag first
    int start = special.find("rof");
    if (start != string::npos)
    {
        // Now get the delay value of the tag
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        res = stoi(special.substr(start, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetROFRateWT(" << res << ")" << endl;
#endif

    return res;
}

/*
 * CheckROF()
 */

bool CheckROF(const string &special, const long &round)
{
    long delay = GetROFDelayWT(special);
    bool res = false;

    if (delay <= 0)
    {
        res = true;
    }

    return res;
}

/*
 * =============================================================================
 * GetRandomTarget returns the index for a randomly chosen target.
 * =============================================================================
 */
int GetRandomTarget(int forceId, const string &unitData, int numTargets)
{
    // Setup the target data point
    string *targetsData;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetRandomTarget(forceId:"
                   << forceId
                   << ";unitData:\""
                   << unitData
                   << "\";numTargets:"
                   << numTargets
                   << endl;
#endif

    if (forceId == 0)
    {
        // Set targets to the list of defenders
        targetsData = BE::SpecialB;
    }
    else
    {
        // Set targets to the list of attackers
        targetsData = BE::SpecialA;
    }

    int targetIndex = -1;
    for (int i = 0; i < 20; i++) // TODO: Replace 20 with a constant
    {
        // Generate the random index given the numTargets
        targetIndex = rand() % numTargets;
        string targetData = targetsData[targetIndex];
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] GetRandomTarget: Try "
                       << i
                       << ", checking target "
                       << targetIndex
                       << ": \""
                       << targetData
                       << "\""
                       << endl;
#endif
        if (!IsMissile(targetData) && (HasReserve(targetData) <= 0 || IsGlobal(unitData)))
        {
            // Since this is not a missile and not in reserve unless the hit is global
            // We will take this target
#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] GetRandomTarget: Target found" << endl;
#endif
            break;
        }
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetRandomTarget => " << targetIndex << endl;
#endif

    // Return that random index
    // This will be -1 if no valid target is found in 20 tries
    return targetIndex;
}

long GetRegenHullValue(const string &source)
{
    long regen = 0;

    const std::regex reg("REGEN\\s+(\\d+)\\s+(\\d+)");
    std::smatch match;

    if (std::regex_search(source, match, reg))
    {
        regen = stol(match[2]);
    }

    return regen;
}

long GetRegenShieldValue(const string &source)
{
    long regen = 0;

    const std::regex reg("REGEN\\s+(\\d+)\\s+(\\d+)");
    std::smatch match;

    if (std::regex_search(source, match, reg))
    {
        regen = stol(match[1]);
    }

    return regen;
}

std::pair<long, long> GetRegenValues(const string &source)
{
    std::pair<long, long> regen;

    const std::regex reg("REGEN\\s+(\\d+)\\s+(\\d+)");
    std::smatch match;

    if (std::regex_search(source, match, reg))
    {
        regen.first = stol(match[1]);
        regen.second = stol(match[2]);
    }

    return regen;
}

/*
 * =============================================================================
 * GetHullTarget returns the index for a randomly chosen target that meets the hull constraints
 * [JLL] This is complete rewrite of the original.  It was so convoluted and
 * the logic errors rampent enough that I decided to scrap it
 * =============================================================================
 */

int GetHullTarget(int forceId, const string &UnitData, int UnitTarget, int UnitScope, int NumTargets)
{
    int TargetSize = 0;
    string TargetData;
    string *targetsData;
    long *targetsHull;
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetHullTarget(forceId:" << forceId << ";UnitData:\"" << UnitData << "\";UnitTarget:" << UnitTarget << ";UnitScope:" << UnitScope << ";NumTargets:" << NumTargets << ")" << endl;
#endif

    // Check the force ID and get the appropriate target info
    if (forceId == 0)
    {
        targetsData = BE::SpecialB;
        targetsHull = BE::MaxHullB;
    }
    else
    {
        targetsData = BE::SpecialA;
        targetsHull = BE::MaxHullA;
    }

    // Try to get a valid target.  Do this 20 times...
    // TODO: Verify with group that this is the desired behavior.
    int targetIndex = -1; // TODO: Replace -1 with constant
    for (int i = 0; i < 20; i++)
    {
        targetIndex = rand() % NumTargets;
        TargetData = targetsData[targetIndex];
        TargetSize = targetsHull[targetIndex];
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] GetHullTarget: Try "
                       << (i + 1)
                       << ", checking target "
                       << targetIndex
                       << ": "
                       << TargetSize
                       << ",\""
                       << TargetData
                       << "\"" << endl;
#endif
        // Don't shoot at missiles or at reserve units unless the hit is global
        if (!IsMissile(TargetData) && (HasReserve(TargetData) <= 0 || IsGlobal(UnitData)))
        {
            // Do the avoidance check
            if (UnitTarget < 0 && (TargetSize < (-UnitTarget - UnitScope) || TargetSize > (-UnitTarget + UnitScope)))
            {
                // We found our target and it is outside the avoidance range
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] GetHullTarget: Target found outside range!" << endl;
#endif
                break;
            }
            else if (UnitTarget > 0 && (TargetSize >= (UnitTarget - UnitScope) && TargetSize <= (UnitTarget + UnitScope)))
            {
                // We found our target and it is inside the priority range
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] GetHullTarget: Target found inside range!" << endl;
#endif
                break;
            }
            else if (UnitTarget == 0)
            {
                // Just use the first non-missile target since hull is 0
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] GetHullTarget: Found any legal target" << endl;
#endif
                break;
            }
        }
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetHullTarget => " << targetIndex << endl;
#endif

    // Return the target index we found
    return targetIndex;
}

/*
 * ============================================================================
 * GetScanTarget returns the index for a randomly chosen target from a list of targets that meet the scan constraints
 * [JLL] This is a complete rewrite of the original.  It was so convoluted
 *  and had logic errors that I decided to scrap it.
 * ============================================================================
 */
int GetScanTarget(int forceId, const string &UnitData, int UnitTarget, int UnitScope, int NumTargets)
{
    int ValidTarget = 0, TargetSize = 0;
    string TargetData;
    // int ValidTargets[9999] = {0}; // FIXME: Change to dynamic array.  Most battles will not have 10k units.  Can use NumTargets!
    int *ValidTargets{new int[NumTargets]{}};
    int target = 0;
    string *targetsData;
    long *targetsHull;

    if (forceId == 0)
    {
        targetsData = BE::SpecialB;
        targetsHull = BE::MaxHullB;
    }
    else
    {
        targetsData = BE::SpecialA;
        targetsHull = BE::MaxHullA;
    }

    // Scan through all targets and build a list of valid targets
    for (int i = 0; i < NumTargets; i++)
    {
        // Get the size and data for the possible target
        TargetSize = targetsHull[i];
        TargetData = targetsData[i];

        // Is the target a missile? Those get skipped entirely
        // Also skip units in reserve unless the hit is global
        if (!IsMissile(TargetData) && (HasReserve(TargetData) <= 0 || IsGlobal(UnitData)))
        {
            bool keeper = false;
            // First, is the base negative?
            if (UnitTarget < 0 && (TargetSize < (-UnitTarget - UnitScope) || TargetSize > (-UnitTarget + UnitScope)))
            {
                // Negative base means avoidance
                // With avoidance we test for the TargetSize being lower than base-scope or higher than base+scope
                // Since we are here, this is a keeper.
                keeper = true;
            }
            else if (UnitTarget > 0 && (TargetSize >= (UnitTarget - UnitScope) && TargetSize <= (UnitTarget + UnitScope)))
            {
                // Positive base means priority
                // With avoidance we test for the TargetSize being lower than base-scope or higher than base+scope
                // Since we are here, this is a keeper.
                keeper = true;
            }
            else if (UnitTarget == 0)
            {
                // Incase someone sets base scan as 0
                keeper = true;
            }

            // Is this unit a keeper?
            if (keeper)
            {
                ValidTargets[ValidTarget] = i;
                ValidTarget++;
            }
        }
    }

    // Check if any valid targets were found
    if (ValidTarget == 0)
    {
        // Roll a random target, from all available targets, since there are no valid targets
        int targetIndex = rand() % ValidTarget; // This generates a random number from 0 to ValidTargets-1
        target = ValidTargets[targetIndex];
    }
    else
    {
        // Roll a random target from the list of valid targets
        target = GetRandomTarget(forceId, UnitData, NumTargets);
    }

    // Cleanup after ourselves
    delete[] ValidTargets;

    // Return the index of the targetted unit
    return target;
}

int GetTriHex(const string &digit)
{
    int val = stoi(digit, 0, 36);

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetTriHex(\"" << digit << "\")" << endl;
#endif

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] GetTriHex(" << val << ")" << endl;
#endif

    return val;
}

int HasAmmoWT(const string &special)
{
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
    if (start != string::npos)
    {
        int middle = special.find(" ", start);
        int end = special.find(" ", middle + 1);
        int len = end - middle;
        string ammo = special.substr(middle, len);
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasAmmoWT(ammo:" << ammo << ")" << endl;
#endif
        res = stoi(ammo);
    }
    else
    {
        res = CBE::AMMO_INFINITE;
    }

    // Return the result
    return res;
}

bool HasArmor(const string &special, long &ar)
{
    // TODO: This function needs renamed.  It checks for SR and has nothing to do with Shield points remaining.
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasArmor(\"" << special << "\")" << endl;
#endif

    // Look for `AR` in the special string
    int start = special.find("AR");
    if (start != string::npos)
    {
        // Found it!  Now get the base and the scope
        res = true;
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        ar = stoi(special.substr(start, start - end + 1));
#ifdef CBE_DEBUG
        CBE::debugFile << ("[INFO] HasArmor: ar=" + ar) << endl;
#endif
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasArmor => " << res << endl;
#endif

    return res;
}

bool HasArtilleryWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasArtilleryWT(\"" << special << "\")" << endl;
#endif

    // Look for `artillery` in the special string
    int start = special.find("artillery");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasArtilleryWT => " << res << endl;
#endif

    return res;
}

int HasBatteries(const string &special)
{
    int res = 0;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasBatteries(\"" << special << "\")" << endl;
#endif

    for (int i = 0; i < special.length(); i++)
    {
        // Count the opening square brackers to determine number of batteries
        if (special.at(i) == '[')
        {
            res++;
        }
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasBatteries => " << res << endl;
#endif

    return res;
}

bool HasBPWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasBPWT(\"" << special << "\")" << endl;
#endif

    // Look for `bp` in the special string
    int start = special.find("bp");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasBPWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasBreak(const string &special, long &brk)
{
    const regex reg("BREAK\\s+(\\d+)");
    smatch match;
    bool res = false;

    if (regex_search(special, match, reg))
    {
        brk = stol(match[1]);
        res = true;
    }

    return res;
}

bool HasDamage(const string &special, long &damage)
{
    const regex reg("DAMAGE\\s+(\\d+)");
    smatch match;
    bool res = false;

    if (regex_search(special, match, reg))
    {
        damage = stol(match[1]);
        res = true;
    }

    return res;
}

bool HasDatalinkWT(const string &special, int &group)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDatalinkWT(\"" << special << "\")" << endl;
#endif

    // Look for `meson` in the special string
    int start = special.find("dl ");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
        // Now get the datalink group
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        char value = special.substr(start + 1, end - start)[0];
        // Indices is thus:
        // Index 0 << A = 65
        // Index 1 << B = 66
        // Index 26 << a = 97
        // Index 27 << b = 98
        if (value >= 65 && value <= 90)
        {
            // The datalink group is A-Z
            group = value - 65;
        }
        else if (value >= 97 && value <= 122)
        {
            // The datalink group is a-z
            group = value - 71;
        }
        else
        {
// Not a valid group label.
// TODO: Throw an exception
#ifdef CBE_DEBUG
            CBE::debugFile << "[ERROR] HasDatalinkWT(): Datalink group label is invalid." << endl;
#endif
        }
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDatalinkWT => " << res << endl;
#endif

    return res;
}

bool HasCrackWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasCrackWT(\"" << special << "\")" << endl;
#endif

    // Look for `crack` in the special string
    int start = special.find("crack");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasCrackWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasCrew(const string &special, long &crew)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasCrew(\"" << special << "\")" << endl;
#endif

    int start = special.find("CREW");
    if (start != std::string::npos)
    {
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        crew = stol(special.substr(start, end - start));
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasCrew: " << crew << endl;
#endif
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasCrew => " << res << endl;
#endif

    return res;
}

int HasDefense(const string &special)
{
    int start = 0;
    int res = std::numeric_limits<int>::min(); // TODO: Replace with constant for consistancy and clarity.

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDefense(\"" << special << "\")" << endl;
#endif

    start = special.find("DEFENSE");
    // IF the position of DEFENSE is NOT no position
    if (start != string::npos)
    {
        // Get the value of the defense tag and return it.
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        res = stoi(special.substr(start + 1, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDefense => " << res << endl;
#endif

    return 0;
}

int HasDelay(const string &special)
{
    int start = 0;
    int res = -1;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDelay(special:\"" << special << "\")" << endl;
#endif

    start = special.find("DELAY");
    if (start != string::npos)
    {
        int middle = special.find(" ", start);
        int end = special.find(" ", middle + 1);
        int len = end - middle;
        string delay = special.substr(middle, len);
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasDelay(delay:" << delay << ")" << endl;
#endif
        res = stoi(delay);
    }

    return res;
}

bool HasDisableWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDisableWT(\"" << special << "\")" << endl;
#endif

    // Look for `dis` in the special string
    int start = special.find("dis");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasDisableWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasFlakWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasFlakWT(\"" << special << "\")" << endl;
#endif

    // Look for `artillery` in the special string
    int start = special.find("flak");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasFlakWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasFlicker(const string &special, long &flicker)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasFlicker(\"" << special << "\")" << endl;
#endif

    // Look for `artillery` in the special string
    int start = special.find("FLICKER");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        flicker = stoi(special.substr(start, start - end));
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasFlicker: flicker=" << flicker << endl;
#endif
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasFlicker => " << res << endl;
#endif

    return res;
}

bool HasHeatWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasHeatWT(\"" << special << "\")" << endl;
#endif

    // Look for `heat` in the special string
    int start = special.find("heat");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasHeatWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasHull(const string &special, int &base, int &scope)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasHull(\"" << special << "\")" << endl;
#endif

    // Look for `hull` in the special string
    int start = special.find("hull");
    if (start != string::npos)
    {
        // Found it!  Now get the base and the scope
        res = true;
        int mark1 = special.find(" ", start);
        int mark2 = special.find(" ", mark1 + 1);
        int mark3 = special.find(" ", mark2 + 1);
        base = stoi(special.substr(mark1, mark2 - mark1 + 1));
        scope = stoi(special.substr(mark2, mark3 - mark2 + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasHull(" << res << ")" << endl;
#endif

    return res;
}

bool HasLong(const string &special)
{
    bool res = false;

    // IF the position of "LONG" is NOT npos (no position)
    if (special.find("LONG") != string::npos)
    {
        // The it must be a long range weapons
        res = true;
    }

    return res;
}

bool HasLongWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasLongWT(special:\"" + special + "\")" << endl;
#endif

    // IF the position of "long" is NOT npos (no position)
    if (special.find("long") != string::npos)
    {
        // Then it must have a long range weapon bracket
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasLongWT => " << res << endl;
#endif

    return res;
}

bool HasMesonWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMesonWT(\"" << special << "\")" << endl;
#endif

    // Look for `meson` in the special string
    int start = special.find("meson");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMesonWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasMissileWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMissileWT(\"" << special << "\")" << endl;
#endif

    int start = special.find("mis");

    // IF the position of "mis" is NOT npos (no position)
    if (start != string::npos)
    {
        // Then it must have a mis tag in a bracket
        res = true;

        // This part is ridiculous...
        string B = special.substr(start + 3, 1);
        string S = special.substr(start + 4, 1);
        string T = special.substr(start + 5, 1);
        string H = special.substr(start + 6, 1);
        BE::MissileB = GetTriHex(B);
        BE::MissileS = GetTriHex(S);
        BE::MissileT = GetTriHex(T);
        BE::MissileH = GetTriHex(H);
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMissileWT => " << res << endl;
#endif

    return res;
}

int HasMultiWT(const string &special)
{
    int res = -1; // TODO: Use a constant here

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMultiWT(\"" << special << "\")" << endl;
#endif

    // Look for `multi` in the special string
    int start = special.find("multi");
    if (start != string::npos)
    {
        // Found it!  Now get the packet size
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        res = stoi(special.substr(start, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasMultiWT => " << res << endl;
#endif

    return res;
}

bool HasPenWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasPenWT(\"" << special << "\")" << endl;
#endif

    // Look for `pen` in the special string
    int start = special.find("pen");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasPenWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasPointDefense(const string &special, int &pd)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasPointDefense(\"" << special << "\")" << endl;
#endif

    // Look for `pen` in the special string
    int start = special.find("PD");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
        // Now get the actual PD value
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        pd = stoi(special.substr(start, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasPointDefense(res:" << res << ",pd:" << pd << ")" << endl;
#endif

    return res;
}

int HasReserve(const string &special)
{
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasReserve(special:\"" << special << "\")" << endl;
#endif
    int res = -1;

    // Look for the starting position of "RESERVE"
    int start = special.find("RESERVE");
    if (start != string::npos)
    {
        int middle = special.find(" ", start);
        int end = special.find(" ", middle + 1);
        int len = end - middle;
        string reserve = special.substr(middle, len);

        res = stoi(reserve);
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasReserve => " << res << endl;
#endif

    return res;
}

bool HasResist(const string &special, long &resist)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasResist(\"" << special << "\")" << endl;
#endif

    // Look for `vibro` in the special string
    int start = special.find("RESIST");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;

        // Now get the resist value
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        resist = stoi(special.substr(start + 1, end - start + 1));
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasResist(resist:" << resist << ")" << endl;
#endif
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasResist => " << res << endl;
#endif

    return res;
}

bool HasScan(const string &special, int &base, int &scope)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasScan(\"" << special << "\")" << endl;
#endif

    // Look for `hull` in the special string
    int start = special.find("scan");
    if (start != string::npos)
    {
        // Found it!  Now get the base and the scope
        res = true;
        int mark1 = special.find(" ", start);
        int mark2 = special.find(" ", mark1 + 1);
        int mark3 = special.find(" ", mark2 + 1);
        base = stoi(special.substr(mark1, mark2 - mark1 + 1));
        scope = stoi(special.substr(mark2, mark3 - mark2 + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasScan => " << res << endl;
#endif

    return res;
}

bool HasShields(const string &special, long &sr)
{
    // TODO: This function needs renamed.  It checks for SR and has nothing to do with Shield points remaining.
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasShields(\"" << special << "\")" << endl;
#endif

    // Look for `hull` in the special string
    int start = special.find("SR");
    if (start != string::npos)
    {
        // Found it!  Now get the base and the scope
        res = true;
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        sr = stoi(special.substr(start, start - end + 1));
#ifdef CBE_DEBUG
        CBE::debugFile << ("[INFO] HasShields: sr=" + sr) << endl;
#endif
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasShields => " << res << endl;
#endif

    return res;
}

int HasSpecialWT(const string &special)
{
    int res = -1; // TODO: Replace with constant for consistancy and clarity

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasSpecialWT(\"" << special << "\")" << endl;
#endif

    // Look for `special` in the special string
    int start = special.find("special");
    if (start != string::npos)
    {
        // Found it!  Now get the special table ID
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        res = stoi(special.substr(start, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasSpecialWT(" << res << ")" << endl;
#endif

    return res;
}

int HasTarget(const string &special)
{
    int start = 0;
    int res = std::numeric_limits<int>::min(); // TODO: Replace with constant for consistancy and clarity. TODO: Use negative infinity or at least min()

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasTarget(\"" << special << "\")" << endl;
#endif

    start = special.find("TARGET");
    // IF the position of DEFENSE is NOT no position
    if (start != string::npos)
    {
        // Get the value of the defense tag and return it.
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        res = stoi(special.substr(start + 1, end - start + 1));
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasTarget(" << res << ")" << endl;
#endif

    return 0;
}

bool HasTime(const string &special, long &time)
{
    bool res = false;
    regex reg("TIME\\s+(\\d+)");
    smatch match;

    if (regex_match(special, match, reg))
    {
        time = stol(match[1]);
        res = true;
    }

    return res;
}

bool HasVibroWT(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasVibroWT(\"" << special << "\")" << endl;
#endif

    // Look for `vibro` in the special string
    int start = special.find("vibro");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasVibroWT(" << res << ")" << endl;
#endif

    return res;
}

bool HasYield(const string &special, long &yield)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasYield(\"" << special << "\")" << endl;
#endif

    // Look for `vibro` in the special string
    int start = special.find("yield");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;

        // Now get the yield value
        start = special.find(" ", start);
        int end = special.find(" ", start + 1);
        yield = stoi(special.substr(start + 1, end - start + 1));
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] HasYield(yield:" << yield << ")" << endl;
#endif
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] HasYield => " << res << endl;
#endif

    return res;
}

string InsertOffline(string source)
{
    int start = 0;
    int end_tag = 1;
    string temp_str;

    while (true)
    {
        start = source.find("]", end_tag);
        if (start != string::npos)
        {
            end_tag = start + 10;
            temp_str = source.substr(0, start) + " offline" + source.substr(start);
            source = temp_str;
        }
        else
        {
            return source;
        }
    }
}

string InsertRandomOffline(string source)
{
    int start = 0;
    int end_tag = 1;
    string temp_str;

    while (true)
    {
        start = source.find("]", end_tag);
        if (start != string::npos)
        {
            if (rand() % 2 == 0)
            {
                temp_str = source.substr(0, start) + " offline" + source.substr(start);
                source = temp_str;
                end_tag = start + 10;
            }
            else
            {
                end_tag = start + 10;
            }
        }
        else
        {
            return source;
        }
    }
}

bool IsMissile(const string &special)
{
    bool res = false;

    // If the position of "MSL" is NOT npos
    if (special.find("MSL") != string::npos)
    {
        // Then it must be a missile
        res = true;
    }

    return res;
}

bool IsBuilding(const std::string &data_str)
{
    return (data_str.find("BUILDING") != std::string::npos);
}

bool IsCarrier(const std::string &data_str)
{
    return (data_str.find("CARRIER") != std::string::npos);
}

bool IsOrbital(const std::string &data_str)
{
    return (data_str.find("ORBITAL") != std::string::npos);
}

bool IsVolatile(const std::string &data_str)
{
    return (data_str.find("VOLATILE") != std::string::npos);
}

bool IsVehicle(const std::string &data_str)
{
    return (data_str.find("VEHICLE") != std::string::npos);
}

bool IsBio(const std::string &data_str)
{
    return (data_str.find("BIO") != std::string::npos);
}

bool IsCaptured(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsCaptured(\"" << special << "\")" << endl;
#endif

    // IF the position of "CAPTURED" is NOT npos (no position)
    if (special.find("CAPTURED") != string::npos)
    {
        // Then it must be captured
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsCaptured => " << res << endl;
#endif

    return res;
}

bool IsCloak(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsCloak(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "CLOAK" is NOT npos (no position)
    if (special.find("CLOAK") != string::npos)
    {
        // The it must be cloaked
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsCloak => " << res << endl;
#endif

    return res;
}

bool IsCrippled(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsCripple(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "CRIPPLE" is NOT npos (no position)
    if (special.find("CRIPPLE") != string::npos)
    {
        // Then it must be crippled
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsCripple => " << res << endl;
#endif

    return res;
}

bool IsDrifting(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsDrifting(\"" << special << "\")" << endl;
#endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if (special.find("DRIFTING") != string::npos)
    {
        // Then it must be drifting
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsDrifting => " << res << endl;
#endif

    return res;
}

bool IsFighter(const string &special)
{
    bool res = false;
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFighter(" << special << ")" << endl;
#endif

    // IF the position of "FIGHTER" is NOT npos (no position)
    if (special.find("FIGHTER") != string::npos)
    {
        // Then it must be a fighter
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFighter => " << res << endl;
#endif

    return res;
}

bool IsFled(const string &special)
{
    bool res = false;

    // IF the position of "FLED" is NOT npos (no position)
    if (special.find("FLED") != string::npos)
    {
        // Then it must be fled
        res = true;
    }

    return res;
}

bool IsFlee(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFlee(\"" << special << "\")" << endl;
#endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if (special.find("FLEE") != string::npos)
    {
        // Then it must be drifting
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsFlee(" << res << ")" << endl;
#endif

    return res;
}

bool IsGlobal(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsGlobal(\"" << special << "\")" << endl;
#endif

    // Look for `pen` in the special string
    int start = special.find("global");
    if (start != string::npos)
    {
        // Found it!  Set the result to TRUE
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsGlobal(" << res << ")" << endl;
#endif

    return res;
}

bool IsGround(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsGround(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if (special.find("GROUND") != string::npos)
    {
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsGround => " << res << endl;
#endif

    return res;
}

bool IsMine(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsMine(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if (special.find("MINE") != string::npos)
    {
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsMine => " << res << endl;
#endif

    return res;
}

bool IsNoMove(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsNoMove(\"" << special << "\")" << endl;
#endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if (special.find("NOMOVE") != string::npos)
    {
        // Then it must be drifting
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsNoMove =>" << res << endl;
#endif

    return res;
}

bool IsOffline(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsOffline(\"" << special << "\")" << endl;
#endif

    // IF the position of "DRIFTING" is NOT npos (no position)
    if (special.find("offline") != string::npos)
    {
        // Then it must be drifting
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsOffline(" << res << ")" << endl;
#endif

    return res;
}

bool IsSolid(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsSolid(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if (special.find("SOLID") != string::npos)
    {
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsSolid => " << res << endl;
#endif

    return res;
}

bool IsScuicide(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsScuicide(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if (special.find("SUICIDE") != string::npos)
    {
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsScuicide => " << res << endl;
#endif

    return res;
}

bool IsStatis(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsStatis(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if (special.find("STASIS") != string::npos)
    {
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsStatis => " << res << endl;
#endif

    return res;
}

bool IsSurprise(const string &special)
{
    bool res = false;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsSurprise(special:\"" << special << "\")" << endl;
#endif

    // IF the position of "SURPRISE" is NOT npos (no position)
    if (special.find("SURPRISE") != string::npos)
    {
        res = true;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] IsSurprise => " << res << endl;
#endif

    return res;
}

bool IsToothless(long enemy, long ID)
{
    bool noTeeth = true;

    if (enemy == 0)
    {
        string tempStr = BE::SpecialA[ID];
        if (HasBatteries(tempStr))
        {
            vector<string> batteries = GetBrackets(tempStr);

            for (int i = 0; i < batteries.size(); i++)
            {
                if (batteries[i].find("bp") == string::npos)
                {
                    int ammo = HasAmmoWT(batteries[i]);
                    if (ammo > 0 || ammo == CBE::AMMO_INFINITE)
                    {
                        noTeeth = false;
                    }
                }
            }
        }
    }
    else
    {
        string tempStr = BE::SpecialB[ID];
        if (HasBatteries(tempStr))
        {
            vector<string> batteries = GetBrackets(tempStr);

            for (int i = 0; i < batteries.size(); i++)
            {
                if (batteries[i].find("bp") == string::npos)
                {
                    int ammo = HasAmmoWT(batteries[i]);
                    if (ammo > 0 || ammo == CBE::AMMO_INFINITE)
                    {
                        noTeeth = false;
                    }
                }
            }
        }
    }

    return noTeeth;
}

string RebuildBatteryTags(const string &special, const BE::SalvoInfo *salvos, int count)
{
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] RebuildBatteryTags(\"" + special + "\")" << endl;
    for (int i = 0; i < count; i++)
    {
        CBE::debugFile << "[INFO] RebuildBatteryTags(salvo" + to_string(i) + ":\"" + salvos[i].DataStr + "\")" << endl;
    }
#endif
    string res = "";
    bool strip = false;

    // First, strip out all of the old battery tags
    for (int i = 0; i < special.size(); i++)
    {
        if (special[i] == '[')
        {
            strip = true;
        }
        if (!strip)
        {
            res = res + special[i];
        }
        if (special[i] == ']')
        {
            strip = false;
        }
    }

    // Now build the new battery tags from the salvos
    for (int i = 0; i < count; i++)
    {
        res = salvos[i].DataStr + res;
    }

    return res;
}

string ReduceCrew(const string &source, int crewDamage)
{
    string res = "";
    return res;
}

string RemoveTag(const string &source, const string &target, int num_fields)
{
    // Remove the `target` tag from the `source` string and return a new string

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] RemoveTag(source:\"" << source << "\";target:\"" << target << "\";num_fields:" << num_fields << ")" << endl;
#endif

    int A = 0, start = 0, end_tag = 0, tag1 = 0, tag2 = 0;
    string res = "";

    start = source.find(target);
    if (start != string::npos)
    {
        end_tag = source.find(" ", start + 1);
        if (end_tag == string::npos)
        {
            end_tag = source.length();
        }
        // If the tag has at least 1 field, search for the next " " character
        if (num_fields > 0)
        {
            end_tag = source.find(" ", end_tag + 1);
            if (end_tag == string::npos)
            {
                end_tag = source.length();
            }
        }
        // If the tag has at least 2 fields, search for the next " " character again
        if (num_fields > 1)
        {
            end_tag = source.find(" ", end_tag + 1);
            if (end_tag == string::npos)
            {
                end_tag = source.length();
            }
        }

#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] RemoveTag(start:" << start << ";end_tag:" << end_tag << ")" << endl;
#endif
        // Get the "left" portion of the source string
        string left = source.substr(0, start - 1);
#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] RemoveTag(left:\"" << left << "\")" << endl;
#endif
        // Get the "right" portion of the source string
        string right = "";
        if (end_tag < source.length())
        {
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
        if (res.at(0) == '[')
        {
            for (int i = 0; i < res.length(); i++)
            {
                // Check if the character is a '['
                if (res.at(i) == '[')
                {
                    tag1++;
                }
                // Check if the character is a ']'
                else if (res.at(i) == ']')
                {
                    tag2++;
                }
            }
            if (tag1 > tag2)
            {
                res = res + "]";
            }
        }
    }
    else
    {
        // The target tag does not exist, just return the original source string
        res = source;
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] RemoveTag(res:\"" << res << "\")" << endl;
#endif

    return res;
}

int SetFlagsWT(const string &special, int bit_flag)
{

    // Check for `dis`
    if (HasDisableWT(special))
    {
        bit_flag = bit_flag | BE::saDis;
    }

    // Check for `heat`
    if (HasHeatWT(special))
    {
        bit_flag = bit_flag | BE::saHeat;
    }

    // Check for `meson`
    if (HasMesonWT(special))
    {
        bit_flag = bit_flag | BE::saMeson;
    }

    // Check for `vibro`
    if (HasVibroWT(special))
    {
        bit_flag = bit_flag | BE::saVibro;
    }

    // Check for `crack`
    if (HasCrackWT(special))
    {
        bit_flag = bit_flag | BE::saCrack;
    }

    // Check for `bp`
    if (HasBPWT(special))
    {
        bit_flag = bit_flag | BE::saBp;
    }

    // Check for `pen`
    if (HasPenWT(special))
    {
        bit_flag = bit_flag | BE::saPen;
    }

    // Check for `special`
    if (HasSpecialWT(special) > 0)
    {
        bit_flag = bit_flag | BE::saSpecial;
    }

    return bit_flag;
}

void writeBattleString(ofstream &iostr, const string &str)
{
    if (str.size() > 0)
    {
        iostr << str << "\n";
    }
}

void writeRemainingUnits(ofstream &iostr,const int unitCount,const std::string *unitNames,const long *unitCurBeam,const long *unitCurShield,const long *unitCurTorp,const long *unitCurHull,const long *unitMaxHull) {
    // This function outputs all remaining ships in a fleet
    // This is the pattern: <ship name> Bm=# Sh=# Tp=# Hl=# Hull Damage Level: #%

    for(int i = 0;i < unitCount;i++) {
        iostr << (i+1) << ": " << unitNames[i];
        iostr << " Bm=" << unitCurBeam[i];
        iostr << " Sh=" << unitCurShield[i];
        iostr << " Tp=" << unitCurTorp[i];
        iostr << " Hl=" << unitCurHull[i];
        int damage = unitCurHull[i] * 100 / unitMaxHull[i];
        iostr << " Hull Damage Level: " << damage << "%\n";
    }
}

void writeTempFiles()
{
    long old_AttShipsLeft = 0;
    long old_DefShipsLeft = 0;

    old_AttShipsLeft = BE::AttShipsLeft; // TODO: Move to variable declaration.
    old_DefShipsLeft = BE::DefShipsLeft; // TODO: Move to variable declaration.

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO][writeTempFiles] ############################## WRITE TEMP FILES ##############################" << endl;
    CBE::debugFile << "[INFO][writeTempFiles] CombatRound: " << BE::CombatRound << "; Attackers: " << old_AttShipsLeft << "; Defenders: " << old_DefShipsLeft << endl;
#endif

    // Write the Working and Temp files
    /*ofstream turnTempA;
    turnTempA.open("tempA" + to_string(BE::CombatRound) + ".csv", ios::out | ios::binary | ios::app);
    ofstream turnTempB;
    turnTempB.open("tempB" + to_string(BE::CombatRound) + ".csv", ios::out | ios::binary | ios::app);//*/

    // Find Dead and Fled Ships
    ofstream attFledFile;
    attFledFile.open("fled_att.csv", ios::out | ios::binary | ios::app);
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO][writeTempFiles] ##### PROCESSING FLED ATTACKERS " << BE::AttShipsLeft << " #####" << endl;
#endif
    for (int i = 0; i < old_AttShipsLeft; i++)
    {
        // IF the current ships hull is less than 1 OR (the current ship is a missile AND the combat round greater than 0)
        // This clears out dead ships and missiles from the ships list/array
        if (BE::CurHullA[i] < 1 || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0))
        {
            // TODO: Add writing to a att_dead.csv file
            BE::AttShipsLeft = BE::AttShipsLeft - 1;
#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO][writeTempFiles] DEAD/MISSILE: " << BE::AttShipStr[i] << endl;
#endif
        }
        // ELSE non-dead ships that are not missiles
        else
        {
            // IF NOT a captured unit AND NOT a crippled unit
            if (!IsCaptured(BE::SpecialA[i]) && !IsCrippled(BE::SpecialA[i]))
            {
                // IF the unit has fled
                // Units that have been marked as fled but are captured or cripple never get considered fled.
                if (IsFled(BE::SpecialA[i]))
                {
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
            else
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][writeTempFiles] CAPTURE/CRIPPLE: " << BE::AttShipStr[i] << endl;
#endif
            }
        }
    }
    // Close the fled_att.csv file here
    attFledFile.flush();
    attFledFile.close();

    // Open TempAFile here
    ofstream tempAFile;
    tempAFile.open(BE::TempAFile, ios::out | ios::binary | ios::trunc);
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO][writeTempFiles] ##### PROCESSING OTHER ATTACKERS " << BE::AttShipsLeft << " #####" << endl;
#endif
    // Write the header to the fleet file here (TempAFile)
    tempAFile << BE::AttRaceName << "," << BE::AttFleetName << "," << BE::AttBreakOff << "," << BE::AttShipsTotal << "," << BE::AttFleetStrength << "," << BE::AttShipsLeft << "," << BE::AttTargetBonus << "," << BE::AttTargetPriority << "," << BE::AttReserve << "\n";
    if (BE::AttShipsLeft > 0)
    {
        for (int i = 0; i < old_AttShipsLeft; i++)
        {
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
            if (BE::CurHullA[i] < 1 || IsFled(BE::SpecialA[i]) || (IsMissile(BE::SpecialA[i]) && BE::CombatRound > 0))
            {
// Do nothing as this ship is either dead, fled, or a missile
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][writeTempFiles] DEAD/FLED/MSL: " << BE::AttShipStr[i] << endl;
#endif
            }
            else
            {
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
    defFledFile.open("fled_def.csv", ios::out | ios::binary | ios::app);
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO][writeTempFiles] ##### PROCESSING FLED DEFENDERS " << BE::DefShipsLeft << " #####" << endl;
#endif
    for (int i = 0; i < old_DefShipsLeft; i++)
    {
        // IF the current ships hull is less than 1 OR (the current ship is a missile AND the combat round greater than 0)
        // This clears out dead ships and missiles from the ships list/array
        if (BE::CurHullB[i] < 1 || (IsMissile(BE::SpecialB[i]) && BE::CombatRound > 0))
        {
            BE::DefShipsLeft = BE::DefShipsLeft - 1;
            // TODO: Add writing to a att_dead.csv file
        }
        // ELSE non-dead ships that are not missiles
        else
        {
            // IF NOT a captured unit AND NOT a crippled unit
            if (!IsCaptured(BE::SpecialB[i]) && !IsCrippled(BE::SpecialB[i]))
            {
                // IF the unit has fled
                // Units that have been marked as fled but are captured or cripple never get considered fled.
                if (IsFled(BE::SpecialB[i]))
                {
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
            else
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][writeTempFiles] CAPTURE/CRIPPLE: " << BE::DefShipStr[i] << endl;
#endif
            }
        }
    }
    // Close the fled_def.csv file here
    defFledFile.flush();
    defFledFile.close();

    // Open TempAFile here
    ofstream tempBFile;
    tempBFile.open(BE::TempBFile, ios::out | ios::binary | ios::trunc);
#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO][writeTempFiles] ##### PROCESSING OTHER DEFENDERS " << BE::DefShipsLeft << " #####" << endl;
#endif
    // Write the header to the fleet file here (TempAFile)
    tempBFile << BE::DefRaceName << "," << BE::DefFleetName << "," << BE::DefBreakOff << "," << BE::DefShipsTotal << "," << BE::DefFleetStrength << "," << BE::DefShipsLeft << "," << BE::DefTargetBonus << "," << BE::DefTargetPriority << "," << BE::DefReserve << "\n";
    if (BE::DefShipsLeft > 0)
    {
        for (int i = 0; i < old_DefShipsLeft; i++)
        {
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
            if (BE::CurHullB[i] < 1 || IsFled(BE::SpecialB[i]) || (IsMissile(BE::SpecialB[i]) && BE::CombatRound > 0))
            {
// Do nothing as this ship is either dead, fled, or a missile
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][writeTempFiles] DEAD/FLED/MSL: " << BE::DefShipStr[i] << endl;
#endif
            }
            else
            {
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

    /*turnTempA.flush();
    turnTempA.close();
    turnTempB.flush();
    turnTempB.close();//*/
}

void readTempA()
{
    // Open tempA file for reading
    ifstream tempA;
#ifdef CBE_DEBUG
    std::cout << "Opening " << BE::TempAFile << " as tempA" << endl;
    CBE::debugFile << "[INFO] Opening " << BE::TempAFile << " as tempA" << endl;
#endif
    tempA.open(BE::TempAFile, ios::binary | ios::in);
    // Read in the fleet header line
    if (tempA.is_open())
    {
        // Read the first line
        string header = "";
        getline(tempA, header, '\n');
        BE::FleetInfo info = parseFleetHeader(header);
#ifdef CBE_DEBUG
        std::cout << info.RaceName << " - " << info.FleetName << endl;
        CBE::debugFile << info.RaceName << " - " << info.FleetName << endl;
#endif
        BE::AttRaceName = info.RaceName;
        BE::AttFleetName = info.FleetName;
        BE::AttBreakOff = info.BreakOff;
        BE::AttFleetStrength = info.FleetStrength;

        // Read the unit lines
        long numUnits = 0;
        long fleetStrength = 0;
        string line = "";
        while (getline(tempA, line, '\n'))
        {
            // Now break it back out to the individual arrays...because...BASIC
            BE::UnitInfo unit = parseUnit(line);
            if (unit.CurHull > 0)
            {
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
                fleetStrength += unit.MaxHull;
            }
            else
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][readTempA] Loading a unit with no hull" << endl;
#endif
            }
        }

        BE::AttShipsLeft = numUnits;
        if (fleetStrength > BE::AttFleetStrength)
        {
            BE::AttFleetStrength = fleetStrength;
        }

#ifdef CBE_DEBUG
        CBE::debugFile << "[INFO] Attacking Units Loaded: " << BE::AttShipsLeft << endl;
#endif

        // Close the file
        tempA.close();
    }
    else
    {
        std::cout << "Unable to open: " << BE::TempAFile << endl;
    }
}

void readTempB()
{
    // Open tempA file for reading
    ifstream tempB;
#ifdef CBE_DEBUG
    std::cout << "Opening " << BE::TempBFile << " as tempB" << endl;
    CBE::debugFile << "Opening " << BE::TempBFile << " as tempB" << endl;
#endif
    tempB.open(BE::TempBFile, ios::binary | ios::in);
    // Read in the fleet header line
    if (tempB.is_open())
    {
        // Read the first line
        string header = "";
        getline(tempB, header, '\n');
        BE::FleetInfo info = parseFleetHeader(header);
#ifdef CBE_DEBUG
        std::cout << info.RaceName << " - " << info.FleetName << endl;
        CBE::debugFile << info.RaceName << " - " << info.FleetName << endl;
#endif
        BE::DefRaceName = info.RaceName;
        BE::DefFleetName = info.FleetName;
        BE::DefBreakOff = info.BreakOff;
        BE::DefFleetStrength = info.FleetStrength;

        // Read the unit lines
        long numUnits = 0;
        long fleetStrength = 0;
        string line = "";
        while (getline(tempB, line, '\n'))
        {
            // Now break it back out to the individual arrays...because...BASIC
            BE::UnitInfo unit = parseUnit(line);

            if (unit.CurHull > 0)
            {
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
                fleetStrength += unit.MaxHull;
            }
            else
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][readTempB] Loading a unit with no hull" << endl;
#endif
            }
        }

        BE::DefShipsLeft = numUnits;
        if (fleetStrength > BE::DefFleetStrength)
        {
            BE::DefFleetStrength = fleetStrength;
        }

#ifdef CBE_DEBUG
        CBE::debugFile << "Defending Units Loaded: " << BE::DefShipsLeft << endl;
#endif

        // Close the file
        tempB.close();
    }
    else
    {
        std::cout << "Unable to open: " << BE::TempBFile << endl;
    }
}

string DoCrewDamage(const string &source, long crewDamage)
{
    long crewCur;
    string res = source;

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] DoCrewDamage(\"" << source << "\"," << crewDamage << ")" << endl;
#endif

    if (!IsSolid(res))
    {
        if (HasCrew(res, crewCur))
        {
            crewCur -= crewDamage;
            if (crewCur <= 0)
            {
                crewCur = 0;
                res = AddTag(res, "CRIPPLE");
            }
            res = RemoveTag(res, "CREW", 1);
            res = AddTag(res, "CREW " + to_string(crewCur));
        }
        else
        {
            crewCur = 100 - crewDamage;
            res = AddTag(res, "CREW " + to_string(crewCur));
        }
    }

#ifdef CBE_DEBUG
    CBE::debugFile << "[INFO] DoCrewDamage => \"" << res << "\"" << endl;
#endif

    return res;
}

/*
 * Main BattleEngine Loop
 */
void be_main()
{
    // [JLL] When you find comments like this: {{XXXX}} where XXXX is a number.  Those comments refer to specific lines in the original QBASIC code for cross reference purposes.
    critTables = load_crits_file("crits.txt");

    long AttackLoop = 0, AbortCounter = 0;
    long AttShipsRead = 0, DefShipsRead = 0;

    string status = "", NewTag = "", temp_str = "";

    bool AttHasScreen = false, DefHasScreen = false;
    bool AttHasReserveUnits = false, DefHasReserveUnits = false;

    long HullTarget = 0, CombatBonus = 0, ForceID = 0;
    long SeekTarget = 0, HasRaided = 0;
    long AttVal = 0, DefVal = 0, BPDice = 0, SuicideBonus = 0;
    long FirePower1 = 0, FirePower2 = 0;
    long DataLinkA[52] = {-1}, DataLinkB[52] = {-1};
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

    // Set counts and flags to their default values
    BE::CombatRound = 0;
    BE::RetreatFlag = 0;
    // BE::AttShipsLeft = 0;
    // BE::DefShipsLeft = 0;
    AttShipsRead = 0;
    DefShipsRead = 0;
    AttackLoop = 1;         // set to zero to end combat | TODO: This can be moved to variable declaration
    BE::AttacksMax = 20000; // TODO: This can be moved to the variable declaration
    BE::AttacksIndex = 0;

    BE::TempAFile = "tempa.csv";
    BE::TempBFile = "tempb.csv";

    BE::DM_ToHitA = 0;
    BE::DM_ToHitB = 0;

    // Original code has a lot of Form code here for the GUI.  Will pick up after that.

    // Basic init stuff
    BE::BO_Att = 0;                   // TODO: This can be moved to the variable declaration.
    BE::BO_Def = 0;                   // TODO: This can be moved to the variable declaration.
    BaseAccuracy = CBE::baseAccuracy; // TODO: Need to setup a command line arg and a interface for this.  | TODO: Move to variable declaration. | TODO: Move to BE namespace

    // Open the battle_report and damage_report files here for output
    ofstream reportFile("battle_r.txt", ios::out | ios::trunc | ios::binary); // This is #3 in basic
    ofstream damageFile("damage_r.txt", ios::out | ios::trunc | ios::binary); // This is #4 in basic

    // TODO: Do file checks to see if they opened

    // NEW: Set TempAFile and TempBFile to tempa.csv and tempb.csv for initial temp file writes.

    writeTempFiles();

    // NEW: Set back to original values

    // NOTE: TempAFile and TempBFile are set in the menu code when the user selects the attacker and defender files

    // This is where the program comes back in subsequent rounds
    // {{454}}
    do
    {
        // Load in the attackers
        readTempA();
        // Load in the defenders
        readTempB();

        if (BE::AttShipsLeft > 0 && BE::DefShipsLeft > 0)
        { // Trip the Round counter
            BE::CombatRound = BE::CombatRound + 1;

            // Battle finally commences!
            std::cout << "Battle Round: " << BE::CombatRound << endl;
            reportFile << "Battle Round: " << BE::CombatRound << "\n";

            // Write the attackers to the report file
            // {{486}}
            reportFile << "\nAttackers are the " << BE::AttRaceName << ", " << BE::AttFleetName << " " << BE::GroupName << ".\n";
            reportFile << "Current group Break-off level is " << BE::AttBreakOff << "%\n";
            reportFile << "The " << BE::UnitName << " are currently listed as:\n";

            for (int x = 0; x < BE::AttShipsLeft; x++)
            {
                reportFile << BE::AttShipStr[x] << " Bm=" << BE::CurBeamA[x] << " Sh=" << BE::CurShieldA[x] << " Tp=" << BE::CurTorpA[x] << " Hl=" << BE::CurHullA[x] << " \"" << BE::SpecialA[x] << "\"\n";
            }

            // Write the defenders to the report file
            // {{501}}
            reportFile << "\nDefenders are the " << BE::DefRaceName << ", " << BE::DefFleetName << " " << BE::GroupName << ".\n";
            reportFile << "Current group Break-off level is " << BE::DefBreakOff << "%\n";
            reportFile << "The " << BE::UnitName << " are currently listed as:\n";

            for (int x = 0; x < BE::DefShipsLeft; x++)
            {
                reportFile << BE::DefShipStr[x] << " Bm=" << BE::CurBeamB[x] << " Sh=" << BE::CurShieldB[x] << " Tp=" << BE::CurTorpB[x] << " Hl=" << BE::CurHullB[x] << " \"" << BE::SpecialB[x] << "\"\n";
            }

            // Ok, battle actually commences NOW
            reportFile << "\nBattle Results Commence:\n";

            // Clear the old damage array values
            // TODO: Change this to max array size not just ships left
            for (int a = 0; a < BE::AttShipsLeft; a++)
            {
                BE::HitsA[a] = 0;
                BE::PenHitsA[a] = 0;
                BE::BPAttackCritA[a] = 0;
            }
            // TODO: Change this to max array size not just ships left
            for (int b = 0; b < BE::DefShipsLeft; b++)
            {
                BE::HitsB[b] = 0;
                BE::PenHitsB[b] = 0;
                BE::BPAttackCritB[b] = 0;
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

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Beginning round " << BE::CombatRound << endl;
#endif

            // {{540}}
            if (BE::CombatRound == 1)
            {
                // Several special things can happen in turn one.
                // cloaked ships can get a first strike
                // long range weapons can get a first strike if the targets are not cloaked
                // And, we need to determine if FLAK equiped ships have targets

#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Doing Combat Round 1 Special Checks" << endl;
                CBE::debugFile << "[INFO] Checking attacking units" << endl;
#endif

                // Check the attacking fleet
                for (int a = 0; a < BE::AttShipsLeft; a++)
                {
                    // Count the number of units that are cloaked
                    if (IsCloak(BE::SpecialA[a]))
                    {
                        BE::AttIsCloaked = BE::AttIsCloaked + 1;
                    }
                    // Count if the unit has a long range tag
                    // TODO: Add a check for DefHasLongRange so that once it is true the checking can stop
                    /*if (HasLong(BE::SpecialA[a]))
                    {
                        BE::AttHasLongRange = true;
                    }//*/
                    // This checks all weapon tags since we pass it all weapon tags
                    if (HasLongWT(BE::SpecialA[a]))
                    {
                        BE::AttHasLongRange = true;
                    }
                    // Check if a fighter and not in reserve
                    // TODO: Add a check for DefHasFighters to stop the checking early
                    if (IsFighter(BE::SpecialA[a]) && HasReserve(BE::SpecialA[a]) < 0)
                    {
                        BE::AttHasFighters = 1;
                    }
                }

                // Determine if all the attackers are cloaked, if none are, or if some are cloked.
                // TODO: Seperate AttIsCloaked from the counting above
                if (BE::AttIsCloaked > 0)
                {
                    if (BE::AttIsCloaked == BE::AttShipsLeft)
                    {
                        BE::AttIsCloaked = 1;
                    }
                    else
                    {
                        BE::AttIsCloaked = 0;
                        BE::AttIsMixed = 1;
                    }
                }

#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Checking defending units" << endl;
#endif

                // Check the defending fleet
                for (int a = 0; a < BE::DefShipsLeft; a++)
                {
                    // Count the numbe of units that are cloaked
                    if (IsCloak(BE::SpecialB[a]))
                    {
                        BE::DefIsCloaked = BE::DefIsCloaked + 1;
                    }
                    // Count if the unit has a long rangew tag
                    // TODO: Add a check for DefHasLongRange so that once it is true the checking can stop
                    /*if (HasLong(BE::SpecialB[a]))
                    {
                        BE::DefHasLongRange = true;
                    }//*/
                    // This checks all weapon tags since we pass it all weapon tags
                    if (HasLongWT(BE::SpecialB[a]))
                    {
                        BE::DefHasLongRange = true;
                    }
                    // Check if a fighter and not in reserve
                    // TODO: Add a check for DefHasFighters to stop the checking early
                    if (IsFighter(BE::SpecialB[a]) && HasReserve(BE::SpecialB[a]) < 0)
                    {
                        BE::DefHasFighters = 1;
                    }
                }

                // Determine if all the defenders are cloaked, if none are, or if some are cloked.
                // TODO: Seperate DefIsCloaked from the counting above
                if (BE::DefIsCloaked > 0)
                {
                    if (BE::DefIsCloaked == BE::DefShipsLeft)
                    {
                        BE::DefIsCloaked = 1;
                    }
                    else
                    {
                        BE::DefIsCloaked = 0;
                        BE::DefIsMixed = 1;
                    }
                }
            }
            else
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Doing Combat Round " << BE::CombatRound << " Regular Checks" << endl;
                CBE::debugFile << "[INFO] Checking attacking units for DELAY" << endl;
#endif
                // Remove RESERVE tags from delayed units
                for (int a = 0; a < BE::AttShipsLeft; a++)
                {
                    // TODO: Split Has* and Get* functions so that boolean returns are separated from other types.
                    // TODO: Generalize HasTag and GetTag functions?
                    int delay = HasDelay(BE::SpecialA[a]);
                    if (HasReserve(BE::SpecialA[a]) && delay > 0)
                    {
                        if (delay < BE::CombatRound)
                        {
                            BE::SpecialA[a] = RemoveTag(BE::SpecialA[a], "RESERVE", 1);
                        }
                    }
                }

#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Checking defending units for DELAY" << endl;
#endif
                // Remove RESERVE tags from delayed units
                for (int b = 0; b < BE::DefShipsLeft; b++)
                {
                    int delay = HasDelay(BE::SpecialB[b]);
                    if (HasReserve(BE::SpecialB[b]) && delay > 0)
                    {
                        if (delay < BE::CombatRound)
                        {
                            BE::SpecialB[b] = RemoveTag(BE::SpecialB[b], "RESERVE", 1);
                        }
                    }
                }

                // Check attackers and defenders fleets for fighters
                // TODO: Need to turn this into a function: CheckFighters(special[],shipsLeft)
                for (int a = 0; a < BE::AttShipsLeft; a++)
                {
                    if (IsFighter(BE::SpecialA[a]) && !HasReserve(BE::SpecialA[a]))
                    {
                        BE::AttHasFighters = 1;
                        break; // Leave the loop early as we have found at least 1 fighter not in reserve.
                    }
                }
                for (int b = 0; b < BE::DefShipsLeft; b++)
                {
                    if (IsFighter(BE::SpecialB[b]) && !HasReserve(BE::SpecialB[b]))
                    {
                        BE::DefHasFighters = 1;
                        break; // Leave the loop early as we have found at least 1 fighter not in reserve.
                    }
                }
            }
            // {{600}}

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Pre-round Checks are done" << endl;
            CBE::debugFile << "[INFO] Checking attackers screen and reserve" << endl;
#endif

            // Check for attacker reserve & screen
            AttHasScreen = false;       // TODO: Move to BE namespace
            AttHasReserveUnits = false; // TODO: Move to BE namespace
            // {{605}}
            for (int a = 0; a < BE::AttShipsLeft; a++)
            {
                // TODO: What kind of mechanic should there be for not enough screen?
                // The unit is either in the reserve or is part of the screen
                if (HasReserve(BE::SpecialA[a]))
                {
                    AttHasReserveUnits = true;
                }
                else
                {
                    AttHasScreen = true;
                }
                // Check if both there is both a screen and a reserve already.
                if (AttHasScreen && AttHasReserveUnits)
                {
                    // Leave the loop early as we found a unit in the screen and a unit in the reserve.
                    break;
                }
            }

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Checking defenders screen and reserve" << endl;
#endif

            // Check for the defender reserve & screen
            // TODO: Turn this into a function: CheckFleetReserveAndScreen(special[],shipsLeft)
            DefHasScreen = false;       // TODO: Move to BE namespace
            DefHasReserveUnits = false; // TODO: Move to BE namespace
            // {{616}}
            for (int b = 0; b < BE::DefShipsLeft; b++)
            {
                // The unit is either in the reserve or is part of the screen
                if (HasReserve(BE::SpecialB[b]))
                {
                    DefHasReserveUnits = true;
                }
                else
                {
                    DefHasScreen = true;
                }
                // Check if both there is both a screen and a reserve already.
                if (DefHasScreen && DefHasReserveUnits)
                {
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
            // {{628}}
            if (AttHasReserveUnits)
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Attackers have reserve units" << endl;
                CBE::debugFile << "[INFO] Checking for attackers that need to come out of reserve" << endl;
#endif
                // Check those reserve units for BreakOff and for screen
                for (int a = 0; a < BE::AttShipsLeft; a++)
                {
                    // Get the reserve tag value of the ship
                    int reserve = HasReserve(BE::SpecialA[a]);
                    // Do the attackers have a screen?
                    if (!AttHasScreen)
                    {
                        // Does the ship have a reserve value?
                        if (reserve >= 0)
                        {
                            // Remove the reserve tag
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Unit leaving reserve because there is no screen" << endl;
#endif
                            BE::SpecialA[a] = RemoveTag(BE::SpecialA[a], "RESERVE", 1);
                            AttHasReserveUnits = false;
                        }
                    }
                    else
                    {
                        // Is the reserve tag equal to or less than the attackers BreakOff?
                        if (BE::BO_Att >= reserve && reserve >= 0)
                        {
                            // Remove the reserve tag
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Unit leaving reserve because reserve is less than break off" << endl;
#endif
                            BE::SpecialA[a] = RemoveTag(BE::SpecialA[a], "RESERVE", 1);
                        }
                    }
                }
            }

            // {{643}}
            if (DefHasReserveUnits)
            {
#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Defenders have reserve units" << endl;
                CBE::debugFile << "[INFO] Checking for defenders that need to come out of reserve" << endl;
#endif
                // Check those reserve units for BreakOff and for screen
                for (int b = 0; b < BE::AttShipsLeft; b++)
                {
                    // Get the reserve tag from the ship
                    int reserve = HasReserve(BE::SpecialB[b]);
                    // Do the defenders have a screen?
                    if (!DefHasScreen)
                    {
                        // Does the ship have a reserve value?
                        if (reserve >= 0)
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Unit leaving reserve because there is no screen" << endl;
#endif
                            // Remove the reserve tag
                            BE::SpecialB[b] = RemoveTag(BE::SpecialB[b], "RESERVE", 1);
                        }
                    }
                    else
                    {
                        // Is the reserve tag equal to or less than the defenders BreakOff?
                        if (BE::BO_Def >= reserve && reserve >= 0)
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Unit leaving reserve because reserve is less than break off" << endl;
#endif
                            // Remove the reserve tag
                            BE::SpecialB[b] = RemoveTag(BE::SpecialB[b], "RESERVE", 1);
                        }
                    }
                }
            }

            // UNKNOWN: Resetting AttackIndexes?
            // {{659}}
            if (BE::AttacksIndex > 0)
            {
                for (int i = 0; i < BE::AttacksIndex; i++)
                {
                    BE::Attacks[i].AttackID = 0;
                    BE::Attacks[i].TargetID = 0;
                    BE::Attacks[i].Damage = 0;
                    BE::Attacks[i].Weapon = 0;
                    BE::Attacks[i].Special = "";
                }
                BE::AttacksIndex = 0;
            }

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Begin spawning missiles" << endl;
#endif

            // Spawn missiles routine
            TempAttShipsLeft = BE::AttShipsLeft;
            TempDefShipsLeft = BE::DefShipsLeft;
            missile_counter = 0;
            // Loop through a combined total of all remaining attacking and defending ships
            // This is horribley convoluted
            // TODO: make a funciton for this...not sure what it needs to look like
            // {{676}}
            for (int A = 0; A < (BE::AttShipsLeft + BE::DefShipsLeft); A++)
            {
                // Determine ForceID
                // 0 => attackers
                // 1 => defenders
                int B = A;
                ForceID = 0; // TODO: Make this a local scope variable
                // Are we done with attackers?
                if (A >= BE::AttShipsLeft)
                {
                    B = A - BE::AttShipsLeft;
                    ForceID = 1; // Defenders
                }

#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Unit index B " << B << endl;
#endif

                // Reset temp_str
                temp_str = "";

                // Get unit values depending on ForceID
                if (ForceID == 0)
                {
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Doing missile check for " << BE::AttShipStr[B] << "," << BE::SpecialA[B] << endl;
#endif
                    temp_str = BE::SpecialA[B]; // TODO: Make this local to the loop scope.  May not be necessary if a function is built.
                    tmp = BE::CurTorpA[B];      // TODO: Make this local to the loop scope.  May not be necessary if a function is built.
                }
                else
                {
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Doing missile check for " << BE::DefShipStr[B] << "," << BE::SpecialB[B] << endl;
#endif
                    temp_str = BE::SpecialB[B];
                    tmp = BE::CurTorpB[B];
                }

                // Check if the unit is crippled or is suprised.  If either is true, then skip the unit
                if (IsCrippled(temp_str) || IsSurprise(temp_str))
                {
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Unit is crippled or suprised.  Skipping missiles spawning." << endl;
#endif
                    continue;
                }

                // Check for batteries in the special string
                // TODO: Get batteries then do missile test on each bracket?
                // TODO: Separate HasMissileWT and GetMissileStats
                // {{695}}
                if (HasBatteries(temp_str) > 0)
                {
                    // Check if any of the batteries has a `misXXXX` tag
                    if (!HasMissileWT(temp_str))
                    {
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] Unit doesn't have a missile salvo.  Skipping missile spawning." << endl;
#endif
                        // Skip the unit since there is no missile tag in a bracket
                        continue;
                    }

#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Unit has a missile savlo(s)." << endl;
#endif

                    tmp = 0;            // TODO: Make this a local variable
                    int SalvoCount = 0; // FIXME: This might not be local?
                    int sc = 0;         // TODO: Make this local to the loop below
                    // Reset the salvo array
                    for (sc = 0; sc < 200; sc++) // TODO: Replace 200 with constant
                    {
                        BE::Salvos[sc].MissileS = 0;
                        BE::Salvos[sc].DataStr = "";
                    }

                    // Get the first openning weapons bracket
                    old_start = temp_str.find("[");
                    sc = 0; // TODO: Not sure what to do with this yet.
                    // Get the brackets for this unit
                    vector<string> brackets = GetBrackets(temp_str);
                    for (int i = 0; i < brackets.size(); i++)
                    {
                        // Setup the salvo object
                        BE::Salvos[sc].DataStr = brackets[i];
                        BE::Salvos[sc].MissileS = 0;

#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] Creating savlo for: " << brackets[i] << endl;
#endif

                        // Does the bracket have a missile tag?
                        if (HasMissileWT(BE::Salvos[sc].DataStr))
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Salvo has a missile tag" << endl;
#endif
                            // Is the unit in reserve and without an artillery tag?
                            if (HasReserve(temp_str) > 0 && !HasArtilleryWT(BE::Salvos[sc].DataStr))
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Salvo does not have an artillery tag and the unit has a reserve tag.  No missile spawnning" << endl;
#endif
                                // This unit is in reserve and doesn't have an artillery tag
                                continue;
                            }
                            else
                            {

                                // This unit is either NOT in the reserve or HAS an artillery tag
                                // Do the long range checks.
                                if ((BE::AttHasLongRange > 0 || BE::DefHasLongRange > 0) && !HasLongWT(BE::Salvos[sc].DataStr))
                                {
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO][Missile Spawn] Salvo does not have a long range tag and the combat is at long range" << endl;
#endif
                                    // We are at long range BUT this unit does NOT have a long tag.
                                    // Do nothing.
                                    continue;
                                }
                                // Check ROF on launcher
                                else if (!CheckROF(BE::Salvos[sc].DataStr, BE::CombatRound))
                                {
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO][Missile Spawn] Launcher ROF delay is not this turn." << endl;
#endif
                                    continue;
                                }
                                else if (IsOffline(BE::Salvos[sc].DataStr))
                                {
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO][Missile Spawn] Launcher is offline" << endl;
#endif
                                    continue;
                                }
                                else
                                {
                                    // Either we are at standard range or this unit has a long tag.
                                    // Does the unit have ammo?
                                    int ammo = HasAmmoWT(BE::Salvos[sc].DataStr);
                                    if (ammo > CBE::AMMO_EMPTY || ammo == CBE::AMMO_INFINITE)
                                    {
#ifdef CBE_DEBUG
                                        CBE::debugFile << "[INFO] Salvo has ammo" << endl;
#endif
                                        // The bracket has ammo OR doesn't use ammo
                                        // ORIGINAL is VAL(MID$(Salvo(sc).DataStr,2)) which is the numeric value of the substring from position 2 to end of string...I think
                                        // VAL(MID$("[2 mis0041 ammo 1 target 15]")) ==> VAL("2 mis0041 ammo 1 target 15]") ==> 2 (Wow...I really dislike QBASIC)
                                        // Essentially it grabs the number at the beginning of the string.  VAL might only return the first number it finds?
                                        // So, we get to do this the hard way for now because I'm not going to learn C++ regex right now.  See TODO below.
                                        // TODO: Convert this mess to a regex extraction of the first number in the string
                                        int start = BE::Salvos[sc].DataStr.find("[");
                                        // If start is npos then there is no "[" in the string.  So the first item in the string should be the size of the salvo.
                                        if (start == string::npos)
                                        {
                                            start = 0;
                                        }
                                        else
                                        {
                                            start++;
                                        }
                                        string sizeStr = BE::Salvos[sc].DataStr.substr(start); // Grab everything past the openning bracket or the start of string
                                        int size = stoi(sizeStr);                              // This will only grab the first number in the string and it must start with a string
#ifdef CBE_DEBUG
                                        CBE::debugFile << "[INFO] Salvo Size: " << size << endl;
#endif
                                        BE::Salvos[sc].MissileS = size; // This is simultaneously the number of missiles to launch and the volley size for non-missile salvos
                                        // BE::Salvos[sc].DataStr = RemoveTag(BE::Salvos[sc].DataStr, "ROF", 2);
                                    }
                                    /*else if (ammo == CBE::AMMO_INFINITE)
                                    {
                                        // The bracket doesn't need ammo
                                        // Do the same things as above....which is a mess
                                        // TODO: Redo logic to collapse these two paths
                                        int start = BE::Salvos[sc].DataStr.find("[");
                                        // If start is npos then there is no "[" in the string.  So the first item in the string should be the size of the salvo.
                                        if (start == string::npos)
                                        {
                                            start = 0;
                                        }
                                        else
                                        {
                                            start++;
                                        }
                                        string sizeStr = BE::Salvos[sc].DataStr.substr(start); // Grab everything past the openning bracket or the start of string
                                        int size = stoi(sizeStr);                              // This will only grab the first number in the string and it must start with a string
#ifdef CBE_DEBUG
                                        CBE::debugFile << "[INFO] Salvo Size: " << size << endl;
#endif
                                        BE::Salvos[sc].MissileS = size; // This is simultaneously the number of missiles to launch and the volley size for non-missile salvos
                                        BE::Salvos[sc].DataStr = RemoveTag(BE::Salvos[sc].DataStr, "ROF", 2);
                                    }//*/
#ifdef CBE_DEBUG
                                    else
                                    {
                                        CBE::debugFile << "[INFO] Salvo is out of ammo" << endl;
                                    }
#endif
                                }
                            }
                        }

                        // Increment the salvo count
                        sc = sc + 1;
                    }

                    // Loop through the salvo count and spawn a missile 'unit' for each entity
                    SalvoCount = sc;
                    // TODO: Clean up the reused sc variable.  Poor thing, mistreated like that.
                    for (sc = 0; sc < SalvoCount; sc++)
                    {
                        // Does the salvo have a size greater than 0?  TODO: Move this check to the loop above?  Might mess with the [0] tags on units that shouldn't flee.  Maybe BRAVE tag.
                        int size = BE::Salvos[sc].MissileS;
                        if (size > 0)
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Spawning " << size << " missiles" << endl;
                            CBE::debugFile << "[INFO] " << size + TempAttShipsLeft + TempDefShipsLeft << " total units" << endl;
#endif
                            // The salvo has a size.  Do all the things!!!
                            // But first, check to make sure the salvo size won't overflow the 9999 max unit limit
                            // TODO: Setup a preprocessor variable for maximum number of units
                            if (size + TempAttShipsLeft + TempDefShipsLeft > 9999)
                            {
                                // There will be a problem.  We should throw an exception here rather than just exiting
                                // TODO: Throw an exception
                                cerr << "The 9999 ship limit has been exceeded while spawning missiles.  Simulation aborted." << endl;
                                exit(1); // TODO: Define error codes...and more constants for consistancy and clarity.
                            }

                            // Check for a missile tag and get the BSTH values from the tag
                            // TODO: Add a flags member to the SalvoInfo struct so these tests don't need to be constantly done over and over and over and over again...
                            // This also sets BE::MissileB, BE::MissileS, BE::MissileT, and BE:MissileH
                            const string &special = BE::Salvos[sc].DataStr; // I got tired of accessing this variable in every function call....
                            HasMissileWT(special);                          // FIXME: I can't describe how much I hate this line.
                            // Check to see if the salvo need ammo.  See todo above...damn.
                            int ammo = HasAmmoWT(special);
                            missile_str.clear();
                            if (ammo > CBE::AMMO_EMPTY)
                            {
                                NewTag = "ammo " + to_string((ammo - 1)); // Decrement the ammo counter.  TODO: Make this a local variable
                                // NewTag += " ";                                           // Had to make this a new line do to type converstion of the (ammo -1) expression.
                                temp_str = RemoveTag(BE::Salvos[sc].DataStr, "ammo", 1); // FIXME: This is so clunky to update the string in the middle of the round
                                temp_str = AddTag(temp_str, NewTag);
                                BE::Salvos[sc].DataStr = temp_str; // FIXME: And this breaks my pointer above?  Maybe not.  We'll see!!!
                                // Remove the ammo string from missiles so there are no 'duds'
                                missile_str = RemoveTag(BE::Salvos[sc].DataStr, "ammo", 1); // Remove the ammo tag and save as missile_str.  TODO: Local?  I'm not sure where this is used next.
                                missile_str = RemoveTag(missile_str, "shots", 1);           // Remove the shots tag because redundancy
                            }
                            else if (ammo == CBE::AMMO_INFINITE)
                            {
                                missile_str = BE::Salvos[sc].DataStr; // Just use the salve string since there is no ammo
                            }

                            missile_str = RemoveTag(missile_str, "rof", 2);
                            missile_str = RemoveTag(missile_str, "offline", 0);

                            // Modify missile_str here instead of below when building the missile units
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] missile_str(old): \"" << missile_str << "\"" << endl;
#endif
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
                            int start = missile_str.find(" ");
                            missile_str = "[" + to_string(BE::MissileB + BE::MissileT) + missile_str.substr(start); // Combine Beam and Torp from missile battery for total damage FIXME: This assumes that the string starts with a battery tag and has a closing ']'
                            missile_str = RemoveTag(missile_str, "mis", 0);                                         // Remove the `mis` tag or missiles will spawn missiles.  Could be used for submunitions...
                            missile_str = AddTag(missile_str, "MSL");                                               // Denotes the unit is a missile
                            missile_str = AddTag(missile_str, "SUICIDE");                                           // Suicide denotes that the unit should be removed at the end of combat
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] missile_str(new): \"" << missile_str << "\"" << endl;
#endif

                            // Spawn those babies...
                            for (int i = 0; i < size; i++)
                            {
                                missile_counter = missile_counter + 1; // This is a global variable that was zeroed at the start of this section.  This tracks all missiles launched from both sides?
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Spawnning missile " << missile_counter << endl;
#endif
                                // Check the ForceID...TODO: Get rid of ForceID so that multiple fleets can be added.  Not to mention factions
                                // TODO: Make a spawn missile function
                                if (ForceID == 0)
                                {
                                    // VB sucks.  Where do MissileB, MissileS, MissileT, and Missile H come from?  From the call to HasMissileWT() about 20 lines up!
                                    // [JLL] This block of code makes me want to vomit.  No wonder tags jump between units.  Argh!!
                                    BE::AttShipStr[TempAttShipsLeft] = "missile " + to_string(missile_counter); // Global missile names too...
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
                                    TempAttShipsLeft = TempAttShipsLeft + 1;
                                }
                                else
                                {
                                    // Do the other foce
                                    // [JLL] TODO: Make this a function
                                    BE::DefShipStr[TempDefShipsLeft] = "missile " + to_string(missile_counter); // Global missile names too...
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
                                    TempDefShipsLeft = TempDefShipsLeft + 1;
                                }
                            }
                        }
                    }

                    // Reassemble weapon tags back into a unit's appropriate Special tag
                    // Get the Specail tag from the special array.  Again with the ForceID junk.  Functions are your friend...
                    // [JLL] This is why string manipulation is bad.  A well built class or struct will render a lot of this code useless.
                    // Check ForceID
                    // TODO: Replace this with a call to RebuildBatteryTags
                    if (ForceID == 0)
                    {
                        temp_str = BE::SpecialA[B]; // [JLL] TODO: Stop global variable abuse!
                    }
                    else
                    {
                        temp_str = BE::SpecialB[B];
                    }

                    Strip = false;
                    new_str = "";

                    // Not entirely sure what this is for.  Might be for removing the [] from the string?
                    // Or does it remove the first salvo from the string?
                    // [JLL] This removes all salvos from the string!  Because they will be rebuilt from the Salvos array below.
                    // [JLL] This builds a new string for the unit, sans salvos, character by character.  Vomit inducing...
                    for (swt = 0; swt < temp_str.size(); swt++)
                    {
                        if (temp_str[swt] == '[')
                        {
                            Strip = true;
                        }
                        if (Strip == false)
                        {
                            new_str = new_str + temp_str[swt];
                        }
                        if (temp_str[swt] == ']')
                        {
                            Strip = false;
                        }
                    }

                    // This builds a new string of Salvos from the Salvos array?  But why?
                    // Also, 200 salvos for the entire simulation...that isn't enough for one fleet!
                    // See the code below this block.  The salvos array is per unit, I think.
                    // [JLL] The Salvos array is per unit.
                    // [JLL] FIXME: This is why salvos jump units some times.  This loops through all 200 Salvos.
                    for (sc = 0; sc < 200; sc++)
                    {
                        if (BE::Salvos[sc].DataStr == "")
                        {
                            break;
                        }
                        new_str = BE::Salvos[sc].DataStr + new_str;
                    }

                    // Assign the new specials string to the SpecialA or SpecialB array for the unit in question depending on ForceID
                    // [JLL] FIXME: This explains why the salvo string jumps from attackers to defenders and vice versa.
                    if (ForceID == 0)
                    {
                        BE::SpecialA[B] = new_str;
                    }
                    else
                    {
                        BE::SpecialB[B] = new_str;
                    }
                } // End of HasBatteries If Block
                else
                {
                    // This unit has no batteries.
                    // [JLL] TODO: Discontinue use of non-battery weapons
                    // Check unit torpedo rating and for Missile
                    // [JLL] I am skipping this for now pending response from the FOTS group - 2023-01-11
#ifdef CBE_DEBUG
                    CBE::debugFile << "[WARNING] Unit has no batteries" << endl;
#endif
                }
            } // End of spawn missile loop....

            // [JLL] Since missiles are added at the end of the units array the code below keeps other units from targeting them.
            AttNumValidTargets = BE::AttShipsLeft;
            DefNumValidTargets = BE::DefShipsLeft;
            BE::AttShipsLeft = TempAttShipsLeft; // [JLL] Need to keep track of missiles
            BE::DefShipsLeft = TempDefShipsLeft; // [JLL] Need to keep track of missiles

            // Copy real values to temp fields so that combat will be fair
            for (int i = 0; i < BE::AttShipsLeft; i++)
            {
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
            for (int i = 0; i < BE::DefShipsLeft; i++)
            {
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
#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Beginning the attack loop" << endl;
#endif
            for (BE::A = 0; BE::A < (BE::AttShipsLeft + BE::DefShipsLeft); BE::A++)
            {
                // The same BS about ForceID...
                int B = BE::A;
                ForceID = 0;
                if (BE::A >= BE::AttShipsLeft)
                {
                    B = BE::A - BE::AttShipsLeft;
                    ForceID = 1;
                }
                if (BE::A == BE::AttShipsLeft)
                {
                    // Print a space in the report file
                    reportFile << endl;
                }

                Special1 = 0;
                Special2 = 0;
                FirePower1 = 0;
                FirePower2 = 0;

#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO] Attack Loop Index B " << B << endl;
#endif

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
                for (int i = 0; i < 200; i++)
                {
                    Hits[i].firepower = 0;
                    Hits[i].special = 0;
                    Hits[i].tag = "";
                    Hits[i].valid = true;
                }
                number_of_attacks = 0; // Assume the attacker has no weapons

                // Determine eligibility and the number of attacks (if any)
                switch (ForceID)
                {
                case 0: // Attacking fleet
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Attack Loop for: " + BE::AttRaceName + " " + BE::AttShipStr[B] << endl;
#endif

                    if (BE::AttShipStr[B].find("missile") != string::npos)
                    {
                        // [JLL] Why the hell do we care if the name of the unit has "missile" in it?
                        BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " is a drone or decoy."; // Check the name of the ship for missile?  Assume it is a drone or decoy.
                    }
                    else
                    {
                        BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " did not fire beams or torps."; // If missile is not in the name then the unit did not fire? [JLL] This is the default state for a unit, the unit did not fire.
                    }
                    // [JLL] I am so confused by the if/else blocks above.  TODO: Can I remove these?

                    // Check if the unit is NOT drifting AND NOT nomove
                    if (!IsDrifting(BE::SpecialA[B]) && !IsNoMove(BE::SpecialA[B]))
                    {
                        // The unit is neither drifting nor nomove
                        // Check if the unit is fleeing
                        if (IsFlee(BE::SpecialA[B]))
                        {
                            // The unit is fleeing.  Remove flee and add fled
                            BE::TempSpecialA[B] = RemoveTag(BE::TempSpecialA[B], "FLEE", 0);
                            BE::TempSpecialA[B] = AddTag(BE::TempSpecialA[B], "FLED");
                            // Check for a DEFENSE tag
                            int defense = HasDefense(BE::TempSpecialA[B]);
                            if (defense != std::numeric_limits<int>::min())
                            {                                                                       // TODO: Replace the limit call with a constant for clarity and consistancy
                                BE::TempSpecialA[B] = RemoveTag(BE::TempSpecialA[B], "DEFENSE", 1); // Remove the defense tag
                            }
                            NewTag = "DEFENSE " + int(defense + BaseAccuracy / 2); // TODO: Replace with local variable.
                            BE::TempSpecialA[B] = AddTag(BE::TempSpecialA[B], NewTag);

                            // Check if the unit has a TARGET tag
                            int target = HasTarget(BE::TempSpecialA[B]);
                            if (target != std::numeric_limits<int>::min())
                            { // TODO: Replace the limit::min call with a constant
                                BE::TempSpecialA[B] = RemoveTag(BE::TempSpecialA[B], "TARGET", 1);
                            }
                            NewTag = "TARGET " + int(target + BaseAccuracy / 2); // TODO: Replace with local variable.  TODO: Replace BaseAccuracy/2 with a constant
                            BE::TempSpecialA[B] = AddTag(BE::TempSpecialA[B], NewTag);
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Checking unit \"" << BE::AttShipStr[B] << "\" is fleeing." << endl;
                            CBE::debugFile << "[INFO] Old Special String: \"" << BE::SpecialA[B] << "\"" << endl;
                            CBE::debugFile << "[INFO] New Special String: \"" << BE::TempSpecialA[B] << "\"" << endl;
#endif
                            BE::SpecialA[B] = BE::TempSpecialA[B]; // Replace the unit's special string NOTE: Why is this replaced now?
                        }
                    }
                    // Check if the unit is drifting
                    // NOTE: [JLL] This appears to be linked with the flee/fled check above.  Meaning, a unit can not flee if it is drifting.  Not really connected to the checks below to see if the unit is skipped.
                    if (IsDrifting(BE::SpecialA[B]))
                    {
                        // Remove the DRIFTING tag.  NOTE: This must be a transitory tag
                        BE::TempSpecialA[B] = RemoveTag(BE::TempSpecialA[B], "DRIFTING", 0);
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] " << BE::AttShipStr[B] << " is no longer drifting." << endl;
#endif
                    }
                    // Check if the unit is surprised!
                    if (IsSurprise(BE::SpecialA[B]))
                    {
                        // Print the unit is suprised message
                        BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " is surprised!";
                        // Skip the rest of this unit's round
                        break; // TODO: This might need to be continue but I don't think so.
                    }
                    // Check if the unit is captured! TODO: Have the capture mechanic move the unit to the opposing fleet
                    if (IsCaptured(BE::SpecialA[B]))
                    {
                        // Print the unit has been captured
                        BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " has been captured and can not attack!";
                        // Skip the rest of this unit's round
                        break;
                    }
                    // Check if the unit is crippled!
                    if (IsCrippled(BE::SpecialA[B]))
                    {
                        // Print the unit is crippled
                        BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " is crippled and can not attack!";
                        // Skip the rest of this unit's round
                        break;
                    }
                    // Check if the defending fleet has a cloak
                    if (BE::DefIsCloaked == 1)
                    {
                        // TODO: This needs to be generalized to are there targets availabled?  Move to target selection code!
                        BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " can not lock onto a target!";
                        // Skip the rest of the unit's round
                        break;
                    }
                    // This is the start of the battery processing
                    // Check if the unit has batteries
                    if (HasBatteries(BE::SpecialA[B]))
                    {
                        // Check if the unit is in the RESERVE and NOT ARTILLERY
                        if (HasReserve(BE::SpecialA[B]) > 0 && !HasArtilleryWT(BE::SpecialA[B]))
                        {
                            // The unit can not fire from the reserve
                            BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " is being held in reserve!";
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] " + BE::AttRaceName + " " + BE::AttShipStr[B] + " is being held in reserve" << endl;
#endif
                            // Skip the rest of the unit's round
                            break;
                        }

                        // Getting the bracket attacks and adding them to the Salvos array
                        number_of_attacks = 0;      // TODO: Replace with a local variable
                        temp_str = BE::SpecialA[B]; // TODO: Replace with a local variable
                        // old_start = temp_str.find("["); // TODO: Replace with a local variable
                        old_start = 0;
                        sc = 0; // TODO: Replace with a local variable?  This may be used later
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] " << BE::AttRaceName << " " << BE::AttShipStr[B] << " looking for batteries: " << temp_str << endl;
#endif
                        // Reset all salvo objects
                        for (int i = 0; i < 200; i++)
                        {
                            BE::Salvos[i].DataStr.clear();
                            BE::Salvos[i].MissileS = 0;
                            BE::Salvos[i].valid = true;
                        }

                        // TODO: Turn the code below into a function?  Use GetBrackets()
                        while (old_start != string::npos)
                        {
                            start = temp_str.find("[", old_start); // TODO: Replace with a local variable
                            start1 = temp_str.find("]", start);    // TODO: Replace with a local variable
                            // NOTE: [JLL] Not sure what the below does
                            if (start1 == string::npos)
                            {
                                old_start = int(string::npos); // Set to no position so that the loop stops NOTE: [JLL] Explicit typecast to int causes this to be -1.
                                start1 = temp_str.size();      // Get the whole string since there is no closing bracket TODO: This should be handled at load with a validation check.
                            }
                            else
                            {
                                old_start = temp_str.find("[", start1); // Find the start of the next salvo string
                            }
                            // Save the bracket string
                            BE::Salvos[sc].DataStr = temp_str.substr(start, start1 - start + 1);
                            BE::Salvos[sc].MissileS = stoi(temp_str.substr(1));
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] " << BE::AttRaceName << " " << BE::AttShipStr[B] << " has battery[" << sc << "]: " << BE::Salvos[sc].DataStr << endl;
#endif
                            // Increment the salvo count
                            sc = sc + 1;
                        }

#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] Number of salvos " << sc << endl;
#endif
                        // Did we find any salvos?
                        if (sc > 0)
                        {
                            for (int i = 0; i < sc; i++)
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Determining hits for: \"" << BE::Salvos[i].DataStr << "\"" << endl;
#endif
                                // Do everything we can to set the firepower to zero [JLL] Meaning, if there is a reason to set the firepower to 0 do so.  Such as missile, lack long, ROF, offline, etc.
                                // Check if this is a missile battery which was taken care of earlier
                                int ammo = HasAmmoWT(BE::Salvos[i].DataStr);
                                if (HasMissileWT(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0; // TODO: Can I move these checks to building the salvo array above?
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Is a missile battery.  That was handled earlier." << endl;
#endif
                                }
                                // Check if the battery is offline
                                else if (IsOffline(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is offline." << endl;
#endif
                                }
                                // Check for a firing delay in the ROF tag
                                else if (GetROFDelayWT(BE::Salvos[i].DataStr) > 0)
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] ROF delay is not this round" << endl;
#endif
                                }
                                // Check if only LR attacks are valid and if the battery is NOT long
                                else if ((BE::AttHasLongRange > 0 || BE::DefHasLongRange > 0) && !HasLongWT(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is not long range" << endl;
#endif
                                }
                                // Check for reserve and artillery tags
                                // TODO: Can't this be removed?  The checks before salvos are built should skip this
                                else if (HasReserve(BE::SpecialA[B]) > 0 && !HasArtilleryWT(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is not artillery and unit is in reserve" << endl;
#endif
                                }
                                // Check if the battery needs ammo and has some
                                else if (ammo == CBE::AMMO_EMPTY)
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is out of ammo" << endl;
#endif
                                }

                                // Cycle weapons updating counters and removing offline tags
                                // TODO: Check for offline tag first?
                                BE::Salvos[i].DataStr = RemoveTag(BE::Salvos[i].DataStr, "offline", 0);
                                // Update the ROF tag if it is present
                                int rate = GetROFRateWT(BE::Salvos[i].DataStr);
                                if (rate > 0)
                                {
                                    // Weapon is cycling using ROF rate
                                    int delay = GetROFDelayWT(BE::Salvos[i].DataStr);
                                    string newROF = "";
                                    if (delay > 0)
                                    {
                                        // Compute a new ROF tag by decrementing the delay value by 1.
                                        newROF = "rof " + to_string(rate) + " " + to_string(delay - 1);
                                    }
                                    else
                                    {
                                        // Compute a new ROF tag by setting the delay value to the rate value
                                        newROF = "rof " + to_string(rate) + " " + to_string(rate - 1); // Minus 1 because a rate of 2 is fire every other turn
                                    }
                                    string temp = RemoveTag(BE::Salvos[i].DataStr, "rof", 2); // Remove the old ROF tag
                                    temp = AddTag(temp, newROF);                              // Add the new ROF tag
                                    BE::Salvos[i].DataStr = temp;
                                }

                                // [JLL] Now that the salvo strings have been updated (offline & rof)
                                // [JLL] rebuild the battery tags in the unit special string
                                // [JLL] FIXME: This should only happen at the end of processing this unit.
                                string temp = RebuildBatteryTags(BE::TempSpecialA[B], BE::Salvos, sc);
                                BE::TempSpecialA[B] = temp;
                                BE::SpecialA[B] = temp;

                                // Does the salvo have firepower?
                                if (BE::Salvos[i].MissileS > 0)
                                {
                                    // Flag the salvo as valid
                                    BE::Salvos[i].valid = true;
                                    // Does the battery have FLAK and does the defense have fighters?
                                    if (HasFlakWT(BE::Salvos[i].DataStr) && BE::DefHasFighters > 0)
                                    {
                                        // Create the flak packets
                                        for (int j = 0; j < BE::Salvos[i].MissileS; j++)
                                        {
                                            Hits[number_of_attacks].firepower = 1;                                                                // Fill out the hits array, with the firepower split into packets of 1
                                            Hits[number_of_attacks].special = BE::saMulti;                                                        // Batteries are multi-targetting
                                            Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, Hits[number_of_attacks].special); // Combine any bitwise flags
                                            Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                                                  // Set the tags for the `hit` [JLL] Really this is a possible hit at this point.  More like shots.
                                            number_of_attacks = number_of_attacks + 1;                                                            // Increment the number of attacks
                                        }
                                    }
                                    else if (HasMultiWT(BE::Salvos[i].DataStr) > 0)
                                    { // TODO: Use a constant here
                                        // This is a multi attack
                                        packet_size = HasMultiWT(BE::Salvos[i].DataStr); // TODO: Convert to local variable TODO: Differentiate HasMulti and GetMulti functions
                                        for (int j = 0; j < int(BE::Salvos[i].MissileS / packet_size); j++)
                                        {
                                            Hits[number_of_attacks].firepower = packet_size;
                                            Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, BE::saMulti); // Start with multi because batteries
                                            Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                              // Set the tags for the hit
                                            number_of_attacks = number_of_attacks + 1;                                        // Incremet the number of attacks
                                        }
                                        // Check for left over firepower due to packet sizing
                                        if ((BE::Salvos[i].MissileS % packet_size) > 0)
                                        {
                                            // Yep, there is a remainder
                                            Hits[number_of_attacks].firepower = BE::Salvos[i].MissileS % packet_size;
                                            Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, BE::saMulti); // Start with multi because batteries
                                            Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                              // Set the tags for the hit
                                            number_of_attacks = number_of_attacks + 1;                                        // Incremet the number of attacks
                                        }
                                    }
                                    else
                                    {
                                        // If it is not flak and not multi then it must be a single target attack!
                                        Hits[number_of_attacks].firepower = BE::Salvos[i].MissileS;                       // Set the firepower from the salvo
                                        Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, BE::saMulti); // Start with multi because batteries
                                        Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                              // Set the tags for the hit
                                        number_of_attacks = number_of_attacks + 1;                                        // Incremet the number of attacks
                                    }
                                }

#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] (Att) Salvo has no firepower" << endl;
#endif

                                // Check for ammo
                                ammo = HasAmmoWT(BE::Salvos[i].DataStr);
                                if (!HasMissileWT(BE::Salvos[i].DataStr) && ammo != CBE::AMMO_INFINITE && ammo > CBE::AMMO_EMPTY)
                                {
                                    // We are using ammo
                                    // Create a new ammo tag
                                    string newTag = "ammo " + to_string((ammo - 1));
                                    string tempStr = RemoveTag(BE::Salvos[i].DataStr, "ammo", 1);
                                    tempStr = AddTag(tempStr, newTag);
                                    BE::Salvos[i].DataStr = tempStr;

                                    // Reassemble the weapon tags again for the unit
                                    // TODO: Why are we doing this again so soon?  Why not at the end?  Need to check the logic in the hit building above.
                                    string temp = RebuildBatteryTags(BE::TempSpecialA[B], BE::Salvos, sc);
                                    BE::TempSpecialA[B] = temp;
                                    BE::SpecialA[B] = temp;
                                }
                            }
                        }
                    }
                    break;
                case 1: // Defending fleet //////////////////////////////////////////////////////////////////////////////////////////////
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Attack Loop for: " + BE::DefRaceName + " " + BE::DefShipStr[B] << endl;
#endif
                    if (BE::DefShipStr[B].find("missile") != string::npos)
                    {
                        // [JLL] Why the hell do we care if the name of the unit has "missile" in it?
                        BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " is a drone or decoy."; // Check the name of the ship for missile?  Assume it is a drone or decoy.
                    }
                    else
                    {
                        BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " did not fire beams or torps."; // If missile is not in the name then the unit did not fire?
                    }
                    // [JLL] I am so confused by the if/else blocks above.  TODO: Can I remove these?

                    // Check if the unit is NOT drifting AND NOT nomove
                    if (!IsDrifting(BE::SpecialB[B]) && !IsNoMove(BE::SpecialB[B]))
                    {
                        // The unit is neither drifting nor nomove
                        // Check if the unit is fleeing
                        if (IsFlee(BE::SpecialB[B]))
                        {
                            // The unit is fleeing.  Remove flee and add fled
                            BE::TempSpecialB[B] = RemoveTag(BE::TempSpecialB[B], "FLEE", 0);
                            BE::TempSpecialB[B] = AddTag(BE::TempSpecialB[B], "FLED");
                            // Check for a DEFENSE tag
                            int defense = HasDefense(BE::TempSpecialB[B]);
                            if (defense != std::numeric_limits<int>::min())
                            {                                                                       // TODO: Replace the limit call with a constant for clarity and consistancy
                                BE::TempSpecialB[B] = RemoveTag(BE::TempSpecialB[B], "DEFENSE", 1); // Remove the defense tag
                            }
                            NewTag = "DEFENSE " + int(defense + BaseAccuracy / 2); // TODO: Replace with local variable.
                            BE::TempSpecialB[B] = AddTag(BE::TempSpecialB[B], NewTag);

                            // Check if the unit has a TARGET tag
                            int target = HasTarget(BE::TempSpecialB[B]);
                            if (target != std::numeric_limits<int>::min())
                            { // TODO: Replace the limit::min call with a constant
                                BE::TempSpecialB[B] = RemoveTag(BE::TempSpecialB[B], "TARGET", 1);
                            }
                            NewTag = "TARGET " + int(target + BaseAccuracy / 2); // TODO: Replace with local variable.  TODO: Replace BaseAccuracy/2 with a constant
                            BE::TempSpecialB[B] = AddTag(BE::TempSpecialB[B], NewTag);
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Checking unit \"" << BE::DefShipStr[B] << "\" is fleeing." << endl;
                            CBE::debugFile << "[INFO] Old Special String: \"" << BE::SpecialB[B] << "\"" << endl;
                            CBE::debugFile << "[INFO] New Special String: \"" << BE::TempSpecialB[B] << "\"" << endl;
#endif
                            BE::SpecialB[B] = BE::TempSpecialB[B]; // Replace the unit's special string NOTE: Why is this replaced now?
                        }
                    }
                    // Check if the unit is drifting
                    // NOTE: [JLL] This appears to be linked with the flee/fled check above.  Meaning, a unit can not flee if it is drifting.  Not really connected to the checks below to see if the unit is skipped.
                    if (IsDrifting(BE::SpecialB[B]))
                    {
                        // Remove the DRIFTING tag.  NOTE: This must be a transitory tag
                        BE::TempSpecialB[B] = RemoveTag(BE::TempSpecialB[B], "DRIFTING", 0);
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] " << BE::DefShipStr[B] << " is no longer drifting." << endl;
#endif
                    }
                    // Check if the unit is surprised!
                    if (IsSurprise(BE::SpecialB[B]))
                    {
                        // Print the unit is suprised message
                        BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " is surprised!";
                        // Skip the rest of this unit's round
                        break; // TODO: This might need to be continue but I don't think so.
                    }
                    // Check if the unit is captured! TODO: Have the capture mechanic move the unit to the opposing fleet
                    if (IsCaptured(BE::SpecialB[B]))
                    {
                        // Print the unit has been captured
                        BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " has been captured and can not attack!";
                        // Skip the rest of this unit's round
                        break;
                    }
                    // Check if the unit is crippled!
                    if (IsCrippled(BE::SpecialB[B]))
                    {
                        // Print the unit is crippled
                        BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " is crippled and can not attack!";
                        // Skip the rest of this unit's round
                        break;
                    }
                    // Check if the defending fleet has a cloak
                    if (BE::DefIsCloaked == 1)
                    {
                        // TODO: This needs to be generalized to are there targets availabled?  Move to target selection code!
                        BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " can not lock onto a target!";
                        // Skip the rest of the unit's round
                        break;
                    }
                    // This is the start of the battery processing
                    // Check if the unit has batteries
                    if (HasBatteries(BE::SpecialB[B]))
                    {
                        // Check if the unit is in the RESERVE and NOT ARTILLERY
                        if (HasReserve(BE::SpecialB[B]) > 0 && !HasArtilleryWT(BE::SpecialB[B]))
                        {
                            // The unit can not fire from the reserve
                            BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " is being held in reserve!";
                            // Skip the rest of the unit's round
                            break;
                        }

                        // Getting the bracket attacks and adding them to the Salvos array
                        number_of_attacks = 0;      // TODO: Replace with a local variable
                        temp_str = BE::SpecialB[B]; // TODO: Replace with a local variable
                        // old_start = temp_str.find("["); // TODO: Replace with a local variable
                        old_start = 0;
                        sc = 0; // TODO: Replace with a local variable?  This may be used later
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] " << BE::DefRaceName << " " << BE::DefShipStr[B] << " looking for batteries: " << temp_str << endl;
#endif
                        // Reset all salvo objects
                        for (int i = 0; i < 200; i++)
                        {
                            BE::Salvos[i].DataStr.clear();
                            BE::Salvos[i].MissileS = 0;
                            BE::Salvos[i].valid = true;
                        }

                        // TODO: Turn the code below into a function?
                        while (old_start != string::npos)
                        {
                            start = temp_str.find("[", old_start); // TODO: Replace with a local variable
                            start1 = temp_str.find("]", start);    // TODO: Replace with a local variable
                            // NOTE: [JLL] Not sure what the below does
                            if (start1 == string::npos)
                            {
                                old_start = int(string::npos); // Set to no position so that the loop stops NOTE: [JLL] Explicit typecast to int causes this to be -1.
                                start1 = temp_str.size();      // Get the whole string since there is no closing bracket TODO: This should be handled at load with a validation check.
                            }
                            else
                            {
                                old_start = temp_str.find("[", start1); // Find the start of the next salvo string
                            }
                            // Save the bracket string
                            BE::Salvos[sc].DataStr = temp_str.substr(start, start1 - start + 1);
                            BE::Salvos[sc].MissileS = stoi(temp_str.substr(1));
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] " << BE::DefRaceName << " " << BE::DefShipStr[B] << " has battery[" << sc << "]: " << BE::Salvos[sc].DataStr << endl;
#endif
                            // Increment the salvo count
                            sc = sc + 1;
                        }

#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] Number of salvos " << sc << endl;
#endif

                        // Did we find any salvos?
                        if (sc > 0)
                        {
                            for (int i = 0; i < sc; i++)
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Determining hits for: \"" << BE::Salvos[i].DataStr << "\"" << endl;
#endif
                                // Do everything we can to set the firepower to zero [JLL] Meaning, if there is a reason to set the firepower to 0 do so.  Such as missile, lack long, ROF, offline, etc.
                                // Check if this is a missile battery which was taken care of earlier
                                int ammo = HasAmmoWT(BE::Salvos[i].DataStr);
                                if (HasMissileWT(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0; // TODO: Can I move these checks to building the salvo array above?
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Is a missile battery.  That was handled earlier." << endl;
#endif
                                }
                                // Check if the battery is offline
                                else if (IsOffline(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is offline." << endl;
#endif
                                }
                                // Check for a firing delay in the ROF tag
                                else if (GetROFDelayWT(BE::Salvos[i].DataStr) > 0)
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] ROF delay is not this round" << endl;
#endif
                                }
                                // Check if only LR attacks are valid and if the battery is NOT long
                                else if ((BE::AttHasLongRange > 0 || BE::DefHasLongRange > 0) && !HasLongWT(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is not long range" << endl;
#endif
                                }
                                // Check for reserve and artillery tags
                                // TODO: Can't this be removed?  The checks before salvos are built should skip this
                                else if (HasReserve(BE::SpecialB[B]) > 0 && !HasArtilleryWT(BE::Salvos[i].DataStr))
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is not artillery and unit is in reserve" << endl;
#endif
                                }
                                // Check if the battery needs ammo and has some
                                else if (ammo == CBE::AMMO_EMPTY)
                                {
                                    BE::Salvos[i].MissileS = 0;
                                    BE::Salvos[i].valid = false;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Battery is out of ammo" << endl;
#endif
                                }

                                // Cycle weapons updating counters and removing offline tags
                                // TODO: Check for offline tag first?
                                BE::Salvos[i].DataStr = RemoveTag(BE::Salvos[i].DataStr, "offline", 0);
                                // Update the ROF tag if it is present
                                int rate = GetROFRateWT(BE::Salvos[i].DataStr);
                                if (rate > 0)
                                {
                                    // Weapon is cycling using ROF rate
                                    int delay = GetROFDelayWT(BE::Salvos[i].DataStr);
                                    string newROF = "";
                                    if (delay > 0)
                                    {
                                        // Compute a new ROF tag by decrementing the delay value by 1.
                                        newROF = "rof " + to_string(rate) + " " + to_string(delay - 1);
                                    }
                                    else
                                    {
                                        // Compute a new ROF tag by setting the delay value to the rate value
                                        newROF = "rof " + to_string(rate) + " " + to_string(rate - 1); // Minus 1 because a rate of 2 is fire every other turn
                                    }
                                    string temp = RemoveTag(BE::Salvos[i].DataStr, "rof", 2); // Remove the old ROF tag
                                    temp = AddTag(temp, newROF);                              // Add the new ROF tag
                                    BE::Salvos[i].DataStr = temp;
                                }

                                // [JLL] Now that the salvo strings have been updated (offline & rof)
                                // [JLL] rebuild the battery tags in the unit special string
                                // [JLL] FIXME: This should only happen at the end of processing this unit.
                                string temp = RebuildBatteryTags(BE::TempSpecialB[B], BE::Salvos, sc);
                                BE::TempSpecialB[B] = temp;
                                BE::SpecialB[B] = temp;

                                // Does the salvo have firepower?
                                if (BE::Salvos[i].MissileS > 0)
                                {
                                    // Mark this salvo is valid
                                    BE::Salvos[i].valid = true;
                                    // Does the battery have FLAK and does the defense have fighters?
                                    if (HasFlakWT(BE::Salvos[i].DataStr) && BE::AttHasFighters > 0)
                                    {
                                        // Create the flak packets
                                        for (int j = 0; j < BE::Salvos[i].MissileS; j++)
                                        {
                                            Hits[number_of_attacks].firepower = 1;                                                                // Fill out the hits array, with the firepower split into packets of 1
                                            Hits[number_of_attacks].special = BE::saMulti;                                                        // Batteries are multi-targetting
                                            Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, Hits[number_of_attacks].special); // Combine any bitwise flags
                                            Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                                                  // Set the tags for the `hit` [JLL] Really this is a possible hit at this point.  More like shots.
                                            number_of_attacks = number_of_attacks + 1;                                                            // Increment the number of attacks
                                        }
                                    }
                                    else if (HasMultiWT(BE::Salvos[i].DataStr) > 0)
                                    { // TODO: Use a constant here
                                        // This is a multi attack
                                        packet_size = HasMultiWT(BE::Salvos[i].DataStr); // TODO: Convert to local variable TODO: Differentiate HasMulti and GetMulti functions
                                        for (int j = 0; j < int(BE::Salvos[i].MissileS / packet_size); j++)
                                        {
                                            Hits[number_of_attacks].firepower = packet_size;
                                            Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, BE::saMulti); // Start with multi because batteries
                                            Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                              // Set the tags for the hit
                                            number_of_attacks = number_of_attacks + 1;                                        // Incremet the number of attacks
                                        }
                                        // Check for left over firepower due to packet sizing
                                        if ((BE::Salvos[i].MissileS % packet_size) > 0)
                                        {
                                            // Yep, there is a remainder
                                            Hits[number_of_attacks].firepower = BE::Salvos[i].MissileS % packet_size;
                                            Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, BE::saMulti); // Start with multi because batteries
                                            Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                              // Set the tags for the hit
                                            number_of_attacks = number_of_attacks + 1;                                        // Incremet the number of attacks
                                        }
                                    }
                                    else
                                    {
                                        // If it is not flak and not multi then it must be a single target attack!
                                        Hits[number_of_attacks].firepower = BE::Salvos[i].MissileS;                       // Set the firepower from the salvo
                                        Hits[number_of_attacks].special = SetFlagsWT(BE::Salvos[i].DataStr, BE::saMulti); // Start with multi because batteries
                                        Hits[number_of_attacks].tag = BE::Salvos[i].DataStr;                              // Set the tags for the hit
                                        number_of_attacks = number_of_attacks + 1;                                        // Incremet the number of attacks
                                    }
                                }
#ifdef CBE_DEBUG
                                else
                                {
                                    CBE::debugFile << "[INFO] (DEF) Salvo has no firepower" << endl;
                                }
#endif

                                // Check for ammo
                                ammo = HasAmmoWT(BE::Salvos[i].DataStr);
                                if (!HasMissileWT(BE::Salvos[i].DataStr) && ammo != CBE::AMMO_INFINITE && ammo > CBE::AMMO_EMPTY)
                                {
                                    // We are using ammo
                                    // Create a new ammo tag
                                    string newTag = "ammo " + to_string((ammo - 1));
                                    string tempStr = RemoveTag(BE::Salvos[i].DataStr, "ammo", 1);
                                    tempStr = AddTag(tempStr, newTag);
                                    BE::Salvos[i].DataStr = tempStr;

                                    // Reassemble the weapon tags again for the unit
                                    // TODO: Why are we doing this again so soon?  Why not at the end?  Need to check the logic in the hit building above.
                                    string temp = RebuildBatteryTags(BE::TempSpecialB[B], BE::Salvos, sc);
                                    BE::TempSpecialB[B] = temp;
                                    BE::SpecialB[B] = temp;
                                }
                            }
                        }
                    }
                    break;
                }
                // ROLL ATTACKS
                HasRaided = 0;
                if (number_of_attacks == 0)
                {
// Skip the rest of the unit's round
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Unit has no attackes! Skipping rest of unit's turn." << endl;
#endif
                    if (ForceID == 0)
                    {
                        // TODO: Change AttBattleStr and DefBattleStr to a single local variable.  Unless these get added to later in the loop. (Written at the Salvos point)
                        if (BE::AttBattleStr != "")
                        {
                            reportFile << BE::AttBattleStr << "\n";
                        }
                    }
                    else
                    {
                        if (BE::DefBattleStr != "")
                        {
                            reportFile << BE::DefBattleStr << "\n";
                        }
                    }
                    continue;
                }
                // Handle the attacks
                for (int i = 0; i < number_of_attacks; i++)
                {
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO] Processing hit " << i << endl;
#endif
                    int firepower = Hits[i].firepower;
                    // Is the firepower greater than 0
                    if (firepower > 0)
                    {
                        int AbortCounter = 0;
                        do
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] Seeking a target" << endl;
#endif
                            AbortCounter++;
                            if (AbortCounter >= 10)
                            {
                                // We have tried 10 times with this attack.  Skip it.
                                // Check forceID
                                if (ForceID == 0)
                                {
                                    BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " aborts attack!";
                                    reportFile << BE::AttBattleStr << "\n";
                                }
                                else
                                {
                                    BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " aborts attack!";
                                    reportFile << BE::DefBattleStr << "\n";
                                }
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Unit has aborted an attack after 10 tries!" << endl;
#endif
                                continue;
                            }

                            // This section of code sets Target1.  The variable is set/reset if it is
                            // the first attack roll (i = 0) or bit 16 is set for scatter pack targetting.
                            // [JLL] I removed the logic tests for first hit and for bit 16 as all hits now have bit 16 set
                            int HullTarget = 0;
                            if (ForceID == 0) // Attackers
                            {
                                string CombatStr = Hits[i].tag;
                                int HullScope = 0;
                                // Does the fleet have a target priority?
                                if (BE::AttTargetPriority > 0)
                                {
                                    HullTarget = BE::AttTargetPriority;
                                }
                                HasHull(CombatStr, HullTarget, HullScope);
                                HasScan(CombatStr, HullTarget, HullScope);
                                int dlGroup = -1; // TODO: Replace -1 with constant
                                if (HasDatalinkWT(CombatStr, dlGroup))
                                {
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Hit has datalink, group " << dlGroup << endl;
#endif
                                    BE::Target1 = DataLinkA[dlGroup];
                                    // [JLL] Check if there is a target for the datalink group already
                                    if (BE::Target1 == -1) // TODO: Replace -1 with a constant
                                    {
                                        if (HasScan(CombatStr, HullTarget, HullScope))
                                        {
                                            // Get a scan target
                                            BE::Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, HullScope, DefNumValidTargets);
                                        }
                                        else if (HasHull(CombatStr, HullTarget, HullScope))
                                        {
                                            // Get a hull target
                                            BE::Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, HullScope, DefNumValidTargets);
                                        }
                                        else
                                        {
                                            // Get a random target
                                            BE::Target1 = GetRandomTarget(ForceID, CombatStr, DefNumValidTargets);
                                        }
                                    }
                                }
                                else
                                {
                                    // No datalink, just get a target
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] No datalink, just get a target" << endl;
#endif
                                    if (HasScan(CombatStr, HullTarget, HullScope))
                                    {
                                        // Get a scan target
                                        BE::Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, HullScope, DefNumValidTargets);
                                    }
                                    else if (HasHull(CombatStr, HullTarget, HullScope))
                                    {
                                        // Get a hull target
                                        BE::Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, HullScope, DefNumValidTargets);
                                    }
                                    else
                                    {
                                        // Get a random target
                                        BE::Target1 = GetRandomTarget(ForceID, CombatStr, DefNumValidTargets);
                                    }
                                }
                            }
                            else // Defenders
                            {
                                string CombatStr = Hits[i].tag;
                                int HullScope = 0;
                                // Does the fleet have a target priority?
                                if (BE::DefTargetPriority > 0)
                                {
                                    HullTarget = BE::DefTargetPriority;
                                }
                                int dlGroup = -1;
                                if (HasDatalinkWT(CombatStr, dlGroup))
                                {
                                    BE::Target1 = DataLinkB[dlGroup];
                                    // Check if there is a target for the datalink group already
                                    if (BE::Target1 == -1)
                                    {
                                        // No, get a target
                                        // Check for scan tag
                                        if (HasScan(CombatStr, HullTarget, HullScope))
                                        {
                                            // Yes to scan tag
                                            BE::Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, HullScope, AttNumValidTargets);
                                        }
                                        // Check for hull tag
                                        else if (HasHull(CombatStr, HullTarget, HullScope))
                                        {
                                            // Yes to hull tag
                                            BE::Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, HullScope, AttNumValidTargets);
                                        }
                                        else
                                        {
                                            // Get a random target if all else failes
                                            BE::Target1 = GetRandomTarget(ForceID, CombatStr, AttNumValidTargets);
                                        }
                                    }
                                }
                                else
                                {
                                    // No datalink, just get a target
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] No datalink, just get a target" << endl;
#endif
                                    if (HasScan(CombatStr, HullTarget, HullScope))
                                    {
                                        // Get a scan target
                                        BE::Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, HullScope, AttNumValidTargets);
                                    }
                                    else if (HasHull(CombatStr, HullTarget, HullScope))
                                    {
                                        // Get a hull target
                                        BE::Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, HullScope, AttNumValidTargets);
                                    }
                                    else
                                    {
                                        // Get a random target
                                        BE::Target1 = GetRandomTarget(ForceID, CombatStr, AttNumValidTargets);
                                    }
                                }
                            }

                            SeekTarget = 0;
                            // Reject captured, just captured, and cloaked targets
                            if (ForceID == 0)
                            {
                                // TODO: Replace 99 with a constance (See next line)
                                if (IsCaptured(BE::SpecialB[BE::Target1]) || BE::BPAttackCritB[BE::Target1] > 99 || IsCloak(BE::SpecialB[BE::Target1]))
                                {
                                    SeekTarget = 1;
                                }
                            }
                            else
                            {
                                // TODO: Replace 99 with a constance (See next line)
                                if (IsCaptured(BE::SpecialA[BE::Target1]) || BE::BPAttackCritA[BE::Target1] > 99 || IsCloak(BE::SpecialA[BE::Target1]))
                                {
                                    SeekTarget = 1;
                                }
                            }
                        } while (SeekTarget == 1);

#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] Target selected: index=" << BE::Target1 << endl;
#endif

                        //  Since target selection is not done until now, point defense can not engage missiles until now
                        if (ForceID == 0)
                        {
                            // Does the target have PD and is the attacker a missile?
                            int pd = 0;
                            if (HasPointDefense(BE::SpecialB[BE::Target1], pd) && IsMissile(BE::SpecialA[B]))
                            {
                                // Roll for a missile intercept
                                int roll = rand() % 100;
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] PD=" << pd << "; Roll=" << roll << endl;
#endif
                                // Check if the roll succeded: roll must be less than pd tag value
                                if (roll < pd)
                                {
                                    // The missile was intercepted
                                    BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " missile intercepted!";
                                    BE::CurHullA[B] = 0;
                                    Hits[i].valid = false;
                                    writeBattleString(reportFile, BE::AttBattleStr); // TODO: Remove
                                    continue;
                                }
                            }
                        }
                        else
                        {
                            // Does the target have PD and is the attacker a missile?
                            int pd = 0;
                            if (HasPointDefense(BE::SpecialA[BE::Target1], pd) && IsMissile(BE::SpecialB[B]))
                            {
                                // Roll for the missile intercept
                                int roll = rand() % 100;
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] PD=" << pd << "; Roll=" << roll << endl;
#endif
                                // Check if the roll succedded: roll must be less than pd tag value
                                if (roll < pd)
                                {
                                    // The missiles was intercepted
                                    BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " missile intercepted!";
                                    BE::CurHullB[B] = 0;
                                    Hits[i].valid = false;
                                    writeBattleString(reportFile, BE::DefBattleStr); // TODO: Remove
                                    continue;
                                }
                            }
                        }

                        // Determine attack roll and damage inflicted

                        /*
                         * Base chance to hit is 100% modified by targeting and defense
                         * Base damage multiplier (1d100 + yield - resistance + get_hit_bonus) [min 1, max 100]
                         * Base damage is int((firepower * multiplier) / 100)
                         * Suicide adds to hit which may add to the great_hit_bonus
                         * BPs always 'hit' but may not be effective due to enemy shields or defenders
                         */

                        switch (ForceID)
                        {
                        case 0: // Attackers
                            if (Hits[i].special & BE::saBp == BE::saBp)
                            {
                                // This is a boarding party attack
                                // Invalid targets are: solid, fighter, ground, vehicle, mine, and shielded targets unless the bp has pen
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Boarding party attack" << endl;
#endif
                                bool attempt = true;
                                if (IsSolid(BE::SpecialB[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsFighter(BE::SpecialB[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsGround(BE::SpecialB[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsVehicle(BE::SpecialB[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsMine(BE::SpecialB[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (BE::CurShieldB[BE::Target1] > 0 && Hits[i].special & BE::saPen != BE::saPen)
                                {
                                    attempt = false;
                                }
                                if (attempt)
                                {
                                    BE::dice1 = 100;
                                }
                                else
                                {
                                    BE::dice1 = 0;
                                }
                            }
                            else
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Regular attack" << endl;
#endif
                                // A non-boarding patry attack
                                SuicideBonus = 0;
                                CombatBonus = BE::AttTargetBonus;
                                YieldBonus = 0;
                                if (IsScuicide(BE::SpecialA[B]))
                                {
                                    SuicideBonus = 1 + rand() % 99;
                                }
                                int target = HasTarget(CombatStr);
                                if (target > 0)
                                {
                                    CombatBonus = target; // FIXME: Should this not be additive to the fleet's target bonus?
                                }
                                HasYield(CombatStr, YieldBonus);
                                long resist = 0;
                                if (HasResist(BE::SpecialB[BE::Target1], resist))
                                {
                                    YieldBonus -= resist;
                                }
                                long defense = HasDefense(BE::SpecialB[BE::Target1]);
                                if (defense <= 0)
                                {
                                    defense = 0;
                                }
                                AutoHit = 0;
                                AutoMiss = 0;
                                BE::dice1 = 1 + (rand() % 99);
                                if (BE::dice1 == 0)
                                {
                                    AutoMiss = 1;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Automatic miss due to dice roll of 1" << endl;
#endif
                                }
                                else if (BE::dice1 == 100)
                                {
                                    AutoHit = 1;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Automatic hit due to dice roll of 100" << endl;
#endif
                                }
                                BE::dice1 = BE::dice1 + BE::DM_ToHitA + CombatBonus - defense;
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Target dice: " << BE::dice1 << endl;
#endif
                            }
                            break;
                        case 1: // Defenders
                            if (Hits[i].special & BE::saBp == BE::saBp)
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Boarding party attack" << endl;
#endif
                                // This is a boarding party attack
                                // Invalid targets are: solid, fighter, ground, vehicle, mine, and shielded targets unless the bp has pen
                                bool attempt = true;
                                if (IsSolid(BE::SpecialA[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsFighter(BE::SpecialA[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsGround(BE::SpecialA[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsVehicle(BE::SpecialA[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (IsMine(BE::SpecialA[BE::Target1]))
                                {
                                    attempt = false;
                                }
                                if (BE::CurShieldA[BE::Target1] > 0 && Hits[i].special & BE::saPen != BE::saPen)
                                {
                                    attempt = false;
                                }
                                if (attempt)
                                {
                                    BE::dice1 = 100;
                                }
                                else
                                {
                                    BE::dice1 = 0;
                                }
                            }
                            else
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Regular attack" << endl;
#endif
                                // A non-boarding patry attack
                                SuicideBonus = 0;
                                CombatBonus = BE::AttTargetBonus;
                                YieldBonus = 0;
                                if (IsScuicide(BE::SpecialB[B]))
                                {
                                    SuicideBonus = 1 + rand() % 99;
                                }
                                int target = HasTarget(CombatStr);
                                if (target > 0)
                                {
                                    CombatBonus = target; // FIXME: Should this not be additive to the fleet's target bonus?
                                }
                                HasYield(CombatStr, YieldBonus);
                                long resist = 0;
                                if (HasResist(BE::SpecialA[BE::Target1], resist))
                                {
                                    YieldBonus -= resist;
                                }
                                long defense = HasDefense(BE::SpecialA[BE::Target1]);
                                if (defense <= 0)
                                {
                                    defense = 0;
                                }
                                AutoHit = 0;
                                AutoMiss = 0;
                                BE::dice1 = 1 + (rand() % 99);
                                if (BE::dice1 == 0)
                                {
                                    AutoMiss = 1;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Automatic miss due to dice roll of 1" << endl;
#endif
                                }
                                else if (BE::dice1 == 100)
                                {
                                    AutoHit = 1;
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Automatic hit due to dice roll of 100" << endl;
#endif
                                }
                                BE::dice1 = BE::dice1 + BE::DM_ToHitB + CombatBonus - defense;
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Target dice: " << BE::dice1 << endl;
#endif
                            }
                            break;
                        }

                        // Ok, dice1 < 1 is a miss.  Dice1 > 100 earns a Dice1-100 great_hit_bonus
                        // Dice 1 to 100 is a normal hit
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] Calculating damage.  Firepower: " << firepower << endl;
#endif
                        if ((BE::dice1 <= (100 - BaseAccuracy) || AutoMiss == 1) && AutoHit == 0)
                        {
                            // Miss
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO] The hit is a miss.  dice1:" << BE::dice1 << endl;
#endif
                            BE::Damage3 = 0;
                        }
                        else
                        {
                            // Check if the MaximumDamage option is true
                            if (BE::MaximumDamage == 1) // TODO: Add to command line arguments
                            {
                                BE::Damage3 = firepower;
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Using maximum damage" << endl;
#endif
                            }
                            else
                            {
                                // We are not using max damage so roll damage
                                if (BE::dice1 > 100)
                                {
                                    // Calculate the good hit bonu
                                    YieldBonus += (BE::dice1 - 100);
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Good hit bonus!" << endl;
#endif
                                }

                                // BE::Damage3 = long(round((1 + (rand() % (firepower - 1))) * (1.0f + (float(YieldBonus) / 100.0f))));
                                long damageRoll = rand() % 100 + YieldBonus;
                                double actualDamage = (double)damageRoll / 100.0f * (double)firepower;
                                double roundedDamage = round(actualDamage);

                                BE::Damage3 = (long)roundedDamage;

#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] Raw damage: " << BE::Damage3 << endl;
#endif
                                if (BE::Damage3 < 1)
                                {
                                    BE::Damage3 = 0;
                                }
                                else if (BE::Damage3 > firepower)
                                {
                                    BE::Damage3 = firepower;
                                }
                            }
                        }
                        BE::Damage2 = 0; // Clear pen damage
                        BE::Damage1 = BE::Damage3;
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO] damage: " << BE::Damage1 << endl;
#endif
                        if (ForceID == 0) // Attackers
                        {
                            if (BE::dice1 <= (100 - BaseAccuracy))
                            {
                                // This is a miss.  Make the appropriate battle strings to write out later
                                if (Hits[i].special & BE::saBp == BE::saBp)
                                {
                                    BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " can not engage " + BE::DefRaceName + " " + BE::DefShipStr[BE::Target1] + " in boarding party combat.";
                                }
                                else
                                {
                                    BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " fires on " + BE::DefRaceName + " " + BE::DefShipStr[BE::Target1] + " and misses.";
                                }
                            }
                            else
                            {
                                // This is a hit!
                                // Check if this is a boarding party hit
                                if (Hits[i].special & BE::saBp == BE::saBp)
                                {
                                    AttVal = firepower; // Can just use the bracket firepower as there is no need to check for unit BP attack value.

                                    // Calculate the targets defensive bp value
                                    DefVal = GetBPDefVal(BE::SpecialB[BE::Target1], BE::CurHullB[BE::Target1]);

                                    // Roll a random number between 1 and (DefVal + AttVal)
                                    // Then check if the random number is greater than the DefVal of the target
                                    // If the number is greater, then the attack party was successful
                                    BPDice = (rand() % (DefVal + AttVal - 1)) + 1;
                                    if (BPDice > DefVal)
                                    {
                                        // Now weee roll another die.  This is for the 25% chance that attacking
                                        // boarding party captured the target
                                        // TODO: This should not be a flat 25%.  Should scale with DefVal vs AttVal
                                        int catpureDie = rand() % 4;
                                        if (catpureDie == 0)
                                        {
                                            // The target was captured.  Setup the report string and the critical hit
                                            BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " captures " + BE::DefShipStr[BE::Target1];
                                            BE::BPAttackCritB[BE::Target1] += 100;                                                    // FIXME: Change this to a constant
                                            BE::TempSpecialB[BE::Target1] = AddTag(BE::TempSpecialB[BE::Target1], "NOMOVE CAPTURED"); // The ship cannot run away
                                            // FIXME: The BP should be expended
                                        }
                                        else
                                        {
                                            // The defend has suffered a normal BP crit rather than a capture
                                            BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " is conducting a hit and run raid on " + BE::DefShipStr[BE::Target1];
                                            BE::BPAttackCritB[BE::Target1] += 1;
                                            // [JLL] TODO: I have no idea what this code is for and why it is here...
                                            BE::Shields = 0;
                                            BE::Crits = 0;
                                            BE::Hull = BE::TempCurHullB[BE::Target1];
                                            BE::X = BE::Target1;
                                            if (BE::TempCurHullB[BE::Target1] == 0)
                                            {
                                                tmp = 0;
                                            }
                                            else
                                            {
                                                tmp = 100 - ((BE::TempCurHullB[BE::Target1] * 100) / BE::MaxHullB[BE::Target1]); // TODO: Clear up to Cur/Max * 100 for clarity.  Will need to do type casting for this to work well.
                                            }
                                            BE::Attacks[BE::AttacksIndex].AttackID = BE::A;
                                            BE::Attacks[BE::AttacksIndex].TargetID = BE::Target1 + BE::AttShipsLeft;
                                            BE::Attacks[BE::AttacksIndex].Damage = BE::Damage1;
                                            BE::Attacks[BE::AttacksIndex].Weapon = Hits[i].special;
                                            BE::Attacks[BE::AttacksIndex].Special = Hits[i].tag;
                                            BE::AttacksIndex++;
                                            // TODO: Better too many hit handling
                                            if (BE::AttacksIndex >= BE::AttacksMax)
                                            {
#ifdef CBE_DEBUG
                                                CBE::debugFile << "[ERROR] Number of attacks exceeded maximum number allowed.  Max is " << BE::AttacksMax << endl;
#endif
                                                cerr << "I have reached the maximum number of attacks I can handle." << endl;
                                                exit(1);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // Attackers repulsed
                                        // TODO: This should scale with AttVal vs DefVal
                                        if (rand() % 4 == 0)
                                        {
                                            // Defenders win and boarding party is expended
                                            // FIXME: THE BP should be expended
                                            // TODO: Add which unit (Target1) ambushed the borders.
                                            BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " borders ambushed, multiple causualties!";
                                        }
                                        else
                                        {
                                            // Defenders win and boarding party is repulsed.  Boarding party can try again later
                                            // TODO: Add which unit (Target1) repulsed the boarders
                                            BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " borders repulsed!";
                                        }
                                    }
                                }
                                else
                                {
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Attacker Regular Attack" << endl;
#endif
                                    BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " fires on " + BE::DefRaceName + " " + BE::DefShipStr[BE::Target1] + " scoring " + to_string(BE::Damage1) + " hits (" + to_string(BE::dice1) + "%)";
                                    BE::Attacks[BE::AttacksIndex].AttackID = BE::A;
                                    BE::Attacks[BE::AttacksIndex].TargetID = BE::Target1 + BE::AttShipsLeft;
                                    BE::Attacks[BE::AttacksIndex].Damage = BE::Damage1;
                                    BE::Attacks[BE::AttacksIndex].Weapon = Hits[i].special;
                                    BE::Attacks[BE::AttacksIndex].Special = Hits[i].tag;
                                    BE::AttacksIndex++;
                                    if (BE::AttacksIndex >= BE::AttacksMax)
                                    {
#ifdef CBE_DEBUG
                                        CBE::debugFile << "[ERROR] Maximum number of attacks exceeded." << endl;
#endif
                                        // TODO: Throw an exception to be caught
                                        exit(1);
                                    }
                                }
                            }
                            if (number_of_attacks > 1)
                            {
                                // This is to indicate in the report file which attack this is from the unit
                                BE::AttBattleStr = BE::AttBattleStr + "[" + to_string(i + 1) + " of " + to_string(number_of_attacks) + "]";
                            }
                        }    // Done with attackers
                        else // Defenders
                        {
                            // ForceID == 1
                            // Do all of the above again, but now for the defenders.
                            if (BE::dice1 <= (100 - BaseAccuracy))
                            {
                                // This is a miss.  Make the appropriate battle strings to write out later
                                if (Hits[i].special & BE::saBp == BE::saBp)
                                {
                                    BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " can not engage " + BE::AttRaceName + " " + BE::AttShipStr[BE::Target1] + " in boarding party combat.";
                                }
                                else
                                {
                                    BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " fires on " + BE::AttRaceName + " " + BE::AttShipStr[BE::Target1] + " and misses.";
                                }
                            }
                            else
                            {
                                // This is a hit!
                                // Check if this is a boarding party hit
                                if (Hits[i].special & BE::saBp == BE::saBp)
                                {
                                    AttVal = firepower; // Can just use the bracket firepower as there is no need to check for unit BP attack value.

                                    // Calculate the targets defensive bp value
                                    DefVal = GetBPDefVal(BE::SpecialA[BE::Target1], BE::CurHullA[BE::Target1]);

                                    // Roll a random number between 1 and (DefVal + AttVal)
                                    // Then check if the random number is greater than the DefVal of the target
                                    // If the number is greater, then the attack party was successful
                                    BPDice = (rand() % (DefVal + AttVal - 1)) + 1;
                                    if (BPDice > DefVal)
                                    {
                                        // Now weee roll another die.  This is for the 25% chance that attacking
                                        // boarding party captured the target
                                        // TODO: This should not be a flat 25%.  Should scale with DefVal vs AttVal
                                        int catpureDie = rand() % 4;
                                        if (catpureDie == 0)
                                        {
                                            // The target was captured.  Setup the report string and the critical hit
                                            BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " captures " + BE::AttShipStr[BE::Target1];
                                            BE::BPAttackCritA[BE::Target1] += 100;                                                    // FIXME: Change this to a constant
                                            BE::TempSpecialA[BE::Target1] = AddTag(BE::TempSpecialA[BE::Target1], "NOMOVE CAPTURED"); // The ship cannot run away
                                            // FIXME: The BP should be expended
                                        }
                                        else
                                        {
                                            // The defend has suffered a normal BP crit rather than a capture
                                            BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " is conducting a hit and run raid on " + BE::AttShipStr[BE::Target1];
                                            BE::BPAttackCritA[BE::Target1] += 1;
                                            // [JLL] TODO: I have no idea what this code is for and why it is here...
                                            BE::Shields = 0;
                                            BE::Crits = 0;
                                            BE::Hull = BE::TempCurHullA[BE::Target1];
                                            BE::X = BE::Target1;
                                            if (BE::TempCurHullA[BE::Target1] == 0)
                                            {
                                                tmp = 0;
                                            }
                                            else
                                            {
                                                tmp = 100 - ((BE::TempCurHullA[BE::Target1] * 100) / BE::MaxHullA[BE::Target1]); // TODO: Clear up to Cur/Max * 100 for clarity.  Will need to do type casting for this to work well.
                                            }
                                            BE::Attacks[BE::AttacksIndex].AttackID = BE::A + BE::AttShipsLeft;
                                            BE::Attacks[BE::AttacksIndex].TargetID = BE::Target1;
                                            BE::Attacks[BE::AttacksIndex].Damage = BE::Damage1;
                                            BE::Attacks[BE::AttacksIndex].Weapon = Hits[i].special;
                                            BE::Attacks[BE::AttacksIndex].Special = Hits[i].tag;
                                            BE::AttacksIndex++;
                                            // TODO: Better too many hit handling
                                            if (BE::AttacksIndex >= BE::AttacksMax)
                                            {
#ifdef CBE_DEBUG
                                                CBE::debugFile << "[ERROR] Number of attacks exceeded maximum number allowed.  Max is " << BE::AttacksMax << endl;
#endif
                                                cerr << "I have reached the maximum number of attacks I can handle." << endl;
                                                exit(1);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // Attackers repulsed
                                        // TODO: This should scale with AttVal vs DefVal
                                        if (rand() % 4 == 0)
                                        {
                                            // Defenders win and boarding party is expended
                                            // FIXME: THE BP should be expended
                                            // TODO: Add which unit (Target1) ambushed the borders.
                                            BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " borders ambushed, multiple causualties!";
                                        }
                                        else
                                        {
                                            // Defenders win and boarding party is repulsed.  Boarding party can try again later
                                            // TODO: Add which unit (Target1) repulsed the boarders
                                            BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " borders repulsed!";
                                        }
                                    }
                                }
                                else
                                {
#ifdef CBE_DEBUG
                                    CBE::debugFile << "[INFO] Defender regular attack" << endl;
#endif
                                    BE::DefBattleStr = BE::DefRaceName + " " + BE::DefShipStr[B] + " fires on " + BE::AttRaceName + " " + BE::AttShipStr[BE::Target1] + " scoring " + to_string(BE::Damage1) + " hits (" + to_string(BE::dice1) + "%)";
                                    BE::Attacks[BE::AttacksIndex].AttackID = BE::A + BE::AttShipsLeft;
                                    BE::Attacks[BE::AttacksIndex].TargetID = BE::Target1;
                                    BE::Attacks[BE::AttacksIndex].Damage = BE::Damage1;
                                    BE::Attacks[BE::AttacksIndex].Weapon = Hits[i].special;
                                    BE::Attacks[BE::AttacksIndex].Special = Hits[i].tag;
                                    BE::AttacksIndex++;
                                    if (BE::AttacksIndex >= BE::AttacksMax)
                                    {
#ifdef CBE_DEBUG
                                        CBE::debugFile << "[ERROR] Maximum number of attacks exceeded." << endl;
#endif
                                        // TODO: Throw an exception to be caught
                                        exit(1);
                                    }
                                }
                            }
                            if (number_of_attacks > 1)
                            {
                                // This is to indicate in the report file which attack this is from the unit
                                BE::DefBattleStr = BE::DefBattleStr + "[" + to_string(i + 1) + " of " + to_string(number_of_attacks) + "]";
                            }
                        } // Done with defenders
                    }
                    else // firepower is less than or equal to 0?
                    {
                        if (ForceID == 0) // Attackers
                        {
                            BE::AttBattleStr = BE::AttRaceName + " " + BE::AttShipStr[B] + " no normal attack. (Firepower is equal to 0)";
                        }
                        else // Defenders
                        {
                            BE::DefBattleStr = BE::DefBattleStr + " " + BE::DefShipStr[B] + " no normal attack. (Firepower is equal to 0)";
                        }
                    }

                    // Check ForceID to print the correct message
                    if (ForceID == 0)
                    {
                        // TODO: Change AttBattleStr and DefBattleStr to a single local variable.  Unless these get added to later in the loop. (Written at the Salvos point)
                        if (BE::AttBattleStr != "")
                        {
                            reportFile << BE::AttBattleStr << "\n";
                        }
                    }
                    else
                    {
                        if (BE::DefBattleStr != "")
                        {
                            reportFile << BE::DefBattleStr << "\n";
                        }
                    }
                } // End for number_of_attacks
            }

            reportFile << "\n";

            // Damage routine
            // Clear the battle strings for some reason
            BE::AttBattleStr = "";
            BE::DefBattleStr = "";

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO] Begining of damage routine" << endl;
#endif

            for (long A = 0; A < (BE::AttShipsLeft + BE::DefShipsLeft); A++)
            {
                // [JLL] Do the usual convoluted bullshit for attackers and defenders
                long B = A;
                ForceID = 0;
                if (A >= BE::AttShipsLeft)
                {
                    B = A - BE::AttShipsLeft;
                    ForceID = 1;
                }
                else if (A == BE::AttShipsLeft)
                {
                    reportFile << "\n";
                }

#ifdef CBE_DEBUG
                CBE::debugFile << "[INFO][Damage Routine] A=" << A << ";B=" << B << ";ForceID=" << ForceID << endl;
#endif

                ShipHit = 0;
                // Check to see if the ship has was attacked
                // [JLL] Not sure why this is done
                // This is done to collect all of the attacks against a specific unit
                // for grouping in the report file
                // TODO: Store hits by unit rather than globally
                for (long E = 0; E < BE::AttacksIndex; E++)
                {
                    // Check for the target of the attack matching the current ship
                    // This is to group all attacks against this ship
                    // For grouping in the reports file
                    if (BE::Attacks[E].TargetID == A)
                    {
                        B = BE::Attacks[E].AttackID;
#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO][Damage Routine] AttackID=" << B << endl;
#endif
                        if (B >= BE::AttShipsLeft)
                        {
                            B = B - BE::AttShipsLeft;
                            ForceID = 1;
                            BE::Target1 = BE::Attacks[E].TargetID;
                        }
                        else
                        {
                            ForceID = 0;
                            BE::Target1 = BE::Attacks[E].TargetID - BE::AttShipsLeft;
                        }

#ifdef CBE_DEBUG
                        CBE::debugFile << "[INFO][Damage Routine] Target1=" << BE::Target1 << endl;
#endif

                        BE::Damage1 = BE::Attacks[E].Damage;

                        // [JLL] This set of if statements is used to print out the header for a block of attacks against
                        // a specific ship.  I have collapsed the logic some from the original and placed the ShipHit check
                        // first because it is the one that will change most frequently.
                        if (ShipHit == 0 && ForceID == 0)
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO][Damage Routine] Attacker Target1(" << BE::Target1 << "):" << BE::DefShipStr[BE::Target1] << " hit." << endl;
#endif
                            reportFile << BE::DefShipStr[BE::Target1] + " has been hit.\n";
                            ShipHit = 1;
                        }
                        else if (ShipHit == 0 && ForceID == 1)
                        {
#ifdef CBE_DEBUG
                            CBE::debugFile << "[INFO][Damage Routine] Defender Target1(" << BE::Target1 << "):" << BE::AttShipStr[BE::Target1] << " hit." << endl;
#endif
                            reportFile << BE::AttShipStr[BE::Target1] + " has been hit.\n";
                            ShipHit = 1;
                        }

                        if (BE::Attacks[E].Weapon & BE::saBp == BE::saBp)
                        {
                            reportFile << "  Boarding party attack is in progress.\n";
                        }
                        else
                        {
                            // TODO: Think about better logic than simply not boarding party
                            reportFile << "  Takes " + to_string(BE::Damage1) + " points of damage.";
                        }

                        if (ForceID == 0)
                        {
                            long flicker = 0;
                            if (HasFlicker(BE::SpecialB[BE::Target1], flicker))
                            {
                                long flickerRoll = 1 + (rand() % 99);
                                if (flickerRoll <= flicker)
                                {
                                    BE::Damage1 = 0;
                                    reportFile << " <attack deflected>";
                                }
                            }
                            // FIXME: If flicker has already worked then why do we need to check for shields?
                            if (BE::CurShieldB[BE::Target1] > 0)
                            {
                                long sr = 0;
                                if (HasShields(BE::SpecialB[BE::Target1], sr))
                                {
                                    BE::Damage1 -= sr;
                                    if (BE::Damage1 < 1)
                                    {
                                        reportFile << " <attack deflected>";
                                        BE::Damage1 = 0;
                                    }
                                    else
                                    {
                                        reportFile << " <" << sr << " pts deflected>";
                                    }
                                }
                                else
                                {
                                    // [JLL] Crack only works if SR is not present?
                                    // TODO: Think about if crack weapons should effect targets with SR
                                    // FIXME: Need to move this around so that crack is check and damage increased
                                    // then have the SR checked.
                                    if (BE::Attacks[E].Weapon & BE::saCrack == BE::saCrack)
                                    {
                                        long crackDamage = BE::Damage1 * 2;
                                        if (crackDamage < BE::TempCurShieldB[BE::Target1])
                                        {
                                            // [JLL] Why would ret be set to Damage1 * 2?
                                            BE::TempCurShieldB[BE::Target1] -= crackDamage;
                                            BE::Damage1 = 0; // This keeps crack damage from overflowing to armor/hull
                                            reportFile << " <shield disruption>";
                                        }
                                        else
                                        {
                                            // [JLL] ret here gets set to the remaining shields...
                                            BE::TempCurShieldB[BE::Target1] = 0;
                                            BE::Damage1 = 0; // This keeps crack damage from overflowing to armor/hull
                                            reportFile << " <shield disrupted>";
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // No shields, check for crack so that damage can be set to 0.
                                if (BE::Attacks[E].Weapon & BE::saCrack == BE::saCrack)
                                {
                                    BE::Damage1 = 0;
                                    reportFile << " <attack dissipates>";
                                }
                            }
                        }
                        else // Defenders
                        {
                            long flicker = 0;
                            if (HasFlicker(BE::SpecialA[BE::Target1], flicker))
                            {
                                long flickerRoll = 1 + (rand() % 99);
                                if (flickerRoll <= flicker)
                                {
                                    BE::Damage1 = 0;
                                    reportFile << " <attack deflected>";
                                }
                            }
                            if (BE::CurShieldA[BE::Target1] > 0)
                            {
                                long sr = 0;
                                if (HasShields(BE::SpecialA[BE::Target1], sr))
                                {
                                    BE::Damage1 -= sr;
                                    if (BE::Damage1 < 1)
                                    {
                                        reportFile << " <attack deflected>";
                                        BE::Damage1 = 0;
                                    }
                                    else
                                    {
                                        reportFile << " <" << sr << " pts deflected>";
                                    }
                                }
                                else
                                {
                                    // [JLL] Crack only works if SR is not present?
                                    // TODO: Think about if crack weapons should effect targets with SR
                                    // FIXME: Need to move this around so that crack is check and damage increased
                                    // then have the SR checked.
                                    if (BE::Attacks[E].Weapon & BE::saCrack == BE::saCrack)
                                    {
                                        long crackDamage = BE::Damage1 * 2;
                                        if (crackDamage < BE::TempCurShieldA[BE::Target1])
                                        {
                                            // [JLL] Why would ret be set to Damage1 * 2?
                                            BE::TempCurShieldA[BE::Target1] -= crackDamage;
                                            BE::Damage1 = 0; // This keeps crack damage from overflowing to armor/hull
                                            reportFile << " <shield disruption>";
                                        }
                                        else
                                        {
                                            // [JLL] ret here gets set to the remaining shields...
                                            BE::TempCurShieldA[BE::Target1] = 0;
                                            BE::Damage1 = 0; // This keeps crack damage from overflowing to armor/hull
                                            reportFile << " <shield disrupted>";
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // No shields, check for crack so that damage can be set to 0.
                                if (BE::Attacks[E].Weapon & BE::saCrack == BE::saCrack)
                                {
                                    BE::Damage1 = 0;
                                    reportFile << " <attack dissipates>";
                                }
                            }
                        }

                        // Do the pen damage checks
                        long X = BE::Target1;
                        string temp_str = "";
                        BE::ShipCritStr = ""; // TODO: Change to local scope variable
                        BE::Crits = 0;        // TODO: Change to local scope variable

                        if (ForceID == 0)
                        {
                            if (BE::TempCurShieldB[X] < BE::Damage1)
                            {
                                BE::Shields = 0;                                       // TODO: Change to local scope variable
                                BE::DamageLevel = BE::Damage1 - BE::TempCurShieldB[X]; // TODO: Change to local scope variable
                            }
                            else
                            {
                                BE::Shields = BE::TempCurShieldB[X] - BE::Damage1;
                                BE::DamageLevel = 0;
                            }

                            BE::DamageLevel += BE::Damage2; // Penetrating damage bypass shields TODO: Rename variables for clearer code
                            long ar = 0;

                            if (BE::DamageLevel > 0 && HasArmor(BE::SpecialB[BE::Target1], ar))
                            {
                                BE::DamageLevel -= ar;
                                if (BE::DamageLevel < 1)
                                {
                                    reportFile << " <attack blocked>";
                                    BE::TempCurShieldB[X] = BE::Shields;
                                    // [JLL] FIXME: I have no idea what this code is here for
                                    if (BE::Attacks[E].Weapon & BE::saBp != BE::saBp)
                                    {
                                        // [JLL] Something about marines get through.  But this isn't a test for BP, this is for the absence of BP...
                                        reportFile << "\n";
                                        continue;
                                    }
                                }
                                else
                                {
                                    reportFile << " <" << to_string(ar) << " pts blocked>";
                                }
                            }
                        }
                        else
                        {
                            if (BE::TempCurShieldA[X] < BE::Damage1)
                            {
                                BE::Shields = 0;
                                BE::DamageLevel = BE::Damage1 - BE::TempCurShieldA[X];
                            }
                            else
                            {
                                BE::Shields = BE::TempCurShieldA[X] - BE::Damage1;
                                BE::DamageLevel = 0;
                            }

                            BE::DamageLevel += BE::Damage2;
                            long ar = 0;

                            if (BE::DamageLevel > 0 && HasArmor(BE::SpecialA[BE::Target1], ar))
                            {
                                BE::DamageLevel -= ar;
                                if (BE::DamageLevel < 1)
                                {
                                    reportFile << " <attack blocked>";
                                    BE::TempCurShieldA[X] = BE::Shields;
                                    if (BE::Attacks[E].Weapon & BE::saBp != BE::saBp)
                                    {
                                        reportFile << "\n";
                                        continue;
                                    }
                                }
                                else
                                {
                                    reportFile << " <" << to_string(ar) << " pts blocked>";
                                }
                            }
                        }

                        reportFile << "\n";

                        // Stasis snap shield logic
                        // Statis shield only activates when the shield fails
                        // so pen-hits get through unless the shield falls at the same time.
                        if (ForceID == 0)
                        {
                            if (IsStatis(BE::SpecialB[X]))
                            {
                                if (BE::Shields == 0 && BE::CurShieldB[X] > 0 && BE::DamageLevel > 0)
                                {
                                    BE::DamageLevel = 0;
                                    BE::TempCurShieldB[X] = 0;
                                    reportFile << "  Statis field activated.  Extra damage deflected.\n";
                                    continue;
                                }
                            }
                        }
                        else
                        {
                            if (IsStatis(BE::SpecialA[X]))
                            {
                                if (BE::Shields == 0 && BE::CurShieldA[X] > 0 && BE::DamageLevel > 0)
                                {
                                    BE::DamageLevel = 0;
                                    BE::TempCurShieldA[X] = 0;
                                    reportFile << "  Statis field activated.  Extra damage deflected.\n";
                                    continue;
                                }
                            }
                        }

                        // [JLL] I think this applies damage to the hull
                        // [JLL] This is remaining hull, hull percentage, hull damage, and shield percentage.
                        if (ForceID == 0)
                        {
                            if (BE::TempCurHullB[X] <= 0)
                            {
                                tmp = 0;
                            }
                            else
                            {
                                // This is the percentage of the max hull damage.  Used below for crit calculations
                                tmp = 100 - (BE::TempCurHullB[X] * 100) / BE::MaxHullB[X];
                            }

                            BE::Hull = BE::TempCurHullB[X] - BE::DamageLevel;
                            if (BE::Hull < 0)
                            {
                                BE::Hull = 0;
                            }

                            if (BE::MaxShieldB[X] == 0 || BE::Shields == 0)
                            {
                                BE::ShieldsPercent = 0;
                            }
                            else
                            {
                                BE::ShieldsPercent = BE::Shields * 100 / BE::MaxShieldB[X];
                            }

                            if (BE::MaxHullB[X] == 0 || BE::Hull == 0)
                            {
                                BE::HullPercent = 0;
                            }
                            else
                            {
                                BE::HullPercent = 100 - (BE::Hull * 100) / BE::MaxHullB[X];
                            }
                        }
                        else
                        {
                            if (BE::TempCurHullA[X] <= 0)
                            {
                                tmp = 0;
                            }
                            else
                            {
                                // This is the percentage of the max hull damage.  Used below for crit calculations
                                tmp = 100 - (BE::TempCurHullA[X] * 100) / BE::MaxHullA[X];
                            }

                            BE::Hull = BE::TempCurHullA[X] - BE::DamageLevel;
                            if (BE::Hull < 0)
                            {
                                BE::Hull = 0;
                            }

                            if (BE::MaxShieldA[X] == 0 || BE::Shields == 0)
                            {
                                BE::ShieldsPercent = 0;
                            }
                            else
                            {
                                BE::ShieldsPercent = BE::Shields * 100 / BE::MaxShieldA[X];
                            }

                            if (BE::MaxHullA[X] == 0 || BE::Hull == 0)
                            {
                                BE::HullPercent = 0;
                            }
                            else
                            {
                                BE::HullPercent = 100 - (BE::Hull * 100) / BE::MaxHullA[X];
                            }
                        }

                        /*
                         * OK, if we got this far we have inflicted some damage.  Possibly even ZERO damage
                         * due to shield absorption.  But, it's enough to possibly trigger a special effect.
                         *
                         * Special attribute flags
                         *      1   sa_dis
                         *      2   sa_heat
                         *      4   sa_meson
                         *      8   sa_vibro
                         *     16   sa_multi    scatter pack (user for targetting)
                         *     32   sa_crack
                         *     64   sa_bp       boarding party
                         *    128   sa_pen
                         */

                        // TODO: Change all of these to local scope variables
                        BE::Crits = 0;
                        BE::CRIT_DIS = 0;
                        BE::CRIT_HEAT = 0;
                        BE::CRIT_MESON = 0;
                        BE::CRIT_VIBRO = 0;
                        BE::CRIT_BP = 0;
                        BE::CRIT_SPECIAL = 0;

                        // It's possible to have multiple special effect weapons, so each is rolled separately
                        // Yes.  That means a heat/dis weapon has a 40% chance of causing a crit.  20% for each crit type.
                        if (BE::Attacks[E].Weapon > 0)
                        {
                            BE::dice = rand() % 10;
                            if (BE::dice < 2 && (BE::Attacks[E].Weapon & BE::saDis) == BE::saDis)
                            {
                                BE::CRIT_DIS = 1;
                            }

                            BE::dice = rand() % 10;
                            if (BE::dice < 2 && (BE::Attacks[E].Weapon & BE::saHeat) == BE::saHeat)
                            {
                                BE::CRIT_HEAT = 1;
                            }

                            BE::dice = rand() % 10;
                            if (BE::dice < 2 && (BE::Attacks[E].Weapon & BE::saMeson) == BE::saMeson)
                            {
                                BE::CRIT_MESON = 1;
                            }

                            BE::dice = rand() % 10;
                            if (BE::dice < 2 && (BE::Attacks[E].Weapon & BE::saVibro) == BE::saVibro)
                            {
                                BE::CRIT_VIBRO = 1;
                            }

                            BE::dice = rand() % 10;
                            if (BE::dice < 2 && (BE::Attacks[E].Weapon & BE::saSpecial) == BE::saSpecial)
                            {
                                BE::CRIT_SPECIAL = 1;
                            }

                            // Boarding parties always crit if they hit
                            if ((BE::Attacks[E].Weapon & BE::saBp) == BE::saBp)
                            {
                                BE::CRIT_BP = 1;
                            }
                        }

                        // Figure the damage threshold
                        // [JLL] TODO: tmp is last references almost 100 lines above.  This needs a rename.
                        if (tmp >= 0 && tmp <= 19)
                        {
                            if (BE::HullPercent >= 20)
                            {
                                BE::Crits = 1;
                                if (BE::HullPercent >= 40)
                                {
                                    BE::Crits = 2;
                                    if (BE::HullPercent >= 60)
                                    {
                                        BE::Crits = 3;
                                        if (BE::HullPercent >= 80)
                                        {
                                            BE::Crits = 4;
                                        }
                                    }
                                }
                            }
                        }
                        else if (tmp >= 20 && tmp <= 39)
                        {
                            if (BE::HullPercent >= 40)
                            {
                                BE::Crits = 1;
                                if (BE::HullPercent >= 60)
                                {
                                    BE::Crits = 2;
                                    if (BE::HullPercent >= 80)
                                    {
                                        BE::Crits = 3;
                                    }
                                }
                            }
                        }
                        else if (tmp >= 40 && tmp <= 59)
                        {
                            if (BE::HullPercent >= 60)
                            {
                                BE::Crits = 1;
                                if (BE::HullPercent >= 80)
                                {
                                    BE::Crits = 2;
                                }
                            }
                        }
                        else if (tmp >= 60 && tmp <= 79)
                        {
                            if (BE::HullPercent >= 80)
                            {
                                BE::Crits = 1;
                            }
                        }

                        BE::Crits = BE::Crits + BE::CRIT_DIS + BE::CRIT_HEAT + BE::CRIT_MESON + BE::CRIT_VIBRO + BE::CRIT_BP + BE::CRIT_SPECIAL; // Remeber, these are not flags just counters

                        temp_str = "";
                        BE::CriticalStr = "";

                        if (ForceID == 0)
                        {
                            // Fighters and ground units ignore the crit hit chart
                            if (IsFighter(BE::SpecialB[X]) || IsGround(BE::SpecialB[X]) || IsMine(BE::SpecialB[X]))
                            {
                                // Fighters are fragile and any damage destroys them
                                if (IsFighter(BE::SpecialB[X]) && BE::DamageLevel > 0)
                                {
                                    BE::ShipCritStr = "Fighter Destroyed";
                                    temp_str = "";
                                    BE::Hull = 0;
                                }
                                if (IsGround(BE::SpecialB[X]) && BE::Hull == 0)
                                {
                                    BE::ShipCritStr = "Unit Destroyed";
                                    temp_str = "";
                                }
                                if (IsMine(BE::SpecialB[X]) && BE::Hull == 0)
                                {
                                    BE::ShipCritStr = "Mine Destroyed";
                                }
                            }
                            else
                            {
                                if (BE::Crits > 0)
                                {
                                    BE::ReactorBreachFlag = 0;
                                    for (int C = 0; C < BE::Crits; C++)
                                    {
                                        BE::CritDamageFlag = 0;
                                        BE::CritSpecialFlag = 0;
                                        std::pair<std::string, long> critInfo;

                                        // Call the critical hit generator function
                                        if (BE::ReactorBreachFlag != 1)
                                        {
                                            if (BE::CRIT_DIS == 1)
                                            {
                                                BE::CRIT_DIS = 0;
                                                critInfo = critTables[1].getRandomOutput();
                                            }
                                            else if (BE::CRIT_HEAT == 1)
                                            {
                                                BE::CRIT_HEAT = 0;
                                                critInfo = critTables[2].getRandomOutput();
                                            }
                                            else if (BE::CRIT_MESON == 1)
                                            {
                                                BE::CRIT_MESON = 0;
                                                critInfo = critTables[3].getRandomOutput();
                                            }
                                            else if (BE::CRIT_VIBRO == 1)
                                            {
                                                BE::CRIT_VIBRO = 0;
                                                critInfo = critTables[4].getRandomOutput();
                                            }
                                            else if (BE::CRIT_SPECIAL == 1)
                                            {
                                                // TODO: There can only be one special tag.  Expand this to any number of special tags
                                                // FIXME: Throw an error if the specialIndex is not found.
                                                BE::CRIT_SPECIAL = 0;
                                                int specialIndex = HasSpecialWT(BE::Attacks[E].Special);
                                                if (specialIndex > 0)
                                                {
                                                    critInfo = critTables[specialIndex].getRandomOutput();
                                                }
                                            }
                                            else if (IsVehicle(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[11].getRandomOutput();
                                            }
                                            else if (IsBio(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[10].getRandomOutput();
                                            }
                                            else if (IsVolatile(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[9].getRandomOutput();
                                            }
                                            else if (IsOrbital(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[8].getRandomOutput();
                                            }
                                            else if (IsCarrier(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[7].getRandomOutput();
                                            }
                                            else if (IsBuilding(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[6].getRandomOutput();
                                            }
                                            else
                                            {
                                                critInfo = critTables[5].getRandomOutput();
                                            }
                                            // TODO: Orbital crit table overrides carrier crit table.  This means bases with hangars aren't carriers...

                                            BE::CriticalStr = critInfo.first;
                                            damageFile << BE::DefRaceName << " " << BE::DefShipStr[X] << " " << BE::CriticalStr << "\n"; // TODO: Double check that #4 is damage file

                                            if (BE::CRIT_BP == 1 && BE::CRIT_DIS == 0 && BE::CRIT_HEAT == 0 && BE::CRIT_MESON == 0 && BE::CRIT_VIBRO == 0)
                                            {
                                                BE::CRIT_BP = 0;
                                                BE::CriticalStr = "BP: " + BE::CriticalStr;
                                            }
                                        }

                                        long critType = critInfo.second;
                                        if ((critType > 0 && critType < 11) || critType == 100)
                                        {
                                            BE::CritDamageFlag = critType;
                                        }
                                        else if (critType > 10 && critType < 100)
                                        {
                                            BE::CritSpecialFlag = critType - 10;
                                        }
                                        else
                                        {
                                            // TODO: Add some sort of error or debug code here
                                        }

                                        // constant CRIT_WEAPON_ONE   1   Weapons Offline for 1 turn
                                        // constant CRIT_WEAPON_HALF  2   Half Weapons Offline until repaired
                                        // constant CRIT_WEAPON_NONE  3   All Weapons Offline until repaired
                                        // constant CRIT_DRIFTING     4   No movement for 1 turn
                                        // constant CRIT_NOMOVE       5   No movement until repaired
                                        // constant CRIT_CRIPPLE      6   Crippled
                                        // constant CRIT_SHIELDS      7   Shields Offline
                                        // constant CRIT_AMMO         8   Ammo explosion
                                        // constant CRIT_CREW1        9   5% crew casualties
                                        // constant CRIT_CREW2       10   10% crew casualties
                                        // constant CRIT_CREW3       11   25% crew casualties + cripple

                                        switch (BE::CritSpecialFlag)
                                        {
                                        case 1:
                                            BE::TempSpecialB[X] = InsertOffline(BE::TempSpecialB[X]);
                                            break;
                                        case 2:
                                            BE::TempSpecialB[X] = InsertRandomOffline(BE::TempSpecialB[X]);
                                            break;
                                        case 3:
                                            BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "CRIPPLE");
                                            break;
                                        case 4:
                                            BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "DRIFTING");
                                            break;
                                        case 5:
                                            BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "NOMOVE");
                                            break;
                                        case 6:
                                            BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "CRIPPLE");
                                            BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "NOMOVE");
                                            break;
                                        case 7:
                                            BE::TempCurShieldB[X] = 0;
                                            BE::Shields = 0;
                                            BE::ShieldsPercent = 0;
                                            break;
                                        case 8:
                                            // TODO: Need to change this as the torp field is going away
                                            BE::CritDamageFlag = BE::TempCurTorpB[X];
                                            break;
                                        case 9:
                                            // 5% Crew Casualties
                                            BE::TempSpecialB[X] = DoCrewDamage(BE::TempSpecialB[X], 5);
                                            break;
                                        case 10:
                                            BE::TempSpecialB[X] = DoCrewDamage(BE::TempSpecialB[X], 10);
                                            break;
                                        case 11:
                                            BE::TempSpecialB[X] = DoCrewDamage(BE::TempSpecialB[X], 25);
                                            BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "CRIPPLE");
                                        default:
                                            // TODO: Add some error/debug handling here
                                            break;
                                        }

                                        BE::Hull -= BE::CritDamageFlag;
                                        if (BE::Hull < 1 || BE::TempCurHullB[X] < 1)
                                        {
                                            BE::HullPercent = 100;
                                            BE::Hull = 0;
                                        }
                                        else
                                        {
                                            BE::HullPercent = 100 - ((BE::Hull * 100) / BE::TempCurHullB[X]);
                                        }
                                        if (BE::CritDamageFlag == 100)
                                        {
                                            BE::Crits = 1;
                                            break;
                                        }
                                    }
                                } // End crits code
                                if (BE::Hull == 0)
                                {
                                    if (IsMissile(BE::SpecialB[X]))
                                    {
                                        BE::ShipCritStr = "Missile Destroyed.";
                                    }
                                    else if (IsBuilding(BE::SpecialB[X]) || IsOrbital(BE::SpecialB[X]))
                                    {
                                        BE::ShipCritStr = "Building Destroyed";
                                    }
                                    else
                                    {
                                        BE::ShipCritStr = "Unit Destroyed.";
                                    }
                                }
                            } // End of IsFighter code

                            if (BE::Hull == 0)
                            {
                                BE::Shields = 0;
                                BE::TempCurBeamB[X] = 0; // TODO: Can be removed
                                BE::TempCurTorpB[X] = 0; // TODO: Can be removed
                            }
                            if (BE::Hull > 0 && BE::BPAttackCritB[X] > 99)
                            {
                                if (IsBuilding(BE::SpecialB[X]) || IsOrbital(BE::SpecialB[X]))
                                {
                                    BE::ShipCritStr = "Base Captured.";
                                }
                                else
                                {
                                    BE::ShipCritStr = "Ship Captured."; // TODO: Can this be Unit instead of Ship
                                }
                                BE::TempSpecialB[X] = AddTag(BE::TempSpecialB[X], "CAPTURED");
                            }
                            BE::DefCritStr[X] = BE::CriticalStr;
                            if (BE::DefCritStr[X] != "")
                            {
                                reportFile << "    " + BE::DefCritStr[X] + "\n";
                            }
                            BE::TempStr = "  Bm=" + to_string(BE::TempCurBeamB[X]) + " Sh=" + to_string(BE::Shields) + " Tp=" + to_string(BE::TempCurTorpB[X]) + " Hl=" + to_string(BE::Hull) + " " + BE::ShipCritStr;
                            BE::TempCurDamB[X] = BE::HullPercent;
                            if (BE::ShipCritStr != "")
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] X=" << X << ";ForceId=" << ForceID << endl;
                                CBE::debugFile << "[INFO] " << BE::DefRaceName << " " << BE::DefShipStr[X] << " " << BE::ShipCritStr << endl;
#endif
                                damageFile << BE::DefRaceName + " " + BE::DefShipStr[X] + " " + BE::ShipCritStr << "\n";
                            }
                            BE::TempCurShieldB[X] = BE::Shields;
                            BE::TempCurHullB[X] = BE::Hull;
                        }
                        else // Defenders
                        {
                            // Fighters and ground units ignore the crit hit chart
                            if (IsFighter(BE::SpecialA[X]) || IsGround(BE::SpecialA[X]) || IsMine(BE::SpecialA[X]))
                            {
                                // Fighters are fragile and any damage destroys them
                                if (IsFighter(BE::SpecialA[X]) && BE::DamageLevel > 0)
                                {
                                    BE::ShipCritStr = "Fighter Destroyed";
                                    temp_str = "";
                                    BE::Hull = 0;
                                }
                                if (IsGround(BE::SpecialA[X]) && BE::Hull == 0)
                                {
                                    BE::ShipCritStr = "Unit Destroyed";
                                    temp_str = "";
                                }
                                if (IsMine(BE::SpecialA[X]) && BE::Hull == 0)
                                {
                                    BE::ShipCritStr = "Mine Destroyed";
                                }
                            }
                            else
                            {
                                if (BE::Crits > 0)
                                {
                                    BE::ReactorBreachFlag = 0;
                                    for (int C = 0; C < BE::Crits; C++)
                                    {
                                        BE::CritDamageFlag = 0;
                                        BE::CritSpecialFlag = 0;
                                        std::pair<std::string, long> critInfo;

                                        // Call the critical hit generator function
                                        if (BE::ReactorBreachFlag != 1)
                                        {
                                            if (BE::CRIT_DIS == 1)
                                            {
                                                BE::CRIT_DIS = 0;
                                                critInfo = critTables[1].getRandomOutput();
                                            }
                                            else if (BE::CRIT_HEAT == 1)
                                            {
                                                BE::CRIT_HEAT = 0;
                                                critInfo = critTables[2].getRandomOutput();
                                            }
                                            else if (BE::CRIT_MESON == 1)
                                            {
                                                BE::CRIT_MESON = 0;
                                                critInfo = critTables[3].getRandomOutput();
                                            }
                                            else if (BE::CRIT_VIBRO == 1)
                                            {
                                                BE::CRIT_VIBRO = 0;
                                                critInfo = critTables[4].getRandomOutput();
                                            }
                                            else if (BE::CRIT_SPECIAL == 1)
                                            {
                                                // TODO: There can only be one special tag.  Expand this to any number of special tags
                                                // FIXME: Throw an error if the specialIndex is not found.
                                                BE::CRIT_SPECIAL = 0;
                                                int specialIndex = HasSpecialWT(BE::Attacks[E].Special);
                                                if (specialIndex > 0)
                                                {
                                                    critInfo = critTables[specialIndex].getRandomOutput();
                                                }
                                            }
                                            else if (IsVehicle(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[11].getRandomOutput();
                                            }
                                            else if (IsBio(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[10].getRandomOutput();
                                            }
                                            else if (IsVolatile(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[9].getRandomOutput();
                                            }
                                            else if (IsOrbital(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[8].getRandomOutput();
                                            }
                                            else if (IsCarrier(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[7].getRandomOutput();
                                            }
                                            else if (IsBuilding(BE::SpecialB[X]))
                                            {
                                                critInfo = critTables[6].getRandomOutput();
                                            }
                                            else
                                            {
                                                critInfo = critTables[5].getRandomOutput();
                                            }
                                            // TODO: Orbital crit table overrides carrier crit table.  This means bases with hangars aren't carriers...

                                            BE::CriticalStr = critInfo.first;
                                            damageFile << BE::AttRaceName << " " << BE::AttShipStr[X] << " " << BE::CriticalStr << "\n"; // TODO: Double check that #4 is damage file

                                            if (BE::CRIT_BP == 1 && BE::CRIT_DIS == 0 && BE::CRIT_HEAT == 0 && BE::CRIT_MESON == 0 && BE::CRIT_VIBRO == 0)
                                            {
                                                BE::CRIT_BP = 0;
                                                BE::CriticalStr = "BP: " + BE::CriticalStr;
                                            }
                                        }

                                        long critType = critInfo.second;
                                        if ((critType > 1 && critType < 11) || critType == 100)
                                        {
                                            BE::CritDamageFlag = critType;
                                        }
                                        else if (critType > 10 && critType < 100)
                                        {
                                            BE::CritSpecialFlag = critType - 10;
                                        }
                                        else
                                        {
                                            // TODO: Add some sort of error or debug code here
                                        }

                                        // constant CRIT_WEAPON_ONE   1   Weapons Offline for 1 turn
                                        // constant CRIT_WEAPON_HALF  2   Half Weapons Offline until repaired
                                        // constant CRIT_WEAPON_NONE  3   All Weapons Offline until repaired
                                        // constant CRIT_DRIFTING     4   No movement for 1 turn
                                        // constant CRIT_NOMOVE       5   No movement until repaired
                                        // constant CRIT_CRIPPLE      6   Crippled
                                        // constant CRIT_SHIELDS      7   Shields Offline
                                        // constant CRIT_AMMO         8   Ammo explosion
                                        // constant CRIT_CREW1        9   5% crew casualties
                                        // constant CRIT_CREW2       10   10% crew casualties
                                        // constant CRIT_CREW3       11   25% crew casualties + cripple

                                        switch (BE::CritSpecialFlag)
                                        {
                                        case 1:
                                            BE::TempSpecialA[X] = InsertOffline(BE::TempSpecialA[X]);
                                            break;
                                        case 2:
                                            BE::TempSpecialA[X] = InsertRandomOffline(BE::TempSpecialA[X]);
                                            break;
                                        case 3:
                                            BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "CRIPPLE");
                                            break;
                                        case 4:
                                            BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "DRIFTING");
                                            break;
                                        case 5:
                                            BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "NOMOVE");
                                            break;
                                        case 6:
                                            BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "CRIPPLE");
                                            BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "NOMOVE");
                                            break;
                                        case 7:
                                            BE::TempCurShieldA[X] = 0;
                                            BE::Shields = 0;
                                            BE::ShieldsPercent = 0;
                                            break;
                                        case 8:
                                            // TODO: Need to change this as the torp field is going away
                                            BE::CritDamageFlag = BE::TempCurTorpB[X];
                                            break;
                                        case 9:
                                            // 5% Crew Casualties
                                            BE::TempSpecialA[X] = DoCrewDamage(BE::TempSpecialA[X], 5);
                                            break;
                                        case 10:
                                            BE::TempSpecialA[X] = DoCrewDamage(BE::TempSpecialA[X], 10);
                                            break;
                                        case 11:
                                            BE::TempSpecialA[X] = DoCrewDamage(BE::TempSpecialA[X], 25);
                                            BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "CRIPPLE");
                                        default:
                                            // TODO: Add some error/debug handling here
                                            break;
                                        }

                                        BE::Hull -= BE::CritDamageFlag;
                                        if (BE::Hull < 1 || BE::TempCurHullA[X] < 1)
                                        {
                                            BE::HullPercent = 100;
                                            BE::Hull = 0;
                                        }
                                        else
                                        {
                                            BE::HullPercent = 100 - ((BE::Hull * 100) / BE::TempCurHullA[X]);
                                        }
                                        if (BE::CritDamageFlag == 100)
                                        {
                                            BE::Crits = 1;
                                            break;
                                        }
                                    }
                                } // End crits code
                                if (BE::Hull == 0)
                                {
                                    if (IsMissile(BE::SpecialA[X]))
                                    {
                                        BE::ShipCritStr = "Missile Destroyed.";
                                    }
                                    else if (IsBuilding(BE::SpecialA[X]) || IsOrbital(BE::SpecialA[X]))
                                    {
                                        BE::ShipCritStr = "Building Destroyed";
                                    }
                                    else
                                    {
                                        BE::ShipCritStr = "Unit Destroyed.";
                                    }
                                }
                            } // End of IsFighter code

                            if (BE::Hull == 0)
                            {
                                BE::Shields = 0;
                                BE::TempCurBeamA[X] = 0; // TODO: Can be removed
                                BE::TempCurTorpA[X] = 0; // TODO: Can be removed
                            }
                            if (BE::Hull > 0 && BE::BPAttackCritA[X] > 99)
                            {
                                if (IsBuilding(BE::SpecialA[X]) || IsOrbital(BE::SpecialA[X]))
                                {
                                    BE::ShipCritStr = "Base Captured.";
                                }
                                else
                                {
                                    BE::ShipCritStr = "Ship Captured."; // TODO: Can this be Unit instead of Ship
                                }
                                BE::TempSpecialA[X] = AddTag(BE::TempSpecialA[X], "CAPTURED");
                            }
                            BE::AttCritStr[X] = BE::CriticalStr;
                            if (BE::AttCritStr[X] != "")
                            {
                                reportFile << "    " + BE::AttCritStr[X] + "\n";
                            }
                            BE::TempStr = "  Bm=" + to_string(BE::TempCurBeamA[X]) + " Sh=" + to_string(BE::Shields) + " Tp=" + to_string(BE::TempCurTorpA[X]) + " Hl=" + to_string(BE::Hull) + " " + BE::ShipCritStr;
                            BE::TempCurDamA[X] = BE::HullPercent;
                            if (BE::ShipCritStr != "")
                            {
#ifdef CBE_DEBUG
                                CBE::debugFile << "[INFO] X=" << X << ";ForceId=" << ForceID << endl;
                                CBE::debugFile << "[INFO] " << BE::AttRaceName << " " << BE::AttShipStr[X] << " " << BE::ShipCritStr << endl;
#endif
                                damageFile << BE::AttRaceName + " " + BE::AttShipStr[X] + " " + BE::ShipCritStr << "\n";
                            }
                            BE::TempCurShieldA[X] = BE::Shields;
                            BE::TempCurHullA[X] = BE::Hull;
                        }
                    }
                    // End if ship was shot.
                }
                // Next E
                if (ShipHit == 1)
                {
                    reportFile << BE::TempStr << "\n"; // TODO: Make consistant
                }
            } // Next A
            reportFile << "\n";

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO][End of Round] Applying combat - Copying temp values to real values" << endl;
#endif

            // Copy real values from temp fields so that combat will be applied
            BE::BO_AttackTotal = 0;
            for (int A = 0; A < BE::AttShipsLeft; A++)
            {
                BE::AttCritStr[A] = BE::TempAttCritStr[A];
                BE::CurBeamA[A] = BE::TempCurBeamA[A];
                BE::CurShieldA[A] = BE::TempCurShieldA[A];
                BE::CurTorpA[A] = BE::TempCurTorpA[A];
                BE::CurHullA[A] = BE::TempCurHullA[A];
                BE::HitsA[A] = BE::TempHitsA[A];
                BE::PenHitsA[A] = BE::TempPenHitsA[A];
                BE::CurDamA[A] = BE::TempCurDamA[A];
                BE::SpecialA[A] = BE::TempSpecialA[A];
                if (!IsCaptured(BE::SpecialA[A]) && !IsFled(BE::SpecialA[A]) && !IsMissile(BE::SpecialA[A]))
                {
                    BE::BO_AttackTotal += BE::CurHullA[A];
                }
            }

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO][End of Round] Remaining Attack Fleet Strength: " << BE::BO_AttackTotal << endl;
#endif

            BE::BO_DefenseTotal = 0;
            for (int A = 0; A < BE::DefShipsLeft; A++)
            {
                BE::DefCritStr[A] = BE::TempDefCritStr[A];
                BE::CurBeamB[A] = BE::TempCurBeamB[A];
                BE::CurShieldB[A] = BE::TempCurShieldB[A];
                BE::CurTorpB[A] = BE::TempCurTorpB[A];
                BE::CurHullB[A] = BE::TempCurHullB[A];
                BE::HitsB[A] = BE::TempHitsB[A];
                BE::PenHitsB[A] = BE::TempPenHitsB[A];
                BE::CurDamB[A] = BE::TempCurDamB[A];
                BE::SpecialB[A] = BE::TempSpecialB[A];
                if (!IsCaptured(BE::SpecialB[A]) && !IsFled(BE::SpecialB[A]) && !IsMissile(BE::SpecialB[A]))
                {
                    BE::BO_DefenseTotal += BE::CurHullB[A];
                }
            }

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO][End of Round] Remaining Defense Fleet Strength: " << BE::BO_DefenseTotal << endl;
#endif

            // Remove cloaking
            if (BE::CombatRound == 1)
            {
                for (int i = 0; i < BE::AttShipsLeft; i++)
                {
                    BE::SpecialA[i] = RemoveTag(BE::SpecialA[i], "CLOAK", 0);
                }
                for (int i = 0; i < BE::AttShipsLeft; i++)
                {
                    BE::SpecialB[i] = RemoveTag(BE::SpecialB[i], "CLOAK", 0);
                }
            }

            // Printing
            reportFile << "Damage Results:\n\n";
            reportFile << BE::AttRaceName + " " + BE::GroupName + " Damage:\n";
            reportFile << BE::AttDamageStr;
            if (BE::AttFleetStrength == 0)
            {
                BE::BO_AttackPercent = 0;
            }
            else
            {
                BE::BO_AttackPercent = (BE::BO_AttackTotal * 100) / BE::AttFleetStrength;
            }
            BE::BO_Att = 100 - BE::BO_AttackPercent;
            BE::TempStr = "Current Damage Level: " + to_string(BE::BO_Att) + "% (" + to_string(BE::BO_AttackTotal) + "/" + to_string(BE::AttFleetStrength) + ")\n";
            BE::BO_AttackTotal = 0;
            reportFile << BE::TempStr;

            for (int A = 0; A < BE::AttShipsLeft; A++)
            {
                // Only do things to ships that are not dead
                if (BE::CurHullA[A] > 0)
                {
                    // Do shield regen
                    std::pair<long, long> regen = GetRegenValues(BE::SpecialA[A]); // Get the regen values as a pair.  first is shield, second is hull
                    if (BE::CurShieldA[A] < BE::MaxShieldA[A] && BE::CurShieldA[A] > 0 && regen.first > 0)
                    {
                        BE::CurShieldA[A] += regen.first;
                        reportFile << "  " + BE::AttShipStr[A] + " - Shield regeneration detected.\n";
                        if (BE::CurShieldA[A] > BE::MaxShieldA[A])
                        {
                            BE::CurShieldA[A] = BE::MaxShieldA[A];
                        }
                    }
                    // Do hull regen
                    if (BE::CurHullA[A] < BE::MaxHullA[A] && regen.second > 0)
                    {
                        BE::CurHullA[A] += regen.second;
                        reportFile << "  " + BE::AttShipStr[A] + " - Hull regeneration detected.\n";
                        if (BE::CurHullA[A] > BE::MaxHullA[A])
                        {
                            BE::CurHullA[A] = BE::MaxHullA[A];
                        }
                    }

                    if (!IsNoMove(BE::SpecialA[A]) && !IsCaptured(BE::SpecialA[A]) && !IsCrippled(BE::SpecialA[A]) && !IsMissile(BE::SpecialA[A]))
                    {
                        // Do flee/fled
                        if (IsFlee(BE::SpecialA[A]) || BE::AttBreakOff == 0)
                        {
                            BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLED");
                            reportFile << "  " << BE::AttShipStr[A] << " disengages.\n";
                            BE::AttFledFlag = 1;
                        }
                        else
                        {
                            long brk = 0;
                            if (HasBreak(BE::SpecialA[A], brk))
                            {
                                if (brk == 0)
                                {
                                    // TODO: Why is the unit in fled if break is 0?  This does result in the unit leaving combat after 1 round.  Hit and run?
                                    BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLED");
                                    reportFile << "  " << BE::AttShipStr[A] << " disengages.\n";
                                    BE::AttFledFlag = 1;
                                }
                                else if (BE::BO_Att >= brk)
                                {
                                    BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                    reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                }
                            }
                            long damage = 0;
                            if (HasDamage(BE::SpecialA[A], damage))
                            {
                                // TODO: Can this be simplified?
                                if (damage < 100 && (BE::CurHullA[A] * 100 / BE::MaxHullA[A]) <= damage)
                                {
                                    BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                    reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                }
                                else if (damage == 100 && BE::CurShieldA[A] == 0)
                                {
                                    BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                    reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                }
                                else if (damage > 100 && (BE::CurShieldA[A] * 100 / BE::MaxShieldA[A]) <= (damage - 100))
                                {
                                    BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                    reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                }
                            }
                            else
                            {
                                if (BE::BO_Att >= BE::AttBreakOff)
                                {
                                    BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                    reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                }
                                else
                                {
                                    long time = 0;
                                    if (HasTime(BE::SpecialA[A], time))
                                    {
                                        if (BE::CombatRound >= time)
                                        {
                                            BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                            reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                        }
                                    }
                                    else
                                    {
                                        if (IsToothless(0, A))
                                        {
                                            BE::SpecialA[A] = AddTag(BE::SpecialA[A], "FLEE");
                                            reportFile << "  " << BE::AttShipStr[A] << " is breaking off.\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            reportFile << "\n";
            BE::DefDamageStr = BE::DefRaceName + " " + BE::GroupName + " Damage:\n";
            reportFile << BE::DefDamageStr;
            if (BE::DefFleetStrength == 0)
            {
                BE::BO_DefensePercent = 0;
            }
            else
            {
                BE::BO_DefensePercent = (BE::BO_DefenseTotal * 100) / BE::DefFleetStrength;
            }
            BE::BO_Def = 100 - BE::BO_DefensePercent;
            BE::TempStr = "Current Damage Level: " + to_string(BE::BO_Def) + "% (" + to_string(BE::BO_DefenseTotal) + "/" + to_string(BE::DefFleetStrength) + ")\n";
            BE::BO_DefenseTotal = 0;
            reportFile << BE::TempStr;

            for (int B = 0; B < BE::DefShipsLeft; B++)
            {
                // Only do things to ships that are not dead
                if (BE::CurHullB[B] > 0)
                {
                    // Do shield regen
                    std::pair<long, long> regen = GetRegenValues(BE::SpecialB[B]); // Get the regen values as a pair.  first is shield, second is hull
                    if (BE::CurShieldB[B] < BE::MaxShieldB[B] && BE::CurShieldB[B] > 0 && regen.first > 0)
                    {
                        BE::CurShieldB[B] += regen.first;
                        reportFile << BE::DefShipStr[B] + " - Shield regeneration detected.\n";
                        if (BE::CurShieldB[B] > BE::MaxShieldB[B])
                        {
                            BE::CurShieldB[B] = BE::MaxShieldB[B];
                        }
                    }
                    // Do hull regen
                    if (BE::CurHullB[B] < BE::MaxHullB[B] && regen.second > 0)
                    {
                        BE::CurHullB[B] += regen.second;
                        reportFile << BE::DefShipStr[B] + " - Hull regeneration detected.\n";
                        if (BE::CurHullB[B] > BE::MaxHullB[B])
                        {
                            BE::CurHullB[B] = BE::MaxHullB[B];
                        }
                    }

#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO][End of Round] Begin Break Off Checks for [" << B << "]: " << BE::DefShipStr[B] << endl;
#endif

                    if (!IsNoMove(BE::SpecialB[B]) && !IsCaptured(BE::SpecialB[B]) && !IsCrippled(BE::SpecialB[B]) && !IsMissile(BE::SpecialB[B]))
                    {
                        // Do flee/fled
                        if (IsFlee(BE::SpecialB[B]) || BE::DefBreakOff == 0)
                        {
                            BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLED");
                            reportFile << "  " << BE::DefShipStr[B] << " disengages.\n";
                            BE::DefFledFlag = 1;
                        }
                        else
                        {
                            long brk = 0;
                            if (HasBreak(BE::SpecialB[B], brk))
                            {
                                if (brk == 0)
                                {
                                    // TODO: Why is the unit in fled if break is 0?  This does result in the unit leaving combat after 1 round.  Hit and run?
                                    BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLED");
                                    reportFile << "  " << BE::DefShipStr[B] << " disengages.\n";
                                    BE::DefFledFlag = 1;
                                }
                                else if (BE::BO_Def >= brk)
                                {
                                    BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                    reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                }
                            }
                            long damage = 0;
                            if (HasDamage(BE::SpecialB[B], damage))
                            {
                                // TODO: Can this be simplified?
                                if (damage < 100 && (BE::CurHullB[B] * 100 / BE::MaxHullB[B]) <= damage)
                                {
                                    BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                    reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                }
                                else if (damage == 100 && BE::CurShieldB[B] == 0)
                                {
                                    BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                    reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                }
                                else if (damage > 100 && (BE::CurShieldB[B] * 100 / BE::MaxShieldB[B]) <= (damage - 100))
                                {
                                    BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                    reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                }
                            }
                            else
                            {
                                if (BE::BO_Def >= BE::DefBreakOff)
                                {
                                    BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                    reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                }
                                else
                                {
                                    long time = 0;
                                    if (HasTime(BE::SpecialB[B], time))
                                    {
                                        if (BE::CombatRound >= time)
                                        {
                                            BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                            reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                        }
                                    }
                                    else
                                    {
                                        if (IsToothless(1, B))
                                        {
                                            BE::SpecialB[B] = AddTag(BE::SpecialB[B], "FLEE");
                                            reportFile << "  " << BE::DefShipStr[B] << " is breaking off.\n";
                                        }
                                    }
                                }
                            }
                        }
                    }
#ifdef CBE_DEBUG
                    CBE::debugFile << "[INFO][End of Round] End Break Off Checks for [" << B << "]" << endl;
#endif
                }
            }

            // Start of combat done check
            AttGone = 1;
            DefGone = 1;

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO][End of Round] Checking for end of combat" << endl;
#endif

            for (int A = 0; A < BE::AttShipsLeft; A++)
            {
                if (!IsFled(BE::SpecialA[A]) && !IsCaptured(BE::SpecialA[A]) && !IsCrippled(BE::SpecialA[A]) && !IsMissile(BE::SpecialA[A]))
                {
                    if (!IsToothless(0, A))
                    {
                        AttGone = 0;
                        break;
                    }
                }
            }
            for (int B = 0; B < BE::DefShipsLeft; B++)
            {
                if (!IsFled(BE::SpecialB[B]) && !IsCaptured(BE::SpecialB[B]) && !IsCrippled(BE::SpecialB[B]) && !IsMissile(BE::SpecialB[B]))
                {
                    if (!IsToothless(1, B))
                    {
                        DefGone = 0;
                        break;
                    }
                }
            }

#ifdef CBE_DEBUG
            CBE::debugFile << "[INFO][End of Round] AttGone=" << AttGone << ",DefGone=" << DefGone << endl;
#endif

            // TODO: Combine this with the iterations above for speed?
            if (BE::CombatRound == 1)
            {
                if (AttGone == 0)
                {
                    for (int A = 0; A < BE::AttShipsLeft; A++)
                    {
                        if (IsSurprise(BE::SpecialA[A]))
                        {
                            BE::SpecialA[A] = RemoveTag(BE::SpecialA[A], "SURPRISE", 0);
                        }
                    }
                }
                if (DefGone == 0)
                {
                    for (int B = 0; B < BE::DefShipsLeft; B++)
                    {
                        if (IsSurprise(BE::SpecialB[B]))
                        {
                            BE::SpecialB[B] = RemoveTag(BE::SpecialB[B], "SURPRISE", 0);
                        }
                    }
                }
            }

            // END OF ROUND!!!!
            writeTempFiles();

            // Do end of combat checks
            if (AttGone == 1 || BE::CombatRound >= 100) // FIXME: Change the 100 to a constant that can be set
            {
                if (BE::AttShipsLeft == 0)
                {
                    BE::TempStr = "Attacking " + BE::GroupName + " gone.\n";
                }
                else
                {
                    BE::TempStr = "Attacking " + BE::GroupName + " breaks off or surrenders.\n";
                }
                BE::RetreatFlag = 1;
                reportFile << "\n";
                reportFile << BE::TempStr;
                AttackLoop = 0;
            }
            else
            {
                if (DefGone == 1)
                {
                    if (BE::DefShipsLeft == 0)
                    {
                        BE::TempStr = "Defending " + BE::GroupName + " gone.\n";
                    }
                    else
                    {
                        BE::TempStr = "Defending " + BE::GroupName + " breaks off or surrenders.\n";
                    }
                    BE::RetreatFlag = 2;
                    reportFile << "\n";
                    reportFile << BE::TempStr;
                    AttackLoop = 0;
                }
            }

            // FIXME: Memory cleanup?

            reportFile << "\n";
            reportFile.flush();
            damageFile.flush();

            if (oneStep && AttackLoop == 1)
            {
                string tempIn;
                cout << "Run another turn? (Y\\N): ";
                cin >> tempIn;
                if (tempIn == "N" || tempIn == "n")
                {
                    AttackLoop = 0;
                }
            }
        }
        else
        {
            // Handle empty fleets here
            if (BE::AttShipsLeft == 0) // FIXME: Change the 100 to a constant that can be set
            {
                reportFile << "\n";
                reportFile << "Attacking " + BE::GroupName + " gone.\n";
                AttackLoop = 0;
            }
            if (BE::DefShipsLeft == 0)
            {
                reportFile << "\n";
                reportFile << "Defending " + BE::GroupName + " gone.\n";
                AttackLoop = 0;
            }
        }
    } while (AttackLoop == 1);

    // Write Out Combat Stats
    reportFile << "\n";
    reportFile << "Surviving Ships:" << "\n";
    reportFile << "\n";

    // Deal with attacking fleet first
    if(BE::AttShipsLeft > 0) {
        // There are attacking ships left, list them
        // Uses same code as participating ships
        // Print name of fleet first
        reportFile << BE::AttRaceName << ", " << BE::AttFleetName << " " << BE::GroupName << "." << "\n";
        reportFile << "\n";
        writeRemainingUnits(reportFile,BE::AttShipsLeft,BE::AttShipStr,BE::CurBeamA,BE::CurShieldA,BE::CurTorpA,BE::CurHullA,BE::MaxHullA);
    }
    else {
        // There are no attacking ships left.
        reportFile << BE::AttRaceName << ", " << BE::AttFleetName << " " << BE::GroupName << " is gone." << "\n";
    }

    reportFile << "\n";

    // Deel with defend fleet next
    if(BE::DefShipsLeft > 0) {
        // There are defending ships left, list them
        // Uses same code as participating ships
        // Print name of fleet first
        reportFile << BE::DefRaceName << ", " << BE::DefRaceName << " " << BE::GroupName << "." << "\n";
        reportFile << "\n";
        writeRemainingUnits(reportFile,BE::DefShipsLeft,BE::DefShipStr,BE::CurBeamB,BE::CurShieldB,BE::CurTorpB,BE::CurHullB,BE::MaxHullB);
    }
    else {
        // There are no defending ships left.
        reportFile << BE::DefRaceName << ", " << BE::DefRaceName << " " << BE::GroupName << " is gone." << "\n";
    }

    reportFile << "\n";

    // List ships that are captured or unable to retreat
    // Only the retreating fleet needs to list captured/disabled ships
    if(BE::AttShipsLeft == 0) {}
    if(BE::DefShipsLeft == 0) {}

    cout << "Done with combat!" << endl;

    // FIXME: Memory cleanup?

    // Flush the files and close them.
    reportFile.flush();
    damageFile.flush();

    reportFile.close();
    damageFile.close();
}

/*
 * valid command line arguments are:
 * -a <string> - the filename for the attacking fleet csv
 * -d <string> - the filename for the defending fleet csv
 * -g <string> - the group name for this simulation (default is Fleets)
 * -u <string> - the unit name for this simulation (default is Ships)
 */
int main(int argc, char *argv[])
{
    // Seed the random number generator
    std::srand(time(0));
#ifdef CBE_DEBUG
    CBE::debugFile << "############################## DEBUG ##############################" << endl;
#endif
    int i = 1;
    string fname = "";
    bool headless = false;
    string simulationName = "";
    while (i < argc)
    {
        string cmd = argv[i];
        if (cmd == "-a")
        {
            // Move to the next argument
            i++;
            // Load the attacking fleet
            fname = argv[i];
            loadAttackingFleet(fname);
        }
        else if (cmd == "-d")
        {
            // Move to the next argument
            i++;
            // Load the defending fleet
            fname = argv[i];
            loadDefendingFleet(fname);
        }
        else if (cmd == "-hit")
        {
            // Move to the next argument
            i++;
            // Set the to hit
            CBE::baseAccuracy = stof(argv[i]);
        }
        else if (cmd == "-e" || cmd == "--fight")
        {
            // The simulation should run without user interaction
            headless = true;
        }
        else if (cmd == "-1")
        {
            oneStep = true;
        }
        else if (cmd == "-n" || cmd == "--name") {
            // The name for this simulation
            // Will be used for workflowing simulations
            i++;
            simulationName = argv[i];
        }
        else
        {
            std::cout << "Unknown command: " << cmd << endl;
        }
        i++;
    }

    // Are we running headless?
    if (headless)
    {
        // Just run the simulation without using the GUI.
        if(CBE::defenderLoaded && CBE::attackerLoaded) {
            // Commence the headless execution.
            std::cout << "Running headless simulation \"" << simulationName << "\"" << endl;
            be_main();
        }
        else {
            // Something is missing, let's see what it is
            std::cout << "Unable to continue with headless execution." << endl;
            if(!CBE::attackerLoaded) {
                std::cout << "No attacking fleet loaded." << endl;
            }
            if(!CBE::defenderLoaded) {
                std::cout << "No defending fleet loaded." << endl;
            }
        }
    }
    else
    { // Present the menu to the user
        bool done = false;
        int choice = 0;
        while (!done)
        {
            // Display the menu
            std::cout << "BattleEngine 1.1" << endl;
            if (CBE::attackerLoaded)
            {
                std::cout << "\t1: Attacking Fleet: " << BE::AttFleetStr << endl;
            }
            else
            {
                std::cout << "\t1: Load Attacking Fleet" << endl;
            }
            if (CBE::defenderLoaded)
            {
                std::cout << "\t2: Defending Fleet: " << BE::DefFleetStr << endl;
            }
            else
            {
                std::cout << "\t2: Load Defending Fleet" << endl;
            }
            std::cout << "\t3: Update Settings" << endl;
            std::cout << "\t4: One Turn" << endl;
            std::cout << "\t5: Fight!" << endl;
            std::cout << "\t6: Debug Output" << endl;
            std::cout << "\t7: Crit Test" << endl;
            std::cout << "\t0: Exit" << endl;
            std::cout << "Selection: ";

            // Wait for user input
            std::cin >> choice;

            // What did the user choose?
            std::unordered_map<int, CriticalHitTable> tables;
            std::pair<std::string, long> output;
            switch (choice)
            {
            case 1:
                // Load the attacking fleet
                std::cout << "Attackers File Name: ";
                std::cin >> fname;
                loadAttackingFleet(fname);
                break;
            case 2:
                // Load the defending fleet
                std::cout << "Defenders File Name: ";
                std::cin >> fname;
                loadDefendingFleet(fname);
                break;
            case 4:
                std::cout << "Doing one round of combat" << endl;
                break;
            case 5:
                // Do the thing!
                std::cout << "Starting combat" << endl;
                be_main();
                break;
            case 6:
                debugPrintUnits();
                break;
            case 7:
                // Test the CriticalHitTables class
                output = tables[5].getRandomOutput();
                cout << output.first << endl;
                break;
            default:
                done = true;
            }
        }

        std::cout << "Goodbye!" << endl;
    }

    // Close the debug file
    CBE::debugFile.close();

    // Return a clean run
    return 0;
}