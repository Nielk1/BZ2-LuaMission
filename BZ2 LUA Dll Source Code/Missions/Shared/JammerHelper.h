#ifndef _JammerHelper_
#define _JammerHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class JammerHelper
{
private:

	struct JammerClass {
		Handle JammerObject; // This Jammer Object.
		int JammerIndex; // Jammer Tap Index.
		char JammerODF[MAX_ODF_LENGTH]; // Jammer Tap ODF.

		JammerClass() { memset(this, 0, sizeof(JammerClass)); };
	};
	std::vector<JammerClass> JammerList;
	static bool ShouldRemoveJammer(const JammerClass &entry) { return !IsAround(entry.JammerObject); }

public:
	JammerHelper();

	void AddJammerObject(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif

