#include "BZ1Helper.h"
#include "ShieldHelper.h"
#include "BZCScriptUtils.h"
#include "BuildingHelper.h"
#include "math.h"
#include <algorithm>

ShieldHelper::ShieldHelper()
{
}

bool ShieldHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		ShieldList.resize(size);
		if(size)
			Read(&ShieldList.front(), sizeof(ShieldTowerClass)*size);
	}
	return true;
}

bool ShieldHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<ShieldTowerClass>::iterator iter = ShieldList.begin(); iter != ShieldList.end(); ++iter)
		{
			ShieldTowerClass &sao = *iter;
			ConvertHandles(&sao.ShieldTowerObject, 1);

			for (int i = 0; i < MAX_TAPS; ++i)
			{
				ShieldWallClass &wall = sao.Walls[i];
				ConvertHandles(wall.Effects, wall.Count);
				ConvertHandles(wall.Fields, wall.Count);
			}
		}
	}
	return true;
}

bool ShieldHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(ShieldList.size());
		Write(&size, 1);
		if(size)
			Write(&ShieldList.front(), sizeof(ShieldTowerClass)*size);
	}
	return true;
}

void ShieldHelper::AddShieldTowerObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<ShieldTowerClass>::iterator iter = ShieldList.begin(); iter != ShieldList.end(); ++iter)
	{
		if(iter->ShieldTowerObject == h)
			return;
	}

	ShieldList.resize(ShieldList.size()+1);
	ShieldTowerClass &sao = ShieldList[ShieldList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(ShieldTowerClass)); // Zero things out at start.

	GetODFFloat(TempODF1, TempODF2, "ShieldTowerClass", "MatchRange", &sao.MaxRange, 520.0f);
	GetODFFloat(TempODF1, TempODF2, "ShieldTowerClass", "MatchConeAngle", &sao.MatchConeAngle, 0.15f);
	sao.MatchConeAngle = clamp(sao.MatchConeAngle, 0.0f, 1.0f);
	GetODFInt(TempODF1, TempODF2, "ShieldTowerClass", "MatchTeamFilter", &sao.TeamFilter, 3);
	// Make 0 all teams. 
	if(sao.TeamFilter == 0)
		sao.TeamFilter = 7;

	char TempODF3[MAX_ODF_LENGTH] = {0}; // First temp odf, don't wanna modify our stored ODFName.
	char TempODF4[MAX_ODF_LENGTH] = {0}; // Second temp odf, for grabbing out classlabel.

	char DesiredValue[MAX_ODF_LENGTH] = {0};
	char DesiredValue2[MAX_ODF_LENGTH] = {0};
	for(int x = 0;x<MAX_TAPS;x++)
	{
		Vector TempFront(0, 0, 0);
		sprintf_s(DesiredValue, "ShieldFront%d", x+1);

		if(GetODFVector(TempODF1, TempODF2, "ShieldTowerClass", DesiredValue, &TempFront))
			++sao.EmitterCount;
		else
			break; // Abort, we reached the end of emitters.

		sao.Walls[x].Rotation = Vector_Scale(TempFront, DEG_2_RAD);

		sprintf_s(DesiredValue, "EmitterODF%d_%d", x+1, BZ1Helper::m_GameTPS);
		sprintf_s(DesiredValue2, "EmitterODF_%d", BZ1Helper::m_GameTPS);
		if((GetODFString(TempODF1, TempODF2, "ShieldTowerClass", DesiredValue, MAX_ODF_LENGTH, sao.Walls[x].ShieldEmitterODF)) || 
			(GetODFString(TempODF1, TempODF2, "ShieldTowerClass", DesiredValue2, MAX_ODF_LENGTH, sao.Walls[x].ShieldEmitterODF)))
		{
			// Got it.
		}

		sprintf_s(DesiredValue, "EffectODF%d", x+1);
		if((GetODFString(TempODF1, TempODF2, "ShieldTowerClass", DesiredValue, MAX_ODF_LENGTH, sao.Walls[x].ShieldEffectODF)) || 
			(GetODFString(TempODF1, TempODF2, "ShieldTowerClass", "EffectODF", MAX_ODF_LENGTH, sao.Walls[x].ShieldEffectODF)))
		{
			// Got it.
		}

		sprintf_s(TempODF3, "%s.odf", sao.Walls[x].ShieldEmitterODF); // Add .odf to the end.
		if((OpenODF2(TempODF3)) && (GetODFString(TempODF3, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF4))) // If this ODF is inherited, then try that ODF.
		{
			strcat_s(TempODF4, ".odf"); // Add .odf to the end.
			OpenODF2(TempODF4);
		}

		if(sao.Walls[x].ShieldEmitterODF[0])
		{
			if(DoesODFExist(sao.Walls[x].ShieldEmitterODF))
			{
				// ShieldEmitterODF object is not a shieldtower class! ABORT!
				if(_stricmp(TempODF4, "shieldtower.odf") == 0)
				{
					float TempDistMin = 0.0f;
					float TempDistMax = 0.0f;
					GetODFFloat(TempODF3, TempODF4, "ShieldTowerClass", "shieldMinZ", &TempDistMin, 0.0f);
					GetODFFloat(TempODF3, TempODF4, "ShieldTowerClass", "shieldMaxZ", &TempDistMax, 0.0f);
					sao.Walls[x].Width = TempDistMax - TempDistMin;

					GetODFFloat(TempODF3, TempODF4, "ShieldTowerClass", "shieldMinX", &TempDistMin, 0.0f);
					GetODFFloat(TempODF3, TempODF4, "ShieldTowerClass", "shieldMaxX", &TempDistMax, 0.0f);
					sao.Walls[x].Interval = TempDistMax - TempDistMin;

				//	FormatLogMessage("Added Shield Object: Interval %d: %f, Width: %f", x, ShieldTowerEmitterInterval[Team][m_NumShieldTowerObjects[Team]][x], ShieldEmitterWidth[Team][m_NumShieldTowerObjects[Team]][x]);
				}
				else
				{
					FormatLogMessage("ERROR: Shield Tower EmitterODF%d '%s' is not a shieldtower classlabel!", x+1, sao.Walls[x].ShieldEmitterODF);
					break; // Abort, we hit a speed bump...
				}
			}
			else
			{
				FormatLogMessage("ERROR: Shield Tower EmitterODF%d '%s' Doesn't exist! Fix ODFS!", x+1, sao.Walls[x].ShieldEmitterODF);
			}
		}

		if(DoesODFExist(sao.Walls[x].ShieldEmitterODF))
			PreloadODF(sao.Walls[x].ShieldEmitterODF);
		char TempPreloadODF[MAX_ODF_LENGTH] = {0};
		sprintf_s(TempPreloadODF, "%sa", sao.Walls[x].ShieldEffectODF);
		if(DoesODFExist(TempPreloadODF))
			PreloadODF(TempPreloadODF);
		sprintf_s(TempPreloadODF, "%sb", sao.Walls[x].ShieldEffectODF);
		if(DoesODFExist(TempPreloadODF))
			PreloadODF(TempPreloadODF);
	}

	// If we're not setup correctly, let the handle remain 0. List cleanup will delete our other data.
	sao.ShieldTowerObject = h;
}

void ShieldHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// Sweep through the Shield Towers and remove any of them that are no longer around
	std::vector<ShieldTowerClass>::iterator validEnd = ShieldList.begin();
	std::vector<ShieldTowerClass>::iterator lastValid = ShieldList.begin();
	for (std::vector<ShieldTowerClass>::iterator iter = ShieldList.begin(); iter != ShieldList.end(); ++iter)
	{
		// If the tower is around...
		if (IsAround(iter->ShieldTowerObject))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			// Remove all of this Shield Tower's Shield emitters.
			for(int t = 0; t < MAX_TAPS;t++)
			{
				ShieldWallClass &wall = iter->Walls[t];
				for(int x = 0; x < wall.Count; x++)
				{
					if(Logging)
						FormatLogMessage("Removing Dead Shield Index: %d, Handle: %08x, Emitters: %d, Piece: %d", iter - ShieldList.begin(), iter->ShieldTowerObject, t, x);

					if(IsAround(wall.Fields[x]))
						RemoveObject(wall.Fields[x]);
					if(IsAround(wall.Effects[x]))
						RemoveObject(wall.Effects[x]);
				}
			}
		}
	}
	// Now erase any invalid entries left behind
	ShieldList.erase(validEnd, ShieldList.end());

	// Loop through all Shields, doing Shield stuff. :)
	for (std::vector<ShieldTowerClass>::iterator iter = ShieldList.begin(); iter != ShieldList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->ShieldTowerObject);

		for(int EmitterA = 0; EmitterA < iter->EmitterCount; EmitterA++)
		{
			// This tower is not active. Watch for a matching tower set.
			if(!iter->Walls[EmitterA].IsActive)
			{
				// Only proceed if it has power, and Interval is > 0.
				if((BuildingHelper::Get().IsBuildingPowered(iter->ShieldTowerObject)) && (iter->Walls[EmitterA].Interval > 0))
				{
					for (std::vector<ShieldTowerClass>::iterator iterB = ShieldList.begin(); iterB != ShieldList.end(); ++iterB)
					{
						int t = GetTeamNum(iterB->ShieldTowerObject);

						// Check that it is not itself, the other end is powered, and Teamfilter matches.
						if( (TeamFilterCheck(iter->TeamFilter, t, Team)) && // Team Filter matches.
							(BuildingHelper::Get().IsBuildingPowered(iterB->ShieldTowerObject)) && // It's powered.
							(iter->ShieldTowerObject != iterB->ShieldTowerObject)) // Not itself.
						{
							// Loop over taps, find the taps on this shield object.
							for(int EmitterB = 0; EmitterB < iterB->EmitterCount; EmitterB++)
							{
								Matrix PositionA = Matrix_Multiply(Build_Position_Rotation_Matrix(iter->Walls[EmitterA].Rotation.x, iter->Walls[EmitterA].Rotation.y, iter->Walls[EmitterA].Rotation.z), GetMatrixPosition(iter->ShieldTowerObject));
								Matrix PositionB = Matrix_Multiply(Build_Position_Rotation_Matrix(iterB->Walls[EmitterB].Rotation.x, iterB->Walls[EmitterB].Rotation.y, iterB->Walls[EmitterB].Rotation.z), GetMatrixPosition(iterB->ShieldTowerObject));

								/*
								char ODFName[MAX_ODF_LENGTH] = {0};
								if(IsObjectFacingObject(PositionA, PositionB, iter->MatchConeAngle))
									FormatLogMessage("%08x Index: %d: Emitter: %d Is Facing -> %08x Index: %d: Emitter: %d", iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA, iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB);
								else
									FormatLogMessage("%08x Index: %d: Emitter: %d !NOT Facing -> %08x Index: %d: Emitter: %d", iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA, iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB);

								if(IsObjectFacingObject(PositionB, PositionA, iterB->MatchConeAngle))
									FormatLogMessage("%08x Index: %d: Emitter: %d Is Facing -> %08x Index: %d: Emitter: %d", iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB, iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA);
								else
									FormatLogMessage("%08x Index: %d: Emitter: %d !NOT Facing -> %08x Index: %d: Emitter: %d", iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB, iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA);

								Handle TempH = BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iterB->ShieldTowerObject, iter->Walls[EmitterA].Width, PositionA.front, PositionB.front);
								GetObjInfo(TempH, Get_CFG, ODFName);
								if(!TempH)
									FormatLogMessage("%08x Index: %d: Emitter: %d Is Not Blocked -> %08x Index: %d: Emitter: %d", iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA, iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB);
								else
									FormatLogMessage("%08x Index: %d: Emitter: %d IS BLOCKED -> %08x Index: %d: Emitter: %d, Blocked by: %08x %s", iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA, iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB, TempH, ODFName);

								TempH = BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iterB->ShieldTowerObject, iterB->Walls[EmitterB].Width, PositionA.front, PositionB.front);
								GetObjInfo(TempH, Get_CFG, ODFName);
								if(!TempH)
									FormatLogMessage("%08x Index: %d: Emitter: %d Is Not Blocked -> %08x Index: %d: Emitter: %d", iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA, iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB);
								else
									FormatLogMessage("%08x Index: %d: Emitter: %d IS BLOCKED -> %08x Index: %d: Emitter: %d, Blocked by: %08x %s", iter->ShieldTowerObject, iter - ShieldList.begin(), EmitterA, iterB->ShieldTowerObject, iterB - ShieldList.begin(), EmitterB, TempH, ODFName);
								FormatLogMessage("\n");
								*/

								// It's within range of matching, and that the Shield is facing the Target Shield, also check that said Target Shield is facing our direction.
								if( (IsObjectFacingObject(PositionA, PositionB, iter->MatchConeAngle)) && // A is facing B.
									(IsObjectFacingObject(PositionB, PositionA, iterB->MatchConeAngle)) && // B is facing A.
									(!BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iterB->ShieldTowerObject, iter->Walls[EmitterA].Width, PositionA.front, PositionB.front)) && // Nothing between A ... B.
									(!BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iterB->ShieldTowerObject, iterB->Walls[EmitterB].Width, PositionA.front, PositionB.front))) // Nothing between B ... A.
								{
									float DistanceSquared = GetDistance3DSquared(PositionA.posit, PositionB.posit);
									if(DistanceSquared <= Squared(iter->MaxRange)) // It's within range.
									{
										float const distScale = iter->Walls[EmitterA].Interval / GetDistance2D(PositionB.posit, PositionA.posit);
										int const EmitterCount = int(floorf(1.0f / distScale + 0.5f));

										if(Logging)
											FormatLogMessage("Distance Is: %f, EmitterCount is: %d, Segment Count is: %d", sqrtf(DistanceSquared), EmitterCount, iter->Walls[EmitterA].Count);

										if(EmitterCount < MAX_SHIELDS+1)
										{
											Vector VecUp(0.0f, 1.0f, 0.0f);
											Matrix Position = Build_Orthogonal_Matrix(VecUp, Sub_Vectors(PositionB.posit, PositionA.posit));
											Position = Matrix_Multiply(Build_Yaw_Matrix(90.0f * DEG_2_RAD), Position);
											Position.posit = PositionA.posit;

											float const dx = (PositionB.posit.x - PositionA.posit.x) * distScale;
											float const dz = (PositionB.posit.z - PositionA.posit.z) * distScale;
											float MeanHeight = ((PositionA.posit.y + PositionB.posit.y) / 2);

											int Count = iter->Walls[EmitterA].Count;
											if((Count) && (Count != EmitterCount))// Start at 1. Skip start of 2nd cycle too.
											{
												if(Count < EmitterCount)
													GetPosition(iter->Walls[EmitterA].Fields[Count-1], Position); // Position of Previous Shield
												else
													GetPosition(iter->Walls[EmitterA].Fields[Count-EmitterCount-1], Position); // Start over from beginning.

												Position.posit.x = Position.posit.x + dx;
												Position.posit.z = Position.posit.z + dz;
											}

											Position.posit.y = MeanHeight; //TerrainFindFloor(Position.posit.x,Position.posit.z);

											if(Count < 2 * EmitterCount)
											{
												if(Count < EmitterCount)
												{
													if(DoesODFExist(iter->Walls[EmitterA].ShieldEmitterODF))
														iter->Walls[EmitterA].Fields[Count] = BuildObject(iter->Walls[EmitterA].ShieldEmitterODF, Team, Position);

													if(Logging)
														FormatLogMessage("!!!--- Shield Emitter Count: %d", Count);
												}
												else
												{
													// Do some fancy EffectODF name manipulation for the end pieces. :)
													char TempShieldODF[MAX_ODF_LENGTH] = {0};
													sprintf_s(TempShieldODF, "%sa", iter->Walls[EmitterA].ShieldEffectODF);
													char TempShieldODF2[MAX_ODF_LENGTH] = {0};
													sprintf_s(TempShieldODF2, "%sb", iter->Walls[EmitterA].ShieldEffectODF);

													if((Count == EmitterCount) && (DoesODFExist(TempShieldODF))) // Build Start piece here.
														iter->Walls[EmitterA].Effects[Count-EmitterCount] = BuildObject(TempShieldODF, Team, Position);
													else if((Count == 2*EmitterCount-1) && (DoesODFExist(TempShieldODF2))) // Build End piece here.
														iter->Walls[EmitterA].Effects[Count-EmitterCount] = BuildObject(TempShieldODF2, Team, Position);
													else if(DoesODFExist(iter->Walls[EmitterA].ShieldEffectODF)) // Build rest of shield here. Use this if no ends exist.
														iter->Walls[EmitterA].Effects[Count-EmitterCount] = BuildObject(iter->Walls[EmitterA].ShieldEffectODF, Team, Position);

													if(Logging)
														FormatLogMessage("Placing Shield Index: %d -> %d, Handle: %08x -> %08x, Emitter: %d, Piece %d: %f %f %f", iter - ShieldList.begin(), iterB - ShieldList.begin(), iter->ShieldTowerObject, iterB->ShieldTowerObject, EmitterA, Count-EmitterCount, Position.posit.x, Position.posit.y, Position.posit.z);
												}

												iter->Walls[EmitterA].Count++; // Increase the count of shields for this wall.
											}
											else // We've placed all of our emitters.
											{
												iter->Walls[EmitterA].IsActive = true; // Flag as active.
												iter->Walls[EmitterA].Target = iterB->ShieldTowerObject;
												iter->Walls[EmitterA].TargetFront = PositionB.front;

												iter->Walls[EmitterA].TargetWidth = iterB->Walls[EmitterB].Width;

												if(Logging)
													FormatLogMessage("Shield Index: %d, Handle: %08x Active!", iter - ShieldList.begin(), iter->ShieldTowerObject);
											}
										} // EmitterCount < MAX_SHIELDS
										break; // We found one within range and alignment.
									} // Its within range.
								} // Its not blocked by a building.
							} // End loop over ShieldB's Taps.
						} // End TeamFilter matches and ShieldB is Powered.
					} // End loop over t's Shield Objects.				
				} // Building is Powered amd Interval > 0.
			}
			else // It's currently active. Watch for it to lose it's end, or power to go out.
			{
				// Re-Calc Front vector?
				Matrix PositionA = Matrix_Multiply(Build_Position_Rotation_Matrix(iter->Walls[EmitterA].Rotation.x, iter->Walls[EmitterA].Rotation.y, iter->Walls[EmitterA].Rotation.z), GetMatrixPosition(iter->ShieldTowerObject));

				/*
				if(Logging)
				{
					if(!BuildingHelper::Get().IsBuildingPowered(iter->ShieldTowerObject))
						FormatLogMessage("Removed by Host not Powered?!?!!????");
					if(!BuildingHelper::Get().IsBuildingPowered(iter->Walls[EmitterA].Target))
						FormatLogMessage("Removed by Target not Powered...");
					if(!IsAround(iter->Walls[EmitterA].Target))
						FormatLogMessage("Removed by Target Not Around. :(");
					if(BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iter->Walls[EmitterA].Target, iter->Walls[EmitterA].Width, PositionA.front, iter->Walls[EmitterA].TargetFront))
						FormatLogMessage("Removed by Blocked by Building 1");
					if(BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iter->Walls[EmitterA].Target, iter->Walls[EmitterA].TargetWidth, PositionA.front, iter->Walls[EmitterA].TargetFront))
						FormatLogMessage("Removed by Blocked by Building 2");
				}
				*/

				if ((!BuildingHelper::Get().IsBuildingPowered(iter->ShieldTowerObject)) || // Start Tower is not powered.
					(!BuildingHelper::Get().IsBuildingPowered(iter->Walls[EmitterA].Target)) || // End Tower is not powered.
					(!IsAround(iter->Walls[EmitterA].Target)) || // End Tower is not around.
					(BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iter->Walls[EmitterA].Target, iter->Walls[EmitterA].Width, PositionA.front, iter->Walls[EmitterA].TargetFront)) || // If anything is between A ... B.
					(BZ1Helper::Get().IsBlockedByBuilding(iter->ShieldTowerObject, iter->Walls[EmitterA].Target, iter->Walls[EmitterA].TargetWidth, PositionA.front, iter->Walls[EmitterA].TargetFront))) // If anything is between B ... A.
				{
					// Remove all the shield emitters.
					for(int x = 0; x < iter->Walls[EmitterA].Count; x++)
					{
						if(Logging)
							FormatLogMessage("Removing Shield Index: %d, Handle: %08x <- %08x, Emitter: %d, Piece: %d", iter - ShieldList.begin(), iter->Walls[EmitterA].Target, iter->ShieldTowerObject, EmitterA, x);

						if(IsAround(iter->Walls[EmitterA].Fields[x]))
							RemoveObject(iter->Walls[EmitterA].Fields[x]);
						if(IsAround(iter->Walls[EmitterA].Effects[x]))
							RemoveObject(iter->Walls[EmitterA].Effects[x]);
					}
					iter->Walls[EmitterA].IsActive = false; // Flag deactivation.
					iter->Walls[EmitterA].Count = 0; // Clear this since we just removed this wall.
				}
			} // End not active.
		} // End loop over tap emitters.
	} // End loop over all shield objects.
}