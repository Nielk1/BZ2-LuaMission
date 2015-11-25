#ifndef _ReplaceOnDeathHelper_
#define _ReplaceOnDeathHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class ReplaceOnDeathHelper
{
private:

	struct ReplaceOnDeathClass {
		Handle ReplaceObject; // This ReplaceOnDeath Object.
		Handle ReplaceHostObject; // This ReplaceOnDeath Object's Host if it is a Tap.
		bool DoExplode; // Perform an Explosion?
		bool StayTap; // Perform an Explosion?
		int TapIndex; // Which Tap?
		bool DidDemolish; // Was it demolished?
		Matrix Position; // Position of demise.
		float ReplaceHeight; // Refill time. How long it must wait.
		int ReplaceTeam; // Team to put the replaced object on?
		int ReplaceTime; // Internal timer, in ticks, how long till it get's replaced.
		char ReplaceODF[MAX_ODF_LENGTH]; // What to replace this object with.
		bool ReplaceOnlyIfShot; // Only replace if killed?
		int ReplaceCount; // How many items to spawn.
		bool RandomHeading; // Replace with a Random heading angle?
		int ShotTurn; // Turn this object was shot.
		bool WasPlayer; // Was this the player?
		int CurTeam; // Current Team.

		ReplaceOnDeathClass() { memset(this, 0, sizeof(ReplaceOnDeathClass)); };
	};
	std::vector<ReplaceOnDeathClass> ReplaceOnDeathList;

public:
	ReplaceOnDeathHelper();
	virtual ~ReplaceOnDeathHelper();

	void AddReplaceOnDeathObject(const Handle h, const char *ReplaceODF = NULL, const int Team = -1, const float HeightOffset = 0.0, const bool DoExplode = false, const bool StaysTap = true, const float MinReplaceTime = 0.0f, const float MaxReplaceTime = 0.0f, const bool RandomizeTime = false);
	void DeleteReplaceOnDeathObject(const Handle h);

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
