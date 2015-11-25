#include "BZ1Helper.h"
#include "JammerHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

JammerHelper::JammerHelper()
{
}

bool JammerHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		JammerList.resize(size);
		if(size)
			Read(&JammerList.front(), sizeof(JammerClass)*size);
	}
	return true;
}

bool JammerHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<JammerClass>::iterator iter = JammerList.begin(); iter != JammerList.end(); ++iter)
		{
			JammerClass &sao = *iter;
			ConvertHandles(&sao.JammerObject, 1);
		}
	}
	return true;
}

bool JammerHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(JammerList.size());
		Write(&size, 1);
		if(size)
			Write(&JammerList.front(), sizeof(JammerClass)*size);
	}
	return true;
}

void JammerHelper::AddJammerObject(const Handle h)
{
	for (std::vector<JammerClass>::iterator iter = JammerList.begin(); iter != JammerList.end(); ++iter)
	{
		if(iter->JammerObject == h)
			return;
	}

	int TempIndex = -1;
	for(int x = 0;x<MAX_TAPS;x++)
	{
		char ObjClass[MAX_ODF_LENGTH] = {0};
		GetObjInfo(GetTap(h, x), Get_GOClass, ObjClass);
		if(_stricmp(ObjClass, "CLASS_JAMMER") == 0)
		{
			TempIndex = x;
			break;
		}
		if(x == MAX_TAPS)
			return; // No Jammer class tap. Abort!
	}

	JammerList.resize(JammerList.size()+1);
	JammerClass &sao = JammerList[JammerList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(JammerClass)); // Zero things out at start.

	sao.JammerObject = h;
	sao.JammerIndex = TempIndex;
	//GetObjInfo(GetTap(h, TempIndex), Get_CFG, sao.JammerODF);
	GetODFName(GetTap(h, TempIndex), sao.JammerODF);
}

void JammerHelper::Execute(BZ1Helper &bz1Helper)
{
	//bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	JammerList.erase(std::remove_if(JammerList.begin(), JammerList.end(), ShouldRemoveJammer), JammerList.end());

	for (std::vector<JammerClass>::iterator iter = JammerList.begin(); iter != JammerList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->JammerObject);
		Handle TempH = GetTap(iter->JammerObject, iter->JammerIndex);
		if(IsAround(TempH))
		{
			char ObjClass[MAX_ODF_LENGTH] = {0};
			GetObjInfo(TempH, Get_GOClass, ObjClass);

			if(BuildingHelper::Get().IsBuildingPowered(iter->JammerObject))
			{
				if(_stricmp(ObjClass, "CLASS_JAMMER") != 0)
				{
					Matrix TapPos = GetMatrixPosition(TempH);
					RemoveObject(TempH);
					TempH = BuildObject(iter->JammerODF, Team, TapPos);
					SetTap(iter->JammerObject, iter->JammerIndex, TempH);
				}
			}
			else
			{
				if(_stricmp(ObjClass, "CLASS_JAMMER") == 0)
				{
					Matrix TapPos = GetMatrixPosition(TempH);
					RemoveObject(TempH);
					TempH = BuildObject("BZCDummyTap", Team, TapPos);
					SetTap(iter->JammerObject, iter->JammerIndex, TempH);
				}
			}
		}
	}
}
