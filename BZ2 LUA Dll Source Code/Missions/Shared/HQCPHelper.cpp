#include "BZ1Helper.h"
#include "HQCPHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

HQCPHelper::HQCPHelper()
{
}

bool HQCPHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		HQCPList.resize(size);
		if(size)
			Read(&HQCPList.front(), sizeof(HQCPClass)*size);
	}
	return true;
}

bool HQCPHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<HQCPClass>::iterator iter = HQCPList.begin(); iter != HQCPList.end(); ++iter)
		{
			HQCPClass &sao = *iter;
			ConvertHandles(&sao.CommandCenterObject, 1);
		}
	}
	return true;
}

bool HQCPHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(HQCPList.size());
		Write(&size, 1);
		if(size)
			Write(&HQCPList.front(), sizeof(HQCPClass)*size);
	}
	return true;
}

void HQCPHelper::AddHQCPObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<HQCPClass>::iterator iter = HQCPList.begin(); iter != HQCPList.end(); ++iter)
	{
		if(iter->CommandCenterObject == h)
			return;
	}

	HQCPList.resize(HQCPList.size()+1);
	HQCPClass &sao = HQCPList[HQCPList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(HQCPClass)); // Zero things out at start.

	GetODFFloat(TempODF1, TempODF2, "HQCPClass", "rangeScan", &sao.Range, 0.0f);
	GetODFString(TempODF1, TempODF2, "HQCPClass", "WarningSound", MAX_ODF_LENGTH, sao.WarningSound);
	GetODFFloat(TempODF1, TempODF2, "HQCPClass", "SoundDelay", &sao.SoundDelay, 10.0f);
	GetODFInt(TempODF1, TempODF2, "HQCPClass", "TargetTeamFilter", &sao.TeamFilter, 4);
	// Make 0 all teams. 
	if(sao.TeamFilter == 0)
		sao.TeamFilter = 7;

	GetODFInt(TempODF1, TempODF2, "HQCPClass", "TargetFilter", &sao.TargetFilter, 9);
	// Make 0 all classes. 
	if(sao.TargetFilter == 0)
		sao.TargetFilter = 63;

	sao.CommandCenterObject = h; // Add the comm tower to the list.
}

void HQCPHelper::Execute(BZ1Helper &bz1Helper)
{
	// pre-clean the vector
	HQCPList.erase(std::remove_if(HQCPList.begin(), HQCPList.end(), ShouldRemoveHQCP), HQCPList.end());

	for (std::vector<HQCPClass>::iterator iter = HQCPList.begin(); iter != HQCPList.end(); ++iter)
	{
		iter->Position = GetPosition(iter->CommandCenterObject);
		if(iter->ThreatTimer > 0)
			--iter->ThreatTimer;
	}
}

void HQCPHelper::ExecuteHandle(int HandleListIndex)
{
	bool Logging = BZ1Helper::Logging;

	BZ1Helper::ObjectClass &objectInfo = BZ1Helper::Get().RefHandleInfo(HandleListIndex);
	Handle TempH = objectInfo.ObjectHandle;

	//if((objectInfo.CanObjectify) && ((objectInfo.CanObjectifyDeployed) || (!IsDeployed(TempH)))) // Passes Deployed setting.
	//{
		bool ShouldObjectify[MAX_TEAMS] = {0};
		bool ShouldPlaySound[MAX_TEAMS] = {0};
		char TempWarningSound[MAX_ODF_LENGTH] = {0};
		int t = objectInfo.CurTeamNum;

		for (std::vector<HQCPClass>::iterator iter = HQCPList.begin(); iter != HQCPList.end(); ++iter)
		{
			int Team = GetTeamNum(iter->CommandCenterObject);

			if( (iter->Range > 0) && // Do we even bother?
				(GetMaxHealth(TempH) > 0) && // It's not indestructible.
				(BuildingHelper::Get().IsBuildingPowered(iter->CommandCenterObject)) && // Is this Command powered?
				(TeamFilterCheck(iter->TeamFilter, t, Team)) && // It passes teamfilter checks.
				(GetDistance3DSquared(iter->Position, objectInfo.Position) < Squared(iter->Range)) && // Is within Range.
				( (((iter->TargetFilter & 1) == 1) && (objectInfo.CategoryID == Vehicle)) || //IsVehicle(TempH)) ||  // Vehicle
				(((iter->TargetFilter & 2) == 2) && (objectInfo.CategoryID == Building)) || //IsBuilding(TempH)) ||  // Building
				(((iter->TargetFilter & 4) == 4) && (objectInfo.CategoryID == Person)) || //IsPerson(TempH)) ||  // Pilot
				(((iter->TargetFilter & 8) == 8) && (objectInfo.CategoryID == Powerup)) || //IsPowerup(TempH)) ||  // Powerup
				(((iter->TargetFilter & 16) == 16) && (objectInfo.CategoryID == Weapon)) || //IsWeapon(TempH)) ||  // Weapon
				(((iter->TargetFilter & 32) == 32) ) )  // Other
				)
			{
				ShouldObjectify[Team] = true;
				strcpy_s(TempWarningSound, iter->WarningSound);

				if(Logging)
					FormatLogMessage("Found A Valid HQCP Target Object: %08x, Team: %d, Targeting HQ is: %08x", TempH, t, iter->CommandCenterObject);

				if(iter->ThreatTimer <= 0)
				{
					ShouldPlaySound[Team] = true;
					if(!objectInfo.IsObjectified[Team])
						iter->ThreatTimer = ConvertToTurns(iter->SoundDelay);
				}
				break; // We found a HQCP Objectifying this.
			} // End objectification filters.
		} // End Loop over all Commands.

		// If this object passed in any of the HQCP Checks, Objectify/Deobjectify it as needed.
		for(int Team = 1; Team < MAX_TEAMS; Team++)
		{
			if(ShouldObjectify[Team])
			{
				if(!objectInfo.IsObjectified[Team])
				{
					for(int AllyTeams = 1; AllyTeams < MAX_TEAMS; AllyTeams++)
					{
						if((IsTeamAllied(Team, AllyTeams)) && (AllyTeams == GetLocalPlayerTeamNumber()))
						{
							SetObjectiveOn(TempH);
							if((TempWarningSound[0]) && (ShouldPlaySound[Team]))
							{
								if(DoesFileExist(TempWarningSound))
								{
									for(int AllyTeams = 0; AllyTeams < MAX_TEAMS; AllyTeams++)
										if((IsTeamAllied(Team, AllyTeams)) && (AllyTeams == GetLocalPlayerTeamNumber()))
											AudioMessage(TempWarningSound);
								}
								else
								{
									FormatLogMessage("ERROR: HQCP Warning Sound file: %s doesn't exist! Fix ODFs!", TempWarningSound); 
								}
							}
						}
					}

					objectInfo.IsObjectified[Team] = true;

					if(Logging)
						FormatLogMessage("Objectified a Valid HQCP Target Object: %08x, Team: %d", TempH, Team);
				}
			}
			else
			{
				if(objectInfo.IsObjectified[Team])
				{
					for(int AllyTeams = 1; AllyTeams < MAX_TEAMS; AllyTeams++)
						if((IsTeamAllied(Team, AllyTeams)) && (AllyTeams == GetLocalPlayerTeamNumber()))
							SetObjectiveOff(TempH);

					objectInfo.IsObjectified[Team] = false;

					if(Logging)
						FormatLogMessage("Removed a Valid HQCP Target Object: %08x, Team: %d", TempH, Team);
				}
			}
		}
//	}
}
