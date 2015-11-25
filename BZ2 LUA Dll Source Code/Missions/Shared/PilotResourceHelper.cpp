#include "BZ1Helper.h"
#include "PilotResourceHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

PilotResourceHelper *PilotResourceHelper::m_PilotResourceHelper;

PilotResourceHelper::PilotResourceHelper()
{
	m_PilotResourceHelper = this;
}

PilotResourceHelper::~PilotResourceHelper()
{
	 m_PilotResourceHelper = NULL;
}

bool PilotResourceHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		PilotedShipList.resize(size);
		if(size)
			Read(&PilotedShipList.front(), sizeof(PilotedShipClass)*size);

		size;
		Read(&size, 1);
		PilotList.resize(size);
		if(size)
			Read(&PilotList.front(), sizeof(PilotClass)*size);
	}
	return true;
}

bool PilotResourceHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<PilotedShipClass>::iterator iter = PilotedShipList.begin(); iter != PilotedShipList.end(); ++iter)
		{
			PilotedShipClass &sao = *iter;
			ConvertHandles(&sao.PilotedShipObject, 1);
		}

		for (std::vector<PilotClass>::iterator iter = PilotList.begin(); iter != PilotList.end(); ++iter)
		{
			PilotClass &sao = *iter;
			ConvertHandles(&sao.PilotObject, 1);
			ConvertHandles(&sao.PilotDestination, 1);
		}
	}
	return true;
}

bool PilotResourceHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(PilotedShipList.size());
		Write(&size, 1);
		if(size)
			Write(&PilotedShipList.front(), sizeof(PilotedShipClass)*size);

		size = int(PilotList.size());
		Write(&size, 1);
		if(size)
			Write(&PilotList.front(), sizeof(PilotClass)*size);
	}
	return true;
}

void PilotResourceHelper::AddPilotObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int BZCClassHash, const char *ODFName)
{
	bool InEditor = false;
	
#ifdef EDITOR
	InEditor = GetVarItemInt("shell.dll.editor.active");
#endif

	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	// For adding/subtracting to curr/max pilots.
	int TempPilotHold = 0, TempPilotCost = 0;

	//char TempAIProcess[MAX_ODF_LENGTH] = {0};
	//GetODFString(h, "GameObjectClass", "aiName", MAX_ODF_LENGTH, TempAIProcess);

	// Look for new ODF Parameter pilotHold. 
	if((GetODFInt(TempODF1, TempODF2, "GameObjectClass", "pilotHold", &TempPilotHold)) || 
		(GetODFInt(TempODF1, TempODF2, "BarracksClass", "pilotHold", &TempPilotHold))) // Grab the value.
	{
		if(TempPilotHold)
		{
			teamInfo.MaxPilots += TempPilotHold;
			if(teamInfo.CurrPilots + TempPilotHold > teamInfo.MaxPilots)
				teamInfo.CurrPilots = teamInfo.MaxPilots;
			else
				teamInfo.CurrPilots += TempPilotHold;
		}
	}

	// Look for new ODF Parameter pilotCost. -not in EDITOR.
	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "pilotCost", &TempPilotCost);
	
	if(TempPilotCost)
	{
		bool AddToList = true;
		for (std::vector<PilotedShipClass>::iterator iter = PilotedShipList.begin(); iter != PilotedShipList.end(); ++iter)
		{
			if(iter->PilotedShipObject == h)
			{
				AddToList = false;
				break;
			}
		}
		if(AddToList)
		{
			PilotedShipList.resize(PilotedShipList.size()+1);
			PilotedShipClass &sao = PilotedShipList[PilotedShipList.size()-1]; // No temporary being created, *much* faster
			memset(&sao, 0, sizeof(PilotedShipClass)); // Zero things out at start.

			sao.PilotedShipObject = h; // Add the pilot to the list.

			// Check to see if this is a spawned object before we care about it. But not in Editor.
			if((!InEditor) && (!IsPlayer(h)) && (IsAliveAndPilot2(h)) && (BZ1Helper::m_TurnCounter > 1))
			{
				if(teamInfo.CurrPilots < TempPilotCost)
				{
					 // Easter Egg Bug Recreation of sorts. :)
					if(GetPlayerHandle(Team))
						RemovePilotAI(h);
				}
				else
				{
					teamInfo.CurrPilots -= TempPilotCost;
				}
			}
		}
	}

	if((BZCClassHash == BZCCLASS_PERSON_PILOT) || (BZCClassHash == BZCCLASS_PERSON_SOLDIER)) //(_stricmp(TempAIProcess, "SoldierProcess") == 0))
	{
		bool AddToList = true;
		for (std::vector<PilotClass>::iterator iter = PilotList.begin(); iter != PilotList.end(); ++iter)
		{
			if(iter->PilotObject == h)
			{
				AddToList = false;
				break;
			}
		}
		if(AddToList)
		{
			PilotList.resize(PilotList.size()+1);
			PilotClass &sao = PilotList[PilotList.size()-1]; // No temporary being created, *much* faster
			memset(&sao, 0, sizeof(PilotClass)); // Zero things out at start.

			//if(_stricmp(TempAIProcess, "SoldierProcess") == 0)
			if(BZCClassHash == BZCCLASS_PERSON_SOLDIER)
				sao.IsSoldier = true;
			else
				SetIndependence(h, 0); // Don't let the AI do stuff on it's own.

			GetODFFloat(TempODF1, TempODF2, "CraftClass", "VehicleSearchDist", &sao.VehicleSearchDist, ScaleDistance(100.0f));
			GetODFFloat(TempODF1, TempODF2, "CraftClass", "PersonRetreatRecycleDist", &sao.RecycleDist, 25.0f);
			//sao.RecycleDist += 1.0f; // Add 1 for padding to trigger before game engine does.

			// Calculate EngageRange. Whooboy.
			sao.EngageRange = GetEngageRange(h);
			//SetIndependence(h, 0); // Don't let the AI do stuff on it's own.
			sao.IsIndependent = true;
			sao.Timeout = 1; // Wait 1 tick. //  * BZ1Helper::m_GameTPS; // 1 second wait.
			sao.PilotObject = h; // Add the pilot to the list.
		}
	}

	// Ensure there's only one pilot limiter per team. 
	if((ODFName) && (_strnicmp(ODFName,"BZClimitpilots",14) == 0))
	{
		if((IsAround(teamInfo.ImaPilot)) && (teamInfo.ImaPilot != h))
		{
			RemoveObject(teamInfo.ImaPilot);
			teamInfo.ImaPilot = h;
		}
	}

}

void PilotResourceHelper::DeletePilotObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int BZCClassHash, const int ObjClassHash)
{
	int TempPilotHold = 0, TempPilotCost = 0;

	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	//	if(m_UsePilots[Team])
//	{
	// Look for new ODF Parameter pilotHold. 
	if((GetODFInt(TempODF1, TempODF2, "GameObjectClass", "pilotHold", &TempPilotHold)) || 
		(GetODFInt(TempODF1, TempODF2, "BarracksClass", "pilotHold", &TempPilotHold))) // Grab the value.
	{
		if(TempPilotHold)
			teamInfo.MaxPilots -= TempPilotHold;
		//! Disable next 4 lines to enable BZ1 barracks exploit. 
	//	if((CurrPilots[Team] - TempPilotHold) < 0)
	//		CurrPilots[Team] = 0;
	//	else
	//		CurrPilots[Team] = CurrPilots[Team]-TempPilotHold;

	//	if(Logging)
	//		FormatLogMessage("MaxPilots for Team %d Are: CurrPilots: %d, MaxPilots: %d", Team, CurrPilots[Team], MaxPilots[Team]); 
	}

	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "pilotCost", &TempPilotCost);

	// Keep track of Ships.
	if(TempPilotCost)
	{
		for (std::vector<PilotedShipClass>::iterator iter = PilotedShipList.begin(); iter != PilotedShipList.end(); ++iter)
		{
			if(iter->PilotedShipObject == h) // If this item in the list is our dead guy, look him up. 
			{
				if( (iter->ShipDistance < 50.0f) && // Distance is < 50.0f.
					((iter->ShipCommand == CMD_RECYCLE) || ((ObjClassHash == CLASS_TUG) && (iter->ShipCommand == CMD_NONE))) && // Cmd Recycle, NOTE: BZ2 Tugs use Cmd_Go for Recycling, so we have to treat them special.
					(!IsPlayer(iter->PilotedShipObject)) && // It's not a Player.
					(teamInfo.CurrPilots < teamInfo.MaxPilots)) // Pilots are < Max.
					teamInfo.CurrPilots += TempPilotCost;

				break;
			}
		} // i loop over all objects
	}

	// Keep track of Pilots.
	if(BZCClassHash == BZCCLASS_PERSON_PILOT)
	{
		for (std::vector<PilotClass>::iterator iter = PilotList.begin(); iter != PilotList.end(); ++iter)
		{
			if(iter->PilotObject == h) // If this item in the list is our dead guy, look him up. 
			{
				// IsWithin(h, RecyclePoint[Team], 30) // Doesn't work :(
				if( (iter->IsRecycling) && // This Pilot is flagged as Recycling.
					(iter->PilotDistance < iter->RecycleDist) && //|| ((GetNearestBuilding(h) == iter->PilotDestination) && (iter->PilotDistance < iter->RecycleDist))) &&  //teamInfo.RecyclePoint) && (iter->PilotDistance < 70))) && 
					(teamInfo.CurrPilots < teamInfo.MaxPilots)) // Our current Pilots are less then our Max Pilots.
					teamInfo.CurrPilots += 1;

				break;
			}
		} // i loop over all objects
	}
//	}
}

void PilotResourceHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		// Pilot Cheat from AIP file.
		if((teamInfo.m_CPUPilotCheatDelay > 0) && (BZ1Helper::m_TurnCounter % ConvertToTurns(teamInfo.m_CPUPilotCheatDelay) == 0)) // It's time to add money.
			BZ1Helper::Get().AddPilot(Team, teamInfo.m_CPUPilotCheatAmount);

		// If we're using pilots...
		if(teamInfo.m_UsePilots)
		{
			// Floor these values to keep them from going out of range. 
			if(teamInfo.MaxPilots < 0)
				teamInfo.MaxPilots = 0;

			teamInfo.CurrPilots = clamp(teamInfo.CurrPilots, 0, teamInfo.MaxPilots);

			if((!IsAround(teamInfo.ImaPilot)) && (teamInfo.CurrPilots > 0))
			{
				teamInfo.ImaPilot = BuildObject("BZClimitpilots",Team,BZ1Helper::Where);

				if(Logging)
					FormatLogMessage("Placing ImaPilot for Team %d", Team);
			}

			if((teamInfo.MaxPilots > 0) && (!teamInfo.m_HadPilots))
				teamInfo.m_HadPilots = true;

			if((teamInfo.CurrPilots <= 0) && (teamInfo.m_HadPilots) && (IsAround(teamInfo.ImaPilot)))
			{
				// Your pilots are depleted. 
				if(Team == GetLocalPlayerTeamNumber())
				{
					char SoundFile[MAX_ODF_LENGTH] = {0};
					char EventODF[MAX_ODF_LENGTH] = {0};
					sprintf_s(EventODF, "%cevent.odf", teamInfo.TeamRace);

					if(!DoesODFExist(EventODF))
						strcpy_s(EventODF, "aevent.odf");

					if(OpenODF2(EventODF))
					{
						if(GetODFString(EventODF, "BettyVoice", "EVENT_SOUND_12", MAX_ODF_LENGTH, SoundFile)) // Grab the sound file, with allowance for a large filename.
						{
							if(SoundFile[0])
							{
								if(DoesFileExist(SoundFile))
									AudioMessage(SoundFile);
								else
									FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix Event_Sound_12 in ODF: %s", SoundFile, EventODF); 
							}
						}
					}
				}

				RemoveObject(teamInfo.ImaPilot);

				if(Logging)
					FormatLogMessage("Removed Pilot Limiter, Team: %d", Team); 

				// Stop all builds in progress.
				for(int i = 0; i < 14; i++)
				{
					Handle TempH = GetObjectByTeamSlot(Team, i+1);
					char ObjClass[MAX_ODF_LENGTH] = {0};
					GetObjInfo(TempH, Get_GOClass, ObjClass);
					//unsigned int ObjClassHash = Hash(ObjClass);
					//if((ObjClassHash == CLASS_RECYCLER) || (ObjClassHash == CLASS_FACTORY))
					if((_stricmp(ObjClass,"CLASS_RECYCLER") == 0) || (_stricmp(ObjClass,"CLASS_FACTORY") == 0))
						SetCommand(TempH, CMD_CANCEL, 0);
				}
			}

			if((teamInfo.MaxPilots == 0) && (teamInfo.m_HadPilots))
				teamInfo.m_HadPilots = false;
		}
		else // Not using Pilots, remove limiter if present.
		{
			if(IsAround(teamInfo.ImaPilot))
				RemoveObject(teamInfo.ImaPilot);
		}
	}

	PilotedShipList.erase(std::remove_if(PilotedShipList.begin(), PilotedShipList.end(), ShouldRemovePilotedShip), PilotedShipList.end());
	PilotList.erase(std::remove_if(PilotList.begin(), PilotList.end(), ShouldRemovePilot), PilotList.end());

	// Loop over all Pilots, update distance to their destination.
	for (std::vector<PilotClass>::iterator iter = PilotList.begin(); iter != PilotList.end(); ++iter)
	{
		// If it's idle timer is done. This is so it delays a tick to allow DLLs to give the pilot Orders / Set its independance before this code kicks in.
		if(iter->Timeout <= 0)
		{
			int Team = GetTeamNum(iter->PilotObject);
			iter->PilotDistance = GetDistance(iter->PilotObject, iter->PilotDestination); //teamInfo.RecyclePoint);

		//	if(Logging)
		//		FormatLogMessage("Pilot distance is %f", iter->PilotDistance); 

			// Manual Recycle Code. Sends pilot to nearest Barracks or Recycler. NO SILOS!
			if((!IsPlayer(iter->PilotObject)) && (iter->IsIndependent))
			{
				// If this is not a soldier, look for empty ships to get into.
				if(!iter->IsSoldier)
				{
					// Look for an empty ship first.
					if(!iter->IsGoingToShip)
					{
						Handle TempDestination = 0;
						float BestDistance = FLT_MAX;
						Handle TeamPlayer = GetPlayerHandle(Team);
						Handle Hoppy = HoppedOutOf(TeamPlayer);
						Handle TempShip = HoppedOutOf(iter->PilotObject);
						bool PlayerIsPerson = IsPerson(TeamPlayer);

				//		if(Logging)
				//			FormatLogMessage("Pilot hopped out of: %08x", TempShip);

						// Assume they are switching ships. New since BZ1, BZ1 would just omit the ship they hopped out of. (TempShip)
						if( (PlayerIsPerson) && // Player is out of thier ship.
							(Hoppy) && // The player used to be in a ship.
							(!IsAliveAndPilot2(Hoppy)) && // The Player's last ship they were in is Empty. (Or not around?)
							(GetDistance(Hoppy, TeamPlayer) < iter->VehicleSearchDist) && // The Distance between the Player and his last ship is < SearchRange.
							(GetDistance(iter->PilotObject, TempShip) < iter->VehicleSearchDist)) // The Distance between us and our last ship is < SearchRange.
						{
							TempDestination = Hoppy;
						}
						else // Look for a random empty ship within range.
						{
							// Loop over all possible ships, find the closest empty one.
							for(int j = 0; j < BZ1Helper::Get().GetNumHandles(); j++)
							{
								BZ1Helper::ObjectClass &objectInfo = BZ1Helper::Get().RefHandleInfo(j);
								Handle TempHandle = objectInfo.ObjectHandle;

								if( (objectInfo.CategoryID == Vehicle) && // It's a Vehicle.
									// (TempHandle != TempShip) && // BZ1 did this... It's not the ship we just hopped out of.
									(!IsNotDeadAndPilot2(TempHandle)) && // It's empty.
									(!IsPlayer(TempHandle)) // Not the Player.
									) // Loop through and compare distances. If it finds one closer, save it.
								{
									float TempDist = GetDistance(iter->PilotObject, TempHandle);
									float PlayerDist = GetDistance(TeamPlayer, TempHandle);

									if( (TempDist < iter->VehicleSearchDist) && // It's within our pilot's search range.
										((!PlayerIsPerson) || (PlayerDist > iter->VehicleSearchDist)) && // The Player is not within search range of the ship, or not a pilot.
										(BestDistance > TempDist)) // This one is closer then our last selection.
									{
										TempDestination = TempHandle;
										BestDistance = TempDist; // Save new distance. 
									}
								}
							}
						}

						if(TempDestination)
						{
							iter->PilotDestination = TempDestination;
							//GetIn(iter->PilotObject, iter->PilotDestination, 0); // Well Poo.
							Goto(iter->PilotObject, iter->PilotDestination, 0);
							//SetIndependence(iter->PilotObject, 0);
							iter->IsGoingToShip = true;
							iter->IsRecycling = false;

							if(Logging)
							{
								if(Hoppy == iter->PilotDestination)
									FormatLogMessage("Pilot Sent to PLAYER's Ship: %d ODF: %s, Distance: %f", iter->PilotDestination, GetODFName(iter->PilotDestination), GetDistance(iter->PilotObject, iter->PilotDestination));
								else
									FormatLogMessage("Pilot Sent to Empty Ship: %d ODF: %s, Distance: %f", iter->PilotDestination,  GetODFName(iter->PilotDestination), GetDistance(iter->PilotObject, iter->PilotDestination));
							}
						}
					}
					else
					{
						// If the destination went boom, got a pilot, or the player jumped in, reset.
						if((!IsAround(iter->PilotDestination)) || (IsNotDeadAndPilot2(iter->PilotDestination)) || (IsPlayer(iter->PilotDestination)))
						{
							iter->IsGoingToShip = false; // Damn, it's gone or someone else beat us to it, resume normal behavior.

							if(Logging)
								FormatLogMessage("Pilot Destination Empty Ship: %d ODF: %s, Not Around or Piloted! Aborting!", iter->PilotDestination, GetODFName(iter->PilotDestination));
						}
					}
				}
				else // Soldier behavior, If anything is in range, shoot it, else retreat to recycler.
				{
					// If we're not currently Attacking something.
					if(GetCurrentCommand(iter->PilotObject) != CMD_ATTACK)
					{
						Handle Target = 0;
						float Distance = FLT_MAX;

						// Loop over all possible Targets, attack the closest one.
						for(int j = 0; j < BZ1Helper::Get().GetNumHandles(); j++)
						{
							BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(j);
							Handle TempHandle = objectInfo.ObjectHandle;
							int TempTeam = objectInfo.CurTeamNum;

							if( (!IsTeamAllied(Team, TempTeam)) && // It's an Enemy!
								(GetMaxHealth(TempHandle) > 0) )// It can be killed.
							//	(Target != TempHandle) && // Isn't our current Target?
							{
								float NewDistance = GetDistance(iter->PilotObject, TempHandle);

								if( (NewDistance < iter->EngageRange) && // This one is within EngageRange.
									(NewDistance < Distance) && // This one is Closer.
									((objectInfo.CategoryID == Vehicle) || // It's a Vehicle.
									(objectInfo.CategoryID == Person) || // Its a Pilot.
									(IsGunTower(TempHandle)) || // It's a Gun Tower!
									(IsProducer(TempHandle))) ) // It's a Production Vehicle! KILL IT!
								{
									Target = TempHandle;
									Distance = NewDistance;
								}
							}
						}

						if(Target) // The above code found a valid target.
						{
							Attack(iter->PilotObject, Target, 0);
							iter->IsGoingToShip = true; // NOT recycling anymore.
							iter->IsRecycling = false;

							if(Logging)
								FormatLogMessage("Soldier Ordered to Attack: %d!, ODF: %s", Target, GetODFName(Target));
						}
						else
						{
							iter->IsGoingToShip = false;
						}
					}
					else // It's currently attacking something.
					{
						Handle Target = GetCurrentWho(iter->PilotObject); //GetTarget(iter->PilotObject);

						if(Target)
						{
							float NewDistance = GetDistance(iter->PilotObject, Target);

							// Let go of target if it gets too far.
							if(NewDistance > (1.25f * iter->EngageRange))
							{
								if(iter->IsGoingToShip)
								{
									iter->IsGoingToShip = false;
									Stop(iter->PilotObject, 0);

									if(Logging)
										FormatLogMessage("Soldier Target Out of Range! Stopping attack on: %d!, ODF: %s", Target, GetODFName(Target));
								}
							}
							else
							{
								iter->IsGoingToShip = true;
							}
						}
					}
				} // It's a soldier.

				// No empty ships nearby, head to nearest Barracks or Recycler/Muf/Slf.
				if((!iter->IsRecycling) && (!iter->IsGoingToShip))
				{
					float DistanceSquared = FLT_MAX;
					iter->PilotDestination = 0;

					// BZ1 does this...
					if(IsAround(BZ1Helper::Get().GetRecy(Team)))
						iter->PilotDestination = BZ1Helper::Get().GetRecy(Team);
					else if(IsAround(BZ1Helper::Get().GetMuf(Team)))
						iter->PilotDestination = BZ1Helper::Get().GetMuf(Team);
					else if(IsAround(BZ1Helper::Get().GetSlf(Team)))
						iter->PilotDestination = BZ1Helper::Get().GetSlf(Team);

					// No porduction units on our team, look for allies.
					if(!IsAround(iter->PilotDestination))
					{
						for(int t = 1; t < MAX_TEAMS; t++)
						{
							if((t != Team) && (IsTeamAllied(Team, t)))
							{
								if(IsAround(BZ1Helper::Get().GetRecy(t)))
									iter->PilotDestination = BZ1Helper::Get().GetRecy(t);
								else if(IsAround(BZ1Helper::Get().GetMuf(t)))
									iter->PilotDestination = BZ1Helper::Get().GetMuf(t);
								else if(IsAround(BZ1Helper::Get().GetSlf(t)))
									iter->PilotDestination = BZ1Helper::Get().GetSlf(t);

								if(iter->PilotDestination)
									break; // We found one.
							}
						}
					}

					if(iter->PilotDestination) // If the above found something.
						DistanceSquared = GetDistance3DSquared(iter->PilotObject, iter->PilotDestination);

					// Loop over all possible barracks, find the closest one.
					for(int j = 0; j < BuildingHelper::Get().GetNumBuildingObjects(); j++)
					{
						BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(j);
						Handle TempHandle = buildingInfo.BuildingObject;
						float TempDistSquared = GetDistance3DSquared(GetPosition(iter->PilotObject), buildingInfo.Position);
						int t = buildingInfo.CurTeamNum;

						if( (t != 0) && // Skip Team 0.
							((t == Team) || (IsTeamAllied(t, Team))) && // It's Friendly.
							(IsBarracks(TempHandle)) && // It's a Barracks.
							(TempDistSquared < DistanceSquared)) // This one is closer.
						{
							iter->PilotDestination = TempHandle;
							DistanceSquared = TempDistSquared; // Save new distance. 

							if(Logging)
								FormatLogMessage("Pilot on Team: %d Index: %d Chose Recycle point on Team: %d Index: %d Distance: %d", Team, iter - PilotList.begin(), t, j, sqrtf(DistanceSquared)); 
						}
					}

					if(iter->PilotDestination) // If the above found something.
					{
						if(Logging)
							FormatLogMessage("Pilot on Team: %d Index: %d Was set to Recycle at a point on Team: %d.", Team, iter - PilotList.begin(), GetTeamNum(iter->PilotDestination)); 

						Retreat(iter->PilotObject, iter->PilotDestination, 0);
						//SetIndependence(iter->PilotObject, 0);
						iter->IsRecycling = true;
					}
					else if(!iter->IsStuck)
					{
						iter->IsStuck = true;
						Defend(iter->PilotObject, 0);

						//SetIndependence(iter->PilotObject, 1);

						if(Logging)
							FormatLogMessage("Pilot has no where to go, Defending area!");
					}
				}

				if(iter->IsRecycling)
				{
					if(!IsAround(iter->PilotDestination))
					{
						iter->IsRecycling = false;

						if(Logging)
							FormatLogMessage("Pilot Recycle location %d destroyed, choosing new one.", iter->PilotDestination);
					}
					else if(GetDistance(iter->PilotObject, iter->PilotDestination) < iter->RecycleDist)
					{
						RemoveObject(iter->PilotObject); // Delete object should handle the rest?
					}
				}
			}
		}
		else
		{
			--iter->Timeout;
		}
	}

	// Loop over all Vehicles, update distance to Recy.
	for (std::vector<PilotedShipClass>::iterator iter = PilotedShipList.begin(); iter != PilotedShipList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->PilotedShipObject);
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		iter->ShipDistance = GetDistance(iter->PilotedShipObject, teamInfo.RecyclePoint);
		iter->ShipCommand = GetCurrentCommand(iter->PilotedShipObject);

	//	if(Logging)
	//		FormatLogMessage("Ship distance is %f, ShipCommand Index: %d is %d", iter->ShipDistance, iter - PilotedShipList.begin(), iter->ShipCommand); 
	}
}

// This is a DLL script function that sets if the pilot is DLL Controlled or not.
void PilotResourceHelper::SetPilotIndependence(const Handle h, const bool Independence)
{
	for (std::vector<PilotClass>::iterator iter = PilotList.begin(); iter != PilotList.end(); ++iter)
	{
		if(iter->PilotObject == h)
		{
			iter->IsIndependent = Independence;
			SetIndependence(h, !Independence); // Flip control from DLL back to Game, or visa versa.
			break;
		}
	}
}