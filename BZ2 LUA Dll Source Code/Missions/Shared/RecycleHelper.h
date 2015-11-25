#ifndef _RecycleHelper_
#define _RecycleHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class RecycleHelper
{
private:

	static RecycleHelper *m_RecycleHelper;

	/*
	struct RecyclePointClass {
		Handle RecycleObject; // Handle of the "host" object.
		Handle RecyclePointObject; // This Recycle Point Object.
	};
	std::vector<RecyclePointClass> RecyclePointList;
	*/

	//static bool ShouldRemoveRecyclePoint(const RecyclePointClass &entry) { return !IsAround(entry.RecyclePointObject); }

public:
	RecycleHelper();

	/*
	void AddRecyclePointObject(Handle h, int Team);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);
	*/

	void Execute(BZ1Helper &bz1Helper);
};

#endif
