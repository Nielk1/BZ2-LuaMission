#ifndef _SwitchClassHelper_
#define _SwitchClassHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class SwitchClassHelper
{
private:

	struct SwitchObjectClass {
		Handle SwitchObject; // This Switch Object.
		bool Switched; // Is it Switched?
		int SwitchTime; // SwitchTime.
		int OriginalTeam; // Original Team.
		int NewTeam; // Team it is switched to.

		SwitchObjectClass() { memset(this, 0, sizeof(SwitchObjectClass)); };
	};
	std::vector<SwitchObjectClass> SwitchObjectList;
	static bool ShouldRemoveSwitchObject(const SwitchObjectClass &entry) { return !IsAround(entry.SwitchObject) || entry.SwitchTime <= 0; }

public:
	SwitchClassHelper();
	virtual ~SwitchClassHelper();

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
