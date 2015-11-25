#include "BZ1Helper.h"
#include "DispatchHelper.h"
#include "ProductionClassHelper.h"
#include "BuildingHelper.h"
#include "ScrapHelper.h"
#include "MorphObjectHelper.h"
#include "UtilityShipHelper.h"
#include "BZCScriptUtils.h"
#include "math.h"
#include <algorithm>

DispatchHelper *DispatchHelper::m_DispatchHelper;

DispatchHelper::DispatchHelper()
{
	m_DispatchHelper = this;
}
DispatchHelper::~DispatchHelper()
{
	 m_DispatchHelper = NULL;
}

bool DispatchHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		DispatchList.resize(size);
		if(size)
			Read(&DispatchList.front(), sizeof(DispatchClass)*size);
	}
	return true;
}

bool DispatchHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
		{
			DispatchClass &sao = *iter;
			ConvertHandles(&sao.DispatchObject, 1);
			ConvertHandles(&sao.DispatchTarget, 1);
			ConvertHandles(&sao.FleeTarget, 1);
		}
	}
	return true;
}

bool DispatchHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(DispatchList.size());
		Write(&size, 1);
		if(size)
			Write(&DispatchList.front(), sizeof(DispatchClass)*size);
	}
	return true;
}

bool DispatchHelper::AddToDispatch(const Handle h, const float IdleTime, const bool SkipAIPCheck, const int CloakPriority, const bool CanFlee, const bool CloakNow, const bool Hunt, const bool CanGetService) //, Handle Target)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false; // Invalid Handle, bail.

	unsigned int ObjClassHash = Hash(ObjClass);

	char TempODF1[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	GetAndOpenODFNames(h, TempODF1, TempODF2);

	bool DoDispatch = true;
	// Grab out the ODF, and look inside it. Pull out all vars we might need. -GBD
	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(TempODF1, TempODF2, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);
	unsigned int BZCClassHash = Hash(BZCClass);
	// Flag to disable dispatch entirely.
	GetODFBool(TempODF1, TempODF2, "CraftClass", "DoDLLDispatch", &DoDispatch, true);

	const char * Label = GetLabel(h);

	if((Label) && (_strnicmp(Label,"nodispatch",10) == 0))
		DoDispatch = false;

	// Skip this object if it isn't a valid class to dispatch. (If it doesn't do dispatchy stuff)
	if(((ObjClassHash != CLASS_AIR) && (ObjClassHash != CLASS_MORPHTANK) && (ObjClassHash != CLASS_WINGMAN) && 
		(ObjClassHash != CLASS_ASSAULTHOVER) && (ObjClassHash != CLASS_ASSAULTTANK) && (ObjClassHash != CLASS_SAV) &&
		(ObjClassHash != CLASS_TURRETTANK) && (ObjClassHash != CLASS_MINELAYER) && (ObjClassHash != CLASS_WALKER) && 
		//(ObjClassHash != CLASS_SERVICE) && (ObjClassHash != CLASS_SERVICEH) && // !!!- SERVICER CODE NOT YET WRITTEN! -GBD
		(ObjClassHash != CLASS_ARTILLERY) && (ObjClassHash != CLASS_HOWITZER)) || // Include Howitzers for AI sending them ammo.
		(BZCClassHash == BZCCLASS_APC) || // Skip APCs.
		(!DoDispatch)) // Don't dispatch this object esplicity.
		return false; // Skip it.

	// If we got this far, DoDispatch is true.
	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
			return false;
	}

	DispatchList.resize(DispatchList.size()+1);
	DispatchClass &sao = DispatchList[DispatchList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(DispatchClass)); // Zero things out at start.

	// Pull out this unit's _config.odf. 
	const char *BaseODFName = GetConfigODF(h);

	// Does this unit do stuff even if no AIP is running?
	sao.SkipAIPCheck = SkipAIPCheck;

	int Team = GetTeamNum(h);
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);
	bool AICloaker = true;
	float HuntChance = 0.0f;
	float PatrolChance = 0.0f;
	float SniperChance = 0.0f;
	float EscortChance = 0.0f;

	if((BaseODFName) && (OpenODF2(BaseODFName)))
	{
		for(int i = 4; i >= 0; i--)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			if(teamInfo.m_CPUAIDifficulty == 1)
				sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 2)
				sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 3)
				sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 4)
				sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

			if(GetODFFloat(BaseODFName, DesiredValue, "AIHuntChance", &HuntChance, 0.0f)) // 10% chance default.
				break;
		}
		for(int i = 4; i >= 0; i--)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			if(teamInfo.m_CPUAIDifficulty == 1)
				sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 2)
				sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 3)
				sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 4)
				sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

			if(GetODFFloat(BaseODFName, DesiredValue, "AIPatrolChance", &PatrolChance, 0.0f)) // 10% chance default.
				break;
		}
		for(int i = 4; i >= 0; i--)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			if(teamInfo.m_CPUAIDifficulty == 1)
				sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 2)
				sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 3)
				sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 4)
				sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

			if(GetODFFloat(BaseODFName, DesiredValue, "AISniperChance", &SniperChance, 0.0f)) // 10% chance default.
				break;
		}
		for(int i = 4; i >= 0; i--)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			if(teamInfo.m_CPUAIDifficulty == 1)
				sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 2)
				sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 3)
				sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
			else if(teamInfo.m_CPUAIDifficulty == 4)
				sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

			if(GetODFFloat(BaseODFName, DesiredValue, "AIEscortChance", &EscortChance, 0.0f)) // 10% chance default.
				break;
		}
	}

	HuntChance = clamp(HuntChance, 0.0f, 1.0f);
	PatrolChance = clamp(PatrolChance, 0.0f, 1.0f);
	SniperChance = clamp(SniperChance, 0.0f, 1.0f);
	EscortChance = clamp(EscortChance, 0.0f, 1.0f);

	// Decide which type of unit this will be.
	if(ObjClassHash == CLASS_TURRETTANK)
	{
		sao.DispatchType = DispatchType_Turret;
	}
	else if(BZCClassHash == BZCCLASS_MINELAYER)
	{
		sao.DispatchType = DispatchType_Minelayer;
	}
	else if((ObjClassHash == CLASS_ARTILLERY) || (ObjClassHash == CLASS_HOWITZER))
	{
		sao.DispatchType = DispatchType_Howitzer; // Only influences how these ships get Reload/Repair atm.
	}
	else if ((Hunt) || ((Label) && (_strnicmp(Label,"hunter",6) == 0)))
	{
		sao.DispatchType = DispatchType_Hunter;
	}
	else if ((Label) && (_strnicmp(Label,"patrol",6) == 0))
	{
		sao.DispatchType = DispatchType_Patrol;
	}
	else if ((Label) && (_strnicmp(Label,"escort",6) == 0))
	{
		sao.DispatchType = DispatchType_Escort;
	}
	else // If the above didn't catch anything, pick randomly.
	{
		int Choice = GetRandomInt(2);

		switch (Choice)
		{
			case  0: // First try Hunter.
			{
				if(GetRandomFloat(1.0) < HuntChance)
					sao.DispatchType = DispatchType_Hunter;
				else if(GetRandomFloat(1.0) < PatrolChance)
					sao.DispatchType = DispatchType_Patrol;
				else if(GetRandomFloat(1.0) < EscortChance)
					sao.DispatchType = DispatchType_Escort;
			}
			break;
			case  1: // Next try Patrol.
			{
				if(GetRandomFloat(1.0) < PatrolChance)
					sao.DispatchType = DispatchType_Patrol;
				else if(GetRandomFloat(1.0) < HuntChance)
					sao.DispatchType = DispatchType_Hunter;
				else if(GetRandomFloat(1.0) < EscortChance)
					sao.DispatchType = DispatchType_Escort;
			}
			break;
			case  2: // Finally, try Escort.
			{
				if(GetRandomFloat(1.0) < EscortChance)
					sao.DispatchType = DispatchType_Escort;
				else if(GetRandomFloat(1.0) < HuntChance)
					sao.DispatchType = DispatchType_Hunter;
				else if(GetRandomFloat(1.0) < PatrolChance)
					sao.DispatchType = DispatchType_Patrol;
			}
			break;
		}
	}
	/*
	//!!-- Move to DispatchHelper !IsDispatched, so that pre-placed objects on maps can be dispatched.
	else if(((teamInfo.m_CPUMaxHuntUnits > 0) && (teamInfo.m_CPUNumHuntUnits < teamInfo.m_CPUMaxHuntUnits) && (GetRandomFloat(1.0) < HuntChance)) || (Hunt))
	{
		sao.IsHunter = true;
		++teamInfo.m_CPUNumHuntUnits;
	}
	else if((teamInfo.m_CPUMaxPatrolUnits > 0) && (teamInfo.m_CPUNumPatrolUnits < teamInfo.m_CPUMaxPatrolUnits) && (GetRandomFloat(1.0) < PatrolChance))
	{
		sao.IsPatrol = true;
		++teamInfo.m_CPUNumPatrolUnits;
	}
	else if((teamInfo.m_CPUMaxEscortUnits > 0) && (teamInfo.m_CPUNumEscortUnits < teamInfo.m_CPUMaxEscortUnits) && (GetRandomFloat(1.0) < EscortChance))
	{
		// Pick a random object to Escort.
		sao.DispatchTarget = UtilityShipHelper::Get().GetEscortTarget();

		if(sao.DispatchTarget)
		{
			//Follow(h, sao.DispatchTarget);
			Defend2(h, sao.DispatchTarget);
			sao.IsEscorting = true;
			sao.IsDispatched = true;
			++teamInfo.m_CPUNumEscortUnits;
		}
	}
	*/
	//!!--

	// Will it Snipe?
	if(GetRandomFloat(1.0f) < SniperChance)
	{
		char PilotODF[MAX_ODF_LENGTH] = {0};
		sprintf_s(PilotODF, "%csuser", GetRace(h));
		SetPilotClass(h, PilotODF);
	}

	// Claoker?
	GetODFBool(TempODF1, TempODF2, "MorphTankClass", "HiddenWhenMorphed", &sao.IsCloaker, false);
	// Flag to disable AI cloaking.
	GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanAICloak", &AICloaker, true);
	if(!AICloaker)
		sao.IsCloaker = false;

	// Get the deploy time for how long it takes to cloak.
	GetODFFloat(TempODF1, TempODF2, "DeployableClass", "timeDeploy", &sao.TimeDeploy, 5.0f);

	sao.EngageRange = GetEngageRange(h);

	// Does it flee? // Only these classes can flee. (hovercraft derived)
	if( (ObjClassHash == CLASS_WINGMAN) || 
		(ObjClassHash == CLASS_MORPHTANK) || 
		(ObjClassHash == CLASS_ASSAULTHOVER) ||
		(ObjClassHash == CLASS_AIR))  // || (ObjClassHash == CLASS_ASSAULTTANK) || (ObjClassHash == CLASS_WALKER))
	{
		GetODFBool(TempODF1, TempODF2, "CraftClass", "CanAIFlee", &sao.CanFlee, CanFlee);
		if(sao.CanFlee) // Flee Minimum Health Lost Ratio.
			GetODFFloat(TempODF1, TempODF2, "CraftClass", "AIFleeHealthRatio", &sao.FleeHealthRatio, 0.75f);
	}
	else
	{
		sao.CanFlee = false; // Nope, doesn't Flee.
	}


	// If the AIP is set to UnitsService, or the dispatch caller specifically set it.
	if((teamInfo.m_CPUUnitsCanService) || (CanGetService))
	{
		// Can this unit automatically repair?
		GetODFBool(TempODF1, TempODF2, "CraftClass", "CanAIRepair", &sao.CanGetRepair, GetMaxHealth(h));

		// Can this unit automatically reload?
		GetODFBool(TempODF1, TempODF2, "CraftClass", "CanAIReload", &sao.CanGetReload, GetMaxAmmo(h));

		/*
		if(GetMaxHealth(h) <= 0)
			sao.CanGetRepair = false; // Nope, you can't do it.

		if(GetMaxAmmo(h) <= 0)
			sao.CanGetReload = false; // Nope, you can't do it.
		*/

		// Get the Ratio at which it will go to get repairs.
		if((BaseODFName) && (OpenODF2(BaseODFName)))
		{
			for(int i = 4; i >= 0; i--)
			{
				char DesiredValue[MAX_ODF_LENGTH] = {0};
				if(teamInfo.m_CPUAIDifficulty == 1)
					sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
				else if(teamInfo.m_CPUAIDifficulty == 2)
					sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
				else if(teamInfo.m_CPUAIDifficulty == 3)
					sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
				else if(teamInfo.m_CPUAIDifficulty == 4)
					sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

				if(GetODFFloat(BaseODFName, DesiredValue, "AIRepairHealthRatio", &sao.GetRepairRatio, 0.15f)) // 15% chance default.
					break;
			}

			// Read the AIServiceChance per difficulty setting. 
			for(int i = 4; i >= 0; i--) // Start at weaponSlot 5 and go up. 
			{
				char DesiredValue[MAX_ODF_LENGTH] = {0};
				if(teamInfo.m_CPUAIDifficulty == 1)
					sprintf_s(DesiredValue, "EasyWeaponSlot%d", i+1);
				else if(teamInfo.m_CPUAIDifficulty == 2)
					sprintf_s(DesiredValue, "MediumWeaponSlot%d", i+1);
				else if(teamInfo.m_CPUAIDifficulty == 3)
					sprintf_s(DesiredValue, "HardWeaponSlot%d", i+1);
				else if(teamInfo.m_CPUAIDifficulty == 4)
					sprintf_s(DesiredValue, "ExtremeWeaponSlot%d", i+1);

				if(GetODFFloat(BaseODFName, DesiredValue, "AIServiceChance", &sao.ServiceChance, 0.0f)) // 0% chance default.
					break;
			}

			ReadChanceValues(BaseODFName, "CloakChances", sao.CloakChance);
		}
	//	GetODFFloat(h, "GameObjectClass", "AIRepairHealthRatio", &sao.GetRepairRatio, 0.15f);

		/*
		// If this unit adds ammo, override canAIReload.
		bool AddAmmo = false;
		if(GetODFBool(h, "GameObjectClass", "addAmmo", &AddAmmo)) // Grab the value.
		{
			// Got it.
		}
		else if(GetODFBool(h, "GameObjectClass", "aiAddAmmo", &AddAmmo)) // Grab the value.
		{
			// Got it.
		}
		if((GetMaxAmmo(h) <= 0) || (AddAmmo))
			sao.CanGetReload = false; // Nope, you can't do it.

		// If this unit adds health, override CanAIRepair.
		bool AddHealth = false;
		if(GetODFBool(h, "GameObjectClass", "addHealth", &AddHealth)) // Grab the value.
		{
			// Got it.
		}
		else if(GetODFBool(h, "GameObjectClass", "aiAddHealth", &AddHealth)) // Grab the value.
		{
			// Got it.
		}
		if((GetMaxHealth(h) <= 0) || (AddHealth))
			sao.CanGetRepair = false; // Nope, you can't do it.
		*/
	}

	if(_stricmp(BZCClass, "morphobject") == 0)
	{
		sao.IsMorpher = true;
		GetODFBool(TempODF1, TempODF2, "MorphObjectClass", "MorphIsAssault", &sao.MorphIsAssault, true); // !!!-- Write me, IsAssault on target flag == 1 and This true, then morph it to attack. -GBD
	}

	sao.GetRepairRatio = clamp(sao.GetRepairRatio, 0.0f, 1.0f);
	sao.ServiceChance = clamp(sao.ServiceChance, 0.0f, 1.0f);

	sao.FleeHealthValue = GetCurHealth(h);
	sao.DispatchObject = h; // Add the object to the list.
	sao.IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(IdleTime));
	sao.CloakPriority = CloakPriority;

	if(sao.IsCloaker)
	{
		if(CloakNow)
			Cloak(h, CloakPriority);
		else
			sao.CloakIdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(IdleTime));
	}

	return true;
}

void DispatchHelper::DeleteDispatchObject(const Handle h, const int Team)
{
	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
		{
			switch (iter->DispatchType)
			{
			case DispatchType_Patrol:
				--teamInfo.m_CPUNumPatrolUnits;
				break;
			case DispatchType_Hunter:
				--teamInfo.m_CPUNumHuntUnits;
				break;
			case DispatchType_Escort:
				--teamInfo.m_CPUNumEscortUnits;
				break;
			}
			break;
		}
	}

}

void DispatchHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	// These should be stationary for this run.
	int numObjects = BZ1Helper::Get().GetNumHandles();
	int numBuildings = BuildingHelper::Get().GetNumBuildingObjects();

	for(int Team = 0; Team<MAX_TEAMS; Team++)
	{
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);
		if((teamInfo.TeamAIPFile[0]) && (IsAround(teamInfo.Slf)) && (BZ1Helper::m_TurnCounter % ConvertToTurns(30.0f) == 0))
		{
			float RandomChance = GetRandomFloat(1.0f);
			if(RandomChance < teamInfo.m_CPUDayWreckerChance)
			{
				char DayWreckerODF[MAX_ODF_LENGTH] = {0};
				int DayWreckerCost = 0;
				GetBuildItem(teamInfo.Slf, DayWreckerODF, (int*)DayWreckerCost, 4, "daywrecker");

				// Daywrecker Code.
				if((GetScrap(Team) >= DayWreckerCost) && (DayWreckerODF[0]) && (DoesODFExist(DayWreckerODF)))
				{
					// This is a valid target team! Lock onto one of their stuffs...
					if(BZ1Helper::Get().GetNumObjects(Team, Enemies, Building) > 0)
					{
						// Choose a target.
						Handle TempTarget = BZ1Helper::Get().GetRandomObject(Team, Enemies, Building);

						for(int i = 0; i < numObjects; i++)
						{
							BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(i);
							Handle TempH = objectInfo.ObjectHandle;
							if((objectInfo.CurTeamNum == Team) && (GetDistance(TempH, TempTarget) < objectInfo.EngageRange)) //&& (!IsTeamAllied(Team, GetTeamNum(BZ1Helper::Get().RefBuildingHandle(RandomTarget)))))
							{
								Vector TargetPos = GetPosition(TempTarget);
								TargetPos.y += 350.0f;
								BuildObject(DayWreckerODF, Team, TargetPos);
								AddScrap(Team, -DayWreckerCost); // Take away the cost.
								break;
							}
						}
					}
				}
			}
		}
	}

	// pre-clean the vector
	DispatchList.erase(std::remove_if(DispatchList.begin(), DispatchList.end(), ShouldRemoveDispatch), DispatchList.end());

	// Turrets, randomly go to either A: Near Our Base, B: Scrap Fields, or C: Near Enemy Base. Units, just pick a random target.
	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		int DispatchIndex = iter - DispatchList.begin(); 
		int Team = GetTeamNum(iter->DispatchObject);
		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		// If it's around, not a human, and not on a human controlled team. 
		if(!IsPlayer(iter->DispatchObject)) //&& (GetPlayerHandle(Team) == 0))
		{
			if(!GetPlayerHandle(Team)) // No player on this team.
			{
				// This unit hasn't been given orders.
				switch (iter->DispatchState)
				{
				case DispatchState_Idle:
					{
						// If it's idle timer's up, and it's not currently doing something, lets figure out what to tell it to do.
						if( (BZ1Helper::m_TurnCounter >= iter->IdleTime) && 
							(GetCurrentCommand(iter->DispatchObject) == CMD_NONE) && 
							((iter->SkipAIPCheck) || (teamInfo.m_CPUDoDispatch)))
						{
							Handle Enemy = GetNearestEnemy(iter->DispatchObject);

							switch (iter->DispatchType)
							{
							case DispatchType_Turret:
								{
									// If it's a Turret, and there are no enemies nearby.
									if(GetDistance(iter->DispatchObject, Enemy) > iter->EngageRange + 50.0f)
									{
										// Friendly: Find farthest away building, get random building within 50-100m of that range, get position further from it, and go to.

										// Scrap: Randomly pick a piece of scrap to getpositionnear(), min 10, max 50. 

										// Enemy: Find the closest enemy, randomly pick until we find one within randomfloat of 50-100 of that range, (close to enemy front
										// lines?) and go to it. Stop outside it's engage range, + randomfloat of 50-150m. 
										/*
										Type 0: Friendly Base Defence.
										- Method 0: Furthest away building.
										- Method 1: Random friendly building.
										- Method 2: Random base slot building (recy/fact/armory)
										Type 1: Goto Scrap.
										- Method 0: Closest.
										- Method 1: Random.
										- Method 2: Farthest.
										Type 2: Siege Enemy Base.
										- Method 0: Closest enemy building.
										- Method 1: Random Enemy building.
										- Method 2: Enemy base slot building.
										*/

										int DispatchMode = GetRandomInt(2); // floor(GetRandomFloat(3.0f)));
				  
										Handle TempTarget = 0;
										float TempDist = 0.0f;
										int Random = 0;
										int RandomTeam = 0;
										int DispatchMethod = GetRandomInt(2);

										if(Logging)
											FormatLogMessage("Index is: %d, DispatchMode is: %d, DispatchMethod is: %d", iter-DispatchList.begin(), DispatchMode, DispatchMethod); 

										switch (DispatchMode)
										{
										case 0: // Friendly Base Guard Duty. Find a spot near edge of base.
											{
												iter->DispatchTarget = 0; // Don't save a target to compare distance to for base defence.
												TempDist = 0.0f; //Start off small, we're looking for further away.

												// Choose the type of defence selection. A: Furthest, B: Random Building, or C: Random Production Unit.
												switch (DispatchMethod)
												{
												case  0: // Select based on Furthest Away.
													{
														for(int b = 0; b < numBuildings; b++)
														{
															BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(b);
															Handle TempBuilding = buildingInfo.BuildingObject;

															// This is a more desirable base "edge".
															if((GetDistance(iter->DispatchObject, TempBuilding) > TempDist) && (GetMaxHealth(TempBuilding) > 0) && (buildingInfo.CurTeamNum == Team))
															{
																TempTarget = TempBuilding;
																TempDist = GetDistance(iter->DispatchObject, TempBuilding); // Save it for next loop.
															}
														}
													}
													break;
												case 1: // Select Random Friendly Building.
													{
														// Try <NumberofObjects> times to pick a building on our team.
														for(int Tries = 0; Tries < numBuildings; Tries++)
														{
															Random = GetRandomInt(numBuildings-1);
															BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(Random);
															Handle TempBuilding = buildingInfo.BuildingObject;

															if((Team == buildingInfo.CurTeamNum) && (GetMaxHealth(TempBuilding) > 0))
															{
																TempTarget = TempBuilding;
																break;
															}
														}
													}
													break;
												case 2: // Choose from Friendly Production Units.
													{
														if(teamInfo.Recy)
															TempTarget = teamInfo.Recy;
														else if(teamInfo.Muf)
															TempTarget = teamInfo.Muf;
														else if(teamInfo.Cnst)
															TempTarget = teamInfo.Cnst;
														else if(teamInfo.Slf)
															TempTarget = teamInfo.Slf;
														else // No production units? Pick random then.
														{
															for(int Tries = 0; Tries < numBuildings; Tries++)
															{
																Random = GetRandomInt(numBuildings-1);
																BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(Random);
																Handle TempBuilding = buildingInfo.BuildingObject;
																if((Team == buildingInfo.CurTeamNum) && (GetMaxHealth(TempBuilding) > 0))
																{
																	TempTarget = TempBuilding;
																	break;
																}
															}
														}
													}
													break;
												}

												if(TempTarget) // IF the above found something valid.
												{
													/*
													// Now that we have the furthest building, lets spice it up so all our turrets don't go to the same place.
													TempDist = 200.0f; // 100.0f;
													for(int b = 0; b < numBuildings; b++)
													{
														BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(b);
														Handle TempRandomTarget = buildingInfo.BuildingObject;

														// This building is nearby our goal, lets try to find a position farther away from it.
														if( (buildingInfo.CanBeIdleVictim) && // It can be an Idle Victum.
															//(IsAround(TempRandomTarget)) && //It's around.
															( (numBuildings == 1) || (TempRandomTarget != TempTarget) ) && // It's not itself, or there's only one.
															(GetMaxHealth(TempRandomTarget) > 0) && // It can be a thing.
															(Team == buildingInfo.CurTeamNum) && // Team Matches.
															(GetDistance(TempTarget,TempRandomTarget) <= TempDist)) //It's close enough to our target.
														{
															// Choose a position further away from the current target, within tolerance.
															iter->DispatchSpot = GetPositionNearOrFar(iter->DispatchObject, TempRandomTarget, 50.0f, 150.0f, false);

															if(Logging)
																FormatLogMessage("Index is: %d Type is: %d Method is: %d Spot is: %f %f %f Target Is: %s, Distance is: %f", iter-DispatchList.begin(), DispatchState, DispatchMethod, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z, GetODFName(TempRandomTarget), GetDistance(TempTarget,TempRandomTarget)); 

															Goto(iter->DispatchObject,iter->DispatchSpot,0);
															iter->RandomDist = GetRandomFloat(10.0f) + 10.0f;
															iter->Timeout = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
															iter->IsDispatched = true;

															break;
														}
													} // End loop over possible nearby buildings.
													*/

													iter->DispatchTarget = TempTarget;
													iter->DispatchSpot = GetPositionNearOrFar(iter->DispatchObject, iter->DispatchTarget, 50.0f, 250.0f, false);
													Goto(iter->DispatchObject,iter->DispatchSpot, 0);
													iter->RandomDist = GetRandomFloat(10.0f) + 10.0f;
													iter->IdleTime = 0; // Flag this to force one-time action.
													iter->DispatchState = DispatchState_Goto;

													if(Logging)
														FormatLogMessage("Index is: %d Mode is: %d Method is: %d Spot is: %f %f %f Target Is: %s", iter-DispatchList.begin(), DispatchMode, DispatchMethod, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z, GetODFName(TempTarget)); 
												}
											}
											break;
										case 1: // Scrap Field, Go to a position "near" a Random piece of Scrap. :)
											{
												TempDist = FLT_MAX; //Start off big, we're looking for closest.

												switch (DispatchMethod)
												{
												case  0: // Select based on Closest.
													{
														for(int b = 0; b < ScrapHelper::Get().GetNumScrapObjects(); b++)
														{
															Handle ScrapPiece = ScrapHelper::Get().RefScrapHandle(b);

															// This scrap is closer.
															if((GetDistance(iter->DispatchObject,ScrapHelper::Get().RefScrapHandle(b)) < TempDist))
															{
																iter->DispatchTarget = ScrapPiece;
																TempDist = GetDistance(iter->DispatchObject, ScrapPiece); // Save it for next loop.
															}
														}
													}
													break;
												case 1: // Select Random Scrap to go to.
													{
														Random = GetRandomInt(ScrapHelper::Get().GetNumScrapObjects()-1);
														iter->DispatchTarget = 0;

														if(ScrapHelper::Get().GetNumScrapObjects() > 0)
														{
															Handle ScrapPiece = ScrapHelper::Get().RefScrapHandle(Random);

															//if(IsAround(ScrapPiece))
															iter->DispatchTarget = ScrapPiece;
														}
													}
													break;
												case 2: // Choose based on furthest away.
													{
														TempDist = 0.0f; // Start off small, we're looking for Further away.
														for(int b = 0; b < ScrapHelper::Get().GetNumScrapObjects(); b++)
														{
															Handle ScrapPiece = ScrapHelper::Get().RefScrapHandle(b);

															// This scrap is further away.
															if((GetDistance(iter->DispatchObject, ScrapPiece) > TempDist))
															{
																iter->DispatchTarget = ScrapPiece;
																TempDist = GetDistance(iter->DispatchObject, ScrapPiece); // Save it for next loop.
															}
														}
													}
													break;
												}

												iter->DispatchSpot = GetPositionNear(GetPosition(iter->DispatchTarget), 10.0f, 50.0f);

												if(Logging)
													FormatLogMessage("Index is: %d Mode is: %d Method is: %d Spot is: %f %f %f", iter-DispatchList.begin(), DispatchMode, DispatchMethod, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z); 

												if(IsNullVector(iter->DispatchSpot))
												{
													// Pick a random object.
													iter->DispatchTarget = BZ1Helper::Get().GetRandomObject();
													iter->DispatchSpot = GetPositionNear(GetPosition(iter->DispatchTarget), 10.0f, 50.0f);
													if(Logging)
														FormatLogMessage("Pathing Failed, using random object. Index is: %d Mode is: %d Method is: %d Spot is: %f %f %f.", iter-DispatchList.begin(), DispatchMode, DispatchMethod, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z); 
												}

												Goto(iter->DispatchObject,iter->DispatchSpot, 0);
												iter->RandomDist = GetRandomFloat(50.0f) + 10.0f;
												iter->IdleTime = 0; // Flag this to force one-time action.
												iter->DispatchState = DispatchState_Goto;
											}
											break;
										case 2: // Enemy Base, pick their closest building...
											{
												iter->DispatchTarget = 0;
												TempDist = FLT_MAX;

												switch (DispatchMethod)
												{
												case 0: // Select Closest Enemy Building.
													{
														for(int b = 0; b < numBuildings; b++)
														{
															BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(b);
															Handle TempBuilding = buildingInfo.BuildingObject;
															// This is a more desirable enemy base "edge".
															if((GetDistance(iter->DispatchObject, TempBuilding) < TempDist) && (GetMaxHealth(TempBuilding) > 0) && (!IsTeamAllied(Team, buildingInfo.CurTeamNum)))
															{
																TempTarget = TempBuilding;
																TempDist = GetDistance(iter->DispatchObject, TempBuilding); // Save it for next loop.
															}
														}
													}
													break;
												case 1: // Select Random Enemy Building.
													{
														for(int Tries = 0; Tries < numBuildings; Tries++)
														{
															Random = GetRandomInt(numBuildings-1);
															BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(Random);
															Handle TempBuilding = buildingInfo.BuildingObject;
															if((!IsTeamAllied(Team, buildingInfo.CurTeamNum)) && (GetMaxHealth(TempBuilding) > 0))
															{
																TempTarget = TempBuilding;
																break;
															}
														}
													}
													break;
												case 2: // Select Random Enemy Production Unit.
													{
														// Select a random team that might have a production unit.
														RandomTeam = GetRandomTeam(4, Team, true);
														BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(RandomTeam);
														if(teamInfo.Recy)
															TempTarget = teamInfo.Recy;
														else if(teamInfo.Muf)
															TempTarget = teamInfo.Muf;
														else if(teamInfo.Cnst)
															TempTarget = teamInfo.Cnst;
														else if(teamInfo.Slf)
															TempTarget = teamInfo.Slf;
														else // No production units? Pick random then.
														{
															for(int Tries = 0; Tries < numBuildings; Tries++)
															{
																Random = GetRandomInt(numBuildings-1);
																BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(Random);
																Handle TempBuilding = buildingInfo.BuildingObject;
																if((!IsTeamAllied(Team, buildingInfo.CurTeamNum)) && (GetMaxHealth(TempBuilding) > 0))
																{
																	TempTarget = TempBuilding;
																	break;
																}
															}
														}
													}
													break;
												}

												if(TempTarget)
												{
													/*
													// Now that we have the furthest building, lets spice it up so all our turrets don't go to the same place.
													TempDist = 200.0f;
													for(int b = 0; b < numBuildings; b++)
													{
														BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(b);
														Handle TempRandomTarget = buildingInfo.BuildingObject;

														// This building is nearby our goal, lets try to find a position farther away from it.
														if((buildingInfo.CanBeIdleVictim) && 
															//(IsAround(TempRandomTarget)) && 
															( (numBuildings == 1) || (TempRandomTarget != TempTarget) ) &&
															(GetMaxHealth(TempRandomTarget) > 0) && 
															(!IsTeamAllied(Team, buildingInfo.CurTeamNum)) && 
															(GetDistance(TempTarget,TempRandomTarget) <= TempDist)) //|| (BZ1Helper::Get().GetNumTeamBuildingObjects(RandomTeam) == 1))
														{
															iter->DispatchSpot = GetPositionNearOrFar(iter->DispatchObject, TempRandomTarget, 50.0f, 150.0f, true);

															if(Logging)
																FormatLogMessage("Index is: %d Type is: %d Method is: %d Spot is: %f %f %f Target Is: %s, Distance is: %f", iter-DispatchList.begin(), DispatchState, DispatchMethod, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z, GetODFName(TempRandomTarget), GetDistance(TempTarget,TempRandomTarget)); 

															Goto(iter->DispatchObject,iter->DispatchSpot,0);
															iter->RandomDist = GetRandomFloat(iter->EngageRange) + 100.0f;
															iter->Timeout = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
															iter->IsDispatched = true;

															break;
														}
													}
													*/

													iter->DispatchTarget = TempTarget;
													iter->DispatchSpot = GetPositionNearOrFar(iter->DispatchObject, iter->DispatchTarget, 50.0f, 250.0f, true);
													Goto(iter->DispatchObject,iter->DispatchSpot, 0);
													iter->RandomDist = GetRandomFloat(iter->EngageRange) + 100.0f;
													iter->IdleTime = 0; // Flag this to force one-time action.
													iter->DispatchState = DispatchState_Goto;

													if(Logging)
														FormatLogMessage("Index is: %d Mode is: %d Method is: %d Spot is: %f %f %f Target Is: %s", iter-DispatchList.begin(), DispatchMode, DispatchMethod, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z, GetODFName(TempTarget)); 
												}
											}
											break;
										} // End case statements.

										/*
										// If this didn't return a position (i.e. no scrap on map, or spot never got set above and is still 0,0,0)
										if(IsNullVector(Spot))
										{
											// Pick a random object.
											iter->DispatchTarget = BZ1Helper::Get().GetRandomObject();
											Spot = GetPositionNear(GetPosition(iter->DispatchTarget), 10.0f, 50.0f);

											if(Logging)
												FormatLogMessage("Pathing Failed, using random object. Index is: %d Type is: %d Method is: %d Spot is: %f %f %f.", iter-DispatchList.begin(), DispatchState, DispatchMethod, Spot.x, Spot.y, Spot.z); 
										}
										// The above cases should have filled in Spot and RandDist. Send the turret to it's spot and dispatch it.
										Goto(iter->DispatchObject,Spot);
										iter->RandomDist = TempRandDist;
										iter->Timeout = (BZ1Helper::m_TurnCounter + (300.0f * BZ1Helper::m_GameTPS)); // Timeout after 5 minutes of travel.
										iter->IsDispatched = true;
										*/

									} // It's a Turret.
								}
								break;
							case DispatchType_Patrol:
								{
									if((teamInfo.m_CPUMaxPatrolUnits > 0) && (teamInfo.m_CPUNumPatrolUnits < teamInfo.m_CPUMaxPatrolUnits))
									{
										++teamInfo.m_CPUNumPatrolUnits;

										int NumPatrolPaths = bz1Helper.PatrolPathList[Team].size();

										if(NumPatrolPaths > 0)
										{
											// Choose a pathpoint, random out of "#Patrol*" where # is Team number and * is an index #, 1 - #, in order.
											int Random = GetRandomInt(NumPatrolPaths-1);
											const char * PathName = bz1Helper.PatrolPathList[Team][Random];

											if(DoesPathExist(PathName))
											{
												Patrol(iter->DispatchObject, PathName, 0);
												iter->DispatchState = DispatchState_Goto;
												iter->IdleTime = 0; // Never time out, use CMD to trigger release.

												if(Logging)
													FormatLogMessage("Index: %d sent to Patrol path: %s", iter-DispatchList.begin(), PathName);
											}
										}
										else // No patrol paths set, use dynamic Patrol between same-team buildings.
										{
											float Chance = GetRandomFloat(1.0f);
											Handle Target = 0;

											if(Chance < 0.2f)
												Target = BZ1Helper::Get().GetRandomObject(Team, SameTeam+Allies, Building);
											else
												Target = BZ1Helper::Get().GetRandomObject(Team, SameTeam, Building);

											if((Chance < 0.1f) || (!Target)) // 10% chance, or if the above search failed, Goto a random spot, 100m - 5x EngageRange away.
												iter->DispatchSpot = GetPositionNear(GetPosition(iter->DispatchObject), 100.0f, 5 * iter->EngageRange);
											else if(Chance < 0.2f) // 20% chance to patrol Allied building too.
												iter->DispatchSpot = GetPositionNear(GetPosition(Target), 50.0f, 100.0f);
											else // 90% chance Goto near a same-team/allied building.
												iter->DispatchSpot = GetPositionNear(GetPosition(Target), 50.0f, 100.0f);

											Goto(iter->DispatchObject, iter->DispatchSpot, 0);
											iter->DispatchState = DispatchState_Goto;
											iter->IdleTime = 0; // Never time out, use CMD to trigger release.

											if(Logging)
												FormatLogMessage("Index: %d sent to Dynamic Patrol Near Handle: %08x, Position: %f %f %f", iter-DispatchList.begin(), Target, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z);
										}
									}
								}// It's a patrol ship.
								break;
							case DispatchType_Minelayer:
								{
									int NumMLayPaths = bz1Helper.MLayPathList[Team].size();

									if(NumMLayPaths > 0)
									{
										int Random = GetRandomInt(NumMLayPaths-1);
										const char * PathName = bz1Helper.MLayPathList[Team][Random];

										if(DoesPathExist(PathName))
										{
											iter->DispatchSpot = GetVectorFromPath(PathName);
											Goto(iter->DispatchObject, iter->DispatchSpot, 0); // Go to the Vector instead.
											iter->DispatchState = DispatchState_Goto;
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
											SetIndependence(iter->DispatchObject, 0); // Remove ability to be interrupted. 

											if(Logging)
												FormatLogMessage("Index: %d sent to Lay Mines at path: %s", iter-DispatchList.begin(), PathName);
										}
									}
									else // Dynamic Minelayer commands.
									{
										int DispatchState = GetRandomInt(2);

										switch (DispatchState)
										{
										case 0: // Friendly Base Guard Duty. Find a spot near edge of base.
											{
												iter->DispatchSpot = GetPositionNearOrFar(BZ1Helper::Get().GetRandomObject(Team, Enemy, Building), BZ1Helper::Get().GetRandomObject(Team, SameTeam, Building), 100.0f, 200.0f, true);
												break;
											}
										case 1: // Random Spot in Field.
											{
												float MapDepth = BZ1Helper::Get().GetMapInfo().MapDepth;
												float MapWidth = BZ1Helper::Get().GetMapInfo().MapWidth;
												iter->DispatchSpot = Vector(GetRandomFloat(-MapDepth, MapDepth), 0, GetRandomFloat(-MapWidth, MapWidth));
												iter->DispatchSpot.y = TerrainFindFloor(iter->DispatchSpot.x, iter->DispatchSpot.z);
												break;
											}
										case 2: // Enemy Base Siege, near enemy.
											{
												iter->DispatchSpot = GetPositionNearOrFar(iter->DispatchObject, BZ1Helper::Get().GetRandomObject(Team, Enemy, Building), 100.0f, 200.0f, true);
												break;
											}
										}

										Goto(iter->DispatchObject, iter->DispatchSpot, 0);
										iter->DispatchState = DispatchState_Goto;
										iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
										SetIndependence(iter->DispatchObject, 0); // Remove ability to be interrupted. 

										if(Logging)
											FormatLogMessage("Index: %d sent to Dynamic Minelaying, DispatchState: %d, Position: %f %f %f", iter-DispatchList.begin(), DispatchState, iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z);
									}

								}// Its a minelayer.
								break;
							case DispatchType_Hunter:
								{
									if((teamInfo.m_CPUMaxHuntUnits > 0) && (teamInfo.m_CPUNumHuntUnits < teamInfo.m_CPUMaxHuntUnits))
									{
										++teamInfo.m_CPUNumHuntUnits;

										// Old Code, Manual Hunt behavior.
										if(BZ1Helper::m_GameVersion <= v13PB65)
										{
											int Random = GetRandomInt(ScrapHelper::Get().GetNumScrapObjects()-1);
											if(ScrapHelper::Get().GetNumScrapObjects() > 0) //&& (IsAround(ScrapHelper::Get().RefScrapHandle((Random)))))
												iter->DispatchTarget = ScrapHelper::Get().RefScrapHandle(Random);
											else
												iter->DispatchTarget = BZ1Helper::Get().GetRandomObject();

											if(IsAround(iter->DispatchTarget))
											{
												iter->DispatchSpot = GetPosition(iter->DispatchTarget);
												iter->DispatchState = DispatchState_Goto;
												iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.

												if(Logging)
													FormatLogMessage("Index: %d Hunt Task Starting, Target: %s", iter-DispatchList.begin(), GetODFName(iter->DispatchTarget));

												Goto(iter->DispatchObject, iter->DispatchSpot, 0);
											}
										}
										else // New code, just tell it to Cmd_Hunt, let the game do the rest.
										{
											Hunt(iter->DispatchObject, 0);
											iter->DispatchState = DispatchState_Goto;
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
										}
									}
								}// Its an Hunt unit.
								break;
							case DispatchType_Escort:
								{
									if((teamInfo.m_CPUMaxEscortUnits > 0) && (teamInfo.m_CPUNumEscortUnits < teamInfo.m_CPUMaxEscortUnits) )
									{
										// Pick a random object to Escort.
										iter->DispatchTarget = UtilityShipHelper::Get().GetEscortTarget();

										if(iter->DispatchTarget)
										{
											//Follow(iter->DispatchObject, iter->DispatchTarget, 0);
											Defend2(iter->DispatchObject, iter->DispatchTarget, 0);
											iter->DispatchState = DispatchState_Goto;
											iter->IdleTime = 0; // Never time out, use CMD to trigger release.

											++teamInfo.m_CPUNumEscortUnits;
										}
									}
								}// It's an escort unit.
								break;
							} // End Switch DispatchType.
						} // Idle timer is up. (And AIP is Running OR SkipAIP is true)
					} // Dispatch hasn't been set.
					break;
				case DispatchState_Goto: // It's doing something, track when it's there.
					{
						// Special case for Turrets, they might get interrupted en-route.
						if(iter->DispatchType == DispatchType_Turret)
						{
							// If Timeout was never set, Set it once we undeploy and get moving, else watch for us to get interrupted.
							if((!iter->IdleTime) && (!IsDeployed(iter->DispatchObject)))
								iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
							else if((iter->IdleTime) && (IsDeployed(iter->DispatchObject)))
								iter->DispatchState = DispatchState_Idle; // It got interrupted.
						}

						if((iter->SkipAIPCheck) || (teamInfo.m_CPUDoDispatch))
						{
							// If this unit's timeout has passed, re-dispatch it.
							if((iter->IdleTime > 0) && (BZ1Helper::m_TurnCounter >= iter->IdleTime))
								ClearDispatch(DispatchIndex);

							// If its a Turret. (and there's an AIP running)
							switch (iter->DispatchType)
							{
							case DispatchType_Turret:
								{
									bool Stop = false;
									bool Return = false;
									// If it encounters enemy building's early, stop here.
									for(int p = 0; p < numObjects; p++)
									{
										BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(p);
										Handle TempH = objectInfo.ObjectHandle;

										if( (!IsTeamAllied(Team, objectInfo.CurTeamNum)) && // It's an Enemy.
											((objectInfo.CategoryID == Building) || ((IsTurret(TempH)) && (IsDeployed(TempH)))) && // It's a Building or a Deployed Turret.
											(GetDistance(iter->DispatchObject,TempH) < (objectInfo.EngageRange + 50.0f))) // We're close, stop here.
										{
											Stop = true;
											// 50% chance that it will decide to go someplace else instead. 
											if(GetRandomFloat(1.0f) < 0.5f)
												Return = true;

											if(Logging)
												FormatLogMessage("Index: %d Turret Encountered an Enemy on Team: %d Distance Is: %f, Return is: %d", iter-DispatchList.begin(), objectInfo.CurTeamNum, GetDistance(iter->DispatchObject,TempH), Return); 

											break;
										}
									}

									// If our target is around and we're within randomDist+5, or we encountered an enemy, or we deployed.
									if(((IsAround(iter->DispatchTarget)) && (GetDistance(iter->DispatchObject, iter->DispatchSpot) <= iter->RandomDist+5.0f)) || (Stop) || (IsDeployed(iter->DispatchObject)))
									{
										if(Return)
										{
											ClearDispatch(DispatchIndex);
										}
										else
										{
											if(Stop)
											{
												iter->DispatchSpot = GetPositionNear(GetPosition(iter->DispatchObject), 10.0f, 30.0f);
												Goto(iter->DispatchObject,iter->DispatchSpot, 0);

												if(Logging)
													FormatLogMessage("Index is: %d Spot is: %f %f %f", iter-DispatchList.begin(), iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z); 
											}
											else
											{
												Defend(iter->DispatchObject, 0); // Sit here until I decide to tell you otherwise. 
											}
											iter->DispatchState = DispatchState_Idle;
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Wait 5 minutes before trying to move again.
										}
									}
								} // It's a turret.
								break;
							case DispatchType_Patrol:
								{
									if(iter->Command == CMD_NONE)
									{
										int NumPatrolPaths = bz1Helper.PatrolPathList[Team].size();

										if(NumPatrolPaths) // Unit got interrupted on path-based Patrol, reset it.
										{
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(15.0f)); // Wait 15 seconds before trying to move again.

											if(Logging)
												FormatLogMessage("Patrol Unit %d interrupted, resetting idle timer.", iter-DispatchList.begin());
										}
										else
										{
											iter->IdleTime = 0; // Throw it back on the pile...

											if(Logging)
												FormatLogMessage("Index: %d sent to Dynamic Patrol Reset, Cmd == CMD_NONE", iter-DispatchList.begin());
										}

										iter->DispatchState = DispatchState_Idle;

										/*
										else // Dynamic patrol, keep going.
										{
											float Chance = GetRandomFloat(1.0f);

											if(Chance < 0.1f) // Goto a random spot, 100m - 5x EngageRange away. This should try to pick a spot on relatively same-level terrain.
												Goto(iter->DispatchObject, GetPositionNear(GetPosition(iter->DispatchObject), 100.0f, 5 * iter->EngageRange), 0);
											else if(Chance < 0.2f) // 20% chance to patrol Allied building too.
												Goto(iter->DispatchObject, GetPositionNear(BZ1Helper::Get().GetRandomObject(Team, SameTeam+Allies, Building), 50.0f, 100.0f), 0);
											else // 90% chance Goto near a same-team/allied building.
												Goto(iter->DispatchObject, GetPositionNear(BZ1Helper::Get().GetRandomObject(Team, SameTeam, Building), 50.0f, 100.0f), 0);
										}
										*/
									}
								}// Its a patrol ship.
								break;
							case DispatchType_Minelayer:
								{
									// We're here. Activate mining beams!  EVE Online referance. :P
									if(GetDistance(iter->DispatchObject, iter->DispatchSpot) < 10.0f)
									{
										SetCommand(iter->DispatchObject, CMD_LAY_MINES, 0); //Mine(iter->DispatchObject, PathName);
										iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Wait 5 minutes before trying to move again.

										if(Logging)
											FormatLogMessage("Index: %d Minelayer reached Spot: %f %f %f, laying mines...", iter-DispatchList.begin(), iter->DispatchSpot.x, iter->DispatchSpot.y, iter->DispatchSpot.z);
									}
								}// Its a minelayer.
								break;
							case DispatchType_Hunter:
								{
									if(BZ1Helper::m_GameVersion <= v13PB65)
									{
										// If it got distracted? or was given other orders, exit Hunt mode for now.
										if((IsAround(iter->DispatchTarget)) && (GetCurrentWho(iter->DispatchObject) != iter->DispatchTarget))
										{
											if(Logging)
												FormatLogMessage("Index: %d Hunt Task aborting, CurrentWho: %s, DispatchTarget: %s", iter-DispatchList.begin(), GetODFName(iter->DispatchObject), GetODFName(iter->DispatchTarget));

											iter->DispatchState = DispatchState_Idle;
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Wait 5 minutes before trying to move again.
										}

										// When it's there, or its current target is gone, pick new goto. // Modified to use Vector Spot, since the object we wen't to might have moved.
										if((!IsAround(iter->DispatchTarget)) || (GetDistance(iter->DispatchObject, iter->DispatchSpot) < 50.0f))
										{
											int Random = GetRandomInt(ScrapHelper::Get().GetNumScrapObjects()-1);
											if(ScrapHelper::Get().GetNumScrapObjects() > 0) //&& (IsAround(ScrapHelper::Get().RefScrapHandle((Random)))))
												iter->DispatchTarget = ScrapHelper::Get().RefScrapHandle(Random);
											else
												iter->DispatchTarget = BZ1Helper::Get().GetRandomObject();

											iter->DispatchSpot = GetPosition(iter->DispatchTarget);

											if(Logging)
												FormatLogMessage("Index: %d Hunt Task Continuing, Picking new Target: %s", iter-DispatchList.begin(), GetODFName(iter->DispatchTarget));

											Goto(iter->DispatchObject, iter->DispatchSpot, 0);
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
										}
									}
									else // New code, let the engine do all the work.
									{
										if(GetCurrentCommand(iter->DispatchObject) == CMD_NONE) // It's not doing anything, reset Hunt?
										{
											Hunt(iter->DispatchObject, 0); //iter->DispatchState = DispatchState_Idle;
											iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Wait 5 minutes before trying to move again.
										}
									}
								}// Its a hunt unit.
								break;
							case DispatchType_Escort: // If this unit was told to Escort a scavenger (or minelayer?) Do it.
								{
									if(!IsAround(iter->DispatchTarget)) // Uh Oh, you Escort Target Died! Do something!
									{
										// Go escort again.
										if(GetRandomFloat(1.0f) < 0.5f)
										{
											iter->DispatchTarget = UtilityShipHelper::Get().GetEscortTarget(Team, SameTeam+Allies);
											Defend2(iter->DispatchObject, iter->DispatchTarget, 0);
										}
										else // Do something else.
										{
											iter->DispatchType = DispatchType_None; //false;
											ClearDispatch(DispatchIndex);
											--teamInfo.m_CPUNumEscortUnits;
											// Go attack the nearest thing, chances are it's what killed your escort target, Bezerk mode. :)
											Attack(iter->DispatchObject, GetNearestEnemy(iter->DispatchObject, true, false), 0);
										}
									}
								}
								break;
							} // End Switch over DispatchType.
						}
					}
					break;
				case DispatchState_Reload:
				case DispatchState_Repair:
					{
						// If this unit was reloading or repairing, and it's done, clear it's orders so it can continue.
						if( ((iter->DispatchState == DispatchState_Repair) && (GetHealth(iter->DispatchObject) > iter->GetRepairRatio)) || // If they were repairing and their health is better now.
							((iter->DispatchState == DispatchState_Reload) && (GetCurAmmo(iter->DispatchObject) > iter->MinFireAmmo)) || // They were reloading and can fire again. :)
							(((BZ1Helper::m_TurnCounter >= iter->IdleTime) || (!IsAround(iter->DispatchTarget))))) // Idle Timer has passed, or destination went poof.
						{
							if(Logging)
								FormatLogMessage("Index: %d Unit: %s Rearmed/Repaired, resuming duties", iter-DispatchList.begin(), GetODFName(iter->DispatchObject));

							SetIndependence(iter->DispatchObject, 1);
							ClearDispatch(DispatchIndex);
							iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(15.0f)); // Wait 15 seconds before trying to move again.
						}
					} // Unit not in position, is enroute.
					break;
				case DispatchState_Flee: // Unit Fleeing Code.
					{
						if(BZ1Helper::m_TurnCounter >= iter->IdleTime) //iter->FleeTimeout)
						{
							SetIndependence(iter->DispatchObject, 1); // Return normal AI control. 
							iter->FleeHealthValue = GetCurHealth(iter->DispatchObject);
							// !!== Restore previous dispatch mode? ????
							ClearDispatch(DispatchIndex); // Reset orders.
							iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(15.0f)); // Wait 15 seconds before trying to move again.

							if(IsAround(iter->FleeTarget))
								Attack(iter->DispatchObject, iter->FleeTarget, 0);
						}
					}
					break;
				} // DispatchState Switch.


				// If these units can get service, and are not currently doing so.
				if( (teamInfo.m_CPUUnitsCanService) && 
					(iter->DispatchState != DispatchState_Repair) && 
					(iter->DispatchState != DispatchState_Reload)) // If this unit isn't currently performing a reload or repair function.
				{
					if( (iter->CanGetRepair) && // It can repair.
						(BZ1Helper::m_TurnCounter % ConvertToTurns(10.0f) == 0) && // Check every 10 seconds.
						(GetHealth(iter->DispatchObject) < iter->GetRepairRatio)) // If its health is < the threshold.
					{
						char RepairPodODF[MAX_ODF_LENGTH] = {0};
						int RepairPodCost = 0;
						Handle TempH = teamInfo.Slf;

						if(TempH)
							GetBuildItem(TempH, RepairPodODF, (int*)RepairPodCost, 1); // Get build costs out.

						// This is a stationary deployed item, send ammo/repair to it if we have enough scrap and an Armory.
						if(((TempH) && ((iter->DispatchType == DispatchType_Turret) || (iter->DispatchType == DispatchType_Howitzer))) && // Armory exists and it's a Turrety thing.
							(IsDeployed(iter->DispatchObject)) && // It's deployed.
							(GetScrap(Team) >= RepairPodCost)) // We have enough scrap.
						{
							Vector TargetPos = GetPosition(iter->DispatchObject);
							TargetPos.y += 350.0f;
							BuildObject(RepairPodODF, Team, TargetPos);
							AddScrap(Team, -RepairPodCost); // Take away the cost.

							iter->DispatchState = DispatchState_Repair;
							int TempIdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f));
							if(iter->IdleTime < TempIdleTime)
								iter->IdleTime = TempIdleTime; // Set to 30 seconds to let powerup land, don't want to drive off while it's landing on us.
						}
						// Random % chance this unit will go get service, every 10 seconds.
						else if(GetRandomFloat(1.0f) < iter->ServiceChance)
						{
							float TempDist = FLT_MAX;
							Handle TempTarget = 0;
							// Loop over all objects, looking for the closest place to refill.
							for(int i = 0; i < numObjects; i++)
							{
								BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(i);
								Handle TempH = objectInfo.ObjectHandle;

								if((objectInfo.ObjClassHash == CLASS_POWERUP_REPAIR) || (objectInfo.ObjClassHash == CLASS_POWERUP_SERVICE) || 
									( (objectInfo.ObjClassHash == CLASS_SUPPLYDEPOT) && ((objectInfo.HealthRate > 0) || (objectInfo.ServiceRate > 0)) && 
									(objectInfo.ServiceCraft) && (IsTeamAllied(objectInfo.CurTeamNum, Team))) && (GetDistance(iter->DispatchObject, TempH) < TempDist))
								{
									TempTarget = TempH;
									TempDist = GetDistance(iter->DispatchObject,TempH); // Save it for next loop.
								}
							}
							// If the above found something.
							if(TempTarget)
							{
								if(Logging)
									FormatLogMessage("Index: %d Unit: %s going to get Repair", iter-DispatchList.begin(), GetODFName(iter->DispatchObject));

								iter->DispatchTarget = TempTarget;
								Goto(iter->DispatchObject, iter->DispatchTarget, 0);
								SetIndependence(iter->DispatchObject, 0);
								iter->DispatchState = DispatchState_Repair;
								iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Wait 30 seconds before trying to attack again.
							}
						}
					}
					// Get Ammo code, this makes AI ships get ammo when they run out.
					if( (iter->CanGetReload) && // Can reload.
						(BZ1Helper::m_TurnCounter % ConvertToTurns(10.0f) == 0) && // Every 10 seconds.
						(GetCurAmmo(iter->DispatchObject) < GetMaxAmmo(iter->DispatchObject))) // If it's not currently full.
					{
						// Calculate this unit's minimum required ammo to fire.
						iter->MinFireAmmo = GetMinFireAmmo(iter->DispatchObject);

						// It doesn't have enough to fire it's main weapons, send it to reload.
						if(GetCurAmmo(iter->DispatchObject) < iter->MinFireAmmo)
						{
							char AmmoPodODF[MAX_ODF_LENGTH] = {0};
							int AmmoPodCost = 0;
							Handle TempH = teamInfo.Slf;

							if(TempH)
								GetBuildItem(TempH, AmmoPodODF, (int*)AmmoPodCost, 2);

							// This is a stationary deployed item, send ammo/repair to it if we have enough scrap.
							if((TempH) && ((iter->DispatchType == DispatchType_Turret) || (iter->DispatchType == DispatchType_Howitzer)) && // This is a deploey thing.
								(IsDeployed(iter->DispatchObject)) && // It's deployed.
								(GetScrap(Team) >= AmmoPodCost)) // We have enough scrap.
							{
								Vector TargetPos = GetPosition(iter->DispatchObject);
								TargetPos.y += 350.0f;
								BuildObject(AmmoPodODF, Team, TargetPos);
								AddScrap(Team, -AmmoPodCost); // Take away the cost.

								iter->DispatchState = DispatchState_Reload;
								int TempIdleTime = BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f);
								if(iter->IdleTime < TempIdleTime)
									iter->IdleTime = TempIdleTime; // Set to 30 seconds to let powerup land.
							//	iter->Timeout = (BZ1Helper::m_TurnCounter + (300.0f * BZ1Helper::m_GameTPS)); // Timeout after 5 minutes of travel.
							}
							else // its mobile, or we don't have an armory or enough scrap.
							{
								float TempDist = FLT_MAX;
								Handle TempTarget = 0;
								// Loop over all objects, looking for the closest place to refill.
								for(int i = 0; i < numObjects; i++)
								{
									BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(i);
									Handle TempH = objectInfo.ObjectHandle;
									
									if((objectInfo.ObjClassHash == CLASS_POWERUP_RELOAD) || (objectInfo.ObjClassHash == CLASS_POWERUP_SERVICE) || 
										((objectInfo.ObjClassHash == CLASS_SUPPLYDEPOT) && ((objectInfo.AmmoRate > 0) || (objectInfo.ServiceRate > 0)) && 
										(objectInfo.ServiceCraft) && (IsTeamAllied(objectInfo.CurTeamNum, Team))) && (GetDistance(iter->DispatchObject, TempH) < TempDist))
									{
										TempTarget = TempH;
										TempDist = GetDistance(iter->DispatchObject,TempH); // Save it for next loop.
									}
								}
								if(IsAround(TempTarget))
								{
									if(Logging)
										FormatLogMessage("Index: %d Unit: %s going to get Reload", iter-DispatchList.begin(), GetODFName(iter->DispatchObject));

									iter->DispatchTarget = TempTarget;
									Goto(iter->DispatchObject, iter->DispatchTarget, 0);
									SetIndependence(iter->DispatchObject, 0);
									iter->DispatchState = DispatchState_Reload;
									iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f)); // Wait 30 seconds before trying to attack again.
									//iter->Timeout = (BZ1Helper::m_TurnCounter + ConvertToTurns(300.0f)); // Timeout after 5 minutes of travel.
								}
							}
						//	else
						//	{
						//		iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(10.0f)); // Wait 10 seconds before trying to reload again.
						//	}
						}
					}
				} // end can get service.

				// Update the unit's saved command for comparison.
				if(GetCurrentCommand(iter->DispatchObject) != iter->Command)
					iter->Command = GetCurrentCommand(iter->DispatchObject);

				/*
				// Cloaking code. Handle cloaking when no enemies are nearby.
				*/


			} // There's no players on this team.

			if((iter->IsCloaker)) // && (!iter->CloakPriority)) // If CloakPriority is 1, only allow scripts Cloak/Decloak commands to control Cloaking.
			{
				if(!iter->IsCloaked)
				{ // If it isn't cloaked, or it's orders have changed.
					if((iter->CloakNextTick)) // && (BZ1Helper::m_TurnCounter >= iter->CloakIdleTime))
					{
						if(!IsDeployed(iter->DispatchObject))
						{
							if(Logging)
								FormatLogMessage("Index: %d Cloaking Activating, Timer: %d", iter-DispatchList.begin(), BZ1Helper::m_TurnCounter);
						//	if(iter->CloakPriority)
						//	{
								SetCommand(iter->DispatchObject, CMD_MORPH_SETDEPLOYED, 0); // Cloak the object.
								iter->DeployTimer = (BZ1Helper::m_TurnCounter + ConvertToTurns(iter->TimeDeploy) + 1);
								iter->CloakLocked = true;
						//	}
						//	else
						//	{
							//	SetCommand(iter->DispatchObject, CMD_MORPH_UNLOCK, 0); // Don't allow Re-Cloaking.
						//		Deploy(iter->DispatchObject);
							//	iter->CloakLocked = false;
						//	}
						}
						iter->IsCloaked = true;
						iter->CloakNextTick = false;
					} // End Cloak Command. 
					// This handles manual Cloaking AI behavior. If Not Player Team, AIPCheck, CloakPriority = 0, amd IdleTime passed OR Command Changed, re-calculate if we should cloak.
					else if((!GetPlayerHandle(Team)) && ((iter->SkipAIPCheck) || (teamInfo.m_CPUDoDispatch)) && (!iter->CloakPriority) && ((BZ1Helper::m_TurnCounter >= iter->CloakIdleTime) || ((GetCurrentCommand(iter->DispatchObject) != iter->Command))))
					{
						float Chance = GetRandomFloat(1.0f);
						Handle Enemy = GetNearestEnemy(iter->DispatchObject);
						// If its not deployed, and there are no enemies in range.
						if(((!IsDeployed(iter->DispatchObject)) || (iter->IsMorpher)) && ((!Enemy) || (GetDistance3DSquared(iter->DispatchObject, Enemy) > Squared(iter->EngageRange))))
						{
							if(Logging)
								FormatLogMessage("Index: %d Attempting to Cloak unit, Chance: %f, CloakCommand: %d Chance: %f", iter-DispatchList.begin(), Chance, iter->Command, iter->CloakChance[iter->Command]);
							if(Chance < iter->CloakChance[iter->Command])
							{
								if(Logging)
									FormatLogMessage("Index: %d Cloak Successful", iter-DispatchList.begin());
								Cloak(iter->DispatchObject, iter->CloakPriority);
							//	Deploy(iter->DispatchObject);
							//	SetCommand(iter->DispatchObject, CMD_MORPH_SETDEPLOYED, 1); // Lock morph state until we say otherwise.
							//	iter->CloakLocked = true;
							//	iter->IsCloaked = true;
							}
							iter->CloakIdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(15.0f)); // Wait 15 seconds before trying to cloak again.
						} // Unit is not deployed and there are no enemies around.
					} // Cloak idle time is up.
				} // Unit is not cloaked.
				else // Unit is cloaked.
				{
					Handle Enemy = GetNearestEnemy(iter->DispatchObject);
					Handle Target = GetCurrentWho(iter->DispatchObject);
					if((IsDeployed(iter->DispatchObject)))
					{
						// DLL Ordered us to Decloak.
						if(iter->DecloakNextTick)
						{
							SetCommand(iter->DispatchObject, CMD_MORPH_SETUNDEPLOYED, 0); // Decloak the object.
							iter->DecloakNextTick = false;
						}
						else
						{
							// If AI Can manually Decloak, Unlock ability for AI to decloak on it's own when an enemy is nearby.
							if(!iter->CloakPriority)
							{
								if((iter->CloakLocked) && (GetDistance3DSquared(iter->DispatchObject, Enemy) < Squared(iter->EngageRange)))
								{
									if(Logging)
										FormatLogMessage("Index: %d Unit is Cloaked, Enemy detected, Unlocking Cloak", iter-DispatchList.begin());
									SetCommand(iter->DispatchObject, CMD_MORPH_UNLOCK, 0); // Unlock morph state so it can Attack something.
									iter->CloakLocked = false;
								}
							}
							// If we were ordered to Attack, and are now within range of our target, Decloak.
							if((iter->Command == CMD_ATTACK) && (GetDistance3DSquared(iter->DispatchObject, Target) < Squared(iter->EngageRange)))
							{
								if(Logging)
									FormatLogMessage("Index: %d Unit is Cloaked, Target In Range, Decloaking", iter-DispatchList.begin());
								Decloak(iter->DispatchObject, iter->CloakPriority);
							//	SetCommand(iter->DispatchObject, CMD_MORPH_UNLOCK, 0); // Unlock morph state so it can Attack something.
								iter->CloakLocked = false;
							}
							/*
							if((iter->AttackingWhenCloaked) && (GetDistance3DSquared(iter->DispatchObject, iter->DispatchTarget) < Squared(iter->EngageRange)))
							{
								FormatLogMessage("Index: %d Unit is Cloaked, TARGET IN RANGE! Unlocking Cloak", iter-DispatchList.begin());
								Attack(iter->DispatchObject, iter->DispatchTarget);
								iter->AttackingWhenCloaked = false;
							}
							*/
						}
					}
					// If it becomes uncloaked, for whatever reason, flag as not cloaked and reset timer.
					else if(BZ1Helper::m_TurnCounter > iter->DeployTimer)
					{
						if((GetPlayerHandle(Team) == 0) && ((iter->SkipAIPCheck) || (teamInfo.m_CPUDoDispatch)))
						{
							if(GetCurrentCommand(iter->DispatchObject) == iter->Command)
							{
								if(Logging)
									FormatLogMessage("Index: %d Command Identicle, setting CloakIdle time for 15 seconds.", iter-DispatchList.begin());
								iter->CloakIdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(15.0f)); // Wait 15 seconds before trying to cloak again.
							}
							else
							{
								if(Logging)
									FormatLogMessage("Index: %d Command Different, Setting CloakIdle time for NOW.", iter-DispatchList.begin());
								iter->CloakIdleTime = BZ1Helper::m_TurnCounter; // Command changed, dont wait to recalculate cloak.
							}
						}

						iter->IsCloaked = false; // It's past the deployTimer, this unit has Decloaked.
						if(Logging)
							FormatLogMessage("Unit is not cloaked! Timer is: %f, DeployTime: %f", iter->DeployTimer, iter->TimeDeploy);
					}
				} // Unit is not cloaked.
			//	if(!iter->IsMorpher)
			//	{
			//		if(IsDeployed(iter->DispatchObject))
			//			iter->IsCloaked = true;
			//		else
			//			iter->IsCloaked = false;
			//	}

			} // Its a cloaker.

		} // Dispatch is not player.
	}
}

void DispatchHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	bool Logging = BZ1Helper::Logging;
	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if((victimHandle == iter->DispatchObject) && (!GetPlayerHandle(GetTeamNum(iter->DispatchObject))))
		{
			// This turret got shot on it's way to where it was going. Make it stop and deploy.
			if((iter->DispatchType == DispatchType_Turret) && (!IsDeployed(iter->DispatchObject)))
			{
				//Stop(iter->DispatchObject, 1);
				Defend(iter->DispatchObject, 0); // Sit here until I decide to tell you otherwise.
				iter->DispatchState = DispatchState_Idle;
				iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f)); // Wait 30 seconds before trying to move again.

				return; // We're done, exit early. 
			}
			else if((iter->DispatchType == DispatchType_Minelayer)  && (iter->DispatchState == DispatchState_Goto))
			{
				SetCommand(iter->DispatchObject, CMD_LAY_MINES, 0);
				iter->DispatchState = DispatchState_Idle;
				iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(30.0f)); // Wait 30 seconds before trying to move again.

				return; // We're done, exit early.
			}
			else if((iter->CanFlee) && // It can Flee.
					(iter->DispatchState != DispatchState_Flee) && // It's not already Fleeing.
					(BZ1Helper::m_TurnCounter > iter->IdleTime) && //iter->FleeTimeout) && // Flee Timeout is up.
					(iter->DispatchState != DispatchState_Repair) && // It's not reloading.
					(iter->DispatchState != DispatchState_Reload)) // It's not repairing.
			{
				long MaxHealth = GetMaxHealth(iter->DispatchObject);
				long CurHealth = GetCurHealth(iter->DispatchObject);
				long Damage = GetOrdnanceDamageDealt(iter->DispatchObject, pOrdnanceODF, true);
				if( (GetHealth(iter->DispatchObject) < iter->FleeHealthRatio) && // Minimum Health Ratio is met.
					((iter->FleeHealthValue - CurHealth) >= (MaxHealth / 10)) && // Has lost 10% of it's health since last Flee state ended.
					//(GetRandomFloat(1.0f) < 0.25f)) // 25% chance of fleeing. // was 50% chance.
					(GetRandomFloat(float(max(4 * Damage, CurHealth))) < Damage)) // % chance, based on ordnance damage. Can't be > 25%.
				{
					//iter->FleeTimeout = (BZ1Helper::m_TurnCounter + ConvertToTurns(3.0f + GetRandomFloat(2.0f))); // Flee for 3-5 seconds.
					iter->IdleTime = (BZ1Helper::m_TurnCounter + ConvertToTurns(3.0f + GetRandomFloat(2.0f))); // Flee for 3-5 seconds.
					iter->FleeTarget = GetTarget(iter->DispatchObject); //shooterHandle;
					// If there is no target, return fire! 
					bool Retarget = false;
					GetODFBool(iter->DispatchObject, "CraftClass", "RetargetOnStrafe", &Retarget, false);
					if((!IsAround(iter->FleeTarget)) && (Retarget))
						iter->FleeTarget = shooterHandle;

					SetIndependence(iter->DispatchObject, 0); // Dont let the AI do stuff on its own.
					Vector FleePosition = GetPositionNearOrFar(iter->DispatchObject, shooterHandle, 40.0f, 75.0f, false);
					Goto(iter->DispatchObject, FleePosition, 0);
					if(iter->IsCloaker) // && (!iter->CloakPriority))
					{
						if(GetRandomFloat(1.0f) < 0.5f) // 50% chance of cloaking, if we can cloak. :P
							Deploy(iter->DispatchObject);
					}
					iter->DispatchState = DispatchState_Flee;

					if(Logging)
						FormatLogMessage("Flee Target Shot, Target is: %s, IdleTime is: %f", GetODFName(iter->DispatchObject), iter->IdleTime);
				}
			}
			return; // We're done, exit early. 
		}
	}
}

void DispatchHelper::ReadChanceValues(char const *const file, char const * const block, float value[], float const defval)
{
	if(OpenODF2(file))
	{
		for (int i = 0; i < NUM_CMD; ++i)
			GetODFFloat(file, block, CommandList[i], &value[i], defval);
	}
}


void DispatchHelper::Cloak(const Handle h, const int Priority)
{
	bool Logging = BZ1Helper::Logging; //true;

	if(!IsAround(h))
		return;

	for(int t = 0; t < MorphObjectHelper::Get().GetNumMorphObjects();t++)
	{
		if((MorphObjectHelper::Get().RefMorphObjectHandle(t) == h) && (MorphObjectHelper::Get().GetMorphObjectCloaker(t)))
		{
			SetCommand(h, CMD_HUNT, 0); // Cloak.
			return;
		}
	}

	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
		{
			/*
			Handle Target = GetCurrentWho(iter->DispatchObject);
			if(!IsAround(Target))
				Target = GetTarget(iter->DispatchObject);
			int Command = GetCurrentCommand(iter->DispatchObject);
		//	FormatLogMessage("Cloaking Already-In-List item, command: %d", Command);
			*/

			if(IsPlayer(h))
			{
				if(!IsDeployed(h))
					Deploy(h);
			}
			else
			{
				iter->CloakPriority = Priority;
				iter->CloakNextTick = true;
				//iter->CloakIdleTime = (BZ1Helper::m_TurnCounter + 1); // Wait 1 tick then cloak.

				if(Logging)
					FormatLogMessage("Cloak Command Given, Time: %d", BZ1Helper::m_TurnCounter);
			}

			/*
			if(!IsDeployed(iter->DispatchObject))
			{
				if(Priority)
				{
					SetCommand(iter->DispatchObject, CMD_MORPH_SETDEPLOYED, 0);
					iter->CloakLocked = true;
				}
				else
				{
					SetCommand(iter->DispatchObject, CMD_MORPH_UNLOCK, 0); // Don't allow Re-Cloaking.
					Deploy(iter->DispatchObject);
					iter->CloakLocked = false;
				}
			}
			iter->IsCloaked = true;
			if(Command == CMD_ATTACK)
			{
		//		FormatLogMessage("Overriding Attack with Goto, Target: %d", Target);
		//		Goto(iter->DispatchObject, Target, 0);
				iter->AttackingWhenCloaked = true;
				iter->DispatchTarget = Target;
			}
	//			SetCommand(iter->DispatchObject, CMD_GO, Priority, Target);
	//		else
	//			SetCommand(iter->DispatchObject, Command, Priority, Target);
			*/
			return;
		}
	}

	AddToDispatch(h, 0.0f, true, Priority, true, true);
}
void DispatchHelper::Decloak(const Handle h, const int Priority)
{
	if(!IsAround(h))
		return;

	for(int t = 0; t < MorphObjectHelper::Get().GetNumMorphObjects();t++)
	{
		if((MorphObjectHelper::Get().RefMorphObjectHandle(t) == h) && (MorphObjectHelper::Get().GetMorphObjectCloaker(t)))
		{
			SetCommand(h, CMD_HUNT); // Cloak.
			return;
		}
	}

	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
		{
			if(IsPlayer(h))
			{
				if(IsDeployed(h))
					Deploy(h);
			}
			else
			{
				iter->DecloakNextTick = true;
			}
			/*
			if(IsDeployed(iter->DispatchObject))
			{
			//	if(Priority)
			//	{
					SetCommand(iter->DispatchObject, CMD_MORPH_SETUNDEPLOYED, 0); // Don't allow Re-Cloaking.
				//	iter->CloakLocked = true;
			//	}
			//	else
			//	{
			//		SetCommand(iter->DispatchObject, CMD_MORPH_UNLOCK, 0); // Don't allow Re-Cloaking.
			//		Deploy(iter->DispatchObject);
				//	iter->CloakLocked = false;
			//	}
			}
			iter->CloakPriority = Priority;
			iter->IsCloaked = false;
			*/
			return;
		}
	}
}
/*
void DispatchHelper::CloakAttack(Handle me, Handle him, int Priority)
{
	bool IsMorphObject = false;
	for(int t = 0; t < MorphObjectHelper::Get().GetNumMorphObjects();t++)
	{
		if((MorphObjectHelper::Get().RefMorphObjectHandle(t) == me) && (MorphObjectHelper::Get().GetMorphObjectCloaker(t)))
		{
			SetCommand(me, CMD_HUNT); // Cloak.
			IsMorphObject = true;
			break;
		}
	}
	if(!IsMorphObject)
	{
		bool AddToList = true;
		for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
		{
			if(iter->DispatchObject == me)
			{
				AddToList = false;
				SetCommand(iter->DispatchObject, CMD_MORPH_SETDEPLOYED);
				Goto(iter->DispatchObject, him, 0);
				if(!IsDeployed(iter->DispatchObject))
					Deploy(iter->DispatchObject);
				iter->CloakLocked = true;
				iter->CloakPriority = Priority;

				iter->DispatchTarget = him;
				iter->AttackingWhenCloaked = true;

				break;
			}
		}
		if(AddToList)
			AddToDispatch(me, 0.0, Priority, true, true, false, false, him);
	}
}
*/

bool DispatchHelper::IsCloaked(const Handle h)
{
	if(!IsAround(h))
		return false;

	for(int t = 0; t < MorphObjectHelper::Get().GetNumMorphObjects();t++)
	{
		if((MorphObjectHelper::Get().RefMorphObjectHandle(t) == h) && (MorphObjectHelper::Get().GetMorphObjectCloaker(t)))
			return MorphObjectHelper::Get().GetMorphObjectIsActive(t);
	}

	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
			return ((BZ1Helper::m_TurnCounter <= iter->DeployTimer)) || IsDeployed(iter->DispatchObject);
	}

	return false;
}

void DispatchHelper::DLLHunt(const Handle h, const int Priority)
{
	if(!IsAround(h))
		return;

	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
		{
			iter->DispatchType = DispatchType_Hunter;
			++BZ1Helper::Get().RefTeamInfo(GetTeamNum(h)).m_CPUNumHuntUnits;
			return;
		}
	}

	AddToDispatch(h, 0.0f, true, Priority, true, false, true);
}

void DispatchHelper::SetUseService(const Handle h, const bool Service)
{
	if(!IsAround(h))
		return;

	for (std::vector<DispatchClass>::iterator iter = DispatchList.begin(); iter != DispatchList.end(); ++iter)
	{
		if(iter->DispatchObject == h)
		{
			iter->CanGetReload = Service;
			iter->CanGetRepair = Service;
			return;
		}
	}

	AddToDispatch(h, 0.0f, true, 0, true, false, false, true);
}

void DispatchHelper::ClearDispatch(const int Index)
{
	DispatchClass Object = DispatchList[Index];

	Object.IdleTime = 0;
	Object.DispatchState = DispatchState_Idle;
	Object.DispatchTarget = 0;
	Object.DispatchSpot = Vector(0, 0, 0);
	SetCommand(Object.DispatchObject, CMD_NONE, 0);
}