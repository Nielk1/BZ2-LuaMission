#include "BZ1Helper.h"
#include "SwitchClassHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

SwitchClassHelper::SwitchClassHelper()
{
}
SwitchClassHelper::~SwitchClassHelper()
{
	 //m_BZ1Helper = NULL;
}

bool SwitchClassHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		SwitchObjectList.resize(size);
		if(size)
			Read(&SwitchObjectList.front(), sizeof(SwitchObjectClass)*size);
	}
	return true;
}

bool SwitchClassHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<SwitchObjectClass>::iterator iter = SwitchObjectList.begin(); iter != SwitchObjectList.end(); ++iter)
		{
			SwitchObjectClass &sao = *iter;
			ConvertHandles(&sao.SwitchObject, 1);
		}
	}
	return true;
}

bool SwitchClassHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(SwitchObjectList.size());
		Write(&size, 1);
		if(size)
			Write(&SwitchObjectList.front(), sizeof(SwitchObjectClass)*size);
	}
	return true;
}

void SwitchClassHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2)
{
	//* SwitchClass Code*
	int Team = GetTeamNum(victimHandle);
	int SwitchTeam = ordnanceTeam;
	if(!SwitchTeam)
		SwitchTeam = GetTeamNum(shooterHandle);

	// read Original Team if this object is something that already is switched.
	for (std::vector<SwitchObjectClass>::iterator iter = SwitchObjectList.begin(); iter != SwitchObjectList.end(); ++iter)
	{
		if(iter->SwitchObject == victimHandle)
		{
			Team = iter->OriginalTeam;
			break;
		}
	}

	if((!IsTeamAllied(SwitchTeam, Team)) && (!IsPlayer(victimHandle)))
	{
		char ObjClass[MAX_ODF_LENGTH] = {0};
		GetObjInfo(victimHandle, Get_GOClass, ObjClass);
		unsigned int ObjClassHash = Hash(ObjClass);

		// If this object can be switched.
		bool CanSwitch = true;
		GetODFBool(victimHandle, "GameObjectClass", "CanSwitch", &CanSwitch, true);
		if(!CanSwitch)
			return;
		
		// If it is NOT in a Base Slot.
		for(int i = 1; i<15;i++)
		{
			if(GetObjectByTeamSlot(Team,i) == victimHandle)
				return;
		}

		// It's a switchclass, get the info.
		if( (IsVehicle(victimHandle)) || (IsPerson(victimHandle)) || //(ObjClassHash == CLASS_PERSON) || 
			(ObjClassHash == CLASS_ARMORY) || (ObjClassHash == CLASS_FACTORY) || 
			(ObjClassHash == CLASS_RECYCLER) || (ObjClassHash == CLASS_TURRET))
		{
			float TempSwitchTime = 0.0f;
			GetODFFloat(TempODF1, TempODF2, "TeamSwitcherClass", "switchTime", &TempSwitchTime, 10.0f);

			for (std::vector<SwitchObjectClass>::iterator iter = SwitchObjectList.begin(); iter != SwitchObjectList.end(); ++iter)
			{
				if(iter->SwitchObject == victimHandle)
				{
					iter->SwitchTime = ConvertToTurns(TempSwitchTime);
					return;
				}
			}

			SwitchObjectList.resize(SwitchObjectList.size()+1);
			SwitchObjectClass &sao = SwitchObjectList[SwitchObjectList.size()-1]; // No temporary being created, *much* faster
			memset(&sao, 0, sizeof(SwitchObjectClass)); // Zero things out at start.

			sao.SwitchTime = ConvertToTurns(TempSwitchTime);
			if((sao.SwitchTime > 0) && (Team != SwitchTeam))
				sao.NewTeam = SwitchTeam;

			sao.SwitchObject = victimHandle;
			sao.OriginalTeam = Team;
		}
		//* ---SwitchClass
	}
}

void SwitchClassHelper::Execute(BZ1Helper &bz1Helper)
{
//	bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	SwitchObjectList.erase(std::remove_if(SwitchObjectList.begin(), SwitchObjectList.end(), ShouldRemoveSwitchObject), SwitchObjectList.end());

	for (std::vector<SwitchObjectClass>::iterator iter = SwitchObjectList.begin(); iter != SwitchObjectList.end(); ++iter)
	{
		// If it hasn't been switched yet.
		if(!iter->Switched)
		{
			if((iter->SwitchTime > 0.0f) && (GetTeamNum(iter->SwitchObject) != iter->NewTeam))
			{
				SetTeamNum(iter->SwitchObject, iter->NewTeam);
				iter->Switched = true;
			}
		}
		else // It's already been switched, count the time to switch it back.
		{
			if(iter->SwitchTime > 0.0f)
				--iter->SwitchTime;

			// It's time to switch back.
			if(iter->SwitchTime <= 0.0f)
			{
				// If it's NOT the player, switch it's team and stuff. Leave the players alone!
				if(!IsPlayer(iter->SwitchObject))
				{
					SetTeamNum(iter->SwitchObject,iter->OriginalTeam);
					SetBestGroup(iter->SwitchObject);
					ResetTeamSlot(iter->SwitchObject);
				}
				iter->Switched = false;
			}
		}
	}
}


#if 0 
void SwitchClassHelper::AddSwitchClassObject(Handle h, int Team, char *TempODF, char *TempODF2)
{
	if (m_NumSwitchObjects[Team] < MAX_OBJECTS)
	{
		bool AddToList = true;
		for(int a = 0; a < m_NumSwitchObjects[Team]; a++)
			if(SwitchTeamObjects[Team][a] == h)
			{
				AddToList = false;
				break;
			}
		if(AddToList)
		{
			bool Proceed = true;
			// If this object can be switched.
			if(GetODFBool(TempODF, "GameObjectClass", "CanSwitch", &Proceed, true))
			{
				// Got it.
			}
			else if(GetODFBool(TempODF2, "GameObjectClass", "CanSwitch", &Proceed, true))
			{
				// Got it.			
			}
			// If it is NOT in a Team Slot.
			for(int i = 1; i<15;i++)
				if(GetObjectByTeamSlot(Team,i) == h)
					Proceed = false;

			// Then proceed.
			if(Proceed)
			{
				/*
				// Get it's Armor class for later use.
				char TempArmorClass[MAX_ODF_LENGTH];
				memset(TempArmorClass, 0, sizeof(TempArmorClass));

				if(GetODFString(TempODF, "GameObjectClass", "armorClass", MAX_ODF_LENGTH, &TempArmorClass[0], "N")) // Grab the value.
				{
					// Got it.
				}
				else if(GetODFString(TempODF2, "GameObjectClass", "armorClass", MAX_ODF_LENGTH, &TempArmorClass[0], "N")) // Grab the value.
				{
					// Got it.			
				}
				m_SwitchObjectArmor[Team][m_NumSwitchObjects[Team]] = int(TempArmorClass[0]);
				// N = 78, n = 110, L = 76, l = 108, H = 72, h = 104
				*/

				(SwitchTeamObjects[Team])[m_NumSwitchObjects[Team]] = h;
				(m_SwitchOriginalTeam[Team])[m_NumSwitchObjects[Team]] = Team;
			//	(m_SwitchNewTeam[Team])[m_NumSwitchObjects[Team]] = GetCurHealth(h);

			//	if(Logging)
			//	{
			//		FormatLogMessage("SavedHealth is: %d ArmorClass is: %s Int is: %d", m_SwitchNewTeam[Team][m_NumSwitchObjects[Team]], TempArmorClass, m_SwitchObjectArmor[Team][m_NumSwitchObjects[Team]]); 
			//	}

				++m_NumSwitchObjects[Team];
			}
		}
	}
}

void SwitchClassHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	//* SwitchClass Code*
	int SwitchTeam = ordnanceTeam;
	if(!SwitchTeam)
		SwitchTeam = GetTeamNum(shooterHandle);

	//int Team = GetTeamNum(victimHandle);
	//if(SwitchTeam != Team)
	if(!IsTeamAllied(SwitchTeam, GetTeamNum(victimHandle)))
	{
		float TempSwitchTime = 0;

		char ObjClass[MAX_ODF_LENGTH] = {0};
		GetObjInfo(victimHandle, Get_GOClass, ObjClass);

		char TempODF[MAX_ODF_LENGTH] = {0};
		char TempODF2[MAX_ODF_LENGTH] = {0};
		char BZCClass[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
		strcpy_s(TempODF, pOrdnanceODF);
		// If this ODF does not exist, it might have been built by a factory, check for that.
		if(!DoesFileExist(TempODF))
		{
			char *dot = strchr(TempODF, ':'); // Remove everything from the : on.
			int len = dot - TempODF;
			strncpy_s(TempODF, TempODF, len);
			sprintf_s(TempODF, "%s.odf", TempODF); // Add .odf to the end.
		}
		OpenODF(TempODF);
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0]))
			sprintf_s(TempODF2, "%s.odf", TempODF2); // Add .odf to the end.
		if(DoesFileExist(TempODF2))
			OpenODF(TempODF2);

		if(GetODFString(TempODF, "OrdnanceClass", "BZCClasslabel", MAX_ODF_LENGTH, &BZCClass[0]))
		{
			// Got it.
		}
		else if(GetODFString(TempODF2, "OrdnanceClass", "BZCClasslabel", MAX_ODF_LENGTH, &BZCClass[0]))
		{
			// Got it?
		}

		for(int Team = 0; Team < MAX_TEAMS; Team++)
		{
			for(int i = 0; i < m_NumSwitchObjects[Team]; i++)
			{
				// The object is around, and this handle is one of them, and it's not already switched, and its not the player.
				if((IsAround(SwitchTeamObjects[Team][i])) && (SwitchTeamObjects[Team][i] == victimHandle))
				{
					if(!IsPlayer(SwitchTeamObjects[Team][i]))
					{
						// It's a switchclass, get the info.
						if(_stricmp(BZCClass, "switch") == 0)
						{
							if(GetODFFloat(TempODF, "TeamSwitcherClass", "switchTime", &TempSwitchTime, 10.0f)) // Grab the value.
							{
								// Got it.
							}
							else if(GetODFFloat(TempODF2, "TeamSwitcherClass", "switchTime", &TempSwitchTime, 10.0f)) // Grab the value.
							{
								// Got it.
							}

							m_SwitchObjectTime[Team][i] = ((TempSwitchTime * BZ1Helper::m_GameTPS) + BZ1Helper::m_TurnCounter);

							if((m_SwitchObjectTime[Team][i] > 0) && (GetTeamNum(SwitchTeamObjects[Team][i]) != SwitchTeam))
								m_SwitchNewTeam[Team][i] = SwitchTeam;
						}
					}
					break; // We found the target.
				}
			}
		}
		//* ---SwitchClass
		CloseODF(TempODF2);
		CloseODF(TempODF);
	}
}

void SwitchClassHelper::Execute(BZ1Helper &bz1Helper)
{
//	bool Logging = BZ1Helper::Logging;

	for(int Team = 0; Team<MAX_TEAMS; Team++)
	{
		if(m_NumSwitchObjects[Team] > 0)
		{
			for(int i = 0; i < m_NumSwitchObjects[Team]; i++)
			{
				if(IsAround((SwitchTeamObjects[Team])[i]))
				{
					// Omit "switched" objects. The whole concept of Team Switcher classlabels tends to go against this bit of code :P -GBD
					if(!m_SwitchObjectSwitched[Team][i])
					{
						int NewTeam = GetTeamNum((SwitchTeamObjects[Team])[i]);
						if(Team != NewTeam)
						{
							for(int x = 0; x<MAX_TAPS; x++)
							{
								Handle TempTap = GetTap((SwitchTeamObjects[Team])[i], x);
								if(IsAround(TempTap))
									SetTeamNum(TempTap, NewTeam);
							}
							// Set the DeployObject to the new Team's list of handles.
							SwitchTeamObjects[NewTeam][m_NumSwitchObjects[NewTeam]] = SwitchTeamObjects[Team][i];
							m_SwitchOriginalTeam[NewTeam][m_NumSwitchObjects[NewTeam]] = m_SwitchOriginalTeam[Team][i];
							m_SwitchNewTeam[NewTeam][m_NumSwitchObjects[NewTeam]] = m_SwitchNewTeam[Team][i];
						//	m_SwitchObjectArmor[NewTeam][m_NumSwitchObjects[NewTeam]] = m_SwitchObjectArmor[Team][i];
							m_SwitchObjectTime[NewTeam][m_NumSwitchObjects[NewTeam]] = m_SwitchObjectTime[Team][i];
							++m_NumSwitchObjects[NewTeam];

							// Now, delete out the i'th entry of this list
							int j;
							for(j=i; j<(m_NumSwitchObjects[Team]-1); ++j)
							{
								SwitchTeamObjects[Team][j] = SwitchTeamObjects[Team][j+1];
								m_SwitchOriginalTeam[Team][j] = m_SwitchOriginalTeam[Team][j+1];
								m_SwitchNewTeam[Team][j] = m_SwitchNewTeam[Team][j+1];
						//		m_SwitchObjectArmor[Team][j] = m_SwitchObjectArmor[Team][j+1];
								m_SwitchObjectTime[Team][j] = m_SwitchObjectTime[Team][j+1];
							}
							if(i>0)
								--i; // back up, so we don't skip an object this pass
							--m_NumSwitchObjects[Team]; // we deleted an entry

							SwitchTeamObjects[Team][m_NumSwitchObjects[Team]] = 0;
							m_SwitchOriginalTeam[Team][m_NumSwitchObjects[Team]] = 0;
							m_SwitchNewTeam[Team][m_NumSwitchObjects[Team]] = 0;
					//		m_SwitchObjectArmor[Team][m_NumSwitchObjects[Team]] = 0;
							m_SwitchObjectTime[Team][m_NumSwitchObjects[Team]] = 0;
						}
					}
				}
				if((!IsAround((SwitchTeamObjects[Team])[i])) && (m_NumSwitchObjects[Team] > 0))
				{
					// Now, delete out the i'th entry of this list
					int j;
					for(j=i; j<(m_NumSwitchObjects[Team]-1); ++j)
					{
						SwitchTeamObjects[Team][j] = SwitchTeamObjects[Team][j+1];
						m_SwitchOriginalTeam[Team][j] = m_SwitchOriginalTeam[Team][j+1];
						m_SwitchNewTeam[Team][j] = m_SwitchNewTeam[Team][j+1];
				//		m_SwitchObjectArmor[Team][j] = m_SwitchObjectArmor[Team][j+1];
						m_SwitchObjectTime[Team][j] = m_SwitchObjectTime[Team][j+1];
					}
					--i; // back up, so we don't skip an object this pass
					--m_NumSwitchObjects[Team]; // we deleted an entry

					SwitchTeamObjects[Team][m_NumSwitchObjects[Team]] = 0;
					m_SwitchOriginalTeam[Team][m_NumSwitchObjects[Team]] = 0;
					m_SwitchNewTeam[Team][m_NumSwitchObjects[Team]] = 0;
				//	m_SwitchObjectArmor[Team][m_NumSwitchObjects[Team]] = 0;
					m_SwitchObjectTime[Team][m_NumSwitchObjects[Team]] = 0;
				}

				if(IsAround((SwitchTeamObjects[Team])[i]))
				{
					/*
					Handle Assasin = GetWhoShotMe(SwitchTeamObjects[Team][i]);
					int SwitchTeam = GetTeamNum(Assasin);
					float What = GetLastEnemyShot(SwitchTeamObjects[Team][i]);
					float What2 = GetLastFriendShot(SwitchTeamObjects[Team][i]);
					bool CheckDamage = false;
					int WeaponNumber = 0;
					*/

					// If it hasn't been switched yet.
					if(!m_SwitchObjectSwitched[Team][i])
					{
						if((m_SwitchObjectTime[Team][i] > 0) && (GetTeamNum(SwitchTeamObjects[Team][i]) != m_SwitchNewTeam[Team][i]))
						{
							SetTeamNum(SwitchTeamObjects[Team][i], m_SwitchNewTeam[Team][i]);
							m_SwitchObjectSwitched[Team][i] = true;
						}
					}
					/*
						// IF it's been shot (recently? Ever? idk...), and it's not friendly fire, and It's healh has decreased, and it's not the player.
						if((Assasin) && (SwitchTeam != m_SwitchOriginalTeam[Team][i]) && (m_SwitchNewTeam[Team][i] != GetCurHealth(SwitchTeamObjects[Team][i])) && (!IsPlayer(SwitchTeamObjects[Team][i])))
						{
							//int SwitchTeam = GetTeamNum(Assasin);
							char Weapon[MAX_HARDPOINTS][MAX_ODF_LENGTH];
							memset(Weapon, 0, sizeof(Weapon));
							char TempODF2[MAX_ODF_LENGTH];
							memset(TempODF2, 0, sizeof(TempODF2));
							char Ordinence[MAX_ODF_LENGTH];
							memset(Ordinence, 0, sizeof(Ordinence));
							char WeaponBZCClass[MAX_ODF_LENGTH];
							memset(WeaponBZCClass, 0, sizeof(WeaponBZCClass));

							GetObjInfo(Assasin, Get_Weapon0ODF, Weapon[0]);
							GetObjInfo(Assasin, Get_Weapon1ODF, Weapon[1]);
							GetObjInfo(Assasin, Get_Weapon2ODF, Weapon[2]);
							GetObjInfo(Assasin, Get_Weapon3ODF, Weapon[3]);
							GetObjInfo(Assasin, Get_Weapon4ODF, Weapon[4]);

							for(int x = 0;x<5;x++)
							{
								if(Logging)
								{
									FormatLogMessage("Weapon ODF Is: %s", Weapon[x]); 
								}

								if(DoesFileExist(Weapon[x]))
								{
									OpenODF(Weapon[x]);
									if(GetODFString(Weapon[x], "WeaponClass", "ordName", MAX_ODF_LENGTH, &Ordinence[0])) // Grab the value.
									{
										// Got it.
									}
									else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
									{
										if(GetODFString(Weapon[x], "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
										{
											strcat_s(TempODF2, ".odf"); // Add .odf to the end.
											OpenODF(TempODF2);
											if(GetODFString(TempODF2, "WeaponClass", "ordName", MAX_ODF_LENGTH, &Ordinence[0])) // Grab the value.
											{
												// Got it.
											}
											CloseODF(TempODF2);
										}
									}
									CloseODF(Weapon[x]);

									strcat_s(Ordinence, ".odf"); // Add .odf to the end.
									OpenODF(Ordinence);
									if(GetODFString(Ordinence, "OrdnanceClass", "BZCClasslabel", MAX_ODF_LENGTH, &WeaponBZCClass[0])) // Grab the value.
									{
										// Got it.
									}
									else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
									{
										if(GetODFString(Ordinence, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
										{
											strcat_s(TempODF2, ".odf"); // Add .odf to the end.
											OpenODF(TempODF2);
											if(GetODFString(TempODF2, "OrdnanceClass", "BZCClasslabel", MAX_ODF_LENGTH, &WeaponBZCClass[0])) // Grab the value.
											{
												// Got it.
											}
											CloseODF(TempODF2);
										}
									}
									CloseODF(Weapon[x]);

									// If this is a weapon we give a crap about.
									if(_strnicmp(WeaponBZCClass,"switch",6) == 0)
									{
										CheckDamage = true;
										WeaponNumber = x; // Save which weapon is the first SwitchClass match.
										break;
									}
								}
							}

							// Now we need to figure out which damage value to look at.
							if(CheckDamage)
							{
								// N = 78, n = 110, L = 76, l = 108, H = 72, h = 104
								// A = 65, a = 97, D = 68, d = 100, S = 83, s = 115
								//m_SwitchObjectArmor[Team][i]

								char HostWeapon[MAX_HARDPOINTS][MAX_ODF_LENGTH];
								memset(HostWeapon, 0, sizeof(HostWeapon));
								char HostWeaponClass[MAX_HARDPOINTS][MAX_ODF_LENGTH];
								memset(HostWeaponClass, 0, sizeof(HostWeaponClass));
								char TempODF2[MAX_ODF_LENGTH];
								memset(TempODF2, 0, sizeof(TempODF2));

								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon0ODF, HostWeapon[0]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon1ODF, HostWeapon[1]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon2ODF, HostWeapon[2]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon3ODF, HostWeapon[3]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon4ODF, HostWeapon[4]);

								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon0GOClass, HostWeaponClass[0]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon1GOClass, HostWeaponClass[1]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon2GOClass, HostWeaponClass[2]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon3GOClass, HostWeaponClass[3]);
								GetObjInfo(SwitchTeamObjects[Team][i], Get_Weapon4GOClass, HostWeaponClass[4]);

								for(int x = 0;x<5;x++)
								{
									if(Logging)
									{
										FormatLogMessage("Weapon Config Is: %s", HostWeaponClass[x]); 
									}

									if(_stricmp(HostWeaponClass[x],"CLASS_SHIELDUP") == 0)
									{
										char TempShieldClass[MAX_ODF_LENGTH];
										OpenODF(HostWeapon[x]);
										if(GetODFString(HostWeapon[x], "ShieldUpgradeClass", "shieldClass", MAX_ODF_LENGTH, &TempShieldClass[0], "S")) // Grab the value.
										{
											// Got it.
										}
										else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
										{
											if(GetODFString(HostWeapon[x], "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
											{
												strcat_s(TempODF2, ".odf"); // Add .odf to the end.
												OpenODF(TempODF2);
												if(GetODFString(TempODF2, "ShieldUpgradeClass", "shieldClass", MAX_ODF_LENGTH, &TempShieldClass[0], "S")) // Grab the value.
												{
													// Got it.
												}
												CloseODF(TempODF2);
											}
										}
										CloseODF(Weapon[x]);

										m_SwitchObjectArmor[Team][i] = int(TempShieldClass[0]);

										if(Logging)
										{
											FormatLogMessage("ShieldClass is: %s Int is: %d", TempShieldClass, m_SwitchObjectArmor[Team][i]); 
										}

										break;
									}
								} // Shield Check.

								// Now that we know which type of damage is delt, calculate how much Damage the weapon should have done.
								char DamageType[MAX_ODF_LENGTH];
								memset(DamageType, 0, sizeof(DamageType));
								int BulletDamage = 0;

								/*
								char ExplosionType[MAX_ODF_LENGTH];
								memset(ExplosionType, 0, sizeof(ExplosionType));
								char Explosion[MAX_ODF_LENGTH];
								memset(Explosion, 0, sizeof(Explosion));
								int ExplosionDamage = 0;

								if(m_SwitchIsBuilding[Team][i])
									strcpy_s(ExplosionType, "xplBuilding");
								else
									strcpy_s(ExplosionType, "xplVehicle");

								// Get the Explosion file.
								OpenODF(Ordinence);
								if(GetODFString(Ordinence, "OrdnanceClass", ExplosionType, MAX_ODF_LENGTH, &Explosion[0])) // Grab the value.
								{
									// Got it.
								}
								else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
								{
									if(GetODFString(Ordinence, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
									{
										strcat_s(TempODF2, ".odf"); // Add .odf to the end.

										OpenODF(TempODF2);
										if(GetODFString(TempODF2, "OrdnanceClass", ExplosionType, MAX_ODF_LENGTH, &Explosion[0])) // Grab the value.
										{
											// Got it.
										}
										CloseODF(TempODF2);
									}
								}
								CloseODF(Ordinence);

								if(Logging)
								{
									FormatLogMessage("Explosion ODF: %s", Explosion); 
								}

								// Get Explosion Damage
								strcat_s(Explosion, ".odf"); // Add .odf to the end.
								OpenODF(Explosion);
								if(GetODFInt(Explosion, "ExplosionClass", DamageType, &ExplosionDamage, 0)) // Grab the value.
								{
									// Got it.
								}
								else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
								{
									if(GetODFString(Ordinence, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
									{
										strcat_s(TempODF2, ".odf"); // Add .odf to the end.

										OpenODF(TempODF2);
										if(GetODFInt(TempODF2, "ExplosionClass", DamageType, &ExplosionDamage, 0)) // Grab the value.
										{
											// Got it.
										}
										CloseODF(TempODF2);
									}
								}
								CloseODF(Explosion);
								*/

								/*
								sprintf_s(DamageType, "damageValue(%c)", char(m_SwitchObjectArmor[Team][i]));
								// Get Bullet Damage
								OpenODF(Ordinence);
								if(GetODFInt(Ordinence, "OrdnanceClass", DamageType, &BulletDamage, 0)) // Grab the value.
								{
									// Got it.
								}
								else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
								{
									if(GetODFString(Ordinence, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
									{
										strcat_s(TempODF2, ".odf"); // Add .odf to the end.
										OpenODF(TempODF2);
										if(GetODFInt(TempODF2, "OrdnanceClass", DamageType, &BulletDamage, 0)) // Grab the value.
										{
											// Got it.
										}
										CloseODF(TempODF2);
									}
								}
								CloseODF(Ordinence);

								if(Logging)
								{
									FormatLogMessage("DamageType Is: %s DamageDone is: %d SavedHealth is: %d RealHealth Is: %d BulletDamage is: %d", DamageType, (m_SwitchNewTeam[Team][i]-GetCurHealth(SwitchTeamObjects[Team][i])), m_SwitchNewTeam[Team][i], GetCurHealth(SwitchTeamObjects[Team][i]), BulletDamage); 
								}

								// If the damage matches, it's "likely" that this is the weapon that shot our target. NOT 100% Guarenteed!
								//if((BulletDamage+ExplosionDamage) == (m_SwitchNewTeam[Team][i]-GetCurHealth(SwitchTeamObjects[Team][i])))
								if((BulletDamage) == (m_SwitchNewTeam[Team][i]-GetCurHealth(SwitchTeamObjects[Team][i])))
								{
									float TempSwitchTime = 0.0f;
									// Get SwitchTime
									OpenODF(Ordinence);
									if(GetODFFloat(Ordinence, "TeamSwitcherClass", "switchTime", &TempSwitchTime, 10.0f)) // Grab the value.
									{
										// Got it.
									}
									else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
									{
										if(GetODFString(Ordinence, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, &TempODF2[0])) // Grab the classlabel.
										{
											strcat_s(TempODF2, ".odf"); // Add .odf to the end.
											OpenODF(TempODF2);
											if(GetODFFloat(TempODF2, "TeamSwitcherClass", "switchTime", &TempSwitchTime, 10.0f)) // Grab the value.
											{
												// Got it.
											}
											CloseODF(TempODF2);
										}
									}
									CloseODF(Ordinence);

									m_SwitchObjectTime[Team][i] = ((TempSwitchTime * m_GameTPS) + m_TurnCounter);
									SetTeamNum(SwitchTeamObjects[Team][i],SwitchTeam);
									m_SwitchObjectSwitched[Team][i] = true;
									//SetBestGroup(SwitchTeamObjects[Team][i]);

									if(Logging)
									{
										FormatLogMessage("SwitchTime is: %f ObjectTime is: %f TurnCounter Is: %d", TempSwitchTime, m_SwitchObjectTime[Team][i], m_TurnCounter); 
									}
								}
							} // Damage Check
						}
					}
					*/
					else // It's already been switched, count the time to switch it back.
					{
						// It's time to switch back.
						if(BZ1Helper::m_TurnCounter >= m_SwitchObjectTime[Team][i])
						{
							// If it's NOT the player, switch it's team and stuff. Leave the players alone!
							if(!IsPlayer(SwitchTeamObjects[Team][i]))
							{
								SetTeamNum(SwitchTeamObjects[Team][i],m_SwitchOriginalTeam[Team][i]);
								SetBestGroup(SwitchTeamObjects[Team][i]);
								ResetTeamSlot(SwitchTeamObjects[Team][i]);
							}
							m_SwitchObjectTime[Team][i] = 0.0f; // Reset this.
							m_SwitchObjectSwitched[Team][i] = false;
						}
					}
				//	m_SwitchNewTeam[Team][i] = GetCurHealth(SwitchTeamObjects[Team][i]);
				} // It's around.
			}
		}
	}
}
#endif