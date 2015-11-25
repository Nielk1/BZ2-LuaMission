#ifndef _GeyserHelper_
#define _GeyserHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class GeyserHelper
{
private:

	struct GeyserClass {
		Handle GeyserObject; // This Geyser Object.
		bool PlacedDummyGeyser; // Have we placed our "dummy" geyser yet?

		GeyserClass() { memset(this, 0, sizeof(GeyserClass)); };
	};
	std::vector<GeyserClass> GeyserList;
	static bool ShouldRemoveGeyser(const GeyserClass &entry) { return !IsAround(entry.GeyserObject); }

public:
	GeyserHelper();

	void AddGeyserObject(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
