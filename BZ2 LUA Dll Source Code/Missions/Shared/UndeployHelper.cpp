#include "BZ1Helper.h"
#include "UndeployHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

UndeployHelper::UndeployHelper()
{
}

bool UndeployHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		UndeployList.resize(size);
		if(size)
			Read(&UndeployList.front(), sizeof(UndeployClass)*size);
	}
	return true;
}

bool UndeployHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<UndeployClass>::iterator iter = UndeployList.begin(); iter != UndeployList.end(); ++iter)
		{
			UndeployClass &sao = *iter;
			ConvertHandles(&sao.UndeployingObject, 1);
		}
	}
	return true;
}

bool UndeployHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(UndeployList.size());
		Write(&size, 1);
		if(size)
			Write(&UndeployList.front(), sizeof(UndeployClass)*size);
	}
	return true;
}

void UndeployHelper::AddUndeployObject(const Handle h, const int Team, const char *ODFName)
{
	BZ1Helper::Get().DeleteAfterDelay(h); // Delete this the next game tick, since it's job is done now.
	Handle hbuild = 0;
	bool IsAnimated = false;
	float UndeployTime = 5.0f; // Default.

	// Loop through all 9 Base Slots, find the one that matches this undeploy item.
	for(int x = 0; x < 9; x++)
	{
		int i = x+1;
		if(x>2)
			i = x+6;
		char DesiredValue[MAX_ODF_LENGTH] = {0};
		sprintf_s(DesiredValue, "BZCundep%d", x+1);
		
		if((ODFName) && (_strnicmp(ODFName,DesiredValue, 9) == 0) && (IsAround(GetObjectByTeamSlot(Team, i))))
		{
			hbuild = GetObjectByTeamSlot(Team, i);
			break; // Exit early, we got it.
		}
	}

	// If this is around, add it to the list.
	if(IsAround(hbuild))
	{
		for (std::vector<UndeployClass>::iterator iter = UndeployList.begin(); iter != UndeployList.end(); ++iter)
		{
			if(iter->UndeployingObject == hbuild)
				return;
		}

		UndeployList.resize(UndeployList.size()+1);
		UndeployClass &sao = UndeployList[UndeployList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(UndeployClass)); // Zero things out at start.

		char TempODF1[MAX_ODF_LENGTH] = {0};
		char TempODF2[MAX_ODF_LENGTH] = {0};
		GetAndOpenODFNames(hbuild, TempODF1, TempODF2);

		char AnimationName[MAX_ODF_LENGTH] = {0};
		int AnimationCount = 0;
		GetODFInt(TempODF1, TempODF2, "GameObjectClass", "animCount", &AnimationCount);
		// Find out if it is animated!
		for (int i = 1; i < AnimationCount+1; i++) // Look through 16 animnames. If you made a unit that has more then 16 anims, 1: Congratulations, and 2: The deploy/undeploy one better be between 1 and 16.
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "animName%d", i);
			if(GetODFString(TempODF1, TempODF2, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, AnimationName))
			{
				if(((_stricmp(AnimationName,"deploy") == 0) && (AnimationCount == 1)) || (_stricmp(AnimationName,"undeploy") == 0))
				{
					IsAnimated = true;
					break; // We found it, abort!
				}
			}
		}

		// Pull out DeployTime. Look in Vehicle ODF.
		char UnDepTempODF[MAX_ODF_LENGTH] = {0};
		char UnDepTempODF2[MAX_ODF_LENGTH] = {0};
		GetObjInfo(hbuild, Get_ODF, UnDepTempODF);
		UnDepTempODF[1] = 'v';
		if(OpenODF2(UnDepTempODF))
		{
			if(GetODFString(UnDepTempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, UnDepTempODF2)) // Grab the classlabel.
			{
				strcat_s(UnDepTempODF2, ".odf"); // Add .odf to the end.
				OpenODF2(UnDepTempODF2);
			}

			// Look in Non Tracked (Scavenger/ScavengerH) form first.
			if((GetODFFloat(UnDepTempODF, UnDepTempODF2, "DeployableClass", "timeUndeploy", &UndeployTime, 5.0f)) || 
				(GetODFFloat(UnDepTempODF, UnDepTempODF2, "TrackedDeployableClass", "timeUndeploy", &UndeployTime, 5.0f)))
				if(UndeployTime < 0) // Sanity check.
					UndeployTime = 0;
		}

		sao.UndeployingObject = hbuild;
		sao.UndeployTime = ConvertToTurns(UndeployTime); // Now dependant on ODF setting. :) //50; // 5 seconds, 10 ticks per second

		// Special note: The Game will AV if there isn't an animation to play, but the engine seems to be able to use "deploy" anim, and simply reverses it, ONLY if it is the *ONLY* animation on the object. If there is > 1 animations, it won't work.
		if(IsAnimated)
		{
			sao.UndeployFrames = SetAnimation(sao.UndeployingObject,"undeploy", 1);
			StartAnimation(sao.UndeployingObject);
			sao.IsAnimated = true;
		}
	}
}

void UndeployHelper::Execute(BZ1Helper &bz1Helper)
{
	// pre-clean the vector
	UndeployList.erase(std::remove_if(UndeployList.begin(), UndeployList.end(), ShouldRemoveUndeployObject), UndeployList.end());

	//BZC Undeploy additions by GSH, adapted by General BlackDragon.
	for (std::vector<UndeployClass>::iterator iter = UndeployList.begin(); iter != UndeployList.end(); ++iter)
	{
		--iter->UndeployTime;

		if((iter->UndeployTime < 0) && // Wait for undeploy time, and if it's got an undeploy animation, wait for the animation to finish.
			((!iter->IsAnimated) || (GetAnimationFrame(iter->UndeployingObject, "undeploy") >= iter->UndeployFrames-1)))
		{
			//replace building form with vehicle form
		//	Matrix NULoc = GetMatrixPosition(iter->UndeployingObject);
		//	float HealthP = GetHealth(iter->UndeployingObject);
			int SaveTeam = GetTeamNum(iter->UndeployingObject);

			int SaveScrap = GetScrap(SaveTeam);
			int SavePilots = BZ1Helper::Get().GetPilot(SaveTeam);

			char NewODF[MAX_ODF_LENGTH] = {0};
			char TempODF[MAX_ODF_LENGTH] = {0};
			char TempODF2[MAX_ODF_LENGTH] = {0};
			Handle NewUnit = 0;

			// --- New Code from Nielk1. 
		//	GetObjInfo(m_UndeployingObjects[i], Get_ODF, TempODF);  
		//	strncpy_s(NewODF,TempODF,strlen(TempODF)-4); 
		//	NewODF[1] = 'v';
			// ---

			// Look for a specified UndeployClass = "odf" line in this objects ODF. If you DON'T find one, use traditional naming method.
			if((!GetODFString(iter->UndeployingObject, "GameObjectClass", "UndeployClass", MAX_ODF_LENGTH, NewODF)) || (!NewODF[0]))
			{
				GetODFName(iter->UndeployingObject, TempODF);
				strcpy_s(NewODF, TempODF);
				NewODF[1] = 'v';
			}

			int CategoryType = 0;
			bool AddToGroup = true;
			bool AddScrap = false;
			// Look for ODF Parameters. 
			sprintf_s(TempODF, "%s.odf", NewODF); // Add .odf to the end.
			if(OpenODF2(TempODF))
			{
				if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2)) // Grab the classlabel.
				{
					strcat_s(TempODF2, ".odf"); // Add .odf to the end.
					OpenODF2(TempODF2);
				}

				GetODFInt(TempODF, TempODF2, "GameObjectClass", "CategoryTypeOverride", &CategoryType);
			}

			if((CategoryType > 0) && (CategoryType < 10))
				AddToGroup = false;

			// Build the new unit and restore as much information as possible. Also add it to a group if it needs to be in one.
			if((NewODF[0]) && (DoesODFExist(NewODF)) && ((!AddToGroup) || (GetFirstEmptyGroup(SaveTeam) >= 0)))
			{
			//	RemoveObject(iter->UndeployingObject);
			//	NewUnit = BuildObject(NewODF, SaveTeam, NULoc);
			//	long SaveHealth = std::max<long>(long((GetCurHealth(NewUnit) * HealthP)),1);
			//	SetCurHealth(NewUnit, SaveHealth);

				NewUnit = BZ1Helper::Get().ReplaceObject(iter->UndeployingObject, NewODF);

				if(AddToGroup)
					SetBestGroup(NewUnit);

				char ObjClass[MAX_ODF_LENGTH] = {0};
				GetObjInfo(NewUnit, Get_GOClass, ObjClass);					
				if((GetODFBool(NewUnit, "ScavengerClass", "AddScrapHoldToTotal", &AddScrap)) || 
					(GetODFBool(NewUnit, "ScavengerHClass", "AddScrapHoldToTotal", &AddScrap)))
				{
					if((AddScrap) || (_stricmp(ObjClass, "CLASS_RECYCLERVEHICLE") == 0))
						SetScrap(SaveTeam, SaveScrap);
				}

				BZ1Helper::Get().SetPilot(SaveTeam, SavePilots);
			}
			else
			{
				if(!DoesODFExist(NewODF))
				{
					FormatLogMessage("ERROR: ODF '%s' Doesn't Exist! Cannot Undeploy!", NewODF); 
				}
				else
				{
					FormatLogMessage("ERROR: No Free Group! Cannot Undeploy!");

					if((GetLocalPlayerTeamNumber() == SaveTeam) && (IsNetworkOn()))
						AddToMessagesBox2("Can't Undeploy, No Free Groups.", RED);

					// If there's no free group, and it's animated, build it anyway???? (I confused, why did I do this? -GBD)
					/*
					if(iter->IsAnimated)
					{
						strncpy_s(NewODF,TempODF,strlen(TempODF)-4);
						NewUnit = BuildObject(NewODF, SaveTeam, NULoc);
						long SaveHealth = std::max<long>(long((GetCurHealth(NewUnit) * HealthP)), 1);
						SetCurHealth(NewUnit, SaveHealth);
						//SetBestGroup(NewUnit);
						char ObjClass[MAX_ODF_LENGTH] = {0};
						GetObjInfo(NewUnit, Get_GOClass, ObjClass);
						if((AddScrap) || (_stricmp(ObjClass, "CLASS_RECYCLERVEHICLE") == 0))
							SetScrap(SaveTeam, SaveScrap);
						BZ1Helper::Get().SetPilot(SaveTeam, SavePilots);
					}
					*/
				}
			}
		}
	}
}