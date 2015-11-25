#ifndef _TransportHelper_
#define _TransportHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class TransportHelper
{
private:

	struct TransportClass {
		Handle TransportObject;
		bool IsInProgress; // Is it landing/taking off?
		bool IsLanded; // Is it landed?
		bool SpawnPowerups; // Spawn some powerups too?
		bool Objectify; // Objectify the Transport?
		int Timer; // Timer, for counting time.
		int DownTime; // How long it stays down for.
		int DownCounter; // How long it really stays down for.
		int LandTime; // How long it took to land, this + DownCounter is to calculate how long it takes for takeoff.
		int PilotAmount; // Pilot Amount.
		int ScrapAmount; // Scrap Amount.
		int UnitsAmount; // Units Amount.
		int UnitsCount; // Count how many we've deployed this landing.
		int UnitsGroup; // Saved group for this landing.

		TransportClass() { memset(this, 0, sizeof(TransportClass)); };
	};
	std::vector<TransportClass> TransportList;
	static bool ShouldRemoveTransport(const TransportClass &entry) { return !IsAround(entry.TransportObject); }

public:
	TransportHelper();

	Handle BuildTransportObject(const char *ODF, const int Team, const Matrix Position, const float DownTime = 0.0f, const float HeightOffset = 0.0f, const int PilotAmount = 0, const int ScrapAmount = 0, const int UnitsAmount = 0, const bool SpawnPowerups = false, const bool Objectify = false, const bool AddToGroup = false);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif

