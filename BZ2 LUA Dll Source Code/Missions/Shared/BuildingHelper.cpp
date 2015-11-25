#include "BZ1Helper.h"
#include "BuildingHelper.h"
#include "PowerHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

BuildingHelper *BuildingHelper::m_BuildingHelper;

BuildingHelper::BuildingHelper()
{
	m_BuildingHelper = this;
}
BuildingHelper::~BuildingHelper()
{
	m_BuildingHelper = NULL;
}

bool BuildingHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		BuildingList.resize(size);
		if(size)
			Read(&BuildingList.front(), sizeof(BuildingClass)*size);

		Read(&size, 1);
		RepairPowerupList.resize(size);
		if(size)
			Read(&RepairPowerupList.front(), sizeof(RepairPowerupClass)*size);
	}
	return true;
}

bool BuildingHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
		{
			BuildingClass &sao = *iter;
			ConvertHandles(&sao.BuildingObject, 1);
			for(int i = 0; i < MAX_TAPS; i++)
				ConvertHandles(&sao.BuildingTap[i], 1);
		}

		for (std::vector<RepairPowerupClass>::iterator iter = RepairPowerupList.begin(); iter != RepairPowerupList.end(); ++iter)
		{
			RepairPowerupClass &sao = *iter;
			ConvertHandles(&sao.RepairPowerupObject, 1);
		}
	}
	return true;
}

bool BuildingHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(BuildingList.size());
		Write(&size, 1);
		if(size)
			Write(&BuildingList.front(), sizeof(BuildingClass)*size);

		size = int(RepairPowerupList.size());
		Write(&size, 1);
		if(size)
			Write(&RepairPowerupList.front(), sizeof(RepairPowerupClass)*size);
	}
	return true;
}

void BuildingHelper::AddRepairPowerupObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team)
{
	for (std::vector<RepairPowerupClass>::iterator iter = RepairPowerupList.begin(); iter != RepairPowerupList.end(); ++iter)
	{
		if(iter->RepairPowerupObject == h)
			return;
	}

	RepairPowerupList.resize(RepairPowerupList.size()+1);
	RepairPowerupClass &sao = RepairPowerupList[RepairPowerupList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(RepairPowerupClass)); // Zero things out at start.

	sao.RepairPowerupObject = h;
	if(GetODFLong(TempODF1, TempODF2, "HealthPowerupClass", "healthUp", &sao.HealthUp) || 
		GetODFLong(TempODF1, TempODF2, "ServicePowerupClass", "serviceUp", &sao.HealthUp))
	{
		// Got it.
	}

	GetODFString(TempODF1, TempODF2, "PowerUpClass", "soundPickup", MAX_ODF_LENGTH, sao.SoundPickup); // Grab the animation name.
	GetODFString(TempODF1, TempODF2, "PowerUpClass", "soundReject", MAX_ODF_LENGTH, sao.SoundReject); // Grab the animation name.

	sao.DelayTime = ConvertToTurns(1.0f);
}

void BuildingHelper::AddBuildingObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int ODFNameHash, const int ObjClassHash, const int BZCClassHash)
{
	int BuildingListIndex = -1;
	for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
	{
		if(iter->BuildingObject == h)
		{
			if(iter->ODFNameHash == unsigned int(ODFNameHash))
				return;

			// Temp re-count of taps.
			int Taps = 0;
			for(int x = 0; x < MAX_TAPS; x++)
			{
				Handle TempTap = GetTap(iter->BuildingObject, x);

				if((TempTap) && (GetMaxHealth(TempTap) > 0))
					++Taps;
			}
			// Now update tap number if it doesn't match.
			if(iter->TapNumber != Taps)
				iter->TapNumber = Taps;

			BuildingListIndex = iter - BuildingList.begin();

			break;
		}
	}

	if(BuildingListIndex < 0) // New Object, Increase list size to compensate.
	{
		BuildingList.resize(BuildingList.size()+1);
		BuildingListIndex = BuildingList.size()-1; // Use the new index.
	}
	BuildingClass &sao = BuildingList[BuildingList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(BuildingClass)); // Zero things out at start.

	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "RequirePowerMask", &sao.PowerType, 0);
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanBeIdleVictim", &sao.CanBeIdleVictim, true);

	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "GameObjectClassPowerMask", &sao.PowerMask, 0);
	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "GameObjectClassPowerMatch", &sao.PowerMatch, 0);
	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "GameObjectClassPowerProvides", &sao.PowerProvide, 0);

	GetODFFloat(TempODF1, TempODF2, "GameObjectClass", "CollisionRadius", &sao.CollisionRadius, 0.0f);
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanAcceptPowerups", &sao.CanAcceptPowerups, false);
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanCollide", &sao.CanCollide, false);

	sao.CurTeamNum = Team;
	bool IsApcRefillDefault = (BZCClassHash == BZCCLASS_RECYCLER || IsBarracks(h)); //(h == BZ1Helper::Get().GetRecy(Team) || IsBarracks(h));
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "APCRefillPoint", &sao.IsAPCRefill, IsApcRefillDefault);

	// Keep track of dummy recycle points, we have to reset base slot 1.
	bool RecyclePoint = (((BZCClassHash == BZCCLASS_RECYCLER) && (IsBuilding(h))) || (IsSilo(h))); //(((h == BZ1Helper::Get().GetRecy(Team)) && (IsBuilding(h))) || (IsSilo(h)));
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "RecyclePoint", &sao.IsRecyclePoint, RecyclePoint);

	bool Animated = true;
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "IsAnimated", &Animated, true);

	if(Animated)
	{
		bool HasAnim1 = false;
		bool HasAnim2 = false;
		char AnimationName[MAX_ODF_LENGTH] = {0};
		int AnimationCount = 0;
		GetODFInt(TempODF1, TempODF2, "GameObjectClass", "animCount", &AnimationCount);
		// Look for new ODF Parameters. 
		for (int i = 1; i < AnimationCount+1; i++) // Look through 16 animnames. If you made a unit that has more then 16 anims, 1: Congratulations, and 2: The loop and off ones better be between 1 and 16.
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "animName%d", i);
			if(GetODFString(TempODF1, TempODF2, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, AnimationName)) // Grab the animation name.
			{
				if((_stricmp(AnimationName,"BZCloop")) == 0)
					HasAnim1 = true;
				else if((_stricmp(AnimationName,"BZCoff")) == 0)
					HasAnim2 = true;

				if((HasAnim1) && (HasAnim2))
				{
					sao.IsAnimated = true;
					break; // We found it, abort!
				}
			}
			// !-- ? Does BZ2 stop looking for anim names if there's a gap? if so, else { break; } here.
		}
	}

	Matrix HostMatrix = GetMatrixPosition(h);
	int Taps = 0;
	for(int x = 0; x < MAX_TAPS; x++)
	{
		Handle TempTap = GetTap(h, x);
		char ODFName[MAX_ODF_LENGTH] = {0};
		char TempODF3[MAX_ODF_LENGTH] = {0};
		char TempODF4[MAX_ODF_LENGTH] = {0};
		GetAndOpenODFNames(TempTap, TempODF3, TempODF4, ODFName);
		//if(IsAround(TempTap)) // Bad, IsAround fails in AddObject since the tap kindof doesn't exist yet.
	//	if((TempTap) && (GetMaxHealth(TempTap) > 0))
	//		++Taps;
	//	else
	//		continue;

		if(ODFName[0])
		{
			++Taps;

			sao.BuildingTap[x] = TempTap;
			Matrix TapMatrix = GetMatrixPosition(TempTap);
			Matrix InvHostMatrix = Matrix_Inverse(HostMatrix);
			sao.TapOffsetMatrix[x] = Matrix_Multiply(TapMatrix, InvHostMatrix);

			// Find out if the taps are animated.
			bool HasAnim1 = false;
			bool HasAnim2 = false;
			char AnimationName[MAX_ODF_LENGTH] = {0};
			int AnimationCount = 0;
			GetODFInt(TempODF3, TempODF4, "GameObjectClass", "animCount", &AnimationCount);
			// Look for new ODF Parameters. 
			for (int a = 1; a < AnimationCount+1; a++) // Look through 16 animnames. If you made a unit that has more then 16 anims, 1: Congratulations, and 2: The loop and off ones better be between 1 and 16.
			{
				char DesiredValue[MAX_ODF_LENGTH] = {0};
				sprintf_s(DesiredValue, "animName%d", a);
				if(GetODFString(TempODF3, TempODF4, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, AnimationName)) // Grab the animation name.
				{
					if((_stricmp(AnimationName,"BZCloop")) == 0)
						HasAnim1 = true;
					else if((_stricmp(AnimationName,"BZCoff")) == 0)
						HasAnim2 = true;

					if((HasAnim1) && (HasAnim2))
					{
						sao.TapAnimated[x] = true;
						break; // We found it, abort!
					}
				}
			}
		}
		// !-- ? Does BZ2 stop looking for anim names if there's a gap? if so, else { break; } here.
	}
	sao.TapNumber = Taps;
	sao.ODFNameHash = ODFNameHash;
	sao.ObjClassHash = ObjClassHash;

	sao.PowerState = -1; // Autodetermine.
	sao.BuildingObject = h;
	sao.Position = GetPosition(h);
	sao.BuildingFront = HostMatrix.front;
}

void BuildingHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging; //false;

	// pre-clean the vector
	BuildingList.erase(std::remove_if(BuildingList.begin(), BuildingList.end(), ShouldRemoveBuilding), BuildingList.end());
	RepairPowerupList.erase(std::remove_if(RepairPowerupList.begin(), RepairPowerupList.end(), ShouldRemoveRepairPowerup), RepairPowerupList.end());

	// 
	for (std::vector<RepairPowerupClass>::iterator iter = RepairPowerupList.begin(); iter != RepairPowerupList.end(); ++iter)
	{
		if(iter->DelayTime > 0)
			--iter->DelayTime;
	}

	for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
	{
		iter->CurTeamNum = GetTeamNum(iter->BuildingObject); // Update this every tick once here.
		Matrix HostMatrix = GetMatrixPosition(iter->BuildingObject);
		int PTeam = GetPerceivedTeam(iter->BuildingObject);

		//	Handle PowerTap = GetTap(iter->BuildingObject, 0);
		//	float Power = GetCurLocalAmmo(PowerTap, 0); // Pass it's ammo into a tempvar.
		// If we're on Autodetermine, use normal behavior.
		if(iter->PowerState < 0)
		{
			bool IsInRangeOfPower = false;
			if((iter->PowerType & 1) == PType_BZ1) // Requires BZ1 Power.
			{
				for(int i = 0; i < PowerHelper::Get().GetNumPowerObjects(); i++)
				{
					PowerHelper::PowerClass powerInfo = PowerHelper::Get().GetPowerInfo(i);
					if(	(powerInfo.IsPowered) && // Is this power plant powered? //(IsBuildingPowered(TempPower)) && // *brain explodes* Infitite loop! BAD!
						(IsTeamAllied(iter->CurTeamNum, powerInfo.PowerTeam)) && // Teams are allied.
						(GetDistance3D(iter->Position, powerInfo.Position) < powerInfo.PowerRange) && // Is in range of PowerPlant.
						(((powerInfo.PowerProvide & iter->PowerMask) == iter->PowerMatch) && // Power Provides match Building requirements.
						((iter->PowerProvide & powerInfo.PowerMask) == powerInfo.PowerMatch)) ) // Building Provides match Power requirements.
					{
						IsInRangeOfPower = true;
						break;
					}
				}
			}

			int Taps = GetTapCount(iter->BuildingObject, true);

		//	if( ((!iter->UseBZ1Power) || (Power != 0)) && // Requires BZ1 Power?
			/*
			if( ((!iter->UseBZ1Power) || (IsInRangeOfPower)) && // Requires BZ1 Power?
				((!iter->UseBZ2Power) || (GetPower(iter->CurTeamNum) >= 0)) && // Requires BZ2 Power?
				((!iter->UseLungPower) || (Taps == iter->TapNumber)) )// Requires Lung Power?
			*/
			if( (iter->PowerType == PType_None) || // Power Type is 0 == None.
				(((iter->PowerType & 1) == PType_BZ1) && (IsInRangeOfPower)) || // PowerType is 1 == Requires BZ1 Power.
				(((iter->PowerType & 2) == PType_BZ2) && (GetPower(iter->CurTeamNum) >= 0)) || // PowerType is 2 == Requires BZ2 Power.
				(((iter->PowerType & 4) == PType_Lung) && (Taps == iter->TapNumber))) // PowerType is 4 == Requires Lung Power.
				iter->IsPowered =  true;
			else
				iter->IsPowered =  false;
		}
		else
		{
			iter->IsPowered = iter->PowerState; // Defer to DLL control.
		}

		// Main Building Animations.
		if(iter->IsAnimated)
		{
			if(iter->IsPowered)
			{
				if(!iter->PlayAnim)
				{
					//if(GetAnimationFrame(iter->BuildingObject, "BZCoff") >= iter->MaxAnimationFrame[0])
					if(fabsf(GetAnimationFrame(iter->BuildingObject, "BZCoff") - iter->MaxAnimationFrame[0]) < 0.5f)
					{
						iter->PlayAnim = true;
						iter->MaxAnimationFrame[0] = SetAnimation(iter->BuildingObject, "BZCLoop", 0); // Restart the animation. 
						StartAnimation(iter->BuildingObject);
					}
				}
			}
			else if(iter->PlayAnim)
			{
				// minimize the snaping.
				//if(GetAnimationFrame(iter->BuildingObject, "BZCLoop") >= iter->MaxAnimationFrame[0])
				if(fabsf(GetAnimationFrame(iter->BuildingObject, "BZCLoop") - iter->MaxAnimationFrame[0]) < 0.5f)
				{
					iter->PlayAnim = false;
					iter->MaxAnimationFrame[0] = SetAnimation(iter->BuildingObject, "BZCoff", 0); // Restart the animation. 
					StartAnimation(iter->BuildingObject);
				}
			}
		}

		// Tap animations.
		for(int x = 0; x < iter->TapNumber; x++)
		{
		//	Handle TempTap = GetTap(iter->BuildingObject, x);

			// Make sure TAPS are on the same Team. (If DLL swaps the items team, the TAPS don't change with it.
			//if(TempTap)
			if(IsAround(iter->BuildingTap[x])) // If the tap is around, watch for it's position to move. Keep its position compared to its parent.
			{
				Matrix TapMatrix = GetMatrixPosition(iter->BuildingTap[x]);
				
				/*
				// If this object switched teams, make sure it's Tap's Teams match. 
				Handle TempTap = GetTap(iter->TapBuildingObject, x);
				if(IsAround(TempTap))
				{
					if(GetTeamNum(TempTap) != Team)
						SetTeamNum(TempTap, Team);

					if(GetPerceivedTeam(TempTap) != PTeam)
						SetPerceivedTeam(TempTap, PTeam);
				}
				*/

				if(iter->CurTeamNum != GetTeamNum(iter->BuildingTap[x]))
					SetTeamNum(iter->BuildingTap[x], iter->CurTeamNum);

				if(GetPerceivedTeam(iter->BuildingTap[x]) != PTeam)
					SetPerceivedTeam(iter->BuildingTap[x], PTeam);

				if(iter->TapAnimated[x])
				{
					if(iter->IsPowered)
					{
						if(!iter->PlayTapAnim[x])
						{
							if(GetAnimationFrame(iter->BuildingTap[x], "BZCoff") >= iter->MaxAnimationFrame[x+1])
							{
								iter->PlayTapAnim[x] = true;
								iter->MaxAnimationFrame[x+1] = SetAnimation(iter->BuildingTap[x], "BZCLoop", 0); // Restart the animation. 
								StartAnimation(iter->BuildingTap[x]);
							}
						}
					}
					else if(iter->PlayTapAnim[x])
					{
						// minimize the snaping.
						if(GetAnimationFrame(iter->BuildingTap[x], "BZCLoop") >= iter->MaxAnimationFrame[x+1])
						{
							iter->PlayTapAnim[x] = false;
							iter->MaxAnimationFrame[x+1] = SetAnimation(iter->BuildingTap[x], "BZCoff", 0); // Restart the animation. 
							StartAnimation(iter->BuildingTap[x]);
						}
					}
				}

				// If its position is off from what we have stored, fix it.
				if((!close_enough(iter->TapOffsetMatrix[x].posit, Vector_TransformInv(HostMatrix, TapMatrix.posit), 0.01f)) || //Sub_Vectors(TapMatrix.posit, HostMatrix.posit), 0.01f)) ||
					(!close_enough(iter->BuildingFront, HostMatrix.front, 0.01f)))
				{
					Matrix NewMatrix = Matrix_Multiply(iter->TapOffsetMatrix[x], HostMatrix);
					SetPosition(iter->BuildingTap[x], NewMatrix);

					if(Logging)
						FormatLogMessage("Tap Not Matching Host: %08x %s! Current Tap: %08x %s Old Offset: %f %f %f New Offset: %f %f %f, Old Building Front: %f %f %f New Building Front: %f %f %f", iter->BuildingObject, GetODFName(iter->BuildingObject), iter->BuildingTap[x], GetODFName(iter->BuildingTap[x]), iter->TapOffsetMatrix[x].posit.x, iter->TapOffsetMatrix[x].posit.y, iter->TapOffsetMatrix[x].posit.z, (TapMatrix.posit.x - HostMatrix.posit.x), (TapMatrix.posit.y - HostMatrix.posit.y), (TapMatrix.posit.z - HostMatrix.posit.z), iter->BuildingFront.x, iter->BuildingFront.y, iter->BuildingFront.z, HostMatrix.front.x, HostMatrix.front.y, HostMatrix.front.z);

					// Save new Offset Matrix.
					Matrix NewTapMatrix = GetMatrixPosition(iter->BuildingTap[x]);
					Matrix InvHostMatrix = Matrix_Inverse(HostMatrix);
					iter->TapOffsetMatrix[x] = Matrix_Multiply(NewTapMatrix, InvHostMatrix);
					// Save new Front.
					iter->BuildingFront = HostMatrix.front;
				}
			}
		//--- Old Code, 1.3 PB6.5 and < doesn't retain the Handle ID when a building is Upgraded, so we have to watch for a tap to be suddenly replaced by something else.
			else if(BZ1Helper::m_GameVersion <= v13PB65) // This tap is either A: non existant, or B: currently missing. Hard to tell, so look for a building in the *exact* same place as the original tap.
			{
				// If this position was ever anything other then 0, there used to be a tap here. Look for a potentially upgraded replacement. 
				if(!IsNullVector(iter->TapOffsetMatrix[x].posit))
				{
					for(int t = 0; t < BZ1Helper::Get().GetNumHandles(); t++)
					{
						Handle TempH = BZ1Helper::Get().GetHandleInfo(t).ObjectHandle;
						if(iter->BuildingObject != TempH) // This is NOT the same as our host.
						{
							Matrix TapMatrix = GetMatrixPosition(TempH);

							if((fabsf(iter->TapOffsetMatrix[x].posit.x - (TapMatrix.posit.x - HostMatrix.posit.x)) < 0.01) && (fabsf(iter->TapOffsetMatrix[x].posit.y - (TapMatrix.posit.y - HostMatrix.posit.y)) < 0.01) && (fabsf(iter->TapOffsetMatrix[x].posit.z - (TapMatrix.posit.z - HostMatrix.posit.z)) < 0.01))
							{
								SetTap(iter->BuildingObject, x, TempH);
								iter->BuildingTap[x] = TempH; // Set this too.
								if(Logging)
									FormatLogMessage("New Building Found, Position: %f, %f, %f", TapMatrix.posit.x, TapMatrix.posit.y, TapMatrix.posit.z);
								break; // We found one, abort search.
							}
						}
					}
				}
			}
		}

		if(IsGunTower(iter->BuildingObject))
		{
			if((iter->GunTowerOff) && (iter->IsPowered))
			{
				iter->GunTowerOff = false;
				SetMaxAmmo(iter->BuildingObject, 9001); // It's over 9000!
				SetIndependence(iter->BuildingObject, 1); // restore AI.
			}
			if((!iter->GunTowerOff) && (!iter->IsPowered))
			{
				iter->GunTowerOff = true;
				SetIndependence(iter->BuildingObject, 0); // Shutdown AI.
				Stop(iter->BuildingObject, 1);
				SetMaxAmmo(iter->BuildingObject, 0);
			}

			// An attempt to make the guntower not move while unpowered. It does funny things...
		//	if((iter->GunTowerOff) && (GetDistance3D(iter->BuildingObject, GetNearestEnemy(iter->BuildingObject)) < GetEngageRange(iter->BuildingObject)))
		//		LookAt(iter->BuildingObject, iter->BuildingObject, 1);

		}

		// Powerup Repair code. BZ2 without bullet physics doesn't normally allow buildings to accept powerups unless canCollide = 1. So, here we emulate it via DLL. :)
		if((!iter->CanCollide) && (GetCurHealth(iter->BuildingObject) < GetMaxHealth(iter->BuildingObject)))
		{
			for (std::vector<RepairPowerupClass>::iterator iterB = RepairPowerupList.begin(); iterB != RepairPowerupList.end(); ++iterB)
			{
				if(iterB->DelayTime <= 0)
				{
					bool PowerupArrived = false;
					Vector PowerupPosition = GetPosition(iterB->RepairPowerupObject);
					// Old code, can't get velocity, so we have to look for it to be resting on the surface.
					if(BZ1Helper::m_GameVersion <= v13PB65)
					{
						float TargetHeight = TerrainFindFloor(iter->Position.x, iter->Position.z);
						if(close_enough(PowerupPosition.y, TargetHeight, 2.25f))
							PowerupArrived = true;
					}
					else // New code, if velocity is < 0.01.
					{
						if(GetVelocity(iterB->RepairPowerupObject).y < 0.01f)
							PowerupArrived = true;
					}

					if((PowerupArrived) && (GetDistance3DSquared(iter->Position, PowerupPosition) < Squared((iter->CollisionRadius))))
					{
						StartAudio3D(iterB->SoundPickup, iter->BuildingObject, AUDIO_CAT_WEAPON);
						AddHealth(iter->BuildingObject, iterB->HealthUp);
						RemoveObject(iterB->RepairPowerupObject);
					}
				}
			}
		}
	}
}

bool BuildingHelper::IsBuildingPowered(const Handle h)
{
	if(!IsAround(h))
		return false;

	//int Team = GetTeamNum(h);
	for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
	{
		if(iter->BuildingObject == h)
			return iter->IsPowered;
	}
	return false;
}

void BuildingHelper::SetBuildingPowered(const Handle h, const int PoweredState)
{
	if(!IsAround(h))
		return;

	for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
	{
		if(iter->BuildingObject == h)
		{
			iter->PowerState = PoweredState;
			return;
		}
	}
}