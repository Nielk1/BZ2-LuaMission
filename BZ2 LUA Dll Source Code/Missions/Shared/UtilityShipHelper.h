#ifndef _UtilityShipHelper_
#define _UtilityShipHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class UtilityShipHelper
{
private:

	static UtilityShipHelper *m_UtilityShipHelper;

	struct UtilityShipClass {
		Handle UtilityShipObject; // This UtilityShip Object.
		bool ItsAScavenger; // Is this a scavenger?
		int ScrapAmount; // This Scavenger's scraphold.
		int EscortPriority; // How important is this? 0 = None, 1 = Low, 2 = Medium, 3 = High.

		UtilityShipClass() { memset(this, 0, sizeof(UtilityShipClass)); };
	};
	std::vector<UtilityShipClass> UtilityShipList;
	static bool ShouldRemoveUtilityShip(const UtilityShipClass &entry) { return !IsAround(entry.UtilityShipObject); }

public:
	UtilityShipHelper();
	~UtilityShipHelper();
	static UtilityShipHelper &Get() { return *m_UtilityShipHelper; }

	int GetNumUtilityShipObjects() { return UtilityShipList.size(); }
	Handle &RefUtilityShipHandle(const int index) { return UtilityShipList[index].UtilityShipObject; }

	void AddUtilityShipObject(const Handle h, const int ObjClassHash, const int BZCClassHash, const int TempEscortPriority);

	Handle GetEscortTarget(const int Team = -1, const int TeamFilter = 0);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
	//void Init(BZ1Helper &bz1Helper);
};

#endif
