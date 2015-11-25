#include "BZ1Helper.h"
#include "MorphObjectHelper.h"
#include "DispatchHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

MorphObjectHelper *MorphObjectHelper::m_MorphObjectHelper;

MorphObjectHelper::MorphObjectHelper()
{
	m_MorphObjectHelper = this;
}
MorphObjectHelper::~MorphObjectHelper()
{
	 m_MorphObjectHelper = NULL;
}

bool MorphObjectHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		MorphObjectList.resize(size);
		if(size)
			Read(&MorphObjectList.front(), sizeof(MorphObjectClass)*size);
	}
	return true;
}

bool MorphObjectHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<MorphObjectClass>::iterator iter = MorphObjectList.begin(); iter != MorphObjectList.end(); ++iter)
		{
			MorphObjectClass &sao = *iter;
			ConvertHandles(&sao.MorphObject, 1);
			ConvertHandles(&sao.CurrentWho, 1);
		}
	}
	return true;
}

bool MorphObjectHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(MorphObjectList.size());
		Write(&size, 1);
		if(size)
			Write(&MorphObjectList.front(), sizeof(MorphObjectClass)*size);
	}
	return true;
}

// In BZ1Helper, if morph object.
void MorphObjectHelper::AddMorphObjectHostObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	if(!BZ1Helper::Get().RefStopAddObject()) // RUDE HACK!
	{
		for (std::vector<MorphObjectClass>::iterator iter = MorphObjectList.begin(); iter != MorphObjectList.end(); ++iter)
		{
			if(iter->MorphObject == h)
				return;
		}

		MorphObjectList.resize(MorphObjectList.size()+1);
		MorphObjectClass &sao = MorphObjectList[MorphObjectList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(MorphObjectClass)); // Zero things out at start.

		// Get the _config.odf. (fills baseODFName)
		const char *BaseODFName = GetConfigODF(h);

		if(DoesFileExist(BaseODFName))
			//for(int i=0;i<NUM_CMD;i++)
			DispatchHelper::Get().ReadChanceValues(BaseODFName, "CloakChance", sao.CloakChance);

		// Get the units EngageRange, if not present use RangeScan.
		GetODFBool(TempODF1, TempODF2, "CraftClass", "CanAICloak", &sao.IsCloaker, false);

		// Save EngageRange.
		sao.EngageRange = GetEngageRange(h);

		// Preload the ODF here, just in case.
		char SwitchODF[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "MorphObjectClass", "SwitchODF", MAX_ODF_LENGTH, SwitchODF);
		if((SwitchODF[0]) && (DoesODFExist(SwitchODF)))
			PreloadODF(SwitchODF);

		sao.MorphObject = h; // Save it.
		sao.MaxAmmo = GetMaxAmmo(h);
		sao.IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f));
	}
}

// Toggle activators for the replacement of the walker object.
void MorphObjectHelper::AddMorphObjectToggleObject(const Handle h)
{
	float BestDistanceSquared = FLT_MAX;
	for (std::vector<MorphObjectClass>::iterator iter = MorphObjectList.begin(); iter != MorphObjectList.end(); ++iter)
	{
		float DistanceSquared = GetDistance3D(h, iter->MorphObject);
		if((DistanceSquared < BestDistanceSquared) && (h != iter->MorphObject))
		{
			BestDistanceSquared = DistanceSquared;
			if(BestDistanceSquared < Squared(5.0f)) // This is toggle distance.
			{
				if(iter->WeaponActive) // Toggle it off.
				{
					iter->WeaponActive = false;
					iter->ObjectSwitch = true; // Flag this to trigger swapping.
				}
				else
				{
					iter->WeaponActive = true; // toggle it back on.
					iter->ObjectSwitch = true; // Flag this to trigger swapping.
				}

				break;
			}
		}
	}
	//SetLifespan(h, 0.1f);
	BZ1Helper::Get().DeleteAfterDelay(h);
}

void MorphObjectHelper::Execute(BZ1Helper &bz1Helper)
{
	//bool Logging = BZ1Helper::Logging; //false;

	// pre-clean the vector
	MorphObjectList.erase(std::remove_if(MorphObjectList.begin(), MorphObjectList.end(), ShouldRemoveMorphObject), MorphObjectList.end());

	for (std::vector<MorphObjectClass>::iterator iter = MorphObjectList.begin(); iter != MorphObjectList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->MorphObject);
		// It's active.
	//	if(iter->WeaponActive)
	//	{
			if(iter->ObjectSwitch)
			{
				char SwitchODF[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
				bool SwitchAllWeapons = false;

				GetODFString(iter->MorphObject, "MorphObjectClass", "SwitchODF", MAX_ODF_LENGTH, SwitchODF);
				GetODFBool(iter->MorphObject, "MorphObjectClass", "CanFireWhenSwitched", &iter->CanFire, true);
				GetODFBool(iter->MorphObject, "MorphObjectClass", "SwitchAllWeapons", &SwitchAllWeapons, true);

				// Upon activation, save these.
				if(iter->WeaponActive)
				{
					GetODFBool(iter->MorphObject, "MorphObjectClass", "CanFireWhenSwitched", &iter->CanFire, true);
					iter->NormalAmmo = GetCurAmmo(iter->MorphObject);
				}

				char NewWeapon[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
				for(int x = 0; x < MAX_HARDPOINTS; x++)
				{
					GetObjInfo(iter->MorphObject, ObjectInfoType(Get_Weapon0Config+x), NewWeapon[x]);

					bool SwapWeapon = false;
					char WeaponODF[MAX_ODF_LENGTH] = {0};
					GetObjInfo(iter->MorphObject, ObjectInfoType(Get_Weapon0ODF+x), WeaponODF);

					if(iter->WeaponActive) // Upon activation, save this.
						iter->LocalAmmo[x] = GetCurLocalAmmo(iter->MorphObject, x);

					if(OpenODF2(WeaponODF))
					{
						char MineODF[MAX_ODF_LENGTH] = {0};
						if(GetODFString(WeaponODF, "DispenserClass", "objectClass", MAX_ODF_LENGTH, MineODF)) // Grab the value.
						{
							char TempBZCClass[MAX_ODF_LENGTH] = {0};
							strcat_s(MineODF, ".odf"); // Add .odf to the end.
							if((OpenODF2(MineODF)) && (GetODFString(MineODF, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, TempBZCClass))) // Grab the value.
							{
								if(_stricmp(TempBZCClass,"MorphObjecttoggle") == 0)
									SwapWeapon = true;
							}
						}
					}
					if(!SwitchAllWeapons) // Only swap this one morph weapon.
					{
						if(SwapWeapon)
						{
							if(GetODFString(WeaponODF, "WeaponClass", "altName", MAX_ODF_LENGTH, NewWeapon[x])) // Grab the value.
								break;
						}
					}
					else // Switch all weapons.
					{
						if(GetODFString(WeaponODF, "WeaponClass", "altName", MAX_ODF_LENGTH, NewWeapon[x])) // Grab the value.
						{
							// Got it.
						}
					}
				//	CloseODF(WeaponODFs[x]);
				}

				if(SwitchODF[0])
				{
					if(DoesODFExist(SwitchODF))
					{
						BZ1Helper::Get().RefStopAddObject() = true; // RUDE HACK!
						if(GetCurrentCommand(iter->MorphObject) == CMD_HUNT)
						{
							if((iter->Command != GetCurrentCommand(iter->MorphObject)) && (iter->Command != CMD_HUNT))
								iter->MorphObject = BZ1Helper::Get().ReplaceObject(iter->MorphObject, SwitchODF, -1, 0.0f, -1, false, -1, false, iter->Command, iter->CurrentWho);
							else
								iter->MorphObject = BZ1Helper::Get().ReplaceObject(iter->MorphObject, SwitchODF, -1, 0.0f, -1, false, -1, false);
						}
						else
						{
							iter->MorphObject = BZ1Helper::Get().ReplaceObject(iter->MorphObject, SwitchODF, -1, 0.0f, -1, false);
						}
						BZ1Helper::Get().RefStopAddObject() = false; // RUDE HACK!

						// Play a Sound here.
						char SoundFile[MAX_ODF_LENGTH] = {0};
						if(iter->WeaponActive)
							GetODFString(iter->MorphObject, "DeployableClass", "soundDeploy", MAX_ODF_LENGTH, SoundFile);
						else
							GetODFString(iter->MorphObject, "DeployableClass", "soundUndeploy", MAX_ODF_LENGTH, SoundFile);

						if(DoesFileExist(SoundFile))
							StartAudio3D(SoundFile, iter->MorphObject, AUDIO_CAT_EXPLOSION);
						else if(SoundFile[0])
							FormatLogMessage("ERROR: Morph Object SoundDeploy '%s' Doesn't Exist! FIX ODFs!", SoundFile);

						if(iter->IsCloaker)
						{
							if(iter->WeaponActive)
								SetPerceivedTeam(iter->MorphObject, 0);
							else
								SetPerceivedTeam(iter->MorphObject, Team);
						}


						if(!iter->CanFire)
						{
							if(iter->WeaponActive)
								SetCurAmmo(iter->MorphObject, 0);
								//SetMaxAmmo(MorphObjectHostObjects[Team][i], 0); // Prevents beeping of service bays, but also prevents picking up of ammo.
							else
								SetCurAmmo(iter->MorphObject, iter->NormalAmmo);
						}

						iter->ObjectSwitch = false;

						ReplaceWeapons(iter->MorphObject, NewWeapon, iter->LocalAmmo);
					}
					else
					{
						FormatLogMessage("ERROR: SwitchODF '%s' does not exist. Fix ODFs!", SwitchODF); 
					}
				}
			}
			else // Object is active, and switched.
			{
				if(iter->WeaponActive)
				{
					if(!iter->CanFire)
					{
						// While cloaked, prevent ammo pickup, but add it to the current ammo if its greater then the max of our stored values.
						if(GetCurAmmo(iter->MorphObject) > 0)
						{
							if(GetCurAmmo(iter->MorphObject) + iter->NormalAmmo > iter->MaxAmmo)
								iter->NormalAmmo = iter->MaxAmmo;
							else
								iter->NormalAmmo += GetCurAmmo(iter->MorphObject);

							SetCurAmmo(iter->MorphObject, 0);
						}
					}
					else // Remember our current ammo when we switch again.
					{
						if(iter->NormalAmmo != GetCurAmmo(iter->MorphObject))
							iter->NormalAmmo = GetCurAmmo(iter->MorphObject);
					}
					// Keep us hidden, even if we hop out / back in.
					if(iter->IsCloaker)
						SetPerceivedTeam(iter->MorphObject, 0);
				}

				// Watch for AI Decloaking.
				if((iter->IsCloaker) && (!IsPlayer(iter->MorphObject)) && (GetPlayerHandle(Team) == 0))
				{
					// Watch for nearby enemies, and switch back if they're near.
					if(iter->WeaponActive)
					{
						Handle Enemy = GetNearestEnemy(iter->MorphObject);
						if(GetDistance3DSquared(iter->MorphObject, Enemy) < Squared(iter->EngageRange))
						{
							SetCommand(iter->MorphObject, CMD_HUNT, 0); // Decloak.
							iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(15.0f)); // Wait 15 seconds before trying to cloak again.
						}
					}
					else
					{
						// Set Cloak Idle time, and Cloak if we want to.
						if(BZ1Helper::m_TurnCounter >= iter->IdleTime)
						{
							int CurrentCommand = GetCurrentCommand(iter->MorphObject);
							float Chance = GetRandomFloat(1.0f);
							Handle Enemy = GetNearestEnemy(iter->MorphObject);
							// If there are no enemies in range.
							if((!Enemy) || (GetDistance3DSquared(iter->MorphObject, Enemy) > Squared(iter->EngageRange+50.0f)))
							{
								if(Chance < iter->CloakChance[CurrentCommand])
									SetCommand(iter->MorphObject, CMD_HUNT, 0);

								iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f)); // Wait 30 seconds before trying to cloak again.
							} // Unit is not deployed and there are no enemies around.
						} // Cloak idle time is up.
					}
				}

				// Watch for Deactivation.
				if(GetCurrentCommand(iter->MorphObject) == CMD_HUNT)
				{
					char MineObject[MAX_ODF_LENGTH] = {0};
					GetSwitchMineODF(iter->MorphObject, MineObject);
					BuildObject(MineObject, Team, iter->MorphObject); // Toggle the switch.
				}
			}
	//	}
		/*
		else // The morph weapon is not activate.
		{
			// This one is already cloaked, time to deactivate it.
			if(iter->ObjectSwitch)
			{
				char SwitchODF[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
				bool SwitchAllWeapons = false;
		
				GetODFString(iter->MorphObject, "MorphObjectClass", "SwitchODF", MAX_ODF_LENGTH, SwitchODF);
				GetODFBool(iter->MorphObject, "MorphObjectClass", "SwitchAllWeapons", &SwitchAllWeapons, false);

				//char Weapon[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
				char NewWeapon[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};

				for(int x = 0; x < MAX_HARDPOINTS; x++)
				{
					GetObjInfo(iter->MorphObject, ObjectInfoType(Get_Weapon0Config+x), NewWeapon[x]);

					bool SwapWeapon = false;
					char WeaponODF[MAX_ODF_LENGTH] = {0};
					GetObjInfo(iter->MorphObject, ObjectInfoType(Get_Weapon0ODF+x), WeaponODF);
					if(OpenODF2(WeaponODF))
					{
						char MineODF[MAX_ODF_LENGTH] = {0};
						if(GetODFString(WeaponODF, "DispenserClass", "objectClass", MAX_ODF_LENGTH, MineODF)) // Grab the value.
						{
							char TempBZCClass[MAX_ODF_LENGTH] = {0};
							strcat_s(MineODF, ".odf"); // Add .odf to the end.
							if((OpenODF2(MineODF)) && (GetODFString(MineODF, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, TempBZCClass))) // Grab the value.
							{
								if(_stricmp(TempBZCClass,"MorphObjecttoggle") == 0)
									SwapWeapon = true;
							}
						}
					}
					if(!SwitchAllWeapons) // Only swap this one morph weapon.
					{
						if(SwapWeapon)
						{
							if(GetODFString(WeaponODF, "WeaponClass", "altName", MAX_ODF_LENGTH, NewWeapon[x])) // Grab the value.
								break;
						}
					}
					else // Switch all weapons.
					{
						if(GetODFString(WeaponODF, "WeaponClass", "altName", MAX_ODF_LENGTH, NewWeapon[x])) // Grab the value.
						{
							// Got it.
						}
					}
				//	CloseODF(WeaponODFs[x]);
				}

				if(SwitchODF[0])
				{
					if(DoesODFExist(SwitchODF))
					{
						BZ1Helper::Get().RefStopAddObject() = true; // RUDE HACK!
						if(GetCurrentCommand(iter->MorphObject) == CMD_HUNT)
						{
							if((iter->Command != GetCurrentCommand(iter->MorphObject)) && (iter->Command != CMD_HUNT))
								iter->MorphObject = BZ1Helper::Get().ReplaceObject(iter->MorphObject, SwitchODF, -1, 0.0, -1, false, -1, false, iter->Command, iter->CurrentWho);
							else
								iter->MorphObject = BZ1Helper::Get().ReplaceObject(iter->MorphObject, SwitchODF, -1, 0.0, -1, false, -1, false);
						}
						else
						{
							iter->MorphObject = BZ1Helper::Get().ReplaceObject(iter->MorphObject, SwitchODF, -1, 0.0, -1, false);
						}
						BZ1Helper::Get().RefStopAddObject() = false; // RUDE HACK!

						// Play a Sound here.
						char SoundFile[MAX_ODF_LENGTH] = {0};
						GetODFString(iter->MorphObject, "DeployableClass", "soundUndeploy", MAX_ODF_LENGTH, SoundFile);
						if(DoesFileExist(SoundFile))
							StartAudio3D(SoundFile, iter->MorphObject, AUDIO_CAT_EXPLOSION);
						else if(SoundFile[0])
							FormatLogMessage("ERROR: Morph Object SoundDeploy '%s' Doesn't Exist! FIX ODFs!", SoundFile);

						if(iter->IsCloaker)
							SetPerceivedTeam(iter->MorphObject, Team);
						SetCurAmmo(iter->MorphObject, iter->NormalAmmo);
						iter->ObjectSwitch = false;

						ReplaceWeapons(iter->MorphObject, NewWeapon, iter->LocalAmmo);
					}
					else
					{
						FormatLogMessage("ERROR: SwitchODF '%s' does not exist. Fix ODFs!", SwitchODF); 
					}
				}
			} // Morph weapon built.
			else // It's not currently cloaked. Look for activation via AI trigger "Hunt". 
			{
				// Handle AI Cloaking.
				if((iter->IsCloaker) && (!IsPlayer(iter->MorphObject)) && (GetPlayerHandle(Team) == 0))
				{
					if(BZ1Helper::m_TurnCounter >= iter->IdleTime)
					{
						int CurrentCommand = GetCurrentCommand(iter->MorphObject);
						float Chance = GetRandomFloat(1.0f);
						Handle Enemy = GetNearestEnemy(iter->MorphObject);
						// If there are no enemies in range.
						if((!Enemy) || (GetDistance3DSquared(iter->MorphObject, Enemy) > Squared(iter->EngageRange+50.0f)))
						{
							if(Chance < iter->CloakChance[CurrentCommand])
								SetCommand(iter->MorphObject, CMD_HUNT, 0);

							iter->IdleTime = (BZ1Helper::m_TurnCounter + (30.0f * BZ1Helper::m_GameTPS)); // Wait 30 seconds before trying to cloak again.
						} // Unit is not deployed and there are no enemies around.
					} // Cloak idle time is up.
				}

				// Watch for Deactivation command "Hunt". 
				if(GetCurrentCommand(iter->MorphObject) == CMD_HUNT)
				{
					char MineObject[MAX_ODF_LENGTH] = {0};
					GetSwitchMineODF(iter->MorphObject, MineObject);
					BuildObject(MineObject, Team, iter->MorphObject); // Toggle the switch.
				}
			}
		} // Weapon not active.
		*/
		if(GetCurrentCommand(iter->MorphObject) != CMD_HUNT)
		{
			iter->Command = GetCurrentCommand(iter->MorphObject);
			iter->CurrentWho = GetCurrentWho(iter->MorphObject);
		}
	}
}

// Gets the first "morphobjecttoggle" weapon in a unit.
void MorphObjectHelper::GetSwitchMineODF(const Handle h, char *Value)
{
	static char MineObject[MAX_ODF_LENGTH] = {0};

	for(int x = 0; x < MAX_HARDPOINTS; x++)
	{
		char WeaponODF[MAX_ODF_LENGTH] = {0};
		GetObjInfo(h, ObjectInfoType(Get_Weapon0ODF+x), WeaponODF);
		if(OpenODF2(WeaponODF))
		{
			char MineODF[MAX_ODF_LENGTH] = {0};
			if(GetODFString(WeaponODF, "DispenserClass", "objectClass", MAX_ODF_LENGTH, MineObject)) // Grab the value.
			{
				char TempBZCClass[MAX_ODF_LENGTH] = {0};
				sprintf_s(MineODF, "%s.odf", MineObject); // Add .odf to the end.
				if((OpenODF2(MineODF)) && (GetODFString(MineODF, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, TempBZCClass))) // Grab the value.
				{
					if(_stricmp(TempBZCClass,"MorphObjectToggle") == 0)
						break;
				}
			}
		}
	}

	strcpy_s(Value, sizeof(MineObject), MineObject);
}