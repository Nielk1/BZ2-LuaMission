#ifndef _SatelliteHelper_
#define _SatelliteHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class SatelliteHelper
{
private:

	static SatelliteHelper *m_SatelliteHelper;

	struct CommTowerClass {
		Handle CommTowerObject; // This CommTower Object.
		bool PlaySound; // Does it play a sound?

		CommTowerClass() { memset(this, 0, sizeof(CommTowerClass)); };
	};
	std::vector<CommTowerClass> CommTowerList;
	static bool ShouldRemoveCommTower(const CommTowerClass &entry) { return !IsAround(entry.CommTowerObject); }

	/*
		char PilotWeapon[MAX_TEAMS][MAX_HARDPOINTS][MAX_ODF_LENGTH];
		char PlayerPilotODF[MAX_TEAMS][MAX_ODF_LENGTH];

	bool b_first,
		CommTowerAround[MAX_TEAMS], // Get a flag as to if the team has any commtowers. -GBD
		PlayerWasPilot[MAX_TEAMS], // Save this, or else DIE!!!!
		GiveBackWeapons[MAX_TEAMS], // Flag to restore a pilot's weapons, if they picked some up.
		PlayCommTowerSound[MAX_TEAMS], // Flag to play CommTower sound.
		UseSiteCamera[MAX_TEAMS], // Flag to use site camera or not?
		b_last;

	Handle h_first,
		CommTower[MAX_TEAMS], // Save the Primary Selectable CommTower. -GBD
		Player[MAX_TEAMS], // Store a player's ship if they're in satellite. NOTE: This is ONLY used in Satellite Code!
		SatellitePlayer[MAX_TEAMS], // Sat Object for the player. 
		SatelliteOn[MAX_TEAMS], // Satellite! OFF MAP!
		SatelliteOff[MAX_TEAMS], // Satellite! // OFF MAP!
		SatelliteLock[MAX_TEAMS], // Satellite! // OFF MAP!
		h_last;

	int i_first,
		SatelliteViewActive[MAX_TEAMS], // Store if satellite is on/off for deactivation/cleaning up. Multi Stage flag. -GBD
		SatelliteOutOfAmmo[MAX_TEAMS], // Counter to delay Satellite deactivation some.
		i_last;

	bool *b_array;
	int b_count;

	int *h_array;
	int h_count;

	int *i_array;
	int i_count;
	*/

public:
	SatelliteHelper();
	virtual ~SatelliteHelper();
	static SatelliteHelper &Get() { return *m_SatelliteHelper; }

	/*
	Handle &RefPlayerHandle(const int Team) { return Player[Team]; }
	Handle &RefSatellitePlayerHandle(const int Team) { return SatellitePlayer[Team]; }

	Handle &RefSatelliteOnHandle(const int Team) { return SatelliteOn[Team]; }
	Handle &RefSatelliteOffHandle(const int Team) { return SatelliteOff[Team]; }
	Handle &RefSatelliteLockHandle(const int Team) { return SatelliteLock[Team]; }

	char * const RefPilotODF(const int Team) { return PlayerPilotODF[Team]; }

	int &RefSatelliteViewActive(const int Team) { return SatelliteViewActive[Team]; }
	int &RefSatelliteOutOfAmmo(const int Team) { return SatelliteOutOfAmmo[Team]; }
	*/

	void AddSatelliteObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team);
	void AddSatelliteSwitchObject(const Handle h, const int Team, const char *ODFName);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
	virtual PreGetInReturnCodes PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle);
	void PilotWeaponReplacement(void);

	void ToggleSatelliteOn(const int Team);
	void ToggleSatelliteOff(const int Team);
	void LockSatellite(const int Team);
	void UnlockSatellite(const int Team);
	//bool IsSatelliteLocked(const int Team);
	//bool IsSatelliteOn(const int Team);
	//bool IsSatelliteOff(const int Team);
};

#endif
