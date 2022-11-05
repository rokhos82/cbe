Attribute VB_Name = "Module1"
' Fire On The Suns Battle Engine
' Copyright 1991, 1997, 1998, 1999, 2000, 2001 and 2002 by
' Gregory W. Ellis and Robert Boettcher
'
' Code updated and ported to VB by Todd Zircher, 1998
'
' TO_DO LIST...
'
'   add the SWEEPER tag     (searches for MINE tags)
'
'   add the AA/aa tag       (weapon can not penetrate shields and might
'                            add search for unshielded targets such as
'                            damaged ships, fighters, squadrons, space
'                            monsters, mines, etc.)
'
'   add the SQUADRON tag
'
'   target load balancing   (make it so that 18 fighters in a squadron
'                            are just as big of a target as 18 fighters
'                            in a loose formation)
'
'   add HOT and SINK tag for BTech support
'
'   add BALANCE tag that causes breakoffs depending on the enemy to
'     friendly hull point total ratio
'
'   add KILL/kill for crew tracking, crewless units,
'     units immune to hull breaches, and weapons specifically to cause
'     crew casualties.
'
'   add a BE summary report
'
'     # of Ships on Side A Breakoff %
'     # of Ships on Side B Breakoff %
'     Total Ships Engaged Total Hull Pts Engaged
'     # of Ships on Side A Destroyed
'     # of Ships on Side B Destroyed
'     # of Ships on Side A Fled
'     # of Ships on Side B Fled
'     # of Hull Pts on Side A Destroyed % of Total Engaged
'     # of Hull Pts on Side B Destroyed % of Total Engaged
'
'     Summary Post-Battle Ship Listing for Side A
'     Summary Post-Battle Ship Listing for Side B
'      (list crits on surviving ships)

Option Explicit ' Force explicit variable declaration.
                
Option Base 1   ' Just a reminder that arrays begin at element one (QB's default)

Public Const CRIT_WEAPON_ONE As Long = 1   ' Weapons Offline for 1 turn
Public Const CRIT_WEAPON_HALF As Long = 2  ' Half Weapons Offline until repaired
Public Const CRIT_WEAPON_NONE As Long = 3  ' All Weapons Offline until repaired
Public Const CRIT_DRIFTING As Long = 4     ' No movement for 1 turn
Public Const CRIT_NOMOVE As Long = 5       ' No movement until repaired
Public Const CRIT_CRIPPLE As Long = 6      ' Crippled
Public Const CRIT_SHIELDS As Long = 7      ' Shields Offline
Public Const CRIT_AMMO As Long = 8         ' Ammo explosion
Public Const CRIT_CREW1 As Long = 9        ' crew +5%
Public Const CRIT_CREW2 As Long = 10       ' crew +10%
Public Const CRIT_CREW3 As Long = 11       ' crew +25% and cripple

' Status codes  (FLAGS)
Public Const valNormal = 0        ' no special capabilities
Public Const valFighter = 1       ' no crits, just dies
Public Const valReserve = 2       ' hides behind fighters & escorts
Public Const valSurprise = 4      ' can not fire on the first round
Public Const valCloaked = 8       ' can force a first strike advantage
Public Const valMissile = 16      ' can force a first strike against non-cloaked units
Public Const valMovement = 32     ' can not retreat until repaired
Public Const valNoBeam = 64       ' can not fire beam weapons for one round
Public Const valNoTorp = 128      ' can not fire torpedoes for one round
Public Const valCripple = 256     ' no weapons fire until repaired
Public Const valFlak = 512        ' divides its firepower into 1 point blocks vs fighters
Public Const valMulti = 1024      ' divides its firepower into 3 point blocks
Public Const valAmmo = 2048       ' unit can only use its torps for x number of turns
Public Const valCracker = 4096    ' x2 damage versus shielded targets only
Public Const valStasis = 8192     ' stasis snap shields
Public Const valPen = 16384       ' attack may do penetrating damage
Public Const valSuicide = 32768   ' suicidal attack

Public AttFleetStr As String, AttRaceName As String, AttFleetName As String
Public AttBattleStr As String, AttDamageStr As String, AttRetreatStr As String
Public DefFleetStr  As String, DefRaceName As String, DefFleetName As String
Public DefBattleStr As String, DefDamageStr As String, DefRetreatStr As String
Public edit_text  As String

Public AttShipsTotal As Long, DefShipsTotal As Long
Public AttBreakOff As Long, AttTargetBonus As Long
Public DefBreakOff As Long, DefTargetBonus As Long
Public AttShipsLeft As Long, AttFleetStrength As Long
Public AttTargetPriority As Long, AttIsCloaked As Long
Public AttIsMixed As Long
Public AttSurprised As Long, AttReserve As Long
Public DefShipsLeft As Long, DefFleetStrength As Long
Public DefTargetPriority As Long, DefIsCloaked As Long
Public DefIsMixed As Long
Public DefSurprised As Long, DefReserve As Long

Dim CombatRound As Long, RetreatFlag As Long, AttFledFlag As Long
Dim DefFledFlag As Long, ReactorBreachFlag As Long, CritCount As Long
Dim firepower As Long

Dim DM_ToHitA As Double, DM_ToHitB As Double

Dim A As Long, B As Long, C As Long, D As Long, E As Long, L As Long
Dim X As Long, Y As Long, aa As Long, bb As Long
Dim Target1 As Long, Target2 As Long
Dim dice1 As Long, dice2 As Long, dice3 As Long
Dim Damage1 As Long, Damage2 As Long, Damage3 As Long, Scope As Long
Dim Crits As Long, Shields As Long, Hull As Long, DamageLevel As Long
Dim AttHasFighters As Long, DefHasFighters As Long
Dim CritDamageFlag As Long, CritSpecialFlag As Long

Dim BO_AttackTotal As Long, BO_AttackPercent As Long, BO_Att As Long
Dim BO_DefenseTotal As Long, BO_DefensePercent As Long, BO_Def As Long

Dim MissileB As Long, MissileS As Long, MissileT As Long, MissileH As Long
   
Dim ShieldsPercent As Single, HullPercent As Single
  
Dim AttFile As String, DefFile As String, TempAFile As String, TempBFile As String
Dim ActiveFile As String, TempStr As String, ShipCritStr As String
Dim CriticalStr As String, RaceStr As String
   
' Probably never use this as it's too much work
'
'Private Type ShipInfo
'  ShipStr As String
'  CritStr As String
'
'  MaxBeam As long
'  CurBeam As long
'  MaxShield As long
'  CurShield As long
'  MaxTorp As long
'  CurTorp As long
'  MaxHull As long
'  CurHull As long
'
'  status As Long
'  Ammo As long
'  CurDam As long
'  Special As String
'
'  Hits As long
'  PenHits As long
'  BPAttackCrit As long
'End Type
'
' Public AttShips(9999) As ShipInfo
' Public DefShips(9999) As ShipInfo
' Public TempA(9999) As ShipInfo
' Public TempB(9999) As ShipInfo
   
Public Type AttackInfo
  AttackID As Long   ' attacker, used to get Special and ShipStr
  TargetID As Long   ' defender, used to get Special and ShipStr
  Damage As Long     ' firepower of the attack
  Weapon As Long     ' special weapon data  (Meson, Heat, etc.)
  Special As String     ' some weapons need additional data
End Type

Public MaximumDamage As Long
Public AttacksMax As Long
Public AttacksIndex As Long
Public Attacks(20000) As AttackInfo

' Most ships that have missiles have one slavo or kind of missile that they
' launch.  But, ships with weapon batteries can have mixed slavos and require
' a salvo array to track the number of missiles and the kinds launched.

Private Type SalvoInfo
  MissileS As Long
  DataStr As String
End Type

Public Salvos(200) As SalvoInfo
Public SalvoCount As Long

' Declaring arrays
Public AttShipStr(9999) As String
Public DefShipStr(9999) As String
Public AttCritStr(9999) As String
Public DefCritStr(9999) As String
   
Public CurBeamA(9999) As Long
Public CurShieldA(9999) As Long
Public CurTorpA(9999) As Long
Public CurHullA(9999) As Long
Public HitsA(9999) As Long
Public PenHitsA(9999) As Long
Public StatusA(9999) As Long
Public AmmoA(9999) As Long
Public MaxBeamA(9999) As Long
Public MaxShieldA(9999) As Long
Public MaxTorpA(9999) As Long
Public MaxHullA(9999) As Long
Public CurDamA(9999) As Long
Public SpecialA(9999) As String
Public BPAttackCritA(9999) As Long

Public CurBeamB(9999) As Long
Public CurShieldB(9999) As Long
Public CurTorpB(9999) As Long
Public CurHullB(9999) As Long
Public HitsB(9999) As Long
Public PenHitsB(9999) As Long
Public StatusB(9999) As Long
Public AmmoB(9999) As Long
Public MaxBeamB(9999) As Long
Public MaxShieldB(9999) As Long
Public MaxTorpB(9999) As Long
Public MaxHullB(9999) As Long
Public CurDamB(9999) As Long
Public SpecialB(9999) As String
Public BPAttackCritB(9999) As Long
Public ValidTargets(9999) As Long

'Temporary sets for intra-combat use...
Public TempAttCritStr(9999) As String
Public TempDefCritStr(9999) As String
   
Public TempCurBeamA(9999) As Long
Public TempCurShieldA(9999) As Long
Public TempCurTorpA(9999) As Long
Public TempCurHullA(9999) As Long
Public TempHitsA(9999) As Long
Public TempPenHitsA(9999) As Long
Public TempStatusA(9999) As Long
Public TempCurDamA(9999) As Long
Public TempSpecialA(9999) As String

Public TempCurBeamB(9999) As Long
Public TempCurShieldB(9999) As Long
Public TempCurTorpB(9999) As Long
Public TempCurHullB(9999) As Long
Public TempHitsB(9999) As Long
Public TempPenHitsB(9999) As Long
Public TempStatusB(9999) As Long
Public TempCurDamB(9999) As Long
Public TempSpecialB(9999) As String

Public terrain(7) As String
Public terrain_def(7) As Long
Public terrain_special(7) As String

Public LocationA(9999) As Long
Public LocationB(9999) As Long
Public OrdersA(9999) As Long
Public OrdersB(9999) As Long

' values for critical hit status checks
Dim CRIT_DIS As Long, CRIT_HEAT As Long, CRIT_MESON As Long
Dim CRIT_VIBRO As Long, CRIT_BP As Long, CRIT_SPECIAL As Long

Dim dice As Long
Dim ret As Long  ' This is the global return value for the Has functions.
                    ' HasMissile being the major exception since it has it own
                    ' globals that it populates.
                    ' IsX functions will always return true or false.
                    ' And, GetX functions will always return a string or long
                    ' value with zero being false.
                    
Public LoadA_ready As Boolean, LoadD_ready As Boolean
Dim AttHasLongRange As Boolean, DefHasLongRange As Boolean
Public GroupName As String, UnitName As String

' Special attribute flags
Public Const sa_dis As Long = 1
Public Const sa_heat As Long = 2
Public Const sa_meson As Long = 4
Public Const sa_vibro As Long = 8
Public Const sa_multi As Long = 16   ' scatter pack (used for targetting)
Public Const sa_crack As Long = 32
Public Const sa_bp As Long = 64      ' boarding party
Public Const sa_pen As Long = 128
Public Const sa_special As Long = 256
'
'
Public master_index(50) As Long
Public master_weight(50) As Long
Public table_id() As Long
Public table_weight() As Long
Public table_sub() As Long
Public table_info() As String

Public crit_dice1 As Long
Public crit_dice2 As Long
Public crit_dice3 As Long
Public FetchValue As Long


' fire_array(200)
' special_array(200)
' tag_array(200)
'
Public Type CombatInfo
  firepower As Long   ' damage inflicted
  Special As Long     ' bitmask flag for special attacks
  tag As String       ' additional weapon data
End Type


Sub Main()
On Error Resume Next

Kill ("fled_att.csv")
Kill ("fled_def.csv")

On Error GoTo RUN_TIME_ERROR

Dim AttackLoop As Long, AbortCounter As Long
Dim AttShipsRead As Long, DefShipsRead As Long

Dim status As String, NewTag As String, temp_str As String

Dim AttHasScreen As Boolean, DefHasScreen As Boolean
Dim AttHasReserveUnits As Boolean, DefHasReserveUnits As Boolean

Dim HullTarget As Long, CombatBonus As Long, ForceID As Long
Dim SeekTarget As Long, HasRaided As Long
Dim AttVal As Long, DefVal As Long, BPDice As Long, SuicideBonus As Long
Dim FirePower1 As Long, FirePower2 As Long
Dim DataLinkA(52)  As Long, DataLinkB(52)  As Long
Dim AttGone As Long, DefGone As Long, Special1 As Long, Special2 As Long
Dim number_of_attacks As Long

' fire_array(200)
' special_array(200)
' tag_array(200)
'
Dim Hits(200) As CombatInfo

Dim CombatStr As String

Dim tmp As Long, TempAttShipsLeft As Long, TempDefShipsLeft As Long
Dim missile_counter As Long, BeamsOut As Long, TorpsOut As Long
Dim beam_count As Long, torp_count As Long
Dim AttNumValidTargets As Long, DefNumValidTargets As Long
Dim YieldBonus As Long, ShipHit As Long, offset As Long

Dim BaseAccuracy As Double

Dim start As Long, start1 As Long, old_start As Long, sc As Long, packet_size As Long

Dim weapon_str As String
Dim missile_str As String
Dim swt As Long
Dim Strip As Boolean
Dim new_str As String
Dim AutoMiss As Long
Dim AutoHit As Long

Randomize Timer  ' Only needs to be done once
    
GET_FILE_NAMES:
' Get user input
'AttFleetStr = InputBox("Attacking Fleet", "Attacking Fleet")
'DefFleetStr = InputBox("Defending Fleet", "Defending Fleet")
              
' Set counters and flags to their default values
CombatRound = 0
RetreatFlag = 0
AttShipsLeft = 0: DefShipsLeft = 0
AttShipsRead = 0: DefShipsRead = 0
AttackLoop = 1 ' set to zero to end combat
AttacksMax = 20000
AttacksIndex = 0

' building filenames
AttFile = AttFleetStr + ".csv"
DefFile = DefFleetStr + ".csv"
TempAFile = "tempa.csv"
TempBFile = "tempb.csv"
   
DM_ToHitA = 0: DM_ToHitB = 0

' show the status page
    Form1.Show

    ' set the widths of the colums in the Fleet Header grid
    Form1.MSFlexGrid2.ColWidth(0) = 200
    Form1.MSFlexGrid2.ColWidth(1) = 1000
    Form1.MSFlexGrid2.ColWidth(2) = 1800
    Form1.MSFlexGrid2.ColWidth(3) = 800
    Form1.MSFlexGrid2.ColWidth(4) = 500
    Form1.MSFlexGrid2.ColWidth(5) = 700
    Form1.MSFlexGrid2.ColWidth(6) = 700
    Form1.MSFlexGrid2.ColWidth(7) = 900
    Form1.MSFlexGrid2.ColWidth(8) = 700
    Form1.MSFlexGrid2.ColAlignment(0) = 2

    ' populate row 0, the column headers
    Form1.MSFlexGrid2.Row = 0
    Form1.MSFlexGrid2.Col = 1:  Form1.MSFlexGrid2.Text = "Race"
    Form1.MSFlexGrid2.Col = 2:  Form1.MSFlexGrid2.Text = "Group ID"
    Form1.MSFlexGrid2.Col = 3:  Form1.MSFlexGrid2.Text = "Break Off"
    Form1.MSFlexGrid2.Col = 4:  Form1.MSFlexGrid2.Text = "Units"
    Form1.MSFlexGrid2.Col = 5:  Form1.MSFlexGrid2.Text = "Hull Pts"
    Form1.MSFlexGrid2.Col = 6:  Form1.MSFlexGrid2.Text = "To Hit"
    Form1.MSFlexGrid2.Col = 7:  Form1.MSFlexGrid2.Text = "Hull Target"
    Form1.MSFlexGrid2.Col = 8:  Form1.MSFlexGrid2.Text = "Reserve"

    Form1.MSFlexGrid1.ColWidth(0) = 120
    Form1.MSFlexGrid1.ColWidth(1) = 1100
    For L = 2 To 9: Form1.MSFlexGrid1.ColWidth(L) = 550: Next L
    Form1.MSFlexGrid1.ColWidth(10) = 700
    Form1.MSFlexGrid1.ColWidth(11) = 5000

    Form1.MSFlexGrid1.Row = 0
    Form1.MSFlexGrid1.Col = 1:  Form1.MSFlexGrid1.Text = "Unit Name"
    Form1.MSFlexGrid1.Col = 2:  Form1.MSFlexGrid1.Text = "Beam"
    Form1.MSFlexGrid1.Col = 3:  Form1.MSFlexGrid1.Text = " "
    Form1.MSFlexGrid1.Col = 4:  Form1.MSFlexGrid1.Text = "Shield"
    Form1.MSFlexGrid1.Col = 5:  Form1.MSFlexGrid1.Text = " "
    Form1.MSFlexGrid1.Col = 6:  Form1.MSFlexGrid1.Text = "Torps"
    Form1.MSFlexGrid1.Col = 7:  Form1.MSFlexGrid1.Text = " "
    Form1.MSFlexGrid1.Col = 8:  Form1.MSFlexGrid1.Text = "Hull"
    Form1.MSFlexGrid1.Col = 9:  Form1.MSFlexGrid1.Text = " "
    Form1.MSFlexGrid1.Col = 10:  Form1.MSFlexGrid1.Text = "CurDam"
    Form1.MSFlexGrid1.Col = 11:  Form1.MSFlexGrid1.Text = "Special"

    Do
      DoEvents
    Loop While Form1.OK_FLAG = False
  
    Form1.Hide  ' Remove the status page - begin processing

If Form1.Check2.value Then
  MaximumDamage = 1
Else
  MaximumDamage = 0
End If

' Basic init stuff
BO_Att = 0 ' attack fleet damage level
BO_Def = 0 ' defense fleet damage level
BaseAccuracy = val(Form1.Text5.Text)

' open the battle_report file
ActiveFile = Form1.Text1.Text
Open ActiveFile For Output As #3

' open the damage_report file
ActiveFile = Form1.Text2.Text
Open ActiveFile For Output As #4
ActiveFile = "unknown1"
   
WriteTempFiles
  
' This is where the program comes back in subsequent rounds
Do
  ActiveFile = TempAFile
  Open ActiveFile For Input As #1
  ' This is the first line of the Attacker fleet file that is generated each round
  Input #1, AttRaceName, AttFleetName, AttBreakOff, AttShipsTotal, AttFleetStrength, AttShipsLeft, AttTargetBonus, AttTargetPriority, AttReserve
  L = 1
  Do While Not EOF(1)
    ' These are the UDL lines from the Attacker fleet file that is generated each round
    ' Each value is stored in a different array
    Input #1, AttShipStr(L), MaxBeamA(L), CurBeamA(L), MaxShieldA(L), CurShieldA(L), MaxTorpA(L), CurTorpA(L), MaxHullA(L), CurHullA(L), CurDamA(L), StatusA(L), AmmoA(L), SpecialA(L)
    L = L + 1
  Loop
  AttShipsLeft = L - 1
  Close #1
   
  ActiveFile = TempBFile
  Open ActiveFile For Input As #2
  ' The Defender fleet file that is generated each round.  The first line is fleet information.
  Input #2, DefRaceName, DefFleetName, DefBreakOff, DefShipsTotal, DefFleetStrength, DefShipsLeft, DefTargetBonus, DefTargetPriority, DefReserve
  L = 1
  Do While Not EOF(2)
    ' See note above for the Attackers fleet file
    Input #2, DefShipStr(L), MaxBeamB(L), CurBeamB(L), MaxShieldB(L), CurShieldB(L), MaxTorpB(L), CurTorpB(L), MaxHullB(L), CurHullB(L), CurDamB(L), StatusB(L), AmmoB(L), SpecialB(L)
    L = L + 1
  Loop
  DefShipsLeft = L - 1
  Close #2
  ActiveFile = "unknown2"
  
  ' Trip the Round counter
  CombatRound = CombatRound + 1
  
  ' Battle finally commences
  TempStr = "Battle Round " + Format(CombatRound)
  Print #3, " ": Print #3, TempStr
  TempStr = "Attackers are the " + AttRaceName + ", " + AttFleetName + " " + GroupName + "."
  Print #3, TempStr
  TempStr = "Current group Break-off level is " + Format(AttBreakOff) + "%"
  Print #3, TempStr
  Print #3, "The " + UnitName + " are currently listed as:" ' UnitName is the name entered in GUI Fleet/Starship
  X = 0
  For X = 1 To AttShipsLeft
    ' Loop through the attacking ships and print out each one
    TempStr = AttShipStr(X) + " Bm=" + Format(CurBeamA(X)) + " Sh=" + Format(CurShieldA(X)) + " Tp=" + Format(CurTorpA(X)) + " Hl=" + Format(CurHullA(X)) + " " + Chr(34) + SpecialA(X) + Chr(34)
    Print #3, TempStr
    TempStr = " "
  Next X
  Print #3, " "
  TempStr = "Defenders are the " + DefRaceName + ", " + DefFleetName + " " + GroupName + "."
  Print #3, TempStr
  TempStr = "Current group Break-off level is " + Format(DefBreakOff) + "%"
  Print #3, TempStr
  Print #3, "The " + UnitName + " are currently listed as:"
  X = 0
  For X = 1 To DefShipsLeft
    ' Loop through the defending ships and print out each one
    TempStr = DefShipStr(X) + " Bm=" + Format(CurBeamB(X)) + " Sh=" + Format(CurShieldB(X)) + " Tp=" + Format(CurTorpB(X)) + " Hl=" + Format(CurHullB(X)) + " " + Chr(34) + SpecialB(X) + Chr(34)
    Print #3, TempStr
    TempStr = " "
  Next X
  
  ' OK, battle actually commences NOW
  Print #3, " "
  Print #3, "Battle Results Commence:"
  Print #3, " "

  ' clear old damage array values
  For A = 1 To AttShipsLeft: HitsA(A) = 0: PenHitsA(A) = 0: BPAttackCritA(A) = 0: Next A
  For B = 1 To DefShipsLeft: HitsB(B) = 0: PenHitsB(B) = 0: BPAttackCritB(B) = 0: Next B
  
  ' clear old data link indexes
  For A = 1 To 52: DataLinkA(A) = -1: Next A
  For B = 1 To 52: DataLinkB(B) = -1: Next B
  
  ' Cloaks and Missiles affect the battle order
  ' Fighters, Surprise and Reserve affect individual ships
  
  AttIsCloaked = 0
  AttIsMixed = 0
  AttHasLongRange = False
  AttHasFighters = 0
  DefIsCloaked = 0
  DefIsMixed = 0
  DefHasLongRange = False
  DefHasFighters = 0
  
  If CombatRound = 1 Then
    ' Several special things can happen in turn one.
    ' cloaked ships can get a first strike
    ' long range weapons can get a first strike if the targets are not cloaked
    ' And, we need to determine if FLAK equiped ships have targets
    For A = 1 To AttShipsLeft
      ' Count the number of units that are cloaked
      If IsCloak(SpecialA(A)) Then AttIsCloaked = AttIsCloaked + 1
      ' Check if the unit has a long range tag
      If HasLong(SpecialA(A)) Then AttHasLongRange = True
      ' this checks ALL weapon tags since we pass it all weapon tags
      If HasLongWT(SpecialA(A)) Then AttHasLongRange = True
      ' Check if a fighter and not in reserve
      If IsFighter(SpecialA(A)) And Not HasReserve(SpecialA(A)) Then AttHasFighters = 1
    Next A
    ' Determine if all attackers are cloaked, if none are, or if some are cloaked
    If AttIsCloaked > 0 Then If AttIsCloaked = AttShipsLeft Then AttIsCloaked = 1 Else AttIsCloaked = 0: AttIsMixed = 1

    ' Now do the defenders
    For A = 1 To DefShipsLeft
      If IsCloak(SpecialB(A)) Then DefIsCloaked = DefIsCloaked + 1
      If HasLong(SpecialB(A)) Then DefHasLongRange = True
      ' this checks ALL weapon tags since we pass it all weapon tags
      If HasLongWT(SpecialB(A)) Then DefHasLongRange = True
      If IsFighter(SpecialB(A)) And Not HasReserve(SpecialB(A)) Then DefHasFighters = 1
    Next A
    If DefIsCloaked > 0 Then If DefIsCloaked = DefShipsLeft Then DefIsCloaked = 1 Else DefIsCloaked = 0: DefIsMixed = 1

    ' if both sides are cloaked, they neutralize each other
    If AttIsCloaked = 1 And DefIsCloaked = 1 Then AttIsCloaked = 0: DefIsCloaked = 0
  
    ' cloaks void long range first strike capability
    If AttIsCloaked = 1 Then DefHasLongRange = False
    If DefIsCloaked = 1 Then AttHasLongRange = False
  Else
    ' remove reserve tags from delayed units
    For A = 1 To AttShipsLeft
      If HasReserve(SpecialA(A)) And HasDelay(SpecialA(A)) Then
        If ret < CombatRound Then
          SpecialA(A) = RemoveTag(SpecialA(A), "RESERVE", 1)
        End If
      End If
    Next A
  
    ' remove reserve tags from delayed units
    For B = 1 To DefShipsLeft
      If HasReserve(SpecialB(B)) And HasDelay(SpecialB(B)) Then
        If ret < CombatRound Then
          SpecialB(B) = RemoveTag(SpecialB(B), "RESERVE", 1)
        End If
      End If
    Next B
    
    ' Check attackers and defenders fleets for fighters
    For A = 1 To AttShipsLeft
      If IsFighter(SpecialA(A)) And Not HasReserve(SpecialA(A)) Then AttHasFighters = 1
    Next A
    For A = 1 To DefShipsLeft
      If IsFighter(SpecialB(A)) And Not HasReserve(SpecialB(A)) Then DefHasFighters = 1
    Next A
  End If

  ' Check for attacker reserve & screen
  AttHasScreen = False
  AttHasReserveUnits = False
  For A = 1 To AttShipsLeft
    If HasReserve(SpecialA(A)) Then
      AttHasReserveUnits = True
    Else
      AttHasScreen = True
    End If
  Next A
  
  ' Check for defender reserve & screen
  DefHasScreen = False
  DefHasReserveUnits = False
  For B = 1 To DefShipsLeft
    If HasReserve(SpecialB(B)) Then
      DefHasReserveUnits = True
    Else
      DefHasScreen = True
    End If
  Next B
  
  ' if there is no screen, the reserve automatically fails
  ' NOTE: if Reserve > BreakOff the reserves will NEVER engage.  Handy when
  ' you're trying to protect something like a badly damaged capitol ship or
  ' freighters and the GM says they're vulnerable to attack.
  For A = 1 To AttShipsLeft
    If AttHasReserveUnits = True And AttHasScreen = False Then
      If HasReserve(SpecialA(A)) Then
        SpecialA(A) = RemoveTag(SpecialA(A), "RESERVE", 1)
        AttHasReserveUnits = False
      End If
    Else
      If HasReserve(SpecialA(A)) Then
        If BO_Att >= ret Then
          SpecialA(A) = RemoveTag(SpecialA(A), "RESERVE", 1)
        End If
      End If
    End If
  Next A
  
  For B = 1 To DefShipsLeft
    If DefHasReserveUnits = True And DefHasScreen = False Then
      If HasReserve(SpecialB(B)) Then
        SpecialB(B) = RemoveTag(SpecialB(B), "RESERVE", 1)
        DefHasReserveUnits = False
      End If
    Else
      If HasReserve(SpecialB(B)) Then
        If BO_Def >= ret Then
          SpecialB(B) = RemoveTag(SpecialB(B), "RESERVE", 1)
        End If
      End If
    End If
  Next B
  
  ' UNKNOWN
  If AttacksIndex > 0 Then
    For A = 1 To AttacksIndex
      Attacks(A).AttackID = 0
      Attacks(A).TargetID = 0
      Attacks(A).Damage = 0
      Attacks(A).Weapon = 0
      Attacks(A).Special = ""
    Next A
    AttacksIndex = 0
  End If
  
  ' spawn missiles routine
  TempAttShipsLeft = AttShipsLeft
  TempDefShipsLeft = DefShipsLeft
  missile_counter = 0
  ' Loop through a combined total of all remaining attacking and defending ships
  ' This is horribely convoluted
  For A = 1 To AttShipsLeft + DefShipsLeft
    ' Determine ForceID
    ' 0 => Attackers
    ' 1 => Defenders
    B = A: ForceID = 0
    If A > AttShipsLeft Then B = A - AttShipsLeft: ForceID = 1
    
    ' Get unit values depending on ForceID
    If ForceID = 0 Then
      ' Attackers
      temp_str = SpecialA(B): tmp = CurTorpA(B)
    Else
      ' Defenders
      temp_str = SpecialB(B): tmp = CurTorpB(B)
    End If
    
    ' Check if the unit is crippled or is suprised.  If either is true then skip processing the unit
    If IsCripple(temp_str) Or IsSurprise(temp_str) Then GoTo skip_unit
    
    If HasBatteries(temp_str) Then
      If HasMissileWT(temp_str) = False Then GoTo skip_unit
      
      ' Walk through each of the batteries and look for missiles with ammo,
      ' artillery, or long flags.  Also, fix the tmp value.
      ' assume no valid missiles
      tmp = 0
      SalvoCount = 0
      For sc = 1 To 200: Salvos(sc).MissileS = 0: Salvos(sc).DataStr = "": Next
      
      old_start = InStr(1, temp_str, "["): sc = 0
      Do While old_start > 0
        ' Get the text string for each salvo bracket
        start = InStr(old_start, temp_str, "[")
        start1 = InStr(start, temp_str, "]")
        old_start = InStr(start1, temp_str, "[")
        sc = sc + 1
        Salvos(sc).DataStr = Mid$(temp_str, start, (start1 - start) + 1)
        Salvos(sc).MissileS = 0

        ' Does the salvo have a misXXXX tag?
        If HasMissileWT(Salvos(sc).DataStr) Then
          ' in reserve?
          If HasReserve(temp_str) And (HasArtilleryWT(Salvos(sc).DataStr) = False Or ret = 1) Then
            ' Here would be something about the unit is in the reserve and does not have an artillery tag
          Else
            ' The unit is either not in the reserve or has an artillery tag
            ' at long range?
            If (AttHasLongRange Or DefHasLongRange) And _
                Not HasLongWT(Salvos(sc).DataStr) Then
                ' There is long range units present but this unit is not one of them
            Else
              ' is there ammo?
              If HasAmmoWT(Salvos(sc).DataStr) Then
                ' there should be ammo though it may be 0(?)
                If ret > 0 Then
                  ' assuming everything is OK, let's build the salvo array
                  ' Assumptions are bad...
                  ' This should be: the salvo has ammo.  Build the salvo array.
                  Salvos(sc).MissileS = val(Mid$(Salvos(sc).DataStr, 2))
                End If
              Else
                ' assuming everything is OK, let's build the salvo array
                ' Assumption = BAD
                ' Should be: Savlo doesn't need ammo.  Build the salvo array.
                Salvos(sc).MissileS = val(Mid$(Salvos(sc).DataStr, 2))
              End If
            End If
          End If
        End If
      Loop
    
      ' Loop through the salvo count and spawn a missile 'unit' for each entity
      SalvoCount = sc
      For sc = 1 To SalvoCount
        If Salvos(sc).MissileS > 0 Then
          If Salvos(sc).MissileS + TempAttShipsLeft + TempDefShipsLeft > 9999 Then
            D = MsgBox("The 9999 ship limit has been exceeded while spawning missiles.  Abort?", vbYesNo, "Major Simulation Error")
            If D = vbYes Then End
          End If
        
          HasMissileWT (Salvos(sc).DataStr) ' read and set the values.  This also sets the global variables MissileB,MissileS,MissileT, and MissileH
          If HasAmmoWT(Salvos(sc).DataStr) Then
            NewTag = "ammo " + Format(ret - 1) + " " ' Decrement the ammo counter
            temp_str = RemoveTag(Salvos(sc).DataStr, "ammo", 1) ' Remove the old ammo tag
            temp_str = AddTag(temp_str, CStr(NewTag)) ' Put in the new ammo tag
            Salvos(sc).DataStr = temp_str
            'remove the ammo string from missiles so there are no 'duds'
            missile_str = RemoveTag(Salvos(sc).DataStr, "ammo", 1) ' remove the ammo tag and save that as the missile string
            missile_str = RemoveTag(missile_str, "shots", 1) ' remove shots if it is there
          Else
            missile_str = Salvos(sc).DataStr ' just use the salvo string if there is no ammo
          End If
          
          ' spawn them babies...
          For C = 1 To Salvos(sc).MissileS
            ' increment ship counts
            missile_counter = missile_counter + 1
            If ForceID = 0 Then
              ' VB sucks.  Where do MissileB, MissileS, MissileT, and MissileH come from?
              ' They are globals referenced in the function HasMissileWT.  That is when the get set.  Twenty lines up...
              TempAttShipsLeft = TempAttShipsLeft + 1
              AttShipStr(TempAttShipsLeft) = "missile " & Format(missile_counter)
              MaxBeamA(TempAttShipsLeft) = MissileB
              CurBeamA(TempAttShipsLeft) = MissileB
              MaxShieldA(TempAttShipsLeft) = MissileS
              CurShieldA(TempAttShipsLeft) = MissileS
              MaxTorpA(TempAttShipsLeft) = MissileT
              CurTorpA(TempAttShipsLeft) = MissileT
              MaxHullA(TempAttShipsLeft) = MissileH
              CurHullA(TempAttShipsLeft) = MissileH
              ' inherit properties and convert to a missile
              SpecialA(TempAttShipsLeft) = missile_str ' Use the missile string from the Salvos array (see ~25 lines up)
              ' change the begining number to match the missile strength
              start = InStr(SpecialA(TempAttShipsLeft), " ")
              If start > 0 Then SpecialA(TempAttShipsLeft) = "[" & Format(MissileB + MissileT) & Mid$(SpecialA(TempAttShipsLeft), start) ' This creates a salvo for Beam rating + Torp rating
              SpecialA(TempAttShipsLeft) = RemoveTag(SpecialA(TempAttShipsLeft), "mis", 0) ' Remove the misXXXX tag or the missile will spawn a missile...submunitions?
              SpecialA(TempAttShipsLeft) = AddTag(SpecialA(TempAttShipsLeft), "MSL ") ' Special tag to denote the unit as a missile
              SpecialA(TempAttShipsLeft) = AddTag(SpecialA(TempAttShipsLeft), "SUICIDE ") ' Suicide tags the unit to be destroyed at end of turn
            Else
              ' Do the other Force....
              ' See my notes above
              TempDefShipsLeft = TempDefShipsLeft + 1
              DefShipStr(TempDefShipsLeft) = "missile " & Format(missile_counter)
              MaxBeamB(TempDefShipsLeft) = MissileB
              CurBeamB(TempDefShipsLeft) = MissileB
              MaxShieldB(TempDefShipsLeft) = MissileS
              CurShieldB(TempDefShipsLeft) = MissileS
              MaxTorpB(TempDefShipsLeft) = MissileT
              CurTorpB(TempDefShipsLeft) = MissileT
              MaxHullB(TempDefShipsLeft) = MissileH
              CurHullB(TempDefShipsLeft) = MissileH
              ' inherit properties and convert to a missile
              SpecialB(TempDefShipsLeft) = missile_str
              ' change the begining number to match the missile strength
              start = InStr(SpecialB(TempDefShipsLeft), " ")
              If start > 0 Then SpecialB(TempDefShipsLeft) = "[" & Format(MissileB + MissileT) & Mid$(SpecialB(TempDefShipsLeft), start)
              SpecialB(TempDefShipsLeft) = RemoveTag(SpecialB(TempDefShipsLeft), "mis", 0)
              SpecialB(TempDefShipsLeft) = AddTag(SpecialB(TempDefShipsLeft), "MSL ")
              SpecialB(TempDefShipsLeft) = AddTag(SpecialB(TempDefShipsLeft), "SUICIDE ")
            End If
          Next C
        End If 'if missiles present for spawning
      Next sc
      
      ' reassemble weapon tags back into a unit's appropriate Special tag
      ' get the Special tag from the special arrays.  Again with the ForceID junk.  Functions are your friend...
      If ForceID = 0 Then temp_str = SpecialA(B) Else temp_str = SpecialB(B)
      
      Strip = False
      new_str = ""
      
      ' Not entirely sure what this is for.  Might be for removing the [] from the string?
      ' Or does it remove the first salvo from the string?
      For swt = 1 To Len(temp_str)
        If Mid$(temp_str, swt, 1) = "[" Then Strip = True
        If Strip = False Then new_str = new_str + Mid$(temp_str, swt, 1)
        If Mid$(temp_str, swt, 1) = "]" Then Strip = False
      Next swt

      ' This builds a new string of salvos from the Salvos array?  Buy why?
      ' Also, 200 salvos for the entire simulation....that isn't enough for one fleet!
      ' See the code below this block.  The salvos array is per unit, I think.      
      For sc = 1 To 200
        If Salvos(sc).DataStr = "" Then Exit For
        new_str = new_str + Salvos(sc).DataStr
      Next sc
      
      ' Assign the new specials string to the SpecialA or Special B array for the unit in question depending on ForceID
      If ForceID = 0 Then SpecialA(B) = new_str Else SpecialB(B) = new_str
      ' End of HasBatteries if/then block?
    Else ' has no batteries
      If IsNoTorp(temp_str) Or Not HasMissile(temp_str) Then GoTo skip_unit ' skip if the unit has not trop rating or if the special string for the unit does NOT have missile.  These are non-bracket values
      If HasAmmo(temp_str) Then If ret < 1 Then GoTo skip_unit ' skip if the unit doesn't have enough ammo
      If HasReserve(temp_str) Then ' check for reserve
        If HasArtillery(temp_str) = False Or ret = 1 Then GoTo skip_unit  ' because only the beams have artillery
      End If
      If AttHasLongRange Or DefHasLongRange Then ' check for long range missiles
        HasLong (temp_str) ' check the special string for 'LONG'  These are non-bracket values
        If ret < 2 Then GoTo skip_unit
      End If
      ' begin non-weapons tag version of spawn missiles
      ' check for 9999 unit limit
      If tmp + TempAttShipsLeft + TempDefShipsLeft > 9999 Then
        D = MsgBox("The 9999 ship limit has been exceeded while spawning missiles.  Abort?", vbYesNo, "Major Simulation Error")
        If D = vbYes Then End
      End If
          
      HasMissile (temp_str) ' read and set the values
      If HasAmmo(temp_str) Then
        NewTag = "AMMO " + Format(ret - 1) + " " ' build the new AMMO tag
        temp_str = RemoveTag(temp_str, "AMMO", 1) ' remove the old AMMO tag
        temp_str = AddTag(temp_str, CStr(NewTag)) ' put in the new AMMO tag
        If ForceID = 0 Then SpecialA(B) = temp_str Else SpecialB(B) = temp_str ' Set the new special string to the appropriate specials array based on the ForceID
        'remove the ammo and shots string from missiles so there are no 'duds'
        missile_str = RemoveTag(temp_str, "AMMO", 1)
        missile_str = RemoveTag(missile_str, "SHOTS", 1)
      Else
        missile_str = temp_str
      End If
        
      ' spawn them babies...
      For C = 1 To tmp
        ' increment ship counts
        missile_counter = missile_counter + 1
        If ForceID = 0 Then
          TempAttShipsLeft = TempAttShipsLeft + 1
          AttShipStr(TempAttShipsLeft) = "missile " & Format(missile_counter)
          MaxBeamA(TempAttShipsLeft) = MissileB
          CurBeamA(TempAttShipsLeft) = MissileB
          MaxShieldA(TempAttShipsLeft) = MissileS
          CurShieldA(TempAttShipsLeft) = MissileS
          MaxTorpA(TempAttShipsLeft) = MissileT
          CurTorpA(TempAttShipsLeft) = MissileT
          MaxHullA(TempAttShipsLeft) = MissileH
          CurHullA(TempAttShipsLeft) = MissileH
          ' inherit properties and convert to a missile
          SpecialA(TempAttShipsLeft) = missile_str
          SpecialA(TempAttShipsLeft) = RemoveTag(SpecialA(TempAttShipsLeft), "MIS", 0)
          ' Missiles are not allowed to raid ships
          SpecialA(TempAttShipsLeft) = RemoveTag(SpecialA(TempAttShipsLeft), "BP", 2)
          ' The weird case when a damaged ship is firing det-nuke laser warhead missiles
          SpecialA(TempAttShipsLeft) = RemoveTag(SpecialA(TempAttShipsLeft), "NOBEAM", 0)
          SpecialA(TempAttShipsLeft) = AddTag(SpecialA(TempAttShipsLeft), "MSL ")
          SpecialA(TempAttShipsLeft) = AddTag(SpecialA(TempAttShipsLeft), "SUICIDE ")
        Else
          TempDefShipsLeft = TempDefShipsLeft + 1
          DefShipStr(TempDefShipsLeft) = "missile " & Format(missile_counter)
          MaxBeamB(TempDefShipsLeft) = MissileB
          CurBeamB(TempDefShipsLeft) = MissileB
          MaxShieldB(TempDefShipsLeft) = MissileS
          CurShieldB(TempDefShipsLeft) = MissileS
          MaxTorpB(TempDefShipsLeft) = MissileT
          CurTorpB(TempDefShipsLeft) = MissileT
          MaxHullB(TempDefShipsLeft) = MissileH
          CurHullB(TempDefShipsLeft) = MissileH
          ' inherit properties and convert to a missile
          SpecialB(TempDefShipsLeft) = missile_str
          SpecialB(TempDefShipsLeft) = RemoveTag(SpecialB(TempDefShipsLeft), "MIS", 0)
          ' Missiles are not allowed to raid ships
          SpecialB(TempDefShipsLeft) = RemoveTag(SpecialB(TempDefShipsLeft), "BP", 2)
          ' The weird case when a damaged ship is firing det-nuke laser warhead missiles
          SpecialB(TempDefShipsLeft) = RemoveTag(SpecialB(TempDefShipsLeft), "NOBEAM", 0)
          SpecialB(TempDefShipsLeft) = AddTag(SpecialB(TempDefShipsLeft), "MSL ")
          SpecialB(TempDefShipsLeft) = AddTag(SpecialB(TempDefShipsLeft), "SUICIDE ")
        End If
      Next C
      ' end non-weapon tag missile spawning
    End If
skip_unit:
  Next A
  
  AttNumValidTargets = AttShipsLeft
  DefNumValidTargets = DefShipsLeft
  AttShipsLeft = TempAttShipsLeft
  DefShipsLeft = TempDefShipsLeft
  
  ' copy real values to temp fields so that combat will be fair
  For A = 1 To AttShipsLeft
    TempAttCritStr(A) = AttCritStr(A)
    TempCurBeamA(A) = CurBeamA(A)
    TempCurShieldA(A) = CurShieldA(A)
    TempCurTorpA(A) = CurTorpA(A)
    TempCurHullA(A) = CurHullA(A)
    TempHitsA(A) = HitsA(A)
    TempPenHitsA(A) = PenHitsA(A)
    TempCurDamA(A) = CurDamA(A)
    TempSpecialA(A) = SpecialA(A)
  Next A

  For A = 1 To DefShipsLeft
    TempDefCritStr(A) = DefCritStr(A)
    TempCurBeamB(A) = CurBeamB(A)
    TempCurShieldB(A) = CurShieldB(A)
    TempCurTorpB(A) = CurTorpB(A)
    TempCurHullB(A) = CurHullB(A)
    TempHitsB(A) = HitsB(A)
    TempPenHitsB(A) = PenHitsB(A)
    TempCurDamB(A) = CurDamB(A)
    TempSpecialB(A) = SpecialB(A)
  Next A

'-----------------------------------------------------------------------
  
  ' Attack routine
  For A = 1 To AttShipsLeft + DefShipsLeft
    ' The same BS about ForceID....
    B = A: ForceID = 0
    If A > AttShipsLeft Then B = A - AttShipsLeft: ForceID = 1
    If A = AttShipsLeft + 1 Then Print #3, " " ' add a space to the results

    Special1 = 0: Special2 = 0
    FirePower1 = 0: FirePower2 = 0

' OK, here's the problem in a nut shell.  Each ship can have multiple
' attacks.  In the past, the firepower was added up and the damage
' spread out.  But that won't work with the new multi's, Special
' effect weapons, and resistant defenses.  The firepower needs to be
' divided before combat and each shot rolled and applied separately.

' Determine the number of attacks:  Split, Multi, Flak, Special
' Effect weapons that are unsplit are lumped together into a
' single attack.  [ie. A 4 pt heat beam plus a 6 pt pen torp
' becomes a 10 pt heat+pen attack.]  I'll assume that 200
' individual attacks is enough to drive most individual units.

' Clear the firepower and TempSpecial attributes arrays.
  For tmp = 1 To 200
    Hits(tmp).firepower = 0: Hits(tmp).Special = 0: Hits(tmp).tag = ""
  Next tmp
  number_of_attacks = 0  ' assume that the attacker has no weapons

  ' determine eligibility and the number of attacks (if any)
  Select Case ForceID
    Case 0 ' Attacking Fleet
      If InStr(AttShipStr(B), "missile ") > 0 Then
        AttBattleStr = AttRaceName + " " + AttShipStr(B) + " is a drone or decoy."  ' Check the name of the ship for missile?  Assume it is a drone or decoy.
      Else
        AttBattleStr = AttRaceName + " " + AttShipStr(B) + " did not fire beams or torps."  ' If missile is not in the name then the unit did not fire?
      End If
      If (Not IsDrifting(SpecialA(B))) And (Not IsNoMove(SpecialA(B))) Then ' Check if the unit is drifting or has NOMOVE
        If IsFlee(SpecialA(B)) Then ' Check if the unit is fleeing
          TempSpecialA(B) = RemoveTag(TempSpecialA(B), "FLEE", 0) ' Remove FLEE tag
          TempSpecialA(B) = AddTag(TempSpecialA(B), "FLED ") ' Add FLED tag
          If HasDefense(TempSpecialA(B)) Then ' Check if the unit has a DEFENSE tag
            TempSpecialA(B) = RemoveTag(TempSpecialA(B), "DEFENSE", 1) ' Remove DEFENSE tag
          End If
          
          NewTag = "DEFENSE " + Format(ret + Int(BaseAccuracy / 2)) + " " ' Calculate the new DEFENSE tag as half the BaseAccuracy
          TempSpecialA(B) = AddTag(TempSpecialA(B), CStr(NewTag)) ' Add the tag to the unit
          If HasTarget(TempSpecialA(B)) Then ' Check if the unit has a TARGET tag
            TempSpecialA(B) = RemoveTag(TempSpecialA(B), "TARGET", 1) ' Remove the TARGET tag
          End If
          NewTag = "TARGET " + Format(ret - Int(BaseAccuracy / 2)) + " " ' Calculate the new TARGET tag as half the BaseAccuracy
          TempSpecialA(B) = AddTag(TempSpecialA(B), CStr(NewTag)) ' Add the tag to the unit
          SpecialA(B) = TempSpecialA(B) ' Replace the unit's special string
        End If
      End If
      If IsDrifting(SpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "DRIFTING", 0) ' Remove the DRIFTING tag
      
      If IsSurprise(SpecialA(B)) Then
        AttBattleStr = AttRaceName + " " + AttShipStr(B) + " is surprised!" ' Check for suprised and print the message
        GoTo ABORT_ATTACK ' Skip the rest of the attack
      End If
      
      If IsCaptured(SpecialA(B)) Then ' Is the unit captured?
        AttBattleStr = AttRaceName + " " + AttShipStr(B) + " has been captured and can not attack!" ' Yes, print the message
        GoTo ABORT_ATTACK ' Skip the rest of the attack
      End If
      
      If IsCripple(SpecialA(B)) Then ' Is the unit crippled?
        AttBattleStr = AttRaceName + " " + AttShipStr(B) + " is crippled and can not attack!" ' Yes, print the message
        GoTo ABORT_ATTACK ' Skip the rest of the attack
      End If
      
      If DefIsCloaked = 1 Then ' Is the Defending fleet cloacked?
        AttBattleStr = AttRaceName + " " + AttShipStr(B) + " can not lock onto a target!" ' Yes, print the message
        GoTo ABORT_ATTACK ' Skip the rest of the attack
      End If
      
      ' This is the start of the battery processing
      If HasBatteries(SpecialA(B)) Then ' Does the unit have batteries?
        If HasReserve(SpecialA(B)) And Not HasArtilleryWT(SpecialA(B)) Then ' Does the unit have RESERVE & not ARTILLERY
          AttBattleStr = AttRaceName + " " + AttShipStr(B) + " is being held in reserve!" ' Yes, print the message
          GoTo ABORT_ATTACK ' Skip the rest of the attack
        End If
        
        ' Getting the bracket attacks and adding them to the Savlos array
        number_of_attacks = 0
        temp_str = SpecialA(B)
        old_start = InStr(1, temp_str, "[")
        sc = 0
        Do While old_start > 0
          start = InStr(old_start, temp_str, "[")
          start1 = InStr(start, temp_str, "]")
          If start1 = 0 Then old_start = 0: start1 = Len(temp_str) Else old_start = InStr(start1, temp_str, "[")
          sc = sc + 1
          Salvos(sc).DataStr = Mid$(temp_str, start, (start1 - start) + 1)
          Salvos(sc).MissileS = val(Mid$(Salvos(sc).DataStr, 2))
        Loop
        
        If sc > 0 Then ' Are there any salvos?
          For aa = 1 To sc
          ' do everything we can to set the firepower to zero
          If HasMissileWT(Salvos(aa).DataStr) Then Salvos(aa).MissileS = 0 ' Look for missiles in the savlo
          If IsOffline(Salvos(aa).DataStr) Then Salvos(aa).MissileS = 0 ' If the salvo is OFFLINE then set MissileS to 0....but why?
          If GetROFDelayWT(Salvos(aa).DataStr) > 0 Then Salvos(aa).MissileS = 0 ' Get for ROF delay
          If AttHasLongRange Or DefHasLongRange Then  ' Only LR attacks are valid
            If HasLongWT(Salvos(aa).DataStr) = False Then Salvos(aa).MissileS = 0 ' No LONG tag no pew pew
          End If
          If HasReserve(SpecialA(B)) Then  ' only true if artillery present
            If HasArtilleryWT(Salvos(aa).DataStr) = False Then Salvos(aa).MissileS = 0
          End If
          If HasAmmoWT(Salvos(aa).DataStr) Then ' Does the unit have ammo?
            If ret = 0 Then Salvos(aa).MissileS = 0 ' Nope, no missile...I think
          End If
          
          ' cycle weapons by updating counters and removing offline tags
          Salvos(aa).DataStr = RemoveTag(Salvos(aa).DataStr, "offline", 0) ' Remove offline tag
          If GetROFRateWT(Salvos(aa).DataStr) > 0 Then ' Check for ROF rate
            If GetROFDelayWT(Salvos(aa).DataStr) > 0 Then ' weapon is cycling using ROF delay
              NewTag = "rof " + Format(GetROFRateWT(Salvos(aa).DataStr)) + " " + Format(GetROFDelayWT(Salvos(aa).DataStr) - 1) + " " ' count down
            Else
              If Salvos(aa).MissileS > 0 Then NewTag = "rof " + Format(GetROFRateWT(Salvos(aa).DataStr)) + " " + Format(GetROFRateWT(Salvos(aa).DataStr) - 1) + " " ' re-set if firing
            End If
            temp_str = RemoveTag(Salvos(aa).DataStr, "rof", 2) ' Remove the old ROF tag
            temp_str = AddTag(temp_str, CStr(NewTag)) ' Add the new ROF tag
            Salvos(aa).DataStr = temp_str ' Save the new salvo tag to the array
          End If
            ''''''''''''''' TAZ
            ' Reassemble weapon tags back into a unit's appropriate Special tag.
            ' This requires stripping all the weapon tags and then appending them.
            temp_str = ""
            Strip = False
            For swt = 1 To Len(TempSpecialA(B))
              If Mid$(TempSpecialA(B), swt, 1) = "[" Then Strip = True
              If Strip = False Then temp_str = temp_str + Mid$(TempSpecialA(B), swt, 1)
              If Mid$(TempSpecialA(B), swt, 1) = "]" Then Strip = False
            Next swt
              
            For bb = 1 To 200
              If Salvos(bb).DataStr = "" Then Exit For
              temp_str = temp_str + Salvos(bb).DataStr
            Next bb
            TempSpecialA(B) = temp_str
            SpecialA(B) = temp_str
            ''''''''''''''''
          If Salvos(aa).MissileS > 0 Then ' Are there missiles?
            If HasFlakWT(Salvos(aa).DataStr) And DefHasFighters = 1 Then ' Are they flak missiles and do the defenders have fighters?
              ' create flak packets
              For tmp = 1 To Salvos(aa).MissileS
                number_of_attacks = number_of_attacks + 1 ' Increment number of attacks
                Hits(number_of_attacks).firepower = 1 ' Fill out hits array?  This must be possible hits
                Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special)) ' Combine the bitwise flags
                Hits(number_of_attacks).tag = Salvos(aa).DataStr ' Set the tags for the "hit"
              Next
            Else ' NOT a FLAK attack
              If HasMultiWT(Salvos(aa).DataStr) Then ' Check for MULTI tag
                packet_size = ret ' Use the multi packet size
                ' create multi packets
                For tmp = 1 To Int(Salvos(aa).MissileS / packet_size)
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = packet_size
                  Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                  Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special)) ' Combine the bitwise flags
                  Hits(number_of_attacks).tag = Salvos(aa).DataStr ' Set the tags for the "hit"
                Next
                If Salvos(aa).MissileS Mod packet_size > 0 Then
                  ' we have some "spare change"
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = Salvos(aa).MissileS Mod packet_size
                  Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                  Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special))
                  Hits(number_of_attacks).tag = Salvos(aa).DataStr
                End If
              Else ' NOT MULTI tag
                ' if single attack
                number_of_attacks = number_of_attacks + 1
                Hits(number_of_attacks).firepower = Salvos(aa).MissileS
                Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special))
                Hits(number_of_attacks).tag = Salvos(aa).DataStr
              End If
            End If
        
            If HasAmmoWT(Salvos(aa).DataStr) Then
              ' Create a new AMMO tag and replace the tag on the Salvo
              NewTag = "ammo " + Format(ret - 1) + " "
              temp_str = RemoveTag(Salvos(aa).DataStr, "ammo", 1)
              temp_str = AddTag(temp_str, CStr(NewTag))
              Salvos(aa).DataStr = temp_str
              
              ' Reassemble weapon tags back into a unit's appropriate Special tag.
              ' This requires stripping all the weapon tags and then appending them.
              temp_str = ""
              Strip = False
              For swt = 1 To Len(TempSpecialA(B))
                If Mid$(TempSpecialA(B), swt, 1) = "[" Then Strip = True
                If Strip = False Then temp_str = temp_str + Mid$(TempSpecialA(B), swt, 1)
                If Mid$(TempSpecialA(B), swt, 1) = "]" Then Strip = False
              Next swt
              
              For bb = 1 To 200
                If Salvos(bb).DataStr = "" Then Exit For
                temp_str = temp_str + Salvos(bb).DataStr
              Next bb
              TempSpecialA(B) = temp_str
              SpecialA(B) = temp_str
            End If
          End If
          Next aa
        End If ' is sc greater than zero
      
      Else ' there are no batteries
        If HasReserve(SpecialA(B)) And Not HasArtillery(SpecialA(B)) Then
          AttBattleStr = AttRaceName + " " + AttShipStr(B) + " is being held in reserve!"
          GoTo ABORT_ATTACK
        End If
    
        If NotFunctional(CurBeamA(B), CurTorpA(B), TempSpecialA(B)) Then
          If ret = 1 Then ' no weapons
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " has no functional weapons!"
            If IsNoBeam(TempSpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "NOBEAM", 0)
            If IsNoTorp(TempSpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "NOTORP", 0)
            GoTo ABORT_ATTACK
          End If
          If ret = 2 Then ' no weapons, has missiles
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " launches missiles."
            If IsNoBeam(TempSpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "NOBEAM", 0)
            If IsNoTorp(TempSpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "NOTORP", 0)
            GoTo ABORT_ATTACK
          End If
        Else ' is functional
          If IsNoBeam(TempSpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "NOBEAM", 0)
          If IsNoTorp(TempSpecialA(B)) Then TempSpecialA(B) = RemoveTag(TempSpecialA(B), "NOTORP", 0)
        End If
        
        ' OK, those are the exceptions.  Time to fill those fire and Special arrays...
        ' First of all there may be times when firepower is limited...
        If AttHasLongRange Or DefHasLongRange Then  ' Only LR attacks are valid
          If HasLong(SpecialA(B)) = False Then
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " has no long range weapons!"
            GoTo ABORT_ATTACK
          End If
        End If
      
        ' the default is one combined attack
        tmp = 1
        If HasShots(SpecialA(B)) Then If ret = 0 Then tmp = 0
        If AttHasLongRange Or DefHasLongRange Then
          HasLong (SpecialA(B))
          If ret = 2 Then tmp = 0
        End If
        If HasReserve(SpecialA(B)) Then  ' only true if artillery present
          HasArtillery (SpecialA(B))
          If ret = 2 Then tmp = 0        ' no beam
        End If
        If IsNoBeam(SpecialA(B)) Or tmp = 0 Then
          ' oops, no beam play today
          FirePower1 = 0
          Special1 = 0
        Else
          If HasShots(SpecialA(B)) Then
             NewTag = "SHOTS " + Format(ret - 1) + " "
             TempSpecialA(B) = RemoveTag(TempSpecialA(B), "SHOTS", 1)
             TempSpecialA(B) = AddTag(TempSpecialA(B), CStr(NewTag))
          End If
          FirePower1 = CurBeamA(B)
          Special1 = SetFlags(SpecialA(B), CInt(Special1), 1) ' set flags for beams
        End If
        
        tmp = 1
        If HasAmmo(SpecialA(B)) Then If ret = 0 Then tmp = 0
        If AttHasLongRange Or DefHasLongRange Then
          HasLong (SpecialA(B))
          If ret = 1 Then tmp = 0
        End If
        If HasReserve(TempSpecialA(B)) Then  ' only true if artillery present
          HasArtillery (SpecialA(B))
          If ret = 1 Then tmp = 0        ' no torp
        End If
        If HasMissile(SpecialA(B)) Or IsNoTorp(SpecialA(B)) Or tmp = 0 Then
          ' oops, no torps away today
          FirePower2 = 0
          Special2 = 0
        Else
          If HasAmmo(SpecialA(B)) > 0 Then
             NewTag = "AMMO " + Format(ret - 1) + " "
             TempSpecialA(B) = RemoveTag(TempSpecialA(B), "AMMO", 1)
             TempSpecialA(B) = AddTag(TempSpecialA(B), CStr(NewTag))
          End If
          FirePower2 = CurTorpA(B)
          Special2 = SetFlags(SpecialA(B), CInt(Special2), 2) ' set flags for torps
        End If

        ' if normal, add firepowers and combine specials
        number_of_attacks = 0 ' always start at zero
        tmp = 0: If HasFlak(SpecialA(B)) And DefHasFighters = 1 Then tmp = 1
        
        ' check to see if the first attack is a BP attack
        If GetBPAttVal(SpecialA(B)) > 0 Then ' we have boarding parties!
          number_of_attacks = number_of_attacks + 1
          Hits(number_of_attacks).firepower = GetBPAttVal(SpecialA(B))
          Hits(number_of_attacks).Special = sa_bp ' old school BP attacks have not other flags
          Hits(number_of_attacks).tag = ""
        End If
        
        If Not IsSplit(SpecialA(B)) And tmp = 0 And GetMultiBeamVal(SpecialA(B)) = 0 And GetMultiTorpVal(SpecialA(B)) = 0 Then ' a normal
          ' the default is one combined attack
          number_of_attacks = number_of_attacks + 1
          Hits(number_of_attacks).firepower = FirePower1 + FirePower2
          Hits(number_of_attacks).Special = 0
          If FirePower1 > 0 Then
            Hits(number_of_attacks).Special = SetFlags(SpecialA(B), Int(Hits(number_of_attacks).Special), 1) 'add beam values
          End If
          If FirePower2 > 0 Then
            Hits(number_of_attacks).Special = SetFlags(SpecialA(B), Int(Hits(number_of_attacks).Special), 2) 'add torp values
          End If
          Hits(number_of_attacks).tag = SpecialA(B)
        Else
          If IsSplit(SpecialA(B)) Then  ' try to form two attacks
            If Sgn(FirePower1) = 1 Then
              number_of_attacks = number_of_attacks + 1
              Hits(number_of_attacks).firepower = FirePower1
              Hits(number_of_attacks).Special = Special1
              Hits(number_of_attacks).tag = SpecialA(B)
            End If
            If Sgn(FirePower2) = 1 Then
              number_of_attacks = number_of_attacks + 1
              Hits(number_of_attacks).firepower = FirePower2
              Hits(number_of_attacks).Special = Special2
              Hits(number_of_attacks).tag = SpecialA(B)
            End If
          Else
            ' if FLAK or multi, subdivide the splits
            '   all multi-targetting gets add sa_multi to the special_array values
            If HasFlak(SpecialA(B)) Then
              Select Case ret
                Case 1
                  If Sgn(FirePower1) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower1
                    For tmp = number_of_attacks - FirePower1 To number_of_attacks
                      If tmp < 1 Then tmp = 1
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special1 + sa_multi
                      Hits(tmp).tag = SpecialA(B)
                    Next
                  End If
                  If Sgn(FirePower2) = 1 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower2
                    Hits(number_of_attacks).Special = Special2
                    Hits(number_of_attacks).tag = SpecialA(B)
                  End If
                Case 2
                  If Sgn(FirePower1) = 1 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower1
                    Hits(number_of_attacks).Special = Special1
                    Hits(number_of_attacks).tag = SpecialA(B)
                  End If
                  If Sgn(FirePower2) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower2
                    For tmp = number_of_attacks - FirePower2 To number_of_attacks
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special2 + sa_multi
                      Hits(tmp).tag = SpecialA(B)
                    Next
                  End If
                Case 3
                  If Sgn(FirePower1) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower1
                    For tmp = number_of_attacks - FirePower1 To number_of_attacks
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special1 + sa_multi
                      Hits(tmp).tag = SpecialA(B)
                    Next
                  End If
                  If Sgn(FirePower2) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower2
                    For tmp = number_of_attacks - FirePower2 To number_of_attacks
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special2 + sa_multi
                      Hits(tmp).tag = SpecialA(B)
                    Next
                  End If
              End Select
            Else 'gee, it must be a multi
              ret = 0
              If GetMultiBeamVal(SpecialA(B)) > 0 Then ret = 1
              If GetMultiTorpVal(SpecialA(B)) > 0 Then ret = ret + 2
              If ret = 0 Then
                ' this should never happen...
                AttBattleStr = AttRaceName + " " + AttShipStr(B) + " has a ship config error in MULTI [DEBUG]"
                GoTo ABORT_ATTACK
              End If
            
              If FirePower1 > 0 Then
                ret = GetMultiBeamVal(SpecialA(B))
                If ret > 0 Then
                  If ret >= FirePower1 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower1
                    Hits(number_of_attacks).Special = Special1 + sa_multi
                    Hits(number_of_attacks).tag = SpecialA(B)
                  Else
                    tmp = FirePower1
                    While tmp > ret
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = ret
                      Hits(number_of_attacks).Special = Special1 + sa_multi
                      Hits(number_of_attacks).tag = SpecialA(B)
                      tmp = tmp - ret
                    Wend
                    If tmp > 0 Then
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = tmp
                      Hits(number_of_attacks).Special = Special1 + sa_multi
                      Hits(number_of_attacks).tag = SpecialA(B)
                    End If
                  End If
                Else
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = FirePower1
                  Hits(number_of_attacks).Special = Special1 + sa_multi
                  Hits(number_of_attacks).tag = SpecialA(B)
                End If
              End If
            
              If FirePower2 > 0 Then
                ret = GetMultiTorpVal(SpecialA(B))
                If ret > 0 Then
                  If ret >= FirePower2 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower2
                    Hits(number_of_attacks).Special = Special2 + sa_multi
                    Hits(number_of_attacks).tag = SpecialA(B)
                  Else
                    tmp = FirePower2
                    While tmp > ret
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = ret
                      Hits(number_of_attacks).Special = Special2 + sa_multi
                      Hits(number_of_attacks).tag = SpecialA(B)
                      tmp = tmp - ret
                    Wend
                    If tmp > 0 Then
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = tmp
                      Hits(number_of_attacks).Special = Special2 + sa_multi
                      Hits(number_of_attacks).tag = SpecialA(B)
                    End If
                  End If
                Else
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = FirePower2
                  Hits(number_of_attacks).Special = Special2 + sa_multi
                  Hits(number_of_attacks).tag = SpecialA(B)
                End If
              End If
            End If  ' end of is flak else multi
          End If    ' end of is split
        End If      ' end of is single
      End If ' not a battery
    
    Case 1 ' Do it all again for the other force...why not a function!!!????
      If InStr(DefShipStr(B), "missile ") > 0 Then
        DefBattleStr = DefRaceName + " " + DefShipStr(B) + " is a drone or decoy."
      Else
        DefBattleStr = DefRaceName + " " + DefShipStr(B) + " did not fire beams or torps."
      End If
      If (Not IsDrifting(SpecialB(B))) And (Not IsNoMove(SpecialB(B))) Then
        If IsFlee(TempSpecialB(B)) Then
          TempSpecialB(B) = RemoveTag(TempSpecialB(B), "FLEE", 0)
          TempSpecialB(B) = AddTag(TempSpecialB(B), "FLED ")
          If HasDefense(TempSpecialB(B)) Then
            TempSpecialB(B) = RemoveTag(TempSpecialB(B), "DEFENSE", 1)
          End If
          NewTag = "DEFENSE " + Format(ret + Int(BaseAccuracy / 2)) + " "
          TempSpecialB(B) = AddTag(TempSpecialB(B), CStr(NewTag))
          If HasTarget(TempSpecialB(B)) Then
            TempSpecialB(B) = RemoveTag(TempSpecialB(B), "TARGET", 1)
          End If
          NewTag = "TARGET " + Format(ret - Int(BaseAccuracy / 2)) + " "
          TempSpecialB(B) = AddTag(TempSpecialB(B), CStr(NewTag))
          SpecialB(B) = TempSpecialB(B)
        End If
      End If
      If IsDrifting(SpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "DRIFTING", 0)
      
      If IsSurprise(SpecialB(B)) Then
        DefBattleStr = DefRaceName + " " + DefShipStr(B) + " is surprised!"
        GoTo ABORT_ATTACK
      End If
      
      If IsCaptured(SpecialB(B)) Then
        DefBattleStr = DefRaceName + " " + DefShipStr(B) + " has been captured and can not attack!"
        GoTo ABORT_ATTACK
      End If
      
      If IsCripple(SpecialB(B)) Then
        DefBattleStr = DefRaceName + " " + DefShipStr(B) + " is crippled and can not attack!"
        GoTo ABORT_ATTACK
      End If
      
      If AttIsCloaked = 1 Then
        DefBattleStr = DefRaceName + " " + DefShipStr(B) + " can not lock onto a target!"
        GoTo ABORT_ATTACK
      End If
      
      If HasBatteries(SpecialB(B)) Then
        If HasReserve(SpecialB(B)) And Not HasArtilleryWT(SpecialB(B)) Then
          DefBattleStr = DefRaceName + " " + DefShipStr(B) + " is being held in reserve!"
          GoTo ABORT_ATTACK
        End If
        
        number_of_attacks = 0
        temp_str = SpecialB(B)
        old_start = InStr(1, temp_str, "[")
        sc = 0
        Do While old_start > 0
          start = InStr(old_start, temp_str, "[")
          start1 = InStr(start, temp_str, "]")
          If start1 = 0 Then old_start = 0: start1 = Len(temp_str) Else old_start = InStr(start1, temp_str, "[")
          sc = sc + 1
          Salvos(sc).DataStr = Mid$(temp_str, start, (start1 - start) + 1)
          Salvos(sc).MissileS = val(Mid$(Salvos(sc).DataStr, 2))
        Loop
        
        If sc > 0 Then
          For aa = 1 To sc
          
          ' do everything we can to set the firepower to zero
          If HasMissileWT(Salvos(aa).DataStr) Then Salvos(aa).MissileS = 0
          If IsOffline(Salvos(aa).DataStr) Then Salvos(aa).MissileS = 0
          If GetROFDelayWT(Salvos(aa).DataStr) > 0 Then Salvos(aa).MissileS = 0
          If AttHasLongRange Or DefHasLongRange Then  ' Only LR attacks are valid
            If HasLongWT(Salvos(aa).DataStr) = False Then Salvos(aa).MissileS = 0
          End If
          If HasReserve(SpecialB(B)) Then  ' only true if artillery present
            If HasArtilleryWT(Salvos(aa).DataStr) = False Then Salvos(aa).MissileS = 0
          End If
          If HasAmmoWT(Salvos(aa).DataStr) Then
            If ret = 0 Then Salvos(aa).MissileS = 0
          End If
          
          ' cycle weapons
          Salvos(aa).DataStr = RemoveTag(Salvos(aa).DataStr, "offline", 0)
          If GetROFRateWT(Salvos(aa).DataStr) > 0 Then
            If GetROFDelayWT(Salvos(aa).DataStr) > 0 Then ' weapon is cycling
              NewTag = "rof " + Format(GetROFRateWT(Salvos(aa).DataStr)) + " " + Format(GetROFDelayWT(Salvos(aa).DataStr) - 1) + " " ' count down
            Else
              If Salvos(aa).MissileS > 0 Then NewTag = "rof " + Format(GetROFRateWT(Salvos(aa).DataStr)) + " " + Format(GetROFRateWT(Salvos(aa).DataStr) - 1) + " " ' re-set if firing
            End If
            
            temp_str = RemoveTag(Salvos(aa).DataStr, "rof", 2)
            temp_str = AddTag(temp_str, CStr(NewTag))
            Salvos(aa).DataStr = temp_str
          End If
            ''''''''''''''' TAZ
            ' Reassemble weapon tags back into a unit's appropriate Special tag.
            ' This requires stripping all the weapon tags and then appending them.
            temp_str = ""
            Strip = False
              
            For swt = 1 To Len(TempSpecialB(B))
              If Mid$(TempSpecialB(B), swt, 1) = "[" Then Strip = True
              If Strip = False Then temp_str = temp_str + Mid$(TempSpecialB(B), swt, 1)
              If Mid$(TempSpecialB(B), swt, 1) = "]" Then Strip = False
            Next swt
              
            For bb = 1 To 200
              If Salvos(bb).DataStr = "" Then Exit For
              temp_str = temp_str + Salvos(bb).DataStr
            Next bb
            TempSpecialB(B) = temp_str
            SpecialB(B) = temp_str
            '''''''''''''''
          If Salvos(aa).MissileS > 0 Then
            If HasFlakWT(Salvos(aa).DataStr) And AttHasFighters = 1 Then
              ' create flak packets
              For tmp = 1 To Salvos(aa).MissileS
                number_of_attacks = number_of_attacks + 1
                Hits(number_of_attacks).firepower = 1
                Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special))
                Hits(number_of_attacks).tag = Salvos(aa).DataStr
              Next
            Else
              If HasMultiWT(Salvos(aa).DataStr) Then
                packet_size = ret
                ' create multi packets
                For tmp = 1 To Int(Salvos(aa).MissileS / packet_size)
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = packet_size
                  Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                  Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special))
                  Hits(number_of_attacks).tag = Salvos(aa).DataStr
                Next
                If Salvos(aa).MissileS Mod packet_size > 0 Then
                  ' we have some "spare change"
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = Salvos(aa).MissileS Mod packet_size
                  Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                  Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special))
                  Hits(number_of_attacks).tag = Salvos(aa).DataStr
                End If
              Else
                ' if single attack
                number_of_attacks = number_of_attacks + 1
                Hits(number_of_attacks).firepower = Salvos(aa).MissileS
                Hits(number_of_attacks).Special = 16 ' batteries are multi-targetting
                Hits(number_of_attacks).Special = SetFlagsWT(Salvos(aa).DataStr, Int(Hits(number_of_attacks).Special))
                Hits(number_of_attacks).tag = Salvos(aa).DataStr
              End If
            End If
        
            If HasAmmoWT(Salvos(aa).DataStr) Then
              NewTag = "ammo " + Format(ret - 1) + " "
              temp_str = RemoveTag(Salvos(aa).DataStr, "ammo", 1)
              temp_str = AddTag(temp_str, CStr(NewTag))
              Salvos(aa).DataStr = temp_str
              
              ' Reassemble weapon tags back into a unit's appropriate Special tag.
              ' This requires stripping all the weapon tags and then appending them.
              temp_str = ""
              Strip = False
              
              For swt = 1 To Len(TempSpecialB(B))
                If Mid$(TempSpecialB(B), swt, 1) = "[" Then Strip = True
                If Strip = False Then temp_str = temp_str + Mid$(TempSpecialB(B), swt, 1)
                If Mid$(TempSpecialB(B), swt, 1) = "]" Then Strip = False
              Next swt
              
              For bb = 1 To 200
                If Salvos(bb).DataStr = "" Then Exit For
                temp_str = temp_str + Salvos(bb).DataStr
              Next bb
              TempSpecialB(B) = temp_str
              SpecialB(B) = temp_str
            End If
          End If
        Next aa
        End If ' is sc greater than zero

      Else ' there are no batteries
        If HasReserve(TempSpecialB(B)) And Not HasArtillery(SpecialB(B)) Then
          DefBattleStr = DefRaceName + " " + DefShipStr(B) + " is being held in reserve!"
          GoTo ABORT_ATTACK
        End If
    
        If NotFunctional(CurBeamB(B), CurTorpB(B), TempSpecialB(B)) Then
          If ret = 1 Then
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " has no functional weapons!"
            If IsNoBeam(TempSpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "NOBEAM", 0)
            If IsNoTorp(TempSpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "NOTORP", 0)
            GoTo ABORT_ATTACK
          End If
          If ret = 2 Then
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " launches missiles."
            If IsNoBeam(TempSpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "NOBEAM", 0)
            If IsNoTorp(TempSpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "NOTORP", 0)
            GoTo ABORT_ATTACK
          End If
        Else
          If IsNoBeam(TempSpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "NOBEAM", 0)
          If IsNoTorp(TempSpecialB(B)) Then TempSpecialB(B) = RemoveTag(TempSpecialB(B), "NOTORP", 0)
        End If
      
        ' OK, those are the exceptions.  Time to fill those fire and Special arrays...
        ' First of all there may be times when firepower is limited...
        If AttHasLongRange Or DefHasLongRange Then ' Only LR attacks are valid
          If HasLong(SpecialB(B)) = False Then
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " has no long range weapons!"
            GoTo ABORT_ATTACK
          End If
        End If
      
        ' the default is one combined attack
        tmp = 1
        If HasShots(SpecialB(B)) Then If ret = 0 Then tmp = 0
        If AttHasLongRange Or DefHasLongRange Then
          HasLong (SpecialB(B))
          If ret = 2 Then tmp = 0
        End If
        If HasReserve(TempSpecialB(B)) Then  ' only true if artillery present
          HasArtillery (SpecialB(B))
          If ret = 2 Then tmp = 0        ' no beam
        End If
        If IsNoBeam(SpecialB(B)) Or tmp = 0 Then
          ' oops, no beam play today
          FirePower1 = 0
          Special1 = 0
        Else
          If HasShots(SpecialB(B)) Then
            NewTag = "SHOTS " + Format(ret - 1) + " "
            TempSpecialB(B) = RemoveTag(TempSpecialB(B), "SHOTS", 1)
            TempSpecialB(B) = AddTag(TempSpecialB(B), CStr(NewTag))
          End If
          FirePower1 = CInt(CurBeamB(B))
          Special1 = SetFlags(SpecialB(B), CInt(Special1), 1) ' set flags for beams
        End If
        
        tmp = 1
        If HasAmmo(SpecialB(B)) Then If ret = 0 Then tmp = 0
        If AttHasLongRange Or DefHasLongRange Then
          HasLong (SpecialB(B))
          If ret = 1 Then tmp = 0
        End If
        If HasReserve(TempSpecialB(B)) Then  ' only true if artillery present
          HasArtillery (SpecialB(B))
          If ret = 1 Then tmp = 0        ' no torp
        End If
        If HasMissile(SpecialB(B)) Or IsNoTorp(SpecialB(B)) Or tmp = 0 Then
          ' oops, no torps away today
          FirePower2 = 0
          Special2 = 0
        Else
          If HasAmmo(SpecialB(B)) Then
            NewTag = "AMMO " + Format(ret - 1) + " "
            TempSpecialB(B) = RemoveTag(TempSpecialB(B), "AMMO", 1)
            TempSpecialB(B) = AddTag(TempSpecialB(B), CStr(NewTag))
          End If
          FirePower2 = CurTorpB(B)
          Special2 = SetFlags(SpecialB(B), CInt(Special2), 2) ' set flags for torps
        End If

        ' if normal, add firepowers and combine specials
        number_of_attacks = 0 ' always start at zero
        tmp = 0: If HasFlak(SpecialB(B)) And AttHasFighters = 1 Then tmp = 1
        
        ' check to see if the first attack is a BP attack
        If GetBPAttVal(SpecialB(B)) > 0 Then ' we have boarding parties!
          number_of_attacks = number_of_attacks + 1
          Hits(number_of_attacks).firepower = GetBPAttVal(SpecialB(B))
          Hits(number_of_attacks).Special = sa_bp ' old school BP attacks have no other flags
          Hits(number_of_attacks).tag = ""
        End If
        
        If Not IsSplit(SpecialB(B)) And tmp = 0 And GetMultiBeamVal(SpecialB(B)) = 0 And GetMultiTorpVal(SpecialB(B)) = 0 Then ' a normal
          ' the default is one combined attack
          number_of_attacks = number_of_attacks + 1
          Hits(number_of_attacks).firepower = FirePower1 + FirePower2
          Hits(number_of_attacks).Special = 0
          If FirePower1 > 0 Then
            Hits(number_of_attacks).Special = SetFlags(SpecialB(B), Int(Hits(number_of_attacks).Special), 1) 'add beam values
          End If
          If FirePower2 > 0 Then
            Hits(number_of_attacks).Special = SetFlags(SpecialB(B), Int(Hits(number_of_attacks).Special), 2) 'add torp values
          End If
          Hits(number_of_attacks).tag = SpecialB(B)
        Else
          If IsSplit(SpecialB(B)) Then  ' form two attacks
            If Sgn(FirePower1) = 1 Then
              number_of_attacks = number_of_attacks + 1
              Hits(number_of_attacks).firepower = FirePower1
              Hits(number_of_attacks).Special = Special1
              Hits(number_of_attacks).tag = SpecialB(B)
            End If
            If Sgn(FirePower2) = 1 Then
              number_of_attacks = number_of_attacks + 1
              Hits(number_of_attacks).firepower = FirePower2
              Hits(number_of_attacks).Special = Special2
              Hits(number_of_attacks).tag = SpecialB(B)
            End If
          Else
            ' if FLAK or multi, subdivide the splits
            '   all multi-targetting needs to add sa_multi to the special_array values
            If HasFlak(SpecialB(B)) Then
              Select Case ret
                Case 1
                  If Sgn(FirePower1) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower1
                    For tmp = number_of_attacks - FirePower1 To number_of_attacks
                      If tmp < 1 Then tmp = 1
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special1 + sa_multi
                      Hits(tmp).tag = SpecialB(B)
                    Next
                  End If
                  If Sgn(FirePower2) = 1 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower2
                    Hits(number_of_attacks).Special = Special2
                    Hits(number_of_attacks).tag = SpecialB(B)
                  End If
                Case 2
                  If Sgn(FirePower1) = 1 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower1
                    Hits(number_of_attacks).Special = Special1
                    Hits(number_of_attacks).tag = SpecialB(B)
                  End If
                  If Sgn(FirePower2) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower2
                    For tmp = number_of_attacks - FirePower2 To number_of_attacks
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special2 + sa_multi
                      Hits(tmp).tag = SpecialB(B)
                    Next
                  End If
                Case 3
                  If Sgn(FirePower1) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower1
                    For tmp = number_of_attacks - FirePower1 To number_of_attacks
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special1 + sa_multi
                      Hits(tmp).tag = SpecialB(B)
                    Next
                  End If
                  If Sgn(FirePower2) = 1 Then
                    number_of_attacks = number_of_attacks + FirePower2
                    For tmp = number_of_attacks - FirePower2 To number_of_attacks
                      Hits(tmp).firepower = 1
                      Hits(tmp).Special = Special2 + sa_multi
                      Hits(tmp).tag = SpecialB(B)
                    Next
                  End If
              End Select
            Else 'gee, it must be a multi
              ret = 0
              If GetMultiBeamVal(SpecialB(B)) > 0 Then ret = 1
              If GetMultiTorpVal(SpecialB(B)) > 0 Then ret = ret + 2
              If ret = 0 Then
                ' this should never happen...
                DefBattleStr = DefRaceName + " " + DefShipStr(B) + " has a ship config error in MULTI [DEBUG]"
                GoTo ABORT_ATTACK
              End If
            
              If FirePower1 > 0 Then
                ret = GetMultiBeamVal(SpecialB(B))
                If ret > 0 Then
                  If ret >= FirePower1 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower1
                    Hits(number_of_attacks).Special = Special1 + sa_multi
                    Hits(number_of_attacks).tag = SpecialB(B)
                  Else
                    tmp = FirePower1
                    While tmp > ret
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = ret
                      Hits(number_of_attacks).Special = Special1 + sa_multi
                      Hits(number_of_attacks).tag = SpecialB(B)
                      tmp = tmp - ret
                    Wend
                    If tmp > 0 Then
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = tmp
                      Hits(number_of_attacks).Special = Special1 + sa_multi
                      Hits(number_of_attacks).tag = SpecialB(B)
                    End If
                  End If
                Else
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = FirePower1
                  Hits(number_of_attacks).Special = Special1 + sa_multi
                  Hits(number_of_attacks).tag = SpecialB(B)
                End If
              End If
            
              If FirePower2 > 0 Then
                ret = GetMultiTorpVal(SpecialB(B))
                If ret > 0 Then
                  If ret >= FirePower2 Then
                    number_of_attacks = number_of_attacks + 1
                    Hits(number_of_attacks).firepower = FirePower2
                    Hits(number_of_attacks).Special = Special2 + sa_multi
                    Hits(number_of_attacks).tag = SpecialB(B)
                  Else
                    tmp = FirePower2
                    While tmp > ret
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = ret
                      Hits(number_of_attacks).Special = Special2 + sa_multi
                      Hits(number_of_attacks).tag = SpecialB(B)
                      tmp = tmp - ret
                    Wend
                    If tmp > 0 Then
                      number_of_attacks = number_of_attacks + 1
                      Hits(number_of_attacks).firepower = tmp
                      Hits(number_of_attacks).Special = Special2 + sa_multi
                      Hits(number_of_attacks).tag = SpecialB(B)
                    End If
                  End If
                Else
                  number_of_attacks = number_of_attacks + 1
                  Hits(number_of_attacks).firepower = FirePower2
                  Hits(number_of_attacks).Special = Special2 + sa_multi
                  Hits(number_of_attacks).tag = SpecialB(B)
                End If
              End If
            End If  ' end of is flak else multi
          End If    ' end of is split
        End If      ' end of is single
      End If        ' has batteries
  End Select

ROLL_ATTACKS:
  HasRaided = 0
  If number_of_attacks = 0 Then GoTo ABORT_ATTACK
  
  For D = 1 To number_of_attacks
    firepower = Hits(D).firepower

    If firepower > 0 Then
      AbortCounter = 0
      Do
        AbortCounter = AbortCounter + 1
        If AbortCounter = 10 Then
          If ForceID = 0 Then
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " aborts attack!"
          Else
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " aborts attack!"
          End If
          GoTo ABORT_ATTACK
        End If

        ' This section of code sets Target1.  The variable is set/reset if it is
        ' the first attack roll (D=1) or bit 16 is set for scatter pack targetting
        
        If D = 1 Or (Hits(D).Special And sa_multi) = sa_multi Then
          HullTarget = 0
          If ForceID = 0 Then
            If Hits(D).tag > "" Then CombatStr = Hits(D).tag Else CombatStr = SpecialA(B)
            
            If AttTargetPriority > 0 Then HullTarget = AttTargetPriority
            If HasHull(CombatStr) Then HullTarget = ret ' Scope is set in call to HasHull
            If HasScan(CombatStr) Then HullTarget = ret ' Scope is set in call to HasScan
            If HasDataLink(CombatStr) Or HasDataLinkWT(CombatStr) Then
              'set 'ret' to use the correct string
              If Hits(D).tag > "" Then HasDataLinkWT (Hits(D).tag) Else HasDataLink (SpecialA(B))
              Target1 = DataLinkA(ret)
              If Target1 = -1 Then
                If HasScan(CombatStr) Then
                  Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, Scope, DefNumValidTargets)
                Else
                  Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, Scope, DefNumValidTargets) ' size 0 is no specials
                End If
                'set 'ret' to use the correct string (again)
                If Hits(D).tag > "" Then HasDataLinkWT (Hits(D).tag) Else HasDataLink (SpecialA(B))
                DataLinkA(ret) = Target1
              End If
            Else ' unit is not datalinked
              If HasScan(CombatStr) Then
                Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, Scope, DefNumValidTargets)
              Else
                Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, Scope, DefNumValidTargets) ' size 0 is no specials
              End If
            End If
          Else ' the defender
            If Hits(D).tag > "" Then CombatStr = Hits(D).tag Else CombatStr = SpecialB(B)
            
            If DefTargetPriority > 0 Then HullTarget = DefTargetPriority
            If HasHull(CombatStr) Then HullTarget = ret ' Scope is set call to HasHull
            If HasScan(CombatStr) Then HullTarget = ret ' Scope is set call to HasScan
           If HasDataLink(CombatStr) Or HasDataLinkWT(CombatStr) Then
              'set 'ret' to use the correct string
              If Hits(D).tag > "" Then HasDataLinkWT (Hits(D).tag) Else HasDataLink (SpecialB(B))
              Target1 = DataLinkB(ret)
              If Target1 = -1 Then
                If HasScan(CombatStr) Then
                  Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, Scope, AttNumValidTargets)
                Else
                  Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, Scope, AttNumValidTargets) ' size 0 is no specials
                End If
                'set 'ret' to use the correct string (again)
                If Hits(D).tag > "" Then HasDataLinkWT (Hits(D).tag) Else HasDataLink (SpecialB(B))
                DataLinkB(ret) = Target1
              End If
            Else ' unit is not datalinked
              If HasScan(CombatStr) Then
                Target1 = GetScanTarget(ForceID, CombatStr, HullTarget, Scope, AttNumValidTargets)
              Else
                Target1 = GetHullTarget(ForceID, CombatStr, HullTarget, Scope, AttNumValidTargets)
              End If
            End If
          End If  ' ForceID = 0
        End If    ' D = 1
      
        ' captured, just captured, and cloaked targets are rejected
        SeekTarget = 0
        If ForceID = 0 Then
          If IsCaptured(SpecialB(Target1)) Or BPAttackCritB(Target1) > 99 Or IsCloak(SpecialB(Target1)) Then SeekTarget = 1
        Else
          If IsCaptured(SpecialA(Target1)) Or BPAttackCritA(Target1) > 99 Or IsCloak(SpecialA(Target1)) Then SeekTarget = 1
        End If
      Loop While SeekTarget = 1
      
      ' Since target selection is not done until now, point defense
      ' can not engage missiles until now.
      
      If ForceID = 0 Then
        If HasPointDefense(SpecialB(Target1)) And IsMissile(SpecialA(B)) Then
          If Int(Rnd() * 100) < ret Then
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " missile intercepted!"
            CurHullA(B) = 0
            GoTo ABORT_SHOT
          End If
        End If
      Else
        If HasPointDefense(SpecialA(Target1)) And IsMissile(SpecialB(B)) Then
          If Int(Rnd() * 100) < ret Then
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " missile intercepted!"
            CurHullB(B) = 0
            GoTo ABORT_SHOT
          End If
        End If
      End If
      

      ' Determine attack roll and damage inflicted
      '
      ' Base chance to hit is 100% modified by targeting and defense
      ' Base damage multiplier (1d100 + yield - resistance + great_hit_bonus) [min 1, max 100]
      ' Base damage is int((firepower * multiplier) /100)
      ' Suicide adds to hit which may add to the great_hit_bonus
      '
      ' BPs always 'hit' but may not be effective due to enemy shields or defenders.

      Select Case ForceID
        Case 0 ' the attacker
          If (Hits(D).Special And sa_bp) = sa_bp Then ' this is a BP attack
            tmp = 1  ' default is OK to attempt BP combat
            If IsSolid(SpecialB(Target1)) = True Then tmp = 0     ' invalid target
            If IsFighter(SpecialB(Target1)) = True Then tmp = 0   ' invalid target
            If IsGround(SpecialB(Target1)) = True Then tmp = 0    ' invalid target
            If IsVehicle(SpecialB(Target1)) = True Then tmp = 0   ' invalid target
            If IsMine(SpecialB(Target1)) = True Then tmp = 0      ' invalid target
            If CurShieldB(Target1) > 0 And Not ((Hits(D).Special And sa_pen) = sa_pen) Then tmp = 0             ' shields not down!
            If tmp = 0 Then dice1 = 0 Else dice1 = 100
          Else
            SuicideBonus = 0
            CombatBonus = AttTargetBonus
            YieldBonus = 0
            If IsSuicide(SpecialA(B)) Then SuicideBonus = Int(Rnd * 100) + 1
            If HasTarget(CombatStr) Then CombatBonus = ret
            If HasYield(CombatStr) Then YieldBonus = ret
            If HasResist(SpecialB(Target1)) Then YieldBonus = YieldBonus - ret
            HasDefense (SpecialB(Target1))
            AutoMiss = 0: AutoHit = 0
            dice1 = Int(Rnd() * 100) + 1
            If dice1 = 1 Then AutoMiss = 1
            If dice1 = 100 Then AutoHit = 1
            dice1 = dice1 + DM_ToHitA + CombatBonus - ret
          End If
        Case 1 ' the defender
          If (Hits(D).Special And sa_bp) = sa_bp Then ' this is a BP attack
            tmp = 1  ' default is OK to attempt BP combat
            If IsSolid(SpecialA(Target1)) = True Then tmp = 0     ' invalid target
            If IsFighter(SpecialA(Target1)) = True Then tmp = 0   ' invalid target
            If IsGround(SpecialA(Target1)) = True Then tmp = 0    ' invalid target
            If IsVehicle(SpecialA(Target1)) = True Then tmp = 0   ' invalid target
            If IsMine(SpecialA(Target1)) = True Then tmp = 0      ' invalid target
            If CurShieldA(Target1) > 0 And Not ((Hits(D).Special And sa_pen) = sa_pen) Then tmp = 0              ' shields not down!
            If tmp = 0 Then dice1 = 0 Else dice1 = 100
          Else
            SuicideBonus = 0
            CombatBonus = DefTargetBonus
            YieldBonus = 0
            If IsSuicide(SpecialB(B)) Then SuicideBonus = Int(Rnd * 100) + 1
            If HasTarget(CombatStr) Then CombatBonus = ret
            If HasYield(CombatStr) Then YieldBonus = ret
            If HasResist(SpecialA(Target1)) Then YieldBonus = YieldBonus - ret
            HasDefense (SpecialA(Target1))
            AutoMiss = 0: AutoHit = 0
            dice1 = Int(Rnd() * 100) + 1
            If dice1 = 1 Then AutoMiss = 1
            If dice1 = 100 Then AutoHit = 1
            dice1 = dice1 + DM_ToHitB + CombatBonus + -ret
          End If
      End Select
      
      ' OK, Dice1 < 1 is a miss.  Dice1 > 100 earns a Dice1-100 great_hit_bonus
      ' Dice1 1 to 100 a normal hit.
      
      If (dice1 <= (100 - BaseAccuracy) Or AutoMiss = 1) And AutoHit = 0 Then ' miss
        Damage3 = 0
      Else              ' hit
        If MaximumDamage Then
          Damage3 = firepower
        Else
          If dice1 > 100 Then YieldBonus = (dice1 - 100) + YieldBonus
          Damage3 = Round((Int(Rnd() * firepower) + 1) * (1# + (YieldBonus / 100#)))
          If Damage3 < 1 Then Damage3 = 0
          If Damage3 > firepower Then Damage3 = firepower
        End If
      End If
      Damage2 = 0                           ' clear Pen damage value
      Damage1 = Damage3
      
      If ForceID = 0 Then
        If dice1 <= (100 - BaseAccuracy) Then
          If (Hits(D).Special And sa_bp) = sa_bp Then
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " can not engage " + DefRaceName + " " + DefShipStr(Target1) + " in boarding party combat."
          Else
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " fires on " + DefRaceName + " " + DefShipStr(Target1) + " and misses."
          End If
        Else
          If (Hits(D).Special And sa_bp) = sa_bp Then
            AttVal = GetBPAttVal(SpecialA(B))             ' determine attack ability
            If AttVal = 0 Then AttVal = firepower         ' use bracket value
            
            ' calculate defense ability
            DefVal = GetBPDefVal(CurHullB(Target1), SpecialB(Target1))
            
            BPDice = Int(Rnd() * (DefVal + AttVal)) + 1
            If BPDice > DefVal Then                       ' attackers victory
              If (Int(Rnd() * 4)) = 0 Then
                ' defender captured
                AttBattleStr = AttRaceName + " " + AttShipStr(B) + " captures " + DefShipStr(Target1)
                BPAttackCritB(Target1) = BPAttackCritB(Target1) + 100
                TempSpecialB(Target1) = AddTag(TempSpecialB(Target1), "NOMOVE CAPTURED ")  ' ship can not run away
' BP expended
              Else
                ' defender suffers critical hit
                AttBattleStr = AttRaceName + " " + AttShipStr(B) + " is conducting a hit and run raid on " + DefShipStr(Target1)
                BPAttackCritB(Target1) = BPAttackCritB(Target1) + 1
                Shields = 0: Crits = 0: Hull = TempCurHullB(Target1): X = Target1
                If TempCurHullB(Target1) = 0 Then
                  tmp = 0
                Else
                  tmp = 100 - ((TempCurHullB(Target1) * 100) / MaxHullB(Target1))
                End If
                AttacksIndex = AttacksIndex + 1
                Attacks(AttacksIndex).AttackID = A ' A is a unique ship ID
                Attacks(AttacksIndex).TargetID = Target1 + AttShipsLeft
                Attacks(AttacksIndex).Damage = 0
                Attacks(AttacksIndex).Weapon = sa_bp   ' special crit hit value
                Attacks(AttacksIndex).Special = Hits(D).tag
                If AttacksIndex = AttacksMax Then
                  MsgBox "I've reached the maximum number of attacks I can handle.", vbCritical + vbOKOnly, "BE Fatal Error"
                  End
                End If
              End If
            Else                                       ' else attackers repulsed
              If (Int(Rnd() * 4)) = 0 Then
                ' defenders win
                AttBattleStr = AttRaceName + " " + AttShipStr(B) + " borders ambushed, multiple casualties!"
' BP expended
              Else
                AttBattleStr = AttRaceName + " " + AttShipStr(B) + " borders repulsed!"
              End If
            End If
          Else
            AttBattleStr = AttRaceName + " " + AttShipStr(B) + " fires on " + DefRaceName + " " + DefShipStr(Target1) + " scoring " + Format(Damage1) + " hits (" + Format(dice1) + "%)"
            AttacksIndex = AttacksIndex + 1
            Attacks(AttacksIndex).AttackID = A ' A is a unique ship ID
            Attacks(AttacksIndex).TargetID = Target1 + AttShipsLeft
            Attacks(AttacksIndex).Damage = Damage1
            Attacks(AttacksIndex).Weapon = Hits(D).Special
            Attacks(AttacksIndex).Special = Hits(D).tag
            If AttacksIndex = AttacksMax Then
              MsgBox "I've reached the maximum number of attacks I can handle.", vbCritical + vbOKOnly, "BE Fatal Error"
              End
            End If
          End If
        End If
        If number_of_attacks > 1 Then AttBattleStr = AttBattleStr + " [" + Format(D) + " of " + Format(number_of_attacks) + "]"
      Else  ' forceID = 1
        If dice1 <= (100 - BaseAccuracy) Then
          If (Hits(D).Special And sa_bp) = sa_bp Then
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " can not engage " + AttRaceName + " " + AttShipStr(Target1) + " in boarding party combat."
         Else
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " fires on " + AttRaceName + " " + AttShipStr(Target1) + " and misses."
          End If
        Else
          If (Hits(D).Special And sa_bp) = sa_bp Then
            AttVal = GetBPAttVal(SpecialB(B))             ' determine attack ability
            If AttVal = 0 Then AttVal = firepower         ' use bracket value
            
            ' calculate defense ability
            DefVal = GetBPDefVal(CurHullA(Target1), SpecialA(Target1))
            
            BPDice = Int(Rnd() * (DefVal + AttVal)) + 1
            If BPDice > DefVal Then                       ' attackers victory
              If (Int(Rnd() * 4)) = 0 Then
                ' defender captured
                DefBattleStr = DefRaceName + " " + DefShipStr(B) + " captures " + AttShipStr(Target1)
                BPAttackCritA(Target1) = BPAttackCritA(Target1) + 100
                TempSpecialA(Target1) = AddTag(TempSpecialA(Target1), "NOMOVE CAPTURED ")  ' ship can not run away
' BP expended
              Else
                ' defender suffers critical hit
                DefBattleStr = DefRaceName + " " + DefShipStr(B) + " is conducting a hit and run raid on " + AttShipStr(Target1)
                BPAttackCritA(Target1) = BPAttackCritA(Target1) + 1
                Shields = 0: Crits = 0: Hull = TempCurHullA(Target1): X = Target1
                If TempCurHullA(Target1) = 0 Then
                  tmp = 0
                Else
                  tmp = 100 - ((TempCurHullA(Target1) * 100) / MaxHullA(Target1))
                End If
                AttacksIndex = AttacksIndex + 1
                Attacks(AttacksIndex).AttackID = A ' A is a unique ship ID
                Attacks(AttacksIndex).TargetID = Target1
                Attacks(AttacksIndex).Damage = 0
                Attacks(AttacksIndex).Weapon = sa_bp   ' special crit hit value
                Attacks(AttacksIndex).Special = Hits(D).tag
                If AttacksIndex = AttacksMax Then
                  MsgBox "I've reached the maximum number of attacks I can handle.", vbCritical + vbOKOnly, "BE Fatal Error"
                  End
                End If
              End If
            Else                                       ' else attackers repulsed
              If (Int(Rnd() * 4)) = 0 Then
                ' defenders win
                DefBattleStr = DefRaceName + " " + DefShipStr(B) + " borders ambushed, multiple casualties!"
' BP expended
              Else
                DefBattleStr = DefRaceName + " " + DefShipStr(B) + " borders repulsed!"
              End If
            End If
          Else
            DefBattleStr = DefRaceName + " " + DefShipStr(B) + " fires on " + AttRaceName + " " + AttShipStr(Target1) + " scoring " + Format(Damage1) + " hits (" + Format(dice1) + "%)"
            AttacksIndex = AttacksIndex + 1
            Attacks(AttacksIndex).AttackID = A ' A is a unique ship ID
            Attacks(AttacksIndex).TargetID = Target1
            Attacks(AttacksIndex).Damage = Damage1
            Attacks(AttacksIndex).Weapon = Hits(D).Special
            Attacks(AttacksIndex).Special = Hits(D).tag
            If AttacksIndex = AttacksMax Then
              MsgBox "I've reached the maximum number of attacks I can handle.", vbCritical + vbOKOnly, "BE Fatal Error"
              End
            End If
          End If
        End If
        If number_of_attacks > 1 Then DefBattleStr = DefBattleStr + " [" + Format(D) + " of " + Format(number_of_attacks) + "]"
      End If
    Else                       ' strange, firepower IS equal to zero...
      If ForceID = 0 Then
         AttBattleStr = AttRaceName + " " + AttShipStr(B) + " No normal attack.  (Firepower is equal to zero.)"
      Else
         DefBattleStr = DefRaceName + " " + DefShipStr(B) + " No normal attack.  (Firepower is equal to zero.)"
      End If
    End If

ABORT_SHOT:
    If D < number_of_attacks Then
      If ForceID = 0 Then
        If AttBattleStr > "" Then Print #3, AttBattleStr
      Else
        If DefBattleStr > "" Then Print #3, DefBattleStr
      End If
    End If
  Next D    ' end of multi-attack routine

ABORT_ATTACK:
    If ForceID = 0 Then
      If AttBattleStr > "" Then Print #3, AttBattleStr
    Else
      If DefBattleStr > "" Then Print #3, DefBattleStr
    End If
        
  Next A
  Print #3, " "

' ----------------------------------------------------------------------------
      
  ' Damage routine
  
  AttBattleStr = "":  DefBattleStr = ""
  
  For A = 1 To AttShipsLeft + DefShipsLeft
    B = A: ForceID = 0
    If A > AttShipsLeft Then B = A - AttShipsLeft: ForceID = 1
    
    If A = AttShipsLeft + 1 Then Print #3, ""
      
'   Attacks(AttacksIndex).AttackID = B
'   Attacks(AttacksIndex).TargetID = Target1
'   Attacks(AttacksIndex).Damage = Damage1
'   Attacks(AttacksIndex).Weapon = Hits(D).special
'   Attacks(AttacksIndex).Special = Hits(D).tag
      
    ' for each ship check vis unique ID to see if it was attacked
    ShipHit = 0
    For E = 1 To AttacksIndex
      If Attacks(E).TargetID = A Then
    
      B = Attacks(E).AttackID
      If B > AttShipsLeft Then
        B = B - AttShipsLeft
        ForceID = 1
        Target1 = Attacks(E).TargetID
      Else
        ForceID = 0
        Target1 = Attacks(E).TargetID - AttShipsLeft
      End If
      
      Damage1 = Attacks(E).Damage
      
      If ForceID = 0 Then
        If ShipHit = 0 Then Print #3, DefShipStr(Target1) + " has been hit.": ShipHit = 1
      Else
        If ShipHit = 0 Then Print #3, AttShipStr(Target1) + " has been hit.": ShipHit = 1
      End If
      
      If (Attacks(E).Weapon And sa_bp) = sa_bp Then
        Print #3, "  Boarding party attack is in progress.";
      Else
        Print #3, "  Takes" + Str(Damage1) + " points of damage.";
      End If
      
      If ForceID = 0 Then
          If HasFlicker(SpecialB(Target1)) Then
            If Int(Rnd * 100) + 1 <= ret Then
              Print #3, " <attack deflected>";
              Damage1 = 0 ' GoTo ABORT_SHOT
            End If
          End If
          If CurShieldB(Target1) > 0 Then
            If HasShields(SpecialB(Target1)) Then  ' check for SR
              Damage1 = Damage1 - ret
              If Damage1 < 1 Then
                Print #3, " <attack deflected>";
                Damage1 = 0 ' GoTo ABORT_SHOT
              Else
                Print #3, " <" + Format(ret) + " pts deflected>";
              End If
            Else
              ' check for crack
              If ((Attacks(E).Weapon And sa_crack) = sa_crack) Then
                If Damage1 * 2 < TempCurShieldB(Target1) Then
                  ret = Damage1 * 2
                  TempCurShieldB(Target1) = TempCurShieldB(Target1) - (Damage1 * 2)
                  Damage1 = 0
                  Print #3, " <shield disruption>";
                Else
                  ret = TempCurShieldB(Target1)
                  TempCurShieldB(Target1) = 0
                  Damage1 = 0
                  Print #3, " <shield disrupted>";
                End If
              End If
            End If
          Else ' no shields
            ' check for crack
            If ((Attacks(E).Weapon And sa_crack) = sa_crack) Then
              Damage1 = 0
              Print #3, " <attack dissipates>";
            End If
          End If
      Else ' defender
          If HasFlicker(SpecialA(Target1)) Then
            If Int(Rnd * 100) + 1 <= ret Then
              Print #3, " <attack deflected>";
              Damage1 = 0 ' GoTo ABORT_SHOT
            End If
          End If
          If CurShieldA(Target1) > 0 Then
            If HasShields(SpecialA(Target1)) Then  ' check for SR
              Damage1 = Damage1 - ret
              If Damage1 < 1 Then
                Print #3, " <attack deflected>";
                Damage1 = 0 ' GoTo ABORT_SHOT
              Else
                Print #3, " <" + Format(ret) + " pts deflected>";
              End If
            Else
              ' check for crack
              If ((Attacks(E).Weapon And sa_crack) = sa_crack) Then
                If Damage1 * 2 < TempCurShieldA(Target1) Then
                  ret = Damage1 * 2
                  TempCurShieldA(Target1) = TempCurShieldA(Target1) - (Damage1 * 2)
                  Damage1 = 0
                  Print #3, " <shield disruption>";
                Else
                  ret = TempCurShieldA(Target1)
                  TempCurShieldA(Target1) = 0
                  Damage1 = 0
                  Print #3, " <shield disrupted>";
                End If
              End If
            End If
          Else ' no shields
            ' check for crack
            If ((Attacks(E).Weapon And sa_crack) = sa_crack) Then
              Damage1 = 0
              Print #3, " <attack dissipates>";
            End If
          End If
      End If
      
      Select Case ForceID
        Case 0 ' the attacker
              If HasPen(SpecialA(B)) And Damage1 > 0 Then
                Damage1 = Damage1 - 1
                Damage2 = 1
                TempHitsB(Target1) = TempHitsB(Target1) + Damage1
                TempPenHitsB(Target1) = TempPenHitsB(Target1) + 1
              Else
                TempHitsB(Target1) = TempHitsB(Target1) + Damage1
              End If
              If IsSuicide(SpecialA(B)) Then TempCurHullA(B) = 0  ' it is a suicide attack
        Case 1 ' the defender
              If HasPen(SpecialB(B)) And Damage1 > 0 Then
                Damage1 = Damage1 - 1
                Damage2 = 1
                TempHitsA(Target1) = TempHitsA(Target1) + Damage1
                TempPenHitsA(Target1) = TempPenHitsA(Target1) + 1
              Else
                TempHitsA(Target1) = TempHitsA(Target1) + Damage1
              End If
              If IsSuicide(SpecialB(B)) Then TempCurHullB(B) = 0    ' it is a suicide attack
      End Select

' Start damage allocation phase
    X = Target1  ' <-----------------------------------  our hapless target :-)
    temp_str = "": ShipCritStr = "": Crits = 0
    
    If ForceID = 0 Then
       If TempCurShieldB(X) < Damage1 Then  ' we're going to blow through the shields
         Shields = 0
         DamageLevel = Damage1 - TempCurShieldB(X)
       Else
         Shields = TempCurShieldB(X) - Damage1
         DamageLevel = 0
       End If
       DamageLevel = DamageLevel + Damage2  ' Penetrating hits bypass shields

       If DamageLevel > 0 And HasArmor(SpecialB(Target1)) Then  ' check for AR
         DamageLevel = DamageLevel - ret
         If DamageLevel < 1 Then
           Print #3, " <attack blocked>";
           TempCurShieldB(X) = Shields
           If (Attacks(E).Weapon And sa_bp) <> sa_bp Then Print #3, "": GoTo ABORT_DAMAGE ' marines get through
         Else
           Print #3, " <" + Format(ret) + " pts blocked>";
         End If
       End If
    Else
       If TempCurShieldA(X) < Damage1 Then  ' we're going to blow through the shields
         Shields = 0
         DamageLevel = Damage1 - TempCurShieldA(X)
       Else
         Shields = TempCurShieldA(X) - Damage1
         DamageLevel = 0
       End If
       DamageLevel = DamageLevel + Damage2  ' Penetrating hits bypass shields

       If DamageLevel > 0 And HasArmor(SpecialA(Target1)) Then  ' check for AR
         DamageLevel = DamageLevel - ret
         If DamageLevel < 1 Then
           Print #3, " <attack blocked>";
           TempCurShieldA(X) = Shields
           If (Attacks(E).Weapon And sa_bp) <> sa_bp Then Print #3, "": GoTo ABORT_DAMAGE ' marines get through
         Else
           Print #3, " <" + Format(ret) + "pts blocked>";
         End If
       End If
    End If

    Print #3, ""
      
    ' stasis snap shield logic
    ' stasis shield only activates when the shield fails
    ' so pen-hits will get though unless the shield falls at the same time
    If ForceID = 0 Then
      If IsStasis(SpecialB(X)) Then
        If Shields = 0 And CurShieldB(X) > 0 And DamageLevel > 0 Then
          DamageLevel = 0: TempCurShieldB(X) = 0
          Print #3, "  Stasis field activated.  Extra damage deflected."
          GoTo ABORT_DAMAGE
        End If
      End If
    Else
      If IsStasis(SpecialA(X)) Then
        If Shields = 0 And CurShieldA(X) > 0 And DamageLevel > 0 Then
          DamageLevel = 0: TempCurShieldA(X) = 0
          Print #3, "  Stasis field activated.  Extra damage deflected."
          GoTo ABORT_DAMAGE
        End If
      End If
    End If
    
    If ForceID = 0 Then
      If TempCurHullB(X) = 0 Then tmp = 0 Else tmp = 100 - ((TempCurHullB(X) * 100#) / MaxHullB(X))
      Hull = TempCurHullB(X) - DamageLevel
      If Hull < 0 Then Hull = 0
      If MaxShieldB(X) = 0 Or Shields = 0 Then ShieldsPercent = 0 Else ShieldsPercent = (Shields * 100) / MaxShieldB(X)
      If MaxHullB(X) = 0 Or Hull = 0 Then HullPercent = 0 Else HullPercent = 100 - ((Hull * 100#) / MaxHullB(X))
    Else
      If TempCurHullA(X) = 0 Then tmp = 0 Else tmp = 100 - ((TempCurHullA(X) * 100#) / MaxHullA(X))
      Hull = TempCurHullA(X) - DamageLevel
      If Hull < 0 Then Hull = 0
      If MaxShieldA(X) = 0 Or Shields = 0 Then ShieldsPercent = 0 Else ShieldsPercent = (Shields * 100) / MaxShieldA(X)
      If MaxHullA(X) = 0 Or Hull = 0 Then HullPercent = 0 Else HullPercent = Int(100 - ((Hull * 100#) / MaxHullA(X)))
    End If
    
    ' OK, if we got this far we have inflicted some damage.  Possibly even ZERO damage
    ' due to shield absorption.  But, it's enough to possibly trigger a special effect.
    '
    ' Special attribute flags
    '   1  sa_dis
    '   2  sa_heat
    '   4  sa_meson
    '   8  sa_vibro
    '  16  sa_multi    scatter pack (used for targetting)
    '  32  sa_crack
    '  64  sa_bp         boarding party
    ' 128  sa_pen

    Crits = 0
    CRIT_DIS = 0
    CRIT_HEAT = 0
    CRIT_MESON = 0
    CRIT_VIBRO = 0
    CRIT_BP = 0
    CRIT_SPECIAL = 0
    
    ' It's possible to have multiple special effect weapons, so each is rolled separately
    ' Yes.  That means a heat/dis weapon has a 40% of causing a crit.
    
    If Attacks(E).Weapon > 0 Then
      dice = Int(Rnd() * 10)
      If (dice < 2) And ((Attacks(E).Weapon And sa_dis) = sa_dis) Then ' base 20% chance of a disabling shot
        CRIT_DIS = 1
      End If
      
      dice = Int(Rnd() * 10)
      If (dice < 2) And ((Attacks(E).Weapon And sa_heat) = sa_heat) Then ' base 20% chance of a fire
        CRIT_HEAT = 1
      End If
      
      dice = Int(Rnd() * 10)
      If (dice < 2) And ((Attacks(E).Weapon And sa_meson) = sa_meson) Then ' base 20% chance of an explosion
        CRIT_MESON = 1
      End If
      
      dice = Int(Rnd() * 10)
      If (dice < 2) And ((Attacks(E).Weapon And sa_vibro) = sa_vibro) Then ' base 20% chance of a vibro hit
        CRIT_VIBRO = 1
      End If
    
      dice = Int(Rnd() * 10)
      If (dice < 2) And ((Attacks(E).Weapon And sa_special) = sa_special) Then ' base 20% chance of a special hit
        CRIT_SPECIAL = 1
      End If
    
      If (Attacks(E).Weapon And sa_bp) = sa_bp Then ' the marines scored at crit!
        CRIT_BP = 1
      End If
    End If

    ' Figure the damage threshold.
    Select Case tmp
      Case 0 To 19
        If HullPercent >= 20 Then  ' the first threshold has been met
          Crits = 1
          If HullPercent >= 40 Then  ' the second threshold has been met
            Crits = 2
            If HullPercent >= 60 Then  ' the third threshold has been met
              Crits = 3
              If HullPercent >= 80 Then  ' the fourth threshold has been met
                Crits = 4
              End If
            End If
          End If
        End If
      Case 20 To 39
         If HullPercent >= 40 Then  ' the second threshold has been met
           Crits = 1
           If HullPercent >= 60 Then  ' the third threshold has been met
             Crits = 2
             If HullPercent >= 80 Then  ' the fourth threshold has been met
               Crits = 3
             End If
           End If
         End If
      Case 40 To 59
         If HullPercent >= 60 Then  ' the third threshold has been met
           Crits = 1
           If HullPercent >= 80 Then  ' the fourth threshold has been met
             Crits = 2
           End If
         End If
      Case 60 To 79
         If HullPercent >= 80 Then  ' the fourth threshold has been met
           Crits = 1
         End If
    End Select
    Crits = Crits + CRIT_DIS + CRIT_HEAT + CRIT_MESON + CRIT_VIBRO + CRIT_BP + CRIT_SPECIAL

    temp_str = "": CriticalStr = ""
    Select Case ForceID
      Case 0
        ' fighters and ground units ignore the crit hit chart
        If IsFighter(SpecialB(X)) Or IsGround(SpecialB(X)) Or IsMine(SpecialB(X)) Then
          ' fighters are fragile and any damage destroys them
          If IsFighter(SpecialB(X)) And DamageLevel > 0 Then
            ShipCritStr = "Fighter Destroyed"
            temp_str = ""
            Hull = 0
          End If
          If IsGround(SpecialB(X)) And Hull = 0 Then
            ShipCritStr = "Unit Destroyed"
            temp_str = ""
          End If
          If IsMine(SpecialB(X)) And Hull = 0 Then
            ShipCritStr = "Mine Destroyed"
            temp_str = ""
          End If
        Else
          If Crits > 0 Then
            ReactorBreachFlag = 0
            For C = 1 To Crits
              CritDamageFlag = 0
              CritSpecialFlag = 0
    
              'call the critical hit generator function
              If ReactorBreachFlag <> 1 Then
                CriticalStr = FetchData(GetCrit(SpecialB(X)))  ' defaults to normal (5)
                If IsBuilding(SpecialB(X)) Then CriticalStr = FetchData(6) ' building
                If IsCarrier(SpecialB(X)) Then CriticalStr = FetchData(7)  ' carrier
                If IsOrbital(SpecialB(X)) Then CriticalStr = FetchData(8)  ' orbital
                If IsVolatile(SpecialB(X)) Then CriticalStr = FetchData(9) ' volatile
                If IsBio(SpecialB(X)) Then CriticalStr = FetchData(10)     ' bio-ship
                If IsVehicle(SpecialB(X)) Then CriticalStr = FetchData(11) ' vehicle
                
                If CRIT_DIS = 1 Then
                  CRIT_DIS = 0: CriticalStr = FetchData(1) ' dis
                Else
                  If CRIT_HEAT = 1 Then
                    CRIT_HEAT = 0: CriticalStr = FetchData(2) ' heat
                  Else
                    If CRIT_MESON = 1 Then
                      CRIT_MESON = 0: CriticalStr = FetchData(3) ' meson
                    Else
                      If CRIT_VIBRO = 1 Then
                        CRIT_VIBRO = 0: CriticalStr = FetchData(4) ' vibro
                      Else
                        If CRIT_SPECIAL = 1 Then
                          If InStr(Attacks(E).Special, "[") > 0 Then ' use brackets
                            HasSpecialWT (Attacks(E).Special)
                          Else
                            HasSpecial (Attacks(E).Special)
                          End If
                          CriticalStr = FetchData(ret) ' special
                          CRIT_SPECIAL = 0
                        End If
                      End If
                    End If
                  End If
                End If
                
                Print #4, DefRaceName + " " + DefShipStr(X) + " " + CriticalStr
                If CRIT_BP = 1 Then
                  If CRIT_DIS = 0 And CRIT_HEAT = 0 And CRIT_MESON = 0 And CRIT_VIBRO = 0 Then
                    CRIT_BP = 0: CriticalStr = "BP: " + CriticalStr
                  End If
                End If
              End If
              
              If (FetchValue > 1 And FetchValue < 11) Or FetchValue = 100 Then CritDamageFlag = FetchValue
              If (FetchValue > 10 And FetchValue < 100) Then CritSpecialFlag = FetchValue - 10
              
              'constant CRIT_WEAPON_ONE   1   Weapons Offline for 1 turn
              'constant CRIT_WEAPON_HALF  2   Half Weapons Offline until repaired
              'constant CRIT_WEAPON_NONE  3   All Weapons Offline until repaired
              'constant CRIT_DRIFTING     4   No movement for 1 turn
              'constant CRIT_NOMOVE       5   No movement until repaired
              'constant CRIT_CRIPPLE      6   Crippled
              'constant CRIT_SHIELDS      7   Shields Offline
              'constant CRIT_AMMO         8   Ammo explosion
              'constant CRIT_CREW1        9   5% crew casualties
              'constant CRIT_CREW2       10   10% crew casualties
              'constant CRIT_CREW3       11   25% crew casualties + cripple

              Select Case CritSpecialFlag
                Case 1: 'Weapons Offline for 1 turn
                  If HasBatteries(TempSpecialB(X)) Then
                    TempSpecialB(X) = InsertOffline(TempSpecialB(X))
                  Else
                    TempSpecialB(X) = AddTag(TempSpecialB(X), "NOBEAM ")
                    TempSpecialB(X) = AddTag(TempSpecialB(X), "NOTORP ")
                  End If
                Case 2: ' Half Weapons Offline until repaired
                  If HasBatteries(TempSpecialB(X)) Then
                    TempSpecialB(X) = InsertRandomOffline(TempSpecialB(X))
                  Else
                    If (Not IsNoBeam(TempSpecialB(X))) And (TempCurBeamB(X) > 0) Then
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "NOBEAM ")
                    Else
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "NOTORP ")
                    End If
                  End If
                Case 3: ' All Weapons Offline until repaired
                  TempSpecialB(X) = AddTag(TempSpecialB(X), "CRIPPLE ")
                Case 4: ' No movement for 1 turn
                  TempSpecialB(X) = AddTag(TempSpecialB(X), "DRIFTING ")
                Case 5: ' No movement until repaired
                  TempSpecialB(X) = AddTag(TempSpecialB(X), "NOMOVE ")
                Case 6: ' Crippled
                  TempSpecialB(X) = AddTag(TempSpecialB(X), "NOMOVE ")
                  TempSpecialB(X) = AddTag(TempSpecialB(X), "CRIPPLE ")
                Case 7:
                  TempCurShieldB(X) = 0: Shields = 0
                  ShieldsPercent = 0
                Case 8:
                  CritDamageFlag = TempCurTorpB(X)
                Case 9: ' 5% crew casualties
                  If Not IsSolid(SpecialB(X)) Then
                    If HasCrew(TempSpecialB(X)) Then ' reduce crew level
                      ret = ret - 5
                      If ret <= 0 Then ret = 0: TempSpecialB(X) = AddTag(TempSpecialB(X), "CRIPPLE ")
                      TempSpecialB(X) = RemoveTag(TempSpecialB(X), "CREW", 1)
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "CREW " + Format(ret) + " ")
                    Else
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "CREW 95 ")
                    End If
                  End If
                Case 10: ' 10% crew casualties
                  If Not IsSolid(SpecialB(X)) Then
                    If HasCrew(TempSpecialB(X)) Then ' reduce crew level
                      ret = ret - 10
                      If ret <= 0 Then ret = 0: TempSpecialB(X) = AddTag(TempSpecialB(X), "CRIPPLE ")
                      TempSpecialB(X) = RemoveTag(TempSpecialB(X), "CREW", 1)
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "CREW " + Format(ret) + " ")
                    Else
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "CREW 90 ")
                    End If
                  End If
                Case 11: ' 25% crew casualties and all weapons offline until repaired
                  If Not IsSolid(SpecialB(X)) Then
                    If HasCrew(TempSpecialB(X)) Then ' reduce crew level
                      ret = ret - 25
                      If ret <= 0 Then ret = 0
                      TempSpecialB(X) = RemoveTag(TempSpecialB(X), "CREW", 1)
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "CREW " + Format(ret) + " ")
                    Else
                      TempSpecialB(X) = AddTag(TempSpecialB(X), "CREW 75 ")
                    End If
                  End If
                  TempSpecialB(X) = AddTag(TempSpecialB(X), "CRIPPLE ")
              End Select
              
              Hull = Hull - CritDamageFlag
              If Hull < 1 Then HullPercent = 100: Hull = 0 Else HullPercent = 100 - ((Hull * 100) / TempCurHullB(X))
              If CritDamageFlag = 100 Then Crits = 1: Exit For
            Next C
          End If ' end of crit calc code
          If Hull = 0 Then
            If IsMissile(SpecialB(X)) Then
              ShipCritStr = "Missile Destroyed."
            Else
              If IsBuilding(SpecialB(X)) Or IsOrbital(SpecialB(X)) Then
                ShipCritStr = "Base Destroyed."
              Else
                ShipCritStr = "Unit Destroyed."
              End If
            End If
          End If
        End If '   end of is_fighter code
    
        If Hull = 0 Then Shields = 0: TempCurBeamB(X) = 0: TempCurTorpB(X) = 0
        If Hull > 0 And BPAttackCritB(X) > 99 Then
          If IsBuilding(SpecialB(X)) Or IsOrbital(SpecialB(X)) Then
            ShipCritStr = "Base Captured."
          Else
            ShipCritStr = "Ship Captured."
          End If
          TempSpecialB(X) = AddTag(TempSpecialB(X), "CAPTURED ")
        End If
        DefCritStr(X) = CriticalStr
        If DefCritStr(X) > "" Then Print #3, "    " + DefCritStr(X)
        TempStr = "  Bm=" + Format(TempCurBeamB(X)) + " Sh=" + Format(Shields) + " Tp=" + Format(TempCurTorpB(X)) + " Hl=" + Format(Hull) + "  " + ShipCritStr
        TempCurDamB(X) = HullPercent
        If Left$(ShipCritStr, 1) > " " Then Print #4, DefRaceName + " " + DefShipStr(X) + " " + ShipCritStr
        TempCurShieldB(X) = Shields
        TempCurHullB(X) = Hull
        
      Case 1
        ' fighters and ground units ignore the crit hit chart
        If IsFighter(SpecialA(X)) Or IsGround(SpecialA(X)) Or IsMine(SpecialA(X)) Then
          ' fighters are fragile and any damage destroys them
          If IsFighter(TempSpecialA(X)) And DamageLevel > 0 Then
            ShipCritStr = "Fighter Destroyed"
            temp_str = ""
            Hull = 0
          End If
          If IsGround(TempSpecialA(X)) And Hull = 0 Then
            ShipCritStr = "Unit Destroyed"
            temp_str = ""
          End If
          If IsMine(TempSpecialA(X)) And Hull = 0 Then
            ShipCritStr = "Mine Destroyed"
            temp_str = ""
          End If
        Else
          If Crits > 0 Then
            ReactorBreachFlag = 0
            For C = 1 To Crits
              CritDamageFlag = 0
              CritSpecialFlag = 0
    
              'call the critical hit generator function
              If ReactorBreachFlag <> 1 Then
                CriticalStr = FetchData(GetCrit(SpecialA(X)))  ' defaults to normal (5)
                If IsBuilding(SpecialA(X)) Then CriticalStr = FetchData(6) ' building
                If IsCarrier(SpecialA(X)) Then CriticalStr = FetchData(7)  ' carrier
                If IsOrbital(SpecialA(X)) Then CriticalStr = FetchData(8)  ' orbital
                If IsVolatile(SpecialA(X)) Then CriticalStr = FetchData(9) ' volatile
                If IsBio(SpecialA(X)) Then CriticalStr = FetchData(10)     ' bio-ship
                If IsVehicle(SpecialA(X)) Then CriticalStr = FetchData(11) ' vehicle
                
                If CRIT_DIS = 1 Then
                  CRIT_DIS = 0: CriticalStr = FetchData(1) ' dis
                Else
                  If CRIT_HEAT = 1 Then
                    CRIT_HEAT = 0: CriticalStr = FetchData(2) ' heat
                  Else
                    If CRIT_MESON = 1 Then
                      CRIT_MESON = 0: CriticalStr = FetchData(3) ' meson
                    Else
                      If CRIT_VIBRO = 1 Then
                        CRIT_VIBRO = 0: CriticalStr = FetchData(4) ' vibro
                      Else
                        If CRIT_SPECIAL > 0 Then
                          If InStr(Attacks(E).Special, "[") > 0 Then ' use brackets
                            HasSpecialWT (Attacks(E).Special)
                          Else
                            HasSpecial (Attacks(E).Special)
                          End If
                          CriticalStr = FetchData(ret) ' special
                          CRIT_SPECIAL = 0
                        End If
                      End If
                    End If
                  End If
                End If
                
                Print #4, AttRaceName + " " + AttShipStr(X) + " " + CriticalStr
                If CRIT_BP = 1 Then
                  If CRIT_DIS = 0 And CRIT_HEAT = 0 And CRIT_MESON = 0 And CRIT_VIBRO = 0 Then
                    CRIT_BP = 0: CriticalStr = "BP: " + CriticalStr
                  End If
                End If
              End If
    
              If (FetchValue > 1 And FetchValue < 11) Or FetchValue = 100 Then CritDamageFlag = FetchValue
              If (FetchValue > 10 And FetchValue < 100) Then CritSpecialFlag = FetchValue - 10
              
              ' CRIT_WEAPON_ONE   1   Weapons Offline for 1 turn
              ' CRIT_WEAPON_HALF  2   Half Weapons Offline until repaired
              ' CRIT_WEAPON_NONE  3   All Weapons Offline until repaired
              ' CRIT_DRIFTING     4   No movement for 1 turn
              ' CRIT_NOMOVE       5   No movement until repaired
              ' CRIT_CRIPPLE      6   Crippled
              ' CRIT_SHIELDS      7   Shields Offline
              ' CRIT_AMMO         8   Ammo explosion
              ' CRIT_CREW1        9   5% crew casualties
              ' CRIT_CREW2       10   10% crew casualties
              ' CRIT_CREW3       11   25% crew casualties + cripple
              
              Select Case CritSpecialFlag
                Case 1: 'Weapons Offline for 1 turn
                  If HasBatteries(TempSpecialA(X)) Then
                    TempSpecialA(X) = InsertOffline(TempSpecialA(X))
                  Else
                    TempSpecialA(X) = AddTag(TempSpecialA(X), "NOBEAM ")
                    TempSpecialA(X) = AddTag(TempSpecialA(X), "NOTORP ")
                  End If
                Case 2: ' Half Weapons Offline until repaired
                  If HasBatteries(TempSpecialA(X)) Then
                    TempSpecialA(X) = InsertRandomOffline(TempSpecialA(X))
                  Else
                    If (Not IsNoBeam(TempSpecialA(X))) And (TempCurBeamA(X) > 0) Then
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "NOBEAM ")
                    Else
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "NOTORP ")
                    End If
                  End If
                Case 3: ' All Weapons Offline until repaired
                  TempSpecialA(X) = AddTag(TempSpecialA(X), "CRIPPLE ")
                Case 4: ' No movement for 1 turn
                  TempSpecialA(X) = AddTag(TempSpecialA(X), "DRIFTING ")
                Case 5: ' No movement until repaired
                  TempSpecialA(X) = AddTag(TempSpecialA(X), "NOMOVE ")
                Case 6: ' Crippled
                  TempSpecialA(X) = AddTag(TempSpecialA(X), "NOMOVE ")
                  TempSpecialA(X) = AddTag(TempSpecialA(X), "CRIPPLE ")
                Case 7: ' drop shields
                  TempCurShieldA(X) = 0: Shields = 0
                  ShieldsPercent = 0
                Case 8: ' torps explode
                  CritDamageFlag = TempCurTorpA(X)
                Case 9: ' 5% crew casualties
                  If Not IsSolid(SpecialA(X)) Then
                    If HasCrew(TempSpecialA(X)) Then ' reduce crew level
                      ret = ret - 5
                      If ret <= 0 Then ret = 0: TempSpecialA(X) = AddTag(TempSpecialA(X), "CRIPPLE ")
                      TempSpecialA(X) = RemoveTag(TempSpecialA(X), "CREW", 1)
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "CREW " + Format(ret) + " ")
                    Else
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "CREW 95 ")
                    End If
                  End If
                Case 10: ' 10% crew casualties
                  If Not IsSolid(SpecialA(X)) Then
                    If HasCrew(TempSpecialA(X)) Then ' reduce crew level
                      ret = ret - 10
                      If ret <= 0 Then ret = 0: TempSpecialA(X) = AddTag(TempSpecialA(X), "CRIPPLE ")
                      TempSpecialA(X) = RemoveTag(TempSpecialA(X), "CREW", 1)
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "CREW " + Format(ret) + " ")
                    Else
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "CREW 90 ")
                    End If
                  End If
                Case 11: ' 25% crew casualties and all weapons offline until repaired
                  If Not IsSolid(SpecialA(X)) Then
                    If HasCrew(TempSpecialA(X)) Then ' reduce crew level
                      ret = ret - 25
                      If ret <= 0 Then ret = 0
                      TempSpecialA(X) = RemoveTag(TempSpecialA(X), "CREW", 1)
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "CREW " + Format(ret) + " ")
                    Else
                      TempSpecialA(X) = AddTag(TempSpecialA(X), "CREW 75 ")
                    End If
                  End If
                  TempSpecialA(X) = AddTag(TempSpecialA(X), "CRIPPLE ")
              End Select
              
              Hull = Hull - CritDamageFlag
              If Hull < 1 Then HullPercent = 100: Hull = 0 Else HullPercent = 100 - ((Hull * 100) / TempCurHullA(X))
              If CritDamageFlag = 100 Then Crits = 1: Exit For
            Next C
          End If ' end of crit calc code
          If Hull = 0 Then
            If IsMissile(SpecialA(X)) Then
              ShipCritStr = "Missile Destroyed."
            Else
              If IsBuilding(SpecialA(X)) Or IsOrbital(SpecialA(X)) Then
                ShipCritStr = "Base Destroyed."
              Else
                ShipCritStr = "Unit Destroyed."
              End If
            End If
          End If
        End If '   end of is_fighter code
    
        If Hull = 0 Then Shields = 0: TempCurBeamA(X) = 0: TempCurTorpA(X) = 0
        If Hull > 0 And BPAttackCritA(X) > 99 Then
          If IsBuilding(SpecialA(X)) Or IsOrbital(SpecialA(X)) Then
            ShipCritStr = "Base Captured."
          Else
            ShipCritStr = "Ship Captured."
          End If
          TempSpecialA(X) = AddTag(TempSpecialA(X), "CAPTURED ")
        End If
        AttCritStr(X) = CriticalStr
        If AttCritStr(X) > "" Then Print #3, "    " + AttCritStr(X)
        TempStr = "  Bm=" + Format(TempCurBeamA(X)) + " Sh=" + Format(Shields) + " Tp=" + Format(TempCurTorpA(X)) + " Hl=" + Format(Hull) + "  " + ShipCritStr
        TempCurDamA(X) = HullPercent
        If Left$(ShipCritStr, 1) > " " Then Print #4, AttRaceName + " " + AttShipStr(X) + " " + ShipCritStr
        TempCurShieldA(X) = Shields
        TempCurHullA(X) = Hull
    End Select
        

      End If ' ship was shot
ABORT_DAMAGE:
    Next E   ' walk through attack array
    If ShipHit = 1 Then Print #3, TempStr
  Next A
  Print #3, " "

' =============================================================================

  ' copy real values from temp fields so that combat will be applied
  BO_AttackTotal = 0
  For A = 1 To AttShipsLeft
    AttCritStr(A) = TempAttCritStr(A)
    CurBeamA(A) = TempCurBeamA(A)
    CurShieldA(A) = TempCurShieldA(A)
    CurTorpA(A) = TempCurTorpA(A)
    CurHullA(A) = TempCurHullA(A)
    HitsA(A) = TempHitsA(A)
    PenHitsA(A) = TempPenHitsA(A)
    CurDamA(A) = TempCurDamA(A)
    SpecialA(A) = TempSpecialA(A)
    If IsCaptured(SpecialA(A)) Or IsFled(SpecialA(A)) Or IsMissile(SpecialA(A)) Then
      'do not add to the hull count
    Else
      BO_AttackTotal = BO_AttackTotal + CurHullA(A)
    End If
  Next A

  BO_DefenseTotal = 0
  For A = 1 To DefShipsLeft
    DefCritStr(A) = TempDefCritStr(A)
    CurBeamB(A) = TempCurBeamB(A)
    CurShieldB(A) = TempCurShieldB(A)
    CurTorpB(A) = TempCurTorpB(A)
    CurHullB(A) = TempCurHullB(A)
    HitsB(A) = TempHitsB(A)
    PenHitsB(A) = TempPenHitsB(A)
    CurDamB(A) = TempCurDamB(A)
    SpecialB(A) = TempSpecialB(A)
    If IsCaptured(SpecialB(A)) Or IsFled(SpecialB(A)) Or IsMissile(SpecialB(A)) Then
      'do not add to the hull count
    Else
      BO_DefenseTotal = BO_DefenseTotal + CurHullB(A)
    End If
  Next A

'------------------------- remove cloaking -----------------------------

If CombatRound = 1 Then
  For A = 1 To AttShipsLeft
    SpecialA(A) = RemoveTag(SpecialA(A), "CLOAK", 0)
  Next A
  For A = 1 To DefShipsLeft
    SpecialB(A) = RemoveTag(SpecialB(A), "CLOAK", 0)
  Next A
End If

'-----------------------------------------------------------------------
  
  Print #3, "Damage Results:"
  Print #3, " "
  AttDamageStr = AttRaceName + " " + GroupName + " Damage:"
  Print #3, AttDamageStr
  If AttFleetStrength = 0 Then BO_AttackPercent = 0 Else BO_AttackPercent = (BO_AttackTotal * 100) / AttFleetStrength
  BO_Att = 100 - (CInt(BO_AttackPercent))
  TempStr = "Current Damage Level: " + Format(BO_Att) + "% (" + Format(BO_AttackTotal) + "/" + Format(AttFleetStrength) + ")"
  BO_AttackTotal = 0
  Print #3, TempStr
  
  For A = 1 To AttShipsLeft
    If CurHullA(A) > 0 Then ' the ship has not been destroyed
      If (CurShieldA(A) < MaxShieldA(A)) And GetRegenShieldVal(SpecialA(A)) And CurShieldA(A) > 0 Then
        CurShieldA(A) = CurShieldA(A) + GetRegenShieldVal(SpecialA(A))
        Print #3, AttShipStr(A); " - Shield regeneration detected."
        If CurShieldA(A) > MaxShieldA(A) Then CurShieldA(A) = MaxShieldA(A)
      End If
      If (CurHullA(A) < MaxHullA(A)) And GetRegenHullVal(SpecialA(A)) Then
        CurHullA(A) = CurHullA(A) + GetRegenHullVal(SpecialA(A))
        Print #3, AttShipStr(A) + " - Hull regeneration detected."
        If CurHullA(A) > MaxHullA(A) Then CurHullA(A) = MaxHullA(A)
      End If
    
      If IsNoMove(SpecialA(A)) Or IsCaptured(SpecialA(A)) Or IsCripple(SpecialA(A)) Or IsMissile(SpecialA(A)) Then
      Else
        If IsFlee(SpecialA(A)) Or AttBreakOff = 0 Then
          SpecialA(A) = AddTag(SpecialA(A), "FLED ")
          Print #3, "  "; AttShipStr(A) + " disengages."
          AttFledFlag = 1
        Else
          If HasBreak(SpecialA(A)) Then
            If ret = 0 Then
              SpecialA(A) = AddTag(SpecialA(A), "FLED ")
              Print #3, "  "; AttShipStr(A) + " disengages."
              AttFledFlag = 1
            ElseIf BO_Att >= ret Then
              SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
              Print #3, "  "; AttShipStr(A) + " is breaking off."
            End If
          Else
            If HasDamage(SpecialA(A)) Then
              If (CurHullA(A) / MaxHullA(A)) * 100 <= ret And ret < 100 Then
                SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
                Print #3, "  "; AttShipStr(A) + " is breaking off."
              Else
                If ret > 99 Then ' check the shields
                  If ret = 100 Then ' shields dropped?
                    If CurShieldA(A) = 0 Then
                      SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
                      Print #3, "  "; AttShipStr(A) + " is breaking off."
                    End If
                  Else ' shields dinged?
                    If (CurShieldA(A) / MaxShieldA(A)) * 100 <= (ret - 100) Then
                      SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
                      Print #3, "  "; AttShipStr(A) + " is breaking off."
                    End If
                  End If
                End If
              End If
            Else
              If BO_Att >= AttBreakOff Then
                SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
                Print #3, "  "; AttShipStr(A) + " is breaking off."
              Else
                If HasTime(SpecialA(A)) Then
                  If CombatRound >= ret Then
                    SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
                    Print #3, "  "; AttShipStr(A) + " is breaking off."
                  End If
                Else
                  If IsToothless(0, A) Then
                    SpecialA(A) = AddTag(SpecialA(A), "FLEE ")
                    Print #3, "  "; AttShipStr(A) + " is breaking off."
                  End If
                End If
              End If
            End If
          End If
        End If
      End If
    End If
  Next
  
  Print #3, " "
  DefDamageStr = DefRaceName + " " + GroupName + " Damage:"
  Print #3, DefDamageStr
  If DefFleetStrength = 0 Then BO_DefensePercent = 0 Else BO_DefensePercent = (BO_DefenseTotal * 100) / DefFleetStrength
  BO_Def = 100 - (CInt(BO_DefensePercent))
  TempStr = "Current Damage Level: " + Format(BO_Def) + "% (" + Format(BO_DefenseTotal) + "/" + Format(DefFleetStrength) + ")"
  BO_DefenseTotal = 0
  Print #3, TempStr
  
  For B = 1 To DefShipsLeft
    If CurHullB(B) > 0 Then ' the ship has not been destroyed
      If (CurShieldB(B) < MaxShieldB(B)) And GetRegenShieldVal(SpecialB(B)) And CurShieldB(B) > 0 Then
        CurShieldB(B) = CurShieldB(B) + GetRegenShieldVal(SpecialB(B))
        Print #3, DefShipStr(B); " - Shield regeneration detected."
        If CurShieldB(B) > MaxShieldB(B) Then CurShieldB(B) = MaxShieldB(B)
      End If
      If (CurHullB(B) < MaxHullB(B)) And GetRegenHullVal(SpecialB(B)) Then
        CurHullB(B) = CurHullB(B) + GetRegenHullVal(SpecialB(B))
        Print #3, DefShipStr(B) + " - Hull regeneration detected."
        If CurHullB(B) > MaxHullB(B) Then CurHullB(B) = MaxHullB(B)
      End If
    
      If IsNoMove(SpecialB(B)) Or IsCaptured(SpecialB(B)) Or IsCripple(SpecialB(B)) Or IsMissile(SpecialB(B)) Then
      Else
        If IsFlee(SpecialB(B)) Or DefBreakOff = 0 Then
          SpecialB(B) = AddTag(SpecialB(B), "FLED ")
          Print #3, "  "; DefShipStr(B) + " disengages."
          DefFledFlag = 1
        Else
          If HasBreak(SpecialB(B)) Then
            If ret = 0 Then
              SpecialB(B) = AddTag(SpecialB(B), "FLED ")
              Print #3, "  "; DefShipStr(B) + " disengages."
              DefFledFlag = 1
            ElseIf BO_Def >= ret Then
              SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
              Print #3, "  "; DefShipStr(B) + " is breaking off."
            End If
          Else
            If HasDamage(SpecialB(B)) Then
              If (CurHullB(B) / MaxHullB(B)) * 100 <= ret And ret < 100 Then
                SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
                Print #3, "  "; DefShipStr(B) + " is breaking off."
              Else
                If ret > 99 Then ' check the shields
                  If ret = 100 Then ' shields dropped?
                    If CurShieldB(B) = 0 Then
                      SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
                      Print #3, "  "; DefShipStr(B) + " is breaking off."
                    End If
                  Else ' shields dinged?
                    If (CurShieldB(B) / MaxShieldB(B)) * 100 <= (ret - 100) Then
                      SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
                      Print #3, "  "; DefShipStr(B) + " is breaking off."
                    End If
                  End If
                End If
              End If
            Else
              If BO_Def >= DefBreakOff Then
                SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
                Print #3, "  "; DefShipStr(B) + " is breaking off."
              Else
                If HasTime(SpecialB(B)) Then
                  If CombatRound >= ret Then
                    SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
                    Print #3, "  "; DefShipStr(B) + " is breaking off."
                  End If
                Else
                  If IsToothless(1, B) Then
                    SpecialB(B) = AddTag(SpecialB(B), "FLEE ")
                    Print #3, "  "; DefShipStr(B) + " is breaking off."
                  End If
                End If
              End If
            End If
          End If
        End If
      End If
    End If
  Next
  
  AttGone = 1
  DefGone = 1
  If AttShipsLeft > 0 Then
    For A = 1 To AttShipsLeft
      ' check for FLED, CRIPPLE, CAPTURED, beam+torp of zero
      If IsFled(SpecialA(A)) Or IsCaptured(SpecialA(A)) Or IsCripple(SpecialA(A)) Or IsMissile(SpecialA(A)) Then
        ' Leave AttGone alone
      Else
        ' ship is functional, but it it combat worthy?
        tmp = 1: If HasShots(SpecialA(A)) Then If ret = 0 Then tmp = 0
        If tmp = 1 And CurBeamA(A) > 0 Then AttGone = 0
        
        tmp = 1: If HasAmmo(SpecialA(A)) Then If ret = 0 Then tmp = 0
        If tmp = 1 And CurTorpA(A) > 0 Then AttGone = 0
      End If
    Next
  End If
  If DefShipsLeft > 0 Then
    For B = 1 To DefShipsLeft
      ' check for FLED, CRIPPLE, CAPTURED, beam+torp of zero
      If IsFled(SpecialB(B)) Or IsCaptured(SpecialB(B)) Or IsCripple(SpecialB(B)) Or IsMissile(SpecialB(B)) Then
        ' Leave AttGone alone
      Else
        ' ship is functional, but it it combat worthy?
        tmp = 1: If HasShots(SpecialB(B)) Then If ret = 0 Then tmp = 0
        If tmp = 1 And CurBeamB(B) > 0 Then DefGone = 0
        
        tmp = 1: If HasAmmo(SpecialB(B)) Then If ret = 0 Then tmp = 0
        If tmp = 1 And CurTorpB(B) > 0 Then DefGone = 0
      End If
    Next
  End If
  
  If CombatRound = 1 Then 'remove any SURPRISE tags
    If AttGone = 0 Then
      For A = 1 To AttShipsLeft
        If IsSurprise(SpecialA(A)) Then
          SpecialA(A) = RemoveTag(SpecialA(A), "SURPRISE", 0)
        End If
      Next
    End If
    If DefGone = 0 Then
      For B = 1 To DefShipsLeft
        If IsSurprise(SpecialB(B)) Then
          SpecialB(B) = RemoveTag(SpecialB(B), "SURPRISE", 0)
        End If
      Next
    End If
  End If
  
  ActiveFile = "unknown3"
  WriteTempFiles
  ActiveFile = "unknown4"
  
  If AttGone = 1 Or CombatRound = 100 Then
    If AttShipsLeft = 0 Then
      TempStr = "Attacking " + GroupName + " gone."
    Else
      TempStr = "Attacking " + GroupName + " breaks off action or surrenders."
    End If
    RetreatFlag = 1
    Print #3, " "
    Print #3, TempStr
    ReadTempFiles
    Close #3
    AttackLoop = 0
  Else
    If DefGone = 1 Then
      If DefShipsLeft = 0 Then
        TempStr = "Defending " + GroupName + " gone."
      Else
        TempStr = "Defending " + GroupName + " breaks off action or surrenders."
      End If
      RetreatFlag = 2
      Print #3, " "
      Print #3, TempStr
      ReadTempFiles
      Close #3
      AttackLoop = 0
    End If
  End If
' check for temporary stop
If Form1.ONE_TURN_FLAG = True Then
  A = MsgBox("One turn completed.  Continue execution?", vbYesNo, "ONE TURN MODE")
  If A = vbNo Then AttackLoop = 0
End If
' check for end of Battle_sim
Loop While AttackLoop = 1

Close #4  ' close the damage_report file

ActiveFile = "sort_dam.bat"
Shell "sort_dam.bat", vbMinimizedNoFocus
ActiveFile = "unknown6"

A = MsgBox("Run more battles?", vbYesNo, "Battle completed.")
If A = vbYes Then Form1.Show: Form1.OK_FLAG = False: GoTo GET_FILE_NAMES
End

RUN_TIME_ERROR:
  Screen.MousePointer = 0
  If Err = 53 Then
    A = MsgBox("Error: I couldn't find a required file.  [" & ActiveFile & "]", vbOKCancel + vbExclamation)
    If A = 2 Then End ' cancel application
    Resume GET_FILE_NAMES
  End If
  If Err = 62 Then
    If DefShipsRead = 0 Then
        A = MsgBox("Error: In [" & AttFile & "] it says there are " & AttShipsTotal & " ships.  I could only read " & AttShipsRead & " lines.", vbOKCancel + vbExclamation)
      Else
        A = MsgBox("Error: In [" & DefFile & "] it says there are " & DefShipsTotal & " ships.  I could only read " & DefShipsRead & " lines.", vbOKCancel + vbExclamation)
    End If
    End
  End If
  MsgBox "Error:" & Err & " " & Error$
  Resume Next
  Exit Sub

End Sub


Sub ReadTempFiles()

On Error GoTo ignore_errors

Dim WriteFlag As Long  ' a local "permission to write" flag
Dim t_ShipStr, t_Special As String
Dim t_MaxBeam, t_CurBeam, t_MaxShield, t_CurShield, t_MaxTorp, t_CurTorp, t_MaxHull, t_CurHull, t_CurDam, t_Status, t_Ammo As Long
Dim PrintFlag As Long, err_cnt As Long
err_cnt = 0

Open TempAFile For Input As #7
Input #7, AttRaceName, AttFleetName, AttBreakOff, AttShipsTotal, AttFleetStrength, AttShipsLeft, AttTargetBonus, AttTargetPriority, AttReserve
For X = 1 To AttShipsLeft
  Input #7, AttShipStr(X), MaxBeamA(X), CurBeamA(X), MaxShieldA(X), CurShieldA(X), MaxTorpA(X), CurTorpA(X), MaxHullA(X), CurHullA(X), CurDamA(X), StatusA(X), AmmoA(X), SpecialA(X)
Next X
Close #7

Open TempBFile For Input As #7
Input #7, DefRaceName, DefFleetName, DefBreakOff, DefShipsTotal, DefFleetStrength, DefShipsLeft, DefTargetBonus, DefTargetPriority, DefReserve
For X = 1 To DefShipsLeft
  Input #7, DefShipStr(X), MaxBeamB(X), CurBeamB(X), MaxShieldB(X), CurShieldB(X), MaxTorpB(X), CurTorpB(X), MaxHullB(X), CurHullB(X), CurDamB(X), StatusB(X), AmmoB(X), SpecialB(X)
Next X
Close #7

Print #3, " "
Print #3, "Surviving " + UnitName + ":"
Print #3, " "
        
AttRetreatStr = ".": DefRetreatStr = "."
        
If RetreatFlag = 1 Then
  If AttShipsLeft = 0 Then
    AttRetreatStr = " is gone."
  Else
    AttRetreatStr = " retreats."
  End If
  If AttFledFlag = 1 Then AttRetreatStr = AttRetreatStr + " (Some units have fled.)"
End If
If RetreatFlag = 2 Then
  If DefShipsLeft = 0 Then
    DefRetreatStr = " is gone."
  Else
    DefRetreatStr = " retreats."
  End If
  If DefFledFlag = 1 Then DefRetreatStr = DefRetreatStr + " (Some units have fled.)"
End If
        
TempStr = AttRaceName + ", " + AttFleetName + " " + GroupName + AttRetreatStr
        
Print #3, TempStr
Print #3, " "
        
PrintFlag = 0
For X = 1 To AttShipsLeft
   CurBeamA(X) = MaxBeamA(X)
   CurTorpA(X) = MaxTorpA(X)
   WriteFlag = 1
   If CurHullA(X) = 0 Then WriteFlag = 0
   If RetreatFlag = 1 And (IsNoMove(SpecialA(X)) Or IsCripple(SpecialA(X))) Then WriteFlag = 0
   If WriteFlag = 1 Then
      TempStr = AttShipStr(X) + " Bm=" + Format(CurBeamA(X)) + " Sh=" + Format(CurShieldA(X)) + " Tp=" + Format(CurTorpA(X)) + " Hl=" + Format(CurHullA(X)) + " Hull Damage Level: " + Format(CInt(CurDamA(X))) + "%"
      Print #3, TempStr
      PrintFlag = 1
   End If
Next X
If PrintFlag = 1 Then Print #3, " "

If AttFledFlag = 1 Then
  Open "fled_att.csv" For Input As #8
  Do While Not EOF(8)
    Input #8, t_ShipStr, t_MaxBeam, t_CurBeam, t_MaxShield, t_CurShield, t_MaxTorp, t_CurTorp, t_MaxHull, t_CurHull, t_CurDam, t_Status, t_Ammo, t_Special
    TempStr = "FLED: " + t_ShipStr + " Bm=" + Format(t_MaxBeam) + " Sh=" + Format(t_CurShield) + " Tp=" + Format(t_MaxTorp) + " Hl=" + Format(t_CurHull) + " Hull Damage Level: " + Format(CInt(t_CurDam)) + "%"
    Print #3, TempStr
  Loop
  Close #8
  Print #3, " "
End If

TempStr = DefRaceName + ", " + DefFleetName + " " + GroupName + DefRetreatStr
        
Print #3, TempStr
Print #3, " "
        
PrintFlag = 0
For X = 1 To DefShipsLeft
   CurBeamB(X) = MaxBeamB(X)
   CurTorpB(X) = MaxTorpB(X)
   WriteFlag = 1
   If CurHullB(X) = 0 Then WriteFlag = 0
   If RetreatFlag = 2 And (IsNoMove(SpecialB(X)) Or IsCripple(SpecialB(X))) Then WriteFlag = 0
   If WriteFlag = 1 Then
      TempStr = DefShipStr(X) + " Bm=" + Format(CurBeamB(X)) + " Sh=" + Format(CurShieldB(X)) + " Tp=" + Format(CurTorpB(X)) + " Hl=" + Format(CurHullB(X)) + " Hull Damage Level: " + Format(CInt(CurDamB(X))) + "%"
      Print #3, TempStr
      PrintFlag = 1
   End If
Next X
If PrintFlag = 1 Then Print #3, " "
        
If DefFledFlag = 1 Then
  Open "fled_def.csv" For Input As #8
  Do While Not EOF(8)
    Input #8, t_ShipStr, t_MaxBeam, t_CurBeam, t_MaxShield, t_CurShield, t_MaxTorp, t_CurTorp, t_MaxHull, t_CurHull, t_CurDam, t_Status, t_Ammo, t_Special
    TempStr = "FLED: " + t_ShipStr + " Bm=" + Format(t_MaxBeam) + " Sh=" + Format(t_CurShield) + " Tp=" + Format(t_MaxTorp) + " Hl=" + Format(t_CurHull) + " Hull Damage Level: " + Format(CInt(t_CurDam)) + "%"
    Print #3, TempStr
  Loop
  Close #8
  Print #3, " "
End If
        
If RetreatFlag = 1 Then RaceStr = AttRaceName
If RetreatFlag = 2 Then RaceStr = DefRaceName
        
TempStr = "The following " + RaceStr + " " + UnitName + " were captured or unable to retreat."
Print #3, TempStr
        
A = 0
If RetreatFlag = 1 Then
   For X = 1 To AttShipsLeft
      If (IsNoMove(SpecialA(X)) Or IsCripple(SpecialA(X)) Or IsCaptured(SpecialA(X))) And CurHullA(X) > 0 Then
         TempStr = AttShipStr(X) + " Bm=" + Format(CurBeamA(X)) + " Sh=" + Format(CurShieldA(X)) + " Tp=" + Format(CurTorpA(X)) + " Hl=" + Format(CurHullA(X)) + " Hull Damage Level: " + Format(CInt(CurDamA(X))) + "%"
         Print #3, TempStr
         A = 1
      End If
   Next X
End If

If RetreatFlag = 2 Then
   For X = 1 To DefShipsLeft
      If (IsNoMove(SpecialB(X)) Or IsCripple(SpecialB(X)) Or IsCaptured(SpecialB(X))) And CurHullB(X) > 0 Then
         TempStr = DefShipStr(X) + " Bm=" + Format(CurBeamB(X)) + " Sh=" + Format(CurShieldB(X)) + " Tp=" + Format(CurTorpB(X)) + " Hl=" + Format(CurHullB(X)) + " Hull Damage Level: " + Format(CInt(CurDamB(X))) + "%"
         Print #3, TempStr
         A = 1
      End If
   Next X
End If

If A = 0 Then Print #3, "  None."
Exit Sub

ignore_errors:
  Err.Clear
  err_cnt = err_cnt + 1
  If err_cnt < 20 Then Resume Next
End Sub

Sub WriteTempFiles()
  Dim old_AttShipsLeft As Long
  Dim old_DefShipsLeft As Long

  old_AttShipsLeft = AttShipsLeft
  old_DefShipsLeft = DefShipsLeft

  ' Write the Working and Temp files
    
  ' Find Dead and Fled Ships
  Open "fled_att.csv" For Append As #8
  For X = 1 To old_AttShipsLeft
    If CurHullA(X) < 1 Or (IsMissile(SpecialA(X)) And CombatRound > 0) Then
      AttShipsLeft = AttShipsLeft - 1
    Else
      If Not IsCaptured(SpecialA(X)) And Not IsCripple(SpecialA(X)) Then
        If IsFled(SpecialA(X)) Then
          AttShipsLeft = AttShipsLeft - 1
          Write #8, AttShipStr(X), MaxBeamA(X), CurBeamA(X), MaxShieldA(X), CurShieldA(X), MaxTorpA(X), CurTorpA(X), MaxHullA(X), CurHullA(X), CurDamA(X), StatusA(X), AmmoA(X), SpecialA(X)
        End If
      End If
    End If
  Next X
  Close #8

  If Form1.Check1.value Then Write #3, "--- DEBUG LOG ---"
  Open TempAFile For Output As #7
  Write #7, AttRaceName, AttFleetName, AttBreakOff, AttShipsTotal, AttFleetStrength, AttShipsLeft, AttTargetBonus, AttTargetPriority, AttReserve
  If Form1.Check1.value Then Write #3, AttRaceName, AttFleetName, AttBreakOff, AttShipsTotal, AttFleetStrength, AttShipsLeft, AttTargetBonus, AttTargetPriority, AttReserve
  If AttShipsLeft > 0 Then
    For X = 1 To old_AttShipsLeft
      If CurHullA(X) < 1 Or IsFled(SpecialA(X)) Or (IsMissile(SpecialA(X)) And CombatRound > 0) Then
        ' skip this ship
      Else
        Write #7, AttShipStr(X), MaxBeamA(X), CurBeamA(X), MaxShieldA(X), CurShieldA(X), MaxTorpA(X), CurTorpA(X), MaxHullA(X), CurHullA(X), CurDamA(X), StatusA(X), AmmoA(X), SpecialA(X)
        If Form1.Check1.value Then Write #3, AttShipStr(X), MaxBeamA(X), CurBeamA(X), MaxShieldA(X), CurShieldA(X), MaxTorpA(X), CurTorpA(X), MaxHullA(X), CurHullA(X), CurDamA(X), StatusA(X), AmmoA(X), SpecialA(X)
      End If
    Next X
  End If
  Close #7
          
  ' Find Dead and Fled Ships
  Open "fled_def.csv" For Append As #8
  For X = 1 To old_DefShipsLeft
    If CurHullB(X) = 0 Or (IsMissile(SpecialB(X)) And CombatRound > 0) Then
      DefShipsLeft = DefShipsLeft - 1
    Else
      If Not IsCaptured(SpecialB(X)) And Not IsCripple(SpecialB(X)) Then
        If IsFled(SpecialB(X)) Then
          DefShipsLeft = DefShipsLeft - 1
          Write #8, DefShipStr(X), MaxBeamB(X), CurBeamB(X), MaxShieldB(X), CurShieldB(X), MaxTorpB(X), CurTorpB(X), MaxHullB(X), CurHullB(X), CurDamB(X), StatusB(X), AmmoB(X), SpecialB(X)
        End If
      End If
    End If
  Next X
  Close #8

  Open TempBFile For Output As #7
  Write #7, DefRaceName, DefFleetName, DefBreakOff, DefShipsTotal, DefFleetStrength, DefShipsLeft, DefTargetBonus, DefTargetPriority, DefReserve
  If Form1.Check1.value Then Write #3, DefRaceName, DefFleetName, DefBreakOff, DefShipsTotal, DefFleetStrength, DefShipsLeft, DefTargetBonus, DefTargetPriority, DefReserve
  If DefShipsLeft > 0 Then
    For X = 1 To old_DefShipsLeft
      If CurHullB(X) < 1 Or IsFled(SpecialB(X)) Or (IsMissile(SpecialB(X)) And CombatRound > 0) Then
        ' skip this ship
      Else
        Write #7, DefShipStr(X), MaxBeamB(X), CurBeamB(X), MaxShieldB(X), CurShieldB(X), MaxTorpB(X), CurTorpB(X), MaxHullB(X), CurHullB(X), CurDamB(X), StatusB(X), AmmoB(X), SpecialB(X)
        If Form1.Check1.value Then Write #3, DefShipStr(X), MaxBeamB(X), CurBeamB(X), MaxShieldB(X), CurShieldB(X), MaxTorpB(X), CurTorpB(X), MaxHullB(X), CurHullB(X), CurDamB(X), StatusB(X), AmmoB(X), SpecialB(X)
      End If
    Next X
  End If
  Close #7
  If Form1.Check1.value Then Write #3, "--- DEBUG LOG ---"

  ' Determining who can fire should occur in the attack phase

End Sub

Function GetStatusString(val As Long) As String
  Dim status As String
  
  status = " "
  If (val And valFighter) = valFighter Then status = status + "FIGHTER "
  If (val And valReserve) = valReserve Then status = status + "RESERVE 10 "
  If (val And valSurprise) = valSurprise Then status = status + "SURPRISE "
  If (val And valCloaked) = valCloaked Then status = status + "CLOAK "
  If (val And valMissile) = valMissile Then status = status + "LONG 2 "
  If (val And valMovement) = valMovement Then status = status + "NOMOVE "
  If (val And valNoBeam) = valNoBeam Then status = status + "NOBEAM "
  If (val And valNoTorp) = valNoTorp Then status = status + "NOTORP "
  If (val And valCripple) = valCripple Then status = status + "CRIPPLE "
  If (val And valFlak) = valFlak Then status = status + "FLAK 3 "
  If (val And valMulti) = valMulti Then status = status + "MULTI 3 3 "
  If (val And valAmmo) = valAmmo Then status = status + "AMMO 2 "
  If (val And valCracker) = valCracker Then status = status + "CRACK 3 "
  If (val And valStasis) = valStasis Then status = status + "STASIS "
  If (val And valPen) = valPen Then status = status + "PEN 3 "
  If (val And valSuicide) = valSuicide Then status = status + "SUICIDE "
  
  GetStatusString = status
End Function

Function GetBPAttVal(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "BP ")
  If start > 0 Then
    GetBPAttVal = val(Mid$(data_str, start + 3, 2))
  Else
    GetBPAttVal = 0
  End If
End Function

Function GetBPDefVal(Hull As Long, data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "BP ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    start = InStr(start, data_str, " ")                   ' jump to the second space
    GetBPDefVal = val(Mid$(data_str, start, 3)) + Hull
  Else
    GetBPDefVal = Hull
  End If
End Function

Function GetCrit(data_str As String) As Long
  Dim start As Long
  
  ' default to 5 (normal)
  GetCrit = 5
  start = InStr(data_str, "CRIT ")
  If start > 0 Then
    GetCrit = val(Mid$(data_str, start + 5, 2))
  End If
End Function

Function GetMultiBeamVal(data_str As String) As Long
  Dim start, end1, end2 As Long
  
  start = InStr(data_str, "MULTI ")
  end1 = InStr(start + 1, data_str, " ")                ' jump to the second space
  end2 = InStr(end1 + 1, data_str, " ")                 ' jump to the second space
  If end2 = 0 Then end2 = Len(data_str)

  If start > 0 Then
    GetMultiBeamVal = val(Mid$(data_str, start + 6, end2 - end1))
  Else
    GetMultiBeamVal = 0
  End If
End Function

Function GetMultiTorpVal(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "MULTI ")
  If start > 0 Then
    start = InStr(start, data_str, " ")                   ' jump to the first space
    start = InStr(start + 1, data_str, " ")               ' jump to the second space
    GetMultiTorpVal = val(Mid$(data_str, start, 3))
  Else
    GetMultiTorpVal = 0
  End If
End Function

Function GetMultiValWT(data_str As String) As Long
  Dim start, start2 As Long
  
  start = InStr(data_str, "multi ")
  If start > 0 Then
    start = InStr(start, data_str, " ")      ' jump to the first space
    start2 = InStr(start + 1, data_str, " ") ' jump to the second space
    GetMultiValWT = val(Mid$(data_str, start, start2 - start))
  Else
    GetMultiValWT = 0
  End If
End Function

Function GetRegenShieldVal(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "REGEN ")
  If start > 0 Then
    GetRegenShieldVal = val(Mid$(data_str, start + 8, 4))
  Else
    GetRegenShieldVal = 0
  End If
End Function

Function GetRegenHullVal(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "REGEN ")
  If start > 0 Then
    start = InStr(start, data_str, " ")                   ' jump to the second space
    GetRegenHullVal = val(Mid$(data_str, start, 2))
  Else
    GetRegenHullVal = 0
  End If
End Function

Function GetROFDelay(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "ROF ")
  If start > 0 Then
    start = InStr(start + 4, data_str, " ")                 ' jump to the second space
    GetROFDelay = val(Mid$(data_str, start, 2))
  Else
    GetROFDelay = 0
  End If
End Function

Function GetROFRate(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "ROF ")
  If start > 0 Then
    GetROFRate = val(Mid$(data_str, start, 2))
  Else
    GetROFRate = 0
  End If
End Function

Function GetROFDelayWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "rof ")
  If start > 0 Then
    start = InStr(start + 4, data_str, " ")               ' jump to the second space
    GetROFDelayWT = val(Mid$(data_str, start, 2))
  Else
    GetROFDelayWT = 0
  End If
End Function

Function GetROFRateWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "rof ")
  If start > 0 Then
    GetROFRateWT = val(Mid$(data_str, start + 4, 2))
  Else
    GetROFRateWT = 0
  End If
End Function

Function IsBio(data_str As String) As Long
  If InStr(data_str, "BIO") > 0 Then IsBio = True Else IsBio = False
End Function

Function IsBuilding(data_str As String) As Long
  If InStr(data_str, "BUILDING") > 0 Then IsBuilding = True Else IsBuilding = False
End Function

Function IsCaptured(data_str As String) As Long
  If InStr(data_str, "CAPTURED") > 0 Then IsCaptured = True Else IsCaptured = False
End Function

Function IsCarrier(data_str As String) As Long
  If InStr(data_str, "CARRIER") > 0 Then IsCarrier = True Else IsCarrier = False
End Function

Function IsCloak(data_str As String) As Long
  If InStr(data_str, "CLOAK") > 0 Then IsCloak = True Else IsCloak = False
End Function

Function IsCripple(data_str As String) As Long
  If InStr(data_str, "CRIPPLE") > 0 Then IsCripple = True Else IsCripple = False
End Function

Function IsDrifting(data_str As String) As Long
  If InStr(data_str, "DRIFTING") > 0 Then IsDrifting = True Else IsDrifting = False
End Function

Function IsFighter(data_str As String) As Long
  If InStr(data_str, "FIGHTER") > 0 Then IsFighter = True Else IsFighter = False
End Function

Function IsFled(data_str As String) As Long
  If InStr(data_str, "FLED") > 0 Then IsFled = True Else IsFled = False
End Function

Function IsFlee(data_str As String) As Long
  If InStr(data_str, "FLEE") > 0 Then IsFlee = True Else IsFlee = False
End Function

Function IsGlobal(data_str As String) As Long
  IsGlobal = False
  If InStr(data_str, "GLOBAL") > 0 Then IsGlobal = True
  If InStr(data_str, "global") > 0 Then IsGlobal = True
End Function

Function IsGround(data_str As String) As Long
  If InStr(data_str, "GROUND") > 0 Then IsGround = True Else IsGround = False
End Function

Function IsMine(data_str As String) As Long
  If InStr(data_str, "MINE") > 0 Then IsMine = True Else IsMine = False
End Function

Function IsMissile(data_str As String) As Long
  If InStr(data_str, "MSL") > 0 Then IsMissile = True Else IsMissile = False
End Function

Function IsOffline(data_str As String) As Long
  If InStr(data_str, "offline") > 0 Then IsOffline = True Else IsOffline = False
End Function

Function IsNoBeam(data_str As String) As Long
  If InStr(data_str, "NOBEAM") > 0 Then IsNoBeam = True Else IsNoBeam = False
End Function

Function IsNoMove(data_str As String) As Long
  If InStr(data_str, "NOMOVE") > 0 Then IsNoMove = True Else IsNoMove = False
End Function

Function IsNoTorp(data_str As String) As Long
  If InStr(data_str, "NOTORP") > 0 Then IsNoTorp = True Else IsNoTorp = False
End Function

Function IsOrbital(data_str As String) As Long
  If InStr(data_str, "ORBITAL") > 0 Then IsOrbital = True Else IsOrbital = False
End Function

Function IsSolid(data_str As String) As Long
  If InStr(data_str, "SOLID") > 0 Then IsSolid = True Else IsSolid = False
End Function

Function IsSplit(data_str As String) As Long
  If InStr(data_str, "SPLIT") > 0 Then IsSplit = True Else IsSplit = False
End Function

Function IsStasis(data_str As String) As Long
  If InStr(data_str, "STASIS") > 0 Then IsStasis = True Else IsStasis = False
End Function

Function IsSuicide(data_str As String) As Long
  If InStr(data_str, "SUICIDE") > 0 Then IsSuicide = True Else IsSuicide = False
End Function

Function IsSurprise(data_str As String) As Long
  If InStr(data_str, "SURPRISE") > 0 Then IsSurprise = True Else IsSurprise = False
End Function

Function IsToothless(enemy As Long, ID As Long) As Long
  Dim start As Long, start1 As Long, old_start As Long
  Dim temp_str As String, test_str As String
  
  IsToothless = True
  
  If enemy = 0 Then ' attacker logic
    If HasBatteries(SpecialA(ID)) Then
      ' prove we're not dead
      temp_str = SpecialA(ID)
      
      old_start = InStr(1, temp_str, "[")
      Do While old_start > 0
        start = InStr(old_start, temp_str, "[")
        start1 = InStr(start, temp_str, "]")
        old_start = InStr(start1, temp_str, "[")
        test_str = Mid$(temp_str, start, (start1 - start) + 1)

        'If Not IsOffline(test_str) Then
          If InStr(test_str, "bp") = 0 Then  ' boarding parties do not count
            If HasAmmoWT(test_str) Then
              If ret > 0 Then IsToothless = False
            Else
              IsToothless = False
            End If
          End If
        'End If
      Loop
    Else
      If HasShots(SpecialA(ID)) Then If ret = 0 Then CurBeamA(ID) = 0
      If HasAmmo(SpecialA(ID)) Then If ret = 0 Then CurTorpA(ID) = 0
      If CurBeamA(ID) > 0 Or CurTorpA(ID) > 0 Then
        IsToothless = False
      End If
    End If
  Else ' is defender logic
    If HasBatteries(SpecialB(ID)) Then
      ' prove we're not dead
      temp_str = SpecialB(ID)
      
      old_start = InStr(1, temp_str, "[")
      Do While old_start > 0
        start = InStr(old_start, temp_str, "[")
        start1 = InStr(start, temp_str, "]")
        old_start = InStr(start1, temp_str, "[")
        test_str = Mid$(temp_str, start, (start1 - start) + 1)

        'If Not IsOffline(test_str) Then
          If InStr(test_str, "bp") = 0 Then  ' boarding parties do not count
            If HasAmmoWT(test_str) Then
              If ret > 0 Then IsToothless = False
            Else
              IsToothless = False
            End If
          End If
        'End If
      Loop
    Else
      If HasShots(SpecialB(ID)) Then If ret = 0 Then CurBeamB(ID) = 0
      If HasAmmo(SpecialB(ID)) Then If ret = 0 Then CurTorpB(ID) = 0
      If CurBeamB(ID) > 0 Or CurTorpB(ID) > 0 Then
        IsToothless = False
      End If
    End If
  End If
End Function

Function IsVehicle(data_str As String) As Long
  If InStr(data_str, "VEHICLE") > 0 Then IsVehicle = True Else IsVehicle = False
End Function

Function IsVolatile(data_str As String) As Long
  If InStr(data_str, "VOLATILE") > 0 Then IsVolatile = True Else IsVolatile = False
End Function

Function HasAmmo(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "AMMO ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasAmmo = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasAmmo = False: ret = 0
  End If
End Function

Function HasAmmoWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "ammo ")
  If start = 0 Then start = InStr(data_str, "shots ") ' second chance conversion
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasAmmoWT = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasAmmoWT = False: ret = 0
  End If
End Function

Function HasArmor(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "AR ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasArmor = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasArmor = False: ret = 0
  End If
End Function

Function HasArtillery(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "ARTILLERY ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasArtillery = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasArtillery = False: ret = 0
  End If
End Function

Function HasArtilleryWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "artillery")
  If start > 0 Then
    HasArtilleryWT = True: ret = 1
  Else
    HasArtilleryWT = False: ret = 0
  End If
End Function

Function HasBatteries(data_str As String) As Long
  Dim start, A As Long
  
  start = InStr(data_str, "[")
  If start > 0 Then
    HasBatteries = True
    ret = 0
    For A = 1 To Len(data_str)
      If Mid$(data_str, A, 1) = "[" Then ret = ret + 1
    Next A
  Else
    HasBatteries = False: ret = 0
  End If
End Function

Function HasBPWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "bp")
  If start > 0 Then
    HasBPWT = True: ret = 1
  Else
    HasBPWT = False: ret = 0
  End If
End Function

Function HasBreak(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "BREAK ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasBreak = True
    ret = val(Mid$(data_str, start + 1, 3))
  Else
    HasBreak = False: ret = 0
  End If
End Function

Function HasCrack(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "CRACK ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasCrack = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasCrack = False: ret = 0
  End If
End Function

Function HasCrackWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "crack")
  If start > 0 Then
    HasCrackWT = True: ret = 1
  Else
    HasCrackWT = False: ret = 0
  End If
End Function

Function HasCrew(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "CREW ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasCrew = True
    ret = val(Mid$(data_str, start + 1, 3))
  Else
    HasCrew = False: ret = 0
  End If
End Function

Function HasDamage(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "DAMAGE ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasDamage = True
    ret = val(Mid$(data_str, start + 1, 3))
  Else
    HasDamage = False: ret = 0
  End If
End Function

Function HasDataLink(data_str As String) As Long
  Dim start As Long
  Dim value As String
  
  start = InStr(data_str, "DL ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    value = Mid$(data_str, start + 1, 1)
    HasDataLink = False: ret = 0
    If value >= "A" And value <= "Z" Then HasDataLink = True: ret = Asc(value) - 64
    If value >= "a" And value <= "z" Then HasDataLink = True: ret = Asc(value) - 70
  Else
    HasDataLink = False: ret = 0
  End If
End Function

Function HasDataLinkWT(data_str As String) As Long
  Dim start As Long
  Dim value As String
  
  start = InStr(data_str, "dl ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    value = Mid$(data_str, start + 1, 1)
    HasDataLinkWT = False: ret = 0
    If value >= "A" And value <= "Z" Then HasDataLinkWT = True: ret = Asc(value) - 64
    If value >= "a" And value <= "z" Then HasDataLinkWT = True: ret = Asc(value) - 70
  Else
    HasDataLinkWT = False: ret = 0
  End If
End Function

Function HasDelay(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "DELAY ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasDelay = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasDelay = False: ret = 0
  End If
End Function

Function HasDisable(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "DIS ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasDisable = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasDisable = False: ret = 0
  End If
End Function

Function HasDisableWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "dis")
  If start > 0 Then
    HasDisableWT = True: ret = 1
  Else
    HasDisableWT = False: ret = 0
  End If
End Function

Function HasDefense(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "DEFENSE ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasDefense = True
    ret = val(Mid$(data_str, start + 1, 3))
  Else
    HasDefense = False: ret = 0
  End If
End Function

Function HasFlak(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "FLAK ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasFlak = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasFlak = False: ret = 0
  End If
End Function

Function HasFlakWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "flak")
  If start > 0 Then
    HasFlakWT = True: ret = 1
  Else
    HasFlakWT = False: ret = 0
  End If
End Function

Function HasFlicker(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "FLICKER ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasFlicker = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasFlicker = False: ret = 0
  End If
End Function

Function HasHeat(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "HEAT ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasHeat = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasHeat = False: ret = 0
  End If
End Function

Function HasHeatWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "heat")
  If start > 0 Then
    HasHeatWT = True: ret = 1
  Else
    HasHeatWT = False: ret = 0
  End If
End Function

Function HasHull(data_str As String) As Long
  Dim start As Long, mark1 As Long, mark2 As Long, mark3 As Long
  
  HasHull = False: ret = 0: Scope = 0
  If Left$(data_str, 1) = "[" Then     ' a weapon tag
    start = InStr(data_str, "hull ")
    If start > 0 Then
      HasHull = True
      mark1 = InStr(start, data_str, " ")
      mark2 = InStr(mark1 + 1, data_str, " ")
      mark3 = InStr(mark2 + 1, data_str, " ")
      If mark3 = 0 Then mark3 = Len(data_str)
      ret = val(Mid$(data_str, mark1 + 1, mark2 - mark1))
      Scope = val(Mid$(data_str, mark2 + 1, mark3 - mark2))
    End If
  Else
    start = InStr(data_str, "HULL ")
    If start > 0 Then
      HasHull = True
      mark1 = InStr(start, data_str, " ")
      mark2 = InStr(mark1 + 1, data_str, " ")
      mark3 = InStr(mark2 + 1, data_str, " ")
      If mark3 = 0 Then mark3 = Len(data_str)
      ret = val(Mid$(data_str, mark1 + 1, mark2 - mark1))
      Scope = val(Mid$(data_str, mark2 + 1, mark3 - mark2))
    End If
  End If
End Function

Function HasLong(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "LONG ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasLong = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasLong = False: ret = 0
  End If
End Function

Function HasLongWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "long")
  If start > 0 Then
    HasLongWT = True: ret = 1
  Else
    HasLongWT = False: ret = 0
  End If
End Function

Function HasMeson(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "MESON ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasMeson = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasMeson = False: ret = 0
  End If
End Function

Function HasMesonWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "meson")
  If start > 0 Then
    HasMesonWT = True: ret = 1
  Else
    HasMesonWT = False: ret = 0
  End If
End Function

Function HasMissile(data_str As String) As Long
  Dim start, offset As Long
  Dim value As String
  
  ' Is MIS misspelled ad MISS?
  start = InStr(data_str, "MISS")
  If start > 0 Then offset = 1 Else offset = 0
   
  start = InStr(data_str, "MIS")
  If start > 0 Then
    HasMissile = True
    value = Mid$(data_str, start + 3 + offset, 1)
    MissileB = GetTriHex(value)
    value = Mid$(data_str, start + 4 + offset, 1)
    MissileS = GetTriHex(value)
    value = Mid$(data_str, start + 5 + offset, 1)
    MissileT = GetTriHex(value)
    value = Mid$(data_str, start + 6 + offset, 1)
    MissileH = GetTriHex(value)
  Else
    HasMissile = False: ret = 0
  End If
End Function

Function HasMissileWT(data_str As String) As Long
  Dim start, offset As Long
  Dim value As String
  
  ' Is MIS misspelled as MISS?
  start = InStr(data_str, "miss")
  If start > 0 Then offset = 1 Else offset = 0
   
  start = InStr(data_str, "mis")
  If start > 0 Then
    HasMissileWT = True
    value = Mid$(data_str, start + 3 + offset, 1)
    MissileB = GetTriHex(value)
    value = Mid$(data_str, start + 4 + offset, 1)
    MissileS = GetTriHex(value)
    value = Mid$(data_str, start + 5 + offset, 1)
    MissileT = GetTriHex(value)
    value = Mid$(data_str, start + 6 + offset, 1)
    MissileH = GetTriHex(value)
    ret = 1
  Else
    HasMissileWT = False: ret = 0
  End If
End Function

Function HasMultiWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "multi ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasMultiWT = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasMultiWT = False: ret = 0
  End If
End Function

Function HasPen(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "PEN ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasPen = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasPen = False: ret = 0
  End If
End Function

Function HasPenWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "pen")
  If start > 0 Then
    HasPenWT = True: ret = 1
  Else
    HasPenWT = False: ret = 0
  End If
End Function

Function HasPointDefense(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "PD ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasPointDefense = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasPointDefense = False: ret = 0
  End If
End Function

Function HasReserve(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "RESERVE ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasReserve = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasReserve = False: ret = 0
  End If
End Function

Function HasResist(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "RESIST ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasResist = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasResist = False: ret = 0
  End If
End Function

Function HasScan(data_str As String) As Long
  Dim start As Long, mark1 As Long, mark2 As Long, mark3 As Long
  
  HasScan = False: ret = 0: Scope = 0
  If Left$(data_str, 1) = "[" Then   ' a weapon tag
    start = InStr(data_str, "scan ")
    If start > 0 Then
      HasScan = True
      mark1 = InStr(start, data_str, " ")
      mark2 = InStr(mark1 + 1, data_str, " ")
      mark3 = InStr(mark2 + 1, data_str, " ")
      If mark3 = 0 Then mark3 = Len(data_str)
      ret = val(Mid$(data_str, mark1 + 1, mark2 - mark1))
      Scope = val(Mid$(data_str, mark2 + 1, mark3 - mark2))
    End If
  Else
    start = InStr(data_str, "SCAN ")
    If start > 0 Then
      HasScan = True
      mark1 = InStr(start, data_str, " ")
      mark2 = InStr(mark1 + 1, data_str, " ")
      mark3 = InStr(mark2 + 1, data_str, " ")
      If mark3 = 0 Then mark3 = Len(data_str)
      ret = val(Mid$(data_str, mark1 + 1, mark2 - mark1))
      Scope = val(Mid$(data_str, mark2 + 1, mark3 - mark2))
    End If
  End If
End Function


Function HasShots(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "SHOTS ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasShots = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasShots = False: ret = 0
  End If
End Function

Function HasShields(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "SR ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasShields = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasShields = False: ret = 0
  End If
End Function

Function HasSpecial(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "SPECIAL ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasSpecial = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasSpecial = False: ret = 0
  End If
End Function

Function HasSpecialWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "special ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasSpecialWT = True: ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasSpecialWT = False: ret = 0
  End If
End Function

Function HasTarget(data_str As String) As Long
  Dim start As Long
  
  If Left$(data_str, 1) = "[" Then     ' a weapon tag
    start = InStr(data_str, "target ")
    If start > 0 Then
      start = InStr(start, data_str, " ")
      HasTarget = True
      ret = val(Mid$(data_str, start + 1, 3))
    Else
      HasTarget = False: ret = 0
    End If
  Else
    start = InStr(data_str, "TARGET ")
    If start > 0 Then
      start = InStr(start, data_str, " ")
      HasTarget = True
      ret = val(Mid$(data_str, start + 1, 3))
    Else
      HasTarget = False: ret = 0
    End If
  End If
End Function

Function HasTime(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "TIME ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasTime = True
    ret = val(Mid$(data_str, start + 1, 3))
  Else
    HasTime = False: ret = 0
  End If
End Function

Function HasVibro(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "VIBRO ")
  If start > 0 Then
    start = InStr(start, data_str, " ")
    HasVibro = True
    ret = val(Mid$(data_str, start + 1, 2))
  Else
    HasVibro = False: ret = 0
  End If
End Function

Function HasVibroWT(data_str As String) As Long
  Dim start As Long
  
  start = InStr(data_str, "vibro")
  If start > 0 Then
    HasVibroWT = True: ret = 1
  Else
    HasVibroWT = False: ret = 0
  End If
End Function

Function HasYield(data_str As String) As Long
  Dim start As Long
  
  If Left$(data_str, 1) = "[" Then
    start = InStr(data_str, "yield ")
    If start > 0 Then
      start = InStr(start, data_str, " ")
      HasYield = True
      ret = val(Mid$(data_str, start + 1, 2))
    Else
      HasYield = False: ret = 0
    End If
  Else
    start = InStr(data_str, "YIELD ")
    If start > 0 Then
      start = InStr(start, data_str, " ")
      HasYield = True
      ret = val(Mid$(data_str, start + 1, 2))
    Else
      HasYield = False: ret = 0
    End If
  End If
End Function

Function GetTriHex(value As String) As Long
    If value >= "0" And value <= "9" Then
        GetTriHex = Asc(value) - 48
    Else
      GetTriHex = 0
      If value >= "A" And value <= "Z" Then GetTriHex = Asc(value) - 55
      If value >= "a" And value <= "z" Then GetTriHex = Asc(value) - 87
    End If
End Function

Function ReadAttacker(AttFile As String) As Long
  
  Dim cnt As Long, L As Long, AttShipsRead As Long
  Dim in_str As String, test As String, status As String
  
On Error GoTo read_err
  
  ' Get fleet information for the Attacker and write it to working file
  Open AttFile For Input As #1
  
re_read_1:
  ' Read the header, skipping any blank lines or comments
  Line Input #1, in_str
  Select Case (Left$(in_str, 1))
    Case "", " ", "'", "#"
      If EOF(1) Then ' ran out of data
        ReadAttacker = 0
        Close #1
        MsgBox "File ran out of data.", vbOKOnly, "FILE ERROR"
        Exit Function
      Else
        GoTo re_read_1
      End If
    Case Else
      cnt = 1
      AttRaceName = read_field(in_str, cnt)
      AttFleetName = read_field(in_str, cnt)
      AttBreakOff = val(read_field(in_str, cnt))
      AttShipsTotal = val(read_field(in_str, cnt))
      AttFleetStrength = val(read_field(in_str, cnt))
      AttTargetBonus = val(read_field(in_str, cnt))
      AttTargetPriority = val(read_field(in_str, cnt))
      AttReserve = val(read_field(in_str, cnt))
  End Select
  ' Race, Fleet Name or ID, Breakoff Level, No. of Ships in Fleet, No. of Hull Points in Fleet, Bonus to Hit, Targeting
  ' Input #1, AttRaceName, AttFleetName, AttBreakOff, AttShipsTotal, AttFleetStrength, AttTargetBonus, AttTargetPriority, AttReserve
  
  ' reset the ship and hull totals
  L = 1:  AttShipsTotal = 0:  AttFleetStrength = 0
  ' Read the ship data
  Do While Not EOF(1)
    Line Input #1, in_str
    Select Case (Left$(in_str, 1))
      Case "", " ", "'", "#"
        If EOF(1) Then
          ReadAttacker = L - 1:
          Close #1
          'MsgBox "File ran out of data.  Ship Count(" + Format$(L) + ")", vbOKOnly, "FILE ERROR"
          GoTo display_data
          'Exit Function ' ran out of data
        End If
      Case Else
        cnt = 1
        AttShipStr(L) = read_field(in_str, cnt)
        MaxBeamA(L) = val(read_field(in_str, cnt))
        CurBeamA(L) = val(read_field(in_str, cnt))
        MaxShieldA(L) = val(read_field(in_str, cnt))
        CurShieldA(L) = val(read_field(in_str, cnt))
        MaxTorpA(L) = val(read_field(in_str, cnt))
        CurTorpA(L) = val(read_field(in_str, cnt))
        MaxHullA(L) = val(read_field(in_str, cnt))
        CurHullA(L) = val(read_field(in_str, cnt))
        CurDamA(L) = val(read_field(in_str, cnt))
        StatusA(L) = val(read_field(in_str, cnt))
        AmmoA(L) = val(read_field(in_str, cnt))
        SpecialA(L) = read_field(in_str, cnt)
      
        AttShipsRead = AttShipsRead + 1
        If Not IsMissile(SpecialA(L)) Then
          AttFleetStrength = AttFleetStrength + MaxHullA(L)
          AttShipsTotal = AttShipsTotal + 1
        End If
        
        L = L + 1
    End Select
  Loop
  Close #1
  
display_data:

  AttShipsLeft = AttShipsRead
    
    ' poplate the rest of the Fleet Header grid
    Form1.MSFlexGrid2.Row = 1
    Form1.MSFlexGrid2.Col = 0:  Form1.MSFlexGrid2.Text = "+"
    Form1.MSFlexGrid2.Col = 1:  Form1.MSFlexGrid2.Text = AttRaceName
    Form1.MSFlexGrid2.Col = 2:  Form1.MSFlexGrid2.Text = AttFleetName
    Form1.MSFlexGrid2.Col = 3:  Form1.MSFlexGrid2.Text = AttBreakOff & "%"
    Form1.MSFlexGrid2.Col = 4:  Form1.MSFlexGrid2.Text = AttShipsTotal
    Form1.MSFlexGrid2.Col = 5:  Form1.MSFlexGrid2.Text = AttFleetStrength
    Form1.MSFlexGrid2.Col = 6:  Form1.MSFlexGrid2.Text = AttTargetBonus & "%"
    Form1.MSFlexGrid2.Col = 7:  Form1.MSFlexGrid2.Text = AttTargetPriority
    Form1.MSFlexGrid2.Col = 8:  Form1.MSFlexGrid2.Text = AttReserve & "%"

    For L = 1 To AttShipsLeft
      If L <= 3000 Then
        Form1.MSFlexGrid1.Row = L
        Form1.MSFlexGrid1.Col = 1:  Form1.MSFlexGrid1.Text = AttShipStr(L)
        Form1.MSFlexGrid1.Col = 2:  Form1.MSFlexGrid1.Text = MaxBeamA(L)
        Form1.MSFlexGrid1.Col = 3:  Form1.MSFlexGrid1.Text = CurBeamA(L)
        Form1.MSFlexGrid1.Col = 4:  Form1.MSFlexGrid1.Text = MaxShieldA(L)
        Form1.MSFlexGrid1.Col = 5:  Form1.MSFlexGrid1.Text = CurShieldA(L)
        Form1.MSFlexGrid1.Col = 6:  Form1.MSFlexGrid1.Text = MaxTorpA(L)
        Form1.MSFlexGrid1.Col = 7:  Form1.MSFlexGrid1.Text = CurTorpA(L)
        Form1.MSFlexGrid1.Col = 8:  Form1.MSFlexGrid1.Text = MaxHullA(L)
        Form1.MSFlexGrid1.Col = 9:  Form1.MSFlexGrid1.Text = CurHullA(L)
        Form1.MSFlexGrid1.Col = 10:  Form1.MSFlexGrid1.Text = CurDamA(L)
  
        SpecialA(L) = SpecialA(L) + GetStatusString(StatusA(L))
        Form1.MSFlexGrid1.Col = 11:  Form1.MSFlexGrid1.Text = SpecialA(L)
      End If
    Next L

  ReadAttacker = AttShipsRead
  Exit Function
  
read_err:
  MsgBox "Failed to read fleet file.", vbOKOnly, "FILE ERROR"

End Function

Function ReadDefender(DefFile As String) As Long
  
  Dim L As Long, cnt As Long, DefShipsRead As Long, rownum As Long
  Dim in_str As String, status As String, test As String
    
  ' Get fleet information for the Defender and write it to working file
  Open DefFile For Input As #2
  ' Read the header

re_read_1:
  ' Read the header, skipping any blank lines or comments
  Line Input #2, in_str
  Select Case (Left$(in_str, 1))
    Case "", " ", "'", "#"
      If EOF(2) Then ' ran out of data
        ReadDefender = 0: Exit Function
      Else
        GoTo re_read_1
      End If
    Case Else
      cnt = 1
      DefRaceName = read_field(in_str, cnt)
      DefFleetName = read_field(in_str, cnt)
      DefBreakOff = val(read_field(in_str, cnt))
      DefShipsTotal = val(read_field(in_str, cnt))
      DefFleetStrength = val(read_field(in_str, cnt))
      DefTargetBonus = val(read_field(in_str, cnt))
      DefTargetPriority = val(read_field(in_str, cnt))
      DefReserve = val(read_field(in_str, cnt))
  End Select
  
  ' reset the ship and hull totals
  L = 1:  DefShipsTotal = 0:  DefFleetStrength = 0
  ' Read the ship data
  Do While Not EOF(2)
    Line Input #2, in_str
    Select Case (Left$(in_str, 1))
      Case "", " ", "'", "#"
        If EOF(2) Then ReadDefender = L - 1: Exit Function ' ran out of data
      Case Else
        cnt = 1
        DefShipStr(L) = read_field(in_str, cnt)
        MaxBeamB(L) = val(read_field(in_str, cnt))
        CurBeamB(L) = val(read_field(in_str, cnt))
        MaxShieldB(L) = val(read_field(in_str, cnt))
        CurShieldB(L) = val(read_field(in_str, cnt))
        MaxTorpB(L) = val(read_field(in_str, cnt))
        CurTorpB(L) = val(read_field(in_str, cnt))
        MaxHullB(L) = val(read_field(in_str, cnt))
        CurHullB(L) = val(read_field(in_str, cnt))
        CurDamB(L) = val(read_field(in_str, cnt))
        StatusB(L) = val(read_field(in_str, cnt))
        AmmoB(L) = val(read_field(in_str, cnt))
        SpecialB(L) = read_field(in_str, cnt)
      
        DefShipsRead = DefShipsRead + 1
        If Not IsMissile(SpecialB(L)) Then
          DefFleetStrength = DefFleetStrength + MaxHullB(L)
          DefShipsTotal = DefShipsTotal + 1
        End If
        
        L = L + 1
    End Select
  Loop
  Close #2
  DefShipsLeft = DefShipsRead

    ' poplate the rest of the Fleet Header grid
    Form1.MSFlexGrid2.Row = 2
    Form1.MSFlexGrid2.Col = 0:  Form1.MSFlexGrid2.Text = "+"
    Form1.MSFlexGrid2.Col = 1:  Form1.MSFlexGrid2.Text = DefRaceName
    Form1.MSFlexGrid2.Col = 2:  Form1.MSFlexGrid2.Text = DefFleetName
    Form1.MSFlexGrid2.Col = 3:  Form1.MSFlexGrid2.Text = DefBreakOff & "%"
    Form1.MSFlexGrid2.Col = 4:  Form1.MSFlexGrid2.Text = DefShipsTotal
    Form1.MSFlexGrid2.Col = 5:  Form1.MSFlexGrid2.Text = DefFleetStrength
    Form1.MSFlexGrid2.Col = 6:  Form1.MSFlexGrid2.Text = DefTargetBonus & "%"
    Form1.MSFlexGrid2.Col = 7:  Form1.MSFlexGrid2.Text = DefTargetPriority
    Form1.MSFlexGrid2.Col = 8:  Form1.MSFlexGrid2.Text = DefReserve & "%"
    
    For L = 1 To DefShipsLeft
      rownum = L + AttShipsLeft + 1
      If rownum <= 3000 Then
        Form1.MSFlexGrid1.Row = rownum
        Form1.MSFlexGrid1.Col = 1:  Form1.MSFlexGrid1.Text = DefShipStr(L)
        Form1.MSFlexGrid1.Col = 2:  Form1.MSFlexGrid1.Text = MaxBeamB(L)
        Form1.MSFlexGrid1.Col = 3:  Form1.MSFlexGrid1.Text = CurBeamB(L)
        Form1.MSFlexGrid1.Col = 4:  Form1.MSFlexGrid1.Text = MaxShieldB(L)
        Form1.MSFlexGrid1.Col = 5:  Form1.MSFlexGrid1.Text = CurShieldB(L)
        Form1.MSFlexGrid1.Col = 6:  Form1.MSFlexGrid1.Text = MaxTorpB(L)
        Form1.MSFlexGrid1.Col = 7:  Form1.MSFlexGrid1.Text = CurTorpB(L)
        Form1.MSFlexGrid1.Col = 8:  Form1.MSFlexGrid1.Text = MaxHullB(L)
        Form1.MSFlexGrid1.Col = 9:  Form1.MSFlexGrid1.Text = CurHullB(L)
        Form1.MSFlexGrid1.Col = 10:  Form1.MSFlexGrid1.Text = CurDamB(L)
      
        SpecialB(L) = SpecialB(L) + GetStatusString(StatusB(L))
        Form1.MSFlexGrid1.Col = 11:  Form1.MSFlexGrid1.Text = SpecialB(L)
      End If
    Next L

  ReadDefender = DefShipsRead
  
End Function
   
Function AddTag(source As String, target As String) As String
' add the target tag to the source string and return the new string if not present
  Dim start As Long

  start = InStr(source, target)
  If start = 0 Then
    If Left$(source, 1) = "[" Then
      ' if the target tag is upper case, it goes outside the weapon tag
      If (Left$(target, 1) >= "A" And Left$(target, 1) <= "Z") Or Left$(target, 1) <= " " Then
        AddTag = source
        If Right$(AddTag, 1) <> " " Then AddTag = AddTag + " "
        AddTag = AddTag + target
      Else
        AddTag = Left$(source, Len(source) - 1)
        If Right$(AddTag, 1) <> " " Then AddTag = AddTag + " "
        AddTag = AddTag + target + "]"
      End If
    Else
      AddTag = source
      If Right$(AddTag, 1) <> " " Then AddTag = AddTag + " "
      AddTag = AddTag + target
    End If
  Else
    AddTag = source
  End If
End Function

Function RemoveTag(source As String, target As String, num_fields) As String
' remove the target tag from the source string and return the new string

  Dim A, start, end_tag, tag1, tag2 As Long
  
  start = InStr(source, target)
  If start > 0 Then
    end_tag = InStr(start + 1, source, " "): If end_tag = 0 Then end_tag = Len(source)
    If end_tag = 0 Then end_tag = Len(source)
    If num_fields > 0 Then end_tag = InStr(end_tag + 1, source, " "): If end_tag = 0 Then end_tag = Len(source)
    If num_fields > 1 Then end_tag = InStr(end_tag + 1, source, " "): If end_tag = 0 Then end_tag = Len(source)
    
    RemoveTag = Left(source, start - 1) + Right(source, Len(source) - end_tag)
    
    ' fix weapon tags that have lost their closing bracket
    ' because of inserted spaces we have to count the number of opening and
    ' closing brackets
    tag1 = 0: tag2 = 0
    If Left$(RemoveTag, 1) = "[" Then
      For A = 1 To Len(RemoveTag)
        If Mid$(RemoveTag, A, 1) = "[" Then tag1 = tag1 + 1
      Next A
      For A = 1 To Len(source)
        If Mid$(RemoveTag, A, 1) = "]" Then tag2 = tag2 + 1
      Next A
      If tag1 > tag2 Then RemoveTag = RemoveTag + "]"
    End If
  Else
    RemoveTag = source
  End If
  
End Function

Function RemoveOffline(source As String) As String
' remove all the offline tags from the source string and return the new string

  Dim A, start, end_tag, tag1, tag2 As Long
  Dim temp_str As String
  
do_until_no_more:

  start = InStr(source, "offline")
  If start > 0 Then
    end_tag = start + 7
    temp_str = Left(source, start - 1) + Right(source, Len(source) - end_tag)
    source = temp_str
    GoTo do_until_no_more
  Else
    RemoveOffline = source
  End If
  
End Function

Function InsertOffline(source As String) As String
' remove all the offline tags from the source string and return the new string

  Dim A, start, end_tag, tag1, tag2 As Long
  Dim temp_str As String
  
  end_tag = 1
  
do_until_all_done:

  start = InStr(end_tag, source, "]")
  If start > 0 Then
    end_tag = start + 10
    temp_str = Left(source, start - 1) + " offline" + Right(source, Len(source) - (start - 1))
    source = temp_str
    GoTo do_until_all_done
  Else
    InsertOffline = source
  End If
  
End Function

Function InsertRandomOffline(source As String) As String
' remove all the offline tags from the source string and return the new string

  Dim A, start, end_tag, tag1, tag2 As Long
  Dim temp_str As String
  
  end_tag = 1
  
do_until_done:

  start = InStr(end_tag, source, "]")
  If start > 0 Then
    If Int(Rnd() * 2) = 0 Then
      temp_str = Left(source, start - 1) + " offline" + Right(source, Len(source) - (start - 1))
      source = temp_str
      end_tag = start + 10
    Else
      end_tag = start + 2
    End If
    GoTo do_until_done
  End If
  
  InsertRandomOffline = source
  
End Function

Function SetFlags(data_str As String, bit_flag As Long, weapon_type As Long)
  Dim tmp As Long
  
  If HasDisable(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_dis) <> sa_dis) Then bit_flag = bit_flag + sa_dis  ' if not already set
  End If
  
  If HasHeat(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_heat) <> sa_heat) Then bit_flag = bit_flag + sa_heat  ' if not already set
  End If
  
  If HasMeson(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_meson) <> sa_meson) Then bit_flag = bit_flag + sa_meson  ' if not already set
  End If
  
  If HasVibro(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_vibro) <> sa_vibro) Then bit_flag = bit_flag + sa_vibro  ' if not already set
  End If

  If HasCrack(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_crack) <> sa_crack) Then bit_flag = bit_flag + sa_crack  ' if not already set
  End If

  If HasPen(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_pen) <> sa_pen) Then bit_flag = bit_flag + sa_pen  ' if not already set
  End If

  If HasSpecial(data_str) Then
    If (weapon_type = ret Or ret = 3) And ((bit_flag And sa_special) <> sa_special) Then bit_flag = bit_flag + sa_special  ' if not already set
  End If

  SetFlags = bit_flag
End Function

Function SetFlagsWT(data_str As String, bit_flag As Long)
  Dim tmp As Long
  
  If HasDisableWT(data_str) Then
    If (bit_flag And sa_dis) <> sa_dis Then bit_flag = bit_flag + sa_dis
  End If
  
  If HasHeatWT(data_str) Then
    If (bit_flag And sa_heat) <> sa_heat Then bit_flag = bit_flag + sa_heat
  End If
  
  If HasMesonWT(data_str) Then
    If (bit_flag And sa_meson) <> sa_meson Then bit_flag = bit_flag + sa_meson
  End If
  
  If HasVibroWT(data_str) Then
    If (bit_flag And sa_vibro) <> sa_vibro Then bit_flag = bit_flag + sa_vibro
  End If

  If HasCrackWT(data_str) Then
    If (bit_flag And sa_crack) <> sa_crack Then bit_flag = bit_flag + sa_crack
  End If

  If HasBPWT(data_str) Then
    If (bit_flag And sa_bp) <> sa_bp Then bit_flag = bit_flag + sa_bp
  End If

  If HasPenWT(data_str) Then
    If (bit_flag And sa_pen) <> sa_pen Then bit_flag = bit_flag + sa_pen
  End If

  If HasSpecialWT(data_str) Then
    If (bit_flag And sa_special) <> sa_special Then bit_flag = bit_flag + sa_special
  End If

  SetFlagsWT = bit_flag
End Function

Function GetHullTarget(ForceID, UnitData As String, UnitTarget, UnitScope, NumTargets)
  Dim C, TargetSize As Long
  Dim TargetData As String
  
  On Error GoTo hull_error

  ' if UnitTarget = 0 then it is a random shot
  Select Case UnitTarget
    Case -400 To -1
      ' Assign a target ID - with hull target avoidance
      For C = 1 To 20
        Target1 = Int(Rnd * NumTargets) + 1
        If ForceID = 0 Then
          TargetData = SpecialB(Target1)
          TargetSize = MaxHullB(Target1)
        Else
          TargetData = SpecialA(Target1)
          TargetSize = MaxHullA(Target1)
        End If
        If (Not HasReserve(TargetData)) Or IsGlobal(UnitData) Then
          If IsMissile(TargetData) Then
            ' avoid shooting missiles, it's a waste
          Else
            If TargetSize < UnitTarget - Scope Or TargetSize > UnitTarget + Scope Then Exit For ' we found our _not_ target
          End If
        End If
      Next C
    Case 0
      ' Assign a target ID - standard
      For C = 1 To 20
        Target1 = Int(Rnd * NumTargets) + 1
        If ForceID = 0 Then
          TargetData = SpecialB(Target1)
          TargetSize = MaxHullB(Target1)
        Else
          TargetData = SpecialA(Target1)
          TargetSize = MaxHullA(Target1)
        End If
        If (Not HasReserve(TargetData)) Or IsGlobal(UnitData) Then
          If IsMissile(TargetData) Then
            ' avoid shooting missiles, it's a waste
          Else
            Exit For  ' we found our hull/target
          End If
        End If
      Next C
    Case 1 To 400
      ' Assign a target ID - with hull targetting
      For C = 1 To 20
        Target1 = Int(Rnd * NumTargets) + 1
        If ForceID = 0 Then
          TargetData = SpecialB(Target1)
          TargetSize = MaxHullB(Target1)
        Else
          TargetData = SpecialA(Target1)
          TargetSize = MaxHullA(Target1)
        End If
        If (Not HasReserve(TargetData)) Or IsGlobal(UnitData) Then
          If IsMissile(TargetData) Then
            ' avoid shooting missiles, it's a waste
          Else
            If TargetSize >= UnitTarget - Scope And TargetSize <= UnitTarget + Scope Then Exit For ' we found our hull/target
          End If
        End If
      Next C
  End Select
  
  GetHullTarget = Target1
  Exit Function
  
hull_error:
  DoEvents
End Function

Function GetScanTarget(ForceID, UnitData As String, UnitTarget, UnitScope, NumTargets)
  Dim A, C, ValidTarget, TargetSize As Long
  Dim TargetData As String
  
  On Error GoTo scan_error

  ValidTarget = 0
  For A = 1 To 9999: ValidTargets(A) = 0: Next
  
  Select Case UnitTarget
    Case -400 To -1
      ' Assign a target ID - with hull target avoidance
      For A = 1 To NumTargets
        If ForceID = 0 Then
          TargetData = SpecialB(A)
          TargetSize = MaxHullB(A)
        Else
          TargetData = SpecialA(A)
          TargetSize = MaxHullA(A)
        End If
          
        If TargetSize < UnitTarget - Scope Or TargetSize > UnitTarget + Scope Then
          If (Not HasReserve(TargetData)) Or IsGlobal(UnitData) Then
            If IsMissile(TargetData) Then
              ' avoid shooting missiles, it's a waste
            Else
              ValidTarget = ValidTarget + 1  ' we found our _not_ target
              ValidTargets(ValidTarget) = A
            End If
          End If
        End If
      Next A
      
      If ValidTarget = 0 Then ' roll a random valid target
        GoTo random_legal_target
      Else
        ' find a random target from the list of ValidTargets
        Target1 = Int(Rnd * ValidTarget) + 1
      End If
    
    Case 0
      ' Print #3, "[scanning for size zero is pretty useless]"
random_legal_target:
      ' Assign a target ID - standard
      For C = 1 To 20
        Target1 = Int(Rnd * NumTargets) + 1
        If ForceID = 0 Then
          TargetData = SpecialB(Target1)
          TargetSize = MaxHullB(Target1)
        Else
          TargetData = SpecialA(Target1)
          TargetSize = MaxHullA(Target1)
        End If
        If (Not HasReserve(TargetData)) Or IsGlobal(UnitData) Then
          If IsMissile(TargetData) Then
            ' avoid shooting missiles, it's a waste
          Else
            Exit For  ' we found our target
          End If
        End If
      Next C
    
    Case 1 To 400
      ' Assign a target ID - with hull targetting
      For A = 1 To NumTargets
        If ForceID = 0 Then
          TargetData = SpecialB(A)
          TargetSize = MaxHullB(A)
        Else
          TargetData = SpecialA(A)
          TargetSize = MaxHullA(A)
        End If
          
        If TargetSize >= UnitTarget - Scope And TargetSize <= UnitTarget + Scope Then
          If (Not HasReserve(TargetData)) Or IsGlobal(UnitData) Then
            If IsMissile(TargetData) Then
              ' avoid shooting missiles, it's a waste
            Else
              ValidTarget = ValidTarget + 1  ' we found our _not_ target
              ValidTargets(ValidTarget) = A
            End If
          End If
        End If
      Next A
      
      If ValidTarget = 0 Then ' roll a random valid target
        GoTo random_legal_target
      Else
        ' find a random target from the list of ValidTargets
        Target1 = ValidTargets(Int(Rnd * ValidTarget) + 1)
      End If
  End Select
  
  GetScanTarget = Target1
  Exit Function
  
scan_error:
  DoEvents
End Function

Function NotFunctional(beams As Long, torps As Long, data_str As String) As Boolean

  Dim BeamsOut, TorpsOut, WeaponsOut, old_start, start, sc, weapon_value As Long
  Dim MissilesOnline, start1 As Long
  Dim weapon_str As String
        
  BeamsOut = 0: TorpsOut = 0: sc = 0: MissilesOnline = 0: ret = 0
  NotFunctional = False
        
  If InStr(data_str, "[") Then ' has weapon tags
    WeaponsOut = 1
    old_start = InStr(1, data_str, "["): sc = 0
    Do While old_start > 0
      start = InStr(old_start, data_str, "[")
      start1 = InStr(start, data_str, "]")
      If start1 = 0 Then old_start = 0: start1 = Len(data_str) Else old_start = InStr(start1, data_str, "[")
      sc = sc + 1
      weapon_str = Mid$(data_str, start, (start1 - start) + 1)
      weapon_value = val(Mid$(weapon_str, 2))

      If weapon_value > 0 And (Not IsOffline(weapon_str)) Then
        If HasMissileWT(weapon_str) Then MissilesOnline = 1 Else WeaponsOut = 0
      End If
    Loop
    If WeaponsOut = 1 Then
      NotFunctional = True
      If MissilesOnline = 1 Then ret = 2 Else ret = 1
    End If
  Else
    If beams = 0 Then BeamsOut = 1
    If IsNoBeam(data_str) Then BeamsOut = 1
        
    If torps = 0 Then TorpsOut = 1
    If IsNoTorp(SpecialA(B)) Then TorpsOut = 1
        
    If HasMissile(data_str) And BeamsOut = 1 Then ret = 2: NotFunctional = True
    If BeamsOut = 1 And TorpsOut = 1 Then ret = 1: NotFunctional = True
  End If
End Function
  
' read_field: fetch a CSV field from a string
'   in_str string to parse
'   field_cnt the requested field, starting at one
'
' Updates field_cnt and returns CSV substring
' On error returns an empty string
'
Public Function read_field(in_str As String, field_cnt As Long) As String
  Dim cur_cnt As Long, A As Long, end_of_field As Long, quote As Long
  Dim tmp_str As String

  On Error GoTo no_string
  
  cur_cnt = 1
  
  A = 1
  Do While cur_cnt <= field_cnt And A < Len(in_str)
    tmp_str = "": end_of_field = 0: quote = 0
    
    Do While end_of_field = 0
      If Mid(in_str, A, 1) = Chr(34) Then
        If quote = 0 Then quote = 1 Else quote = 0
      Else
        If quote = 0 And Mid(in_str, A, 1) = "," Then
          end_of_field = 1
        Else
          tmp_str = tmp_str + Mid(in_str, A, 1)
        End If
      End If
      A = A + 1
      If A > Len(in_str) Then end_of_field = 1
    Loop
    
    cur_cnt = cur_cnt + 1
  Loop

  field_cnt = cur_cnt   ' update field count
  read_field = tmp_str  ' return requested field
  Exit Function

no_string:
  Err.Clear
  read_field = ""       ' return empty string
End Function


Public Sub ParseTables()
  Dim line_in As String
  Dim f1 As Long, f2 As Long, f3 As Long
  Dim f4 As String, temp_str As String
  Dim old_id As Long, index_cnt As Long, offset As Long
  
  On Error GoTo handle_parse
  
  index_cnt = 1
  old_id = -99

  Open "be_crits.txt" For Input As #1
  While Not EOF(1)
continue_parse:
    Input #1, line_in
    f1 = val(line_in)
    If f1 = 0 Then
      ' this line is a comment (ignore) or a parameter
      
      If Left(line_in, Len("PLANETS")) = "PLANETS" Then
        Input #1, line_in
        crit_dice1 = val(line_in)
        Input #1, line_in
        crit_dice2 = val(line_in)
        Input #1, line_in
        crit_dice3 = val(line_in)
      End If

    Else
      ' try to parse the data
      Input #1, line_in
      f2 = val(line_in)
      Input #1, line_in
      f3 = val(line_in)
      Input #1, f4
          
      If old_id <> f1 Then
        master_index(f1) = index_cnt
        old_id = f1
      End If
      master_weight(f1) = master_weight(f1) + f2
      
      ReDim Preserve table_id(index_cnt + 1)
      ReDim Preserve table_weight(index_cnt + 1)
      ReDim Preserve table_sub(index_cnt + 1)
      ReDim Preserve table_info(index_cnt + 1)

      table_id(index_cnt) = f1
      table_weight(index_cnt) = f2
      table_sub(index_cnt) = f3
      table_info(index_cnt) = f4
      Debug.Print index_cnt, f1, f2, f3, f4
      index_cnt = index_cnt + 1
    End If
  Wend
  Close #1
  Exit Sub

handle_parse:
  If Err.Number = 53 Then
    MsgBox "File 'be_crits.txt' was not found.", vbOKOnly, "Fatal Error"
    End
  End If
  If Err.Number = -19 Then GoTo continue_parse
  MsgBox "The Battle Engine has found a error that it does not know how to handle." & Chr(10) & Chr(13) & Chr(13) & Err.Description & " - [" & Str(Err.Number) & "]", vbOKOnly, "Unhandled Error"
  Close #1
End Sub


Function FetchData(cur_table As Long) As String
  Dim cur_index, dice, table_value As Long
  Dim found As Boolean
  
  FetchData = ""
  
  found = False
  dice = Int(Rnd() * master_weight(cur_table))
  cur_index = master_index(cur_table)
  
  While Not found
    dice = dice - table_weight(cur_index)
    If dice <= 0 Then
      found = True
    Else
      cur_index = cur_index + 1
    End If
  Wend
  
 FetchValue = table_sub(cur_index)
 FetchData = table_info(cur_index)
End Function
