#include "BZ1Helper.h"
#include "PortalHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <math.h>
#include <algorithm>

PortalHelper *PortalHelper::m_PortalHelper;

PortalHelper::PortalHelper()
{
	m_PortalHelper = this;
}
PortalHelper::~PortalHelper()
{
	 m_PortalHelper = NULL;
}

bool PortalHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		PortalList.resize(size);
		if(size)
			Read(&PortalList.front(), sizeof(PortalClass)*size);

		Read(&size, 1);
		TeleportTargetList.resize(size);
		if(size)
			Read(&TeleportTargetList.front(), sizeof(TeleportTargetClass)*size);
	}
	return true;
}

bool PortalHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
		{
			PortalClass &sao = *iter;
			ConvertHandles(&sao.TeleporterObject, 1);
			ConvertHandles(&sao.TeleporterDestination, 1);
		//	ConvertHandles(&sao.TeleporterTap, 1);
		}

		for (std::vector<TeleportTargetClass>::iterator iter = TeleportTargetList.begin(); iter != TeleportTargetList.end(); ++iter)
		{
			TeleportTargetClass &sao = *iter;
			ConvertHandles(&sao.TeleportTarget, 1);
			ConvertHandles(&sao.TeleportTargetDestination, 1);
		}
	}
	return true;
}

bool PortalHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(PortalList.size());
		Write(&size, 1);
		if(size)
			Write(&PortalList.front(), sizeof(PortalClass)*size);

		size = int(TeleportTargetList.size());
		Write(&size, 1);
		if(size)
			Write(&TeleportTargetList.front(), sizeof(TeleportTargetClass)*size);
	}
	return true;
}

void PortalHelper::AddPortalSwitchObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	//SetLifespan(h, 0.1f);
	int TempChannel = -1;
	// Look for new ODF Parameters. 
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "PortalChannel", &TempChannel, 0);
	if(TempChannel >= 0)
	{
		float BestDistSquared = FLT_MAX;
		int Choice = -1;

		for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
		{
			// Loop through and compare distances. If it finds one closer, save it.
			float DistSquared = GetDistance3D(h, iter->TeleporterObject);
			if(BestDistSquared > DistSquared)
			{
				Choice = iter - PortalList.begin();
				BestDistSquared = DistSquared; // Compare new distance. 
			}
		}
		if(Choice >= 0)
			PortalList[Choice].Channel = TempChannel;
	}
	BZ1Helper::Get().DeleteAfterDelay(h);
}

void PortalHelper::AddPortalObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
			return;
	}

	PortalList.resize(PortalList.size()+1);
	PortalClass &sao = PortalList[PortalList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(PortalClass)); // Zero things out at start.

	GetODFString(TempODF1, TempODF2, "TeleportalClass", "PortalEffect", MAX_ODF_LENGTH, sao.EffectBaseODF);
	//GetODFString(TempODF1, TempODF2, "TeleportalClass", "PortalEffect2", MAX_ODF_LENGTH, sao.EffectBaseODF[1]);
	//GetODFString(TempODF1, TempODF2, "TeleportalClass", "PortalEffect3", MAX_ODF_LENGTH, sao.EffectBaseODF[2]);
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "InEffect", &sao.InEffect, 0);
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "OutEffect", &sao.OutEffect, 0);
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "OnEffect", &sao.OnEffect, 0);

	if((GetODFString(TempODF1, TempODF2, "GameObjectClass", "unitName2", MAX_ODF_LENGTH, sao.TeleporterName)) || 
		(GetODFString(TempODF1, TempODF2, "GameObjectClass", "unitName", MAX_ODF_LENGTH, sao.TeleporterName)))
	{
		// Got it.
	}
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "TeleporterChannel", &sao.Channel, 0);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "ShowChannel", &sao.ShowChannel, true);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "CanEnter", &sao.CanEnter, true);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "CanExit", &sao.CanExit, true);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "RandomDestination", &sao.IsRandom, false);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "RandomUsesChannels", &sao.RandomUsesChannels, true);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "ContinuousFx", &sao.ContinuousFx, false);
	GetODFFloat(TempODF1, TempODF2, "TeleportalClass", "ContinuousFxRange", &sao.ContinuousFxRange, 0.0f);
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "TeleportFilter", &sao.TeleportFilter, 15);
	// Make 0 All Types.
	if(sao.TeleportFilter == 0)
		sao.TeleportFilter = 31;

	GetODFFloat(TempODF1, TempODF2, "TeleportalClass", "ExitVelocity", &sao.ExitSpeed, 0.0f);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "ExitForwardOnly", &sao.ExitForwardOnly, false);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "ExitKickAI", &sao.KickAI, false);
	GetODFBool(TempODF1, TempODF2, "TeleportalClass", "ExitUsesCenter", &sao.UseCenter, true);
	GetODFFloat(TempODF1, TempODF2, "TeleportalClass", "PortalTargetRange", &sao.EnterRange, 10.0f);
	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "TeleportTeamFilter", &sao.TargetTeamFilter, 7);
	// Make 0 all teams. 
	if(sao.TargetTeamFilter == 0)
		sao.TargetTeamFilter = 7;

	GetODFInt(TempODF1, TempODF2, "TeleportalClass", "DestinationTeamFilter", &sao.DestTeamFilter, 3);
	// Make 0 all teams.
	if(sao.DestTeamFilter == 0)
		sao.DestTeamFilter = 7;

	sao.ExitPoint = GetODFPositionOffset(h, 0);
	sao.GotoPoint = GetODFPositionOffset(h, 1);
	sao.Position = GetPosition(h);

//	if(Logging)
//		FormatLogMessage("Added an Teleporter to the list, Team: %d, Index: %d", Team, m_NumTeleporterObjects[Team]); 

	//!! Get Label set function for Map Makers. Only Valid for 1.3 PB6.4+.
//	/*
	const char * Label = GetLabel(h);
	if((Label) && (_strnicmp(Label,"channel_",8) == 0))
	{
		char ChannelNum[MAX_ODF_LENGTH] = {0};
		const char *pUnderscore = strchr(Label, '_');
		strcpy_s(ChannelNum, pUnderscore+1);
		sao.Channel = atoi(ChannelNum);
	}
	// */

	sao.TeleporterObject = h;
}

void PortalHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging; //false;

	// pre-clean the vector
	//PortalList.erase(std::remove_if(PortalList.begin(), PortalList.end(), ShouldRemovePortal), PortalList.end());
	// Sweep through the list and remove any of them that are no longer around
	std::vector<PortalClass>::iterator validEnd = PortalList.begin();
	std::vector<PortalClass>::iterator lastValid = PortalList.begin();
	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		// If the host or weapon is around...
		if (IsAround(iter->TeleporterObject))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			if(IsAround(iter->TeleporterEffect)) // Cleanup effect objects if their host is no longer around.
				RemoveObject(iter->TeleporterEffect);
		}
	}
	// Now erase any invalid entries left behind
	PortalList.erase(validEnd, PortalList.end());

	// pre-clean the vector
	TeleportTargetList.erase(std::remove_if(TeleportTargetList.begin(), TeleportTargetList.end(), ShouldRemoveTeleportTarget), TeleportTargetList.end());
	
	// Loop over all teleporters, and grab nearby targets. 
	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->TeleporterObject);
		iter->Position = GetPosition(iter->TeleporterObject);
		iter->TeleporterDestination = 0; // Zero this out to remove old, possibly obsolete destinations.

		// Set it's Name for allies only.
		if(iter->ShowChannel)
		{
			// Make the name switch back and forth every 3 seconds.
			if(((BZ1Helper::m_TurnCounter % ConvertToTurns(1.0f)) == 0) && (!iter->NameFlash))
			{
				if(IsTeamAllied(GetLocalPlayerTeamNumber(), Team))
					SetObjectiveName(iter->TeleporterObject, iter->TeleporterName);

				iter->NameFlash = true;
			}
			if(((BZ1Helper::m_TurnCounter % ConvertToTurns(3.0f)) == 0) && (iter->NameFlash))
			{
				if(IsTeamAllied(GetLocalPlayerTeamNumber(), Team))
				{
					char Name[MAX_ODF_LENGTH] = {0};
					sprintf_s(Name, "Channel %d", iter->Channel);
					SetObjectiveName(iter->TeleporterObject, Name);
				}
				iter->NameFlash = false;
			}
		}

		// If this teleporter can be entered, and IsPowered, run this code.
		if((iter->CanEnter) && (BuildingHelper::Get().IsBuildingPowered(iter->TeleporterObject)))  // Has Power.
		{
			//float TempDistSquared = FLT_MAX;
			//float TeleDistanceSquared = FLT_MAX;
	//		bool EjectForward = false;
//				bool CheckAllies = true;
	//		bool DestinationUsesCenter = true;
	//		bool DestinationKickAI = false;
			//Vector Velocity(0, 0, 0);
	//		int TempDestSpeed = 0;
	//		float TempDestRange = 10.0f;
		//	char TempDestEffect[MAX_ODF_LENGTH] = {0};
			//long TempCurAmmo = GetCurAmmo(iter->TeleporterObject);
			//long TempMaxAmmo = GetMaxAmmo(iter->TeleporterObject);
			//Vector GoToPos(0, 0, 0);
			//Handle Tugee = 0;

			if(!iter->IsRandom) // Lock on based on distance.
			{
				float TeleDistanceSquared = FLT_MAX;
				Handle TeamTeleporter = 0;
				int TeamIndex = -1;
				int DestTeam = -1;
				// Loop over all Teams. Find one that is closest, and allied.
				for (std::vector<PortalClass>::iterator iterB = PortalList.begin(); iterB != PortalList.end(); ++iterB)
				{
					int t = GetTeamNum(iterB->TeleporterObject);

					// This is a team we care about...
					if( (iter->Channel == iterB->Channel) && // The channels match.
						(iterB->CanExit) && // It can be exited.
						(TeamFilterCheck(iter->DestTeamFilter, t, Team)) && // Team Matches.
						(BuildingHelper::Get().IsBuildingPowered(iterB->TeleporterObject)) && // Target Portal has power.
						(iter->TeleporterObject != iterB->TeleporterObject)) // This teleporter is not itself.
					{
						float DistSquared = GetDistance3DSquared(iter->Position, iterB->Position); // Compare new distance. 

						if(TeleDistanceSquared > DistSquared) // This one is closer.
						{
							if(t == Team) // Only look on our team.
							{
								TeamTeleporter = iterB->TeleporterObject;
								TeamIndex = iterB - PortalList.begin();
							}

							iter->TeleporterDestination = iterB->TeleporterObject;
							iter->DestinationIndex = iterB - PortalList.begin();
							TeleDistanceSquared = DistSquared; // Compare new distance. 
							DestTeam = t;
						}
					}
				}
				// If the closest one is not on our team, try to find one that is.
				if((DestTeam != Team) && ((iter->DestTeamFilter & 1) == 1) && (TeamTeleporter)) // If teamfilter includes Same Team, look there first.
				{
					iter->TeleporterDestination = TeamTeleporter;
					iter->DestinationIndex = TeamIndex;
				}
			}
			else // Pick a Random one.
			{
				int Random = GetRandomInt(PortalList.size()-1);

				int t = GetTeamNum(PortalList[Random].TeleporterObject);

				if( //(IsAround(PortalList[Random].TeleporterObject)) && // It exists.
					((!iter->RandomUsesChannels) || (iter->Channel == PortalList[Random].Channel)) && // If Channels, channels match.
					(PortalList[Random].CanExit) &&  // Can be exited.
					(TeamFilterCheck(iter->DestTeamFilter, t, Team)) && // Teamfilter matches.
					(iter->TeleporterObject != PortalList[Random].TeleporterObject) && //It is not itself.
					(BuildingHelper::Get().IsBuildingPowered(PortalList[Random].TeleporterObject))) // Target Portal Has Power.
				{
					iter->TeleporterDestination = PortalList[Random].TeleporterObject;
					iter->DestinationIndex = Random;
				}
			}

			// If it found something.
			//if(IsAround(iter->TeleporterDestination))
			if(iter->TeleporterDestination) // This is set to 0 at start, so if the above didn't find something, it should still be 0.
			{
				if(iter->ContinuousFx)
				{
					Handle Nearest = GetNearestVehicle(iter->TeleporterObject);
					if((!IsAround(iter->TeleporterEffect)) && ((!iter->ContinuousFxRange) || // Effect is NOT around, and FxRange is 0, or FxRange is > 0 and a unit is within range.
						((Nearest) && (GetDistance3D(iter->Position, GetPosition(Nearest)) < iter->ContinuousFxRange))))
					{
						SetPortalEffectStart(iter->TeleporterObject, iter->OnEffect, PortalOn);
					}
				}
				/*
				// Grab out our destination's stuff, save it for later use. 
				for (std::vector<PortalClass>::iterator iterB = PortalList.begin(); iterB != PortalList.end(); ++iterB)
				{
					if(iterB->TeleporterObject == iter->TeleporterDestination)
					{
						TempDestSpeed = iterB->ExitSpeed;
						TempDestRange = iterB->EnterRange;
						EjectForward = iterB->ExitForwardOnly;
						DestinationUsesCenter = iterB->UseCenter;
						DestinationKickAI = iterB->KickAI;
					//	strcpy_s(TempDestEffect, iterB->EffectOut);
						iter->DestinationPosition = iterB->ExitPoint; 
						DestGoto = iterB->GotoPoint.posit;
						break;
					}
				}
				*/

#if 0
				//iter->DestinationPosition = GetODFPositionOffset(iter->TeleporterDestination, 0);
				Vector DestGoto(0, 0, 0); // = GetODFPositionOffset(iter->TeleporterDestination, 1).posit;

				// Look for valid targets
				for(int d = 0; d < BZ1Helper::Get().GetNumHandles(); d++)
				{
					BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(d);

					if(!objectInfo.CanTeleport)
						continue; // Skip this object.

					Handle TempH = objectInfo.ObjectHandle;

				//	char ObjClass[MAX_ODF_LENGTH] = {0};
				//	GetObjInfo(TempH, Get_GOClass, ObjClass);
					
					bool DontTeleport = false;
					for (std::vector<TeleportTargetClass>::iterator iterB = TeleportTargetList.begin(); iterB != TeleportTargetList.end(); ++iterB)
					{
						if(iterB->TeleportTarget == TempH)
						{
							DontTeleport = true;
							break;
						}
					}
					if(DontTeleport)
						continue; // Skip this object.

					for(int x = 0; x < MAX_TAPS;x++)
					{
						Handle CheckTap = GetTap(iter->TeleporterObject, x);
						if(TempH == CheckTap)
						{
							DontTeleport = true;
							break;
						}
					}
					if(DontTeleport)
						continue; // Skip this object.

					for (std::vector<PortalClass>::iterator iterB = PortalList.begin(); iterB != PortalList.end(); ++iterB)
					{
						if(TempH == iterB->TeleporterObject)
						{
							DontTeleport = true;
							break;
						}
					}
					if(DontTeleport)
						continue; // Skip this object.

					//FormatLogMessage("Distance on Object Team: %d, Index: %d is: %f", t, d, TempDistSquared); 

					int t = objectInfo.CurTeamNum;
					TempDistSquared = GetDistance3DSquared(objectInfo.Position, iter->Position);

					// Conditions for Teleportation.
					if( //(objectInfo.CanTeleport) && // This object can be teleported.
						(TempDistSquared < Squared(iter->EnterRange)) && // Its within Range.
						(TempCurAmmo == TempMaxAmmo) && // Its ammo is fully recharged.
						(TeamFilterCheck(iter->TargetTeamFilter, t, Team)) && // Teamfilter passes.
					//	(BuildingHelper::Get().IsBuildingPowered(iter->TeleporterDestination)) &&  // Destination Portal Has Power.
						((((iter->TeleportFilter & 1) == 1) && (objectInfo.CategoryID == Vehicle)) || //IsVehicle(TempH)) || // Teleport Ships.
						(((iter->TeleportFilter & 2) == 2) && (objectInfo.CategoryID == Person)) || //IsPerson(TempH)) || // Teleport Pilots.
						(((iter->TeleportFilter & 4) == 4) && (objectInfo.CategoryID == Powerup)) || //IsPowerup(TempH)) || // Teleport Powerups.
						(((iter->TeleportFilter & 8) == 8) && (objectInfo.CategoryID == Weapon)) || //IsWeapon(TempH)) || // Teleport Weapons.
						(((iter->TeleportFilter & 16) == 16) && (objectInfo.CategoryID == Other))) && //IsMisc(TempH)) ) && // Teleport Misc.
					//	(!DontTeleport) && // Target didn't just teleport, or isnt a tap of our teleporter.
						(GetTug(TempH) == 0) && // Target isn't being tugged.
						(!IsNullVector(PortalList[iter->DestinationIndex].ExitPoint.posit)) // It's position isn't NULL.
						)
					{

						if(Logging)
							FormatLogMessage("Locking onto Object: %08x, ODF: %s, Distance is: %f", TempH, GetODFName(TempH), sqrtf(TempDistSquared));

						// Look for this object to be tugging something, if so, save it.
						for (std::vector<TeleportTargetClass>::iterator iterB = TeleportTargetList.begin(); iterB != TeleportTargetList.end(); ++iterB)
						{
							if(GetTug(iterB->TeleportTarget) == TempH)
							{
								Tugee = iterB->TeleportTarget;
								break;
							}
						}

						// We're good to Teleport, add the Teleportee to the List.
						TeleportTargetList.resize(TeleportTargetList.size()+1);
						TeleportTargetClass &sao = TeleportTargetList[TeleportTargetList.size()-1]; // No temporary being created, *much* faster
						memset(&sao, 0, sizeof(TeleportTargetClass)); // Zero things out at start.

						sao.TeleportTarget = TempH; // Save it.
						sao.DestEnterRange = PortalList[iter->DestinationIndex].EnterRange; //TempDestRange; // Save it.
						sao.TeleportTargetDestination = iter->TeleporterDestination; // Save it.

						const Matrix &StartPortal = iter->ExitPoint; //GetODFPositionOffset(iter->TeleporterObject, 0);
						Matrix ShipSpawn = GetMatrixPosition(TempH);

						float HighestVelocity = 20;

						Vector ShipVelocity = GetVelocity(TempH);

						/*
						Handle StartTap = GetTap(iter->TeleporterObject, 0);
						if(IsAround(StartTap))
							GetPosition(StartTap, StartPortal);
						else
							GetPosition(iter->TeleporterObject, StartPortal); // Save this here.
						*/
					//	StartPortal = GetODFPositionOffset(iter->TeleporterObject, 0);

//						Matrix NewMatrix = DestinationPosition[Team][i]; // Initialize the default Matrix.

						const Matrix &EndPortal = PortalList[iter->DestinationIndex].ExitPoint; //iter->DestinationPosition;

						/*
						EndPortal.right.x = -EndPortal.right.x;
						EndPortal.right.y = -EndPortal.right.y;
						EndPortal.right.z = -EndPortal.right.z;
						EndPortal.front.x = -EndPortal.front.x;
						EndPortal.front.y = -EndPortal.front.y;
						EndPortal.front.z = -EndPortal.front.z;
						*/

						// New code from Ken Miller.
						Matrix InvStartPortal = Matrix_Inverse(StartPortal);
						Matrix Teleport = Matrix_Multiply(InvStartPortal, EndPortal);
						Matrix NewMatrix = Matrix_Multiply(ShipSpawn, Teleport);
						Velocity = Vector_Rotate(Teleport, ShipVelocity);

						/*
						NewMatrix.right.x = -NewMatrix.right.x;
						NewMatrix.right.y = -NewMatrix.right.y;
						NewMatrix.right.z = -NewMatrix.right.z;
						NewMatrix.front.x = -NewMatrix.front.x;
						NewMatrix.front.y = -NewMatrix.front.y;
						NewMatrix.front.z = -NewMatrix.front.z;
						*/
						NewMatrix.right = Neg_Vector(NewMatrix.right);
						NewMatrix.front = Neg_Vector(NewMatrix.front);
						Velocity.x = -Velocity.x;
						Velocity.z = -Velocity.z;

						// Override position floating, use root exit position.
						if(PortalList[iter->DestinationIndex].UseCenter)
							NewMatrix.posit = PortalList[iter->DestinationIndex].ExitPoint.posit; //iter->DestinationPosition.posit; 


						/*
						// Pre-stabilize any matrix values that are out of range. If acos() is run on a value that 
						// is > 1.0 or < -1.0, it will cause the rest of the code to screw up. 
						// Found that a walker sometimes had a matrix of ShipSpawn.up.y = 1.0000001, to 1.0000007. 
						// This would cause the teleportation to teleport the unit into...nothingness. This is bad. 
						// Things must always come out of the teleporter. It IS a teleporter, not a black hole. :P

						StartPortal.front.y = clamp(StartPortal.front.y, -1.000000f, 1.000000f);
						DestinationPosition[Team][i].front.y = clamp(DestinationPosition[Team][i].front.y, -1.000000f, 1.000000f);
						ShipSpawn.front.y = clamp(ShipSpawn.front.y, -1.000000f, 1.000000f);

						StartPortal.up.y = clamp(StartPortal.up.y, -1.000000f, 1.000000f);
						DestinationPosition[Team][i].up.y = clamp(DestinationPosition[Team][i].up.y, -1.000000f, 1.000000f);
						ShipSpawn.up.y = clamp(ShipSpawn.up.y, -1.000000f, 1.000000f);

						StartPortal.right.y = clamp(StartPortal.right.y, -1.000000f, 1.000000f);
						DestinationPosition[Team][i].right.y = clamp(DestinationPosition[Team][i].right.y, -1.000000f, 1.000000f);
						ShipSpawn.right.y = clamp(ShipSpawn.right.y, -1.000000f, 1.000000f);

					//---Ship Orientation Code (Built by DeathDoom)
					//---Questions?  Email deathdoom08@gmail.com
					//---Or add me on Steam: http://steamcommunity.com/id/deathdoom

					// Front Vector 
					float EntranceThetaFront = portable_acos(StartPortal.front.y);
					float ExitThetaFront = portable_acos(DestinationPosition[Team][i].front.y);
					float OldshipThetaFront = portable_acos(ShipSpawn.front.y);
					float NewshipThetaFront = OldshipThetaFront + (ExitThetaFront - EntranceThetaFront);

					float EntrancePhiFront = portable_atan2(StartPortal.front.z,StartPortal.front.x);
					float ExitPhiFront = portable_atan2(DestinationPosition[Team][i].front.z,DestinationPosition[Team][i].front.x);
					float OldshipPhiFront = portable_atan2(ShipSpawn.front.z,ShipSpawn.front.x);
					float NewshipPhiFront = 2 * portable_atan2(-1,0) +  OldshipPhiFront + (ExitPhiFront - EntrancePhiFront);

					NewMatrix.front.x = portable_sin(NewshipThetaFront) * portable_cos(NewshipPhiFront);
					NewMatrix.front.z = portable_sin(NewshipThetaFront) * portable_sin(NewshipPhiFront);
					NewMatrix.front.y = portable_cos(NewshipThetaFront);

					// Up Vector
					float EntranceThetaUp = portable_acos(StartPortal.up.y);
					float ExitThetaUp = portable_acos(DestinationPosition[Team][i].up.y);
					float OldshipThetaUp = portable_acos(ShipSpawn.up.y);
					float NewshipThetaUp = OldshipThetaUp + (ExitThetaUp - EntranceThetaUp);

					float EntrancePhiUp = portable_atan2(StartPortal.up.z,StartPortal.up.x);
					float ExitPhiUp = portable_atan2(DestinationPosition[Team][i].up.z,DestinationPosition[Team][i].up.x);
					float OldshipPhiUp = portable_atan2(ShipSpawn.up.z,ShipSpawn.up.x);
					float NewshipPhiUp = 2 * portable_atan2(-1,0) +  OldshipPhiUp + (ExitPhiUp - EntrancePhiUp);

					NewMatrix.up.x = portable_sin(NewshipThetaUp) * portable_cos(NewshipPhiUp);
					NewMatrix.up.z = portable_sin(NewshipThetaUp) * portable_sin(NewshipPhiUp);
					NewMatrix.up.y = portable_cos(NewshipThetaUp);

					// Right Vector
					float EntranceThetaRight = portable_acos(StartPortal.right.y);
					float ExitThetaRight = portable_acos(DestinationPosition[Team][i].right.y);
					float OldshipThetaRight = portable_acos(ShipSpawn.right.y);
					float NewshipThetaRight = OldshipThetaRight + (ExitThetaRight - EntranceThetaRight);

					float EntrancePhiRight = portable_atan2(StartPortal.right.z,StartPortal.right.x);
					float ExitPhiRight = portable_atan2(DestinationPosition[Team][i].right.z,DestinationPosition[Team][i].right.x);
					float OldshipPhiRight = portable_atan2(ShipSpawn.right.z,ShipSpawn.right.x);
					float NewshipPhiRight = 2 * portable_atan2(-1,0) +  OldshipPhiRight + (ExitPhiRight - EntrancePhiRight);

					NewMatrix.right.x = portable_sin(NewshipThetaRight) * portable_cos(NewshipPhiRight);
					NewMatrix.right.z = portable_sin(NewshipThetaRight) * portable_sin(NewshipPhiRight);
					NewMatrix.right.y = portable_cos(NewshipThetaRight);

						//Matrix Stuff
						NewMatrix.front = Normalize_A_Vector(NewMatrix.front);

						NewMatrix.right = Find_Cross_Product(NewMatrix.up, NewMatrix.front);
						NewMatrix.right = Normalize_A_Vector(NewMatrix.right);

						NewMatrix.up = Find_Cross_Product(NewMatrix.front, NewMatrix.right);

						// Translate Velocity.
						float VelocityMag = sqrt(pow(ShipVelocity.x,2) + pow(ShipVelocity.y,2) + pow(ShipVelocity.z,2));
						float VelocityTheta = portable_acos(ShipVelocity.y / VelocityMag);
						float VelocityPhi = portable_atan2(ShipVelocity.z,ShipVelocity.x);

						// The above calculation will return -1.#IND00 if the 2nd input is 0. So, if it is, simply set 
						// VelocityTheta to 0 instead. 
						if(VelocityMag == 0)
							VelocityTheta = 0;

						float NewvelocityTheta = VelocityTheta + (NewshipThetaFront - OldshipThetaFront); //(ExitThetaFront - EntranceThetaFront);
						float NewvelocityPhi = 2 * portable_atan2(-1,0) +  VelocityPhi + (NewshipPhiFront - OldshipPhiFront); //(ExitPhiFront - EntrancePhiFront);

						Velocity.x = -VelocityMag * portable_sin(NewvelocityTheta) * portable_cos(NewvelocityPhi);
						Velocity.z = -VelocityMag * portable_sin(NewvelocityTheta) * portable_sin(NewvelocityPhi);
						Velocity.y = VelocityMag * portable_cos(NewvelocityTheta);

						*/


						if(Logging)
						{
							FormatLogMessage("Entrance Front Vector Is %f %f %f", StartPortal.front.x, StartPortal.front.y, StartPortal.front.z); 
							FormatLogMessage("Exit Front Vector Is %f %f %f", PortalList[iter->DestinationIndex].ExitPoint.front.x, PortalList[iter->DestinationIndex].ExitPoint.front.y, PortalList[iter->DestinationIndex].ExitPoint.front.z); 
							FormatLogMessage("Player Front Vector Is %f %f %f", ShipSpawn.front.x, ShipSpawn.front.y, ShipSpawn.front.z); 
							FormatLogMessage("Player Up Vector Is %f %f %f", ShipSpawn.up.x, ShipSpawn.up.y, ShipSpawn.up.z); 
							FormatLogMessage("Player Right Vector Is %f %f %f", ShipSpawn.right.x, ShipSpawn.right.y, ShipSpawn.right.z); 
							FormatLogMessage("Player Velocity Vector Is %f %f %f", ShipVelocity.x, ShipVelocity.y, ShipVelocity.z); 
						//	FormatLogMessage("Angle Differances are: %g", NewAngleH); 
							FormatLogMessage("New Front Vector Is %f %f %f", NewMatrix.front.x, NewMatrix.front.y, NewMatrix.front.z); 
							FormatLogMessage("New Up Vector Is %f %f %f", NewMatrix.up.x, NewMatrix.up.y, NewMatrix.up.z); 
							FormatLogMessage("New Right Vector Is %f %f %f", NewMatrix.right.x, NewMatrix.right.y, NewMatrix.right.z); 
							FormatLogMessage("New Velocity Vector Is %f %f %f", Velocity.x, Velocity.y, Velocity.z); 
						}

						// Speed Bost, override the ship's velocity.
						if(PortalList[iter->DestinationIndex].ExitSpeed != 0.0f)
						{
							if(PortalList[iter->DestinationIndex].ExitForwardOnly)
							{
								/*
								Velocity.x = Velocity.x + (PortalList[iter->DestinationIndex].ExitPoint.front.x * PortalList[iter->DestinationIndex].ExitSpeed);
								Velocity.y = Velocity.y + (PortalList[iter->DestinationIndex].ExitPoint.front.y * PortalList[iter->DestinationIndex].ExitSpeed);
								Velocity.z = Velocity.z + (PortalList[iter->DestinationIndex].ExitPoint.front.z * PortalList[iter->DestinationIndex].ExitSpeed);
								*/
								Velocity = Add_Vectors(Velocity, Vector_Scale(Velocity, PortalList[iter->DestinationIndex].ExitSpeed));
							}
							else
							{
								/*
								Velocity.x = Velocity.x * PortalList[iter->DestinationIndex].ExitSpeed;
								Velocity.y = Velocity.y * PortalList[iter->DestinationIndex].ExitSpeed;
								Velocity.z = Velocity.z * PortalList[iter->DestinationIndex].ExitSpeed;
								*/
								Velocity = Vector_Scale(Velocity, PortalList[iter->DestinationIndex].ExitSpeed);
							}
						}
						// They're not moving. Kick them out a little.
						if((IsNullVector(Velocity)) || 
							((PortalList[iter->DestinationIndex].KickAI) && (!IsPlayer(TempH)))
							)
						{
							/*
							Velocity.x = PortalList[iter->DestinationIndex].ExitPoint.front.x * HighestVelocity;
							Velocity.y = PortalList[iter->DestinationIndex].ExitPoint.front.y * HighestVelocity;
							Velocity.z = PortalList[iter->DestinationIndex].ExitPoint.front.z * HighestVelocity;
							*/
							Velocity = Vector_Scale(PortalList[iter->DestinationIndex].ExitPoint.front, HighestVelocity);
						}

						SetCurAmmo(iter->TeleporterObject, 0);

						// Release target just before teleport.
						if(Tugee)
							Deploy(TempH);

						SetPosition(TempH, NewMatrix);
						SetVelocity(TempH, Velocity);
						//m_TeleporterTargetTeleported[t][d] = true;

					//	/*
						// Rude hacky/fix for tugs. Copy/paste of code above, note: THIS OVERWRITES NewMatrix!
						// Need to rotate the TugOffset to match it's new orientation... :-/
						if(Tugee)
						{
							Vector TugOffset(0, 0, 0);
							Matrix TugSpawn = GetMatrixPosition(Tugee);

							// New code from Ken Miller.
							TugOffset = Vector_TransformInv(ShipSpawn, TugSpawn.posit);
							Vector TugPosition = NewMatrix.posit;
							Matrix NewMatrix = Matrix_Multiply(TugSpawn, Teleport);
							/*
							NewMatrix.right.x = -NewMatrix.right.x;
							NewMatrix.right.y = -NewMatrix.right.y;
							NewMatrix.right.z = -NewMatrix.right.z;
							NewMatrix.front.x = -NewMatrix.front.x;
							NewMatrix.front.y = -NewMatrix.front.y;
							NewMatrix.front.z = -NewMatrix.front.z;
							*/
							NewMatrix.right = Neg_Vector(NewMatrix.right);
							NewMatrix.front = Neg_Vector(NewMatrix.front);
							NewMatrix.posit = TugPosition;
							NewMatrix.posit = Vector_Transform(NewMatrix, TugOffset);

							TeleportTargetList.resize(TeleportTargetList.size()+1);
							TeleportTargetClass &saoB = TeleportTargetList[TeleportTargetList.size()-1]; // No temporary being created, *much* faster
							memset(&saoB, 0, sizeof(TeleportTargetClass)); // Zero things out at start.

							saoB.TeleportTarget = Tugee;
							saoB.DestEnterRange = PortalList[iter->DestinationIndex].EnterRange; // Save it.
							saoB.TeleportTargetDestination = iter->TeleporterDestination; // Save it.

							SetPosition(Tugee, NewMatrix);
							SetVelocity(Tugee, Velocity);

							Pickup(TempH, Tugee);
						}
				//		*/

						// Trigger the Effects to build.
						if(!iter->ContinuousFx)
						{
							SetPortalEffectEnd(iter->TeleporterObject, iter->InEffect);
							SetPortalEffectEnd(iter->TeleporterDestination, iter->OutEffect);
						}
						else
						{
							// Play sound Fx Here.
							char SoundFile[MAX_ODF_LENGTH] = {0};
							GetODFString(iter->TeleporterObject, "TeleportalClass", "soundActivate", MAX_ODF_LENGTH, SoundFile);
							if(DoesFileExist(SoundFile))
								StartAudio3D(SoundFile, iter->TeleporterObject, AUDIO_CAT_EXPLOSION);
							else if(SoundFile[0])
								FormatLogMessage("ERROR: Portal SoundActivate '%s' Doesn't Exist! FIX ODFs!", SoundFile);

							GetODFString(iter->TeleporterDestination, "TeleportalClass", "soundActivate", MAX_ODF_LENGTH, SoundFile);
							if(DoesFileExist(SoundFile))
								StartAudio3D(SoundFile, iter->TeleporterDestination, AUDIO_CAT_EXPLOSION);
							else if(SoundFile[0])
								FormatLogMessage("ERROR: Portal SoundActivate '%s' Doesn't Exist! FIX ODFs!", SoundFile);
						}

						/*
						// Play Teleportation Effects. (After teleportation, so they might play for user on both ends)
						if(DoesODFExist(iter->EffectIn))
						{
							Handle TempPortalEffect = BuildObject(iter->EffectIn, Team, StartPortal);
							SetLifespan(TempPortalEffect, 0.1f);
							SetNoScrapFlagByHandle(TempPortalEffect);
						}
						else if(iter->EffectIn[0])
						{
							FormatLogMessage("Teleport Effect '%s' does not exist. Fix ODFs!", iter->EffectIn); 
						}

						if(DoesODFExist(iter->EffectVeh))
						{
							Handle TempPortalEffect = BuildObject(iter->EffectVeh, Team, ShipSpawn);
							SetLifespan(TempPortalEffect, 0.1f);
							SetNoScrapFlagByHandle(TempPortalEffect);
						}
						else if(iter->EffectVeh[0])
						{
							FormatLogMessage("Teleport Effect '%s' does not exist. Fix ODFs!", iter->EffectVeh); 
						}

						if(DoesODFExist(TempDestEffect))
						{
							Handle TempPortalEffect = BuildObject(TempDestEffect, Team, iter->DestinationPosition);
							SetLifespan(TempPortalEffect, 0.1f);
							SetNoScrapFlagByHandle(TempPortalEffect);
						}
						else if(TempDestEffect[0])
						{
							FormatLogMessage("Teleport Effect '%s' does not exist. Fix ODFs!", TempDestEffect); 
						}
						*/

						Handle TempP = GetPlayerHandle(t);

						if((TempP != 0) && (!Tugee) && (!IsPlayer(TempH))) // Nerf any commands, such as the "goto" the teleporter we just entered. 
						{
							/*
							if(IsAround(TeleporterTap))
								Goto(TempH, TeleporterTap, 0);
							else
								Goto(TempH, iter->TeleporterDestination, 0);
							*/
							Goto(TempH, PortalList[iter->DestinationIndex].GotoPoint.posit, 0);

							SetCommand(TempH, CMD_NONE, 0);
						}

						if(Logging)
							FormatLogMessage("Teleporter on Team: %d Index: %d Aquired Target on Team: %d Index: %d Distance is: %f", Team, iter-PortalList.begin(), t, d, sqrtf(TempDistSquared)); 

					} // Valid Target.
				} // loop d over all targets.
#endif
			} // Destination is around.
		} // Tele can be entered.


		if(iter->EffectTimer > 0)
			--iter->EffectTimer;
		// Generate and Switch Effect states here.
		switch (iter->EffectState)
		{
		case 0: // Effect is off, do nothing.
			{
				break;
			}
		case 1: // Effect State Opening.
			{
				switch (iter->EffectType)
				{
				case 0: // One way.
					{
						// One way building, don't bother with timer or setting, this is done once here.

						// Build portal object (Closing).
					}
					break;
				case 1: // Continuously On.
					{
						// A continuously on portal.
						// Build portal object (Opening).
						// Switch State to Open.
						if(iter->EffectTimer <= 0)
						{
							SetPortalEffectOn(iter->TeleporterObject, iter->EffectNumber, iter->EffectType);
						}
					}
					break;
				case 2: // Build effect. (open then close)
					{
						// Build portal object (Opening).
						// Set State to Close.
						if(iter->EffectTimer <= 0)
						{
							SetPortalEffectEnd(iter->TeleporterObject, iter->EffectNumber, iter->EffectType);
						}
					}
					break;
				}
			}
			break;
		case 2: // Effect State Open.
			{
				if(iter->ContinuousFx)
				{
					Handle Nearest = GetNearestVehicle(iter->TeleporterObject);
					if((!IsAround(iter->TeleporterDestination)) || // If the destination isn't around, or FxRange > 0 and nearest unit is > Range.
						((iter->ContinuousFxRange) && (Nearest) && (GetDistance3D(iter->Position, GetPosition(Nearest)) > iter->ContinuousFxRange)))
					{
						SetPortalEffectEnd(iter->TeleporterObject, iter->EffectNumber, iter->EffectType);
					}
				}
				/*
				switch (iter->EffectType)
				{
				case 0: // One way.
					{
						// Do nothing?
					}
					break;
				case 1: // Continuously On.
					{
						// Watch Timer, when Timer == 0, Remove Portal Opening object, place Portal On object.
					}
					break;
				case 2: // Build effect. (open then close)
					{
						// Do nothing.
					}
					break;
				}
				*/
			}
			break;
		case 3: // Effect State Closing.
			{
				if(IsAround(iter->TeleporterEffect))
				{
					if(iter->EffectTimer <= 0)
					{
						RemoveObject(iter->TeleporterEffect);
					}
				}
				/*
				switch (iter->EffectType)
				{
				case 0: // One way.
					{
						// Do nothing?
					}
					break;
				case 1: // Continuously On.
					{
						// Do nothing? Remove On object, build Closing object.
					}
					break;
				case 2: // Build effect. (open then close)
					{
						// Watch for timer, when == 0, Remove Opening object, build Closing object.
					}
					break;
				}
				*/
			}
			break;
		}

	} // i loop over all Teleporters.
	
	// Loop over all Targets, and see if they cleared a teleporter. 
	for (std::vector<TeleportTargetClass>::iterator iter = TeleportTargetList.begin(); iter != TeleportTargetList.end(); ++iter)
	{
		if(IsAround(iter->TeleportTargetDestination))
		{
			iter->DistanceFromDestination = GetDistance3D(GetPosition(iter->TeleportTarget), GetPosition(iter->TeleportTargetDestination));

			if(Logging)
				FormatLogMessage("Teleporter Target Index: %d Just Teleported, Distnace is: %f", iter-TeleportTargetList.begin(), iter->DistanceFromDestination); 
		}
	}
}

void PortalHelper::ExecuteHandle(int HandleListIndex)
{
	bool Logging = BZ1Helper::Logging; //false;

	BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(HandleListIndex);
	Handle TempH = objectInfo.ObjectHandle;
	Vector DestGoto(0, 0, 0);
	float TempDistSquared = FLT_MAX;
	Handle Tugee = 0;
	Vector Velocity(0, 0, 0);

	for (std::vector<TeleportTargetClass>::iterator iter = TeleportTargetList.begin(); iter != TeleportTargetList.end(); ++iter)
	{
		if(iter->TeleportTarget == TempH)
			return; // Skip this object, it's already a Teleporter Target.
	}

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == TempH)
			return; // Skip this object, it's a teleporter.

		for(int x = 0; x < MAX_TAPS;x++)
		{
			Handle CheckTap = GetTap(iter->TeleporterObject, x);
			if(TempH == CheckTap)
				return; // Skip this object, it's a tap of a Teleporter.
		}
	}

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(!iter->TeleporterDestination) //if(!PortalList[iter->DestinationIndex].TeleporterDestination)
			continue; // This teleporter has no destination, skip it.

		//FormatLogMessage("Distance on Object Team: %d, Index: %d is: %f", t, d, TempDistSquared); 

		long TempCurAmmo = GetCurAmmo(iter->TeleporterObject);
		long TempMaxAmmo = GetMaxAmmo(iter->TeleporterObject);
		int Team = GetTeamNum(iter->TeleporterObject);
		int t = objectInfo.CurTeamNum;
		TempDistSquared = GetDistance3DSquared(objectInfo.Position, iter->Position);

		// Conditions for Teleportation.
		if( //(objectInfo.CanTeleport) && // This object can be teleported.
			(TempDistSquared < Squared(iter->EnterRange)) && // Its within Range.
			(TempCurAmmo == TempMaxAmmo) && // Its ammo is fully recharged.
			(TeamFilterCheck(iter->TargetTeamFilter, t, Team)) && // Teamfilter passes.
		//	(BuildingHelper::Get().IsBuildingPowered(iter->TeleporterDestination)) &&  // Destination Portal Has Power.
			((((iter->TeleportFilter & 1) == 1) && (objectInfo.CategoryID == Vehicle)) || //IsVehicle(TempH)) || // Teleport Ships.
			(((iter->TeleportFilter & 2) == 2) && (objectInfo.CategoryID == Person)) || //IsPerson(TempH)) || // Teleport Pilots.
			(((iter->TeleportFilter & 4) == 4) && (objectInfo.CategoryID == Powerup)) || //IsPowerup(TempH)) || // Teleport Powerups.
			(((iter->TeleportFilter & 8) == 8) && (objectInfo.CategoryID == Weapon)) || //IsWeapon(TempH)) || // Teleport Weapons.
			(((iter->TeleportFilter & 16) == 16) && (objectInfo.CategoryID == Other))) && //IsMisc(TempH)) ) && // Teleport Misc.
		//	(!DontTeleport) && // Target didn't just teleport, or isnt a tap of our teleporter.
			(GetTug(TempH) == 0) && // Target isn't being tugged.
			(!IsNullVector(PortalList[iter->DestinationIndex].ExitPoint.posit)) // It's position isn't NULL.
			)
		{

			if(Logging)
				FormatLogMessage("Locking onto Object: %08x, ODF: %s, Distance is: %f", TempH, GetODFName(TempH), sqrtf(TempDistSquared));

			// Look for this object to be tugging something, if so, save it.
			for (std::vector<TeleportTargetClass>::iterator iterB = TeleportTargetList.begin(); iterB != TeleportTargetList.end(); ++iterB)
			{
				if(GetTug(iterB->TeleportTarget) == TempH)
				{
					Tugee = iterB->TeleportTarget;
					break;
				}
			}

			// We're good to Teleport, add the Teleportee to the List.
			TeleportTargetList.resize(TeleportTargetList.size()+1);
			TeleportTargetClass &sao = TeleportTargetList[TeleportTargetList.size()-1]; // No temporary being created, *much* faster
			memset(&sao, 0, sizeof(TeleportTargetClass)); // Zero things out at start.

			sao.TeleportTarget = TempH; // Save it.
			sao.DestEnterRange = PortalList[iter->DestinationIndex].EnterRange; //TempDestRange; // Save it.
			sao.TeleportTargetDestination = iter->TeleporterDestination; // Save it.

			const Matrix &StartPortal = iter->ExitPoint; //GetODFPositionOffset(iter->TeleporterObject, 0);
			Matrix ShipSpawn = GetMatrixPosition(TempH);

			float HighestVelocity = 20;

			Vector ShipVelocity = GetVelocity(TempH);

			const Matrix &EndPortal = PortalList[iter->DestinationIndex].ExitPoint; //iter->DestinationPosition;

			// New code from Ken Miller.
			Matrix InvStartPortal = Matrix_Inverse(StartPortal);
			Matrix Teleport = Matrix_Multiply(InvStartPortal, EndPortal);
			Matrix NewMatrix = Matrix_Multiply(ShipSpawn, Teleport);
			Velocity = Vector_Rotate(Teleport, ShipVelocity);

			NewMatrix.right = Neg_Vector(NewMatrix.right);
			NewMatrix.front = Neg_Vector(NewMatrix.front);
			Velocity.x = -Velocity.x;
			Velocity.z = -Velocity.z;

			// Override position floating, use root exit position.
			if(PortalList[iter->DestinationIndex].UseCenter)
				NewMatrix.posit = PortalList[iter->DestinationIndex].ExitPoint.posit; //iter->DestinationPosition.posit; 

			if(Logging)
			{
				FormatLogMessage("Entrance Front Vector Is %f %f %f", StartPortal.front.x, StartPortal.front.y, StartPortal.front.z); 
				FormatLogMessage("Exit Front Vector Is %f %f %f", PortalList[iter->DestinationIndex].ExitPoint.front.x, PortalList[iter->DestinationIndex].ExitPoint.front.y, PortalList[iter->DestinationIndex].ExitPoint.front.z); 
				FormatLogMessage("Player Front Vector Is %f %f %f", ShipSpawn.front.x, ShipSpawn.front.y, ShipSpawn.front.z); 
				FormatLogMessage("Player Up Vector Is %f %f %f", ShipSpawn.up.x, ShipSpawn.up.y, ShipSpawn.up.z); 
				FormatLogMessage("Player Right Vector Is %f %f %f", ShipSpawn.right.x, ShipSpawn.right.y, ShipSpawn.right.z); 
				FormatLogMessage("Player Velocity Vector Is %f %f %f", ShipVelocity.x, ShipVelocity.y, ShipVelocity.z); 
			//	FormatLogMessage("Angle Differances are: %g", NewAngleH); 
				FormatLogMessage("New Front Vector Is %f %f %f", NewMatrix.front.x, NewMatrix.front.y, NewMatrix.front.z); 
				FormatLogMessage("New Up Vector Is %f %f %f", NewMatrix.up.x, NewMatrix.up.y, NewMatrix.up.z); 
				FormatLogMessage("New Right Vector Is %f %f %f", NewMatrix.right.x, NewMatrix.right.y, NewMatrix.right.z); 
				FormatLogMessage("New Velocity Vector Is %f %f %f", Velocity.x, Velocity.y, Velocity.z); 
			}

			// Speed Bost, override the ship's velocity.
			if(PortalList[iter->DestinationIndex].ExitSpeed)
			{
				if(PortalList[iter->DestinationIndex].ExitForwardOnly)
					Velocity = Add_Vectors(Velocity, Vector_Scale(Velocity, PortalList[iter->DestinationIndex].ExitSpeed));
				else
					Velocity = Vector_Scale(Velocity, PortalList[iter->DestinationIndex].ExitSpeed);
			}

			// They're not moving. Kick them out a little.
			if((IsNullVector(Velocity)) || ((PortalList[iter->DestinationIndex].KickAI) && (!IsPlayer(TempH))))
				Velocity = Vector_Scale(PortalList[iter->DestinationIndex].ExitPoint.front, HighestVelocity);

			SetCurAmmo(iter->TeleporterObject, 0);

			// Release target just before teleport.
			if(Tugee)
				Deploy(TempH);

			SetPosition(TempH, NewMatrix);
			SetVelocity(TempH, Velocity);

		//	/*
			// Rude hacky/fix for tugs. Copy/paste of code above, note: THIS OVERWRITES NewMatrix!
			// Need to rotate the TugOffset to match it's new orientation... :-/
			if(Tugee)
			{
				Vector TugOffset(0, 0, 0);
				Matrix TugSpawn = GetMatrixPosition(Tugee);

				// New code from Ken Miller.
				TugOffset = Vector_TransformInv(ShipSpawn, TugSpawn.posit);
				Vector TugPosition = NewMatrix.posit;
				Matrix NewMatrix = Matrix_Multiply(TugSpawn, Teleport);

				NewMatrix.right = Neg_Vector(NewMatrix.right);
				NewMatrix.front = Neg_Vector(NewMatrix.front);
				NewMatrix.posit = TugPosition;
				NewMatrix.posit = Vector_Transform(NewMatrix, TugOffset);

				TeleportTargetList.resize(TeleportTargetList.size()+1);
				TeleportTargetClass &saoB = TeleportTargetList[TeleportTargetList.size()-1]; // No temporary being created, *much* faster
				memset(&saoB, 0, sizeof(TeleportTargetClass)); // Zero things out at start.

				saoB.TeleportTarget = Tugee;
				saoB.DestEnterRange = PortalList[iter->DestinationIndex].EnterRange; // Save it.
				saoB.TeleportTargetDestination = iter->TeleporterDestination; // Save it.

				SetPosition(Tugee, NewMatrix);
				SetVelocity(Tugee, Velocity);

				Pickup(TempH, Tugee);
			}
	//		*/

			// Trigger the Effects to build.
			if(!iter->ContinuousFx)
			{
				SetPortalEffectEnd(iter->TeleporterObject, iter->InEffect);
				SetPortalEffectEnd(iter->TeleporterDestination, iter->OutEffect);
			}
			else
			{
				// Play sound Fx Here.
				char SoundFile[MAX_ODF_LENGTH] = {0};
				GetODFString(iter->TeleporterObject, "TeleportalClass", "soundActivate", MAX_ODF_LENGTH, SoundFile);
				if(DoesFileExist(SoundFile))
					StartAudio3D(SoundFile, iter->TeleporterObject, AUDIO_CAT_EXPLOSION);
				else if(SoundFile[0])
					FormatLogMessage("ERROR: Portal SoundActivate '%s' Doesn't Exist! FIX ODFs!", SoundFile);

				GetODFString(iter->TeleporterDestination, "TeleportalClass", "soundActivate", MAX_ODF_LENGTH, SoundFile);
				if(DoesFileExist(SoundFile))
					StartAudio3D(SoundFile, iter->TeleporterDestination, AUDIO_CAT_EXPLOSION);
				else if(SoundFile[0])
					FormatLogMessage("ERROR: Portal SoundActivate '%s' Doesn't Exist! FIX ODFs!", SoundFile);
			}

			Handle TempP = GetPlayerHandle(t);

			if((TempP != 0) && (!Tugee) && (!IsPlayer(TempH))) // Nerf any commands, such as the "goto" the teleporter we just entered. 
			{
				Goto(TempH, PortalList[iter->DestinationIndex].GotoPoint.posit, 0);

				SetCommand(TempH, CMD_NONE, 0);
			}

			if(Logging)
				FormatLogMessage("Teleporter on Team: %d Index: %d Aquired Target on Team: %d Index: %d Distance is: %f", Team, iter-PortalList.begin(), t, HandleListIndex, sqrtf(TempDistSquared)); 

			break; // We found a valid teleporter, teleportation done. Exit early.
		} // Valid Target.
	} // loop d over all Portals
}

void PortalHelper::SetPortalChannel(const Handle h, const int Channel)
{
	if(!IsAround(h))
		return;

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
		{
			iter->Channel = Channel;
			break;
		}
	}
}

int PortalHelper::GetPortalChannel(const Handle h)
{
	if(!IsAround(h))
		return -1;

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
			return iter->Channel;
	}
	return -1;
}

// Portal Effect Controllers.

// Starts a portal Effect. Type defines the color. 0 = Blue, 1 = Red, 2 = Green. Types defined in portal ODF, though, so they could be anything. :)
void PortalHelper::SetPortalEffectStart(const Handle h, const int FxNumber, const int Type)
{
	if(!IsAround(h))
		return;

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
		{
			if(!IsAround(iter->TeleporterEffect))
			{
				if(Type == 0)
				{
					SetPortalEffectEnd(h, FxNumber, Type);
				}
				else
				{
					char TempEffectODF[MAX_ODF_LENGTH] = {0};
					sprintf_s(TempEffectODF, "%s%dStart", iter->EffectBaseODF, FxNumber);
					if(DoesODFExist(TempEffectODF))
					{
						//Matrix PortalPos = iter->ExitPoint; //GetODFPositionOffset(iter->TeleporterObject, 0);
						/*
						memset(&PortalPos, 0, sizeof(PortalPos));
						Handle StartTap = GetTap(iter->TeleporterObject, 0);
						if(IsAround(StartTap))
							GetPosition(StartTap, PortalPos);
						else
							GetPosition(iter->TeleporterObject, PortalPos); // Save this here.
						*/

						iter->TeleporterEffect = BuildObject(TempEffectODF, 0, iter->ExitPoint);

						char SoundFile[MAX_ODF_LENGTH] = {0};
						GetODFString(iter->TeleporterObject, "TeleportalClass", "soundActivate", MAX_ODF_LENGTH, SoundFile);
						if(DoesFileExist(SoundFile))
							StartAudio3D(SoundFile, iter->TeleporterObject, AUDIO_CAT_EXPLOSION);
						else if(SoundFile[0])
							FormatLogMessage("ERROR: Portal SoundActivate '%s' Doesn't Exist! FIX ODFs!", SoundFile);

						strcat_s(TempEffectODF, ".odf"); // Add .odf to the end.

						float TempLifeTime = 0.0f;
						if((OpenODF2(TempEffectODF)) && (GetODFFloat(TempEffectODF, "Render", "lifeTime", &TempLifeTime)))
						{
							// Got it.
							iter->EffectTimer = ConvertToTurns(TempLifeTime);
						}
						iter->EffectType = Type;
						iter->EffectState = 1;
						iter->EffectNumber = FxNumber;
					}
					else if(iter->EffectBaseODF[0])
					{
						FormatLogMessage("Teleport Effect '%s' does not exist. Fix ODFs!", TempEffectODF); 
					}
				}
			}
			else // An Effect Object is already around. Should we override it?
			{
				if(Type == PortalBuild)
				{
					SetPortalEffectEnd(h, FxNumber, Type);
				}
				else if(Type == PortalOn)
				{
					SetPortalEffectOn(h, FxNumber, Type);
				}
			}
			break;
		}
	}
}

void PortalHelper::SetPortalEffectOn(const Handle h, const int FxNumber, const int Type)
{
	if(!IsAround(h))
		return;

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
		{
			char TempEffectODF[MAX_ODF_LENGTH] = {0};
			sprintf_s(TempEffectODF, "%s%d", iter->EffectBaseODF, FxNumber);
			if(DoesODFExist(TempEffectODF))
			{
				//Matrix PortalPos = iter->ExitPoint; //GetODFPositionOffset(iter->TeleporterObject, 0);
				/*
				memset(&PortalPos, 0, sizeof(PortalPos));
				Handle StartTap = GetTap(iter->TeleporterObject, 0);
				if(IsAround(StartTap))
					GetPosition(StartTap, PortalPos);
				else
					GetPosition(iter->TeleporterObject, PortalPos); // Save this here.
				*/

				if(IsAround(iter->TeleporterEffect))
					RemoveObject(iter->TeleporterEffect);

				iter->TeleporterEffect = BuildObject(TempEffectODF, 0, iter->ExitPoint);

				iter->EffectType = Type;
				iter->EffectState = 2;
				iter->EffectNumber = FxNumber;
			}
			else if(iter->EffectBaseODF[0])
			{
				FormatLogMessage("Teleport Effect '%s' does not exist. Fix ODFs!", TempEffectODF); 
			}
		}
	}
}

void PortalHelper::SetPortalEffectEnd(const Handle h, const int FxNumber, const int Type)
{
	if(!IsAround(h))
		return;

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
		{
			char TempEffectODF[MAX_ODF_LENGTH] = {0};
			sprintf_s(TempEffectODF, "%s%dEnd", iter->EffectBaseODF, FxNumber);
			if(DoesODFExist(TempEffectODF))
			{
				//Matrix PortalPos = iter->ExitPoint; //GetODFPositionOffset(iter->TeleporterObject, 0);
				/*
				memset(&PortalPos, 0, sizeof(PortalPos));
				Handle StartTap = GetTap(iter->TeleporterObject, 0);
				if(IsAround(StartTap))
					GetPosition(StartTap, PortalPos);
				else
					GetPosition(iter->TeleporterObject, PortalPos); // Save this here.
				*/

				if(IsAround(iter->TeleporterEffect))
					RemoveObject(iter->TeleporterEffect);

				iter->TeleporterEffect = BuildObject(TempEffectODF, 0, iter->ExitPoint);

				char SoundFile[MAX_ODF_LENGTH] = {0};
				GetODFString(iter->TeleporterObject, "TeleportalClass", "soundActivate", MAX_ODF_LENGTH, SoundFile);
				if(DoesFileExist(SoundFile))
					StartAudio3D(SoundFile, iter->TeleporterObject, AUDIO_CAT_EXPLOSION);
				else if(SoundFile[0])
					FormatLogMessage("ERROR: Portal SoundActivate '%s' Doesn't Exist! FIX ODFs!", SoundFile);

				strcat_s(TempEffectODF, ".odf"); // Add .odf to the end.
				
				float TempLifeTime = 0.0f;
				if((OpenODF2(TempEffectODF)) && (GetODFFloat(TempEffectODF, "Render", "lifeTime", &TempLifeTime)))
				{
					// Got it.
					iter->EffectTimer = ConvertToTurns(TempLifeTime);
				}

				iter->EffectType = Type;
				iter->EffectState = 3;
				iter->EffectNumber = FxNumber;
			}
			else if(iter->EffectBaseODF[0])
			{
				FormatLogMessage("Teleport Effect '%s' does not exist. Fix ODFs!", TempEffectODF); 
			}
		}
	}
}

// Is this portal currently active?
bool PortalHelper::IsPortalEffectActive(const Handle h)
{
	if(!IsAround(h))
		return false;

	for (std::vector<PortalClass>::iterator iter = PortalList.begin(); iter != PortalList.end(); ++iter)
	{
		if(iter->TeleporterObject == h)
			return IsAround(iter->TeleporterEffect);
	}
	return false;
}