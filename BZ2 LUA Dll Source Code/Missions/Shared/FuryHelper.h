#ifndef _FuryHelper_
#define _FuryHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class FuryHelper
{
private:

	struct FuryClass {
		Handle FuryObject; // This Fury Object.
		float TurnChance; // Chance it will turn on you, per minute.
		char ChangeODF[MAX_ODF_LENGTH]; //std::string<>;

		FuryClass() { memset(this, 0, sizeof(FuryClass)); };
	};
	std::vector<FuryClass> FuryList;
	static bool ShouldRemoveFury(const FuryClass &entry) { return !IsAround(entry.FuryObject); }

public:
	FuryHelper();

	void AddFuryObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
