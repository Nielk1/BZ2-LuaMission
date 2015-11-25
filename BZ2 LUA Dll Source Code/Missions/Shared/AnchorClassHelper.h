#ifndef _AnchorClassHelper_
#define _AnchorClassHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class AnchorClassHelper
{
private:

	struct AnchorClass {
		Handle AnchorObject; // This Anchored Object.
		int DragTime; // Time to be drained, in Turns.
		float DragAmount; // Amount to drag.

		AnchorClass() { memset(this, 0, sizeof(AnchorClass)); };
	};
	std::vector<AnchorClass> AnchorList;
	static bool ShouldRemoveAnchorObject(const AnchorClass &entry) { return entry.DragTime <= 0 || !IsAround(entry.AnchorObject); }

public:
	AnchorClassHelper();
	virtual ~AnchorClassHelper();

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
