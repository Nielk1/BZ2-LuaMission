#ifndef _AmmoDrainHelper_
#define _AmmoDrainHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class AmmoDrainHelper
{
private:

	struct AmmoDrainClass {
		Handle AmmoDrainObject; // This AmmoDrain Object (The thing being drained of ammo).
		Handle AmmoDrainTarget; // This AmmoDrain's Target (The thing that shot it and is draining its ammo).
		int DrainTime; // Time, in ticks, to be drained.
		float AmmoDrainAmount; // Ammo Amount to drain. (per second)
		float AmmoDrainRemainder; // Left Over gibblets?
		float HealthDrainAmount; // Health Amount to drain. (per second)
		float HealthDrainRemainder; // Left Over gibblets?
		float ScrapDrainAmount; // Scrap Amount to drain. (per second)
		float ScrapDrainRemainder; // Left Over gibblets?

		AmmoDrainClass() { memset(this, 0, sizeof(AmmoDrainClass)); };
	};
	std::vector<AmmoDrainClass> AmmoDrainList;
	static bool ShouldRemoveAmmoDrain(const AmmoDrainClass &entry) { return entry.DrainTime <= 0 || !IsAround(entry.AmmoDrainObject); }

public:
	AmmoDrainHelper();
	virtual ~AmmoDrainHelper();

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
