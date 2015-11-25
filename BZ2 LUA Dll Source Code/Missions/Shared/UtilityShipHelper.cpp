#include "BZ1Helper.h"
#include "UtilityShipHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

UtilityShipHelper *UtilityShipHelper::m_UtilityShipHelper;

UtilityShipHelper::UtilityShipHelper()
{
	m_UtilityShipHelper = this;
}

UtilityShipHelper::~UtilityShipHelper()
{
	 m_UtilityShipHelper = NULL;
}

bool UtilityShipHelper::Load(bool missionSave)
{
	if(!missionSave)
	{
		int size;
		Read(&size, 1);
		UtilityShipList.resize(size);
		if(size)
			Read(&UtilityShipList.front(), sizeof(UtilityShipClass)*size);
	}
	return true;
}

bool UtilityShipHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<UtilityShipClass>::iterator iter = UtilityShipList.begin(); iter != UtilityShipList.end(); ++iter)
		{
			UtilityShipClass &sao = *iter;
			ConvertHandles(&sao.UtilityShipObject, 1);
		}
	}
	return true;
}

bool UtilityShipHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(UtilityShipList.size());
		Write(&size, 1);
		if(size)
			Write(&UtilityShipList.front(), sizeof(UtilityShipClass)*size);
	}
	return true;
}

void UtilityShipHelper::AddUtilityShipObject(const Handle h, const int ObjClassHash, const int BZCClassHash, const int TempEscortPriority)
{
	for (std::vector<UtilityShipClass>::iterator iter = UtilityShipList.begin(); iter != UtilityShipList.end(); ++iter)
	{
		if(iter->UtilityShipObject == h)
			return;
	}

	UtilityShipList.resize(UtilityShipList.size()+1);
	UtilityShipClass &sao = UtilityShipList[UtilityShipList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(UtilityShipClass)); // Zero things out at start.

	sao.UtilityShipObject = h;

	if((BZCClassHash == BZCCLASS_SCAVENGER) && ((ObjClassHash == CLASS_SCAVENGER) || (ObjClassHash == CLASS_SCAVENGERH)))
		sao.ItsAScavenger = true;

	if(TempEscortPriority)
	{
		sao.EscortPriority = TempEscortPriority;
	}
	else
	{
		if(IsProducer(h))
			sao.EscortPriority = 3; // High
		else if(sao.ItsAScavenger)
			sao.EscortPriority = 2; // Medium
		else if((IsMinelayer(h)) || (IsTug(h)) || (IsAPC(h)))
			sao.EscortPriority = 1; // Low
		else
			sao.EscortPriority = 0; // None
	}
}

void UtilityShipHelper::Execute(BZ1Helper &bz1Helper)
{
	// pre-clean the vector
	UtilityShipList.erase(std::remove_if(UtilityShipList.begin(), UtilityShipList.end(), ShouldRemoveUtilityShip), UtilityShipList.end());

	// Loop over all lists, doing stuff.
	for (std::vector<UtilityShipClass>::iterator iter = UtilityShipList.begin(); iter != UtilityShipList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->UtilityShipObject);
		
		// If this is a Scavenger, perform Scrap Multiplication here. Only if MP is enabled!
		if((iter->ItsAScavenger) && (IsNetworkOn()))
		{
			int TempScrap = GetScavengerCurScrap(iter->UtilityShipObject);
			if(TempScrap < iter->ScrapAmount)
			{
				int GivenScrap = (iter->ScrapAmount - TempScrap) * BZ1Helper::Get().RefScrapMult();
				AddScrap(Team, GivenScrap);
			}
			iter->ScrapAmount = TempScrap;
		}
	}
}

Handle UtilityShipHelper::GetEscortTarget(const int Team, const int TeamFilter)
{
	const int NewTeamFilter = TeamFilter ? TeamFilter : -1;

	float TotalPriorities = 0.0f;

	for (std::vector<UtilityShipClass>::iterator iter = UtilityShipList.begin(); iter != UtilityShipList.end(); ++iter)
	{
		if((Team < 0) || (TeamFilterCheck(NewTeamFilter, GetTeamNum(iter->UtilityShipObject), Team))) // Team Filter matches.
			TotalPriorities += iter->EscortPriority;
	}

	float Random = GetRandomFloat(0.0f, TotalPriorities);

	for (std::vector<UtilityShipClass>::iterator iter = UtilityShipList.begin(); iter != UtilityShipList.end(); ++iter)
	{
		if((Team < 0) || (TeamFilterCheck(NewTeamFilter, GetTeamNum(iter->UtilityShipObject), Team))) // Team Filter matches.
		{
			Random = (Random - float(iter->EscortPriority));
			if((Random <= 0) || (iter == UtilityShipList.end()))
			{
				//FormatLogMessage("Random is: %f, Index is: %d Priority is: %d", Random, iter-UtilityShipList.begin(), iter->EscortPriority);
				return iter->UtilityShipObject;
			}
		}
	}
	return 0; // This should be impossible, but if all else fails, just return nothing.
}