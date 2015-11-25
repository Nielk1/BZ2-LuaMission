#include "BZ1Helper.h"
#include "DropShipHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

DropShipHelper::DropShipHelper()
{
}

bool DropShipHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		DropShipList.resize(size);
		if(size)
			Read(&DropShipList.front(), sizeof(DropShipClass)*size);
	}
	return true;
}

bool DropShipHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<DropShipClass>::iterator iter = DropShipList.begin(); iter != DropShipList.end(); ++iter)
		{
			DropShipClass &sao = *iter;
			ConvertHandles(&sao.DropShipObject, 1);
		}
	}
	return true;
}

bool DropShipHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(DropShipList.size());
		Write(&size, 1);
		if(size)
			Write(&DropShipList.front(), sizeof(DropShipClass)*size);
	}
	return true;
}

void DropShipHelper::AddDropShipObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<DropShipClass>::iterator iter = DropShipList.begin(); iter != DropShipList.end(); ++iter)
	{
		if(iter->DropShipObject == h)
			return;
	}

	DropShipList.resize(DropShipList.size()+1);
	DropShipClass &sao = DropShipList[DropShipList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(DropShipClass)); // Zero things out at start.

	if((GetODFString(TempODF1, TempODF2, "GameObjectClass", "unitName2", MAX_ODF_LENGTH, sao.Name)) || 
		(GetODFString(TempODF1, TempODF2, "GameObjectClass", "unitName", MAX_ODF_LENGTH, sao.Name)))
	{
		// Got it.
	}
	GetODFBool(TempODF1, TempODF2, "DropShipClass", "ObjectifyWhileCarrying", &sao.Objectify, false);
	GetODFFloat(TempODF1, TempODF2, "DropShipClass", "BoardingRange", &sao.BoardingRange, 50.0f);
	GetODFInt(TempODF1, TempODF2, "DropShipClass", "MaxUnitCapacity", &sao.MaxSize, 10); // Default to 10.
	sao.MaxSize = clamp(sao.MaxSize, 0, MAX_DROPSHIP_CARGO);
	GetODFInt(TempODF1, TempODF2, "DropShipClass", "TeamFilter", &sao.TeamFilter, 3);
	sao.DropShipObject = h;
}

void DropShipHelper::DeleteDropShipObject(const Handle h)
{
	for (std::vector<DropShipClass>::iterator iter = DropShipList.begin(); iter != DropShipList.end(); ++iter)
	{
		if(iter->DropShipObject == h)
		{
			if(IsAround(iter->DropShipObject))
				iter->DidDemolish = true;

			return;
		}
	}
}

void DropShipHelper::Execute(BZ1Helper &bz1Helper)
{
	//bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	//DropShipList.erase(std::remove_if(DropShipList.begin(), DropShipList.end(), ShouldRemoveDropShip), DropShipList.end());

	// Sweep through the list and remove any of them that are no longer around
	std::vector<DropShipClass>::iterator validEnd = DropShipList.begin();
	std::vector<DropShipClass>::iterator lastValid = DropShipList.begin();
	for (std::vector<DropShipClass>::iterator iter = DropShipList.begin(); iter != DropShipList.end(); ++iter)
	{
		// If the host or weapon is around...
		if (IsAround(iter->DropShipObject))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			// Spawn a scrap field!
			if((iter->UnitCount) && (!iter->DidDemolish))
			{
				int TotalScrapInside = 0;
				for(int i = 0; i < iter->UnitCount; i++)
					TotalScrapInside += iter->Cargo[i].ScrapValue;

				//float MaxDist = 10.0f * sqrtf(float(TotalScrapInside));
				//const char *ODFName = GetODFName(iter->DropShipObject);
				SpawnScrapField(iter->LastPos, TotalScrapInside, 0.0f, GetRace(iter->DropShipObject));
			}
		}
	}
	// Now erase any invalid entries left behind
	DropShipList.erase(validEnd, DropShipList.end());

	for (std::vector<DropShipClass>::iterator iter = DropShipList.begin(); iter != DropShipList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->DropShipObject);
		iter->LastPos = GetPosition(iter->DropShipObject);

		// Show allies how full the Transport is.
		if(IsTeamAllied(Team, GetLocalPlayerTeamNumber()))
		{
			char NewName[MAX_ODF_LENGTH] = {0};
			sprintf_s(NewName, "%s %d/%d", iter->Name, iter->CurSize, iter->MaxSize);
			SetObjectiveName(iter->DropShipObject, NewName);
		}

		/*
		if(iter->DropShipObject == GetPlayerHandle())
		{
			if(iter->MaxSize)
				BZ1Helper::Get().SetAuxGauge(float(float(iter->CurSize) / float(iter->MaxSize)));
		}
		*/

		if(iter->Objectify)
		{
			if((!iter->IsObjectified) && (!IsDeployed(iter->DropShipObject)))
			{
				SetObjectiveOn(iter->DropShipObject);
				iter->IsObjectified = true;
			}
			else if(IsDeployed(iter->DropShipObject))
			{
				SetObjectiveOff(iter->DropShipObject);
				iter->IsObjectified = false;
			}
		}

		// If this dropship isn't full, watch for units to board it.
		if((iter->CurSize < iter->MaxSize) && (iter->UnitCount < MAX_DROPSHIP_CARGO))
		{
			if(!iter->IsUnloading)
			{
				for(int i = 0; i < BZ1Helper::Get().GetNumHandles(); i++)
				{
					BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(i);
					Handle TempH = objectInfo.ObjectHandle;

					if(TempH == iter->DropShipObject)
						continue; // Skip ourselves.

					// This object is allied and is within range, matches teams, and is following the unit.
					if( (TeamFilterCheck(iter->TeamFilter, Team, objectInfo.CurTeamNum) && 
						(objectInfo.CategoryID == Vehicle) && 
						(IsFollowing(TempH, iter->DropShipObject)) && 
						(GetDistance3DSquared(iter->LastPos, objectInfo.Position) < Squared(iter->BoardingRange))) && 
						(!IsPlayer(TempH)))
					{
						int UnitSize = 1;
						GetODFInt(TempH, "GameObjectClass", "UnitSize", &UnitSize, 1);

						if((UnitSize + iter->CurSize) > iter->MaxSize)
							continue; // Skip this object.

						DropShipItem &nextSlot = iter->Cargo[iter->UnitCount];

						GetODFInt(TempH, "GameObjectClass", "scrapValue", &nextSlot.ScrapValue);

						if (const char *pilotOdf = GetPilotClass(TempH))
						{
							strcpy_s(nextSlot.CurPilot, pilotOdf);
							if (char *dot = strrchr(nextSlot.CurPilot, '.'))
								*dot = '\0';
						}

						// Load the unit into the transport. Digitize it!
						GetODFName(TempH, nextSlot.ODFName);
						nextSlot.CurHealth = GetCurHealth(TempH);
						nextSlot.CurAmmo = GetCurAmmo(TempH);
						nextSlot.CurScrap = GetScavengerCurScrap(TempH);
						nextSlot.UnitSize = UnitSize;

						for(int x = 0; x < MAX_HARDPOINTS; x++)
						{
							nextSlot.LocalAmmo[x] = GetCurLocalAmmo(TempH, x);
							GetObjInfo(TempH, ObjectInfoType(Get_Weapon0Config+x), nextSlot.Weapons[x]);
						}

						RemoveObject(TempH);
						iter->CurSize += UnitSize;
						iter->UnitCount++;
					}
				}
			}
		}

		if(iter->UnitCount) // This dropship has atleast 1 item in it. Watch for unloading.
		{
			if((!iter->IsUnloading) && (IsDeployed(iter->DropShipObject)) && ((GetCurrentCommand(iter->DropShipObject) == CMD_HUNT) || (IsPlayer(iter->DropShipObject))))
			{
				iter->IsUnloading = true;
				iter->UnitsGroup = GetFirstEmptyGroup(Team);

				// If the group is full, or there's no more free groups past Group 6 in MP, don't spawn any ships.
				if((GetGroupCount(Team, iter->UnitsGroup) == 10) || ((IsNetworkOn()) && (iter->UnitsGroup > 5)))
					continue;

				// Get the first empty group to use.
				//if((iter->UnitsGroup == -1) || (GetGroupCount(Team, iter->UnitsGroup) == 10))
				//	iter->UnitsGroup = GetFirstEmptyGroup(Team);
			}

			if(iter->IsUnloading)
			{
				if((GetCurrentCommand(iter->DropShipObject) == CMD_HUNT) && (!IsPlayer(iter->DropShipObject)))
					SetCommand(iter->DropShipObject, CMD_NONE, 1); // Stop current commands. Hope this doesn't cause VO spam.

				if((iter->UnitCount) && (((BZ1Helper::m_TurnCounter) % BZ1Helper::m_GameTPS) == 0)) // Once every second.
				{
					--iter->UnitCount;

					DropShipItem &nextSlot = iter->Cargo[iter->UnitCount];

					Vector DropShipPos = GetPosition(iter->DropShipObject);
					Vector DropLocation = GetPositionNear(DropShipPos, 40.0f, 50.0f);
					// Try to find a more suitable position if this one looks like it might be on a cliff.
					for(int Tries = 0; Tries < 10; Tries++)
					{
						float X = FLT_MAX;
						if(DropShipPos.y > DropLocation.y)
							X = DropShipPos.y - DropLocation.y;
						else
							X = DropLocation.y - DropShipPos.y;
						if(X < 25.0f)
							break;
						else
							DropLocation = GetPositionNear(DropShipPos, 40.0f, 50.0f);
					}
					DropLocation.y = (TerrainFindFloor(DropLocation.x, DropLocation.z) + 3.0f);

					if(DoesODFExist(nextSlot.ODFName))
					{
						if(BZ1Helper::m_TurnCounter > 1)
							BZ1Helper::Get().AddPilot(Team, GetPilotCost(nextSlot.ODFName));

						Handle TempShipH = BuildObject(nextSlot.ODFName, Team, DropLocation);
						SetCurHealth(TempShipH, nextSlot.CurHealth);
						SetCurAmmo(TempShipH, nextSlot.CurAmmo);
						SetPilotClass(TempShipH, nextSlot.CurPilot);
						SetScavengerCurScrap(TempShipH, nextSlot.CurScrap);

						//char CurWeapons[MAX_HARDPOINTS][MAX_ODF_LENGTH];

						ReplaceWeapons(TempShipH, nextSlot.Weapons, nextSlot.LocalAmmo);

						if(IsAround(TempShipH))
							SetGroup(TempShipH, iter->UnitsGroup);
					}
					else
					{
						FormatLogMessage("ERROR: ODF '%s' Doesn't Exist! How did it get in the dropship?!", nextSlot.ODFName);
					}

					iter->CurSize -= nextSlot.UnitSize;

					if(!iter->UnitCount)
						iter->IsUnloading = false;
				}
			}
		}
	}
}
