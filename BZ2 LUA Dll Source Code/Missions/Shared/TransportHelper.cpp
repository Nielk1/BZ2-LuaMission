#include "BZ1Helper.h"
#include "TransportHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

TransportHelper::TransportHelper()
{
}

bool TransportHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		TransportList.resize(size);
		if(size)
			Read(&TransportList.front(), sizeof(TransportClass)*size);
	}
	return true;
}

bool TransportHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<TransportClass>::iterator iter = TransportList.begin(); iter != TransportList.end(); ++iter)
		{
			TransportClass &sao = *iter;
			ConvertHandles(&sao.TransportObject, 1);
		}
	}
	return true;
}

bool TransportHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(TransportList.size());
		Write(&size, 1);
		if(size)
			Write(&TransportList.front(), sizeof(TransportClass)*size);
	}
	return true;
}

Handle TransportHelper::BuildTransportObject(const char *ODF, const int Team, const Matrix Position, const float DownTime, const float HeightOffset, const int PilotAmount, const int ScrapAmount, const int UnitsAmount, const bool SpawnPowerups, const bool Objectify, const bool AddToGroup)
{
	Handle TempH = BZ1Helper::Get().BuildAngleObject(ODF, Team, Position, NULL, 0, -1, false, false, HeightOffset);

	if(IsAround(TempH))
	{
		TransportList.resize(TransportList.size()+1);
		TransportClass &sao = TransportList[TransportList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(TransportClass)); // Zero things out at start.

		sao.TransportObject = TempH;
		sao.UnitsGroup = -1; // Default.
		sao.DownTime = ConvertToTurns(DownTime);
		sao.PilotAmount = PilotAmount;
		sao.ScrapAmount = ScrapAmount;
		sao.UnitsAmount = UnitsAmount;
		sao.SpawnPowerups = SpawnPowerups;

		if(Objectify)
			SetObjectiveOn(sao.TransportObject);

		if(AddToGroup)
			SetBestGroup(TempH);
		else
			Defend(TempH, 1); // Take out of F group, force to Deploy.
	}

	return TempH;
}

void TransportHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	TransportList.erase(std::remove_if(TransportList.begin(), TransportList.end(), ShouldRemoveTransport), TransportList.end());

	for (std::vector<TransportClass>::iterator iter = TransportList.begin(); iter != TransportList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->TransportObject);

		iter->Timer++;

		if((!IsDeployed(iter->TransportObject)) && (!iter->IsLanded))
			iter->LandTime++; // count time it takes to land.

		// If it's landed, and survived not being shot down.
		if ((IsDeployed(iter->TransportObject)) && (!iter->IsLanded))
		{
			if((Logging) && (!iter->IsLanded))
				FormatLogMessage("Deployed! Pilots Arriving Now!");

			iter->IsLanded = true;

			/*
			if((BZ1Helper::Get().GetPilot(Team)+iter->PilotAmount) <= BZ1Helper::Get().GetMaxPilot(Team))
				BZ1Helper::Get().SetPilot(Team, BZ1Helper::Get().GetPilot(Team)+ iter->PilotAmount);
			if((BZ1Helper::Get().GetPilot(Team)+iter->PilotAmount) > BZ1Helper::Get().GetMaxPilot(Team))
				BZ1Helper::Get().SetPilot(Team, BZ1Helper::Get().GetMaxPilot(Team));
			*/
			if(iter->PilotAmount > 0)
				BZ1Helper::Get().AddPilot(Team, iter->PilotAmount);

			if(iter->ScrapAmount > 0)
				AddScrap(Team, iter->ScrapAmount);
		}
		
		// Deploying units! Wait longer to takeoff if this is happening.
		if((iter->IsLanded) && (iter->UnitsCount != iter->UnitsAmount)) //&& ((iter->DownCounter <= iter->DownTime)
		{
			iter->DownCounter++;

			// Get the first empty group to use.
			if((iter->UnitsGroup == -1) || (GetGroupCount(Team, iter->UnitsGroup) == 10))
				iter->UnitsGroup = GetFirstEmptyGroup(Team);

			// If the group is full, or there's no more free groups past Group 6 in MP, don't spawn any ships.
			if((GetGroupCount(Team, iter->UnitsGroup) == 10) || ((IsNetworkOn()) && (iter->UnitsGroup > 5)))
				iter->UnitsCount = iter->UnitsAmount;

			// If we haven't spawned the number of ships desired yet, spawn them one at a time.
			if((iter->UnitsCount < iter->UnitsAmount) && (((BZ1Helper::m_TurnCounter + ConvertToTurns(5.0f)) % BZ1Helper::m_GameTPS) == 0)) // Once every second, after 5 seconds of being landed.
			{
				if(Logging)
					FormatLogMessage("Units Amount: %d, Counter Amount: %d", iter->UnitsAmount, iter->UnitsCount);

				char TempODF1[MAX_ODF_LENGTH] = {0};
				char TempODF2[MAX_ODF_LENGTH] = {0};
				GetAndOpenODFNames(iter->TransportObject, TempODF1, TempODF2);

				char NewShipODFArray[16][MAX_ODF_LENGTH] = {0};
				int NumberOfEntries = 0;
				for (int i = 0; i < 16; i++) // Look through 16 aentries.
				{
					char DesiredValue[MAX_ODF_LENGTH] = {0};
					sprintf_s(DesiredValue, "TransportVehicle%d", i+1);
					if((GetODFString(TempODF1, TempODF2, "TransportClass", DesiredValue, MAX_ODF_LENGTH, NewShipODFArray[i])) && (NewShipODFArray[i][0]))
						++NumberOfEntries;
					else
						break;

					GetWorldVariantODF(NewShipODFArray[i]);
				}

				Handle TempShipH = 0;

				int ShipSelection  = GetRandomInt(NumberOfEntries-1);
				ShipSelection = clamp(ShipSelection, 0, NumberOfEntries);

				Vector Reinforcement = GetPosition(iter->TransportObject);
				Vector Reinforcementspawn = GetPositionNear(Reinforcement, 40.0f, 50.0f);
				// Try to find a more suitable position if this one looks like it might be on a cliff.
				for(int Tries = 0; Tries < 10; Tries++)
				{
					/*
					float X = FLT_MAX;
					if(Reinforcement.y > Reinforcementspawn.y)
						X = Reinforcement.y - Reinforcementspawn.y;
					else
						X = Reinforcementspawn.y - Reinforcement.y;
					*/
					if(fabsf(Reinforcement.y - Reinforcementspawn.y) < 25.0f)
						break;
					else
						Reinforcementspawn = GetPositionNear(Reinforcement, 40.0f, 50.0f);
				}
				Reinforcementspawn.y = (TerrainFindFloor(Reinforcementspawn.x, Reinforcementspawn.z)+3);

				if(DoesODFExist(NewShipODFArray[ShipSelection]))
				{
					if(BZ1Helper::m_TurnCounter > 1)
						BZ1Helper::Get().AddPilot(Team, GetPilotCost(NewShipODFArray[ShipSelection]));

					TempShipH = BuildObject(NewShipODFArray[ShipSelection], Team, Reinforcementspawn);
				}
				else
				{
					FormatLogMessage("ERROR: ODF '%s' Doesn't Exist! Fix Transport ODF Entry %d!", NewShipODFArray[ShipSelection], ShipSelection+1);
				}

				if(IsAround(TempShipH))
					SetGroup(TempShipH, iter->UnitsGroup);

				++iter->UnitsCount;

				if((iter->SpawnPowerups) && ((iter->UnitsCount % ConvertToTurns(2.0f)) == 0))
				{
					// Build some Ammo/Health powerups too.
					char TempShipODF[MAX_ODF_LENGTH] = {0};
					float fRandomNum = GetRandomFloat(1.0f);
					if(fRandomNum < 0.5f)
						strcpy_s(TempShipODF, "apBZCrepa");
					else if(fRandomNum < 1.0f)
						strcpy_s(TempShipODF, "apBZCammo");
					Reinforcementspawn = GetPositionNear(Reinforcement, 30.0f, 40.0f);
					TempShipH = BuildObject(TempShipODF, Team, Reinforcementspawn);
				}
			}
		}

		// If the timer is == to Interval + Land Time + Down Time, it's time to takeoff. Also wait until all units have disembarked, if there are any.
		if ((iter->IsLanded) && (iter->DownTime > 0) && //(iter->UnitsCount == iter->UnitsAmount) && 
			//(iter->Timer == (iter->Interval + iter->LandTime + iter->DownTime + iter->DownCounter)))
			(iter->UnitsCount == iter->UnitsAmount) && // Finished Deploying all the units.
			(iter->Timer == (iter->LandTime + max(iter->DownTime, iter->DownCounter)))) // Landing time has passed.
		{
			if(Logging)
				FormatLogMessage("Bananas! We have Liftoff! Timer is: %f", iter->Timer); 

			Deploy(iter->TransportObject); // The proper way to order an AI unit to deploy. -GBD
			//SetObjectiveOff(iter->TransportObject); // Light 'em up!
		}

		// If the timer is == Interval + DownTime + 2 * LandTime, then it's reached upper atmosphere. Time to remove it and Reset things.
		if ((!IsDeployed(iter->TransportObject)) && (iter->DownTime > 0) && (iter->IsLanded) && 
			(iter->Timer == (iter->DownTime + iter->DownCounter + (2 * iter->LandTime))))
		{
			if(Logging)
				FormatLogMessage("Transport is Away! Timer is: %d", iter->Timer); 

			RemoveObject(iter->TransportObject); // This is all under an isAround transport object. :)
			
			//iter->Timer = iter->DownTime + (2*iter->LandTime);

			//if(Logging)
			//	FormatLogMessage("One Minute Reset! Timer is: %d, Resetting", iter->Timer); 
		}

	}
}
