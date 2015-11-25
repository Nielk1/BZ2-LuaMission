#include "BZ1Helper.h"
#include "LPadHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

LPadHelper::LPadHelper()
{
}

bool LPadHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		LPadList.resize(size);
		if(size)
			Read(&LPadList.front(), sizeof(LPadClass)*size);
	}
	return true;
}

bool LPadHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<LPadClass>::iterator iter = LPadList.begin(); iter != LPadList.end(); ++iter)
		{
			LPadClass &sao = *iter;
			ConvertHandles(&sao.LPadObject, 1);
			ConvertHandles(&sao.TransportObject, 1);
		}
	}
	return true;
}

bool LPadHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(LPadList.size());
		Write(&size, 1);
		if(size)
			Write(&LPadList.front(), sizeof(LPadClass)*size);
	}
	return true;
}

void LPadHelper::AddLPadObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<LPadClass>::iterator iter = LPadList.begin(); iter != LPadList.end(); ++iter)
	{
		if(iter->LPadObject == h)
			return;
	}

	LPadList.resize(LPadList.size()+1);
	LPadClass &sao = LPadList[LPadList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(LPadClass)); // Zero things out at start.

	// Look for new ODF Parameters. 
	float TempInitDelay = 0.0f;
	GetODFFloat(TempODF1, TempODF2, "LaunchPadClass", "TransportInitDelay", &TempInitDelay, 0.0f);
	sao.TransportTimer = ConvertToTurns(TempInitDelay); // Start timer here.

	float TempInterval = 0.0f;
	GetODFFloat(TempODF1, TempODF2, "LaunchPadClass", "TransportInterval", &TempInterval, 60.0f);
	if(TempInterval < 60.0f)
		TempInterval = 60.0f; // Lock it down to a sane minimum. 
	sao.TransportInterval = ConvertToTurns(TempInterval); // Convert to Turns.

	GetODFInt(TempODF1, TempODF2, "LaunchPadClass", "PilotAmount", &sao.PilotAmount);
	GetODFInt(TempODF1, TempODF2, "LaunchPadClass", "ScrapAmount", &sao.ScrapAmount);
	GetODFInt(TempODF1, TempODF2, "LaunchPadClass", "UnitsAmount", &sao.UnitsAmount);

	GetODFBool(TempODF1, TempODF2, "LaunchPadClass", "SpawnPowerups", &sao.SpawnPowerups, false);
	GetODFBool(TempODF1, TempODF2, "LaunchPadClass", "ObjectifyTransport", &sao.ObjectifyTransport, false);
	GetODFFloat(TempODF1, TempODF2, "LaunchPadClass", "DownTime", &sao.TransportDownTime, 3.0f);

	/*
	sao.TransportDownTime = sao.TransportDownTime * BZ1Helper::m_GameTPS;
	if(sao.TransportDownTime > sao.TransportInterval)
		sao.TransportDownTime = sao.TransportInterval; // Lock it down to a sane maximum. 

	/*
	GetODFString(TempODF1, TempODF2, "LaunchPadClass", "TransportName", MAX_ODF_LENGTH, &sao.TransportName);
	*/

	sao.LPadObject = h; // Add the comm tower to the list.
}

void LPadHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	/*
	for(int Team=0;Team<MAX_TEAMS;Team++)
	{
		int MaxCount = BZ1Helper::Get().GetNumHandles();
		for(int i = 0; i < MaxCount; i++)
		{
			Handle TempH = BZ1Helper::Get().RefObjectHandle(i);
			int Team = GetTeamNum(TempH);
			char ObjClass[MAX_ODF_LENGTH] = {0};
			GetObjInfo(TempH, Get_GOClass, ObjClass);

			if((!BZ1Helper::Get().RefHasBarracks(Team)) && (_stricmp(ObjClass, "CLASS_BARRACKS") == 0))
			{
				BZ1Helper::Get().RefHasBarracks(Team) = true;
				break;
			}
			else if(i == MaxCount)
			{
				BZ1Helper::Get().RefHasBarracks(Team) = false;
			}
		}
	}
	*/
	
	// Sweep through the LPads and remove any of them that are no longer around
	std::vector<LPadClass>::iterator validEnd = LPadList.begin();
	std::vector<LPadClass>::iterator lastValid = LPadList.begin();
	for (std::vector<LPadClass>::iterator iter = LPadList.begin(); iter != LPadList.end(); ++iter)
	{
		// If the tower is around...
		if (IsAround(iter->LPadObject))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			if(Logging)
				FormatLogMessage("List Cleanup Removing LPad %d, handle: %08x", iter - LPadList.begin(), iter->LPadObject);

			if(IsAround(iter->TransportObject))
			{
				if(GetMaxHealth(iter->TransportObject) > 0)
					Damage(iter->TransportObject, GetMaxHealth(iter->TransportObject)+9000); // It's over 9000!
				else
					RemoveObject(iter->TransportObject);
			}
		}
	}
	// Now erase any invalid entries left behind
	LPadList.erase(validEnd, LPadList.end());

	// Loop over launch pads, do stuff.
	for (std::vector<LPadClass>::iterator iter = LPadList.begin(); iter != LPadList.end(); ++iter)
	{
		if(!iter->TransportInterval)
			continue; // Well, Poop...

	//	bool DontCount = false;

		// Only start a transport if 1: The launch pad is alive, 2: The team has a barracks, and 3: If their pilots arn't already maxed out, and 4: If this lpad's transport odf is valid.
		int Team = GetTeamNum(iter->LPadObject);
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		// If your curr pilots are = max, scrap = max, or you don't have barracks, and there is NOT already a transport in progress, stall the transport.
		if((((!iter->PilotAmount) || ((teamInfo.CurrPilots >= teamInfo.MaxPilots) || (teamInfo.NumBarracks <= 0))) && // 0 PilotAmount or Pilots == MaxPilots. Also missing Barracks.
			((!iter->ScrapAmount) || ((GetScrap(Team) >= GetMaxScrap(Team)))) && // 0 Scrap Amount or Scrap == MaxScrap
			(!iter->UnitsAmount)) || (IsAround(iter->TransportObject))) // Not dropping off units. // Transport is already around.
			continue;

		iter->TransportTimer--;

		// Time to deploy it.
		if(iter->TransportTimer <= 0)
		{
			if(Logging)
				FormatLogMessage("Bubbles!");

			// If this LPad's transport is still around, stall the timer.
			if(IsAround(iter->TransportObject))
			{
				iter->TransportTimer++;
				continue;
			}

			// Build the transport. Make sure this race has one. Look at ODF value, then for race, then for NSDF one, then nothing...
			char HTransportODF[MAX_ODF_LENGTH] = {0};
			GetODFString(iter->LPadObject, "LaunchPadClass", "TransportName", MAX_ODF_LENGTH, HTransportODF);
			
			if((!HTransportODF[0]) || (!DoesODFExist(HTransportODF)))
			{
				char Race = GetRace(iter->LPadObject); // Pull out it's race. 
				sprintf_s(HTransportODF, "%cvtrans_lnto", Race);
				if(!DoesODFExist(HTransportODF))
			//	{
			//		sprintf_s(HTransportODF, "avtrans_lnto"); // Fall back to Default.
			//		if(!DoesODFExist(HTransportODF))
						continue; // F it, skip rest of code.
			//	}
			}

			char TransODF[MAX_ODF_LENGTH] = {0};
			sprintf_s(TransODF, "%s.odf", HTransportODF);
			float Altitude = 0.0f;
			GetODFFloat(TransODF, "HoverCraftClass", "setAltitude", &Altitude, 300.0f);
			Matrix TransPos = GetODFPositionOffset(iter->LPadObject, 0, Vector(0, Altitude, 0));

			iter->TransportObject = BZ1Helper::Get().BuildTransportObject(HTransportODF, Team, TransPos, iter->TransportDownTime, 0.0f, iter->PilotAmount, iter->ScrapAmount, iter->UnitsAmount, iter->SpawnPowerups, iter->ObjectifyTransport, false);

			iter->TransportTimer = iter->TransportInterval; // Reset Timer.

			/*
			iter->TransportObject = BuildObject(HTransportODF, Team, TransPos);
			Defend(iter->TransportObject, 1); // Take out of F group, force to Deploy.
			iter->TransportInProgress = true;
			iter->TransportLandTime = 0;
			iter->TransportDownCounter = 0;
			iter->UnitsCount = 0;
			iter->UnitsGroup = -1;
			//SetObjectiveOn(Transport[m_CompTeam]); // Light 'em up!
			//SetCommand(iter->TransportObject, CMD_DROPOFF, 1); // This object automatically deploys, so no need to tell it here. :) -GBD
			*/
		}

		/* // Moved to TransportHelper code. -GBD

		if(IsAround(iter->TransportObject))
		{
			if((!IsDeployed(iter->TransportObject)) && (!iter->TransportLanded))
				iter->TransportLandTime++; // count time it takes to land.

			// If it's landed, and survived not being shot down.
			if ((IsDeployed(iter->TransportObject)) && (!iter->TransportLanded))
			{
				if((Logging) && (!iter->TransportLanded))
					FormatLogMessage("Deployed! Pilots Arriving Now!");

				iter->TransportLanded = true;

				if((BZ1Helper::Get().GetPilot(Team)+iter->PilotAmount) <= BZ1Helper::Get().GetMaxPilot(Team))
					BZ1Helper::Get().SetPilot(Team, BZ1Helper::Get().GetPilot(Team)+ iter->PilotAmount);
				if((BZ1Helper::Get().GetPilot(Team)+iter->PilotAmount) > BZ1Helper::Get().GetMaxPilot(Team))
					BZ1Helper::Get().SetPilot(Team, BZ1Helper::Get().GetMaxPilot(Team));

				if(iter->ScrapAmount > 0)
					AddScrap(Team, iter->ScrapAmount);

			}

			// Deploying units! Wait longer to takeoff if this is happening.
			if((iter->TransportLanded) && (iter->UnitsCount != iter->UnitsAmount)) //&& ((iter->TransportDownCounter <= iter->TransportDownTime)
			{
				iter->TransportDownCounter++;

				// Get the first empty group to use.
				if(iter->UnitsGroup == -1)
					iter->UnitsGroup = GetFirstEmptyGroup(Team);

				// If the group is full, or there's no more free groups past Group 6, don't spawn any ships.
				if((GetGroupCount(Team, iter->UnitsGroup) == 10) || (iter->UnitsGroup > 5))
					iter->UnitsCount = iter->UnitsAmount;

				// If we haven't spawned the number of ships desired yet, spawn them one at a time.
				if((iter->UnitsCount < iter->UnitsAmount) && (((BZ1Helper::m_TurnCounter+(5*BZ1Helper::m_GameTPS)) % BZ1Helper::m_GameTPS) == 0)) // Once every second, after 5 seconds of being landed.
				{
					if(Logging)
						FormatLogMessage("Units Amount: %d, Counter Amount: %d", iter->UnitsAmount, iter->UnitsCount);

					char NewShipODFArray[16][MAX_ODF_LENGTH] = {0};
					int NumberOfEntries = 0; 
					for (int i = 0; i < 16; i++) // Look through 16 aentries.
					{
						char DesiredValue[MAX_ODF_LENGTH] = {0};
						sprintf_s(DesiredValue, "TransportVehicle%d", i+1);
						if((GetODFString(iter->TransportObject, "TransportClass", DesiredValue, MAX_ODF_LENGTH, NewShipODFArray[i])) && (NewShipODFArray[i][0]))
							++NumberOfEntries;
						else
							break;

						GetWorldVariantODF(NewShipODFArray[i]);
					}

					Handle TempShipH = 0;

					int ShipSelection  = GetRandomInt(NumberOfEntries-1);
					clamp(ShipSelection, 0, NumberOfEntries);

					Vector Reinforcement = GetPosition(iter->TransportObject);
					Vector Reinforcementspawn = GetPositionNear(Reinforcement, 40.0f, 50.0f);
					// Try to find a more suitable position if this one looks like it might be on a cliff.
					for(int Tries = 0; Tries < 10; Tries++)
					{
						float X = FLT_MAX;
						if(Reinforcement.y > Reinforcementspawn.y)
							X = Reinforcement.y - Reinforcementspawn.y;
						else
							X = Reinforcementspawn.y - Reinforcement.y;
						if(X < 25.0f)
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

					if((iter->UnitsCount % (2 * BZ1Helper::m_GameTPS)) == 0)
					{
						// Build some Ammo/Health powerups too.
						char TempShipODF[MAX_ODF_LENGTH] = {0};
						float fRandomNum = GetRandomFloat(1.0f);
						if(fRandomNum < 0.5f)
							strcpy_s(TempShipODF, "apBZCrepa");
						else if(fRandomNum < 1.0f)
							strcpy_s(TempShipODF, "apBZCammo");
						Reinforcementspawn = GetPositionNear(Reinforcement, 30, 40);
						TempShipH = BuildObject(TempShipODF, Team, Reinforcementspawn);
					}
				}
			}

			// If the timer is == to Interval + Land Time + Down Time, it's time to takeoff. Also wait until all units have disembarked, if there are any.
			if ((iter->TransportLanded) && (iter->TransportDownTime > 0) && //(iter->UnitsCount == iter->UnitsAmount) && 
				(iter->TransportTimer == (iter->TransportInterval + iter->TransportLandTime + iter->TransportDownTime + iter->TransportDownCounter)))
			{
				if(Logging)
					FormatLogMessage("Bananas! We have Liftoff! Timer is: %f", iter->TransportTimer); 

				Deploy(iter->TransportObject); // The proper way to order an AI unit to deploy. -GBD
				//SetObjectiveOff(iter->TransportObject); // Light 'em up!
			}

			// If the timer is == Interval + DownTime + 2 * LandTime, then it's reached upper atmosphere. Time to remove it and Reset things.
			if ((!IsDeployed(iter->TransportObject)) && (iter->TransportDownTime > 0) && (iter->TransportLanded) && (iter->TransportTimer == (iter->TransportInterval + (iter->TransportDownTime + iter->TransportDownCounter + (2*iter->TransportLandTime)))))
			{
				if(Logging)
					FormatLogMessage("Transport is Away! Timer is: %d", iter->TransportTimer); 

				RemoveObject(iter->TransportObject); // This is all under an isAround transport object. :)
				
				iter->TransportTimer = iter->TransportDownTime + (2*iter->TransportLandTime);

				iter->TransportInProgress = false;
				iter->TransportLanded = false;
				iter->TransportLandTime = 0;
				iter->TransportDownCounter = 0;
				iter->UnitsCount = 0;

				if(Logging)
					FormatLogMessage("One Minute Reset! Timer is: %d, Resetting", iter->TransportTimer); 
			}
		}
		else
		{
			iter->TransportInProgress = false;
			iter->TransportLanded = false;
			iter->TransportLandTime = 0;
		}
		*/
	} // End Loop over all LPads
}
