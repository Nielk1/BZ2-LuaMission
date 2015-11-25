#include "BZ1Helper.h"
#include "PowerHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"

PowerHelper *PowerHelper::m_PowerHelper;

PowerHelper::PowerHelper()
{
	m_PowerHelper = this;
}
PowerHelper::~PowerHelper()
{
	m_PowerHelper = NULL;
}

bool PowerHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		PowerList.resize(size);
		if(size)
			Read(&PowerList.front(), sizeof(PowerClass)*size);
	}
	return true;
}

bool PowerHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<PowerClass>::iterator iter = PowerList.begin(); iter != PowerList.end(); ++iter)
		{
			PowerClass &sao = *iter;
			ConvertHandles(&sao.PowerObject, 1);
		}
	}
	return true;
}

bool PowerHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(PowerList.size());
		Write(&size, 1);
		if(size)
			Write(&PowerList.front(), sizeof(PowerClass)*size);
	}
	return true;
}

void PowerHelper::AddPowerObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<PowerClass>::iterator iter = PowerList.begin(); iter != PowerList.end(); ++iter)
	{
		if(iter->PowerObject == h)
			return;
	}

	PowerList.resize(PowerList.size()+1);
	PowerClass &sao = PowerList[PowerList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(PowerClass)); // Zero things out at start.

	GetODFBool(TempODF1, TempODF2, "BuildingClass", "DoBettyLost", &sao.DoBettyLost, true);

	if((GetODFFloat(TempODF1, TempODF2, "PowerPlantClass", "powerRadius", &sao.PowerRange, ScaleDistance(100.0f))) || 
		(GetODFFloat(TempODF1, TempODF2, "PowerPlantClass", "powerRange", &sao.PowerRange, ScaleDistance(100.0f))))
	{
		// Got it.
	}

	GetODFInt(TempODF1, TempODF2, "PowerPlantClass", "PowerClassMask", &sao.PowerMask, 0);
	GetODFInt(TempODF1, TempODF2, "PowerPlantClass", "PowerClassMatch", &sao.PowerMatch, 0);
	GetODFInt(TempODF1, TempODF2, "PowerPlantClass", "PowerClassProvides", &sao.PowerProvide, 0);

	sao.PowerObject = h;
	sao.Position = GetPosition(h);
}

void PowerHelper::DeletePowerObject(const Handle h)
{
	for (std::vector<PowerClass>::iterator iter = PowerList.begin(); iter != PowerList.end(); ++iter)
	{
		if(iter->PowerObject == h)
		{
			if(IsAround(iter->PowerObject)) // Note: IsAround passes true for objects being Recycled in DeleteObject() function.
				iter->WasDemolished = true;

			return;
		}
	}
}

void PowerHelper::Execute(BZ1Helper &bz1Helper)
{
	//bool Logging = BZ1Helper::Logging;

	// Sweep through the Shield Towers and remove any of them that are no longer around
	std::vector<PowerClass>::iterator validEnd = PowerList.begin();
	std::vector<PowerClass>::iterator lastValid = PowerList.begin();
	for (std::vector<PowerClass>::iterator iter = PowerList.begin(); iter != PowerList.end(); ++iter)
	{
		// If the tower is around...
		if (IsAround(iter->PowerObject))
		{
			iter->PowerTeam = GetTeamNum(iter->PowerObject);
			iter->IsPowered = BuildingHelper::Get().IsBuildingPowered(iter->PowerObject);
			iter->Position = GetPosition(iter->PowerObject);

			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			// Play the proper destruction wave file for "supplydepot" power plants. Cna't do this in DeleteObject because it runs over all objects when a Resync happens in Multiplayer. (Basically a Resync deletes and re places all objects on the map)
			if((iter->PowerTeam == GetLocalPlayerTeamNumber()) && (!iter->WasDemolished) && (!iter->DoBettyLost))
			{
				char SoundFile[MAX_ODF_LENGTH] = {0};
				char EventODF[MAX_ODF_LENGTH] = {0};
				sprintf_s(EventODF, "%cevent.odf", BZ1Helper::Get().RefTeamInfo(iter->PowerTeam).TeamRace);

				if(!DoesODFExist(EventODF))
					strcpy_s(EventODF, "aevent.odf");

				if(OpenODF2(EventODF))
				{
					if(GetODFString(EventODF, "BettyVoice", "EVENT_SOUND_11", MAX_ODF_LENGTH, SoundFile)) // Grab the sound file, with allowance for a large filename.
					{
						if(SoundFile[0])
						{
							if(DoesFileExist(SoundFile))
								AudioMessage(SoundFile);
							else
								FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix Event_Sound_11 in ODF: %s", SoundFile, EventODF);
						}
					}
				//	CloseODF(EventODF);
				}
			}
		}
	}
	// Now erase any invalid entries left behind
	PowerList.erase(validEnd, PowerList.end());

}
