#ifndef _PilotResourceHelper_
#define _PilotResourceHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class PilotResourceHelper
{
private:

	static PilotResourceHelper *m_PilotResourceHelper;

	struct PilotedShipClass {
		Handle PilotedShipObject; // This Piloted Ship Object.
		float ShipDistance; // Distance to recycler.
		int ShipCommand; // Current Command.

		PilotedShipClass() { memset(this, 0, sizeof(PilotedShipClass)); };
	};
	std::vector<PilotedShipClass> PilotedShipList;
	static bool ShouldRemovePilotedShip(const PilotedShipClass &entry) { return !IsAround(entry.PilotedShipObject); }

	struct PilotClass {
		Handle PilotObject; // This Pilot Object.
		float PilotDistance; // Distance to recycler.
		Handle PilotDestination; // Where this pilot is going?
		bool IsRecycling; // Flag for if this pilot is currently recycling.
		bool IsSoldier; // Flag for if this is a pilot, not a soldier.
		bool IsIndependent; // Flag for this pilot's Independance. 0 is controlled by DLL, 1 is Skip all DLL stuff.
		bool IsGoingToShip; // Flag for if this pilot is currently trying to get in a ship.
		bool IsStuck; // Flag for if there's no place to go.
		float VehicleSearchDist; // Default distance to search for empty ships. Default is 100.0f.
		float EngageRange; // Engage Range, yay!
		float RecycleDist; // How far it must be for DLL trigger. PersonRetreatRecycleDist+5.0f. 
		int Timeout; // Idle timeout, this gives scripts a few ticks to control our pilot.

		PilotClass() { memset(this, 0, sizeof(PilotClass)); };
	};
	std::vector<PilotClass> PilotList;
	static bool ShouldRemovePilot(const PilotClass &entry) { return !IsAround(entry.PilotObject); }

public:
	PilotResourceHelper();
	~PilotResourceHelper();
	static PilotResourceHelper &Get() { return *m_PilotResourceHelper; }

	void AddPilotObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int BZCClassHash, const char *ODFName);
	void DeletePilotObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int BZCClassHash, const int ObjClassHash);

	void SetPilotIndependence(const Handle h, const bool Independence);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
