#include "BZ1Helper.h"
#include "APCHelper.h"
#include "BZCScriptUtils.h"
#include "BuildingHelper.h"
#include <vector>
#include <algorithm>

APCHelper::APCHelper()
{
}
APCHelper::~APCHelper()
{
	 //m_BZ1Helper = NULL;
}

bool APCHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		APCList.resize(size);
		if(size)
			Read(&APCList.front(), sizeof(APCClass)*size);
	}
	return true;
}

bool APCHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<APCClass>::iterator iter = APCList.begin(); iter != APCList.end(); ++iter)
		{
			APCClass &sao = *iter;
			ConvertHandles(&sao.APCObject, 1);
			ConvertHandles(&sao.RefillChoice, 1);
		//	ConvertHandles(&sao.LastTarget, 1);
		}
	}
	return true;
}

bool APCHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(APCList.size());
		Write(&size, 1);
		if(size)
			Write(&APCList.front(), sizeof(APCClass)*size);
	}
	return true;
}

void APCHelper::AddAPCSoldierObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team)
{
	float BestDistanceSquared = FLT_MAX;
	int APCIndex = -1;
	for (std::vector<APCClass>::iterator iter = APCList.begin(); iter != APCList.end(); ++iter)
	{
		float DistanceSquared = GetDistance3DSquared(h, iter->APCObject);
		if((BestDistanceSquared > DistanceSquared) && (h != iter->APCObject) && (GetTeamNum(iter->APCObject) == Team))
		{
			BestDistanceSquared = DistanceSquared;
			APCIndex = iter-APCList.begin();
		}
	}
	// We have a winner? This soldier is close enough to be considered come from an APC.
	if((BestDistanceSquared < Squared(200.0f)) && (APCIndex >= 0))
	{
		Handle APCHandle = APCList[APCIndex].APCObject;
		Handle APCTarget = GetTarget(APCHandle);

		// Tweak it's height if it spanwed on the ground.
		if(BZ1Helper::m_GameVersion <= v13PB65)
		{
			Vector Position = GetPosition(h);
			Vector APCPosition = GetPosition(APCHandle);
			Position.y = APCPosition.y;
			SetVectorPosition(h, Position);
		}

		/*
		if(IsPlayer(APCHandle))
			APCTarget = GetCurrentWho(APCHandle); //GetUserTarget(Team);
		else
			APCTarget = APCList[APCIndex].LastTarget;
		*/

		/*
		Handle TestHandle1 = GetTarget(APCList[APCIndex].APCObject);
		Handle TestHandle2 = GetCurrentWho(APCHandle); //GetUserTarget(Team);
		char TempPrintODF[MAX_ODF_LENGTH] = {0};
		GetObjInfo(TestHandle1, Get_ODF, TempPrintODF);
		char TempPrintODF2[MAX_ODF_LENGTH] = {0};
		GetObjInfo(TestHandle2, Get_ODF, TempPrintODF2);
		FormatLogMessage("APC Target: %08x : %s, CurrentWho: %08x : %s", TestHandle2, TempPrintODF, TestHandle2, TempPrintODF2);
		*/

		int TargetTeam = GetTeamNum(APCTarget);
		float TempWeaponRange = 0;
		float TempAmmoCost = 0;
		// Get this weapon's aiRange to comparison.
		char TempODF3[MAX_ODF_LENGTH] = {0}; // First temp odf, don't wanna modify our stored ODFName.
		char TempODF4[MAX_ODF_LENGTH] = {0}; // Second temp odf, for grabbing out classlabel.
		GetObjInfo(APCHandle, Get_Weapon0ODF, TempODF3);
		if((OpenODF2(TempODF3)) && (GetODFString(TempODF3, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF4))) // If this ODF is inherited, then try that ODF.
		{
			strcat_s(TempODF4, ".odf"); // Add .odf to the end.
			OpenODF2(TempODF4);
		}
		GetODFFloat(TempODF3, TempODF4, "WeaponClass", "aiRange", &TempWeaponRange);

	//	bool TargetAround = IsAround(APCTarget);
		// If this is a PLAYER driven APC, Impose some restrictions: 1: They must be "T" Targeting something, and B: Must be within aiRange of the target.
		if((IsPlayer(APCHandle)) && ((!APCTarget) || (IsTeamAllied(TargetTeam, Team)) || (GetDistance3DSquared(APCHandle, APCTarget) > Squared(TempWeaponRange))))
		{
			GetODFFloat(TempODF1, TempODF2, "GameObjectClass", "maxAmmo", &TempAmmoCost);
			AddLocalAmmo(APCHandle, TempAmmoCost, 0); // Give back the ammo.
			BZ1Helper::Get().DeleteAfterDelay(h);
			return; // We're done here.
		}

		// Order this soldier to attack the target.
		if((APCTarget) && (!IsTeamAllied(TargetTeam, Team)))
		{
			SetIndependence(h, 0); // Don't let the AI do stuff on it's own.
			SetTarget(h, APCTarget);
			Attack(h, APCTarget, 1);
		}
	}
}

void APCHelper::AddAPCObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	for (std::vector<APCClass>::iterator iter = APCList.begin(); iter != APCList.end(); ++iter)
	{
		if(iter->APCObject == h)
			return;
	}

	APCList.resize(APCList.size()+1);
	APCClass &sao = APCList[APCList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(APCClass)); // Zero things out at start.

	// Look for new ODF Parameters. 
	float TempRefillTime = 0.0f;
	GetODFFloat(TempODF1, TempODF2, "APCClass", "reloadTime", &TempRefillTime, 5.0f);
	sao.RefillTime = ConvertToTurns(TempRefillTime);

	SetIndependence(h, 0); // Dont let the AI do stuff on it's own. Just like in BZ1, for balance.

	sao.APCObject = h;
}

void APCHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	APCList.erase(std::remove_if(APCList.begin(), APCList.end(), ShouldRemoveAPC), APCList.end());

	// Loop through all APCs, doing APC stuff. :)
	for (std::vector<APCClass>::iterator iter = APCList.begin(); iter != APCList.end(); ++iter)
	{
		float APCDistanceSquared = FLT_MAX;
		float APCAmmo = 0;
		float APCMaxAmmo = 0;

		// Get the APC's if they're out of ammo, and flag them.
		if((!iter->HasNoPilot) || (IsPlayer(iter->APCObject))) // Only proceed if said object is around, and not "stuck", Or if it's the Player.
		{
			APCAmmo = GetCurLocalAmmo(iter->APCObject, 0); // Pass it's ammo into a tempvar.
			APCMaxAmmo = GetMaxLocalAmmo(iter->APCObject, 0);

			// Check to see if it's got a weapon. Don't proceed if it doesn't. Warn user with console spam :D
			if(APCMaxAmmo == 0)
			{
				if(Logging)
					FormatLogMessage("APC Has Invalid Weapon! Team: %d Index: %d", GetTeamNum(iter->APCObject), iter - APCList.begin()); 

				//iter->IsStuck = true;
				continue;
			}
			else if(((IsPlayer(iter->APCObject) && (APCAmmo < APCMaxAmmo)) || (APCAmmo == 0)) && (!iter->IsRefilling)) // || (IsPlayer(iter->APCObject)))) // If it's out of ammo, and not flagged as refilling.
			{
				int Team = GetTeamNum(iter->APCObject); // We needs this.

			//	iter->LastTarget = GetCurrentWho(iter->APCObject); //GetTarget(iter->APCObject); // Save it's last Target to tell soldiers to attack that specifically.

				if(Logging)
					FormatLogMessage("APC Is out of ammo! Team: %d Index: %d. Ammo is: %d", Team, iter - APCList.begin(), APCAmmo); 

				// If this APC's ammo WAS full, grab the deploy sound out and play it.
				if((!iter->PlayedSoundFirst) && (APCAmmo == 0))
				{
					iter->PlayedSoundFirst = true; // Flag as having played it.

					char SoundFile[MAX_ODF_LENGTH] = {0};
					GetODFString(iter->APCObject, "CraftClass", "deployedMsg", MAX_ODF_LENGTH, SoundFile); // Grab the sound file, with allowance for a large filename.

					if(Logging)
						FormatLogMessage("Sound Is: %s", SoundFile); 

					if((SoundFile[0]) && (GetLocalPlayerTeamNumber() == Team)) // If This APC belongs to YOU!
					{
						if(DoesFileExist(SoundFile))
							AudioMessage(SoundFile);
						else
							FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix deployMsg in ODF: %s", SoundFile, GetODFName(iter->APCObject)); 
					}
				}

			//	APCDistanceSquared = GetDistance3DSquared(iter->APCObject, (m_APCRefill[Team])[0]); // Compare initial distance. 
			//	m_APCRefillChoice[Team][i] = m_APCRefill[Team][0]; // Set this here, if first is closest.

				iter->RefillChoice = 0;

				// Loop over all possible refills, find the closest one.
				for(int j = 0; j < BuildingHelper::Get().GetNumBuildingObjects(); j++) // We found an APC that's out of ammo, Loop over all APC Refill points for this team. 
				{
					BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(j);
					Handle TempTarget = buildingInfo.BuildingObject;
					float TempDistSquared = GetDistance3DSquared(GetPosition(iter->APCObject), buildingInfo.Position);
					int t = buildingInfo.CurTeamNum; //!-- Add TeamFilterMask for APC Reloading. -GBD
					if((buildingInfo.IsAPCRefill) && (t != 0) && ((t == Team) || (IsTeamAllied(t, Team))) && (APCDistanceSquared > TempDistSquared)) // Team is allied, and this is a refill point building.
					{
						iter->RefillChoice = TempTarget;
						APCDistanceSquared = TempDistSquared; // Save new distance. 

						if(Logging)
							FormatLogMessage("APC on Team: %d Index: %d Chose Refill point on Team: %d Index: %d Distance: %d", Team, iter - APCList.begin(), t, j, sqrtf(APCDistanceSquared)); 
					}
				}

				if(iter->RefillChoice) // If the handle we found isn't null/nothing, then...
				{
					if(Logging)
						FormatLogMessage("APC on Team: %d Index: %d Was set to refill at a refill point on Team: %d.", Team, iter - APCList.begin(), GetTeamNum(iter->RefillChoice)); 

					if(!IsPlayer(iter->APCObject)) // If this object is NOT the player.
						Goto(iter->APCObject, iter->RefillChoice, 1);

				//	SetIndependence(iter->APCObject, 0); // Don't let the AI do stuff on it's own? 
					iter->IsRefilling = true;
				}
				/*
				else // If we got here, that means there's either no allied teams, or no place to refill on allied teams. We're out of luck and will have to wait.
				{
					if((!iter->IsStuck) && (!iter->IsRefilling))
					{
						iter->HasNoRefill = true; // Stuck and refilling.

						if(!IsPlayer(iter->APCObject));
							Stop(iter->APCObject, 1); // Take Command Away.
				//		SetIndependence(iter->APCObject, 0); // Don't let the AI do stuff on it's own? 

						if(Logging)
							FormatLogMessage("APC on Team: %d Index: %d Has found no Refill of it's own, or allied Teams.", Team, iter); 
					}
				}
				*/
			}
		}

		/*
		//This APC was flagged as stuck, but isn't going to the refil point. This can only mean there are none. Look for some.
		if((iter->HasNoRefill) && (!iter->IsRefilling))
		{
			// If there are any on this team.... and the APC has a pilot. 
			if((m_NumAPCRefillObjects[Team] > 0) && (IsAround((m_APCRefill[Team])[0])))
			{
				if(Logging)
					FormatLogMessage("New Refill Points Available on Team: %d.", Team); 

				m_APCHasNoPlaceToGo[Team][i] = false; // Throw this back on the pile.
			}
			else // Look on allies....
			{
				// Loop through all teams. 
				for(int AllyTeam = 0; AllyTeam < MAX_TEAMS; AllyTeam++)
				{
					// If this team is allied with us, and they have any refill points. 
					if((Team != AllyTeam) && (IsTeamAllied(Team, AllyTeam)) && (m_NumAPCRefillObjects[AllyTeam] > 0) &&  (IsAround((m_APCRefill[AllyTeam])[0])))
					{
						if(Logging)
							FormatLogMessage("New Refill Points Available on Allied Team: %d.", AllyTeam); 

						m_APCHasNoPlaceToGo[Team][i] = false; // Throw this back on the pile.
					}
				}
			}
		}
		*/

		if(iter->IsRefilling) // If it is refilling, check distance, and if it gets sniped en route.
		{
			if(!IsAround(iter->RefillChoice)) // If their destination went poof, do this shit again...-.- 
			{
				iter->IsRefilling = false; // Throw it back onto the pile...

				if(!IsPlayer(iter->APCObject))
					Stop(iter->APCObject, 1); // Take it out of command anyway, until there is one available.

				if(Logging)
					FormatLogMessage("APC on Team: %d Index: %d Had Refill point Blow up on them.", GetTeamNum(iter->APCObject), iter - APCList.begin()); 

				continue;
			}

			// We switched ships, or somebody sniped us. Uh Oh...
			if((!iter->HasNoPilot) && (!IsNotDeadAndPilot2(iter->APCObject)) && (!IsPlayer(iter->APCObject)))
			{
				if(Logging)
					FormatLogMessage("APC Has No AI Pilot! Team: %d Index: %d.", GetTeamNum(iter->APCObject), iter - APCList.begin()); 

			//	iter->IsStuck = true;
				iter->HasNoPilot = true; // Oh Noes!
			}

			// This APC is back in action! We need to reset it's course.
			if((iter->HasNoPilot) && ((IsNotDeadAndPilot2(iter->APCObject)) || (IsPlayer(iter->APCObject)))) //&& (iter->IsStuck))
			{
				if(Logging)
					FormatLogMessage("APC Has a new pilot Team: %d Index: %d", GetTeamNum(iter->APCObject), iter - APCList.begin()); 

			//	iter->IsStuck = false;
				iter->HasNoPilot = false; // Oh Yaes!

				Stop(iter->APCObject, 1); // Take away F slot control.
			//	SetIndependence(iter->APCObject, 0); // Don't let the AI do stuff on it's own?	
				iter->IsRefilling = false; // Throw it back onto the pile, to pick a new destination. It could have been sniped a long time ago.
			}

			// Get the distance each tick. 
			APCDistanceSquared = GetDistance3DSquared(iter->APCObject, iter->RefillChoice);

			if(iter->RefillingTimer > 0)
			{
				Stop(iter->APCObject, 1); // Make it stop when close, sit for 5 seconds. 
				// Force it to wait 5 seconds before refilling. Players must STAY within 50m, AI does not.
				if((!IsPlayer(iter->APCObject)) || ((IsPlayer(iter->APCObject) && (APCDistanceSquared < Squared(50.0f)))))
					++iter->RefillingTimer;

				//if((m_APCRefillTimer[Team])[i] > (5.0f * gameTPS))m_APCRefillTime
				if(iter->RefillingTimer > (iter->RefillTime)) //(5.0f * gameTPS))
				{
					AddLocalAmmo(iter->APCObject, GetMaxLocalAmmo(iter->APCObject, 0), 0); // Fill her up, George!
					Stop(iter->APCObject, 0); // Restore F group/command.
					//SetIndependence(iter->APCObject, 1); // Restore AI's own rational thought.
			
					iter->IsRefilling = false; // Reset Flag.
					iter->PlayedSoundFirst = false; // Set the sound to reset.
					iter->RefillingTimer = 0; // Reset reload timer.
				//	iter->LastTarget = 0; // Clear the Last Target.

					if(Logging)
						FormatLogMessage("APC On Team %d Index %d Has Arrived. Ammo is: %d", GetTeamNum(iter->APCObject), iter - APCList.begin(), GetCurLocalAmmo(iter->APCObject, 0)); 

					continue;
				}
			}
			// They have arrived. Start the timer.
			if((iter->RefillingTimer == 0) && (APCDistanceSquared < Squared(50.0f)))
				++iter->RefillingTimer;
		}
	}
}

void APCHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	int Team = GetTeamNum(victimHandle);
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	// If this is not a Human controlled team. 
	if(!GetPlayerHandle(Team))
	{
		for (std::vector<APCClass>::iterator iter = APCList.begin(); iter != APCList.end(); ++iter)
		{
			if(victimHandle == iter->APCObject)
			{
				// If this is not currently Refilling.
				if((!iter->IsRefilling) && (teamInfo.m_CPUAIDifficulty) && (GetCurrentWho(victimHandle) != shooterHandle))
				{
					Attack(victimHandle, shooterHandle);
				//	iter->LastTarget = shooterHandle;
				}
				return; // Were done, exit early. 
			}
		}
	}
}