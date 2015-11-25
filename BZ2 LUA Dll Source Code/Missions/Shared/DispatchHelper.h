#ifndef _DispatchHelper_
#define _DispatchHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

// Dispatch Types.
enum DispatchType 
{
	DispatchType_None,
	DispatchType_Turret,
	DispatchType_Patrol,
	DispatchType_Minelayer,
	DispatchType_Hunter,
	DispatchType_Escort,
	DispatchType_Howitzer,
	//DispatchType_Service,
	//DispatchType_Scavenger,
};

// Dispatch States.
enum DispatchState 
{
	DispatchState_Idle,
	DispatchState_Goto,
	DispatchState_Flee,
	DispatchState_Reload,
	DispatchState_Repair,
	//DispatchState_Defend,
	//DispatchState_Scavenge, // Scavenger manual code? Eeeesh.
};

class DispatchHelper
{
private:

	static DispatchHelper *m_DispatchHelper;

	struct DispatchClass {
		Handle DispatchObject; // This Dispatch Object.
		Handle DispatchTarget; // Handle its Targeting.
		Handle FleeTarget; // Fleeing unit's Target.
		Vector DispatchSpot; // Saved vector for it's designated location.

		int DispatchType; // 0 = None, 1 = Turret, 2 = Patrol, 3 = Minelayer, 4 = Hunter, 5 = Escort.
		int DispatchState; // 0 = Idle, 1 = Goto, 2 = Flee, 3 = Reloading, 4 = Repairing, // 5 = Defend,
		bool SkipAIPCheck; // Does this unit work even without an AIP running?

		bool IsCloaker; // Is it a Cloaker?
		bool IsMorpher; // Is it a Morpher?

		bool CanFlee; // can this unit flee?
		bool CanGetReload; // Can this unit go get ammo on it's own?
		bool CanGetRepair; // Can this unit get repairs on its own?

		bool MorphIsAssault; // Does morph switch to attack IsAssault units?
		bool IsCloaked; // Is it cloaked?
		bool CloakLocked; // Is the cloak locked?
		bool CloakNextTick; // Flag to cloak this unit next execute, reguardless of AIP/auto cloaking.
		bool DecloakNextTick; // Flag to cloak this unit next execute, reguardless of AIP/auto cloaking.

		int IdleTime; // Idle time to wait before dispatching.
		int CloakIdleTime; // Idle time for cloaking.
		int DeployTimer; // Timer for seeing if this unit is finished deploying. m_TurnCounter+TimeDeploy.

		float RandomDist; // Random distance to stop from.
		float EngageRange; // This unit's EngageRange, to see if enemies are nearby.
		float TimeDeploy; // Time to Deploy, used for calculating cloak timing.
		float CloakChance[NUM_CMD]; // Cloak Chances, per AI Command.

		float FleeHealthRatio; // Flee Health Lost Ratio.

		float GetRepairRatio; // Ratio at which it goes to get repair.
		float ServiceChance; // % Chance this unit will service.
		long MinFireAmmo; // Minimum ammo needed to fire it's current weapons.

		int Command; // What its current command is.
		int FleeHealthValue; // Health value since last Flee exited.
		int CloakPriority; // Priority for Cloaking/Hunting.

		/*
		bool IsTurret; // Is it a Turret?
		bool IsPatrol; // Is it a Patrol?
		bool IsMinelayer; // Is it a Minelayer?
		bool IsHunter; // Is it an AI Hunt ship?
		bool IsHowitzer; // Is this Artillary/Howitzer?
		//bool IsServicer; // Is this unit a Service Truck?
		bool IsEscorting; // Is this unit currently Escorting something?

		bool IsDispatched; // Has it been dispatched?
		bool IsEnRoute; // Is it en route?
		bool IsInPosition; // Has it arrived?
		bool IsFleeing; // Is it fleeing?
		bool IsReloading; // Is it reloading?
		bool IsRepairing; // Is it repairing?
		int Timeout; // Timeout trying to get to goal.
		int FleeTimeout; // Flee Timeout.
		//int DispatchPath; // Path Number it's currently on (for Patrol/MLays)
		*/

		DispatchClass() { memset(this, 0, sizeof(DispatchClass)); };
	};
	std::vector<DispatchClass> DispatchList;
	static bool ShouldRemoveDispatch(const DispatchClass &entry) { return !IsAround(entry.DispatchObject); }

public:
	DispatchHelper();
	virtual ~DispatchHelper();
	static DispatchHelper &Get() { return *m_DispatchHelper; }

	bool AddToDispatch(const Handle h, const float IdleTime, const bool SkipAIPCheck = false, const int Priority = 0, const bool CanFlee = true, const bool CloakNow = false, const bool Hunt = false, const bool CanGetService = false); //, Handle Target = 0); // If Priority is 1, it doesn't let DispatchHelper control cloaking.
	void Cloak(const Handle h, const int Priority = 0); // Adds a unit to dispatch if not present, otherwise cloaks it.
	void Decloak(const Handle h, const int Priority = 0); // Decloaks a unit if its already in dispatch.
	bool IsCloaked(const Handle h);
	//void CloakAttack(Handle me, Handle him, int Priority);

	void DLLHunt(const Handle h, const int Priority); // DLL driven Hunt command. Can replace with normal Hunt in 1.3.7+. 
	void DLLPatrol(const Handle h, const int Priority); // DLL driven Patrol (Friendly Hunt) command.

	void SetUseService(const Handle h, const bool Service = true); // DLL switch to set a unit's UseService status.

	void ReadChanceValues(char const *const file, char const * const block, float value[], float const defval = 0);

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void DeleteDispatchObject(Handle h, int Team);
	void Execute(BZ1Helper &bz1Helper);

	void ClearDispatch(const int Index);
};

#endif
