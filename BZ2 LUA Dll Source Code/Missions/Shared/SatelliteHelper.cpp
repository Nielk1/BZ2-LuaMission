#include "BZ1Helper.h"
#include "SatelliteHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

SatelliteHelper *SatelliteHelper::m_SatelliteHelper;

SatelliteHelper::SatelliteHelper()
{
	m_SatelliteHelper = this;
}
SatelliteHelper::~SatelliteHelper()
{
	 m_SatelliteHelper = NULL;
}

bool SatelliteHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		CommTowerList.resize(size);
		if(size)
			Read(&CommTowerList.front(), sizeof(CommTowerClass)*size);
	}

	return true;
}

bool SatelliteHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<CommTowerClass>::iterator iter = CommTowerList.begin(); iter != CommTowerList.end(); ++iter)
		{
			CommTowerClass &sao = *iter;
			ConvertHandles(&sao.CommTowerObject, 1);
		}
	}

	return true;
}

bool SatelliteHelper::Save(bool missionSave)
{
	if (!missionSave)
	{

		int size = int(CommTowerList.size());
		Write(&size, 1);
		if(size)
			Write(&CommTowerList.front(), sizeof(CommTowerClass)*size);
	}

	return true;
}

void SatelliteHelper::AddSatelliteSwitchObject(const Handle h, const int Team, const char *ODFName)
{
	if(ODFName == NULL)
		return;

	// Reference to the teamlist.
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	// Watch for Satellite On switch, and save it. 
	if(_strnicmp(ODFName,"BZCsatviewon",12) == 0) 
	{
		if((IsAround(teamInfo.SatelliteOn)) && (teamInfo.SatelliteOn != h))
			RemoveObject(teamInfo.SatelliteOn);

		teamInfo.SatelliteOn = h;

		if((IsAround(teamInfo.SatelliteOff)) && (teamInfo.SatelliteOff != h))
			RemoveObject(teamInfo.SatelliteOff); // Dangerous, make sure it's not what was just placed!
	}
	// Watch for Satellite Off switch, and save it. 
	else if(_strnicmp(ODFName,"BZCsatviewoff",13) == 0) 
	{
		if((IsAround(teamInfo.SatelliteOff)) && (teamInfo.SatelliteOff != h))
			RemoveObject(teamInfo.SatelliteOff);

		teamInfo.SatelliteOff = h;

		if((IsAround(teamInfo.SatelliteOn)) && (teamInfo.SatelliteOn != h))
			RemoveObject(teamInfo.SatelliteOn);
	}
}

void SatelliteHelper::AddSatelliteObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team)
{
	for (std::vector<CommTowerClass>::iterator iter = CommTowerList.begin(); iter != CommTowerList.end(); ++iter)
	{
		if(iter->CommTowerObject == h)
			return;
	}

	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	CommTowerList.resize(CommTowerList.size()+1);
	CommTowerClass &sao = CommTowerList[CommTowerList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(CommTowerClass)); // Zero things out at start.

	GetODFBool(TempODF1, TempODF2, "SatelliteClass", "PlayEnabledSound", &sao.PlaySound, true);

	sao.CommTowerObject = h; // Add the comm tower to the list.
	teamInfo.CommTower = h; // Set this to our Team's new Primary CommTower.
}

void SatelliteHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	PilotWeaponReplacement(); // Execute this to keep track of pilot weapons.

	// pre-clean the vector
	CommTowerList.erase(std::remove_if(CommTowerList.begin(), CommTowerList.end(), ShouldRemoveCommTower), CommTowerList.end());

	int NumCommTowersTotal[MAX_TEAMS] = {0};
	int NumCommTowersFound[MAX_TEAMS] = {0};

	for (std::vector<CommTowerClass>::iterator iter = CommTowerList.begin(); iter != CommTowerList.end(); ++iter)
	{
		int Team = GetTeamNum(iter->CommTowerObject);

		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		//if((IsAround((m_CommTowerObjects[Team])[i])) && (CommPower != 0)) // Only proceed if the object is still around and working.
		if(BuildingHelper::Get().IsBuildingPowered(iter->CommTowerObject))
			++NumCommTowersFound[Team]; // Add one.

		// If we don't have a saved active commtower, and this one is there, save it.
		if(!IsAround(teamInfo.CommTower))
		{
			teamInfo.CommTower = iter->CommTowerObject; // Set this as your new comm tower.
			//PlayCommTowerSound[Team] = iter->PlaySound;
			ResetTeamSlot(iter->CommTowerObject); // Make it 9 again? 
			//CommTowerAround[Team] = true;
		}
		if(iter->CommTowerObject == teamInfo.CommTower)
			teamInfo.PlayCommTowerSound = iter->PlaySound;

		++NumCommTowersTotal[Team];
	}

	// Main Satellite Code.
	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		// If the number of commtowers on the team is now 0, and we have commtowers flagged as existing, fix it. 
		if(NumCommTowersFound[Team] == 0)
		{
			if(NumCommTowersTotal[Team]) //&& (!IsAround(teamInfo.SatelliteLock))) // Lock it down!
				LockSatellite(Team);

			if(teamInfo.CommTowerAround)
			{
				teamInfo.CommTowerAround = false;

				if(Logging)
					FormatLogMessage("No Comm Towers on Team: %d, Number of Commtowers: %d, flagging...", Team, NumCommTowersFound[Team]);

				if((Team == GetLocalPlayerTeamNumber()) && (teamInfo.PlayCommTowerSound))// && (BZ1Helper::m_TurnCounter > 1))
				{
					char SoundFile[MAX_ODF_LENGTH] = {0};
					char EventODF[MAX_ODF_LENGTH] = {0};
					sprintf_s(EventODF, "%cevent.odf", teamInfo.TeamRace);

					if(!DoesFileExist(EventODF))
						strcpy_s(EventODF, "aevent.odf");

					if(OpenODF2(EventODF))
					{
						if(GetODFString(EventODF, "BettyVoice", "EVENT_SOUND_16", MAX_ODF_LENGTH, SoundFile)) // Grab the sound file, with allowance for a large filename.
						{
							if(SoundFile[0])
							{
								if(DoesFileExist(SoundFile))
									AudioMessage(SoundFile);
								else 
									FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix Event_Sound_16 in ODF: %s", SoundFile, EventODF); 
							}
						}
					}
				}
			}
		}
		else
		{
			if(!teamInfo.CommTowerAround)
			{
				if(Logging)
					FormatLogMessage("Found a new CommTower on Team: %d, Number of CommTowers: %d", Team, NumCommTowersFound[Team]);

				if((Team == GetLocalPlayerTeamNumber()) && (teamInfo.PlayCommTowerSound))// && (BZ1Helper::m_TurnCounter > 1))
				{
					char SoundFile[MAX_ODF_LENGTH] = {0};
					char EventODF[MAX_ODF_LENGTH] = {0};
					sprintf_s(EventODF, "%cevent.odf", teamInfo.TeamRace);

					if(!DoesFileExist(EventODF))
						strcpy_s(EventODF, "aevent.odf");

					if(OpenODF2(EventODF))
					{
						if(GetODFString(EventODF, "BettyVoice", "EVENT_SOUND_13", MAX_ODF_LENGTH, SoundFile)) // Grab the sound file, with allowance for a large filename.
						{
							if(SoundFile[0])
							{
								if(DoesFileExist(SoundFile))
									AudioMessage(SoundFile);
								else
									FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix Event_Sound_13 in ODF: %s", SoundFile, EventODF); 
							}
						}
					}
				}
				teamInfo.CommTowerAround = true;
				ToggleSatelliteOff(Team); // Make first Off Switch.
			}
		}

		// Temp Vars for blasting info into. -GBD
		Matrix PlayerPosition = Identity_Matrix; // Where the players are! :D

		// Manage Commtower Switches. These are limitclass'ed objects that disable On, Off, or both in the CommTower Build menu.
		if(CommTowerList.size())
		{
			// We have a functioning commtower, make sure proper switches are in place.
			if(teamInfo.CommTowerAround)
				UnlockSatellite(Team); // Unlock Satellite
		}
		else // No commtowers around, powered or unpowered, remove all Locks.
		{
			if(IsAround(teamInfo.SatelliteLock))
				RemoveObject(teamInfo.SatelliteLock);
			if(IsAround(teamInfo.SatelliteOn))
				RemoveObject(teamInfo.SatelliteOn);
			if(IsAround(teamInfo.SatelliteOff))
				RemoveObject(teamInfo.SatelliteOff);
		}

		// Start main code here.
		switch (teamInfo.SatelliteViewActive)
		{
		case 0: // Satellite is not currently Active, awaiting activation Trigger.
			{
				if(IsAround(teamInfo.SatelliteOn))// If you've selected the comm tower and your not in satellite. Good Job.
				{
					teamInfo.UseSiteCamera = false; // Default this to false.
					teamInfo.Player = GetPlayerHandle(Team);

					char SatelliteODF[MAX_ODF_LENGTH] = {0};
					if(IsAround(teamInfo.CommTower))
						GetODFString(teamInfo.CommTower, "SatelliteClass", "SatelliteConfig", MAX_ODF_LENGTH, SatelliteODF);

					if((!SatelliteODF[0]) || (!DoesODFExist(SatelliteODF)))
					{
						// Try to find a Race specific one.
						sprintf_s(SatelliteODF, "%cBZCsatview", teamInfo.TeamRace);

						if(!DoesODFExist(SatelliteODF))
							sprintf_s(SatelliteODF, "BZCsatview"); // Fall back to Default.
					}

					// Find out if we are using Site Camera or not.
					if(!BZ1Helper::Get().RefDisableSiteCamMP()) // MP option override, ivar90.
					{
						char SatWeaponODF[MAX_ODF_LENGTH] = {0};
						char SatWeaponClass[MAX_ODF_LENGTH] = {0};
						char TempODF[MAX_ODF_LENGTH] = {0};
						char TempODF2[MAX_ODF_LENGTH] = {0};
						sprintf_s(TempODF, "%s.odf", SatelliteODF); // Add .odf to the end.
						if(OpenODF2(TempODF))
						{
							if(GetODFString(TempODF, "GameObjectClass", "weaponName1", MAX_ODF_LENGTH, SatWeaponODF))
							{
								// Got it.
							}
							else // If this ODF is inherited, and try that ODF. If still nothing, then it's nothing.
							{
								if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
								{
									strcat_s(TempODF2, ".odf"); // Add .odf to the end.
									if((OpenODF2(TempODF2)) && (GetODFString(TempODF2, "GameObjectClass", "weaponName1", MAX_ODF_LENGTH, SatWeaponODF)))
									{
										// Got it?
									}
								}
							}
						}
						sprintf_s(TempODF, "%s.odf", SatWeaponODF); // Add .odf to the end.
						if((OpenODF2(TempODF)) && (GetODFString(TempODF, "WeaponClass", "classlabel", MAX_ODF_LENGTH, SatWeaponClass)))
						{
							if(_stricmp(SatWeaponClass, "terrainexpose") == 0)
							{
								teamInfo.UseSiteCamera = true;
								GetODFLong(TempODF, "SpecialItemClass", "ammoCost", &teamInfo.SatelliteSiteCamAmmo);
							}
						}
					}

					// If the satelite object is not around, and we have a Comm Tower, do it.
	   				if((!IsAround(teamInfo.SatellitePlayer)) && (teamInfo.CommTowerAround)) 
					{
						teamInfo.SatelliteViewActive = 1; // Assume if we got this far, it'll do the rest. This prevents an infinite loop. -GBD

						GetPosition(teamInfo.Player, PlayerPosition); // Get their position when they entered satellite. 
						teamInfo.SatellitePlayer = BuildObject(SatelliteODF,Team,PlayerPosition); // Build the Satellite Object//BuildObject(GetInitialSatelliteODF(TeamRace[Team]),Team,PlayerPosition); // Build the Satellite Object

					//	SetPilotClass(SatellitePlayer[Team], "satpilot"); // Prepare to murder/death/kill the player. 

						Deploy(teamInfo.SatellitePlayer); // Force the player to deploy? No? Damit...have to wait.
						if(teamInfo.UseSiteCamera)
							FireAt(teamInfo.SatellitePlayer, teamInfo.Player, false); // Make the thing engage sitecam?
						//SetMaxAmmo(SatellitePlayer[Team], 5); // Prevent Satellite flicker.
						teamInfo.SatelliteOutOfAmmo = 0; //false;

						SetAsUser(teamInfo.SatellitePlayer, Team); // Put them in it?

						if(Logging)
							FormatLogMessage("Made the Sandwich, Team Race is: %c", GetRaceOfTeam(Team));
					}
				}
			}
			break;
		case 1: // Activation has been triggered, and we are waiting for the SatellitePlayer object to finish Deploying. This takes 1 tick to deploy. :-/
			{
				// If the satellite object is there, and it is deployed, make their ship a dummy.
				if((IsAround(teamInfo.SatellitePlayer)) && (IsDeployed(teamInfo.SatellitePlayer)))
				{
					teamInfo.SatelliteViewActive = 2; // Set the real var for if satellite is on.

					// New code, much simpler. Just add a pilot. :) Only works in 1.3 Public Beta 7+.
					if(BZ1Helper::m_GameVersion >= v13PB7)
					{
						int PTeam = GetPerceivedTeam(teamInfo.Player);
						AddPilotByHandle(teamInfo.Player);
						SetPerceivedTeam(teamInfo.Player, PTeam);
					}
				//----- Old Code that replaces the player's old ship with a new one, due to 1.3 PB6.5 and < bug in AddPilotByHandle(). 
					else
					{
						if(Logging)
							FormatLogMessage("Made me a Sandwich");

						teamInfo.Player = BZ1Helper::Get().ReplaceObject(teamInfo.Player, NULL, -1, 0.0f, false, true, 0);
					}
				//-------------------- Done replacing things ---------------------------------

					//strcpy_s(PlayerPilotODF[Team],GetPilotClass(Player[Team])); // Dangerous, GetPilotClass(H) can return NULL if the object is empty.
					//if (char *dot = strrchr(PlayerPilotODF[Team], '.')) *dot = '\0';
					if (const char *pilotOdf = GetPilotClass(teamInfo.Player))
					{
						strcpy_s(teamInfo.PlayerPilotODF, pilotOdf);
						if (char *dot = strrchr(teamInfo.PlayerPilotODF, '.'))
							*dot = '\0';
					}
					SetPilotClass(teamInfo.Player, "satpilot"); // Make the pilot that ejects out of this non existant nothingness. Damit this does jack all...-GBD

					SetIndependence(teamInfo.Player, 0); // Don't let AI do stuff on it's own.
					SetPosition(teamInfo.SatellitePlayer, PlayerPosition); // Move you directly to your ship. 
					ClearThrust(teamInfo.Player);
					SetCommand(teamInfo.Player, CMD_NONE, 1);

					SetObjectiveOn(teamInfo.Player); // Light 'em up!

					if(IsPerson(teamInfo.Player))
						teamInfo.PlayerWasPilot = true;
					else
						teamInfo.PlayerWasPilot = false;
				}
				else // No, Bad.
				{
					break;
				}
			}
			// No BREAK here, the following code should run ASAP.
		case 2: // Satellite Mode is currently Active, Now we have to watch for it to deactivate, one way or another, and remove it/kick user out.
			{
				if(teamInfo.PlayerWasPilot)
					Retreat(teamInfo.Player, teamInfo.Player, 1); // Shut it down! Every tick! Because pilots listen as well as cats...

				if(IsAround(teamInfo.Player))
				{
					ClearThrust(teamInfo.Player);
					Matrix Position = GetMatrixPosition(teamInfo.Player);
					SetPosition(teamInfo.SatellitePlayer, Position);
					SetVelocity(teamInfo.SatellitePlayer, GetVelocity(teamInfo.Player));
				}

				// Only if the Satellite is flagged to use Site Camera. Extensive jiggery pokery trying to get it to deactivate better in MP, no luck. MUST wait 7 ticks for ammo to dissipate on it's own.
				if(teamInfo.UseSiteCamera)
				{
					// If all these pass, add ammo. If any of these toggles are not true, Let SiteCam die. 
					if((!IsAround(teamInfo.SatelliteOff)) && (teamInfo.CommTowerAround) && (IsAround(teamInfo.Player)) && (IsNotDeadAndPilot2(teamInfo.Player)))
					{
						//AddAmmo(teamInfo.SatellitePlayer, 10);
						//SetCurAmmo(SatellitePlayer[Team], 5); // Give it ammo to keep the sitecam on.
						if(IsDeployed(teamInfo.SatellitePlayer))
						{
							teamInfo.SatelliteOutOfAmmo = 0;

				//			if(Logging)
				//				FormatLogMessage("Satellite Deployed, setting Ammo Counter to 0");
						}
					}
					else
					{
						if(IsDeployed(teamInfo.SatellitePlayer))
						{
							Deploy(teamInfo.SatellitePlayer); // Force the player to deploy? No? Damit...

							if(Logging)
								FormatLogMessage("Satellite Deployed, UnDeploying Satellite?");
						}

						//AddAmmo(SatellitePlayer[Team], -1);
						//SetMaxAmmo(SatellitePlayer[Team], 0); //2); // Reset to allow turn off.

					//	if(Logging)
					//		FormatLogMessage("Max ammo on user is: %d", GetMaxAmmo(teamInfo.SatellitePlayer)); 
					}

					 // You exited Satellite view, one way or another.
					if( (!IsDeployed(teamInfo.SatellitePlayer) || // You undeployed.
						(IsAround(teamInfo.SatelliteOff)) || // You turned Satellite Off.
						(!teamInfo.CommTowerAround)) || // Your Comm Tower exploded or became unpowered.
						(!IsAliveAndPilot2(teamInfo.Player))) // You died.
					{
						if(teamInfo.SatelliteOutOfAmmo == 0)
						{
							SetCurAmmo(teamInfo.SatellitePlayer, teamInfo.SatelliteSiteCamAmmo + 1);

							if(Logging)
								FormatLogMessage("Satellite Off, Set CurAmmo to %d", teamInfo.SatelliteSiteCamAmmo + 1);
						}

						/* Failed :(
						if(teamInfo.SatelliteOutOfAmmo == 0)
						{
							GiveWeapon(teamInfo.SatellitePlayer, "gBZCSatviewOff"); // This should Deactivate SiteCamera.
							FormatLogMessage("GAVE YOU FAKE NEW WEAPON!!!");
						}
						++teamInfo.SatelliteOutOfAmmo;
						*/

						if(IsNetworkOn())
						{
							if(GetCurAmmo(teamInfo.SatellitePlayer) <= teamInfo.SatelliteSiteCamAmmo + 1) // If they're out of ammo, set the flag here.
							{
								++teamInfo.SatelliteOutOfAmmo;

								if(Logging)
									FormatLogMessage("Satellite Out of Ammo! Counter is: %d", teamInfo.SatelliteOutOfAmmo);
							}
						}
						else
						{
							teamInfo.SatelliteOutOfAmmo = 7;
						}

						/*
						if(GetCurAmmo(SatellitePlayer[Team]) == 0) // If they're out of ammo, set the flag here.
						{
							if(IsNetworkOn())
								++SatelliteOutOfAmmo[Team];
							else
								SatelliteOutOfAmmo[Team] = 7;
						}
						*/

					//	if(Logging)
					//		FormatLogMessage("COUNTER IS: %d Ammo is: %d", teamInfo.SatelliteOutOfAmmo, GetCurAmmo(teamInfo.SatellitePlayer)); 
					}
				}

				// This is the deactivation normal (congrats, despite being light up like a christmas tree, you wern't day wreckered. -GBD). 
				// In order: If you press Log Out, Or you select Satellite Off, or your ship Explodes/gets Sniped, or your Comm Tower goes boom.
				if(	(!IsDeployed(teamInfo.SatellitePlayer) || // You pressed "Log Out".
					(IsAround(teamInfo.SatelliteOff)) || // You turned Satellite off via Comm Tower.
					((!IsAround(teamInfo.Player)) || (!IsAliveAndPilot2(teamInfo.Player))) || // Your ship has exploded, or it got sniped.
					(!teamInfo.CommTowerAround)) && // Your Comm Tower blewup or became unpowered.
					((!teamInfo.UseSiteCamera) || (teamInfo.SatelliteOutOfAmmo >= 7))) // SITE CAM has had time to switch off in MP.
				{
					// Deactivate Satellite mode!
					ToggleSatelliteOff(Team);
					teamInfo.SatelliteViewActive = 0; // Flag Satellite as off.

					// Decide what to do with the Player.
					if(IsAround(teamInfo.Player)) // Your ship is still here.
					{
						if(!IsAliveAndPilot2(teamInfo.Player)) // You got Sniped.
						{
							KillPilot(teamInfo.SatellitePlayer); // Murder you!

							if(Logging)
								FormatLogMessage("You can haz been sniped!");
						}
						else // Congradulations, you survived being a Target for everyone.
						{
							SetAsUser(teamInfo.Player, Team); // Put you back into your ship.
							SetPilotClass(teamInfo.Player, teamInfo.PlayerPilotODF); // Restore their original pilot config.

							if(!teamInfo.PlayerWasPilot) // Triggers re-assigning pilot's weapons the second you Hop out.
								teamInfo.GiveBackWeapons = true;

							if(Logging)
								FormatLogMessage("Satellite Deactivated normally.");
						}

						RemoveObject(teamInfo.SatellitePlayer); // Quietly remove the satellite object.
						SetObjectiveOff(teamInfo.Player); // Hide them now!
						SetIndependence(teamInfo.Player, 1); // Restore their Brain.
						Stop(teamInfo.Player, 0); // Restore F-Group Control.
					}
					else // Your ship Exploded.
					{
						if(teamInfo.PlayerWasPilot)
						{
							KillPilot(teamInfo.SatellitePlayer); // Murder you! 
							RemoveObject(teamInfo.SatellitePlayer); // Quietly remove the satellite object.

							if(Logging)
								FormatLogMessage("Your pilot exploded!");
						}
						else
						{
							teamInfo.GiveBackWeapons = true; // Triggers re-assigning pilot's weapons the second you Eject.
							Damage(teamInfo.SatellitePlayer, GetMaxHealth(teamInfo.SatellitePlayer) + 9000); // It's over 9000!
							//EjectPilot(SatellitePlayer[Team]); // Force Ejection, even if BZBODY is on. Doesn't work on players :(

							if(Logging)
								FormatLogMessage("Your ship exploded!");
						}
					}
				}
			}
			break;
		}
	}
}

PreGetInReturnCodes SatelliteHelper::PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle)
{
	int Team = GetTeamNum(pilotHandle);

	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	if(pilotHandle == teamInfo.Player)
	{
		teamInfo.PlayerWasPilot = false;
		teamInfo.Player = emptyCraftHandle;
		SetIndependence(teamInfo.Player, 0); // Don't let AI do stuff on it's own.
		Stop(teamInfo.Player, 1); // Shut it down!
		SetObjectiveOn(teamInfo.Player); // Light 'em up!
	}

	// Always allow the entry
	return PREGETIN_ALLOW;
}

void SatelliteHelper::PilotWeaponReplacement(void)
{
//	bool Logging = BZ1Helper::Logging;

	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		Handle Player = GetPlayerHandle(Team);
		char ObjClass[MAX_ODF_LENGTH] = {0};
		GetObjInfo(Player, Get_GOClass, ObjClass);

		if(_stricmp(ObjClass, "CLASS_PERSON") == 0)
		{
			// If we havn't flagged as them being Killed, save what weapons the pilot had.
			if(!teamInfo.GiveBackWeapons)
			{
				for(int i = 0; i < MAX_HARDPOINTS; i++)
					GetObjInfo(Player, ObjectInfoType(Get_Weapon0Config+i), teamInfo.PilotWeapon[i]);

			//	for(int x = 0;x<MAX_HARDPOINTS;x++)
			//		PilotWeaponLocalAmmo[x] = GetCurLocalAmmo(Player, x);
			}
			else
			{
				ReplaceWeapons(Player, teamInfo.PilotWeapon);
				teamInfo.GiveBackWeapons = false;
			}
		}
	}
}

// Toggles Satellite On.
void SatelliteHelper::ToggleSatelliteOn(const int Team)
{
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	if(IsAround(teamInfo.SatelliteOff)) // Remove the current Off switch.
		RemoveObject(teamInfo.SatelliteOff);

	// If there is no On switch, make it.
	if(!IsAround(teamInfo.SatelliteOn))
	{
		if(IsAround(teamInfo.SatelliteLock)) // Remove the Lock, if present.
			RemoveObject(teamInfo.SatelliteLock);

		teamInfo.SatelliteOn = BuildObject("BZCsatviewon", Team, BZ1Helper::Where);
	}
}

// Toggles Satellite Off.
void SatelliteHelper::ToggleSatelliteOff(const int Team)
{
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	// Remove the current Satellite On switch.
	if(IsAround(teamInfo.SatelliteOn))
		RemoveObject(teamInfo.SatelliteOn);

	// If there is no Off switch, make it.
	if(!IsAround(teamInfo.SatelliteOff))
		teamInfo.SatelliteOff = BuildObject("BZCsatviewoff", Team, BZ1Helper::Where);
}

// Locks Satellite.
void SatelliteHelper::LockSatellite(const int Team)
{
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	if(!IsAround(teamInfo.SatelliteLock)) // Unlock Satellite
		teamInfo.SatelliteLock = BuildObject("BZCsatviewlock", Team, BZ1Helper::Where); // Build the Satellite Lock.

	ToggleSatelliteOff(Team); // Force it off.
}

// Unlocks Satellite.
void SatelliteHelper::UnlockSatellite(const int Team)
{
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	if(IsAround(teamInfo.SatelliteLock)) // Unlock Satellite
	{
		RemoveObject(teamInfo.SatelliteLock);
		ToggleSatelliteOff(Team);
	}
}