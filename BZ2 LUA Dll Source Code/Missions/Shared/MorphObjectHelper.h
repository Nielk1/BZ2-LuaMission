#ifndef _MorphObjectHelper_
#define _MorphObjectHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class MorphObjectHelper
{
private:

	static MorphObjectHelper *m_MorphObjectHelper;

	struct MorphObjectClass {
		Handle MorphObject; // This Morph Object.
		Handle CurrentWho; // This Morph Object's current GetWho.
		bool WeaponActive; // Flag for active status.
		bool ObjectSwitch; // Did we build it yet?
		bool CanFire; // Can fire while switched?
		bool IsCloaker; // Is cloaked while switched?
		float LocalAmmo[MAX_HARDPOINTS]; // Saved Local Ammo.
		float EngageRange; // Engage Range.
		float CloakChance[NUM_CMD]; // Chacnes to Cloak, per AI Command.
		int IdleTime; // Idle time.
		int NormalAmmo; // Normal Ammo amount.
		int MaxAmmo; // Max Normal Ammo amount.
		int Command; // Current Command. Used to trigger AI Cloaking.

		MorphObjectClass() { memset(this, 0, sizeof(MorphObjectClass)); };
	};
	std::vector<MorphObjectClass> MorphObjectList;
	static bool ShouldRemoveMorphObject(const MorphObjectClass &entry) { return !IsAround(entry.MorphObject); }

public:
	MorphObjectHelper();
	~MorphObjectHelper();
	static MorphObjectHelper &Get() { return *m_MorphObjectHelper; }

	int GetNumMorphObjects() { return MorphObjectList.size(); }
	Handle &RefMorphObjectHandle(const int Index) { return MorphObjectList[Index].MorphObject; }
	bool GetMorphObjectCloaker(const int Index) { return MorphObjectList[Index].IsCloaker; }
	bool GetMorphObjectIsActive(const int Index) { return MorphObjectList[Index].WeaponActive; }

	void AddMorphObjectHostObject(const Handle h, const char* TempODF1, const char* TempODF2);
	void AddMorphObjectToggleObject(const Handle h);

	void GetSwitchMineODF(const Handle h, char *Value);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
