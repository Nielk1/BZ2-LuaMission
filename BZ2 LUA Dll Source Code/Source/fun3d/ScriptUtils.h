#ifndef _ScripUtils_
#define _ScripUtils_

// Interface between game and mission ai
#ifndef MISN_INTERNAL
#ifdef _GameObject_
#define MISN_INTERNAL 0
#else
#define MISN_INTERNAL 1
#endif
#endif

// DLL versioning code to make sure things are in sync between app and
// dll-- this magic # should be changed every time the misnExport
// structure changes to trap old DLLs. This magic # was chosen
// randomly, and incrementing it by 1 each time is a good update
// Changes in MisnImport do _NOT_ require changes to LATEST_DLL_VERSION
#define LATEST_DLL_VERSION 0x4C89322D
// Identifier of who last modified LATEST_DLL_VERSION, so that
// sourcesafe will catch attempts to modify from 2 people at
// once. Pick a 4-digit identifier for yourself and add to this list:
// Nathan = '~GSH'
// Brad = 'BRAD'
// Ken = 'KEN3'
#define LATEST_DLL_VERSION_MODIFIER '~GSH'

// Set of return codes from the PlayerEjected/PlayerKilled call to DLL
enum EjectKillRetCodes {
	DoEjectPilot, // Do 'standard' eject
	DoRespawnSafest, // Respawn a 'PLAYER' at safest spawnpoint
	DLLHandled, // DLL handled actions. Do nothing ingame
	DoGameOver, // Game over, man.
};

// Deathmatch01.DLL game subtypes (ivar7) list. Used to synchronize UI
// and gameplay behavior between main code and DLL
enum DeathmatchGameSubtypes {
	DMSubtype_Normal=0,
	DMSubtype_KOH,
	DMSubtype_CTF,
	DMSubtype_Loot,
	DMSubtype_RESERVED1, // For internal Pandemic use-- Andrew's work
	DMSubtype_Race1, // Race, respawn as person
	DMSubtype_Race2, // Race, respawn as vehicle
	DMSubtype_Normal2, // Normal, but respawn as vehicle (no person)
	DMSUBTYPE_COUNT,
};

// For activating the #Laps interface, and behaving like a race DLL
const bool DMIsRaceSubtype[DMSUBTYPE_COUNT]={false,false,false,false,false,true,true,false};

enum PathType;

#define DLLAPI __cdecl

#if MISN_INTERNAL
#include <stdio.h>
#include <limits.h>

class GameObject;
class AiPath;

// Make sure these are available
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define RGB(r,g,b) (0xFF000000|(r)<<16|(g)<<8|(b))
#define RGBA_MAKE(r, g, b, a) \
((unsigned long) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b))) 

#define WHITE RGB(255,255,255)
#define GREEN RGB(0,255,0)
#define RED RGB(255,0,0)

#define SIZEOF(a) (sizeof(a)/sizeof(a[0]))

struct Vector {
	float x;
	float y;
	float z;

	Vector () {}

	Vector(const float vx, const float vy, const float vz)
		:x(vx), y(vy), z(vz)
	{
	}
};

struct VECTOR_2D {
	float x;
	float z;
};

typedef float F32;
struct Matrix
{
	Vector right;
	F32 rightw;
	Vector up;
	F32 upw;
	Vector front;
	F32 frontw;
	Vector posit;
	F32 positw;
};

#define AVD_NONE 0 // don't avoid collisions
#define AVD_FORCE 1 // use force avoidance
#define AVD_PLAN 2 // plan collision avoidance

// keep this enum in sync with AiCmds.h
#include "AiCmds.h"

// functions not worth calling through dllexport
// implement utility functions in the dll (ie SPMission)
Vector Normalize_Vector (const Vector &A);

#endif

typedef char* Name;
typedef int Handle;
typedef int TeamNum;
typedef float Time;
typedef float Dist;
typedef int ScrapValue;
typedef unsigned long DWORD;
typedef DWORD DPID;
#define DPID_UNKNOWN 0xFFFFFFFF

// Max # of different teams that can be used at once; see entities.h
// for real definition
#define MAX_TEAMS 16
// # of multiple-user teams are possible in Teamplay
#define MAX_MULTIPLAYER_TEAMS 2
// # of vehicles usable out of MPVehicles.txt
#define MAX_MP_VEHICLES_ENTRIES 32

#define CTRL_BRACCEL (1<<0)
#define CTRL_STEER (1<<1)
#define CTRL_PITCH (1<<2)
#define CTRL_STRAFE (1<<3)
#define CTRL_JUMP (1<<4)
#define CTRL_DEPLOY (1<<5)
#define CTRL_EJECT (1<<6)
#define CTRL_ABANDON (1<<7)
#define CTRL_FIRE (1<<8)

struct VehicleControls {
	float braccel;
	float steer;
	float pitch;
	float strafe;
	char jump;
	char deploy;
	char eject;
	char abandon;
	char fire;
};

// functions called by mission
// Changes in MisnImport do _NOT_ require changes to LATEST_DLL_VERSION
struct MisnImport {
	float time;
	void (DLLAPI *FailMission)(Time t, char* fileName);
	void (DLLAPI *SucceedMission)(Time t, char* fileName);
	void (DLLAPI *ChangeSide)(void);
	ScrapValue (DLLAPI *AddScrap)(TeamNum t, ScrapValue v);
	ScrapValue (DLLAPI *SetScrap)(TeamNum t, ScrapValue v);
	ScrapValue (DLLAPI *GetScrap)(TeamNum t);
	ScrapValue (DLLAPI *GetMaxScrap)(TeamNum t);
	Handle (DLLAPI *GetTug)(Handle h);
	bool (DLLAPI *HasCargo)(Handle h);
	Dist (DLLAPI *GetDistanceObject)(Handle &h1, Handle &h2);
	Dist (DLLAPI *GetDistancePath)(Handle &h1, Name path, int point);
	Dist (DLLAPI *GetDistancePathPtr)(Handle &h1, AiPath *path, int point);
	Handle (DLLAPI *GetNearestObject)(Handle h);
	Handle (DLLAPI *GetNearestVehicleObject)(Handle h);
	Handle (DLLAPI *GetNearestVehiclePath)(Name path, int point);
	Handle (DLLAPI *GetNearestBuilding)(Handle h);
	Handle (DLLAPI *GetNearestEnemy)(Handle h);
};

// functions called by game. Whenever this structure changes, bump up
// LATEST_DLL_VERSION to trap old DLLs that don't agree with expected API
// Changes in MisnImport do _NOT_ require changes to LATEST_DLL_VERSION
struct MisnExport {
	MisnImport *misnImport;
	unsigned long version; // For version # sanity checking... should be LATEST_DLL_VERSION.
	unsigned long VersionModifier; // For version # sanity checking... should be LATEST_DLL_VERSION_MODIFIER.

	// Function called just after DLL loaded. Do any initial setup stuff here.
	void (DLLAPI *InitialSetup)(void);

	bool (DLLAPI *Save)(bool missionSave);
	bool (DLLAPI *Load)(bool missionSave);
	bool (DLLAPI *PostLoad)(bool missionSave);
	void (DLLAPI *AddObject)(Handle h);
	void (DLLAPI *DeleteObject)(Handle h);
	void (DLLAPI *Update)(void);
	void (DLLAPI *PostRun)(void);

	// AddPlayer is called when someone is added to the game. If
	// IsJoiningPlayer is FALSE, then they're "already" in the game
	// (e.g. server player is already "in" the game when a client
	// joins); IsJoiningPlayer is TRUE when a new client joins. Creating
	// objects should _ONLY_ be done when TRUE is passed in, unless you
	// make sure all versions of the DLL on all machines will behave
	// identically.
	//
	// Returns FALSE if not to be allowed into the game (full,
	// singleplayer, etc) ShouldCreateThem is true if a dll should build
	// objects for this player, false if the server's already created
	// them, and we should use the id for informational purposes only
	bool (DLLAPI *AddPlayer)(DPID id, int Team, bool ShouldCreateThem);

	// Notification call tht someone's leaving the game
	void (DLLAPI *DeletePlayer)(DPID id);

	// Player's bailing out or getting killed. See definition of EjectKillRetCodes for more info
	EjectKillRetCodes (DLLAPI *PlayerEjected)(Handle DeadObjectHandle);
	EjectKillRetCodes (DLLAPI *ObjectKilled)(Handle DeadObjectHandle, Handle KillersHandle);
	EjectKillRetCodes (DLLAPI *ObjectSniped)(Handle DeadObjectHandle, Handle KillersHandle);
	// If PlayerEjected/Killed returned DoRandomRespawnSafest, DLL gets
	// queried as to what ODF to build. This is ONLY called if that's
	// returned; you can default this to returning "PLAYER" or something
	// else if DLL doesn't support that.
	char* (DLLAPI *GetNextRandomVehicleODF)(int ForTeam);
	void (DLLAPI *SetWorld)(int nextWorld);

	// interface handler
	void (DLLAPI *ProcessCommand)(unsigned long crc);

	// Random # seed setter. Called at top of frame
	void (DLLAPI *SetRandomSeed)(unsigned long seed);
};

#if MISN_INTERNAL
extern MisnImport misnImport;
extern "C" MisnExport __declspec(dllexport) * DLLAPI GetMisnAPI(MisnImport *import);
#define DLLEXPORT __declspec( dllimport )
#else
extern MisnExport *misnExport;
void FillMisnImport(MisnImport &misnImport);
#define DLLEXPORT __declspec( dllexport )
#endif

#if MISN_INTERNAL

inline void FailMission(Time t, char* fileName = NULL)
{
	misnImport.FailMission(t, fileName);
}

inline void SucceedMission(Time t, char* fileName = NULL)
{
	misnImport.SucceedMission(t, fileName);
}

inline void ChangeSide(void)
{
	misnImport.ChangeSide();
}

inline ScrapValue AddScrap(TeamNum t, ScrapValue v)
{
	return misnImport.AddScrap(t, v);
}

inline ScrapValue SetScrap(TeamNum t, ScrapValue v)
{
	return misnImport.SetScrap(t, v);
}

inline ScrapValue GetScrap(TeamNum t)
{
	return misnImport.GetScrap(t);
}

inline ScrapValue GetMaxScrap(TeamNum t)
{
	return misnImport.GetMaxScrap(t);
}

inline Time GetTime(void)
{
	return misnImport.time;
}

// tug = GetTug(cargo);
// if (tug != 0)
// Attack(tank1, tug);
inline Handle GetTug(Handle h)
{
	return misnImport.GetTug(h);
}

// if (HasCargo(tug1))
// Attack(tank1, tug1);
inline bool HasCargo(Handle h)
{
	return misnImport.HasCargo(h);
}

inline Dist GetDistance(Handle &h1, Handle &h2)
{
	return misnImport.GetDistanceObject(h1, h2);
}

inline Dist GetDistance(Handle &h1, Name path, int point = 0)
{
	return misnImport.GetDistancePath(h1, path, point);
}

inline Dist GetDistance(Handle &h1, AiPath *path, int point = 0)
{
	return misnImport.GetDistancePathPtr(h1, path, point);
}

inline Handle GetNearestObject(Handle h)
{
	return misnImport.GetNearestObject(h);
}

inline Handle GetNearestVehicle(Handle h)
{
	return misnImport.GetNearestVehicleObject(h);
}

inline Handle GetNearestVehicle(Name path,int point)
{
	return misnImport.GetNearestVehiclePath(path, point);
}

inline Handle GetNearestBuilding(Handle h)
{
	return misnImport.GetNearestBuilding(h);
}

inline Handle GetNearestEnemy(Handle h)
{
	return misnImport.GetNearestEnemy(h);
}

#else

// IN GAME DEFINITIONS

void LoadScriptUtils(void);
void PostLoadScriptUtils(void);
void SaveScriptUtils(void);

void DLLAPI FailMission(Time t, char* fileName = NULL);
void DLLAPI SucceedMission(Time t, char* fileName = NULL);
void DLLAPI ChangeSide(void);

void SetAIP(Name n);

#endif

// Handle friend2 = BuildObject("svturr", 2, friend1);
DLLEXPORT Handle DLLAPI BuildObject(char* odf, int team, Handle h);

// Handle friend2 = BuildObject("svturr", 2, "attack_start");
DLLEXPORT Handle DLLAPI BuildObject(char* odf, int team, Name path);

// AiPath *attack_start = AiPath::Find("attack_start");
// Handle friend2 = BuildObject("svturr", 2, attack_start);
DLLEXPORT Handle DLLAPI BuildObject(char* odf, int team, AiPath *path);

// Vector x = { 0, 0, 0 };
// Handle turret1 = BuildObjectPoint("svturr", 2, x);
DLLEXPORT Handle DLLAPI BuildObject(char* odf, int team, Vector &pos);
DLLEXPORT Handle DLLAPI BuildObject(char* odf, int team, Matrix &mat);

// Handle temp = GetHandle("drone");
// RemoveObject(temp);
//
// Note: during AddObject(Handle h) { .. }, *NEVER* call
// RemoveObject(h) from the middle of that function. You can call
// RemoveObject(SomeOtherH) safely, but AddObject requires that it not
// be deleted for at least a frame.
DLLEXPORT void DLLAPI RemoveObject(Handle h);

DLLEXPORT int DLLAPI GetFirstEmptyGroup(void);
DLLEXPORT void DLLAPI SetGroup(Handle h, int group);

// Attack(friend1, enemy1);
DLLEXPORT void DLLAPI Attack(Handle me, Handle him, int priority = 1);

// Service(friend1, enemy1);
DLLEXPORT void DLLAPI Service(Handle me, Handle him, int priority = 1);

// Goto(friend1, "attack_path");
DLLEXPORT void DLLAPI Goto(Handle me, Name path, int priority = 1);

// Goto(friend1, friend2);
DLLEXPORT void DLLAPI Goto(Handle me, Handle him, int priority = 1);

// Mine
DLLEXPORT void DLLAPI Mine(Handle me, Name path, int priority = 1);

// Follow(friend1, friend2);
DLLEXPORT void DLLAPI Follow(Handle me, Handle him, int priority = 1);

// Defend(friend1);
DLLEXPORT void DLLAPI Defend(Handle me, int priority = 1);

// Defend(friend1, friend2);
DLLEXPORT void DLLAPI Defend2(Handle me, Handle him, int priority = 1);

// Stop(friend1);
DLLEXPORT void DLLAPI Stop(Handle me, int priority = 1);

// Patrol(friend1, "patrol_path");
DLLEXPORT void DLLAPI Patrol(Handle me, Name path, int priority = 1);

// Retreat(friend1, "retreat_path");
DLLEXPORT void DLLAPI Retreat(Handle me, Name path, int priority = 1);

// Retreat(friend1, friend2);
DLLEXPORT void DLLAPI Retreat(Handle me, Handle him, int priority = 1);

// GetIn(friend1, apc1);
DLLEXPORT void DLLAPI GetIn(Handle me, Handle him, int priority = 1);

// Pickup(friend1, object1);
DLLEXPORT void DLLAPI Pickup(Handle me, Handle him, int priority = 1);

// Dropoff(tug1, "return_path");
DLLEXPORT void DLLAPI Dropoff(Handle me, Name path, int priority = 1);

// Build(rig, "sbsilo");
DLLEXPORT void DLLAPI Build(Handle me, char* odf, int priority = 1);

// LookAt(leader1, follow1);
DLLEXPORT void DLLAPI LookAt(Handle me, Handle him, int priority = 1);

// everybody on a team look at somebody
DLLEXPORT void DLLAPI AllLookAt(int team, Handle him, int priority = 1);

// if (IsOdf(enemy1, "svturr"))
// Attack(friend1, enemy1);

DLLEXPORT bool DLLAPI IsOdf(Handle h, char* odf);

// Returns 'i' or 'f' depending on the vehicle's race. Returns 0
// ('\0') if bad handle
DLLEXPORT char DLLAPI GetRace(Handle h);

// get a handle to the local player
DLLEXPORT Handle DLLAPI GetPlayerHandle(void);

// see if a unit is alive and is still pilot controlled 
//
// *** WARNING ***
//
// If the handle you pass in is to a dead object, or it has no AI
// Process, the handle will be cleared to 0 by this function.  If you
// care about the results, then consider replacing code like this:
//
// bool bIsAlive = IsAlive(SomeHandle); // DANGEROUS
//
// // Change to:
// 
// Handle TempH = SomeHandle; // A sacrificial copy of the handle
// bool bIsAlive = IsAlive(TempH); // SAFE
//   - or -
// bool bIsAlive = IsAlive2(SomeHandle); // SAFE
//
// If bIsAlive is false after the calls, the variable passed in is
// zero'd out. Most of the time, you can't get any info out of using
// the old handle (and it'll never be recycled), so zeroing it out
// yourself is a good practice anyhow. Use IsAlive2 for a version that
// doesn't zap the handle passed in.
DLLEXPORT bool DLLAPI IsAlive(Handle &h);

// see if a unit is tagged as flying.
//
// *** WARNING ***
//
// If the handle you pass in is to a dead object, the handle will be
// cleared to 0 by this function.  If you care about the results, then
// consider replacing code like this:
//
// bool bIsFlying = IsFlying(SomeHandle); // DANGEROUS
//
// // Change to:
// 
// Handle TempH = SomeHandle; // A sacrificial copy of the handle
// bool bIsFlying = IsFlying(TempH); // SAFE
//   - or -
// bool bIsFlying = IsFlying2(SomeHandle); // SAFE
//
// If bIsAlive is false after the calls, the variable passed in is
// zero'd out. Most of the time, you can't get any info out of using
// the old handle (and it'll never be recycled), so zeroing it out
// yourself is a good practice anyhow. Use IsFlying2 for a version
// that doesn't zap the handle passed in.
DLLEXPORT bool DLLAPI IsFlying(Handle &h);

//
// *** WARNING ***
//
// If the handle you pass in is to a dead object, or it has no AI
// Process, the handle will be cleared to 0 by this function.  If you
// care about the results, then consider replacing code like this:
//
// bool bIsAliveAndPilot = IsAliveAndPilot(SomeHandle); // DANGEROUS
//
// // Change to:
// 
// Handle TempH = SomeHandle; // A sacrificial copy of the handle
// bool bIsAliveAndPilot = IsAlive(TempH); // SAFE
//   - or -
// bool bIsAliveAndPilot = IsAliveAndPilot(SomeHandle); // SAFE
//
// If bIsAlive is false after the calls, the variable passed in is
// zero'd out. Most of the time, you can't get any info out of using
// the old handle (and it'll never be recycled), so zeroing it out
// yourself is a good practice anyhow. Use IsAliveAndPilot2 for a
// version that doesn't zap the handle passed in.
DLLEXPORT bool DLLAPI IsAliveAndPilot(Handle &h);

// see if a unit still exists
DLLEXPORT bool DLLAPI IsAround(Handle h);

// return the building a unit is in or 0 if no building
DLLEXPORT Handle DLLAPI InBuilding(Handle h);

// return the building of the terminal a unit is at
DLLEXPORT Handle DLLAPI AtTerminal(Handle h);

// Handle friend1 = GetHandle("wave1_turret");
DLLEXPORT Handle DLLAPI GetHandle(Name n);

DLLEXPORT Handle DLLAPI GetHandle(int seqNo);

DLLEXPORT void DLLAPI GetPosition(Handle h, Vector &pos);
inline Vector GetPosition(Handle h)
{
	Vector v;
	GetPosition(h, v);
	return v;
}

// Like GetPosition, but uses the slower search to find handles of
// things that just got blown up. Useful for callbacks from
// ObjectKilled/ObjectSniped/etc.
DLLEXPORT void DLLAPI GetPosition2(Handle h, Vector &pos);

DLLEXPORT void DLLAPI GetFront(Handle h, Vector &dir);
inline Vector GetFront(Handle h)
{
	Vector v;
	GetFront(h, v);
	return v;
}

DLLEXPORT void DLLAPI GetPosition(Handle h, Matrix &m);
DLLEXPORT void DLLAPI SetPosition(Handle h, Matrix &m);

// cause object to take damage
DLLEXPORT void DLLAPI Damage(Handle him, long amt);

// if (GetHealth(friend1) < 0.5f)
// Retreat(friend1, "retreat_path");
DLLEXPORT float DLLAPI GetHealth(Handle h);

DLLEXPORT long DLLAPI GetCurHealth(Handle h);
DLLEXPORT long DLLAPI GetMaxHealth(Handle h);
DLLEXPORT void DLLAPI SetCurHealth(Handle h, long NewHealth);
DLLEXPORT void DLLAPI SetMaxHealth(Handle h, long NewHealth);

DLLEXPORT void DLLAPI AddHealth(Handle h, long health);

DLLEXPORT float DLLAPI GetAmmo(Handle h);
DLLEXPORT void DLLAPI AddAmmo(Handle h, long ammo);

DLLEXPORT long DLLAPI GetCurAmmo(Handle h);
DLLEXPORT long DLLAPI GetMaxAmmo(Handle h);
DLLEXPORT void DLLAPI SetCurAmmo(Handle h, long NewAmmo);
DLLEXPORT void DLLAPI SetMaxAmmo(Handle h, long NewAmmo);

// get the team number of a unit
DLLEXPORT TeamNum DLLAPI GetTeamNum(Handle h);

// set the team number of a unit
DLLEXPORT void DLLAPI SetTeamNum(Handle h, TeamNum t);

DLLEXPORT void DLLAPI SetPosition(Handle h, Name path);
DLLEXPORT void DLLAPI SetVectorPosition(Handle h, Vector where);
DLLEXPORT void DLLAPI SetVelocity(Handle h, const Vector &vel);

DLLEXPORT void DLLAPI SetControls(Handle h, const VehicleControls &controls, unsigned long whichControls = -1);

DLLEXPORT Handle DLLAPI GetWhoShotMe(Handle h);
DLLEXPORT float DLLAPI GetLastEnemyShot(Handle h);
DLLEXPORT float DLLAPI GetLastFriendShot(Handle h);

DLLEXPORT void DLLAPI DefaultAllies(void);
DLLEXPORT void DLLAPI TeamplayAllies(void);
DLLEXPORT void DLLAPI Ally(TeamNum t1, TeamNum t2);
DLLEXPORT void DLLAPI UnAlly(TeamNum t1, TeamNum t2);
DLLEXPORT bool DLLAPI IsAlly(Handle me, Handle him);
// see DLLEXPORT bool DLLAPI IsTeamAllied(TeamNum t1, TeamNum t2) below

DLLEXPORT int DLLAPI AudioMessage(const char* msg, bool purge = false);
DLLEXPORT bool DLLAPI IsAudioMessageDone(int msg);
DLLEXPORT void DLLAPI StopAudioMessage(int Msg);
DLLEXPORT void DLLAPI PreloadAudioMessage(const char* msg);
DLLEXPORT void DLLAPI PurgeAudioMessage(const char* msg);

DLLEXPORT void DLLAPI PreloadMusicMessage(const char* msg);
DLLEXPORT void DLLAPI PurgeMusicMessage(const char* msg);
DLLEXPORT void DLLAPI LoadJukeFile(char* msg);
DLLEXPORT void DLLAPI SetMusicIntensity(int intensity);

DLLEXPORT AiPath *DLLAPI FindAiPath(const Vector &start, const Vector &goal);
DLLEXPORT void DLLAPI FreeAiPath(AiPath *path);
DLLEXPORT void DLLAPI GetAiPaths(int &pathCount, char* *&pathNames);

// SetPathType("patrol_path", LOOP_PATH);
// List of path types:
// enum PathType {
//	ONE_WAY_PATH, // == 0
//	ROUND_TRIP_PATH, // == 1
//	LOOP_PATH, // == 2 
//	BAD_PATH // == 3 -- when it couldn't find a route. Used by HuntTask, Recycle[H]Task
//};
DLLEXPORT void DLLAPI SetPathType(Name path, PathType pathType);

// int low = 0;
// int high = 1;
// SetIndependence(friend1, low);
DLLEXPORT void DLLAPI SetIndependence(Handle me, int independence);

DLLEXPORT bool DLLAPI IsInfo(Name odf);

// start the cockpit timer
DLLEXPORT void DLLAPI StartCockpitTimer(long time, long warn = LONG_MIN, long alert = LONG_MIN);

// start the cockpit timer
DLLEXPORT void DLLAPI StartCockpitTimerUp(long time, long warn = LONG_MAX, long alert = LONG_MAX);

// stop the cockpit timer
DLLEXPORT void DLLAPI StopCockpitTimer(void);

DLLEXPORT void DLLAPI HideCockpitTimer(void);

// get the cockpit timer
DLLEXPORT long DLLAPI GetCockpitTimer(void);

// start an earthquake
DLLEXPORT void DLLAPI StartEarthQuake(float magnitude);

// update an earthquake
DLLEXPORT void DLLAPI UpdateEarthQuake(float magnitude);

// stop an earthquake
DLLEXPORT void DLLAPI StopEarthQuake(void);

DLLEXPORT void DLLAPI ConvertHandles(Handle *h_array, int h_count);
DLLEXPORT bool DLLAPI Read(void *ptr, int bytesize);
DLLEXPORT bool DLLAPI Read(bool *b_array, int b_count);
DLLEXPORT bool DLLAPI Read(float *f_array, int f_count);
// DLLEXPORT bool Read(Handle *h_array, int h_count);
DLLEXPORT bool DLLAPI Read(int *i_array, int i_count);
DLLEXPORT bool DLLAPI Write(void *ptr, int bytesize);
DLLEXPORT bool DLLAPI Write(bool *b_array, int b_count);
DLLEXPORT bool DLLAPI Write(float *f_array, int f_count);
// DLLEXPORT bool Write(Handle *h_array, int h_count);
DLLEXPORT bool DLLAPI Write(int *i_array, int i_count);

DLLEXPORT bool DLLAPI IsPerson(Handle h);


DLLEXPORT int DLLAPI GetCurWorld(void);
DLLEXPORT const char*  DLLAPI GetVarItemStr(char* VarItemName);
DLLEXPORT const int DLLAPI GetVarItemInt(char* VarItemName);

// Gets one of the client-settable variables, based on a player with
// the specified team #. Idx is the civar%d read in; see
// network\vars.txt for explanations/contents
DLLEXPORT const int DLLAPI GetCVarItemInt(int TeamNum,int Idx);
DLLEXPORT const char*  DLLAPI GetCVarItemStr(int TeamNum,int Idx);

// Helps preload assets by letting the DLL say what ODFs
// (e.g. "avrecy") will be used, and be loaded before anything else
DLLEXPORT void DLLAPI PreloadODF(char* cfg);

// Get height of terrain at this location
DLLEXPORT float DLLAPI TerrainFindFloor(float x,float z);

// Adds in a pilot if needed to an object
DLLEXPORT void DLLAPI AddPilotByHandle(Handle h);

// Dumps a string to the console
DLLEXPORT void DLLAPI PrintConsoleMessage(char* msg);

DLLEXPORT float DLLAPI GetRandomFloat(float range);

DLLEXPORT bool DLLAPI IsDeployed(Handle h);
DLLEXPORT void DLLAPI Deploy(Handle h);
DLLEXPORT bool DLLAPI IsSelected(Handle h);

#define HP1_ON 1
#define HP2_ON 2
#define HP3_ON 4
#define HP4_ON 8
#define HP5_ON 16

DLLEXPORT void DLLAPI SetWeaponMask(Handle me, long mask);

DLLEXPORT void DLLAPI GiveWeapon(Handle me, Name weapon);

DLLEXPORT void DLLAPI FireAt(Handle me, Handle him = 0, bool doAim = false);

DLLEXPORT bool DLLAPI IsFollowing(Handle h);
DLLEXPORT Handle DLLAPI WhoFollowing(Handle h);

// set and get the user's target
DLLEXPORT void DLLAPI SetUserTarget(Handle h);

// set and get the user's target
DLLEXPORT Handle DLLAPI GetUserTarget(void);

// make somebody look like they are on a particular team
DLLEXPORT void DLLAPI SetPerceivedTeam(Handle h, TeamNum t);

// see what command a unit is working on
//
// AiCommand what = GetCurrentCommand(friend1);
// if (what == CMD_NONE)
// Attack(friend1, enemy1);
DLLEXPORT AiCommand DLLAPI GetCurrentCommand(Handle me);

// get the target of a units current command
DLLEXPORT Handle DLLAPI GetCurrentWho(Handle me);

// EjectPilot(friend1);
DLLEXPORT void DLLAPI EjectPilot(Handle h);

// make a pilot hop out
DLLEXPORT void DLLAPI HopOut(Handle h);

// what could this possibly do? [Warning-- calls DLL with sniped]
DLLEXPORT void DLLAPI KillPilot(Handle h);

// Removes a vehicle's AI process w/o exploding the pilot (i.e. DLL call)
DLLEXPORT void DLLAPI RemovePilotAI(Handle h);

// see which vehicle a pilot hopped out of
DLLEXPORT Handle DLLAPI HoppedOutOf(Handle h);

// Get the current position and heading of the camera
DLLEXPORT void DLLAPI GetCameraPosition(Vector &pos, Vector &dir);

// Set the current position and heading of the camera
DLLEXPORT void DLLAPI SetCameraPosition(const Vector &pos, const Vector &dir);

// maka camera current and prev positions equals 
DLLEXPORT void DLLAPI ResetCameraPosition();



// find the crc of a string
DLLEXPORT unsigned long DLLAPI CalcCRC(Name n);

// load an interface definition
DLLEXPORT void DLLAPI IFace_Exec(Name n);

// activate a control
DLLEXPORT void DLLAPI IFace_Activate(Name n);

// deactivate a control
DLLEXPORT void DLLAPI IFace_Deactivate(Name n);

// create a mission command
DLLEXPORT void DLLAPI IFace_CreateCommand(Name n);

// create a mission string variable
DLLEXPORT void DLLAPI IFace_CreateString(Name name, Name value);
DLLEXPORT void DLLAPI IFace_SetString(Name name, Name value);
DLLEXPORT void DLLAPI IFace_GetString(Name name, Name value, int maxSize);

// create a mission integer variable
DLLEXPORT void DLLAPI IFace_CreateInteger(Name name, int value);
DLLEXPORT void DLLAPI IFace_SetInteger(Name name, int value);
DLLEXPORT int DLLAPI IFace_GetInteger(Name name);

// set the range of a mission integer variable
DLLEXPORT void DLLAPI IFace_SetIntegerRange(Name name, int low, int high);

DLLEXPORT void DLLAPI IFace_CreateFloat(Name name, float value);
DLLEXPORT void DLLAPI IFace_SetFloat(Name name, float value);
DLLEXPORT float DLLAPI IFace_GetFloat(Name name);

// Listbox accessors
DLLEXPORT void DLLAPI IFace_ClearListBox(Name name);
DLLEXPORT void DLLAPI IFace_AddTextItem(Name name, Name value);
// Returns the selected item from the 'name'd listbox into value. For example:
//   char moveName[40];
//  IFace_GetSelectedItem("MoveManager.MoveList.List", moveName, sizeof(moveName));
DLLEXPORT void DLLAPI IFace_GetSelectedItem(Name name, Name value, int maxSize);

DLLEXPORT void DLLAPI SetSkill(Handle h,int s);

DLLEXPORT void DLLAPI SetPlan(char* cfg, int team = -1);

DLLEXPORT void DLLAPI LogFloat(float v);

DLLEXPORT int DLLAPI GetInstantMyForce(void);
DLLEXPORT int DLLAPI GetInstantCompForce(void);
DLLEXPORT int DLLAPI GetInstantDifficulty(void);
DLLEXPORT int DLLAPI GetInstantGoal(void);
DLLEXPORT int DLLAPI GetInstantType(void);
DLLEXPORT int DLLAPI GetInstantFlag(void);
DLLEXPORT int DLLAPI GetInstantMySide(void);

DLLEXPORT bool DLLAPI StoppedPlayback(void);

/*
CameraReady()
Gets the camera ready to be
used by CameraPath or CameraObject()
This function should only be called once at
the start of a camera sequence.
*/
DLLEXPORT bool DLLAPI CameraReady(void);

/*
CameraPath()
This tells the camera to follow the path Name
at a speed and height (in cm).
The camera will always point toward the target.
Returns true when the path is done.
*/
DLLEXPORT bool DLLAPI CameraPath(Name path_name,int height,int speed,Handle target_handle);

DLLEXPORT bool DLLAPI CameraPathDir(Name path_name,int height,int speed);

/*
PanDone()
Returns true on the frame that the
Camera has finished going down the path.
*/
DLLEXPORT bool DLLAPI PanDone(void);

/*
CameraObject()
This is a camera from the object pointing toward
the target.
i,j,k are the offset in centimeters.
*/
DLLEXPORT bool DLLAPI CameraObject(Handle object_handle,float i,float j,float k,Handle target_handle);

/*
CameraFinish()
Always call this once when you are done with the camera.
*/
DLLEXPORT bool DLLAPI CameraFinish(void);

/*
test to see if user cancelled the cineractive
*/
DLLEXPORT bool DLLAPI CameraCancelled(void);

DLLEXPORT bool DLLAPI FreeCamera(void);
DLLEXPORT bool DLLAPI FreeFinish(void);

DLLEXPORT bool DLLAPI PlayMovie(char name[20]);
DLLEXPORT void DLLAPI StopMovie(void);
DLLEXPORT bool DLLAPI PlayMove(void);
DLLEXPORT bool DLLAPI PlayRecording(char name[20]);
DLLEXPORT bool DLLAPI PlayRecording(char name[20], bool updateCam);
DLLEXPORT bool DLLAPI PlaybackVehicle(char name[20]);

// set the current animation (animType: 0 == loop, 1 == 2way)
// return max frames
DLLEXPORT float DLLAPI SetAnimation(Handle h, Name n, int animType = 0);
DLLEXPORT float DLLAPI GetAnimationFrame(Handle h, Name n);

// start the current animation for an object
DLLEXPORT void DLLAPI StartAnimation(Handle h);

// start/stop emitter effects
DLLEXPORT void DLLAPI MaskEmitter(Handle h, DWORD mask);
DLLEXPORT void DLLAPI StartEmitter(Handle h, int slot);
DLLEXPORT void DLLAPI StopEmitter(Handle h, int slot);

DLLEXPORT void DLLAPI SaveObjects(char* &buffer, unsigned long &size);
DLLEXPORT void DLLAPI LoadObjects(char* buffer, unsigned long size);
DLLEXPORT void DLLAPI IgnoreSync(bool on);
DLLEXPORT bool DLLAPI IsRecording(void);

DLLEXPORT void DLLAPI SetObjectiveOn(Handle h);
DLLEXPORT void DLLAPI SetObjectiveOff(Handle h);
DLLEXPORT void DLLAPI SetObjectiveName(Handle h, Name n);

// clear all objectives
DLLEXPORT void DLLAPI ClearObjectives(void);

// add an objective
DLLEXPORT void DLLAPI AddObjective(char* name, long color, float showTime = 8.0f);

DLLEXPORT bool DLLAPI IsWithin(Handle &h1, Handle &h2, Dist d);
DLLEXPORT int DLLAPI CountUnitsNearObject(Handle h, Dist d, TeamNum t, char* odf);

DLLEXPORT void DLLAPI SetAvoidType(Handle h, int avoidType);

// hit somebody to draw attention
DLLEXPORT void DLLAPI Annoy(Handle me, Handle him);

// don't let a unit apply thrust
DLLEXPORT void DLLAPI ClearThrust(Handle h);

// make a unit talkative even if scripted
DLLEXPORT void DLLAPI SetVerbose(Handle h, bool on);

DLLEXPORT void DLLAPI ClearIdleAnims(Handle h);
DLLEXPORT void DLLAPI AddIdleAnim(Handle h, Name anim);

DLLEXPORT bool DLLAPI IsIdle(Handle h);

DLLEXPORT void DLLAPI CountThreats(Handle h, int &here, int &coming);

DLLEXPORT void DLLAPI SpawnBirds(int group, int count, Name odf, TeamNum t, Name path);
DLLEXPORT void DLLAPI SpawnBirds(int group, int count, Name odf, TeamNum t, Handle startObj, Handle destObj);
DLLEXPORT void DLLAPI RemoveBirds(int group);

DLLEXPORT void DLLAPI SetColorFade(float ratio, float rate, unsigned long color);

DLLEXPORT void DLLAPI StopCheats(void);

DLLEXPORT void DLLAPI CalcCliffs(Handle h1, Handle h2, float radius);
DLLEXPORT void DLLAPI CalcCliffs(Name path);

DLLEXPORT int DLLAPI StartSoundEffect(const char* file, Handle h = 0);
DLLEXPORT int DLLAPI FindSoundEffect(const char* file, Handle h = 0);
DLLEXPORT void DLLAPI StopSoundEffect(int sound);


// base buildings
//

// Note: these listed items are for items that are meant to be
// per-team singletons (isSingle = true). DLL_TEAM_SLOT_RECYCLER is
// meant to be used for the one and only recycler/recyvehicle per
// team. That's why commtower is listed here and not commbunkers--
// Commbunkers, are meant to be used liberally, and so they can't fit
// into the isSingle mentality of commtowers.
//
// If you want to identify a commbunker from script, get its class and
// check against CLASS_COMMBUNKER. For example,
//
// char ObjClass[64]
//GetObjInfo(h, Get_GOClass, ObjClass);
//if(_stricmp(ObjClass, "CLASS_COMMBUNKER") == 0)
//{
//   // it's a commbunker
//}
const int DLL_TEAM_SLOT_RECYCLER = 1;
const int DLL_TEAM_SLOT_FACTORY = 2;
const int DLL_TEAM_SLOT_ARMORY = 3;
const int DLL_TEAM_SLOT_TRAINING = 4;
const int DLL_TEAM_SLOT_BOMBERBAY = 5;
const int DLL_TEAM_SLOT_SERVICE = 6;
const int DLL_TEAM_SLOT_TECHCENTER = 7;
const int DLL_TEAM_SLOT_COMMTOWER = 8;
const int DLL_TEAM_SLOT_BASE4 = 9; // Whatever's in base slot #4 (only w/ mods)
const int DLL_TEAM_SLOT_BASE5 = 10; // see above
const int DLL_TEAM_SLOT_BASE6 = 11; // see above
const int DLL_TEAM_SLOT_BASE7 = 12; // see above
const int DLL_TEAM_SLOT_BASE8 = 13; // see above
const int DLL_TEAM_SLOT_BASE9 = 14; // see above

// Returns the handle of a given object based on team# and slot, as specified above
DLLEXPORT Handle DLLAPI GetObjectByTeamSlot(int TeamNum, int Slot);


// Translate a string via localize.odf. NOTE: DO NOT USE THIS.  I
// repeat. DO NOT USE THIS. This function does not know how big the
// 'Dst' buffer is you're trying to read into is, so it hard-clamps it
// at 16 chars.  Use TranslateString2 instead (see below)
DLLEXPORT void DLLAPI TranslateString(Name Dst, Name Src);

// Functions to do various transcendental math functions in a way that
// they'll return bitwise-identical values across various CPU
// types. While these may not be needed for SP missions, for any
// multiplayer DLLs, this should be done instead of the regular
// compiler-provided functions to avoid sync errors. 
DLLEXPORT float DLLAPI portable_sin(const float ang);
DLLEXPORT float DLLAPI portable_cos(const float ang);
DLLEXPORT float DLLAPI portable_atan2(const float x,const float y);
DLLEXPORT float DLLAPI portable_asin(const float x);
DLLEXPORT float DLLAPI portable_acos(const float x);

// ----------------------- Multiplayer Functions -----------------------------

// Network status queries. 
DLLEXPORT bool DLLAPI IsNetworkOn(void);
DLLEXPORT bool DLLAPI ImServer(void); // Note: returns FALSE if network not on
DLLEXPORT bool DLLAPI ImDedicatedServer(void);
DLLEXPORT bool DLLAPI IsTeamplayOn(void);

// Returns the # of players actively playing the game.
DLLEXPORT int DLLAPI CountPlayers(void);

// Returns the handle of a local or remote player based on the team #
// passed in.
DLLEXPORT Handle DLLAPI GetPlayerHandle(int Team);

// Same as GetRace(Handle), but takes a team number instead
DLLEXPORT char DLLAPI GetRaceOfTeam(int TeamNum);

// Returns true if this is a local or remotely-controlled player
DLLEXPORT bool DLLAPI IsPlayer(Handle h);

// Returns the user's name for the given vehicle
DLLEXPORT const char*  DLLAPI GetPlayerName(Handle h);

// Returns the teamgroup (0..MAX_MULTIPLAYER_TEAMS) for a given team,
// or an illegal value (0xA5A5A5A5) if the team doesn't fit on any
// normal group
DLLEXPORT int DLLAPI WhichTeamGroup(int Team);

// Returns how many allies this team has that are playing. The count
// does NOT include the specified team. I.e. in non-teamplay, this
// will return 0
DLLEXPORT int DLLAPI CountAllies(int Team);

// In teamplay, returns the commander's team # for whatever teamgroup
// the specified Team is. In non-teamplay, simply returns Team back--
// everyone is their own commander
DLLEXPORT int DLLAPI GetCommanderTeam(int Team);

// Returns the ranges (inclusive) of team #s allied with a specified
// team.  Note: these teams may have no players on them; check if
// GetPlayerHandle(x) returns anything to see if the team # is
// actually in use
DLLEXPORT int DLLAPI GetFirstAlliedTeam(int Team);
DLLEXPORT int DLLAPI GetLastAlliedTeam(int Team);

// Alternat to above -- qeries the game for the teamplay team # ranges
// based on which team-group (0,1) is asked
DLLEXPORT void DLLAPI GetTeamplayRanges(int WhichTeam,int &DefenseTeamNum,int &OffenseMinTeamNum,int &OffenseMaxTeamNum);

// Sets the specified craft to a random angle (in 2D-only). Normally,
// craft are always built facing due north; this spices things up.
DLLEXPORT void DLLAPI SetRandomHeadingAngle(Handle h);

// Teamcolor functions-- talk to Ken about these.
DLLEXPORT void DLLAPI ClearTeamColors(void); 
DLLEXPORT void DLLAPI DefaultTeamColors(void); // For FFA/DM, uses colors sent from server
DLLEXPORT void DLLAPI TeamplayTeamColors(void); // For MPI/Team strat, colors sent from server

DLLEXPORT void DLLAPI SetTeamColor(int team, int red, int green, int blue);
DLLEXPORT void DLLAPI ClearTeamColor(int team);



// Sets the specified craft to be invisible, uncollidable, w/o
// weapons, etc. Used for a dedicated server's fake-vehicle.
DLLEXPORT void DLLAPI MakeInert(Handle h);

// Returns a position 'near' Pos, in the ring between MinRadiusAway
// and MaxRadiusAway, and with roughly the same terrain height as the
// terrain at the original position. This is used to place allies near
// their starting point, and randomize respawns near a location so
// it's harder to multi-kill by firing at a fixed location
DLLEXPORT Vector DLLAPI GetPositionNear(Vector Pos, const float MinRadiusAway, const float MaxRadiusAway);

// Given a Team number, returs the ODF appropriate to the race for
// this ODF. Warning: this returns a temp string, please do NOT cache
// this pointer or assume its contents will be unmodified if you call
// it twice in a row. The RandomizeType is specified below:
enum RandomizeType {
	Randomize_None, // Don't modify what they selected in the shell.
	Randomize_ByRace,
	Randomize_Any,
};
DLLEXPORT char*  DLLAPI GetPlayerODF(int TeamNum, RandomizeType RType=Randomize_None);

// Builds an unpiloted new craft 'near' a handle, with a given team. 
DLLEXPORT Handle DLLAPI BuildEmptyCraftNear(Handle h, char* ODF, int Team, float MinRadiusAway, float MaxRadiusAway);

// Function to have the game calculate a new position around a central
// point-- this allows us to use the portable_sin and portable_cos
// functions to get around an inconsistent FPU. The y-posn is the
// terrain at that location
DLLEXPORT void DLLAPI SetCircularPos(const Vector &CenterPos, const float Radius, const float Angle, Vector &NewPos);


// Get safest, closest spawnpoint or next closest spawnpoint to a
// given position. Location is filled with a null vector (0,0,0) if no
// spawnpoints available
DLLEXPORT Vector DLLAPI GetSafestSpawnpoint(void);
// Returns the first spawnpoint found with the specified team #. If
// no team# found, returns same as randomsafest
DLLEXPORT Vector DLLAPI GetSpawnpoint(int TeamNum);
DLLEXPORT Handle DLLAPI GetSpawnpointHandle(int TeamNum);

// Returns a random spawnpoint that 'looks' safe. [Nobody within 100 meters]
DLLEXPORT Vector DLLAPI GetRandomSpawnpoint(int TeamNum = -1);

// Sets the message in the timer box (e.g. "Time Left: 03:14") to the
// specified string
DLLEXPORT void DLLAPI SetTimerBox(const char* message);

// Prints the specified string to the messages box (where chat
// messages, etc go)
DLLEXPORT void DLLAPI AddToMessagesBox(const char* message);

// The Get* functions return 0 if an invalid handle is passed in; the
// Set* functions do nothing if an invalide handle is passed in. [A
// handle is invalid if it doesn't point to a valid object, or it
// points to an object on a team that has no human players.]
DLLEXPORT int DLLAPI GetDeaths(Handle h);
DLLEXPORT int DLLAPI GetKills(Handle h);
DLLEXPORT int DLLAPI GetScore(Handle h);
DLLEXPORT void DLLAPI SetDeaths(Handle h, const int NewValue);
DLLEXPORT void DLLAPI SetKills(Handle h, const int NewValue);
DLLEXPORT void DLLAPI SetScore(Handle h, const int NewValue);

// Alternate functions that add a specified amount to the score
DLLEXPORT void DLLAPI AddDeaths(Handle h, const int DeltaValue);
DLLEXPORT void DLLAPI AddKills(Handle h, const int DeltaValue);
DLLEXPORT void DLLAPI AddScore(Handle h, const int DeltaValue);


// Set an Handle'd object as a local or remote user, depending on what
// team they're on. This must be called after building a new craft on
// entry or respawn
DLLEXPORT void DLLAPI SetAsUser(Handle h, int Team);


// Set or clear an object's flags
DLLEXPORT void DLLAPI SetNoScrapFlagByHandle(Handle h);
DLLEXPORT void DLLAPI ClearNoScrapFlagByHandle(Handle h);

// See comment in NetMgr.h
DLLEXPORT int DLLAPI GetLocalPlayerTeamNumber(void);
DLLEXPORT DPID DLLAPI GetLocalPlayerDPID(void);

DLLEXPORT void DLLAPI FlagSteal(Handle flag, Handle holder);
DLLEXPORT void DLLAPI FlagRecover(Handle flag, Handle holder);
DLLEXPORT void DLLAPI FlagScore(Handle flag, Handle holder);
DLLEXPORT void DLLAPI MoneyScore(int amount, Handle bagman);

// Tells the NetManager to put up the 'Timelimit' end of game screen
DLLEXPORT void DLLAPI NoteGameoverByTimelimit(void);
// Tells the NetManager to put up the 'Game Ended - %s hit killlimit' end of game screen
DLLEXPORT void DLLAPI NoteGameoverByKillLimit(Handle h);
// Tells the NetManager to put up the 'Game Ended - %s won' end of game screen
DLLEXPORT void DLLAPI NoteGameoverByScore(Handle h);
// Tells the NetManager to put up the 'Game Ended - %s destroyed enemy bases' end of game screen
DLLEXPORT void DLLAPI NoteGameoverByLastWithBase(Handle h);
// Tells the NetManager to put up the 'Game Ended - Team %s destroyed
// enemy bases' end of game screen. Teamgroup is 0..MAX_MULTIPLAYER_TEAMS
DLLEXPORT void DLLAPI NoteGameoverByLastTeamWithBase(int Teamgroup);
// Tells the NetManager to put up the 'Game Ended - no bases' end of game screen
DLLEXPORT void DLLAPI NoteGameoverByNoBases(void);

// Finishes the current game in x SecondsFromNow. Should be called
// after one of the NoteGameoverBy*() functions that'll put up the
// appropriate post-game info. This is used in MP games so that only
// the server quits, and that pulls everyone else back to the shell
DLLEXPORT void DLLAPI DoGameover(float SecondsFromNow);


// In Teamplay, this is a reporting by the DLL that a teamgroup (0,1)
// has built a recycler (or otherwise locked in a race) of the given
// race ('i' or 'f')
DLLEXPORT void DLLAPI SetMPTeamRace(int WhichTeamGroup, char RaceIdentifier);

// -------------------------------------------------------------------
// Functions added after 1.3pb1. Do NOT call these functions with 1.2/1.3pb1
//
// These functions MUST be at the end of this file, or all DLL compatability
// will be lost.

// Gets the target for a remote user or any AI unit
DLLEXPORT Handle DLLAPI GetUserTarget(int TeamNum);
DLLEXPORT Handle DLLAPI GetTarget(Handle h);

// Feeds a command to the command console, as if it'd been typed by the user.
//
// DLL authors - just sprintf_s your own strings to pass in, e.g. :
//
// char consoleCmd[256];
// sprintf_s(consoleCmd, "sky.fogColor %d %d %d", colorR, colorG, colorB);
// IFace_ConsoleCmd(consoleCmd);
//
// [Style comment: sprintf is *DANGEROUS*, and can trash memory if not
// careful.  sprintf_s is *SAFE*. MS has finally made security fairly
// easy out of the box with the _s versions of things like sprintf_s,
// strcat_s, strcpy_s, etc]
//
//
// If bSquelchOutput is true, then nothing will be printed to the console
// as the result of the command. This is the default
DLLEXPORT void DLLAPI IFace_ConsoleCmd(const char* pStr, bool bSquelchOutput = true);

// Prints the specified string to the messages box (where chat
// messages, etc go). Color is a RGBA value, use the RGB() or RGBA()
// macros (see above) to make them, e.g.
// AddToMessagesBox2("Hello, World!", RGB(255,0,255)); // bright purple
DLLEXPORT void DLLAPI AddToMessagesBox2(const char* message, unsigned long color);

// Like IFace_SetString/Face_SetInteger, but they do some more work.
// If the current machine is the host in MP, and Name is
// network.session.ivar* or network.session.svar*, then the change is
// propagated to clients. Clients are responsible for polling for
// changes. Does nothing if not host in MP.
//
// Note: timely delivery is *NOT* *GUARANTEED*. With lag, things may
// arrive up to about 150 turns (15 seconds) after they're set. The
// proper way to write a DLL is that every DLL behaves the same way
// given the same inputs-- they query all the players, etc. You *WILL*
// get lots of sync errors if you use this improperly. Don't use these
// unless you're darn sure you know what you're doing.
DLLEXPORT void DLLAPI Network_SetString(Name name, Name value);
DLLEXPORT void DLLAPI Network_SetInteger(Name name, int value);

// Gets the first empty group on the specified team
DLLEXPORT int DLLAPI GetFirstEmptyGroup(int t);

// Gets the velocity of the specified handle. Returns (0,0,0) if the
// handle is invalid.
DLLEXPORT Vector DLLAPI GetVelocity(Handle h);

// New method of querying an object's properties.

enum ObjectInfoType {
	Get_CFG, // Returns the GameObjectClass's cfg string
	Get_ODF, // Returns the ODF of the object
	Get_GOClass_gCfg, // Returns the GameObjectClass's gCfg string (not 100% sure how it differs from the CFG)
	Get_EntityType, // Returns the entity type of the object, one of the follow (though
	//   not all are possible for objects the DLL knows about). Values will be
	//	a string from the following list:

	// 	"CLASS_ID_NONE",		// undefined object (default)
	// 	"CLASS_ID_CRAFT",		// hovering vehicle
	// 	"CLASS_ID_VEHICLE",	// tracked vehicle
	// 	"CLASS_ID_TORPEDO",	// miniature vehicle
	// 	"CLASS_ID_POWERUP",	// power up pod
	// 	"CLASS_ID_PERSON",	// pilot or soldier
	// 	"CLASS_ID_ANIMAL",	// animal
	// 	"CLASS_ID_STRUCT",	// generic building
	// 	"CLASS_ID_BUILDING",	// important building
	// 	"CLASS_ID_SIGN",		// mine object
	// 	"CLASS_ID_SCRAP",		// scrap piece
	// 	"CLASS_ID_DEPOSIT",	// scrap deposit
	// 	"CLASS_ID_BEACON",	// nav beacon
	// 	"CLASS_ID_PLANT",		// plant object
	// 	"CLASS_ID_TERRAIN",	// terrain object
	// 	"CLASS_ID_WEAPON",	// weapon object
	// 	"CLASS_ID_ORDNANCE",	// ordnance object
	// 	"CLASS_ID_EXPLOSION",	// explosion object
	// 	"CLASS_ID_CHUNK",		// chunk object
	// 	"CLASS_ID_CRASH",		// crash object
	// 	"CLASS_ID_COLLAPSE",	// collapsing object

	Get_GOClass, // Returns the GameObject class of the object, one of the following (though
	// not all are possible for objects the DLL knows about). Values
	// will be a string from the following list (trimmed to 63
	// characters; the filename on each line is just a hint in case
	// you're trying to figure out what it means):

	//   "CLASS_AIR" // AirCraft.h
	//   "CLASS_ANCHORROCKET" // AnchorRocketClass.h
	//   "CLASS_APC" // APC.h
	//   "CLASS_ARCCANNON" // ArcCannonClass.h
	//   "CLASS_ARMORY" // Armory.h
	//   "CLASS_ARTIFACT" // Artifact.h
	//   "CLASS_ARTILLERY" // Artillery.h
	//   "CLASS_ASSAULTTANK" // AssaultTank.h
	//   "CLASS_BARRACKS" // Barracks.h
	//   "CLASS_BEAM"
	//   "CLASS_BLINK" // BlinkDeviceClass.h
	//   "CLASS_BOID"
	//   "CLASS_BOMBER" // Bomber.h
	//   "CLASS_BOMBERBAY" // BomberBay.h
	//   "CLASS_BOUNCEBOMB"
	//   "CLASS_BUILDING" // BuildingClass.h
	//   "CLASS_BULLET" // BulletClass.h
	//   "CLASS_CANNON" // CannonClass.h
	//   "CLASS_CANNON_MACHINEGUN" // MachineGunClass.h
	//   "CLASS_CANNON_TARGETING" // TargetingGunClass.h
	//   "CLASS_CHARGEGUN" // ChargeGunClass.h
	//   "CLASS_COMMBUNKER" // CommBunker.h
	//   "CLASS_COMMTOWER" // CommTower.h
	//   "CLASS_COMMVEHICLE" // CommVehicle.h
	//   "CLASS_CONSTRUCTIONRIG" // ConstructionRig.h
	//   "CLASS_CRAFT" // CraftClass.h
	//   "CLASS_DAMAGEFIELD" // DamageFieldClass.h
	//   "CLASS_DAYWRECKER" // DayWrecker.h
	//   "CLASS_DEPLOYABLE" // Deployable.h
	//   "CLASS_DEPLOYBUILDING" // DeployBuilding.h
	//   "CLASS_DEPOSIT" // Deposit.h
	//   "CLASS_DISPENSER" // DispenserClass.h
	//   "CLASS_EXPLOSION" // ExplosionClass.h
	//   "CLASS_EXTRACTOR" // Extractor.h
	//   "CLASS_FACTORY" // Factory.h
	//   "CLASS_FLAG" // FlagObject.h
	//   "CLASS_FLAREMINE" // FlareMineClass.h
	//   "CLASS_GAMEOBJECT" // GameObjectClass.h
	//   "CLASS_GRENADE" // GrenadeClass.h
	//   "CLASS_GRENADE_LASERPOPPER" // LaserPopperClass.h
	//   "CLASS_GRENADE_POPPER" // PopperClass.h
	// 	//  "CLASS_GRENADE_RADARPOPPER" // RadarPopperClass.h [Same sig as GRENADE_POPPER, uses that]
	//   "CLASS_HOVER" // HoverCraft.h
	//   "CLASS_HOWITZER" // Howitzer.h
	//   "CLASS_I76BUILDING" // Building.h
	//   "CLASS_JAMMER" // JammerTower.h
	//   "CLASS_JETPACK" // JetPackClass.h
	//   "CLASS_KINGOFHILL" // KingOfHill.h
	//   "CLASS_LAUNCHER" // LauncherClass.h
	//   "CLASS_LAUNCHER_IMAGE" // ImageLauncherClass.h
	//   "CLASS_LAUNCHER_MULTI" // MultiLauncherClass.h
	//   "CLASS_LAUNCHER_RADAR" // RadarLauncherClass.h
	//   "CLASS_LAUNCHER_THERMAL" // ThermalLauncherClass.h
	//   "CLASS_LAUNCHER_TORPEDO" // TorpedoLauncherClass.h
	// 	//  "CLASS_LEADER_ROUND" // LeaderRoundClass.h [Same sig as CLASS_CANNON_TARGETING, returns that]
	//   "CLASS_LOCKSHELL" // LockShellClass.h
	//   "CLASS_MAGNETGUN" // MagnetGunClass.h
	//   "CLASS_MAGNETSHELL" // MagnetShellClass.h
	//   "CLASS_MINE" // MineClass.h
	//   "CLASS_MINELAYER" // Minelayer.h
	//   "CLASS_MINE_MAGNET" // MagnetMineClass.h
	//   "CLASS_MINE_PROXIMITY" // ProximityMineClass.h
	//   "CLASS_MINE_TRIP" // TripMineClass.h
	//   "CLASS_MINE_WEAPON" // WeaponMineClass.h
	//   "CLASS_MISSILE" // MissileClass.h
	//   "CLASS_MISSILE_IMAGE" // ImageMissileClass.h
	//   "CLASS_MISSILE_LASER" // LaserMissileClass.h
	//   "CLASS_MISSILE_RADAR" // RadarMissileClass.h
	//   "CLASS_MISSILE_THERMAL" // ThermalMissileClass.h
	//   "CLASS_MORPHTANK" // MorphTank.h
	//   "CLASS_MORTAR" // MortarClass.h
	//   "CLASS_MORTAR_REMOTE" // RemoteDetonatorClass.h
	//   "CLASS_MOTIONSENSOR" // MotionSensor.h
	//   "CLASS_NAVBEACON" // NavBeaconClass.h
	//   "CLASS_OBJECTSPAWN" // ObjectSpawn.h
	//   "CLASS_ORDNANCE" // OrdnanceClass.h
	//   "CLASS_PERSON" // PersonClass.h
	//   "CLASS_PLANT" // Plant.h
	//   "CLASS_POWERED" // PoweredBuilding.h
	//   "CLASS_POWERUP_CAMERA" // CameraPod.h
	//   "CLASS_POWERUP_MONEY" // MoneyPowerup.h
	//   "CLASS_POWERUP_RELOAD" // ServicePowerup.h
	//   "CLASS_POWERUP_REPAIR" // ServicePowerup.h
	//   "CLASS_POWERUP_SERVICE" // ServicePowerup.h
	// 	//  "CLASS_POWERUP_WEAPON" // WeaponPowerup.h [Same sig as CLASS_WEAPON, returns that]
	//   "CLASS_PULSESHELL" // PulseShellClass.h
	//   "CLASS_RECYCLER" // Recycler.h
	//   "CLASS_RECYCLERVEHICLE" // RecyclerVehicle.h
	//   "CLASS_SALVOLAUNCHER" // SalvoLauncherClass.h
	//   "CLASS_SATCHELCHARGE" // SatchelCharge.h
	//   "CLASS_SATCHELPACK" // SatchelPackClass.h
	//   "CLASS_SAV" // SAV.h
	//   "CLASS_SCAVENGER" // Scavenger.h
	//   "CLASS_SCAVENGERH" // ScavengerH.h
	//   "CLASS_SCRAP" // Scrap.h
	//   "CLASS_SEEKER" // SeekerClass.h
	//   "CLASS_SEISMICWAVE" // SeismicWaveClass.h
	//   "CLASS_SERVICE" // ServiceTruck.h
	//   "CLASS_SERVICEH" // ServiceTruckH.h
	//   "CLASS_SHIELDTOWER" // ShieldTower.h
	//   "CLASS_SHIELDUP" // ShieldUpgradeClass.h
	//   "CLASS_SIGN" // BuildingClass.h
	//   "CLASS_SILO" // Silo.h
	//   "CLASS_SNIPERSHELL" // SniperShellClass.h
	//   "CLASS_SPAWNBUOY" // SpawnBuoy.h
	//   "CLASS_SPECIAL" // SpecialItemClass.h
	//   "CLASS_SPECIAL_FORCEFIELD" // ForceFieldClass.h
	//   "CLASS_SPECIAL_IMAGEREFRACT" // ImageRefractClass.h
	//   "CLASS_SPECIAL_RADARDAMPER" // RadarDamperClass.h
	//   "CLASS_SPECIAL_TERRAINEXPOSE" // TerrainExposeClass.h
	//   "CLASS_SPRAYBOMB" // SprayBombClass.h
	//   "CLASS_SPRAYBUILDING" // SprayBuildingClass.h
	//   "CLASS_SUPPLYDEPOT" // SupplyDepot.h
	//   "CLASS_TELEPORTAL" // TelePortalClass.h
	//   "CLASS_TERRAIN" // DummyClass.h
	//   "CLASS_TORPEDO" // TorpedoClass.h
	//   "CLASS_TRACKEDDEPLOYABLE" // TrackedDeployable.h
	//   "CLASS_TRACKEDVEHICLE" // TrackedVehicle.h
	//   "CLASS_TUG" // Tug.h
	//   "CLASS_TURRET" // TurretCraft.h - ibgtow/fbspir (Guntower/gunspires)
	//   "CLASS_TURRETTANK" // TurretTank.h - ivturr/fvturr (vehicle turrets)
	//   "CLASS_WALKER" // Walker.h
	//   "CLASS_WEAPON" // WeaponClass.h
	//   "CLASS_WINGMAN" // Wingman.h
	// 	"CLASS_UNKNOWN" // default

	// Returns an empty string if the handle specified is of an object
	// can't carry such a weapon, or has no weapon in that slot, etc.
	Get_Weapon0Config, 
	Get_Weapon1Config,
	Get_Weapon2Config,
	Get_Weapon3Config,
	Get_Weapon4Config,

	// Gets the weapon ODF for weapons 0-4. Returns an empty string if
	// the handle specified is of an object can't carry such a weapon,
	// or has no weapon in that slot, etc.
	Get_Weapon0ODF, 
	Get_Weapon1ODF,
	Get_Weapon2ODF,
	Get_Weapon3ODF,
	Get_Weapon4ODF,

	// Gets the GameObject class for weapons 0-4. See list above in
	// point 4 for a list of possible return values.

	Get_Weapon0GOClass, 
	Get_Weapon1GOClass,
	Get_Weapon2GOClass,
	Get_Weapon3GOClass,
	Get_Weapon4GOClass,
};

// Gets the type of info on the specified object, into the buffer.
// Returns false if the handle is invalid. This should be used in
// place of the really rude overloading of IsODF that 1.3pb1 had.
// 
// Note: *MAKE SURE* you pass in a buffer of 64 chars. C++ won't care
// if you passed char[16] to this function, but it'd then trash
// memory.
DLLEXPORT bool DLLAPI GetObjInfo(Handle h, ObjectInfoType type, char pBuffer[64]);

// Test for whether the specified ODF exists. Used by IA/MPI DLLs to
// build race-specific things and check for files (like *vrecycpu vs
// *vrecy) w/o generating a ton of warnings in the process. Do not add
// ".odf" to the end of what is passed in this function.
DLLEXPORT bool DLLAPI DoesODFExist(char* odf);

// See comments above IsAlive, IsFlying, IsAliveAndPilot above.
DLLEXPORT bool DLLAPI IsAlive2(Handle h);
DLLEXPORT bool DLLAPI IsFlying2(Handle h);
DLLEXPORT bool DLLAPI IsAliveAndPilot2(Handle h);

// Translate a string via localize.odf. This is the version to use, as it
// takes the length parameter of Dst, e.g.:

//   TranslateString2(tempstr, sizeof(tempstr), "MissionS0503");  // Gun Spire Site
DLLEXPORT void DLLAPI TranslateString2(Name Dst, size_t bufsize, Name Src);

// Returns how much scrap a scavenger (tracked or hovering) is
// currently carrying. Returns -1 if the handle is invalid, or doesn't
// point to a valid tracked/hoverscav.
DLLEXPORT int DLLAPI GetScavengerCurScrap(Handle h);
// Same as above, but returns carrying capacity.
DLLEXPORT int DLLAPI GetScavengerMaxScrap(Handle h);

// Sets how much scrap a scavenger (tracked or hovering) is currently
// carrying. Values <0 or > a scav's max scrap are clamped.
DLLEXPORT void DLLAPI SetScavengerCurScrap(Handle h, int aNewScrap);
// Sets the capacity. Values < 0 or > 1000 are clamped
DLLEXPORT void DLLAPI SetScavengerMaxScrap(Handle h, int aNewScrap);

// Almost the same as Damage, but takes a float. Not a true function
// overload, otherwise the compiler can't tell the difference in a lot
// of the SP DLLs.
DLLEXPORT void DLLAPI DamageF(Handle him, float amt);

// Like Damage()/DamageF(), but sets the damage causer to him. This
// prevents anyone else from taking credit for a kill on them, if this
// damage does the job of killing them.
DLLEXPORT void DLLAPI SelfDamage(Handle him, float amt);

// Flag for the game, that it wants PlayerEjected messages for all
// units, not just local/remote players. Added for compatability w/
// FE's 1.2 DLLs
DLLEXPORT void DLLAPI WantBotKillMessages(void);

// If the server's GamePrefs.ini has a TPS >= 10, calling this will
// enable it for the entire game. Returns the rate in use (10, 15, 20,
// or 30)
DLLEXPORT void DLLAPI EnableHighTPS(int& newRate);

// !!WARNING!! Not network friendly. Do NOT call these in a MP DLL,
// because you'll only get the local user's results.
// These functions get the handle being pointed to by the user, or
// selected.
DLLEXPORT Handle DLLAPI GetLocalUserInspectHandle(void);
DLLEXPORT Handle DLLAPI GetLocalUserSelectHandle(void);

// For a gameobject, if it has a team slot of 14 or lower, this
// function will make sure that it 'owns' that slot. Made so that you
// could have multiple factories or the like on the '2' key, and
// switch which one is active from the DLL. (Normally, whatever was
// built later would win)
DLLEXPORT void DLLAPI ResetTeamSlot(Handle h);

// Added for Avatar. If h points to a gameobject, returns the
// GameObjectClass::CategoryTypeOverride parameter. Returns -2
// if h doesn't point to a gameobject.
DLLEXPORT int DLLAPI GetCategoryTypeOverride(Handle h);

// Added for Avatar. If h points to a gameobject, returns the
// categorytype. Returns -2 if h doesn't point to a gameobject.
DLLEXPORT int DLLAPI GetCategoryType(Handle h);

// Other ODF accessors. Tries to retrieve the specified parameters out
// of the ODF. Returns true if successful. If unsuccessful, returns
// false, and the 'defval' is copied to the returned value.

// Use:
// float v;
// OpenODF("ivscav.odf");
// if(GetODFFloat("ivscav.odf", "CraftClass", "rangeScan", &v))
// {
//   // [Got it]
// }
// CloseODF("ivscav.odf")


DLLEXPORT int DLLAPI GetODFHexInt(const char* file, const char* block, const char* name, int* value = NULL, int defval = 0);
DLLEXPORT int DLLAPI GetODFInt(const char* file, const char* block, const char* name, int* value = NULL, int defval = 0);
DLLEXPORT int DLLAPI GetODFLong(const char* file, const char* block, const char* name, long* value = NULL, long defval = 0);
DLLEXPORT int DLLAPI GetODFFloat(const char* file, const char* block, const char* name, float* value = NULL, float defval = 0.0f);
DLLEXPORT int DLLAPI GetODFDouble(const char* file, const char* block, const char* name, double* value = NULL, double defval = 0.0);
DLLEXPORT int DLLAPI GetODFChar(const char* file, const char* block, const char* name, char* value = NULL, char defval = '\0');
DLLEXPORT int DLLAPI GetODFBool(const char* file, const char* block, const char* name, bool* value = NULL, bool defval = false);
DLLEXPORT int DLLAPI GetODFString(const char* file, const char* block, const char* name, size_t ValueLen = 0, char* value = NULL, const char* defval = NULL);
DLLEXPORT int DLLAPI GetODFColor(const char* file, const char* block, const char* name, DWORD* value = NULL, DWORD defval = 0);
DLLEXPORT int DLLAPI GetODFVector(const char* file, const char* block, const char* name, Vector* value = NULL, Vector defval = Vector(0.0f, 0.0f, 0.0f));

// Functions to ensure that a ODF is open and ready for the above
// GetODF* functions to read values. See comments above for use
// sample.
DLLEXPORT bool DLLAPI OpenODF(char *name);
DLLEXPORT bool DLLAPI CloseODF(char *name);

// Tells the NetManager to put up the 'Game Ended - <custom string>'
// end of game screen. This string is copied off by the exe, and does
// not need to be retained.
DLLEXPORT void DLLAPI NoteGameoverWithCustomMessage(const char* pString);

// For a given handle, adds it to the 'best' group, which is defined
// as the first non-full group matching its type. If there are no such
// groups, it adds it to a new group. Returns the group #, same as
// GetFirstEmptyGroup does, or -1 if any problems.
DLLEXPORT int DLLAPI SetBestGroup(Handle h);

// Added so DLL authors can peek into what's in a group.
// type must be one of the following:
//	Get_CFG, // Returns the GameObjectClass's cfg string
//	Get_ODF, // Returns the ODF of the object
//	Get_GOClass_gCfg, // Returns the GameObjectClass's gCfg string (not 100% sure how it differs from the CFG)
// If any of the params is invalid, or there's nothing in that gruop,
// pBuffer is set to an empty string. Note: AI teams ignore groups, so
// don't trust the results.
DLLEXPORT void DLLAPI GetGroup(int team, int group, ObjectInfoType type, char pBuffer[64]);

// Returns the # of units in the specified team & group.
DLLEXPORT int DLLAPI GetGroupCount(int team, int group);

// Added as part of mantis #400-- sets a remaining lifespan on the
// specified object. This parameter is in seconds, and when it counts
// down to 0, the craft is deleted. Pass a value < 0 to clear
// this. [Note: DM DLL passes -1 for all player craft all the time to
// ensure player craft aren't subject to this countdown].
DLLEXPORT void DLLAPI SetLifespan(Handle h, float timeout);

// Test for whether the specified file exists, in the .pak files,
// loose, or any other place that BZ2 will normally find files in.
// Returns false when it doesn't. This differs from DoesODFExist
// in that you should pass in the extension, e.g.
//
//   DoesFileExist("MPVehicles.txt");
//   DoesFileExist("stock13_ff0.aip");
DLLEXPORT bool DLLAPI DoesFileExist(const char* filename);

// Tries to load the file into the user-specified buffer. If the file
// doesn't exist, returns false, and bufSize will be set to 0. If the
// file does exist, but pData is NULL or bufSize is too small, it sets
// bufSize to the minimum required size, and returns false. So, to
// load a file, you can do the following:
//
// if(DoesFileExist("ibscav.odf")
// {
//    size_t bufSize = 0;
//    LoadFile("ibscav.odf", NULL, bufSize);
//    pData = malloc(bufSize);
//    LoadFile("ibscav.odf", pData, bufSize);
//    [...]
// }
//
// The caller (i.e. DLL) owns the memory passed in, and is responsible
// for its creation and deletion.
DLLEXPORT bool DLLAPI LoadFile(const char* filename, void* pData, size_t& bufSize);

// Audio functions. For these, a DLLAudioHandle is 0 if invalid (i.e.
// not playing). Also, audio files being played are not saved in a
// .sav file or a network join.

typedef int DLLAudioHandle;

// The audio category is used to determine which of the user's volume
// sliders is used, and what priority to assign sounds.
enum DLLAudioCategory
{
	AUDIO_CAT_UNKNOWN,
	AUDIO_CAT_UNIT_DLG,
	AUDIO_CAT_MISSION_DLG,
	AUDIO_CAT_INTERFACE,
	AUDIO_CAT_WEAPON,
	AUDIO_CAT_ORDNANCE,
	AUDIO_CAT_EXPLOSION,
	AUDIO_CAT_ENGINE,
	AUDIO_CAT_AMBIENT,
	AUDIO_CAT_MUSIC,
};

// For the 'name' parameters on sounds, this should be a filename,
// e.g. "silence.wav" or "somefile.ogg". If the name ends in ".wav",
// then the game will check for a ".ogg" file first. If the ogg file
// exists, then it's played in favor of the .wav. 

//
// A DLL should be able to do some sort of music playing by doing the
// following:
//
// int audioHandle; // in shared section
//
// void SetupMusic()
// {
//   musicHandle = StartAudio2D("music1.ogg", 1.0f, 0.0f, 44100.0f, false, true);
// }
//[...]
// void UpdateMusic() // called every update cycle
// {
//   if(!IsAudioPlaying(music))
//   {
//      // Or vary the filename based on something else.
//      musicHandle = StartAudio2D("music2.ogg", 1.0f, 0.0f, 44100.0f, false, true);
//   }
// }



// If stopLast is true, if a sound with the same name is already
// triggered on this craft, then the old one is also killed.
DLLEXPORT DLLAudioHandle DLLAPI StartAudio3D(const char *const name, Handle h, DLLAudioCategory cat = AUDIO_CAT_UNKNOWN, bool loop = false, bool stopLast = false);

DLLEXPORT DLLAudioHandle DLLAPI StartAudio3D(const char *const name, float x, float y, float z, DLLAudioCategory cat = AUDIO_CAT_UNKNOWN, bool loop = false);

// Starts a 2D audio file. See comments below around the
// SetVolume/SetPan/SetRate for what the valid ranges are. A volume
// passed in of -1 here means use the ShellSFX level automatically.
DLLEXPORT DLLAudioHandle DLLAPI StartAudio2D(const char *const name, float volume, float pan, float rate, bool loop = false, bool isMusic = false);

// Checks if an audio file has completed playing; returns true if it's
// still playing. Note: a looped file will *always* report true
// (unless a .sav file is reloaded or a client checks what a server's
// playing). Note: if this function returns false, the audio handle
// will automatically cleared to 0.
DLLEXPORT bool DLLAPI IsAudioPlaying(DLLAudioHandle &h);

// Stops an audio file. It can not be resumed after stopping.
DLLEXPORT void DLLAPI StopAudio(DLLAudioHandle h);

// Pause or resume an audio file. Once a file has been stopped (with
// StopAudio()), or is no longer playing (IsAudioPlaying() returns
// false), then it cannot be paused or resumed.
DLLEXPORT void DLLAPI PauseAudio(DLLAudioHandle h);
DLLEXPORT void DLLAPI ResumeAudio(DLLAudioHandle h);

// Sets the volume. Valid values are 0.0 .. 1.0f. This value will be
// scaled by the user's preferences if adjustByVolumes is true
DLLEXPORT void DLLAPI SetVolume(DLLAudioHandle h, float vol, bool adjustByVolumes = true);

// Sets the pan. Valid values are -1.0f .. 1.0f. Valid only for 2D
// sounds
DLLEXPORT void DLLAPI SetPan(DLLAudioHandle h, float pan);

// Sets the rate. Valid values are 0.0f .. 44100.0f. 
DLLEXPORT void DLLAPI SetRate(DLLAudioHandle h, float rate);

// Returns length (in seconds) of this file. Returns 0 if not playing.
DLLEXPORT float DLLAPI GetAudioFileDuration(DLLAudioHandle h);

// Returns whether the specified handle is playing in looped mode
// (false if it's not playing at all, etc.) Used to stop gun sfx if
// the craft morphs, etc.
DLLEXPORT bool DLLAPI IsPlayingLooped(DLLAudioHandle h);

// Like GetNearestEnemy, but has some filter flags. maxDist has an
// effective max of 450m, but you can tighten the radius if desired.
DLLEXPORT Handle DLLAPI GetNearestEnemy(Handle h, bool ignorePilots, bool ignoreScavs, float maxDist = 450.0f);

// Finds the nearest powerup. maxDist has an effective max of 450m,
// but you can tighten the radius if desired. If ignoreSpawnpoints
// is true, then it skips all "pspwn_1" items.
DLLEXPORT Handle DLLAPI GetNearestPowerup(Handle h, bool ignoreSpawnpoints, float maxDist = 450.0f);

// Finds the nearest person. maxDist has an effective max of 450m,
// but you can tighten the radius if desired.
DLLEXPORT Handle DLLAPI GetNearestPerson(Handle h, bool skipFriendlies, float maxDist = 450.0f);

// Raw accessors for setting a unit's AI commands. You could do a lot
// with this, either in a good way or a bad way. Not all possible
// commands take a who and/or where command, though most calls require
// at least a who or where to be passed in.  Param is only used with
// CMD_BUILD, and needs to be the crc32 of the config to make.
//
// Possible values for the command are. I rather doubt that most will
// do what you want. 

// 	CMD_NONE, // == 0
// 	CMD_SELECT, // == 1
// 	CMD_STOP, // == 2
// 	CMD_GO, // == 3
// 	CMD_ATTACK, // == 4
// 	CMD_FOLLOW, // ... you do the math. Values increase by 1 per line
// 	CMD_FORMATION, // not used anywhere in code.
// 	CMD_PICKUP,
// 	CMD_DROPOFF,
// 	CMD_UNDEPLOY,
// 	CMD_DEPLOY,
// 	CMD_NO_DEPLOY, // Used by crigs, deploybuildings to indicate they can't do that there
// 	CMD_GET_REPAIR,
// 	CMD_GET_RELOAD,
// 	CMD_GET_WEAPON,
// 	CMD_GET_CAMERA, // Human players only.
// 	CMD_GET_BOMB,
// 	CMD_DEFEND,
// 	CMD_RESCUE,
// 	CMD_RECYCLE,
// 	CMD_SCAVENGE,
// 	CMD_HUNT,
// 	CMD_BUILD,
// 	CMD_PATROL,
// 	CMD_STAGE,
// 	CMD_SEND,
// 	CMD_GET_IN,
// 	CMD_LAY_MINES,
// 	CMD_LOOK_AT,
// 	CMD_SERVICE, // == 29
// 	CMD_UPGRADE,
// 	CMD_DEMOLISH,
// 	CMD_POWER,
// 	CMD_BACK,
// 	CMD_DONE,
// 	CMD_CANCEL,
// 	CMD_SET_GROUP,
// 	CMD_SET_TEAM,
// 	CMD_SEND_GROUP,
// 	CMD_TARGET,
// 	CMD_INSPECT,
// 	CMD_SWITCHTEAM,
// 	CMD_INTERFACE,
// 	CMD_LOGOFF,
// 	CMD_AUTOPILOT,
// 	CMD_MESSAGE,
// 	CMD_CLOSE,
// 	CMD_MORPH_SETDEPLOYED, // For morphtanks
// 	CMD_MORPH_SETUNDEPLOYED, // For morphtanks
// 	CMD_MORPH_UNLOCK, // For morphtanks
// 	CMD_BAILOUT,
// 	CMD_BUILD_ROTATE, // Update building rotations by 90 degrees.
// 	CMD_CMDPANEL_SELECT,
// 	CMD_CMDPANEL_DESELECT,

DLLEXPORT void DLLAPI SetCommand(Handle me, int cmd, int priority, Handle who, const Vector& where, int param = 0);
DLLEXPORT void DLLAPI SetCommand(Handle me, int cmd, int priority = 0, Handle who = 0, const Name path = NULL, int param = 0);

// Sets the current gravity for the world. Lower values == more
// floaty.  Bugs in physics, AI, etc may well occur if you set this
// value too far off the default.
DLLEXPORT void DLLAPI SetGravity(float gravity = 12.5f);

// Sets whether objects made with BuildObject() are automatically
// slotted into the 'best' group (with SetBestGroup()). Also, if this
// is true at turn 5, all units preplaced on the map will get
// SetBestGroup() called on them if not already set. Stock 1.3pb4++
// DLLs set this to false, because they completely manage grouping.
// For backwards compatability, this defaults to true if not called.
DLLEXPORT void DLLAPI SetAutoGroupUnits(bool autoGroup = true);

// Kicks a player out of the current session, with an optional
// "explanation", and/or ban ability. Note: bans are by IP, but if
// multiple people are connecting from behind one IP, a ban will get
// them all. Use caution if banning.
//
// This function can (and should) be called by every DLL running in a
// MP game, but it'll do the work *only* if (1) MP is on, (2) the call
// is made on the server machine, and (3) *only* if the handle passed
// in is for a client. (Use IsNetworkOn()/ImServer() to determine the
// first two conditions; GetPlayerHandle(int Team) can help with the
// third.) If you're trying to kick the server player, it'll fail. I
// recommend you call NoteGameoverWithCustomMessage() then
// DoGameover() to end the session.
DLLEXPORT void DLLAPI KickPlayer(Handle h, const char* pExplanationStr = "", bool banAlso = false);

// Returns true when the terrain system believes the cell type is
// water. This is a very quick test, checking the 8x8 meter grid cell
// this point is in, to see whether it's got water. Use
// TerrainGetHeightAndNormal() if you want a finer-grained, but slower
// test. Note - only the x & z positions of 'pos' are used in the
// first variant.
DLLEXPORT bool DLLAPI TerrainIsWater(const Vector &pos);
DLLEXPORT bool DLLAPI TerrainIsWater(float x, float z);

// Like TerrainFindFloor, but returns more info, and can be used to
// get water height. Height & normal are returned in the specified
// varbs. Returns true on success, false on problems. If overWater =
// true, it'll return terrain height or water height, whichever is
// higher at the given input position. If overwater = false, it'll
// ignore water and return only terrain height. You can use this
// behavior as another test of water, use the difference (if any) for
// water depth
DLLEXPORT bool DLLAPI TerrainGetHeightAndNormal(const Vector& pos, float& outHeight, Vector& outNormal, bool useWater);

// Gets a directory usable for writing files. This will be "$MYDOCS/My
// Games\Battlezone II" if running under vista or the /vista or
// /usemydocs commandline flags are used. Otherwise, it'll be the
// directory BZ2 is installed to. Note - this is a *wide character*
// path, so be sure to be widechar friendly. 
//
// bufSize is the size, in widechars, of the buffer pointed to by
// pData. This function returns false if the buffer is invalid or too
// small, and fills in the size needed, including the trailing string
// terminator. For an example of how to use this, try this:
//
// #include <windows.h>  // for _MAX_PATH. Hacky to set _MAX_PATH to 1024, but should work, too
// #include <share.h>
//
// wchar_t* pRootOutputDir = NULL;
// size_t bufSize = 0;
// GetOutputPath(bufSize, pRootOutputDir);
// pData = new wchar_t[bufSize];
// if(GetOutputPath(bufSize, pRootOutputDir))
// {
//    wchar_t outPath[_MAX_PATH];
//    swprintf_s(outPath, L"%sMyLogFile.txt", pRootOutputDir);
//    FILE* pFile = _wfsopen(fullPath, L"w", _SH_DENYWR);
//    if(pFile)
//    {
//        fprintf(pFile, "Hello, world!\n");
//        fclose(pFile);
//    }
// }
// delete[] pRootOutputDir;
//
DLLEXPORT bool DLLAPI GetOutputPath(size_t& bufSize, wchar_t* pData);


// Copies out a path's points to a given buffer. If called with pData
// == NULL or bufSize 'too small', it'll fill in the needed bufSize,
// and return false. If the path specified can't be found, then it
// returns false, and bufSize == 0.  Otherwise, it'll copy in the
// points.  NOTE: bufSize is the # of points, so you MUST allocate 2x
// the number of floats. For a usage sample:
//
// size_t bufSize = 0;
// float* pData = NULL;
// GetPathPoints("edge_path", bufSize, pData);
// pData = new float[2 * bufSize];
// if(GetPathPoints("edge_path", bufSize, pData))
// {
//    size_t i;
//    for(i=0; i<bufSize; ++i)
//       printf("Pathpoint %d at (%.2f, %.2f)\n", i, pData[2*i+0], pData[2*i+1]);
// }
// delete[] pData;
DLLEXPORT bool DLLAPI GetPathPoints(Name path, size_t& bufSize, float* pData);


// Returns the owner of a gameobject. Note: not all objects have known
// owners, so this'll probably return 0 much of the time.
DLLEXPORT Handle DLLAPI GetOwner(Handle h);

// Parallel GetTarget/GetOwner
DLLEXPORT void DLLAPI SetTarget(Handle obj, Handle target);
DLLEXPORT void DLLAPI SetOwner(Handle obj, Handle owner);

// Sets the pilot class for a craft. Note - if you do this on a
// non-craft, it'll silently refuse to do anything.
DLLEXPORT void DLLAPI SetPilotClass(Handle obj, char* odf);

// If the user chose to have allow music tracks while playing, then
// this function allows them. By default, if this function is not
// callled, then it's as if it was called with true (i.e.  allow
// random tracks). This is for maximum backwards compatiblity w/ old
// DLLs. BZ2's SP DLLs call this with false, i.e. force the tracks
// listed in the .trn file to be used instead.
//
// Note - this function must be called asap from the DLL for it to
// take effect. For example, do it near these lines in the stock DLLs:
//
//		AllowRandomTracks(false);
//		b_count = &b_last - &b_first - 1;
//		b_array = &b_first + 1;
DLLEXPORT void DLLAPI AllowRandomTracks(bool bAllow = true);


// More teamcolors functions.
enum TEAMCOLOR_TYPE
{
	TEAMCOLOR_TYPE_DEFAULT, // Defaults
	TEAMCOLOR_TYPE_GAMEPREFS, // What's set in gameprefs
	TEAMCOLOR_TYPE_SERVER, // What the server sent us.
	TEAMCOLOR_TYPE_CURRENT, // Valid only to GetFFATeamColor().
};

// Set functions. Probably easier to use these functions when setting
// a bunch. Does nothing if type is invalid
DLLEXPORT void DLLAPI SetFFATeamColors(TEAMCOLOR_TYPE type);
DLLEXPORT void DLLAPI SetTeamStratColors(TEAMCOLOR_TYPE type);

// Get functions. These will set r/g/b to 0 if a setting for type or
// team is invalid. For GetTeamStratColor, the team parameter must be
// 0 (first teamgroup) or 1 (second teamgroup). Note -
// GetFFATeamColor(TEAMCOLOR_TYPE_CURRENT, ...) can be called even if
// team strat is on; it just allows reading of any team's current color.
// Team Strat has team group A on teams 1..5, group B on 6..10.
DLLEXPORT void DLLAPI GetFFATeamColor(TEAMCOLOR_TYPE type, int team, int& red, int& green, int& blue);
DLLEXPORT void DLLAPI GetTeamStratColor(TEAMCOLOR_TYPE type, int team, int& red, int& green, int& blue);

// Swaps the current teamgroup colors, e.g. swap colors of teams 1..5
// <-> 6..10.  Note - this can be called, even if FFA colors are
// set. You may not like the results if you do that.
DLLEXPORT void DLLAPI SwapTeamStratColors(void);

// Returns whether the last set of team colors set were FFA (returns
// true) or Team Strat (returns false). Calling these functions will
// change its return value:
//
// TeamplayTeamColors(), SetTeamStratColors() - will return false
// DefaultTeamColors(), SetFFATeamColors() - will return true
DLLEXPORT bool DLLAPI GetTeamColorsAreFFA(void);

// After ffa or team colors are set up -- i.e. one of the 4 functions
// listed above GetTeamColorsAreFFA() is called -- this will call
// SetFFATeamColors() or SetTeamStratColors(). Just a convenience
// function.
DLLEXPORT void DLLAPI SetTeamColors(TEAMCOLOR_TYPE type);


// Accessors paralleling scrap, but for power. The AddPower/SetPower
// calls return the current power after the action is taken, as if
// GetPower() was called after the operation.
DLLEXPORT int DLLAPI AddPower(TeamNum t, int v);
DLLEXPORT int DLLAPI SetPower(TeamNum t, int v);
DLLEXPORT int DLLAPI GetPower(TeamNum t);
DLLEXPORT int DLLAPI GetMaxPower(TeamNum t);

// Code has functions that seem to support doing this. So, they're
// exposed.
DLLEXPORT void DLLAPI AddMaxScrap(TeamNum t, int v);
DLLEXPORT void DLLAPI AddMaxPower(TeamNum t, int v);
DLLEXPORT void DLLAPI SetMaxScrap(TeamNum t, int v);
DLLEXPORT void DLLAPI SetMaxPower(TeamNum t, int v);


// Like GetTeamStratColor(), but does take a team # in the 1..10
// range.  Teams 1..5 are the first teamgroup in Team Strat (or the
// human team in MPI), teams 6..10 are the second teamgroup in team
// strat. Commanders are teams 1 & 6, respectively. This function
// returns 
DLLEXPORT void DLLAPI GetTeamStratIndividualColor(TEAMCOLOR_TYPE type, int team, int& red, int& green, int& blue);

// Gets the currently loaded map's .trn filename.
DLLEXPORT const char* DLLAPI GetMapTRNFilename(void);

// Returns true if the specified teams are allied.
DLLEXPORT bool DLLAPI IsTeamAllied(TeamNum t1, TeamNum t2);

// Like IsAliveAndPilot2, but differs subtly. The IsAlive* functions
// will return false if the 'destroyed' flag is set on the object --
// i.e. health < 0. But, there's sometimes a slight delay between when
// destroyed = true, and the end of the death sequence (explosion,
// etc), which is when DLL::ObjectKilled() is called. Thus,
// IsNotDeadAndPilot2() will return true as long as the object hasn't
// fully been killed yet.
DLLEXPORT bool DLLAPI IsNotDeadAndPilot2(Handle h);

// Name accessors. This is the label set on items. Note - for GetName,
// if you want to store the returned value long-term, copy it off. The
// next call to GetName will overwrite the value. Also, NULL will be
// returned if the handle is invalid.
DLLEXPORT const char* DLLAPI GetLabel(Handle h);
DLLEXPORT void DLLAPI SetLabel(Handle h, const char* pLabel);

// Gets the nth tap of object h. h must be something that supports
// taps, e.g. poweredbuilding (or derived)/turretcraft.
DLLEXPORT Handle DLLAPI GetTap(Handle h, int index);

// Sets the nth tap of object baseObjectHandle to
// tapObjectHandle. baseObjectHandle must be something that supports
// taps, e.g. poweredbuilding (or derived)/turretcraft.
DLLEXPORT void DLLAPI SetTap(Handle baseObjectHandle, int index, Handle tapObjectHandle);

// Local ammo accessors. If an invalid handle/weaponIndex is passed
// in, it'll return 0.f. Note that all this is done in floats, not the
// way GetCurAmmo/GetMaxAmmo/SetCurAmmo use longs. The underlying code
// uses floats, and it's just easier for me to expose that as-is.
DLLEXPORT float DLLAPI GetCurLocalAmmo(Handle h, int weaponIndex);
DLLEXPORT void DLLAPI AddLocalAmmo(Handle h, float v, int weaponIndex);
DLLEXPORT float DLLAPI GetMaxLocalAmmo(Handle h, int weaponIndex);
DLLEXPORT void DLLAPI SetCurLocalAmmo(Handle h, float v, int weaponIndex);

#endif

