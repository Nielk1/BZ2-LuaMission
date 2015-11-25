#ifndef _BZ1Helper_
#define _BZ1Helper_

# pragma once

//#ifdef EDITORDEBUG //_DEBUG // "Debug" mode seems to crash BZ2 release versions? Idk why... :(
//#pragma optimize("", off)
//#endif

#include "DLLBase.h"
#include "..\..\source\fun3d\ScriptUtils.h"
#include "BZCScriptUtils.h"

//BZC Additions by GSH, adapted by Avatar. Modified extensively by GBD.

// forward declarations
class PUPMgr;
//class TapAssignmentHelper;
class ProductionClassHelper;
class RecycleHelper;
class PilotResourceHelper;
class BuildingHelper;
class APCHelper;
class HQCPHelper;
class LPadHelper;
class PortalHelper;
class AttacherHelper;
class ReplaceOnDeathHelper;
class UtilityShipHelper;
class PowerHelper;
class DeployHelper;
class FuryHelper;
class LavaDamageHelper;
class ScrapHelper;
class DispatchHelper;
class SatelliteHelper;
class LimiterHelper;
class SwitchClassHelper;
class AnchorClassHelper;
class AmmoDrainHelper;
class UndeployHelper;
class QuakeHelper;
class GeyserHelper;
class AIWeaponHelper;
class AttacherWeaponHelper;
class MorphObjectHelper;
class ShieldHelper;
class EnergyArmorHelper;
class JammerHelper;
class CarrierHelper;
class TransportHelper;
class DropShipHelper;
//class RecallMineHelper;

class BZ1Helper : public DLLBase {

public:

	// General map info.
	struct MapInfo {
		float Height;
		float MapWidth;
		float MapDepth;
		float MPG;
		float MinX;
		float MinZ;
		float MaxWidth;
		float MaxDepth;
		float MinWidth;
		float MinDepth;
		float Gravity;
		int GravityState;
		float MinScrapHeight;
		float MaxScrapHeight;

		MapInfo() { memset(this, 0, sizeof(MapInfo)); };
	};

	// Store team based data. This is where we put all of our [MAX_TEAM] entries together.
	struct TeamInfo {
		// Player stuff.
		Handle RealPlayer; // Keep track of what the player actually is (because of satellite mode). 
		Handle Player; // Store a player's ship if they're in satellite. NOTE: This is ONLY used in Satellite Code!
		Handle SatellitePlayer; // Sat Object for the player. 
		// Recycle point.
		Handle RecyclePoint; // This teams primary Recycle point.
		// Invisible Scrap Silos for these units.
		Handle RecScrap; // OFF MAP!
		Handle MufScrap; // OFF MAP!
		Handle SlfScrap; // OFF MAP!
		Handle CnstScrap; // OFF MAP!
		// This Team's production units.
		Handle Recy;
		Handle Muf;
		Handle Slf;
		Handle Cnst;
		// Dummy Radars
		Handle RecyRadar;
		Handle MufRadar;
		Handle SlfRadar;
		// SLF Anim stuff.
		Handle SLFTarget; // SLF Closest powerup target. //was SLFTargets
		// Satellite stuff.
		Handle CommTower; // Save the Primary Selectable CommTower. -GBD
		Handle SatelliteOn; // Satellite! OFF MAP!
		Handle SatelliteOff; // Satellite! // OFF MAP!
		Handle SatelliteLock; // Satellite! // OFF MAP!
		// Limiter Stuff
		Handle ImaPilot; // Pilot Locker. // OFF MAP!
		Handle DeathCameraTarget;
		Handle Transport; 
		Handle LimiterObject[3]; // For limits. Yay! // OFF MAP!
		Handle LimitPlayer; // For limit code.
		// This Team's Resources.
		PilotValue MaxPilots; // Max pilot count.
		PilotValue CurrPilots; // Current pilot count.
		// AI Stuff.
		int m_CPUAIDifficulty; // Difficulty, 0 = none, 1 = Easy, 2 = Medium, 3 = Hard, 4 = Extreme.
		int m_CPUPilotCheatAmount; // Pilot cheat from AIP file, just like their money cheat. 
		int m_CPUPilotCheatDelay; // Ditto. Interval of cheat.
		int m_CPUMaxPatrolUnits; // Max Number of Patrol units to have.
		int m_CPUMaxHuntUnits; // Max Number of Hunt units to have.
		int m_CPUMaxEscortUnits; // Max Number of Escort units to have.
		int m_CPUNumPatrolUnits; // Number of Patrol units.
		int m_CPUNumHuntUnits; // Number of Hunt units.
		int m_CPUNumEscortUnits; // Number of Escort units.
		// Counts.
		int ReinforcementCount; // Transport timer per Team.
		int NumBarracks; // Check number of Barracks. -GBD
		int NumTeamObjects; // Total number of items on this team.
		// Satellite mode switches.
		int SatelliteViewActive; // Store if satellite is on/off for deactivation/cleaning up. Multi Stage flag. -GBD
		int SatelliteOutOfAmmo; // Counter to delay Satellite deactivation some.
		long SatelliteSiteCamAmmo; // How much Ammo the SiteCam requires.
		// Player Lives stuff.
		int PlayerLives; // How many lives are remaining.
		int DeathCameraTime; // DeathCam Time, in ticks.
		// Player Nav Drop Delay.
		int LastNavTime;
		// Bools
		bool TeamIsDead; // Is this player's team out of lives?
		bool DeathCameraOn; // Is the DeathCam on?
		bool m_UsePilots; // Flag to pop it up with the recy. -GBD
		bool m_HadPilots; // Flag to flag if we once Used Pilots, to tell if we play "pilots depleted" or not. -GBD
		bool m_UseLimits; // Does it use BZ1 Limits? 
		// Production Scraphold Flags.
		bool RecyUseScrapHold;
		bool MufUseScrapHold;
		bool SlfUseScrapHold;
		bool CnstUseScrapHold;
		// AI Stuff.
		bool m_CPUUnitsCanService; // Tells if this AI's units automatically Repair/Reload.
		bool m_CPUDoDispatch; // Flag if AIP does Dispatching.
		// Satellite Stuff.
		bool CommTowerAround; // Get a flag as to if the team has any commtowers. -GBD
		bool PlayerWasPilot; // Save this, or else DIE!!!!
		bool GiveBackWeapons; // Flag to restore a pilot's weapons, if they picked some up.
		bool PlayCommTowerSound; // Flag to play CommTower sound.
		bool UseSiteCamera; // Flag to use site camera or not?
		// SLF Stuff.
		bool m_Distance1Check;
		bool m_Distance2Check;
		bool SlfPlayingAnim;
		// Floats
		float Distance1;
		float Distance2;
		float SLFAnimationFrames;
		// Reinforcement stuff.
		float m_CPUDayWreckerChance; // Chance for dayWeckerage.
		Vector PlayerSpawnPos; // Store the Team's Spawn Vector.
		Vector ReinforcementPos; // Store the Reinforcement Spawn Vector.
		Vector GoalPos; // Store the team's CTF Goal position Vector.
		// Chars
		char TeamRace; // Race.
		char PlayerName[MAX_ODF_LENGTH];
		char TeamAIPFile[MAX_ODF_LENGTH];
		char PilotWeapon[MAX_HARDPOINTS][MAX_ODF_LENGTH];
		char PlayerPilotODF[MAX_ODF_LENGTH];

		// This converts all the Handles.
		void ConvertHandles() { ::ConvertHandles( &RealPlayer, &LimitPlayer - &RealPlayer + 1); }

		TeamInfo() { memset(this, 0, sizeof(TeamInfo)); };

		/*
		friend bool operator==(const TeamInfo &a, const TeamInfo &b)
		{
			// Handles
			if (a.RealPlayer != b.RealPlayer)
				return false;
			if (a.Player != b.Player)
				return false;
			if (a.SatellitePlayer != b.SatellitePlayer)
				return false;
			if (a.RecyclePoint != b.RecyclePoint)
				return false;
			if (a.RecScrap != b.RecScrap)
				return false;
			if (a.MufScrap != b.MufScrap)
				return false;
			if (a.SlfScrap != b.SlfScrap)
				return false;
			if (a.CnstScrap != b.CnstScrap)
				return false;
			if (a.Recy != b.Recy)
				return false;
			if (a.Muf != b.Muf)
				return false;
			if (a.Slf != b.Slf)
				return false;
			if (a.Cnst != b.Cnst)
				return false;
			if (a.RecyRadar != b.RecyRadar)
				return false;
			if (a.RealPlayer != b.RealPlayer)
				return false;
			if (a.MufRadar != b.MufRadar)
				return false;
			if (a.SlfRadar != b.SlfRadar)
				return false;
			if (a.SLFTarget != b.SLFTarget)
				return false;
			if (a.CommTower != b.CommTower)
				return false;
			if (a.SatelliteOn != b.SatelliteOn)
				return false;
			if (a.SatelliteOff != b.SatelliteOff)
				return false;
			if (a.SatelliteLock != b.SatelliteLock)
				return false;
			if (a.ImaPilot != b.ImaPilot)
				return false;
			if (a.DeathCameraTarget != b.DeathCameraTarget)
				return false;
			if (a.Transport != b.Transport)
				return false;
			for(int i = 0; i < 3; i++)
				if (a.LimiterObject[i] != b.LimiterObject[i])
					return false;
			if (a.LimitPlayer != b.LimitPlayer)
				return false;
			// Ints.
			if (a.MaxPilots != b.MaxPilots)
				return false;
			if (a.CurrPilots != b.CurrPilots)
				return false;
			if (a.m_CPUAIDifficulty != b.m_CPUAIDifficulty)
				return false;
			if (a.m_CPUPilotCheatAmount != b.m_CPUPilotCheatAmount)
				return false;
			if (a.m_CPUPilotCheatDelay != b.m_CPUPilotCheatDelay)
				return false;
			if (a.m_CPUMaxPatrolUnits != b.m_CPUMaxPatrolUnits)
				return false;
			if (a.m_CPUMaxHuntUnits != b.m_CPUMaxHuntUnits)
				return false;
			if (a.m_CPUMaxEscortUnits != b.m_CPUMaxEscortUnits)
				return false;
			if (a.m_CPUNumPatrolUnits != b.m_CPUNumPatrolUnits)
				return false;
			if (a.m_CPUNumHuntUnits != b.m_CPUNumHuntUnits)
				return false;
			if (a.m_CPUNumEscortUnits != b.m_CPUNumEscortUnits)
				return false;
			if (a.ReinforcementCount != b.ReinforcementCount)
				return false;
			if (a.NumBarracks != b.NumBarracks)
				return false;
			if (a.NumTeamObjects != b.NumTeamObjects)
				return false;
			if (a.SatelliteViewActive != b.SatelliteViewActive)
				return false;
			if (a.SatelliteOutOfAmmo != b.SatelliteOutOfAmmo)
				return false;
			if (a.PlayerLives != b.PlayerLives)
				return false;
			if (a.DeathCameraTime != b.DeathCameraTime)
				return false;
			// Bools
			if (a.TeamIsDead != b.TeamIsDead)
				return false;
			if (a.DeathCameraOn != b.DeathCameraOn)
				return false;
			if (a.m_UsePilots != b.m_UsePilots)
				return false;
			if (a.m_HadPilots != b.m_HadPilots)
				return false;
			if (a.m_UseLimits != b.m_UseLimits)
				return false;
			if (a.m_CPUUnitsCanService != b.m_CPUUnitsCanService)
				return false;
			if (a.m_CPUDoDispatch != b.m_CPUDoDispatch)
				return false;
			if (a.CommTowerAround != b.CommTowerAround)
				return false;
			if (a.PlayerWasPilot != b.PlayerWasPilot)
				return false;
			if (a.GiveBackWeapons != b.GiveBackWeapons)
				return false;
			if (a.PlayCommTowerSound != b.PlayCommTowerSound)
				return false;
			if (a.UseSiteCamera != b.UseSiteCamera)
				return false;
			if (a.m_Distance1Check != b.m_Distance1Check)
				return false;
			if (a.m_Distance2Check != b.m_Distance2Check)
				return false;
			if (a.SlfPlayingAnim != b.SlfPlayingAnim)
				return false;
			// Floats
			if (a.Distance1 != b.Distance1)
				return false;
			if (a.Distance2 != b.Distance2)
				return false;
			if (a.SLFAnimationFrames != b.SLFAnimationFrames)
				return false;
			if (a.m_CPUDayWreckerChance != b.m_CPUDayWreckerChance)
				return false;
			// Vector
			if ((a.PlayerSpawnPos.x != b.PlayerSpawnPos.x) || (a.PlayerSpawnPos.y != b.PlayerSpawnPos.y) || (a.PlayerSpawnPos.z != b.PlayerSpawnPos.z))
				return false;
			if ((a.ReinforcementPos.x != b.ReinforcementPos.x) || (a.ReinforcementPos.y != b.ReinforcementPos.y) || (a.ReinforcementPos.z != b.ReinforcementPos.z))
				return false;
			if ((a.GoalPos.x != b.GoalPos.x) || (a.GoalPos.y != b.GoalPos.y) || (a.GoalPos.z != b.GoalPos.z))
				return false;
			// Char
			if (a.TeamRace != b.TeamRace)
				return false;
			// Char arrays.
			if (_stricmp(a.PlayerName, b.PlayerName))
				return false;
			if (_stricmp(a.TeamAIPFiles, b.TeamAIPFiles))
				return false;
			for(int i = 0; i < MAX_HARDPOINTS; i++)
				if (_stricmp(a.PilotWeapon[i], b.PilotWeapon[i]))
					return false;
			if (a.RealPlayer != b.RealPlayer)
				return false;
			if (_stricmp(a.PlayerPilotODF, b.PlayerPilotODF))
				return false;

			return true;
		}
		*/
	};

	// Master Object Class filters.
	struct ObjectClass {
		Handle ObjectHandle; // The handle.
		unsigned int ODFNameHash; // FNV-1a Hash of the ODF Name.
		unsigned int ObjClassHash; // FNV-1a Hash of the ObjClass.
		int LastCommand; // This unit's current Command Int.
		int CategoryID; // Custom Class ID #, based on Enum CategoryType.
		bool CanObjectify; // Can this be objectified?
		bool CanObjectifyDeployed; // Can this be objectified while deployed?
		bool CanTeleport; // Can this object Teleport?
		bool ShowSkill; // Does this unit show it's skill level on it's name?
		int ShowSkillTeamFilter; // Team Filter for Skill Level Showing.
		int OriginalTeam; // Store it's original Team number for ST Mission code.
		int CurTeamNum; // Current Team num, stored to preseve calls to GetTeamNum. Updated once every tick.
	//	int SkillLevel; // Current Skill Level.
		float EngageRange; // Store it's engageRange.
//		bool IsAPCRefill; // Is it an APC Refill point?
//		float CollisionRadius; // Collision Radius.
		bool IsObjectified[MAX_TEAMS]; // Is it objectified? (per team)
		bool IsBeingJammed[MAX_TEAMS]; // Flag for if this object is being jammed or not. (per team)
		Vector Position; // It's current position.

		float HealthRate; // SupplyDepot Things.
		float AmmoRate;
		float ServiceRate;
		bool ServiceCraft;


		ObjectClass() { memset(this, 0, sizeof(ObjectClass)); };

		/*
		friend bool operator==(const ObjectClass &a, const ObjectClass &b)
		{
			if (a.ObjectHandle != b.ObjectHandle)
				return false;
			if (a.CanObjectify != b.CanObjectify)
				return false;
			if (a.CanObjectifyDeployed != b.CanObjectifyDeployed)
				return false;
			if (a.CanTeleport != b.CanTeleport)
				return false;
			if (a.OriginalTeam != b.OriginalTeam)
				return false;
			if (a.EngageRange != b.EngageRange)
				return false;
			if (memcmp(a.IsObjectified, b.IsObjectified, sizeof(bool)*MAX_TEAMS) != 0)
				return false;
			if (memcmp(a.IsBeingJammed, b.IsBeingJammed, sizeof(bool)*MAX_TEAMS) != 0)
				return false;
			return true;
		}
		*/
	};

private:
	static BZ1Helper *m_BZ1Helper;

	PUPMgr *m_PUPMgr;
	//TapAssignmentHelper *m_TapAssignmentHelper;
	ProductionClassHelper *m_ProductionClassHelper;
	RecycleHelper *m_RecycleHelper;
	PilotResourceHelper *m_PilotResourceHelper;
	BuildingHelper *m_BuildingHelper;
	APCHelper *m_APCHelper;
	HQCPHelper *m_HQCPHelper;
	LPadHelper *m_LPadHelper;
	PortalHelper *m_PortalHelper;
	AttacherHelper *m_AttacherHelper;
	ReplaceOnDeathHelper *m_ReplaceOnDeathHelper;
	UtilityShipHelper *m_UtilityShipHelper;
	PowerHelper *m_PowerHelper;
	DeployHelper *m_DeployHelper;
	FuryHelper *m_FuryHelper;
	LavaDamageHelper *m_LavaDamageHelper;
	ScrapHelper *m_ScrapHelper;
	DispatchHelper *m_DispatchHelper;
	SatelliteHelper *m_SatelliteHelper;
	LimiterHelper *m_LimiterHelper;
	SwitchClassHelper *m_SwitchClassHelper;
	AnchorClassHelper *m_AnchorClassHelper;
	AmmoDrainHelper *m_AmmoDrainHelper;
	UndeployHelper *m_UndeployHelper;
	QuakeHelper *m_QuakeHelper;
	GeyserHelper *m_GeyserHelper;
	AIWeaponHelper *m_AIWeaponHelper;
	AttacherWeaponHelper *m_AttacherWeaponHelper;
	MorphObjectHelper *m_MorphObjectHelper;
	ShieldHelper *m_ShieldHelper;
	EnergyArmorHelper *m_EnergyArmorHelper;
	JammerHelper *m_JammerHelper;
	CarrierHelper *m_CarrierHelper;
	TransportHelper *m_TransportHelper;
	DropShipHelper *m_DropShipHelper;
//	RecallMineHelper *m_RecallMineHelper;
	
	/*
	// Consolidate these arrays into one giant array, here in BZ1Helper.
	// Make associative data here? Or make cleanup code referance GetNumObjects / IsAround &RefHandle?
BuildingPower - Buildings
LPad - BarracksObjects, HasBarracks[Team]
PilotResource - PilotedShipObjects, PilotObjects, 
Portal - TeleportTargets
Power - PowerObjects
ProductionClass - CameraObjects
Scavenger - ScavengerObjects
TapAssignment - TapHostObjects 
Satellite - CommTowerObjects ?
*/

	// Private vars for the above structs.
	MapInfo mapInfo;
	
	// Team info var.
	TeamInfo teamInfo[MAX_TEAMS];

	struct GravityInfo {
		float TranPeriod;
		float OnPeriod;
		//float OffPeriod;
		float Gravitation;
		float Gravity;
		int OnCounter;

		GravityInfo() { memset(this, 0, sizeof(GravityInfo)); };
	};
	GravityInfo gravityInfo[MAX_GRAVITY_TYPES];

	// Master Handle List. Keeps track of all game objects.
	std::vector<ObjectClass> HandleList;
	static bool ShouldRemoveObject(const ObjectClass &entry) { return !IsAround(entry.ObjectHandle); }

	/*
	// Cloud stuff.
	struct CloudTile { 
		int CloudCount; // How many clouds per tile.
		Handle CloudHandle[MAX_CLOUD_OBJECTS]; // Max number of Clouds.
		bool Active; // Is this tile active?

		CloudTile() { memset(this, 0, sizeof(CloudTile)); };
	};
	std::vector<CloudTile> CloudTileList;
	*/

	// Align Terrain Replace objects.
	struct AlignTerrainReplaceClass {
		Handle AlignHandle; // The handle.
		Vector NormalVec; // Normals for TerrainReplace.

		AlignTerrainReplaceClass() { memset(this, 0, sizeof(AlignTerrainReplaceClass)); };

		/*
		friend bool operator==(const AlignTerrainReplaceClass &a, const AlignTerrainReplaceClass &b)
		{
			if (a.AlignHandle != b.AlignHandle)
				return false;
			if (a.NormalVec.x != b.NormalVec.x)
				return false;
			if (a.NormalVec.y != b.NormalVec.y)
				return false;
			if (a.NormalVec.z != b.NormalVec.z)
				return false;
			return true;
		}
		*/
	};
	std::vector<AlignTerrainReplaceClass> AlignTerrainReplaceList;
	static bool ShouldRemoveAlignTerrainReplace(const AlignTerrainReplaceClass &entry) { return !IsAround(entry.AlignHandle); }

	// Delete on Load objects.
	struct DeleteAfterDelayClass {
		Handle DeleteAfterDelayHandle; // The handle.
		int TurnsRemaining; // How many turns left until it's deletion.

		DeleteAfterDelayClass() { memset(this, 0, sizeof(DeleteAfterDelayClass)); };

		/*
		friend bool operator==(const DeleteAfterDelayClass &a, const DeleteAfterDelayClass &b)
		{
			if (a.DeleteAfterDelayHandle != b.DeleteAfterDelayHandle)
				return false;
			if (a.TurnsRemaining != b.TurnsRemaining)
				return false;
			return true;
		}
		*/
	};
	std::vector<DeleteAfterDelayClass> DeleteAfterDelayList;
	static bool ShouldRemoveDeleteAfterDelay(const DeleteAfterDelayClass &entry) { return !IsAround(entry.DeleteAfterDelayHandle); }

	/*
	// AudioMessage stored playlist, used for RepeatAudioMessage and IsAudioMessagePlaying. Must use MissionAudioMessage() to add it to this list.
	struct MissionAudioClass {
		int MissionAudioHandle; // The handle.
		int MissionAudioFile[16]; // RUDE HACK! Storing a char string as int's. 16 == char [64];

		MissionAudioClass() { memset(this, 0, sizeof(MissionAudioClass)); };
	};
	std::vector<MissionAudioClass> MissionAudioList;
	static bool ShouldRemoveMissionAudio(const MissionAudioClass &entry) { return IsAudioMessageDone(entry.MissionAudioHandle); }
	*/

int i_first,
	// MP Scrap Multiplier.
	m_ScrapMultiplier, 
	// MP Max Lives.
	m_MaxLives,
	// Sun Sprite index. Used in Sun moving sky? -- Code Disabled for now... -GBD.
	SunSpriteIndex, 

	m_LastObjectListCount, // Counts the number of ObjectList items at the end of each tick.

	// Reinforcement stuff.
	m_ReinforcementTime, // Interval of arrival.
	m_ReinforcementAmount, // Amount per arrival.

	// Cloud stuff, cloud placing is now iterated over several turns.
	CurCloudType, CurCloudCount,

	MissionAudioHandle, // The handle.
	MissionAudioFile[16], // RUDE HACK! Storing a char string as int's. 16 == char [64];

	i_last;

/*
Handle h_first,

	h_last;
*/

bool b_first,
	m_DidInit, // Flag for initial setup. Internal only. -GBD
	m_DidSetup, // Flag for onload setup. Internal only. -GBD
	m_CloudsDone, // Flag if clouds are finished or not. -GBD
	m_IsFriendlyFireOn, // Global network var. Moved here to reduce duplicate code. -GBD
	m_DisableSiteCamMP, // Bool if to use SiteCam in MP.

	m_BuildPools, // Flag to add BZ2 Pools.
	m_SetupPools, // Flag if we already have BZ2 Pools.

	// AIP stuff.
	m_CPUUseDayWreckers, // Flag for if the AI can use day wreckers or not.

	StopAddObject, // RUDE HACK!!!

	b_last;

float f_first,
	// Sun Sprite stuff.
	SunAngle, 
	SunPeriod, 

	// MP Damage Multiplier.
	m_DamageMultiplier,

	// Reinforcements. 
	m_ReinforcementMinRadius, // Map set min radius for reinfrocement landing.
	m_ReinforcementMaxRadius, // Map set max radius for reinforcement landing.
	m_ReinforcementMaxCliff, // Map set height differance tollerance for cliff detection.

	// Stuff for Cloud Spawning.
	CurTileX, CurTileZ, 

	f_last;

	bool *b_array;
	int b_count;

	float *f_array;
	int f_count;

	/*
	int *h_array;
	int h_count;
	*/

	int *i_array;
	int i_count;

public:
	BZ1Helper(void);
	virtual ~BZ1Helper();
	static BZ1Helper &Get() { return *m_BZ1Helper; }

	// Normal game functions.
	bool Save(bool missionSave);
	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	void AddObject(Handle h);
	void DeleteObject(Handle h);
	void Execute(void);

	// Setup Functions.
	void Init(void);
	void Setup(void);

	virtual PreSnipeReturnCodes PreSnipe(const int curWorld, Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);
	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);
	virtual PreGetInReturnCodes PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle);

	// Note: BZ1 made clouds around the player in a 3x3 TileSize grid, with CloudCount # of clouds placed randomly in each Tile. It then moved them around when 
	// the player got into the next tile area.
	// Clouds.
	void SetupClouds(void);
//	void MoveClouds(void);

// Public Variables. These are saved across network.
	static int m_GameTPS; // Store it.
	static int m_TurnCounter; // Count each game tick. -GBD
	static int TotalMapObjects; // Save this too.
	static Vector Where; // Cosntant vector used for off-map objects. (ScrapHolds and Limiters)
	static char WorldSetting; // What world are we on?
	static int m_GameType; // Game Type Variable.
	static int m_GameVersion; // The version of the game, should be the same across all machines.
	static float m_ModVersion; // The version of the game, should be the same across all machines.

// Local variables not saved.
	static bool Logging; // Logging flag, not saved, only used for console/log file logging.

	// BZ1 Interface 2ndary Health/Ammo bars.
	static float HullGauge2;
	static float AmmoGauge2;
	static float AuxGauge;
	static bool HullGaugeSet;
	static bool AmmoGaugeSet;
	static bool AuxGaugeSet;

	// Path data called once from BZ1 Helper. Shared across all DLLs. NOT Saved across network, must be fetched each game Start/Load.
	int AIpathCount;
	const char **AIpathNames;
	// Saved list of paths.
	std::vector<const char*> MLayPathList[MAX_TEAMS];
	std::vector<const char*> PatrolPathList[MAX_TEAMS];


	// Team Acessories
	const TeamInfo &GetTeamInfo(int Team) const { return teamInfo[Team]; } // Read only version.
	TeamInfo &RefTeamInfo(int Team) { return teamInfo[Team]; }

	// Map Accessors, Read only from data retrieved during initial load.
	const MapInfo &GetMapInfo() const { return mapInfo; }
	MapInfo &RefMapInfo() { return mapInfo; }

	// ST ScrapMult.
	int &RefScrapMult() { return m_ScrapMultiplier; }

	// ST DamageMult.
	float &RefDamageMult() { return m_DamageMultiplier; }

	// ST/DM Max Lives.
	int &RefMaxLives() { return m_MaxLives; }

	// IA/ST Reinforcement stuff.
	int &RefReinforcementTime() {return m_ReinforcementTime;}
	int &RefReinforcementAmount() {return m_ReinforcementAmount;}

	// Flags for BZ2 style Pools.
	bool &RefBuildPools() { return m_BuildPools; }
	bool &RefSetupPools() { return m_SetupPools; }

	// Accessor for this flag.
	bool &RefDisableSiteCamMP() { return m_DisableSiteCamMP; }

	// Rude Hack!
	bool &RefStopAddObject() { return StopAddObject; }

	// Master Object List accessors.
	int GetNumHandles() { return HandleList.size(); }
	const ObjectClass &GetHandleInfo(int index) const { return HandleList[index]; } // Read only version.
	ObjectClass &RefHandleInfo(int index) { return HandleList[index]; }

	/*
	Handle &RefObjectHandle(const int index) { return HandleList[index].ObjectHandle; }
	bool &RefHQCPObjectified(const int index, int team) { return HandleList[index].IsObjectified[team]; }
	bool GetCanHQCPObjectify(const int index) { return HandleList[index].CanObjectify; }
	bool GetCanHQCPObjectifyDeployed(const int index) { return HandleList[index].CanObjectifyDeployed; }
	bool GetCanTeleport(const int index) { return HandleList[index].CanTeleport; }
	bool &RefIsObjectJammed(const int index, int team) { return HandleList[index].IsBeingJammed[team]; }
	int &RefObjectOriginalTeam(const int index) { return HandleList[index].OriginalTeam; }
	float GetObjectEngageRange(const int index) { return HandleList[index].EngageRange; }
//	int GetObjectCID(int index) { return HandleList[index].ClassID; }
	//bool GetAPCRefill(int index) { return HandleList[index].IsAPCRefill; }
//	*/

	// Delete on Load.
//	int GetNumDeleteObjects() { return DeleteAfterDelayList.size(); }
//	Handle &RefDeleteObjectHandle(const int index) { return DeleteAfterDelayList[index].DeleteAfterDelayHandle; }

	// Gets the number of objects the last tick.
	int GetNumObjectsLastTick() { return m_LastObjectListCount; }

	// Delete this item the next Execute Step.
	void DeleteAfterDelay(const Handle h, const float Delay = 0.0f);

	// Spawns a Transport, of the specified ODF, Team, Location, with the downtime, set amount of scrap, pilots, and units to spawn, also optionally to spawn powerups.
	Handle BuildTransportObject(const char *ODF, const int Team, const Matrix Position, const float DownTime = 0.0f, const float HeightOffset = 0.0f, const int PilotAmount = 0, const int ScrapAmount = 0, const int UnitsAmount = 0, const bool SpawnPowerups = false, const bool Objectify = false, const bool AddToGroup = false);
	Handle BuildTransportObject(const char *ODF, const int Team, const Handle him, const float DownTime = 0.0f, const float HeightOffset = 0.0f, const int PilotAmount = 0, const int ScrapAmount = 0, const int UnitsAmount = 0, const bool SpawnPowerups = false, const bool Objectify = false, const bool AddToGroup = false);
	Handle BuildTransportObject(const char *ODF, const int Team, const Name APath, const float DownTime = 0.0f, const float HeightOffset = 0.0f, const int PilotAmount = 0, const int ScrapAmount = 0, const int UnitsAmount = 0, const bool SpawnPowerups = false, const bool Objectify = false, const bool AddToGroup = false);
	Handle BuildTransportObject(const char *ODF, const int Team, const AiPath *APath, const float DownTime = 0.0f, const float HeightOffset = 0.0f, const int PilotAmount = 0, const int ScrapAmount = 0, const int UnitsAmount = 0, const bool SpawnPowerups = false, const bool Objectify = false, const bool AddToGroup = false);
	Handle BuildTransportObject(const char *ODF, const int Team, const Vector &pos, const float DownTime = 0.0f, const float HeightOffset = 0.0f, const int PilotAmount = 0, const int ScrapAmount = 0, const int UnitsAmount = 0, const bool SpawnPowerups = false, const bool Objectify = false, const bool AddToGroup = false);

	// These are various exterior functions that allow missions to see/control some things. 
	// These are Pilot Resource interface functions, they function like GetScrap, SetScrap, AddScrap. 
	// To call these from an exterior DLL you must #include BZ1Helper.h.
	int GetTurnCount(void) {return m_TurnCounter;}

	// Pilot Reousrce Functions. For use by mission DLLs.
	int AddPilot(const int Team, const int Value) {return teamInfo[Team].CurrPilots += Value;}
	int SetPilot(const int Team, const int Value) {return teamInfo[Team].CurrPilots = Value;}
	int GetPilot(const int Team) {return teamInfo[Team].CurrPilots;}
	int GetMaxPilot(const int Team) {return teamInfo[Team].MaxPilots;}
	int AddMaxPilot(const int Team, const int Value) {return teamInfo[Team].MaxPilots += Value;}
	int SetMaxPilot(const int Team, const int Value) {return teamInfo[Team].MaxPilots = Value;}

	// BZ1 Interface Health/Ammo Gauge2 settings.
	void SetHullGauge(const float Amount) { HullGauge2 = Amount; HullGaugeSet = true; }
	void SetAmmoGauge(const float Amount) { AmmoGauge2 = Amount; AmmoGaugeSet = true; }
	void SetAuxGauge(const float Amount) { AuxGauge = Amount; AuxGaugeSet = true; }

	// Production Unit Handles. BZC Production units can undeploy, meaning their handle is lost. Use this in SP mission scripts to keep track of production units.
	Handle GetRecy(const int Team) { return teamInfo[Team].Recy; }
	Handle GetMuf(const int Team) { return teamInfo[Team].Muf; }
	Handle GetSlf(const int Team) { return teamInfo[Team].Slf; }
	Handle GetCnst(const int Team) { return teamInfo[Team].Cnst; }

	// Get/Set if a team is using pilots. 
	bool GetUsePilots(const int Team) { return teamInfo[Team].m_UsePilots; }
	void SetUsePilots(const int Team, const bool Value) { teamInfo[Team].m_UsePilots = Value; }

	// Sets an AIP file and uses "Pilot resource cheat" numbers, and advanced Dispatch functions. Returns true if successful, false if the AIP file doesn't exist.
	bool SetAIPFile(const char* Value, const int Team = -1);
	bool UpdateTeamAIP(const char* AIPFile, const int Team);

	// Adds a unit to a BZ1 Unit Dispatcher. Defaults to requiring an AIP to be running, can be overridden with SkipAIPCheck. If CloakPriority is 1, Dispatcher 
	// doesnt change cloak state. If IsFleer is true, unit will flee unless the ODF says it can't. If CloakNow is true, it will cloak ASAP. If Hunt is true, unit will 
	// begin hunting. If CanGetService is true, it will get ammo on its own (interrupting previous orders/dispatch duties). Returns true if successful, false if 
	// an invalid handle or no BZC AIP is running.
	bool AddToDispatch(const Handle h, const float IdleTime = 15.0f, const bool SkipAIPCheck = false, const int CloakPriority = 0, const bool CanFlee = true, 
		const bool CloakNow = false, const bool Hunt = false, const bool CanGetService = false);
	
	// Cloak Commands, used to interface with DispatchHelper's Dispatch code.
	void Cloak(const Handle h, const int Priority = 0);
	void Decloak(const Handle h, const int Priority = 0);
	bool IsCloaked(const Handle h);
	// Special SP ability to disable cloak. Returns the handle if it switched the objects, or the handle passed in if it did nothing.
	Handle SetCloakAllowed(const Handle h, const bool IsAllowed, const bool PlaySound = false);
	// DLL Hunt, passes units from mission scripts into Dispatcher code. //!!-- Can be replaced by Hunt command in 1.3.6.7+. -GBD
	void DLLHunt(const Handle h, const int Priority = 0);
	// DLL command to set a unit's UseService status.
	void SetUseService(const Handle h, const bool Service = true);

	// Gets a random Escort Target, based on escort priority. By default, Production units are High, Scavs are Medium, APCs/TUGs/Minelayers are Low.
	Handle GetEscortTarget(const int Team = -1, const int TeamFilter = 0);

	// DLL version for StartEarthQuake/UpdateEarthQuake/StopEarthQuake. These alter a minimum quake used by BZC's QuakeHelper Day Wreckers.
	void DLLStartEarthQuake(const float Magnitude);
	void DLLUpdateEarthQuake(const float Magnitude);
	void DLLStopEarthQuake(void);

	// Sets the channel of a Portal.
	void SetPortalChannel(const Handle h, const int Channel);
	int GetPortalChannel(const Handle h);
	// Portal Effect controllers. FxNumber is which EffectODF to make, 0, 1, or 2. Type: 0 = Exit (Close Fx), 1 = Continuous On (Open Fx), 2 = Build (Open then Close Fx)
	void SetPortalEffectStart(const Handle h, const int FxNumber = 0, const int Type = 0);
	void SetPortalEffectOn(const Handle h, const int FxNumber = 0, const int Type = 0);
	void SetPortalEffectEnd(const Handle h, const int FxNumber = 0, const int Type = 0);
	bool IsPortalEffectActive(const Handle h);

	// Sets a Pilot to be independent or not from DLL control. (Flips actual Game independence to the opposite)
	void SetPilotIndependence(const Handle h, const bool Independence);

	// Returns true if a building is Powered.
	bool IsBuildingPowered(const Handle h);
	// Sets a building's power state. -1 = Autodetermine, 0 = Off, 1 = On. (Only effects above function and related DLL code)
	void SetBuildingPowered(const Handle h, const int Powered);

	// Sets the object to Replace itself with another object upon it's death. Optional team, height offset, and if there's an explosion or not. StayTap is a flag as to if this should stay as a tap. MinReplaceTime is an optional timer, in seconds from now, in how long to replace the object. MaxReplaceTime and RandomizeTime are flags that enable an optional random time between min and max time.
	void ReplaceOnDeath(const Handle h, const char *ReplaceODF = NULL, const int Team = -1, const float HeightOffset = 0.0, const bool DoExplode = false, const bool StayTap = true, const float MinReplaceTime = 0.0f, const float MaxReplaceTime = 0.0f, const bool RandomizeTime = false);

	// Replaces an object with another object, retaining as much information about it as possible, with optional team switching, height offset, replacing of 
	// weapons with what it currently has, group, and current command info (may not work for all commands).
	Handle ReplaceObject(const Handle h, const char *ODF = NULL, const int Team = -1, const float HeightOffset = 0.0f, const int Empty = -1, 
		const bool RestoreWeapons = true, const int Group = -1, const bool KeepCommand = true, const int NewCommand = -1, const Handle NewWho = -1);

	// Gets a random object on the map, with filtering options. IncludeItemsAddedThisTurn is a bool for if the check includes items added this game turn. 
	// "Team" is an optional Team to compare against with TeamFilter. TeamFilter is 0 = All, 1 = SameTeam, 2 = Allies, 4 = Enemies.
	// CategoryMask is a bitmask for: 0 = All, 1 = Vehicle, 2 = Building, 4 = Person, 8 = Powerup, 16 = Weapon, 32 = Scrap, 64 = Other. 
	// MatchMask is a bitmask for match type. 0 = Any, 1 = Match CategoryMask, 2 = Match ObjClass, 4 = Match ODF Name. ObjClass is a string returned by Get_GOClass, i.e. CLASS_WINGMAN. ODFName is the ODFName, without the .odf extension.
	Handle GetRandomObject(const int Team = -1, const int TeamFilter = 0, const int CategoryMask = 0, const bool IncludeItemsAddedThisTurn = false, const bool IgnoreInvincible = true, const int MatchMask = 0, const char *ObjClass = NULL, const char *ODFName = NULL);

	// Gets the number of objects on a team, with filtering options. IncludeItemsAddedThisTurn is a bool for if the check includes items added this game turn. 
	// "Team" is an optional Team to compare against with TeamFilter. TeamFilter is 0 = All, 1 = SameTeam, 2 = Allies, 4 = Enemies.
	// CategoryMask is a bitmask for: 0 = All, 1 = Vehicle, 2 = Building, 4 = Person, 8 = Powerup, 16 = Weapon, 32 = Scrap, 64 = Other. 
	// MatchMask is a bitmask for match type. 0 = Any, 1 = Match CategoryMask, 2 = Match ObjClass, 4 = Match ODF Name. ObjClass is a string returned by Get_GOClass, i.e. CLASS_WINGMAN. ODFName is the ODFName, without the .odf extension.
	int GetNumObjects(const int Team = -1, const int TeamFilter = 0, const int CategoryMask = 0, const bool IncludeItemsAddedThisTurn = false, const bool IgnoreInvincible = true, const int MatchMask = 0, const char *ObjClass = NULL, const char *ODFName = NULL);

	// Gets the nearest object to a Vector position, with filtering options. "Team" is an optional Team to compare against with TeamFilter. 
	// "Team" is an optional Team to compare against with TeamFilter. TeamFilter is 0 = All, 1 = SameTeam, 2 = Allies, 4 = Enemies.
	// CategoryMask is a bitmask for: 0 = All, 1 = Vehicle, 2 = Building, 4 = Person, 8 = Powerup, 16 = Weapon, 32 = Scrap, 64 = Other. 
	// MatchMask is a bitmask for match type. 0 = Any, 1 = Match CategoryMask, 2 = Match ObjClass, 4 = Match ODF Name. ObjClass is a string returned by Get_GOClass, i.e. CLASS_WINGMAN. ODFName is the ODFName, without the .odf extension.
	Handle GetNearestObject(const Vector Position, const float MaxRange = 0.0f, const int Team = -1, const int TeamFilter = 0, const int CategoryMask = 0, const bool IgnoreInvincible = true, const Handle Me = 0, const int MatchMask = 0, const char *ObjClass = NULL, const char *ODFName = NULL);
	// Version that takes a Path and point, Handle, and Matrix position.
	Handle GetNearestObject(const Name Path, const int Point = 0, const float MaxRange = 0.0f, const int Team = -1, const int TeamFilter = 0, int CategoryMask = 0, const bool IgnoreInvincible = true, const int MatchMask = 0, const char *ObjClass = NULL, const char *ODFName = NULL);
	Handle GetNearestObject(const Handle h, const float MaxRange = 0.0f, const int Team = -1, const int TeamFilter = 0, const int CategoryMask = 0, const bool IgnoreInvincible = true, const int MatchMask = 0, const char *ObjClass = NULL, const char *ODFName = NULL);
	Handle GetNearestObject(const Matrix Position, const float MaxRange = 0.0f, const int Team = -1, const int TeamFilter = 0, const int CategoryMask = 0, const bool IgnoreInvincible = true, const int MatchMask = 0, const char *ObjClass = NULL, const char *ODFName = NULL);

	// BuildAngleObject functions, Builds an object, with optional label (checks to see that it doesnt exist already), optionally at the desired 
	// Angle(in degrees), in the desired Group, spawn Empty, Deploy on build, HeightOffset, and if they're spread out or not (Defaults to true for paths, only works on non buildings)
	Handle BuildAngleObject(const char* ODF, const int Team, const Matrix &mat, const char* Label = NULL, const float Degrees = 0.0f, const int Group = -1, const bool Empty = false, const bool StartDeploy = false, const float HeightOffset = 0.0, const bool SpreadOut = false, const int AlignToTerrain = -1, const bool AlignUseWater = false);
	Handle BuildAngleObject(const char* ODF, const int Team, const Handle him, const char* Label = NULL, const float Degrees = 0.0f, const int Group = -1, const bool Empty = false, const bool StartDeploy = false, const float HeightOffset = 0.0, const bool SpreadOut = false, const int AlignToTerrain = -1, const bool AlignUseWater = false);
	Handle BuildAngleObject(const char* ODF, const int Team, const Name APath, const char* Label = NULL, const float Degrees = 0.0f, const int Group = -1, const bool Empty = false, const bool StartDeploy = false, const float HeightOffset = 0.0, const bool SpreadOut = true, const int AlignToTerrain = -1, const bool AlignUseWater = false);
	Handle BuildAngleObject(const char* ODF, const int Team, const AiPath *APath, const char* Label = NULL, const float Degrees = 0.0f, const int Group = -1, const bool Empty = false, const bool StartDeploy = false, const float HeightOffset = 0.0, const bool SpreadOut = true, const int AlignToTerrain = -1, const bool AlignUseWater = false);
	Handle BuildAngleObject(const char* ODF, const int Team, const Vector &pos, const char* Label = NULL, const float Degrees = 0.0f, const int Group = -1, const bool Empty = false, const bool StartDeploy = false, const float HeightOffset = 0.0, const bool SpreadOut = false, const int AlignToTerrain = -1, const bool AlignUseWater = false);

	// Sets an AIP via BZC's SetAIPFile command, with proper formatting. Used by ST/IA DLLs to format the correct AIP File name.
	void SetCPUPlan(const char *AIPBaseName, const char Race, const int Team);

	// Rude Hack to add ability to RepeatAudioMessage via Script Command, Saves the last audiomessage played by thi command.
	int MissionAudioMessage(const char* msg); //{ strcpy_s((char *)MissionAudioFile, sizeof(MissionAudioFile), msg); MissionAudioHandle = AudioMessage(msg); return MissionAudioHandle; }
	//int MissionAudioMessage(const char* msg){ strcpy_s((char *)MissionAudioFile, sizeof(MissionAudioFile), msg); MissionAudioHandle = StartAudio3D(msg, GetPlayerHandle(), AUDIO_CAT_MISSION_DLG); return MissionAudioHandle; }
	// Repeats the last audio message played by MissionAudioMessage().
	int RepeatAudioMessage(void); //{ if(MissionAudioFile[0]) { MissionAudioHandle = AudioMessage((char*)(&MissionAudioFile[0])); return MissionAudioHandle; } else { return 0; } }
	//int RepeatAudioMessage(void) { return StartAudio3D((char*)(&MissionAudioFile[0]), GetPlayerHandle(), AUDIO_CAT_MISSION_DLG); }
	// IsAudioMessagePlaying
	inline bool IsAudioMessagePlaying(void) { return !IsAudioMessageDone(MissionAudioHandle); }

	/*
	// Rude Hack to add ability to RepeatAudioMessage via Script Command, Saves the last audiomessage played by thi command.
	int MissionAudioMessage(const char* msg);
	// Repeats the last audio message played by MissionAudioMessage().
	int RepeatAudioMessage(void);
	// IsAudioMessagePlaying
	inline bool IsAudioMessagePlaying(void) { return !MissionAudioList.empty(); }
	*/

	// BZ1 Function for backwards compatability.
	void GetRidOfSomeScrap(const int ScrapLimit = 1000); // Upped scrap limit from BZ1's 300, as BZ2 can have 4096 entities instead of 512.
	// Clears Scrap around a specified position.
	void ClearScrapAround(Dist d, const Vector &pos, const float Percent = 1.0f);
	void ClearScrapAround(Dist d, const Matrix &mat, const float Percent = 1.0f) { return ClearScrapAround(d, mat.posit, Percent); }
	void ClearScrapAround(Dist d, const Handle h, const float Percent = 1.0f) { return ClearScrapAround(d, GetPosition(h), Percent); }
	void ClearScrapAround(Dist d, const Name path, const int point = 0, const float Percent = 1.0f) { return ClearScrapAround(d, GetVectorFromPath(path, point), Percent); }

	// Determine if there are any buildings between Handle 1 and Handle 2, within the Width Padding, taking into account the Building's CollisionRadius odf 
	// setting. Returns the handle of what is blocking the two.
	Handle IsBlockedByBuilding(const Handle h1, const Handle h2, const float WidthPadding, const Vector FrontA = Vector(0, 0, 0), const Vector FrontB = Vector(0, 0, 0), const bool IgnoreInvincible = true);

	// Saves all the objects on a map to a file.
	void SaveMapObjectsToFile(const char *file = NULL);
	// Loads all the objects on a map from a file saved with the above.
	void LoadMapObjectsFromFile(const char *file = NULL);

private:

	// Controls BZ1 pilots or BZ2 Power min/max gauges. 
	void BZ1Interface(void);

	// Rotating Sun Sprite with sun.angle. Only works in EDITOR mode... :(
	void DayNightCycle(void);

	// Run environmental effects, gravity, water, sky, hazards, etc.
	void EnvironmentalSystems(void);

	// Reinforcement code. Now can be set from anywhere. :)
	void ExecuteReinforcements(void);

	// Life management code.
	void BZ1Lives(void);
};

// Handy Time Conversion Stuffs. Done here because Game_Version and m_GameTPS are saved here.
inline int ConvertToTurns(const float Seconds) { if(BZ1Helper::m_GameVersion >= v13PB7) return SecondsToTurns(Seconds); else return int(Seconds * BZ1Helper::m_GameTPS); } // SecondsToTurns(Seconds); }
inline int ConvertToTurns(const int Seconds) { if(BZ1Helper::m_GameVersion >= v13PB7) return SecondsToTurns(float(Seconds)); else return Seconds * BZ1Helper::m_GameTPS; }
inline float ConvertToSeconds(const int Turns)  { if(BZ1Helper::m_GameVersion >= v13PB7) return TurnsToSeconds(Turns); else return float(Turns / BZ1Helper::m_GameTPS); } // TurnsToSeconds(Turns); }
inline float ConvertToSeconds(const float Turns) { if(BZ1Helper::m_GameVersion >= v13PB7) return TurnsToSeconds(int(Turns)); else return Turns / BZ1Helper::m_GameTPS; }


// Template to remove an entry, not used because multiple handles per class.
//template<typename T> bool ShouldRemoveEntry(const T &entry) { return !IsAround(entry.handle); }

#endif


// Notes from various people.
/*
Boolean Expression Simplifcation rules, suggested by John "Nielk1" Klein.

Where ! is not for the next term
+ is or
* is and
0 is false
1 is true

Rules:
A * 0 = 0
A * 1 = A
A + 0 = A
A + 1 = 1
A * A = A
A + A = A
A * !A = 0
A + !A = 1
!!A = A
A + (!A * B) = A + B

DeMorgan's Theorem:
!(A * B) = !A + !B
!(A+B) = !A * !B
*/