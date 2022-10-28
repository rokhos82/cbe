#pragma once

#include <string>

using std::string;

namespace BE {
    struct AttackInfo {
        long AttackID;
        long TargetID;
        long Damage;
        long Weapon;
        string Special;
    };

    struct CombatInfo {
        long firepower;
        long special;
        string tag;
    };

    struct SalvoInfo {
        long MissileS;
        string DataStr;
    };

    // New Structs
    struct FleetInfo {
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

    struct UnitInfo{
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
}