#ifndef _UndeployHelper_
#define _UndeployHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class UndeployHelper
{
private:

//	static UndeployHelper *m_UndeployHelper;

	struct UndeployClass {
		Handle UndeployingObject; // This undeploying Object.
		bool IsAnimated; // Is it animated?
		int UndeployTime; // undeploying time.
		float UndeployFrames; // Animation frames.

		UndeployClass() { memset(this, 0, sizeof(UndeployClass)); };
	};
	std::vector<UndeployClass> UndeployList;
	static bool ShouldRemoveUndeployObject(const UndeployClass &entry) { return !IsAround(entry.UndeployingObject); }

public:
	UndeployHelper();

	void AddUndeployObject(const Handle h, const int Team, const char *ODFName);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
