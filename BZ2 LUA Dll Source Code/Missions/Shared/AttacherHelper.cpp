#include "BZ1Helper.h"
#include "AttacherHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

AttacherHelper::AttacherHelper()
{
}

bool AttacherHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		AttacherList.resize(size);
		if(size)
			Read(&AttacherList.front(), sizeof(AttacherClass)*size);
	}
	return true;
}

bool AttacherHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<AttacherClass>::iterator iter = AttacherList.begin(); iter != AttacherList.end(); ++iter)
		{
			AttacherClass &sao = *iter;
			ConvertHandles(&sao.AttacherObject, 1);
			for(int x = 0;x<MAX_TAPS;x++)
				ConvertHandles(&sao.TapObject[x], 1);
		}
	}
	return true;
}

bool AttacherHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(AttacherList.size());
		Write(&size, 1);
		if(size)
			Write(&AttacherList.front(), sizeof(AttacherClass)*size);
	}
	return true;
}


void AttacherHelper::AddAttacherObject(const Handle h, const char* TempODF1, const char* TempODF2, const int BZCClassHash)
{
	for (std::vector<AttacherClass>::iterator iter = AttacherList.begin(); iter != AttacherList.end(); ++iter)
	{
		if(iter->AttacherObject == h)
			return;
	}

	char TempAttacherODF[MAX_ODF_LENGTH] = {0};
	int NumberOfTaps = 0;

	// If it has a first entry, continue, else don't bother.
	for(int i = 0; i < MAX_TAPS; i++)
	{
		char DesiredValue[MAX_ODF_LENGTH] = {0};
		sprintf_s(DesiredValue, "AttachedObject%d", i+1);
		if((GetODFString(TempODF1, TempODF2, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, TempAttacherODF)) && (TempAttacherODF[0])) // Grab the value.
			NumberOfTaps++;
		else
			break; // We have a gap, or dont have any attached objects, abort here.

		// If this object is trying to attach itself to itself, prevent an infinite loop.
		if(_stricmp(TempAttacherODF, GetODFName(h)) == 0)
		{
			FormatConsoleMessage("ERROR: ODF %s, %s attempted to attach itself, preventing infinite loop.", GetODFName(h), DesiredValue);
			return;
		}
	}
	if(!NumberOfTaps) // No taps, exit early.
		return;

	AttacherList.resize(AttacherList.size()+1);
	AttacherClass &sao = AttacherList[AttacherList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(AttacherClass)); // Zero things out at start.

	// Is this unit a Cerberus?
	if(BZCClassHash == BZCCLASS_CERBERUS)
		sao.IsCerberus = true;

	// Save how many taps we have.
	sao.NumberOfTaps = NumberOfTaps;

	// Attacher Code added because I got tired of waiting for N1's version... -GBD
	for(int i = 0; i < NumberOfTaps; i++)
	{
		char DesiredValue[MAX_ODF_LENGTH] = {0};

		// Grab independance settings.
		sprintf_s(DesiredValue, "AttachedIndependent%d", i+1);
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.TapIndependance[i], true);

		// Grab targeting independance settings.
		sprintf_s(DesiredValue, "AttachedCoordinateFire%d", i+1);
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.TapCoordinateFire[i], false);

		// Grab deploy behavior setting. // -1 = always on, 0 = only exist while undeployed, 1 = only exist while deployed
		sprintf_s(DesiredValue, "AttachedDeployState%d", i+1);
		GetODFInt(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.TapDeployBehavior[i], -1);
		if((sao.TapDeployBehavior[i] < -1) || (sao.TapDeployBehavior[i] > 1))
			sao.TapDeployBehavior[i] = -1; // Fallback to default.

		// Is this tap in PDS mode?
		sprintf_s(DesiredValue, "AttachedPDSMode%d", i+1);
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.PDSMode[i], false);

		// Weapon Symmetry Mask?
		//sprintf_s(DesiredValue, "AttachedWeaponSymmetryMask%d", i+1);
		//GetODFInt(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.TapWeaponSymmetryMask[i], 0);
		sprintf_s(DesiredValue, "AttachedWeaponSymmetryMask%d", i+1);
		GetODFString(TempODF1, TempODF2, "GameObjectClass", DesiredValue, MAX_HARDPOINTS+1, sao.TapWeaponSymmetryMask[i], "00000");

		// Grab XYZ Offset.
		sprintf_s(DesiredValue, "AttachedOffset%d", i+1);
		GetODFVector(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.TapOffset[i]);
	//	FormatLogMessage("Added Tap %d, Number of Taps: %d Offset: %f %f %f", i, sao.NumberOfTaps, sao.TapOffset[i].x, sao.TapOffset[i].y, sao.TapOffset[i].z);

		sprintf_s(DesiredValue, "AttachedRotation%d", i+1);
		GetODFVector(TempODF1, TempODF2, "GameObjectClass", DesiredValue, &sao.TapRotation[i]);
		sao.TapRotation[i] = Vector_Scale(sao.TapRotation[i], DEG_2_RAD);

		//FormatLogMessage("Added Tap Rotation: %f %f %f", sao.TapRotation[i].x, sao.TapRotation[i].y, sao.TapRotation[i].z);

		strcat_s(TempAttacherODF, ".odf"); // Add .odf to the end.
		if(OpenODF2(TempAttacherODF))
		{
			char TempAttacherODF2[MAX_ODF_LENGTH] = {0};
			// If this ODF is inherited, then try that ODF.
			if(GetODFString(TempAttacherODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempAttacherODF2)) // Grab the classlabel.
			{
				strcat_s(TempAttacherODF2, ".odf"); // Add .odf to the end.
				OpenODF2(TempAttacherODF2);
			}

			// Get it's engage range.
			if((GetODFFloat(TempAttacherODF, TempAttacherODF2, "CraftClass", "EngageRange", &sao.TapEngageRange[i], 0.0f)) || 
				(GetODFFloat(TempAttacherODF, TempAttacherODF2, "CraftClass", "RangeScan", &sao.TapEngageRange[i], 0.0f))) // Must be 0, for False shooty stuff.
			{
				// Got it.
			}
		}
	} // Loop over Taps.

	sao.AttacherObject = h; // Save it.
}

void AttacherHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// Sweep through the Shield Towers and remove any of them that are no longer around
	std::vector<AttacherClass>::iterator validEnd = AttacherList.begin();
	std::vector<AttacherClass>::iterator lastValid = AttacherList.begin();
	for (std::vector<AttacherClass>::iterator iter = AttacherList.begin(); iter != AttacherList.end(); ++iter)
	{
		// If the tower is around...
		if (IsAround(iter->AttacherObject))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			for(int x = 0; x < iter->NumberOfTaps; x++)
				if(IsAround(iter->TapObject[x]))
					RemoveObject(iter->TapObject[x]);
		}
	}
	// Now erase any invalid entries left behind
	AttacherList.erase(validEnd, AttacherList.end());

	for (std::vector<AttacherClass>::iterator iter = AttacherList.begin(); iter != AttacherList.end(); ++iter)
	{
		Vector HostVelocity = GetVelocity(iter->AttacherObject);
		Handle HostTarget = GetTarget(iter->AttacherObject);
		int Team = GetTeamNum(iter->AttacherObject);
		int TargetTeam = GetTeamNum(HostTarget);
		Handle TempTapTarget[MAX_TAPS] = {0};

		// Loop over each of our valid taps for this object.
		for(int x = 0; x < iter->NumberOfTaps; x++)
		{
			// If this tap hasn't been built yet, do it here. Originally was done in AddObject but was suggested to move it to Execute.
			if((!iter->BuiltAttacherTap[x]) && ((iter->TapDeployBehavior[x] < 0) || (((iter->TapDeployBehavior[x]) && (IsDeployed(iter->AttacherObject))) || ((!iter->TapDeployBehavior[x]) && (!IsDeployed(iter->AttacherObject))))))
			{
				char TempAttacherODF[MAX_ODF_LENGTH] = {0}; // Temp store our variable.
				char DesiredValue[MAX_ODF_LENGTH] = {0};
				sprintf_s(DesiredValue, "AttachedObject%d", x+1);

				if(GetODFString(iter->AttacherObject, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, TempAttacherODF))
				{
					if(TempAttacherODF[0])
					{
						// Add it if so. 
						if(DoesODFExist(TempAttacherODF))
						{
							iter->TapPosition[x] = Matrix_Multiply(Build_Position_Rotation_Matrix(iter->TapRotation[x].x, iter->TapRotation[x].y, iter->TapRotation[x].z, iter->TapOffset[x]), GetMatrixPosition(iter->AttacherObject));
							iter->TapObject[x] = BuildObject(TempAttacherODF, Team, iter->TapPosition[x]);
							//SetVectorPosition(iter->TapObject[x], TapMatrix.posit);
							//if(!iter->TapIndependance[x])
							SetIndependence(iter->TapObject[x], 0);

							char BZCClass[MAX_ODF_LENGTH] = {0};
							GetODFString(iter->TapObject[x], "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);
							unsigned int BZCClassHash = Hash(BZCClass);

							// Is this unit a Cerberus Tail?
							if(BZCClassHash == BZCCLASS_CERBERUS_TAIL)
							{
								iter->IsCerberusTail[x] = true;
								SetIndependence(iter->TapObject[x], 0);

								GetODFFloat(iter->TapObject[x], "GameObjectClass", "CerbTailFireAngle", &iter->CerbTailConeAngle[x], 0.60f);

								bool HasAnim1 = false;
								bool HasAnim2 = false;
								bool HasAnim3 = false;
								char AnimationName[MAX_ODF_LENGTH] = {0};
								int AnimationCount = 0;
								GetODFInt(iter->TapObject[x], "GameObjectClass", "animCount", &AnimationCount);
								// Look for new ODF Parameters. 
								for (int i = 1; i < AnimationCount+1; i++) // Look through 16 animnames. If you made a unit that has more then 16 anims, 1: Congratulations, and 2: The loop and off ones better be between 1 and 16.
								{
									char DesiredValue[MAX_ODF_LENGTH] = {0};
									sprintf_s(DesiredValue, "animName%d", i);
									if(GetODFString(iter->TapObject[x], "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, AnimationName)) // Grab the animation name.
									{
										if((_stricmp(AnimationName,"CerbTailIdle")) == 0)
											HasAnim1 = true;
										else if((_stricmp(AnimationName,"CerbTailLeft")) == 0)
											HasAnim2 = true;
										else if((_stricmp(AnimationName,"CerbTailRight")) == 0)
											HasAnim3 = true;

										if((HasAnim1) && (HasAnim2) && (HasAnim3))
										{
											iter->IsCerberusTailAnimated[x] = true;
											break; // We found it, abort!
										}
									}
								}

								if(iter->IsCerberusTailAnimated[x])
								{
									iter->CerbAnimLength[x] = SetAnimation(iter->TapObject[x], "CerbTailIdle", 0); // Restart the animation. 
									StartAnimation(iter->TapObject[x]);

									if(Logging)
										FormatLogMessage("Playing Tail Loop Anim");
								}
							}
						}
						else
						{
							FormatLogMessage("ERROR: Attacher ODF '%s' Doesn't Exist!", TempAttacherODF); 
						}
					}
					iter->BuiltAttacherTap[x] = true;
				}
			}

			// If the attached object is around.
			if(IsAround(iter->TapObject[x]))
			{
				// If this tap is only supposed to be around in a certain Deploy state, remove it if it doesn't match.
				if( (iter->TapDeployBehavior[x] >= 0) && // Deploy State not -1 (no special behavior)
					((iter->TapDeployBehavior[x] != 0) == IsDeployed(iter->AttacherObject))) // Deployed state doesn't match. // Code from Ken. :) Alternate: if(!!iter->TapDeployBehavior[x] == IsDeployed(iter->AtacherObject))) [8:47:50 PM] Kenneth Miller: the first ! converts the integer into a bool, and the second negates it.
				//	(((iter->TapDeployBehavior[x]) && (IsDeployed(iter->AttacherObject))) || // Only exist while Deployed.
				//	((!iter->TapDeployBehavior[x]) && (!IsDeployed(iter->AttacherObject))))) // Only exist while Undeployed.
				{
					RemoveObject(iter->TapObject[x]);
					// Zer this tap's settings out.
					iter->IsHostDeployed[x] = 0;
					iter->TapIsAttacking[x] = 0;
					iter->TapShutOff[x] = 0;
					continue; // Skip this one. 
				}

				Matrix TapMatrix = Matrix_Multiply(Build_Position_Rotation_Matrix(iter->TapRotation[x], iter->TapOffset[x]), GetMatrixPosition(iter->AttacherObject));

				if((BZ1Helper::m_GameVersion >= v13PB7) && ((!IsVehicle(iter->TapObject[x])) && (!IsPerson(iter->TapObject[x]))))
					SetInterpolablePosition(iter->TapObject[x], &iter->TapPosition[x], &TapMatrix);
				else
					SetLastCurrentPosition(iter->TapObject[x], iter->TapPosition[x], TapMatrix);

				//SetPosition(iter->TapObject[x], iter->TapPosition[x]);
				//SetVectorPosition(iter->TapObject[x], iter->TapPosition[x].posit);
				SetVelocity(iter->TapObject[x], HostVelocity);
				iter->TapPosition[x] = TapMatrix; // Save this for next tick.


				// Weapon Symmetry. Compare weapons from Host/Taps, and if this tap should mirror that weapon, copy it over if it's different.
				bool ShouldSwapWeapons = false;
				int CurIndex = 0;
				char NewTapWeapons[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
				float PrevLocalAmmo[MAX_HARDPOINTS] = {0};
				for(int i = 0; i < MAX_HARDPOINTS; i++)
				{
					if(iter->TapWeaponSymmetryMask[x][4-i])
					{
						bool WeaponMask = (iter->TapWeaponSymmetryMask[x][4-i] != '0'); // A function to GetWeaponMask(h) would be nice. 
						if(WeaponMask)
						{
							char HostWeapon[MAX_ODF_LENGTH] = {0};
							char TapWeapon[MAX_ODF_LENGTH] = {0};
							if((GetObjInfo(iter->AttacherObject, ObjectInfoType(Get_Weapon0Config+i), HostWeapon)) && 
								(GetObjInfo(iter->TapObject[x], ObjectInfoType(Get_Weapon0Config+i), TapWeapon)))
							{
								PrevLocalAmmo[i] = GetCurLocalAmmo(iter->AttacherObject, i);
								if((HostWeapon) && (TapWeapon) && (_stricmp(HostWeapon, TapWeapon) != 0)) // They don't match.
								{
									ShouldSwapWeapons = true;

									// Build a new weapon list to use with ReplaceWeapons.
									strcpy_s(NewTapWeapons[CurIndex++], HostWeapon);
								}
							}
						}
					}
				}
				if(ShouldSwapWeapons)
					ReplaceWeapons(iter->TapObject[x], NewTapWeapons, PrevLocalAmmo);


				if(!TempTapTarget[x])
					TempTapTarget[x] = GetTarget(iter->TapObject[x]);
			//	if(!TempTapTarget[x])
			//		TempTapTarget[x] = GetCurrentWho(iter->TapObject[x]);

				// If this tap is a shooty thing, do shooty stuff, and is NOT a cerberus tail.
				if(!iter->IsCerberusTail[x])
				{
					if(iter->TapEngageRange[x])
					{
						// If tap is not independant, focus fire on host Target, stop when not around.
						if( (IsAround(HostTarget)) && // Target is around.
							(!IsTeamAllied(Team, TargetTeam)) && // It's an Enemy!
							((!iter->PDSMode[x]) || ((IsPerson(HostTarget)) || (IsWeapon(HostTarget))))) // PDS Mode settings.
						{
							Attack(iter->TapObject[x], HostTarget, 1);
						//	SetTarget(AttachedObject[Team][i][x],HostTarget);
							iter->TapIsAttacking[x] = true;

							if(Logging)
								FormatLogMessage("Tap %x Attacking Host Target: %08x", x, HostTarget);
						}
						else // Find our own Target.
						{
							if(iter->TapIsAttacking[x]) // We were shooting at the Host's target, but they dis-engaged, Stop firing at it.
							{
								SetCommand(iter->TapObject[x], CMD_STOP, 1);
							//	SetTarget(AttachedObject[Team][i][x],0);
								iter->TapIsAttacking[x] = false;

								if(Logging)
									FormatLogMessage("Tap %x Switching to own AI control", x);
							}

							// If there's no Target, choose your own target.
							if(iter->TapIndependance[x])
							{
								// If we don't already have our own Target, Pick a new one.
								if(!TempTapTarget[x])
								{
									if(iter->PDSMode[x])
										TempTapTarget[x] = BZ1Helper::Get().GetNearestObject(TapMatrix.posit, iter->TapEngageRange[x], Team, Enemies, Weapon+Person);
									else
										TempTapTarget[x] = BZ1Helper::Get().GetNearestObject(TapMatrix.posit, iter->TapEngageRange[x], Team, Enemies, Weapon+Person+Building+Vehicle);

									if(TempTapTarget[x]) // If the above found something.
									{
										Attack(iter->TapObject[x], TempTapTarget[x], 1);

										if(Logging)
											FormatLogMessage("Tap %x Set to Self Attack %08x", x, TempTapTarget[x]);
									}
								}
							}

						}

						// If coordinated fire is false, make each tap try to pick its own target, unless the host is Targeting something.
						if((!iter->TapCoordinateFire[x]) && ((!IsAround(HostTarget)) || (IsTeamAllied(Team, TargetTeam)))) 
						{
							bool PickNewTarget = false;

							// If we already have a Target, find out if other Taps are also shooting at it.
							if(TempTapTarget[x])
							{
								for(int d = 0; d < iter->NumberOfTaps; d++)
								{
									if((d != x) && (TempTapTarget[x] == TempTapTarget[d]) && (iter->TapObject[d]) && (TempTapTarget[d]))
									{
										if(Logging)
											FormatLogMessage("Tap %d Target: %08x, picking new target...", d, TempTapTarget[d]);

										PickNewTarget = true; // This is already being shot at by another Tap. Retarget.
										break;
									}
								}

								// loop over all possible targets.
								if(PickNewTarget)
								{
									for(int t = 0; t < BZ1Helper::Get().GetNumHandles(); t++)
									{
										bool Skip = false;

										BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(t);
										Handle NewTarget = objectInfo.ObjectHandle;

										if((IsTeamAllied(Team, objectInfo.CurTeamNum)) || // If this team is allied.
											(GetMaxHealth(NewTarget) <= 0) || // If it's health is 0.
											(GetDistance3DSquared(TapMatrix.posit, objectInfo.Position) > Squared(iter->TapEngageRange[x])) || // If it's outside of range.
											((iter->PDSMode[x]) && ((objectInfo.CategoryID != Person) && (objectInfo.CategoryID != Weapon)))) // If PDSMode and not a weapon or person.
										{
											if(Logging)
												FormatLogMessage("Tap: %d PickTarget Skipping Target: %08x", x, NewTarget);

											continue; // Skip this object.
										}

										for(int d = 0; d < iter->NumberOfTaps; d++)
										{
											if((IsAround(iter->TapObject[d])) && (TempTapTarget[d]) && (NewTarget == TempTapTarget[d]))
											{
												if(Logging)
													FormatLogMessage("Tap: %d Target %08x matches existing Target, Tap %d Target: %08x", x, NewTarget, d, TempTapTarget[d]);

												Skip = true;
												break;
											}
										}
										if(Skip)
											continue; // Skip to next item in the list.

										char ObjClass[MAX_ODF_LENGTH] = {0};
										GetObjInfo(iter->TapObject[x], Get_GOClass, ObjClass);

										// This object is within range, and is facing close enough to us, and it's health is > 0.
										if((_stricmp(ObjClass, "CLASS_WALKER") != 0) || (IsObjectFacingObject(iter->TapObject[x], NewTarget, 3.141592f))) // Object is facing close to us. ~180 degrees
										{
											TempTapTarget[x] = NewTarget;
											Attack(iter->TapObject[x], TempTapTarget[x], 1);

											if(Logging)
												FormatLogMessage("Tap %d Targeting New Target: %08x", x, NewTarget);

											break;
										}
									}
								}
							}
						}

						// Shutoff Tap fire if the host is empty. Ken Code (TM)
						bool tapShutOff = !IsAliveAndPilot2(iter->AttacherObject) && !IsPlayer(iter->AttacherObject);
						if(tapShutOff != iter->TapShutOff[x])
						{
							if(tapShutOff)
								SetCommand(iter->TapObject[x], CMD_STOP, 1);
							iter->TapShutOff[x] = tapShutOff;
						}
						/*
						if((!iter->TapShutOff[x]) && (!IsAliveAndPilot2(iter->AttacherObject)) && (!IsPlayer(iter->AttacherObject)))
						{
							SetCommand(iter->TapObject[x], CMD_STOP, 1);
						//	SetIndependence(iter->TapObject[x], 0);
							iter->TapShutOff[x] = true;
						}
						else if(iter->TapShutOff[x])
						{
						//	if(iter->TapIndependance[x])
						//		SetIndependence(iter->TapObject[x], 1);

							iter->TapShutOff[x] = false;
						}
						*/
					} // This is something that can shoot
				} // end NOT a Cerberus Tail.
				else // Do Cerberus Tail stuff here.
				{
					Handle CerbTailTarget = 0;

					for(int t = 0; t < BZ1Helper::Get().GetNumHandles(); t++)
					{
						BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(t);
						Handle TempTarget = objectInfo.ObjectHandle;

						if( (objectInfo.CategoryID == Vehicle) && // It's a Vehicle.
							(!IsTeamAllied(Team, objectInfo.CurTeamNum)) && // It's an Enemy!
							(GetDistance3DSquared(TapMatrix.posit, objectInfo.Position) < Squared(iter->TapEngageRange[x])) && // Distance is within Range.
							(IsObjectFacingObject(iter->TapObject[x], TempTarget, iter->CerbTailConeAngle[x]))) // It's facing the correct direction.
						{
							CerbTailTarget = TempTarget;

							// The tail isn't currently swinging, and the target is within range, and towards the front of the Tail tap.
							float AnimationFrame = 0;
							switch (iter->CerbTailSwinging[x])
							{
							case 0:
								AnimationFrame = GetAnimationFrame(iter->TapObject[x], "CerbTailIdle");
								break;
							case 1:
								AnimationFrame = GetAnimationFrame(iter->TapObject[x], "CerbTailLeft");
								break;
							case 2:
								AnimationFrame = GetAnimationFrame(iter->TapObject[x], "CerbTailRight");
								break;
							}

							if(AnimationFrame == iter->CerbAnimLength[x])
							{
								// Is it to the right or left?
								if(IsObjectFacingObjectRight(iter->TapObject[x], TempTarget))
								{
									iter->CerbTailSwinging[x] = 2; // Right
									iter->CerbAnimLength[x] = SetAnimation(iter->TapObject[x], "CerbTailRight", 0); // Restart the animation. 
								}
								else
								{
									iter->CerbTailSwinging[x] = 1; // Left
									iter->CerbAnimLength[x] = SetAnimation(iter->TapObject[x], "CerbTailLeft", 0); // Restart the animation. 
								}

								StartAnimation(iter->TapObject[x]);
								//FireAt(iter->TapObject[x], TempTarget, true);
								Attack(iter->TapObject[x], TempTarget, 1);

								if(Logging)
									FormatLogMessage("Playing Tail Swing Anim");
							}
							break; // We found a target, stop looking.
						}
					}
					if((iter->CerbTailSwinging[x]) && (!CerbTailTarget))
					{
						// Wait till anim is done, then resume idle looping animation.
						float AnimationFrame = 0;
						switch (iter->CerbTailSwinging[x])
						{
						case 1:
							AnimationFrame = GetAnimationFrame(iter->TapObject[x], "CerbTailLeft");
							break;
						case 2:
							AnimationFrame = GetAnimationFrame(iter->TapObject[x], "CerbTailRight");
							break;
						}

						if(AnimationFrame == iter->CerbAnimLength[x])
						{
							SetCommand(iter->TapObject[x], CMD_STOP, 1); // Stop shooting now, anims done.
							iter->CerbTailSwinging[x] = 0; // Nope.
							iter->CerbAnimLength[x] = SetAnimation(iter->TapObject[x], "CerbTailIdle", 0); // Restart the animation. 
							StartAnimation(iter->TapObject[x]);

							if(Logging)
								FormatLogMessage("Playing Tail Idle Anim");
						}
					}
				}

				// Play Deploy animations for taps when this host deploys.
				if((!iter->IsHostDeployed[x]) && (IsDeployed(iter->AttacherObject))) // Only proceed if the object is still around and working.
				{
					iter->IsHostDeployed[x] = true;
					SetAnimation(iter->TapObject[x], "deploy", 1); // Restart the animation. 
					StartAnimation(iter->TapObject[x]);
					Deploy(iter->TapObject[x]);
				}
				else if((iter->IsHostDeployed[x]) && (!IsDeployed(iter->AttacherObject)))
				{
					iter->IsHostDeployed[x] = false;
					SetAnimation(iter->TapObject[x], "undeploy", 1); // Restart the animation. 
					StartAnimation(iter->TapObject[x]);
					Deploy(iter->TapObject[x]);
				}

			}
		}
	}
}
