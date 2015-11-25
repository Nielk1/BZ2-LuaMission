#ifndef _CarrierHelper_
#define _CarrierHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

#include <vector>

class BZ1Helper;

class CarrierHelper
{
private:

	struct CarrierClass {
		Handle CarrierObject; // This Carrier Object.
		float DroneControlRange; // How far can this Carrier control it's drones from?
		float RepairRange; // How far to repair drones?
		int MaxDrones; // This carrier's max drones. 0 = Unlimited.
		int CurDrones; // How many drones it currently has control over.
		long HealthRepairAmount; // How much health to repair?
		long AmmoRepairAmount; // How much ammo to repair?
		//Vector Position; // It's current position.

		CarrierClass() { memset(this, 0, sizeof(CarrierClass)); };
	};
	std::vector<CarrierClass> CarrierList;
	static bool ShouldRemoveCarrier(const CarrierClass &entry) { return !IsAround(entry.CarrierObject); }

	struct DroneClass {
		Handle DroneObject; // This Drone Object.
		Handle DroneCarrier; // This Drone's Carrier.
		Handle DroneTarget; // This drone's current Target.
		float ControlRange; // This Drone's current Carrier's DroneControlRange.
		bool IsReturning; // Is this drone returning home?
		bool IsActive; // Is this drone recieving orders from a nearby carrier?
		bool IsDefending; // Is this drone in Defense mode?
		bool UseLocalAmmo; // Flag for if this Drone's Carrier's weapon uses local ammo or not.
		int CarrierWeaponSlot; // Which weapon slot this ship came out of.
		Vector Position; // It's current position.

		DroneClass() { memset(this, 0, sizeof(DroneClass)); };
	};
	std::vector<DroneClass> DroneList;
	static bool ShouldRemoveDrone(const DroneClass &entry) { return !IsAround(entry.DroneObject); }

public:
	CarrierHelper();
	virtual ~CarrierHelper();

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	void AddCarrierDroneObject(const Handle h, const int Team);
	void AddCarrierObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
