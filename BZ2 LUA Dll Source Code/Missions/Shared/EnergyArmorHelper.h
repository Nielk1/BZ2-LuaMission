#ifndef _EnergyArmorHelper_
#define _EnergyArmorHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class EnergyArmorHelper
{
private:

	struct EnergyArmor {
		Handle EnergyArmorObject; // This EnergyArmorArmor Object.
		int MaxBufferSize; // Size of the energy buffer.
		int ArmorBuffer; // Amount of Energy asborbed by this unit's Armor.
		bool AddToHealth; // Does this energy buffer add to Health?
		bool AddToAmmo; // Does this energy buffer add to Ammo?
		bool AddToHealthDeployed; // Does it do it while deployed?
		bool AddToAmmoDeployed; // Does it do it while deployed?
		float BaseTransferRatio; // Base Ratio for transfer falloff curve.
		float BaseTransferRatioDeployed; // Base Ratio for deployed state.

		EnergyArmor() { memset(this, 0, sizeof(EnergyArmor)); };
	};
	std::vector<EnergyArmor> EnergyArmorList;
	static bool ShouldRemoveEnergyArmorObject(const EnergyArmor &entry) { return !IsAround(entry.EnergyArmorObject); }

public:
	EnergyArmorHelper();
	virtual ~EnergyArmorHelper();

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2);

	void AddEnergyArmorObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
