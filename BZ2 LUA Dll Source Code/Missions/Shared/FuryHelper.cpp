#include "BZ1Helper.h"
#include "FuryHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

FuryHelper::FuryHelper()
{
}

bool FuryHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		FuryList.resize(size);
		if(size)
			Read(&FuryList.front(), sizeof(FuryClass)*size);
	}
	return true;
}

bool FuryHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<FuryClass>::iterator iter = FuryList.begin(); iter != FuryList.end(); ++iter)
		{
			FuryClass &sao = *iter;
			ConvertHandles(&sao.FuryObject, 1);
		}
	}
	return true;
}

bool FuryHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(FuryList.size());
		Write(&size, 1);
		if(size)
			Write(&FuryList.front(), sizeof(FuryClass)*size);
	}
	return true;
}


void FuryHelper::AddFuryObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<FuryClass>::iterator iter = FuryList.begin(); iter != FuryList.end(); ++iter)
	{
		if(iter->FuryObject == h)
			return;
	}

	FuryList.resize(FuryList.size()+1);
	FuryClass &sao = FuryList[FuryList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(FuryClass)); // Zero things out at start.

	GetODFFloat(TempODF1, TempODF2, "FuryClass", "TurnChance", &sao.TurnChance, 0.1f);
	sao.TurnChance = clamp(sao.TurnChance, 0.0f, 1.0f);
	GetODFString(TempODF1, TempODF2, "FuryClass", "TurnODF", MAX_ODF_LENGTH, sao.ChangeODF, "hvsav");

	sao.FuryObject = h; // Add the fury to the list.
}

void FuryHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	FuryList.erase(std::remove_if(FuryList.begin(), FuryList.end(), ShouldRemoveFury), FuryList.end());

	for (std::vector<FuryClass>::iterator iter = FuryList.begin(); iter != FuryList.end(); ++iter)
	{
		// Once a minute, if it's not a player, and not empty, % chance, and it's not on Team 0.
		if( (BZ1Helper::m_TurnCounter) && // Make sure this isn't 0.
			(BZ1Helper::m_TurnCounter % ConvertToTurns(60.0f) == 0) && 
			(IsAliveAndPilot2(iter->FuryObject)) && 
			(GetTeamNum(iter->FuryObject) > 0) && 
			(!IsPlayer(iter->FuryObject)))
		{
			char TempTeamRace = 0;
			//sprintf_s(TempTeamRace, "\0"); // Fill this with nothing.
			int SpawnTeam = 15; // Default fury team.
			float Chance = GetRandomFloat(1.0f);

			if(Logging)
				FormatLogMessage("Random Fury Chance Index: %d Is: %g, TurnChance is: %g", iter - FuryList.begin(), Chance, iter->TurnChance); 

			// % chance every minute, per fury. 
			if(Chance < iter->TurnChance)
			{
				for(int x = 0; x < MAX_TEAMS; x++)
				{
					TempTeamRace = GetRaceOfTeam(x);

					if(Logging)
						FormatLogMessage("Team Race Is: %c", TempTeamRace); 

					// If this race is Fury, switch to it instead.
					//if((_strnicmp(TempTeamRace,"h",1) == 0) || (_strnicmp(TempTeamRace,"g",1) == 0)) 
					if((TempTeamRace == 'h') || (TempTeamRace == 'g'))
					{
						SpawnTeam = x; //Team; //-- Team? What was I, drunk? -GBD

						if(Logging)
							FormatLogMessage("Fury Team Is: %d", x); 

						break; // stop loop. 
					}
				}

				// Easter Egg, of sorts? 0.1% chance? 
				if(Chance < 0.001f)
				{
					float HealthP = GetHealth(iter->FuryObject);
					float AmmoP = GetAmmo(iter->FuryObject);
					Matrix FuryPosition = GetMatrixPosition(iter->FuryObject);
					RemoveObject(iter->FuryObject);
					Handle TempH = BuildObject("hvmother", SpawnTeam, FuryPosition);
					SetHealth(TempH, HealthP);
					SetAmmo(TempH, AmmoP);
				}
				else
				{
					if(iter->ChangeODF[0])
					{
						if(DoesODFExist(iter->ChangeODF))
							BZ1Helper::Get().ReplaceObject(iter->FuryObject, iter->ChangeODF, SpawnTeam);
						else
							FormatLogMessage("ERROR: Fury Turn ODF '%s' Doesn't Exist. Fix ODFs!", iter->ChangeODF);
					}
				}
			}
		}
	}
}
