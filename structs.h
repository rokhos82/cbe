#pragma once

#include <string>

using std::string;

namespace BE
{
    struct AttackInfo
    {
        long AttackID;
        long TargetID;
        long Damage;
        long Weapon;
        string Special;
    };

    struct CombatInfo
    {
        long firepower;
        long special;
        string tag;
    };

    struct SalvoInfo
    {
        long MissileS;
        string DataStr;
        bool valid;
    };

    // New Structs
    struct FleetInfo
    {
        string RaceName;
        string FleetName;
        long BreakOff;
        long ShipsTotal;
        long FleetStrength;
        long ShipsLeft;
        long TargetBonus;
        long TargetPriority;
        long Reserve;
    };

    struct UnitInfo
    {
        string UnitName;
        long MaxBeam;
        long CurBeam;
        long MaxShield;
        long CurShield;
        long MaxTorp;
        long CurTorp;
        long MaxHull;
        long CurHull;
        long CurDam;
        long Status;
        long Ammo;
        string Special;
    };

    // Global Variables
    extern string AttFleetStr, AttRaceName, AttFleetName;
    extern string AttBattleStr, AttDamageStr, AttRetreatStr;
    extern string DefFleetStr, DefRaceName, DefFleetName;
    extern string DefBattleStr, DefDamageStr, DefRetreatStr;

    extern long AttShipsTotal, DefShipsTotal;
    extern long AttBreakOff, AttTargetBonus;
    extern long DefBreakOff, DefTargetBonus;
    extern long AttShipsLeft, AttFleetStrength;
    extern long AttTargetPriority, AttIsCloaked;
    extern long AttIsMixed;
    extern long AttSurprised, AttReserve;
    extern long DefShipsLeft, DefFleetStrength;
    extern long DefTargetPriority, DefIsCloaked;
    extern long DefIsMixed;
    extern long DefSurprised, DefReserve;

    extern long CombatRound, RetreatFlag, AttFledFlag, DefFledFlag, ReactorBreachFlag, CritCount, firepower;

    extern double DM_ToHitA, DM_ToHitB;

    extern long A, B, C, D, E, L, X, Y, aa, bb;

    extern long Target1, Target2;
    extern long dice1, dice2, dice3;
    extern long Damage1, Damage2, Damage3, Scope;
    extern long Crits, Shields, Hull, DamageLevel;
    extern long AttHasFighters, DefHasFighters;
    extern long CritDamageFlag, CritSpecialFlag;

    extern long BO_AttackTotal, BO_AttackPercent, BO_Att;
    extern long BO_DefenseTotal, BO_DefensePercent, BO_Def;

    extern long MissileB, MissileS, MissileT, MissileH;

    extern double ShieldsPercent, HullPercent;

    extern string AttFile, DefFile, TempAFile, TempBFile;
    extern string ActiveFile, TempStr, ShipCritStr;
    extern string CriticalStr, RaceStr;

    extern long MaximumDamage, AttacksMax, AttacksIndex;
    extern AttackInfo Attacks[20000];
    extern SalvoInfo Salvos[200];

    extern string AttShipStr[9999];
    extern string DefShipStr[9999];
    extern string AttCritStr[9999];
    extern string DefCritStr[9999];

    extern long CurBeamA[9999];
    extern long CurShieldA[9999];
    extern long CurTorpA[9999];
    extern long CurHullA[9999];
    extern long HitsA[9999];
    extern long PenHitsA[9999];
    extern long StatusA[9999];
    extern long AmmoA[9999];
    extern long MaxBeamA[9999];
    extern long MaxShieldA[9999];
    extern long MaxTorpA[9999];
    extern long MaxHullA[9999];
    extern long CurDamA[9999];
    extern string SpecialA[9999];
    extern long BPAttackCritA[9999];

    extern long CurBeamB[9999];
    extern long CurShieldB[9999];
    extern long CurTorpB[9999];
    extern long CurHullB[9999];
    extern long HitsB[9999];
    extern long PenHitsB[9999];
    extern long StatusB[9999];
    extern long AmmoB[9999];
    extern long MaxBeamB[9999];
    extern long MaxShieldB[9999];
    extern long MaxTorpB[9999];
    extern long MaxHullB[9999];
    extern long CurDamB[9999];
    extern string SpecialB[9999];
    extern long BPAttackCritB[9999];
    extern long ValidTargets[9999];

    extern string TempAttCritStr[9999];
    extern string TempDefCritStr[9999];

    extern long TempCurBeamA[9999];
    extern long TempCurShieldA[9999];
    extern long TempCurTorpA[9999];
    extern long TempCurHullA[9999];
    extern long TempHitsA[9999];
    extern long TempPenHitsA[9999];
    extern long TempStatusA[9999];
    extern long TempCurDamA[9999];
    extern string TempSpecialA[9999];

    extern long TempCurBeamB[9999];
    extern long TempCurShieldB[9999];
    extern long TempCurTorpB[9999];
    extern long TempCurHullB[9999];
    extern long TempHitsB[9999];
    extern long TempPenHitsB[9999];
    extern long TempStatusB[9999];
    extern long TempCurDamB[9999];
    extern string TempSpecialB[9999];

    extern string terrain[7];
    extern long terrain_def[7];
    extern string terrain_special[7];

    extern long LocationA[9999];
    extern long LocationB[9999];
    extern long OrdersA[9999];
    extern long OrdersB[9999];

    extern long CRIT_DIS, CRIT_HEAT, CRIT_MESON;
    extern long CRIT_VIBRO, CRIT_BP, CRIT_SPECIAL;

    extern long dice;
    extern long ret;

    extern bool LoadA_ready, LoadD_ready;
    extern bool AttHasLongRange, DefHasLongRange;
    extern string GroupName, UnitName;
}