#include "BZ1Helper.h"
#include "CarrierHelper.h"
#include "BZCScriptUtils.h"
#include <vector>
#include <algorithm>

CarrierHelper::CarrierHelper()
{
}
CarrierHelper::~CarrierHelper()
{
	 //m_BZ1Helper = NULL;
}

bool CarrierHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		CarrierList.resize(size);
		if(size)
			Read(&CarrierList.front(), sizeof(CarrierClass)*size);

		Read(&size, 1);
		DroneList.resize(size);
		if(size)
			Read(&DroneList.front(), sizeof(DroneClass)*size);
	}
	return true;
}

bool CarrierHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<CarrierClass>::iterator iter = CarrierList.begin(); iter != CarrierList.end(); ++iter)
		{
			CarrierClass &sao = *iter;
			ConvertHandles(&sao.CarrierObject, 1);
		}
		for (std::vector<DroneClass>::iterator iter = DroneList.begin(); iter != DroneList.end(); ++iter)
		{
			DroneClass &sao = *iter;
			ConvertHandles(&sao.DroneObject, 1);
			ConvertHandles(&sao.DroneCarrier, 1);
			ConvertHandles(&sao.DroneTarget, 1);
		}
	}
	return true;
}

bool CarrierHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(CarrierList.size());
		Write(&size, 1);
		if(size)
			Write(&CarrierList.front(), sizeof(CarrierClass)*size);

		size = int(DroneList.size());
		Write(&size, 1);
		if(size)
			Write(&DroneList.front(), sizeof(DroneClass)*size);
	}
	return true;
}

void CarrierHelper::AddCarrierDroneObject(const Handle h, const int Team)
{
	for (std::vector<DroneClass>::iterator iter = DroneList.begin(); iter != DroneList.end(); ++iter)
	{
		if(iter->DroneObject == h)
			return;
	}

	DroneList.resize(DroneList.size()+1);
	DroneClass &sao = DroneList[DroneList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(DroneClass)); // Zero things out at start.

	// Find which carrier launched this drone.
	float BestDistanceSquared = FLT_MAX;
	int CarrierIndex = -1;
	for (std::vector<CarrierClass>::iterator iter = CarrierList.begin(); iter != CarrierList.end(); ++iter)
	{
		float DistanceSquared = GetDistance3DSquared(GetPosition(h), GetPosition(iter->CarrierObject));
		if((DistanceSquared < BestDistanceSquared) && (h != iter->CarrierObject) && (GetTeamNum(iter->CarrierObject) == Team))
		{
			BestDistanceSquared = DistanceSquared;
			CarrierIndex = iter-CarrierList.begin();
		}
	}

	// We have a winner? This Drone is close enough to be considered come from an Carrier.
	if(//(BestDistanceSquared < Squared(50.0f)) && // Within Range.
		(CarrierIndex >= 0) && 
		((!CarrierList[CarrierIndex].DroneControlRange) || // Infinite Range.
		((CarrierList[CarrierIndex].DroneControlRange) && (BestDistanceSquared < CarrierList[CarrierIndex].DroneControlRange))) && // It's within control range.
		((!CarrierList[CarrierIndex].MaxDrones) || // Max Drones is infinite.
		((CarrierList[CarrierIndex].MaxDrones) && (CarrierList[CarrierIndex].CurDrones < CarrierList[CarrierIndex].MaxDrones)))) // Less then max drones.
	{

		sao.IsActive = true;
		sao.DroneCarrier = CarrierList[CarrierIndex].CarrierObject;
		sao.ControlRange = CarrierList[CarrierIndex].DroneControlRange;
		++CarrierList[CarrierIndex].CurDrones;

		/*
		// Tweak it's height if it spanwed on the ground.
		Vector Position = GetPosition(h);
		Vector APCPosition = GetPosition(APCHandle);
		Position.y = APCPosition.y;
		SetVectorPosition(h, Position);
		*/

		// Read ODFs to find which WeaponSlot this Drone came out of.
		const char *ODFName = GetODFName(sao.DroneObject);

		for(int i = 0; i < MAX_HARDPOINTS; i++)
		{
			char WeaponODF[MAX_ODF_LENGTH] = {0};
			GetObjInfo(sao.DroneCarrier, ObjectInfoType(Get_Weapon0ODF+i), WeaponODF);

			char DispenserName[MAX_ODF_LENGTH] = {0};
			if(OpenODF2(WeaponODF))
			{
				if(GetODFString(WeaponODF, "DispenserClass", "objectClass", MAX_ODF_LENGTH, DispenserName))
				{
					if((ODFName) && (_stricmp(ODFName, DispenserName) == 0)) // We found a match.
					{
						sao.CarrierWeaponSlot = i;

						int TempLocalAmmo = 0;
						GetODFInt(WeaponODF, "WeaponClass", "localAmmo", &TempLocalAmmo);
						if(TempLocalAmmo > 0)
							sao.UseLocalAmmo = true;

						break;
					}
				}
			}
		}

	}
	else
	{
		sao.IsActive = false;
		SetIndependence(h, 0); // Shut down.
	}

	sao.DroneObject = h;
}

void CarrierHelper::AddCarrierObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	bool AddToList = true;
	for (std::vector<CarrierClass>::iterator iter = CarrierList.begin(); iter != CarrierList.end(); ++iter)
	{
		if(iter->CarrierObject == h)
		{
			AddToList = false;
			break;
		}
	}
	if(AddToList)
	{
		CarrierList.resize(CarrierList.size()+1);
		CarrierClass &sao = CarrierList[CarrierList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(CarrierClass)); // Zero things out at start.

		// Look for new ODF Parameters. 
		GetODFFloat(TempODF1, TempODF2, "CarrierClass", "controlRange", &sao.DroneControlRange, 0.0f);

		GetODFFloat(TempODF1, TempODF2, "CarrierClass", "repairRange", &sao.RepairRange, 100.0f);
		GetODFLong(TempODF1, TempODF2, "CarrierClass", "HealthRepairAmount", &sao.HealthRepairAmount);
		GetODFLong(TempODF1, TempODF2, "CarrierClass", "AmmoRepairAmount", &sao.AmmoRepairAmount);

		sao.CarrierObject = h;
	}
}

void CarrierHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	CarrierList.erase(std::remove_if(CarrierList.begin(), CarrierList.end(), ShouldRemoveCarrier), CarrierList.end());
	DroneList.erase(std::remove_if(DroneList.begin(), DroneList.end(), ShouldRemoveDrone), DroneList.end());

	// Loop over all Drones.
	for (std::vector<DroneClass>::iterator iter = DroneList.begin(); iter != DroneList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->DroneObject);
		iter->Position = GetPosition(iter->DroneObject);

		if(!iter->IsActive) // No carriers around, look for a new one to come into range.
		{
			for (std::vector<CarrierClass>::iterator iterB = CarrierList.begin(); iterB != CarrierList.end(); ++iterB)
			{
				if( (GetTeamNum(iterB->CarrierObject) == Team) && // Team matches.
					(GetDistance3DSquared(iter->Position, GetPosition(iterB->CarrierObject)) < Squared(iterB->DroneControlRange)) && // It's within control range.
					((!iterB->MaxDrones) ||	(iterB->CurDrones < iterB->MaxDrones))) // Less then max drones.
				{
					iter->IsActive = true;
					SetIndependence(iter->DroneObject, 1);
					iter->DroneCarrier = iterB->CarrierObject;
					iter->ControlRange = iterB->DroneControlRange;

					if(Logging)
						FormatLogMessage("Drone %08x Reassigned to Carrier: %08x", iter->DroneObject, iterB->CarrierObject);

					break;
				}
			}
		}
		else // This drone is currently active and under the control of a Carrier.
		{
			// If this Drone's carrier is Dead, or it is outside of range, find a new one.
			if((!IsAround(iter->DroneCarrier)) && // Carrier is dead, or outside range if Control Range isn't infinite.
				((!iter->ControlRange) || (GetDistance3DSquared(iter->Position, GetPosition(iter->DroneCarrier)) > Squared(iter->ControlRange))))
			{
				iter->IsActive = false;
				SetIndependence(iter->DroneObject, 0);
				Stop(iter->DroneObject, 1);

				if(Logging)
					FormatLogMessage("Drone %08x Carrier Dead or Outside of Range, shutting down", iter->DroneObject);

				continue; // Skip to next Drone.
			}

			// If it's low on health or ammo, and nearby a Carrier, repair it.
			if((NeedsHealth(iter->DroneObject)) || (NeedsAmmo(iter->DroneObject)))
			{
				for (std::vector<CarrierClass>::iterator iterB = CarrierList.begin(); iterB != CarrierList.end(); ++iterB)
				{
					if(GetDistance3DSquared(iter->Position, GetPosition(iterB->CarrierObject)) < Squared(iterB->RepairRange)) // It's within control range.
					{
						if(iterB->HealthRepairAmount)
							AddHealth(iter->DroneObject, (iterB->HealthRepairAmount / BZ1Helper::m_GameTPS));
						if(iterB->AmmoRepairAmount)
							AddAmmo(iter->DroneObject, (iterB->AmmoRepairAmount / BZ1Helper::m_GameTPS));
					}
				}
			}

			Handle Target = 0;

			if(IsPlayer(iter->DroneCarrier))
				Target = GetUserTarget(GetTeamNum(iter->DroneCarrier));
			else
				Target = GetTarget(iter->DroneCarrier);

			/*
			Handle DroneTempTarget = GetTarget(iter->DroneObject);

			if((iter->IsDefending) && (!IsAround(iter->DroneTarget)))
			{
				if(DroneTempTarget)
					iter->DroneTarget = DroneTempTarget;
			}
			*/

			if(!IsAround(iter->DroneTarget))
			{
				if((Target) && (!IsTeamAllied(Team, GetTeamNum(Target))))
				{
			//		if(Target != DroneTempTarget)
			//		{
						Attack(iter->DroneObject, Target, 1);
						iter->DroneTarget = Target;
						iter->IsDefending = false;

						if(Logging)
							FormatLogMessage("Drone %08x Set to Attack: %08x", iter->DroneObject, Target);
			//		}
				}
				else
				{
					// Get distance of nearest Enemy to the Carrier. If it's NOT within ControlRange, recall drone.
					Handle NearestEnemy = BZ1Helper::Get().GetNearestObject(iter->DroneCarrier, iter->ControlRange, Team, Enemies);
					//float Distance = GetDistance3D(NearestEnemy, iter->DroneCarrier);

					if(!iter->IsReturning)
					{
						if((iter->ControlRange) && (!NearestEnemy)) //(Distance > iter->ControlRange))
						{
							Goto(iter->DroneObject, iter->DroneCarrier, 1);
							iter->IsReturning = true;

							if(Logging)
								FormatLogMessage("Drone %08x Set to Return to Carrier: %08x", iter->DroneObject, iter->DroneCarrier);
						}
					}
					else // This drone is returning, watch for it to get close enough to Remove it.
					{
						//if(Distance < iter->ControlRange)
						if(NearestEnemy)
						{
							iter->IsReturning = false;
							iter->IsDefending = true;
							Defend2(iter->DroneObject, iter->DroneCarrier, 1); // Fallback to Defend mode for now.

							if(Logging)
								FormatLogMessage("Drone %08x Aborting Return to Carrier: %08x", iter->DroneObject, iter->DroneCarrier);

							continue;
						}

						if(GetDistance(iter->DroneObject, iter->DroneCarrier) < 50.0f)
						{
							if(iter->UseLocalAmmo)
								AddLocalAmmo(iter->DroneCarrier, float(GetMaxHealth(iter->DroneObject)), iter->CarrierWeaponSlot);
							else
								AddAmmo(iter->DroneCarrier, GetMaxHealth(iter->DroneObject));

							if(Logging)
								FormatLogMessage("Drone %08x Set to Return to Carrier: %08x", iter->DroneObject, iter->DroneCarrier);

							RemoveObject(iter->DroneObject);
							continue; // This drone is gone, skip to next one.
						}
					}

					// If not returning, and not Attacking a Target, the drones are in "Defense" mode.
					if(!iter->IsDefending)
					{
						Defend2(iter->DroneObject, iter->DroneCarrier, 1);
						iter->IsDefending = true;

						if(Logging)
							FormatLogMessage("Drone %08x Set to Defend Carrier: %08x", iter->DroneObject, iter->DroneCarrier);
					}
				}

			}
		}
	}

	/*
	// Set the UI Elements here.
	for (std::vector<CarrierClass>::iterator iter = CarrierList.begin(); iter != CarrierList.end(); ++iter)
	{
		if(iter->CarrierObject == GetPlayerHandle())
		{
			if(iter->MaxDrones)
				BZ1Helper::Get().SetAuxGauge(float(float(iter->CurDrones) / float(iter->MaxDrones)));

			break;
		}
	}
	*/
}

void CarrierHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	// If this is not a Human controlled team. 
	for (std::vector<DroneClass>::iterator iter = DroneList.begin(); iter != DroneList.end(); ++iter)
	{
		if((iter->DroneTarget != shooterHandle) && (victimHandle == iter->DroneCarrier)) // The Carrier is under attack, all drones engage ShooterHandle.
		{
			iter->IsDefending = false;
			iter->DroneTarget = shooterHandle;
			Attack(iter->DroneObject, shooterHandle, 1);
		}
	}
}