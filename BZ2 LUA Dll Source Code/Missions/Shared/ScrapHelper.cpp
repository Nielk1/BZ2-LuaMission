#include "BZ1Helper.h"
#include "ScrapHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

ScrapHelper *ScrapHelper::m_ScrapHelper;

ScrapHelper::ScrapHelper()
{
	m_ScrapHelper = this;
}

ScrapHelper::~ScrapHelper()
{
	 m_ScrapHelper = NULL;
}

bool ScrapHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		ScrapList.resize(size);
		if(size)
			Read(&ScrapList.front(), sizeof(ScrapClass)*size);
	}
	return true;
}

bool ScrapHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<ScrapClass>::iterator iter = ScrapList.begin(); iter != ScrapList.end(); ++iter)
		{
			ScrapClass &sao = *iter;
			ConvertHandles(&sao.ScrapObject, 1);
		}
	}
	return true;
}

bool ScrapHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(ScrapList.size());
		Write(&size, 1);
		if(size)
			Write(&ScrapList.front(), sizeof(ScrapClass)*size);
	}
	return true;
}

void ScrapHelper::AddScrapObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<ScrapClass>::iterator iter = ScrapList.begin(); iter != ScrapList.end(); ++iter)
	{
		if(iter->ScrapObject == h)
			return;
	}

	Vector Pos = GetPosition(h);
	bool RestrictHeight = false;
	GetODFBool(TempODF1, TempODF2, "ScrapClass", "RestrictHeight", &RestrictHeight, false);

	float RestrictSlope = 0.0f;
	GetODFFloat(TempODF1, TempODF2, "ScrapClass", "RestrictSlope", &RestrictSlope, 0.0f);
	RestrictSlope = clamp(RestrictSlope, 0.0f, 1.0f);

	if(RestrictHeight)
	{
		if((Pos.y > BZ1Helper::Get().GetMapInfo().MaxScrapHeight) || 
			(Pos.y < BZ1Helper::Get().GetMapInfo().MinScrapHeight))
		{
			BZ1Helper::Get().DeleteAfterDelay(h); // Delete me.
			return; // Done here.
		}
	}

	if(RestrictSlope)
	{
		Vector ANormal(0, 0, 0);
		float Height = 0.0f;
		TerrainGetHeightAndNormal(Pos, Height, ANormal, false);

		//FormatConsoleMessage("Normal: %f %f %f", ANormal.x, ANormal.y, ANormal.z);
		if(ANormal.y < RestrictSlope)
		{
			BZ1Helper::Get().DeleteAfterDelay(h); // Delete me.
			return; // Done here.
		}
	}

	ScrapList.resize(ScrapList.size()+1);
	ScrapClass &sao = ScrapList[ScrapList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(ScrapClass)); // Zero things out at start.

	sao.ScrapObject = h;

	// Now, if there's too many scrap items, lets remove some, starting with the oldest piece.
	if((ScrapList.size() >= 2048) || (BZ1Helper::TotalMapObjects > 3000)) // BZ2 has an Entity limit of 4096, this allows for up to 1096 ordnance and explosions at once.
	{
		FormatLogMessage("WARNING: Too many objects on map! Deleting oldest scrap!");

		if((IsAround(ScrapList[0].ScrapObject)) && (ScrapList[0].ScrapObject != h)) // If the oldest piece is still here...
			RemoveObject(ScrapList[0].ScrapObject);
	}
}

void ScrapHelper::Execute(BZ1Helper &bz1Helper)
{
	// pre-clean the vector
	ScrapList.erase(std::remove_if(ScrapList.begin(), ScrapList.end(), ShouldRemoveScrap), ScrapList.end());
}

// Deletes scrap until there is the specified amount.
void ScrapHelper::GetRidOfSomeScrap(const int ScrapLimit)
{
	int RemoveCount = ScrapList.size() - ScrapLimit;
	if (RemoveCount > 0)
	{
		for (int i = 0; i < RemoveCount; ++i)
			RemoveObject(ScrapList[i].ScrapObject);

		ScrapList.erase(ScrapList.begin(), ScrapList.begin() + RemoveCount);
	}
}

// Deletes all scrap within X distance of the specified position.
void ScrapHelper::ClearScrapAround(const Dist d, const Vector &pos, const float Percent)
{
	for (std::vector<ScrapClass>::iterator iter = ScrapList.begin(); iter != ScrapList.end(); ++iter)
	{
		if(GetDistance(iter->ScrapObject, pos) < d) // (IsAround(iter->ScrapObject)) && 
		{
			if((Percent >= 1.0f) || (GetRandomFloat(1.0f) <= Percent))
				RemoveObject(iter->ScrapObject);
		}
	}
}

/*
void ScrapHelper::RemoveScrapHandle(int index)
{
	int j;
	for(j=index; j<(m_NumScrapObjects-1); ++j)
		ScrapHandles[j] = ScrapHandles[j+1];
	--m_NumScrapObjects; // we deleted an entry

	ScrapHandles[m_NumScrapObjects] = 0;
}
*/