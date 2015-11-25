#ifndef _DropShipHelper_
#define _DropShipHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class DropShipHelper
{
private:

	struct DropShipItem {
		char ODFName[MAX_ODF_LENGTH]; // Ship ODF Name.
		char CurPilot[MAX_ODF_LENGTH]; // Current pilot config.
		long CurHealth; // Ships Health.
		long CurAmmo; // Ships Ammo.
		char Weapons[MAX_HARDPOINTS][MAX_ODF_LENGTH]; // Ship's weapons.
		float LocalAmmo[MAX_HARDPOINTS]; // Each ship's local weapon ammo.
		int CurScrap; // Saved scrap for scavengers.
		int ScrapValue; // For destruction moneys.
		int UnitSize; // how much space this unit takes up.

		DropShipItem() { memset(this, 0, sizeof(DropShipItem)); };
	};

	struct DropShipClass {
		Handle DropShipObject;
		bool IsUnloading; // Is it unloading?
		bool Objectify; // Objectify the DropShip?
		bool IsObjectified; // Is it currently objectified?
		char Name[MAX_ODF_LENGTH]; // It's name, for flashing capacity in name.
		int UnitCount; // How many ships it's currently carrying. (MAX OF 16!)
		int MaxSize; // How many can it carry?
		int CurSize; // How many units it currently has.
		float BoardingRange; // How far must units be to board this transport?
		int UnitsGroup; // Saved group for this landing.
		int TeamFilter; // Team Filter for which units can board.
		bool DidDemolish; // Flag for if this recycled.
		Vector LastPos; // Last Position.
		DropShipItem Cargo[MAX_DROPSHIP_CARGO];// info for each ship being carried.

		DropShipClass() { memset(this, 0, sizeof(DropShipClass)); };
	};
	std::vector<DropShipClass> DropShipList;
	//static bool ShouldRemoveDropShip(const DropShipClass &entry) { return !IsAround(entry.DropShipObject); }

public:
	DropShipHelper();

	void AddDropShipObject(const Handle h, const char* TempODF1, const char* TempODF2);
	void DeleteDropShipObject(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif

