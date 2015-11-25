#ifndef _HQCPHelper_
#define _HQCPHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class HQCPHelper
{
private:

	struct HQCPClass {
		Handle CommandCenterObject; // This Command Center Object.
		int TeamFilter; // Teamfilter Yay!
		int TargetFilter; // Target Filter, for filtering out different types.
		float Range; // How far to paint things?
		float SoundDelay; // how long to wait before playing the sound again.
		int ThreatTimer; // How often do we play the warning sound?
		char WarningSound[MAX_ODF_LENGTH]; // Warning Sound.
		Vector Position; // It's current position.

		HQCPClass() { memset(this, 0, sizeof(HQCPClass)); };
	};
	std::vector<HQCPClass> HQCPList;
	static bool ShouldRemoveHQCP(const HQCPClass &entry) { return !IsAround(entry.CommandCenterObject); }

public:
	HQCPHelper();

	void AddHQCPObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);

	void ExecuteHandle(int HandleListIndex);
};

#endif
