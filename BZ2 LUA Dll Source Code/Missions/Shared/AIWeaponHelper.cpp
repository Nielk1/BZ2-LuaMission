#include "BZ1Helper.h"
#include "AIWeaponHelper.h"
#include "BZCScriptUtils.h"

AIWeaponHelper::AIWeaponHelper()
{
}

void AIWeaponHelper::AIGiveWeapons(const Handle h, const int Team)
{
	if(!GetNumWeapons(h))
		return;

	bool Logging = BZ1Helper::Logging;
	const char *BaseODFName = GetConfigODF(h);

	if((BaseODFName) && (OpenODF2(BaseODFName)))
	{
		int NumberOfWeapons[MAX_HARDPOINTS] = {0};
		char WeaponODFList[MAX_HARDPOINTS][16][MAX_ODF_LENGTH] = {0}; // Weapon [WEAPON SLOT][NUMBER OF ENTRIES]
		int MinWeaponMask = 0;
		bool WeaponMaskSet = false;
		int PossibleWeaponMasks[16] = {0};
		int NumWeaponMasks = {0};
		bool GotRandomMasks = false;
		int Difficulty = BZ1Helper::Get().RefTeamInfo(Team).m_CPUAIDifficulty;
		for(int i = 4; i >= 0; i--) // Start at weaponSlot 5 and go up. 
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			if(Difficulty == 1)
				sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
			else if(Difficulty == 2)
				sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
			else if(Difficulty == 3)
				sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
			else if(Difficulty == 4)
				sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

			for(int x = 0;x<16;x++)
			{
				char DesiredValue2[MAX_ODF_LENGTH] = {0};
				sprintf_s(DesiredValue2, "WeaponName%d", x+1);
				if(GetODFString(BaseODFName, DesiredValue, DesiredValue2, MAX_ODF_LENGTH, WeaponODFList[i][NumberOfWeapons[i]])) // Grab the value.
					++NumberOfWeapons[i];
				sprintf_s(DesiredValue2, "WeaponMask%d", x+1);
				if(GetODFInt(BaseODFName, DesiredValue, DesiredValue2, &PossibleWeaponMasks[NumWeaponMasks])) // Grab the value.
					++NumWeaponMasks;
			}
			if(NumWeaponMasks)
					GotRandomMasks = true;
			if(!WeaponMaskSet)
				if(GetODFInt(BaseODFName, DesiredValue, "MinWeaponMask", &MinWeaponMask, 1))
					WeaponMaskSet = true;

			int RandomWeapon = -1; // Assume 0 for now, if it doesn't get set, it won't get used. 

			if(NumberOfWeapons[i])
			{
				// Now that we have the weapons, Randomly select one to give to the ships.
				// Check Requirements first, if not, pick another random one. Only try 16 times.
				for(int Tries = 0; Tries<16; Tries++)
				{
					bool StopTrying = true;

					int TempRandomWeapon = GetRandomInt(NumberOfWeapons[i]-1);

					if(Logging)
						FormatLogMessage("Number Of Weapons %d is: %d, Choice: %d WeaponName: %s", i, NumberOfWeapons[i], TempRandomWeapon, WeaponODFList[i][TempRandomWeapon]); 

				//	int TempRandomWeapon = int(GetRandomFloat(float(NumberOfWeapons[i])));
				//	if(TempRandomWeapon > NumberOfWeapons[i]-1)
				//		TempRandomWeapon = NumberOfWeapons[i]-1;

					char TempAPFileName[MAX_ODF_LENGTH] = {0};
					sprintf_s(TempAPFileName, "%s.odf", WeaponODFList[i][TempRandomWeapon]);

					if((WeaponODFList[i][TempRandomWeapon][0]) && (OpenODF2(TempAPFileName)))
					{
						int RequireCount = 0;
						GetODFInt(TempAPFileName, "GameObjectClass", "requireCount", &RequireCount, 0); // Grab the value.

						if(!RequireCount)
						{
							RandomWeapon = TempRandomWeapon;
							break;
						}
						// Loop over provides.
						for(int t = 0; t < RequireCount; t++)
						{
							bool FoundOne = false;

							char RequireProvide[MAX_ODF_LENGTH] = {0};
							// Loop over existing provide names, compare with a loop over buildings, if passes, break from loop, we have a winner.
							char DesiredValue[MAX_ODF_LENGTH] = {0};
							sprintf_s(DesiredValue, "requireName%d", t+1);
							if(GetODFString(TempAPFileName, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, RequireProvide)) // Grab the value.
							{
								// Loop over all the team's objects, looking for this in provide names, and/or baseName/ODFName.
								for(int b = 0; b < BZ1Helper::Get().GetNumHandles(); b++)
								{
									BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(b);
									Handle TempBuilding = objectInfo.ObjectHandle;
									if(objectInfo.CurTeamNum == Team)
									{
										const char *ObjectODFName = GetODFName(TempBuilding);

										// First off, is this ODF file name match the require name? If so, we don't need to continue. 
										if((ObjectODFName) && (_stricmp(RequireProvide, ObjectODFName) == 0))
										{
											FoundOne = true;
											RandomWeapon = TempRandomWeapon;
											break;
										}
										else
										{
											// Grab out the ODF, and look inside it. Pull out all vars we might need. -GBD
											char BuildingProvideName[MAX_ODF_LENGTH] = {0};
											GetODFString(TempBuilding, "GameObjectClass", "baseName", MAX_ODF_LENGTH, BuildingProvideName);
											// Next, check the BaseName, if it matches, we can abort. 
											if(BuildingProvideName[0])
											{
												if(_stricmp(RequireProvide, BuildingProvideName) == 0)
												{
													FoundOne = true;
													RandomWeapon = TempRandomWeapon;
													break;
												}
											}
											else // Nope.avi, now we have to read in it's ProvideNames.
											{
												int ProvideCount = 0;
												GetODFInt(TempBuilding, "GameObjectClass", "provideCount", &ProvideCount, 0);

												for(int p = 0; p < ProvideCount; p++)
												{
													char DesiredValue[MAX_ODF_LENGTH] = {0};
													sprintf_s(DesiredValue, "provideName%d", p+1);
													GetODFString(TempBuilding, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, BuildingProvideName);

													if(_stricmp(RequireProvide, BuildingProvideName) == 0)
													{
														FoundOne = true;
														RandomWeapon = TempRandomWeapon;
														break;
													}
												}
											}
										} // End Else for not direct ODF match.
									} // Teams match.
									if(FoundOne)
										break;
								} // End loop over all buildings/provide objects.
							} // End Sucessfully read requireName
							if(!FoundOne) // If we didn't find one, try again.
								StopTrying = false;
						} // End loop over requireCount
					} // AP file exists.
					if(StopTrying)
						break;
				} // End loop over 16 attempts to find a valid weapon.
			} // if NumberOfWeapons > 0.

			// Only try to give it if it passed the require check.
			if(RandomWeapon >= 0)
			{
				char TempWeaponODF[MAX_ODF_LENGTH] = {0}; // Store the actual weapon name.

				if(WeaponODFList[i][RandomWeapon][0])
				{
					if(DoesODFExist(WeaponODFList[i][RandomWeapon]))
					{
						// Copy ODF name to find real ODF name. 
						sprintf_s(TempWeaponODF, "%s.odf", WeaponODFList[i][RandomWeapon]);
						OpenODF2(TempWeaponODF);

						if(GetODFString(TempWeaponODF, "WeaponPowerupClass", "weaponName", MAX_ODF_LENGTH, TempWeaponODF)) // Grab the value.
						{
							// Got it
						}
					}
					else
					{
						FormatLogMessage("ERROR: %s Weapon '%s' does not exist. Fix ODFs!", BaseODFName, WeaponODFList[i][RandomWeapon]); 
					}
				}

				for(int x = 0; x<=i;x++)
				{
					if(TempWeaponODF[0])
					{
						if(DoesODFExist(TempWeaponODF))
							GiveWeapon(h, TempWeaponODF);
						else // Only print this if the string is not empty.
							FormatLogMessage("ERROR: %s Weapon '%s' does not exist. Fix ODFs!", WeaponODFList[i][RandomWeapon], TempWeaponODF); 
					}
				}
			}
		}

		int WeaponMaskSetting = 1;
		if(NumWeaponMasks > 0)
		{
			int TempWeaponMask = GetRandomInt(NumWeaponMasks-1);
			WeaponMaskSetting = PossibleWeaponMasks[TempWeaponMask];
		}
		else
		{
			float WeaponMaskSelection = GetRandomFloat(5.0f);
			if(WeaponMaskSelection <= 1.0)
				WeaponMaskSetting = 31;
			else if(WeaponMaskSelection <= 2.0)
				WeaponMaskSetting = 15;
			else if(WeaponMaskSelection <= 3.0)
				WeaponMaskSetting = 7;
			else if(WeaponMaskSelection <= 4.0)
				WeaponMaskSetting = 3;
			else
				WeaponMaskSetting = 1;
		}

		if(WeaponMaskSetting < MinWeaponMask)
			WeaponMaskSetting = MinWeaponMask;

		SetWeaponMask(h, WeaponMaskSetting);
	}
}
