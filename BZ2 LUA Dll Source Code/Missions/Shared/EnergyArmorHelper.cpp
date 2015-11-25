#include "BZ1Helper.h"
#include "EnergyArmorHelper.h"
#include "BZCScriptUtils.h"
#include <math.h>
#include <algorithm>

EnergyArmorHelper::EnergyArmorHelper()
{
}
EnergyArmorHelper::~EnergyArmorHelper()
{
}

bool EnergyArmorHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		EnergyArmorList.resize(size);
		if(size)
			Read(&EnergyArmorList.front(), sizeof(EnergyArmor)*size);
	}
	return true;
}

bool EnergyArmorHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<EnergyArmor>::iterator iter = EnergyArmorList.begin(); iter != EnergyArmorList.end(); ++iter)
		{
			EnergyArmor &sao = *iter;
			ConvertHandles(&sao.EnergyArmorObject, 1);
		}
	}
	return true;
}

bool EnergyArmorHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(EnergyArmorList.size());
		Write(&size, 1);
		if(size)
			Write(&EnergyArmorList.front(), sizeof(EnergyArmor)*size);
	}
	return true;
}

void EnergyArmorHelper::AddEnergyArmorObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<EnergyArmor>::iterator iter = EnergyArmorList.begin(); iter != EnergyArmorList.end(); ++iter)
	{
		if(iter->EnergyArmorObject == h)
			return;
	}

	EnergyArmorList.resize(EnergyArmorList.size()+1);
	EnergyArmor &sao =EnergyArmorList[EnergyArmorList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(EnergyArmor)); // Zero things out at start.

	GetODFInt(TempODF1, TempODF2, "EnergyArmorClass", "ArmorBuffer", &sao.MaxBufferSize, (GetMaxHealth(h) / 4));
	GetODFBool(TempODF1, TempODF2, "EnergyArmorClass", "AddToHealth", &sao.AddToHealth, true);
	GetODFBool(TempODF1, TempODF2, "EnergyArmorClass", "AddToAmmo", &sao.AddToAmmo, false);
	GetODFFloat(TempODF1, TempODF2, "EnergyArmorClass", "TransferRatio", &sao.BaseTransferRatio, 0.25f);
	GetODFBool(TempODF1, TempODF2, "EnergyArmorClass", "AddToHealthDeployed", &sao.AddToHealthDeployed, sao.AddToHealth);
	GetODFBool(TempODF1, TempODF2, "EnergyArmorClass", "AddToAmmoDeployed", &sao.AddToAmmoDeployed, sao.AddToAmmo);
	GetODFFloat(TempODF1, TempODF2, "EnergyArmorClass", "TransferRatioDeployed", &sao.BaseTransferRatioDeployed, sao.BaseTransferRatio);
	sao.BaseTransferRatio = clamp(sao.BaseTransferRatio, 0.0f, 1.0f);
	sao.BaseTransferRatioDeployed = clamp(sao.BaseTransferRatioDeployed, 0.0f, 1.0f);

	sao.EnergyArmorObject = h; // Save it.
}

void EnergyArmorHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2)
{
	for (std::vector<EnergyArmor>::iterator iter = EnergyArmorList.begin(); iter != EnergyArmorList.end(); ++iter)
	{
		if(iter->EnergyArmorObject == victimHandle)
		{
			int ArmorClass = 78; // N = 78, n = 110, L = 76, l = 108, H = 72, h = 104, E = 69, e = 101
			int ShieldClass = 0; // A = 65, a = 97, D = 68, d = 100, S = 83, s = 115
			// Get it's Armor class for later use.
			char TempArmorClass = 0;
			GetODFChar(victimHandle, "GameObjectClass", "armorClass", &TempArmorClass, 'N');
			ArmorClass = int(TempArmorClass);

			for(int i = 0; i < MAX_HARDPOINTS; i++)
			{
				char HostWeaponClass[MAX_ODF_LENGTH] = {0};
				GetObjInfo(victimHandle, ObjectInfoType(Get_Weapon0GOClass+i), HostWeaponClass);

				if(_stricmp(HostWeaponClass,"CLASS_SHIELDUP") == 0)
				{
					char HostWeapon[MAX_ODF_LENGTH] = {0};
					GetObjInfo(victimHandle, ObjectInfoType(Get_Weapon0ODF+i), HostWeapon);

					char TempShieldClass = 0;
					if(OpenODF2(HostWeapon))
					{
						char TempWeaponODF2[MAX_ODF_LENGTH] = {0};
						if(GetODFString(HostWeapon, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempWeaponODF2)) // Grab the classlabel.
						{
							strcat_s(TempWeaponODF2, ".odf"); // Add .odf to the end.
							OpenODF2(TempWeaponODF2);
						}

						GetODFChar(HostWeapon, TempWeaponODF2, "ShieldUpgradeClass", "shieldClass", &TempShieldClass, 'S'); // Grab the value.
					}
					ShieldClass = int(TempShieldClass);

					break;
				}
			} // Shield Check.


			char ExplosionODF[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
			char ExplosionODF2[MAX_ODF_LENGTH] = {0};

			int RealDamage = 0;
			int NormalDamage = 0;
			int ShieldADamage = 0;
			int XplNormalDamage = 0;
			int XplShieldADamage = 0;
			float ShotSpeed = 0.0f;

			char DesiredValue[MAX_ODF_LENGTH] = {0};
			char ExplosionType[MAX_ODF_LENGTH] = {0};

			// N = 78, n = 110, L = 76, l = 108, H = 72, h = 104
			// A = 65, a = 97, D = 68, d = 100, S = 83, s = 115
			if((ShieldClass == 65) || (ShieldClass == 97))
				strcpy_s(DesiredValue, "damageValue(A)");
			else if((ShieldClass == 68) || (ShieldClass == 100))
				strcpy_s(DesiredValue, "damageValue(D)");
			else if((ShieldClass == 83) || (ShieldClass == 115))
				strcpy_s(DesiredValue, "damageValue(S)");
			else if((ArmorClass == 72) || (ArmorClass == 104))
				strcpy_s(DesiredValue, "damageValue(H)");
			else if((ArmorClass == 76) || (ArmorClass == 108))
				strcpy_s(DesiredValue, "damageValue(L)");
			else
				strcpy_s(DesiredValue, "damageValue(N)");

			// Retrieve Damage Values here.
			GetODFInt(TempODF1, TempODF2, "OrdnanceClass", "damageValue(N)", &NormalDamage);
			GetODFInt(TempODF1, TempODF2, "OrdnanceClass", "damageValue(A)", &ShieldADamage);
			GetODFFloat(TempODF1, TempODF2, "OrdnanceClass", "shotSpeed", &ShotSpeed);

			// Pull Explosion ODF Damage, and include it.
			if(IsBuilding(victimHandle))
				strcpy_s(ExplosionType, "xplBuilding");
			else
				strcpy_s(ExplosionType, "xplVehicle");

			GetODFString(TempODF1, TempODF2, "OrdnanceClass", ExplosionType, MAX_ODF_LENGTH, ExplosionODF);
			if(OpenODF2(ExplosionODF))
			{
				if(GetODFString(ExplosionODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, ExplosionODF2))
				{
					strcat_s(ExplosionODF2, ".odf");
					OpenODF2(ExplosionODF2);
				}

				GetODFInt(ExplosionODF, ExplosionODF2, "ExplosionClass", "damageValue(N)", &XplNormalDamage);
				GetODFInt(ExplosionODF, ExplosionODF2, "ExplosionClass", "damageValue(A)", &XplShieldADamage);
			}

			// Combine the damage types.
			NormalDamage = int(((NormalDamage + XplNormalDamage) * BZ1Helper::Get().RefDamageMult()));
			ShieldADamage = int(((ShieldADamage + XplShieldADamage) * BZ1Helper::Get().RefDamageMult()));
			RealDamage = int((GetOrdnanceDamageDealt(victimHandle, pOrdnanceODF, true) * BZ1Helper::Get().RefDamageMult()));

			float Scale = (float(RealDamage) / float(NormalDamage));

			int KineticDmg = int(ceil((RealDamage * (0.09f - 0.01f * ShotSpeed / 1000000.0f)))); // Use Velocity to determine kinetic energy amount.;

		//	FormatLogMessage("Kinetic Damage: %d, RealDmg: %d, NormalDmg: %d, Scale: %f", KineticDmg, RealDamage, NormalDamage, Scale);

			if(NormalDamage > ShieldADamage)
			{
				iter->ArmorBuffer += int(((NormalDamage - ShieldADamage) * Scale + KineticDmg));
		//		FormatLogMessage("AmmoBuffer Energy increase by: %d", int(((NormalDamage - ShieldADamage) * Scale + KineticDmg)));
			}
			else if(RealDamage) // Only do energy for damaging weapons.
			{
				iter->ArmorBuffer += KineticDmg;
		//		FormatLogMessage("AmmoBuffer Kinetic increase by: %d", KineticDmg);
			}

			return; // We're done here, exit early.
		} // We have a match.
	}
}

void EnergyArmorHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	EnergyArmorList.erase(std::remove_if(EnergyArmorList.begin(), EnergyArmorList.end(), ShouldRemoveEnergyArmorObject), EnergyArmorList.end());

	for (std::vector<EnergyArmor>::iterator iter = EnergyArmorList.begin(); iter != EnergyArmorList.end(); ++iter)
	{
		int MaxHealth = GetMaxHealth(iter->EnergyArmorObject);
		int CurHealth = GetCurHealth(iter->EnergyArmorObject);
		int MaxAmmo = GetMaxAmmo(iter->EnergyArmorObject);
		int CurAmmo = GetCurAmmo(iter->EnergyArmorObject);
		bool Deployed = IsDeployed(iter->EnergyArmorObject);

		if(iter->ArmorBuffer > 0)
		{
			// This unit has absorbed too much energy, it must now explode.
			if(iter->ArmorBuffer > iter->MaxBufferSize)
			{
				Damage(iter->EnergyArmorObject, MaxHealth+9000); // It's over 9000!
				//EjectPilot(iter->EnergyArmorObject);
			}
			else
			{
				bool ShouldAddHealth = ((((!Deployed) && (iter->AddToHealth)) || (iter->AddToHealthDeployed)) && (CurHealth < MaxHealth));
				bool ShouldAddAmmo = ((((!Deployed) && (iter->AddToAmmo)) || (iter->AddToAmmoDeployed)) && (CurAmmo < MaxAmmo));
				// Store energy if it's not being utilized.
				if((ShouldAddHealth) || (ShouldAddAmmo))
				{
					int EnergyAmount = int(((iter->ArmorBuffer * (Deployed ? iter->BaseTransferRatioDeployed : iter->BaseTransferRatio)) / BZ1Helper::m_GameTPS));

					// If both are in use, split EnergyBuffer between them.
					if((ShouldAddHealth) && (ShouldAddAmmo))
						EnergyAmount = EnergyAmount / 2;

					// Ensure a minimum of 1.
					if(EnergyAmount <= 0)
						EnergyAmount = 1;

					// Add Health.
					if(ShouldAddHealth)
						AddHealth(iter->EnergyArmorObject, EnergyAmount);

					// Add Ammo.
					if(ShouldAddAmmo)
						AddAmmo(iter->EnergyArmorObject, EnergyAmount);

					// Subtract the amount we added from our buffer.
					iter->ArmorBuffer -= EnergyAmount;

					if(Logging)
						FormatLogMessage("Energy Armor Object: %d Adding Health/Ammo: %d, Armor Buffer: %d", iter-EnergyArmorList.begin(), EnergyAmount, iter->ArmorBuffer); 
				}
			}

			// Set the UI elements
			if(iter->EnergyArmorObject == GetPlayerHandle())
			{
				// Health Gauge.
				if((iter->ArmorBuffer + CurHealth) > MaxHealth)
					BZ1Helper::Get().SetHullGauge(1.0f);
				else
					BZ1Helper::Get().SetHullGauge(float(((iter->ArmorBuffer + CurHealth) / MaxHealth)));

				// Ammo Gauge.
				if((iter->ArmorBuffer + CurAmmo) > MaxAmmo)
					BZ1Helper::Get().SetAmmoGauge(1.0f);
				else
					BZ1Helper::Get().SetAmmoGauge(float(((iter->ArmorBuffer + CurAmmo) / MaxAmmo)));

				// Aux Gauge.
				if(iter->MaxBufferSize > 0)
					BZ1Helper::Get().SetAuxGauge(float((iter->ArmorBuffer / iter->MaxBufferSize)));
			}
		}
	}
}
