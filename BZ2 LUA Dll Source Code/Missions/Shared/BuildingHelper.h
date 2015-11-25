#ifndef _BuildingHelper_
#define _BuildingHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class BuildingHelper
{
public:

	// The Building Object class.
	struct BuildingClass {
		Handle BuildingObject; // This Building Object.
		Handle BuildingTap[MAX_TAPS]; // Each tap handle.
		unsigned int ODFNameHash; // FNV-1a Hash of the ODF Name.
		unsigned int ObjClassHash; // FNV-1a Hash of the ObjClass.
		int PowerType; // Bitmask for Power Type. 0 = None, 1 = BZ1 Power, 2 = BZ2 Power, 4 = Lung power.
		bool IsPowered; // Is it currently powered?
		bool IsAnimated; // Is it Animated?
		bool PlayAnim; // Is it playing its animation?
		int TapNumber; // Number of Taps this building has.
		bool TapAnimated[MAX_TAPS]; // Is Each Tap animated?
		bool PlayTapAnim[MAX_TAPS]; // Is each Tap playing its animation?
		float MaxAnimationFrame[MAX_TAPS+1]; // How many frames the Loop animation is. 0 = Host object, rest for 8 taps.
		int PowerState; // This building's default power state.
		int PowerMask; // Bitmask for what bits to look at.
		int PowerMatch; // Bitmask for what the bits need to be.
		int PowerProvide; // Bitmask for what kind of power this provides.
		int CurTeamNum; // Current Team number, used in place of GetTeamNum. Updated once per tick.
		bool CanAcceptPowerups; // Can this building accept powerups?
		bool CanCollide; // Can it be collideded?
		bool CanBeIdleVictim; // Can this be an idle victim of dispatch?
		bool IsAPCRefill; // Is it an APC Refill point?
		bool IsRecyclePoint; // Is this a recycle point?
		float CollisionRadius; // Collision Radius.
		bool GunTowerOff; // If it's a guntower, flag for swithcing on/off.
		Vector Position; // It's current position. (NOT updated, since buildings should never move)
		Matrix TapOffsetMatrix[MAX_TAPS]; // Offset for each Tap.
		Vector BuildingFront; // Front vector for building.

		BuildingClass() { memset(this, 0, sizeof(BuildingClass)); };
	};

private:

	static BuildingHelper *m_BuildingHelper;

	// The building list.
	std::vector<BuildingClass> BuildingList;
	static bool ShouldRemoveBuilding(const BuildingClass &entry) { return !IsAround(entry.BuildingObject); }

	struct RepairPowerupClass {
		Handle RepairPowerupObject; // This Object.
		long HealthUp; // How much it repairs.
		char SoundPickup[MAX_ODF_LENGTH]; // Pickup Sound.
		char SoundReject[MAX_ODF_LENGTH]; // Reject Sound.
		int DelayTime; // Timer for being able to be picked up.

		RepairPowerupClass() { memset(this, 0, sizeof(RepairPowerupClass)); };
	};
	std::vector<RepairPowerupClass> RepairPowerupList;
	static bool ShouldRemoveRepairPowerup(const RepairPowerupClass &entry) { return !IsAround(entry.RepairPowerupObject); }

public:
	BuildingHelper();
	~BuildingHelper();
	static BuildingHelper &Get() { return *m_BuildingHelper; }

	int GetNumBuildingObjects() { return BuildingList.size(); }
	const BuildingClass &GetBuildingInfo(int index) const { return BuildingList[index]; } // Read only version.

	// Is this building powered?
	bool IsBuildingPowered(const Handle h);
	
	// Powered state: -1 = Autodetermine, 0 = Off, 1 = On.
	void SetBuildingPowered(const Handle h, const int PoweredState = -1);

	void AddBuildingObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int ODFNameHash, const int ObjClassHash, const int BZCClassHash);
	void AddRepairPowerupObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
