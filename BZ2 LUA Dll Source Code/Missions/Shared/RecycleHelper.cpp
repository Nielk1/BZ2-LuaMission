#include "BZ1Helper.h"
#include "RecycleHelper.h"
#include "BuildingHelper.h"
#include "ProductionClassHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

RecycleHelper::RecycleHelper()
{
}

#if 0
bool RecycleHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		RecyclePointList.resize(size);
		if(size)
			Read(&RecyclePointList.front(), sizeof(RecyclePointClass)*size);
	}
	return true;
}

bool RecycleHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<RecyclePointClass>::iterator iter = RecyclePointList.begin(); iter != RecyclePointList.end(); ++iter)
		{
			RecyclePointClass &sao = *iter;
			ConvertHandles(&sao.RecycleObject, 1);
			ConvertHandles(&sao.RecyclePointObject, 1);
		}
	}
	return true;
}

bool RecycleHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(RecyclePointList.size());
		Write(&size, 1);
		if(size)
			Write(&RecyclePointList.front(), sizeof(RecyclePointClass)*size);
	}
	return true;
}

void RecycleHelper::AddRecyclePointObject(Handle h, int Team)
{
	for (std::vector<RecyclePointClass>::iterator iter = RecyclePointList.begin(); iter != RecyclePointList.end(); ++iter)
	{
		if(iter->RecycleObject == h)
			return;
	}

	RecyclePointList.resize(RecyclePointList.size()+1);
	RecyclePointClass &sao = RecyclePointList[RecyclePointList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(RecyclePointClass)); // Zero things out at start.

	sao.RecycleObject = h; // Add the Recycle Point to the list.

	/*
	for(int x = 0; x < MAX_TAPS; x++)
	{
		Handle TempTap = GetTap(ProductionClassHelper::Get().GetRecy(Team), x);
		if(TempTap == h)
		{
			BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = h;
			break;
		}
	}

	// Reset Recycler recyclepoint if present.
	if(IsAround(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint))
		ResetTeamSlot(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint); //!! If the recy hasn't been undeployed, keep it as the "primary" recycle point.
	*/
}
#endif

void RecycleHelper::Execute(BZ1Helper &bz1Helper)
{
		//!! Recode this using a track on all objects that can recycle and: 
		// AiCommand what = GetCurrentCommand(friend1);
		// if (what == CMD_RECYCLE)
		// Goto(me, recyclepoint);
		//DLLEXPORT AiCommand DLLAPI GetCurrentCommand(Handle me);
		//if((!IsAround((RecyclePoints[Team])[0])) && (m_NumRecyclePoints[Team] > 0)) RecyclePoint

	bool Logging = BZ1Helper::Logging; //true;

	int NumRecyclePoints[MAX_TEAMS] = {0};
	int RecycleIndex[MAX_TEAMS] = {0};

	for(int i = 0; i < BuildingHelper::Get().GetNumBuildingObjects(); i++)
	{
		BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(i);
		Handle TempH = buildingInfo.BuildingObject;
		int Team = buildingInfo.CurTeamNum;

		if(buildingInfo.IsRecyclePoint) //BuildingHelper::Get().GetBuildingRecyclePoint(i))
		{
			if((TempH == BZ1Helper::Get().GetRecy(Team)) && (TempH != GetObjectByTeamSlot(Team, DLL_TEAM_SLOT_RECYCLER)))
				RecycleIndex[Team] = i+1;

			if(RecycleIndex[Team] <= 0)
				RecycleIndex[Team] = i+1;

			++NumRecyclePoints[Team];
		}
	}

	// Build or Remove the Recycle Point for this team, as needed.
	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		if((NumRecyclePoints[Team]) && (RecycleIndex[Team]))
		{
			Handle RecyPoint = BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint;
			Handle TempH = BuildingHelper::Get().GetBuildingInfo(RecycleIndex[Team]-1).BuildingObject;

			if(TempH != GetObjectByTeamSlot(Team, DLL_TEAM_SLOT_RECYCLER))
			{
				if(!IsAround(RecyPoint))
				{
					char RecyPointODF[MAX_ODF_LENGTH] = {0};
					sprintf_s(RecyPointODF, "%cBZCrecyclepoint", BZ1Helper::Get().GetTeamInfo(Team).TeamRace);
					if(!DoesODFExist(RecyPointODF))
						sprintf_s(RecyPointODF, "BZCrecyclepoint");

					BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = BuildObject(RecyPointODF, Team, TempH);

					if(Logging)
						FormatLogMessage("Building Recycle Point on Team: %d at Index: %d, ODF: %s", Team, RecycleIndex[Team]-1, GetODFName(TempH));
				}
				else
				{
					if(!close_enough(GetPosition(RecyPoint), GetPosition(TempH), 10.0f))
					{
						char RecyPointODF[MAX_ODF_LENGTH] = {0};
						sprintf_s(RecyPointODF, "%cBZCrecyclepoint", BZ1Helper::Get().GetTeamInfo(Team).TeamRace);
						if(!DoesODFExist(RecyPointODF))
							sprintf_s(RecyPointODF, "BZCrecyclepoint");

						//SetPosition(RecyPoint, TempH);
						RemoveObject(RecyPoint);
						BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = BuildObject(RecyPointODF, Team, TempH);
						//ResetTeamSlot(RecyPoint);

						if(Logging)
							FormatLogMessage("Moving Recycle Point on Team: %d to Index: %d", Team, RecycleIndex[Team]-1);
					}
				}
			}
		}
		else
		{
			if(IsAround(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint))
			{
				RemoveObject(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint);
				
				if(Logging)
					FormatLogMessage("Removing Recycle Point on Team %d", Team);
			}
		}
	}

	/*
	// Sweep through the Recycle Points and remove any of them that are no longer around
	std::vector<RecyclePointClass>::iterator validEnd = RecyclePointList.begin();
	std::vector<RecyclePointClass>::iterator lastValid = RecyclePointList.begin();
	for (std::vector<RecyclePointClass>::iterator iter = RecyclePointList.begin(); iter != RecyclePointList.end(); ++iter)
	{
		// If the tower is around...
		if (IsAround(iter->RecycleObject))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			if(Logging)
				FormatLogMessage("List Cleanup Removing Recycle Point %d, handle: %08x", iter - RecyclePointList.begin(), iter->RecyclePointObject);

			if(IsAround(iter->RecyclePointObject))
				RemoveObject(iter->RecyclePointObject);
		}
	}
	// Now erase any invalid entries left behind
	RecyclePointList.erase(validEnd, RecyclePointList.end());
	//RecyclePointList.erase(std::remove_if(RecyclePointList.begin(), RecyclePointList.end(), ShouldRemoveRecyclePoint), RecyclePointList.end());

	for (std::vector<RecyclePointClass>::iterator iter = RecyclePointList.begin(); iter != RecyclePointList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->RecycleObject);

		if(!IsAround(iter->RecyclePointObject))
		{
			iter->RecyclePointObject = BuildObject("BZCrecyclepoint", Team, iter->RecycleObject);

			if(Logging)
				FormatLogMessage("Building Recycle Point for Handle: %08x!", iter->RecycleObject);
		}

		if((!IsAround(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint)) && (IsAround(iter->RecyclePointObject)))
		{
			bool FoundRecycler = false;

			// We heard you like for(), so we put a for() in your for(), so u can loop while you loop.
			for (std::vector<RecyclePointClass>::iterator iterB = RecyclePointList.begin(); iterB != RecyclePointList.end(); ++iterB)
			{
				if((iterB->RecycleObject == BZ1Helper::Get().GetRecy(Team)) && (BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint != iter->RecyclePointObject)) // This is the recycler, Set it as primary Recycle point.
				{
					BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = iterB->RecyclePointObject; // Set this as your new Primary Recycle Point.
					ResetTeamSlot(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint); // Make it 1 again? 
					FoundRecycler = true;

					if(Logging)
						FormatLogMessage("Found the Recycler! Setting as Recycle Point!");
				}
			}
			if(FoundRecycler)
				continue; // Skip to next item early.

			// Didn't find recycler, use oldest Silo instead.
			if(Logging)
				FormatLogMessage("Found Oldest Silo, Set as Recycle Point!");

			BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = iter->RecyclePointObject; // Set this as your new Primary Recycle Point.
			ResetTeamSlot(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint); // Make it 1 again? 
		}

		// Recycler overrides older selection.
		if((iter->RecycleObject == BZ1Helper::Get().GetRecy(Team)) && (BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint != iter->RecyclePointObject)) // This is the recycler, Set it as primary Recycle point.
		{
			BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = iter->RecyclePointObject; // Set this as your new Primary Recycle Point.
			ResetTeamSlot(iter->RecyclePointObject); // Make it 1 again? 

			if(Logging)
				FormatLogMessage("Found the Recycler! Re-Setting as Recycle Point!");
		}
	}
	*/

	/* // Old code is old...
	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		if((!IsAround(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint)) && (RecyclePointList.size() > 0) ) ///&& (GetObjectByTeamSlot(Team, DLL_TEAM_SLOT_RECYCLER) == 0))
		{
			for (std::vector<RecyclePointClass>::iterator iter = RecyclePointList.begin(); iter != RecyclePointList.end(); ++iter)
			{
				if((IsAround(iter->RecyclePointObject)) && (GetTeamNum(iter->RecyclePointObject) == Team)) // Only proceed if the object is still around.
				{
					if(Logging)
						PrintConsoleMessage("Found another Recycle Point!");

					BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint = iter->RecyclePointObject; // Set this as your new Primary Recycle Point.
					ResetTeamSlot(BZ1Helper::Get().RefTeamInfo(Team).RecyclePoint); // Make it 1 again? 
					break;
				}
			}
		}
	}
	*/
}
