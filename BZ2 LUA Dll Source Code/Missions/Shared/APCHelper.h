#ifndef _APCHelper_
#define _APCHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

#include <vector>

class BZ1Helper;

class APCHelper
{
private:

	struct APCClass {
		Handle APCObject; // This APC Object.
		Handle RefillChoice; // This APC's current Choice.
	//	Handle LastTarget; // Thing this APC is ordered to Attack. (Or the last object it attacked before refilling)
		bool IsRefilling; // Is it trying torefill?
	//	bool IsStuck; // Is it stuck? (no pilot? No refil? Basically waiting for a new place to go)
		bool HasNoPilot; // Is it sniped? (Do we really need stuck?)
	//	bool HasNoRefill; // Does it have no valid refill points?
		bool PlayedSoundFirst; // Has it played a sound? 
		int RefillTime; // Refill time. How long it must wait.
		int RefillingTimer; // Tick counter to count how long its been waiting.
		//char DeploySound[MAX_ODF_LENGTH]; //std::string<>;

		APCClass() { memset(this, 0, sizeof(APCClass)); };
	};
	std::vector<APCClass> APCList;
	static bool ShouldRemoveAPC(const APCClass &entry) { return !IsAround(entry.APCObject); }

public:
	APCHelper();
	virtual ~APCHelper();

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	void AddAPCSoldierObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team);
	void AddAPCObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
