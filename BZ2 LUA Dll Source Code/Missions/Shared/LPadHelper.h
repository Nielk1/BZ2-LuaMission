#ifndef _LPadHelper_
#define _LPadHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class LPadHelper
{
private:

	struct LPadClass {
		Handle LPadObject; // This LPAd Object.
		Handle TransportObject; // This LPad's Transport.
		int TransportInterval; // Time between Transports.
		//int TransportInitDelay; // Time between Transports.
		int TransportTimer; // Timer, for counting time.
		//bool TransportInProgress; // Is it landing/taking off?
	//	bool TransportLanded; // Is it landed?
		float TransportDownTime; // How long it stays down for.
	//	float TransportLandTime; // How long it took to land, to calculate how long it takes for takeoff.
	//	float TransportDownCounter; // How long it really stays down for.
		int PilotAmount; // Pilot Amount.
		int ScrapAmount; // Scrap Amount.
		int UnitsAmount; // Units Amount.
		bool SpawnPowerups; // Should it spawn powerups?
		bool ObjectifyTransport; // Should the Transport be Objectified?
	//	int UnitsCount; // Count how many we've deployed this landing.
	//	int UnitsGroup; // Saved group for this landing.

		LPadClass() { memset(this, 0, sizeof(LPadClass)); };
	};
	std::vector<LPadClass> LPadList;

public:
	LPadHelper();

	void AddLPadObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
