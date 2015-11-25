#include "BZ1Helper.h"
#include "GeyserHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

GeyserHelper::GeyserHelper()
{
}

bool GeyserHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		GeyserList.resize(size);
		if(size)
			Read(&GeyserList.front(), sizeof(GeyserClass)*size);
	}
	return true;
}

bool GeyserHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<GeyserClass>::iterator iter = GeyserList.begin(); iter != GeyserList.end(); ++iter)
		{
			GeyserClass &sao = *iter;
			ConvertHandles(&sao.GeyserObject, 1);
		}
	}
	return true;
}

bool GeyserHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(GeyserList.size());
		Write(&size, 1);
		if(size)
			Write(&GeyserList.front(), sizeof(GeyserClass)*size);
	}
	return true;
}

void GeyserHelper::AddGeyserObject(const Handle h)
{
	for (std::vector<GeyserClass>::iterator iter = GeyserList.begin(); iter != GeyserList.end(); ++iter)
	{
		if(iter->GeyserObject == h)
			return;
	}

	GeyserList.resize(GeyserList.size()+1);
	GeyserClass &sao = GeyserList[GeyserList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(GeyserClass)); // Zero things out at start.

	sao.GeyserObject = h; // Add the geyser to the list.
}

void GeyserHelper::Execute(BZ1Helper &bz1Helper)
{
	// Place BZ2 Pools if we havn't already.
	if((BZ1Helper::Get().RefBuildPools()) && (!BZ1Helper::Get().RefSetupPools()))
	{
		char PoolODF[MAX_ODF_LENGTH] = {0};
		char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
		strcpy_s(mapTrnFile, GetMapTRNFilename());
		if(OpenODF2(mapTrnFile))
		{
			if(GetODFString(mapTrnFile, "DLL", "PoolODF", MAX_ODF_LENGTH, PoolODF))
			{
				if(!DoesODFExist(PoolODF)) // If this ODF doesn't exist, use default.
					strcpy_s(PoolODF, "BZCpool");
			}
		}

		for(int i = 0; i<bz1Helper.AIpathCount; ++i)
		{
			const char *label = bz1Helper.AIpathNames[i];
			if((label) && (_strnicmp(label,"pool",4) == 0))
			{
				// Place an object to test the position of nearby items.
				Handle TempTarget = BuildObject("BZCdummytap", 0, label);
				Handle TempStop = GetNearestBuilding(TempTarget);
			//	char ObjClass[MAX_ODF_LENGTH] = {0};
			//	GetObjInfo(TempStop, Get_GOClass, ObjClass);
				if(GetDistance(TempTarget, TempStop) > 64.0f) //|| (_stricmp(ObjClass, "CLASS_SIGN") == 0) || (_stricmp(ObjClass, "CLASS_DEPOSIT") == 0) || (_stricmp(ObjClass, "CLASS_SCRAP") == 0))
				{
					// Now, place down a "stamp" that has a terrain piece and is a guntower, this centralizes the position and flattens the ground for us.
					Vector TempPosition = GetPosition(TempTarget);
					TempPosition.y = TerrainFindFloor(TempPosition.x, TempPosition.z);
					Handle TempStamp = BuildObject("BZCpooltap", 0, TempPosition);
					TempPosition = GetPosition(TempStamp);
					TempPosition.y = TerrainFindFloor(TempPosition.x, TempPosition.z);
					RemoveObject(TempStamp);
					// Build our pool.
					BuildObject(PoolODF, 0, TempPosition);
				}
				RemoveObject(TempTarget);
			}
		}
		BZ1Helper::Get().RefSetupPools() = true;
	}

	// Cleanup GeyserList.
	GeyserList.erase(std::remove_if(GeyserList.begin(), GeyserList.end(), ShouldRemoveGeyser), GeyserList.end());

	// Place dummy geysers if we havn't already.
	for (std::vector<GeyserClass>::iterator iter = GeyserList.begin(); iter != GeyserList.end(); ++iter)
	{
		if(!iter->PlacedDummyGeyser)
		{
			Vector Pos = GetPosition(iter->GeyserObject);
			BuildObject("BZCtapgysr",0,Pos);
			iter->PlacedDummyGeyser = true;
		}
	}
}