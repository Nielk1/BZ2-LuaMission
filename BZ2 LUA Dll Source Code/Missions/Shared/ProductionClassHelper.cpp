#include "BZ1Helper.h"
#include "ProductionClassHelper.h"
//#include "LimiterHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

ProductionClassHelper *ProductionClassHelper::m_ProductionClassHelper;

ProductionClassHelper::ProductionClassHelper()
{
	m_ProductionClassHelper = this;
}
ProductionClassHelper::~ProductionClassHelper()
{
	m_ProductionClassHelper = NULL;
}

bool ProductionClassHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		CameraPodList.resize(size);
		if(size)
			Read(&CameraPodList.front(), sizeof(CameraPodClass)*size);
	}

	return true;
}

bool ProductionClassHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<CameraPodClass>::iterator iter = CameraPodList.begin(); iter != CameraPodList.end(); ++iter)
		{
			CameraPodClass &sao = *iter;
			ConvertHandles(&sao.CameraPodObject, 1);
		}
	}

	return true;
}

bool ProductionClassHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(CameraPodList.size());
		Write(&size, 1);
		if(size)
			Write(&CameraPodList.front(), sizeof(CameraPodClass)*size);
	}

	return true;
}

void ProductionClassHelper::AddCameraObject(const Handle h)
{
	for (std::vector<CameraPodClass>::iterator iter = CameraPodList.begin(); iter != CameraPodList.end(); ++iter)
	{
		if(iter->CameraPodObject == h)
			return;
	}

	CameraPodList.resize(CameraPodList.size()+1);
	CameraPodClass &sao = CameraPodList[CameraPodList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(CameraPodClass)); // Zero things out at start.

	sao.CameraPodObject = h; // Add the camera to the list.
}

void ProductionClassHelper::AddProductionClassObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int ObjClassHash, const int BZCClassHash)
{
	if((TempODF1 == NULL) || (TempODF2 == NULL))
		return;

	BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

	SetIndependence(h, 0); // Don't let AI do stuff on it's own.

	// Look for Scrap Holds for production units.
	bool UseScrapHold = false;
	char ScrapHoldName[MAX_ODF_LENGTH] = {0};
	//sprintf_s(ScrapHoldName, "\0"); // Fill this with nothing.
	if((GetODFString(TempODF1, TempODF2, "GameObjectClass", "ScrapHold", MAX_ODF_LENGTH, ScrapHoldName)) && (ScrapHoldName[0])) // Grab the value.
		UseScrapHold = true;

	// Handle Deploy Sound and DummyRadar for common classes.
	switch (BZCClassHash)
	{
	case BZCCLASS_RECYCLER:
	case BZCCLASS_FACTORY:
	case BZCCLASS_ARMORY:
		{
			// Play the "Deployed" message. Only play it AFTER game initial start. 
			if(IsBuilding(h))
			{
				if(BZ1Helper::m_TurnCounter > 1)
				{
					// For undeploy/deploy sounds on recy/muf/slf. 
					char DeploySound[MAX_ODF_LENGTH] = {0};
					if((GetODFString(TempODF1, TempODF2, "BuildingClass", "deployedMsg", MAX_ODF_LENGTH, DeploySound)) || 
						(GetODFString(TempODF1, TempODF2, "FactoryClass", "deployedMsg", MAX_ODF_LENGTH, DeploySound)))
					{
						// Got it.
					}
					if((GetLocalPlayerTeamNumber() == Team) && (DeploySound[0])) // If This belongs to YOU!
					{
						if(DoesFileExist(DeploySound))
							AudioMessage(DeploySound);
						else
							FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix deployMsg in ODF: %s", DeploySound, GetODFName(h)); 
					}
				}
			}
			else // Vehicle version, if RangeScan is 0, look for "DummyRadarObject" to place.
			{
				float RangeScan = 200.0f; // Build Dummy Radar, only if their ScannerRange is 0.
				if(GetODFFloat(TempODF1, TempODF2, "CraftClass", "rangeScan", &RangeScan))
				{
					if(RangeScan <= 0.0f)
					{
				//		char RadarODF[MAX_ODF_LENGTH] = {0};
				//		if(GetODFString(TempODF1, TempODF2, "GameObjectClass", "DummyRadarObject", MAX_ODF_LENGTH, RadarODF))
				//		{
							if(BZCClassHash == BZCCLASS_RECYCLER)
								teamInfo.RecyRadar = -1; //BuildObject(RadarODF, Team, h);
							else if(BZCClassHash == BZCCLASS_FACTORY)
								teamInfo.MufRadar = -1; //BuildObject(RadarODF, Team, h);
							else if(BZCClassHash == BZCCLASS_ARMORY)
								teamInfo.SlfRadar = -1; //BuildObject(RadarODF, Team, h);
				//		}
					}
				}
			}

			// If we havn't determined if we're using limits, look in production units to tell. 
			if(!teamInfo.m_UseLimits)
				GetODFBool(TempODF1, TempODF2, "GameObjectClass", "UseBZ1Limits", &teamInfo.m_UseLimits, false);

			// If we havn't determined if we're using pilots, look in production units to tell.
			if(!teamInfo.m_UsePilots)
				GetODFBool(TempODF1, TempODF2, "GameObjectClass", "UseBZ1Pilots", &teamInfo.m_UsePilots, false);

			if(!BZ1Helper::Get().RefBuildPools())
				GetODFBool(TempODF1, TempODF2, "GameObjectClass", "UseBZ2Pools", &BZ1Helper::Get().RefBuildPools(), false);

			char DeployODF[MAX_ODF_LENGTH] = {0};
			GetODFString(TempODF1, TempODF2, "GameObjectClass", "UndeployClass", MAX_ODF_LENGTH, DeployODF);
			if((DeployODF[0]) && (DoesODFExist(DeployODF)))
			{
				PetWatchdogThread();
				PreloadODF(DeployODF);
			}
		}
		break;
	}

	// Create ScrapHolds for BZC Production units.
	switch (BZCClassHash)
	{
	case BZCCLASS_RECYCLER:
		teamInfo.Recy = h; // Store it for watching RecyclePoint. 
		//BZ1Helper::Get().RefRecyclePoint(Team) = GetTap(h, 0); // Set this as the Recycler's Tap.
		teamInfo.TeamRace = GetRace(h); // Save the team's race. Note: Placing a new RECY will override the previous Race letter. -GBD
		teamInfo.RecyUseScrapHold = UseScrapHold;

		if((UseScrapHold) && (!IsAround(teamInfo.RecScrap)) && (BZ1Helper::m_TurnCounter > 0))
		{
			if(DoesODFExist(ScrapHoldName))
				teamInfo.RecScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
			else
				teamInfo.RecScrap = BuildObject("recyshold",Team,BZ1Helper::Where);
		}
		break;
	case BZCCLASS_FACTORY:
		teamInfo.Muf = h;
		teamInfo.MufUseScrapHold = UseScrapHold;
		if(teamInfo.TeamRace == 0)
			teamInfo.TeamRace = GetRace(h);

		if((UseScrapHold) && (!IsAround(teamInfo.MufScrap)) && (BZ1Helper::m_TurnCounter > 0))
		{
			if(DoesODFExist(ScrapHoldName))
				teamInfo.MufScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
			else
				teamInfo.MufScrap = BuildObject("mufshold",Team,BZ1Helper::Where);
		}
		break;
	case BZCCLASS_ARMORY:
		teamInfo.Slf = h; // Store this to do stuff later.
		teamInfo.SlfUseScrapHold = UseScrapHold;

	   	if((UseScrapHold) && (!IsAround(teamInfo.SlfScrap)) && (BZ1Helper::m_TurnCounter > 0))
		{
			if(DoesODFExist(ScrapHoldName))
				teamInfo.SlfScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
			else
				teamInfo.SlfScrap = BuildObject("slfshold",Team,BZ1Helper::Where);
		}
		break;
	case BZCCLASS_CONSTRUCTOR:
		teamInfo.Cnst = h;
		teamInfo.CnstUseScrapHold = UseScrapHold;

	   	if((UseScrapHold) && (!IsAround(teamInfo.CnstScrap)) && (BZ1Helper::m_TurnCounter > 0))
		{
			if(DoesODFExist(ScrapHoldName))
				teamInfo.CnstScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
			else
				teamInfo.CnstScrap = BuildObject("cnstshold",Team,BZ1Helper::Where);
		}
		break;
	}

}

void ProductionClassHelper::Execute(BZ1Helper &bz1Helper)
{
	// Cleanup list.
	CameraPodList.erase(std::remove_if(CameraPodList.begin(), CameraPodList.end(), ShouldRemoveCameraPod), CameraPodList.end());

	// Loop over Teams.
	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		int RealTeam = 0;

		BZ1Helper::TeamInfo &teamInfo = BZ1Helper::Get().RefTeamInfo(Team);

		if(IsAround(teamInfo.Recy))
		{
			RealTeam = GetTeamNum(teamInfo.Recy);
			if(RealTeam != Team)
			{
				BZ1Helper::TeamInfo &realTeamInfo = BZ1Helper::Get().RefTeamInfo(RealTeam);

				// Convert Recycler. 
				realTeamInfo.Recy = teamInfo.Recy;
				teamInfo.Recy = 0;
				if(!realTeamInfo.m_UsePilots)
					realTeamInfo.m_UsePilots = teamInfo.m_UsePilots; // Flag this too.
				if(!realTeamInfo.m_UseLimits)
					realTeamInfo.m_UseLimits = teamInfo.m_UseLimits;

				if(IsAround(teamInfo.RecScrap))
				{
					char ODFName[MAX_ODF_LENGTH] = {0};
					GetODFName(teamInfo.RecScrap, ODFName);
					Vector TempLoc = GetPosition(teamInfo.RecScrap);
					//int TempScrap = GetScrap(RealTeam);
					RemoveObject(teamInfo.RecScrap);
					realTeamInfo.RecScrap = BuildObject(ODFName,RealTeam,TempLoc);
					//SetScrap(RealTeam,TempScrap);
					teamInfo.RecScrap = 0; 
				}
			}
		}

		if(IsAround(teamInfo.Muf))
		{
			RealTeam = GetTeamNum(teamInfo.Muf);
			if(RealTeam != Team)
			{
				BZ1Helper::TeamInfo &realTeamInfo = BZ1Helper::Get().RefTeamInfo(RealTeam);

				// Convert Muf. 
				realTeamInfo.Muf = teamInfo.Muf;
				teamInfo.Muf = 0; 
				if(!realTeamInfo.m_UsePilots)
					realTeamInfo.m_UsePilots = teamInfo.m_UsePilots; // Flag this too.
				if(!realTeamInfo.m_UseLimits)
					realTeamInfo.m_UseLimits = teamInfo.m_UseLimits;

				if(IsAround(teamInfo.MufScrap))
				{
					char ODFName[MAX_ODF_LENGTH] = {0};
					GetODFName(teamInfo.MufScrap, ODFName);
					Vector TempLoc = GetPosition(teamInfo.MufScrap);
					//int TempScrap = GetScrap(RealTeam);
					RemoveObject(teamInfo.MufScrap);
					realTeamInfo.MufScrap = BuildObject(ODFName,RealTeam,TempLoc);
					//SetScrap(RealTeam,TempScrap);
					teamInfo.MufScrap = 0; 
				}
			}
		}

		if(IsAround(teamInfo.Slf))
		{
			RealTeam = GetTeamNum(teamInfo.Slf);
			if((RealTeam != Team) && (RealTeam > 0))
			{
				BZ1Helper::TeamInfo &realTeamInfo = BZ1Helper::Get().RefTeamInfo(RealTeam);

				// Convert SLF. 
				realTeamInfo.Slf = teamInfo.Slf;
				teamInfo.Slf = 0; 
				if(!realTeamInfo.m_UseLimits)
					realTeamInfo.m_UseLimits = teamInfo.m_UseLimits;

				if(IsAround(teamInfo.SlfScrap))
				{
					char ODFName[MAX_ODF_LENGTH] = {0};
					GetODFName(teamInfo.SlfScrap, ODFName);
					Vector TempLoc = GetPosition(teamInfo.SlfScrap);
					//int TempScrap = GetScrap(RealTeam);
					RemoveObject(teamInfo.SlfScrap);
					realTeamInfo.SlfScrap = BuildObject(ODFName,RealTeam,TempLoc);
					//SetScrap(RealTeam,TempScrap);
					teamInfo.SlfScrap = 0; 
				}
			}
		}

		if(IsAround(teamInfo.Cnst))
		{
			RealTeam = GetTeamNum(teamInfo.Cnst);
			if(RealTeam != Team)
			{
				BZ1Helper::TeamInfo &realTeamInfo = BZ1Helper::Get().RefTeamInfo(RealTeam);

				// Convert Cnst. 
				realTeamInfo.Cnst = teamInfo.Cnst;
				teamInfo.Cnst = 0;

				if(IsAround(teamInfo.CnstScrap))
				{
					char ODFName[MAX_ODF_LENGTH] = {0};
					GetODFName(teamInfo.CnstScrap, ODFName);
					Vector TempLoc = GetPosition(teamInfo.CnstScrap);
					//int TempScrap = GetScrap(RealTeam);
					RemoveObject(teamInfo.CnstScrap);
					realTeamInfo.CnstScrap = BuildObject(ODFName,RealTeam,TempLoc);
					//SetScrap(RealTeam,TempScrap);
					teamInfo.CnstScrap = 0; 
				}
			}
		}
		// End team reassignment. -GBD/N1

		// Begin ScrapHold Placement.
		if(IsAround(teamInfo.Recy))
		{
			if((teamInfo.RecyUseScrapHold) && (!IsAround(teamInfo.RecScrap)))
			{
				// Look for Scrap Holds for production units.
				char ScrapHoldName[MAX_ODF_LENGTH] = {0};
				if((GetODFString(teamInfo.Recy, "GameObjectClass", "ScrapHold", MAX_ODF_LENGTH, ScrapHoldName)) && (ScrapHoldName[0])) // Grab the value.
				{
					// Got it.
				}
				if(DoesODFExist(ScrapHoldName))
					teamInfo.RecScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
				else
					teamInfo.RecScrap = BuildObject("recyshold",Team,BZ1Helper::Where);
			}
			// Dummy Radar
			if(teamInfo.RecyRadar < 0)
			{
				char RadarODF[MAX_ODF_LENGTH] = {0};
				if(GetODFString(teamInfo.Recy, "GameObjectClass", "DummyRadarObject", MAX_ODF_LENGTH, RadarODF))
					teamInfo.RecyRadar = BuildObject(RadarODF, Team, teamInfo.Recy);
				else
					teamInfo.RecyRadar = 0; // Nothing, don't build it.
			}
			else
			{
				if(IsBuilding(teamInfo.Recy))
					RemoveObject(teamInfo.RecyRadar);
				else
					SetVectorPosition(teamInfo.RecyRadar, GetPosition(teamInfo.Recy));
			}
		}
		else
		{
			teamInfo.Recy = 0; // Zero this out so checks will fail.
			if(IsAround(teamInfo.RecScrap))
				RemoveObject(teamInfo.RecScrap);
			if(IsAround(teamInfo.RecyRadar))
				RemoveObject(teamInfo.RecyRadar);
		}

		if(IsAround(teamInfo.Muf))
		{
			if((teamInfo.MufUseScrapHold) && (!IsAround(teamInfo.MufScrap)))
			{
				// Look for Scrap Holds for production units.
				char ScrapHoldName[MAX_ODF_LENGTH] = {0};
				if((GetODFString(teamInfo.Muf, "GameObjectClass", "ScrapHold", MAX_ODF_LENGTH, ScrapHoldName)) && (ScrapHoldName[0])) // Grab the value.
				{
					// Got it.
				}

				if(DoesODFExist(ScrapHoldName))
					teamInfo.MufScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
				else
					teamInfo.MufScrap = BuildObject("mufshold",Team,BZ1Helper::Where);
			}
			// Dummy Radar
			if(teamInfo.MufRadar < 0)
			{
				char RadarODF[MAX_ODF_LENGTH] = {0};
				if(GetODFString(teamInfo.Muf, "GameObjectClass", "DummyRadarObject", MAX_ODF_LENGTH, RadarODF))
					teamInfo.MufRadar = BuildObject(RadarODF, Team, teamInfo.Muf);
				else
					teamInfo.MufRadar = 0; // Nothing, don't build it.
			}
			else
			{
				if(IsBuilding(teamInfo.Muf))
					RemoveObject(teamInfo.MufRadar);
				else
					SetVectorPosition(teamInfo.MufRadar, GetPosition(teamInfo.Muf));
			}
		}
		else 
		{
			teamInfo.Muf = 0;
			if(IsAround(teamInfo.MufScrap))
				RemoveObject(teamInfo.MufScrap);
			if(IsAround(teamInfo.MufRadar))
				RemoveObject(teamInfo.MufRadar);
		}

		if(IsAround(teamInfo.Slf))
		{
			if((teamInfo.SlfUseScrapHold) && (!IsAround(teamInfo.SlfScrap)))
			{
				// Look for Scrap Holds for production units.
				char ScrapHoldName[MAX_ODF_LENGTH] = {0};
				if((GetODFString(teamInfo.Slf, "GameObjectClass", "ScrapHold", MAX_ODF_LENGTH, ScrapHoldName)) && (ScrapHoldName[0])) // Grab the value.
				{
					// Got it.
				}

				if(DoesODFExist(ScrapHoldName))
					teamInfo.SlfScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
				else
					teamInfo.SlfScrap = BuildObject("slfshold",Team,BZ1Helper::Where);
			}
			// Dummy Radar
			if(teamInfo.SlfRadar < 0)
			{
				char RadarODF[MAX_ODF_LENGTH] = {0};
				if(GetODFString(teamInfo.Slf, "GameObjectClass", "DummyRadarObject", MAX_ODF_LENGTH, RadarODF))
					teamInfo.SlfRadar = BuildObject(RadarODF, Team, teamInfo.Slf);
				else
					teamInfo.SlfRadar = 0; // Nothing, don't build it.
			}
			else
			{
				if(IsBuilding(teamInfo.Slf))
					RemoveObject(teamInfo.SlfRadar);
				else
					SetVectorPosition(teamInfo.SlfRadar, GetPosition(teamInfo.Slf));
			}
		}
		else
		{
			teamInfo.Slf = 0;
			if(IsAround(teamInfo.SlfScrap))
				RemoveObject(teamInfo.SlfScrap);
			if(IsAround(teamInfo.SlfRadar))
				RemoveObject(teamInfo.SlfRadar);
		}

		if(IsAround(teamInfo.Cnst))
		{
			if((teamInfo.CnstUseScrapHold) && (!IsAround(teamInfo.CnstScrap)))
			{
				// Look for Scrap Holds for production units.
				char ScrapHoldName[MAX_ODF_LENGTH] = {0};
				if((GetODFString(teamInfo.Cnst, "GameObjectClass", "ScrapHold", MAX_ODF_LENGTH, ScrapHoldName)) && (ScrapHoldName[0])) // Grab the value.
				{
					// Got it.
				}

				if(DoesODFExist(ScrapHoldName))
					teamInfo.CnstScrap = BuildObject(ScrapHoldName,Team,BZ1Helper::Where);
				else
					teamInfo.CnstScrap = BuildObject("cnstshold",Team,BZ1Helper::Where);
			}
		}
		else
		{
			teamInfo.Cnst = 0;
			if(IsAround(teamInfo.CnstScrap))
				RemoveObject(teamInfo.CnstScrap);
		}


// SLF Specific code, used to make Armories more like BZ1.

		bool Logging = BZ1Helper::Logging;

		if((IsAround(teamInfo.Slf)) && (IsBuilding(teamInfo.Slf)))
		{
			// Clear the previous Target if it's moved too far away.
			if((IsAround(teamInfo.SLFTarget)) && (GetDistance3DSquared(teamInfo.Slf, teamInfo.SLFTarget) > Squared(10.0f)))
				teamInfo.SLFTarget = 0;

			// Get the nearest powerup. 
			Handle BuiltPowerup = GetNearestPowerup(teamInfo.Slf, true, 10.0f); //5.0f);

		//-- Old code, 1.3 PB6.5 and < didn't detect Camera Pods in GetNearestPowerup(), so we have to look for them ourselves. -GBD
			if((BZ1Helper::m_GameVersion <= v13PB65) && (!BuiltPowerup))
			{
				for (std::vector<CameraPodClass>::iterator iter = CameraPodList.begin(); iter != CameraPodList.end(); ++iter)
				{
					if(GetDistance3DSquared(teamInfo.Slf, iter->CameraPodObject) < Squared(25.0f))
					{
						BuiltPowerup = iter->CameraPodObject;
						break;
					}
				}
			}

			bool PlayAnimAndVO = false;

		//---- New Code, uses GetVelocity on the powerup. Only works on 1.3 PB7+. -GBD
			if(BZ1Helper::m_GameVersion >= v13PB7)
			{
				// check if the powerup is moving away 
				Vector SlfPosition = GetPosition(teamInfo.Slf);
				Vector PowerupPosition = GetPosition(BuiltPowerup);
				Vector PowerupVelocity = GetVelocity(BuiltPowerup);
				Vector Direction = Sub_Vectors(PowerupPosition, SlfPosition);

				if (Dot_Product(Direction, PowerupVelocity) > 0)
				{
					PlayAnimAndVO = true;
				}
			}
			else
			{
		//---- Old Code, 1.3 PB6.5 and < don't return GetVelocity for powerups, so we have to do a manual distance check. Eeeek! -GBD
				if((!teamInfo.m_Distance1Check) && (!teamInfo.m_Distance2Check))
				{
					teamInfo.Distance1 = FLT_MAX;
					teamInfo.Distance2 = FLT_MAX;
				}

				if(BuiltPowerup != 0)
				{
					if(!teamInfo.m_Distance1Check) //&& (!teamInfo.m_Distance2Check))
					{
						teamInfo.Distance1 = GetDistance3DSquared(teamInfo.Slf, BuiltPowerup); //Distance; //GetDistance(teamInfo.Slf, BuiltPowerup);
						teamInfo.m_Distance1Check = true;

					//	FormatLogMessage("Distance1 Is: %f", teamInfo.Distance1); 
					}
					//if((!teamInfo.m_Distance2Check) && (teamInfo.m_Distance1Check))
					if(teamInfo.m_Distance1Check)
					{
						teamInfo.Distance2 = GetDistance3DSquared(teamInfo.Slf, BuiltPowerup); //Distance; //GetDistance(teamInfo.Slf, BuiltPowerup);
						if(teamInfo.Distance2 != teamInfo.Distance1)
							teamInfo.m_Distance1Check = false;
					//		teamInfo.m_Distance2Check = true;

					//	FormatLogMessage("Distance2 Is: %f", teamInfo.Distance2); 
					}
				}
				else
				{
					teamInfo.m_Distance1Check = false;
					teamInfo.m_Distance2Check = false;
				}
				// /*

				if((teamInfo.Distance2 > (teamInfo.Distance1 + Squared(0.6f))) && ((teamInfo.Distance1+teamInfo.Distance2) < Squared(40.0f)))
				{
					PlayAnimAndVO = true;
				}
			}

			if((BuiltPowerup != 0) && (!teamInfo.SlfPlayingAnim) && (BuiltPowerup != teamInfo.SLFTarget) && (PlayAnimAndVO))
			{
				teamInfo.SLFTarget = BuiltPowerup;

				char SoundFile[MAX_ODF_LENGTH] = {0};
				char AnimationName[MAX_ODF_LENGTH] = {0};
				bool HasAnim1 = false;

				char PowerupClass[MAX_ODF_LENGTH] = {0};
				GetObjInfo(BuiltPowerup, Get_GOClass, PowerupClass);
				unsigned int PowerupClassHash = Hash(PowerupClass);

				char TempODF1[MAX_ODF_LENGTH] = {0};
				char TempODF2[MAX_ODF_LENGTH] = {0};
				GetAndOpenODFNames(teamInfo.Slf, TempODF1, TempODF2);

				GetODFString(TempODF1, TempODF2, "ArmoryClass", "reloadMsg", MAX_ODF_LENGTH, SoundFile);
				if((SoundFile[0]) && (GetLocalPlayerTeamNumber() == Team))
				{
					if(DoesFileExist(SoundFile))
						 AudioMessage(SoundFile);
					else
						FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix ODF: %s", SoundFile, GetODFName(teamInfo.Slf)); 
				}

				// Decide which Sound File to play when the Armory builds a Powerup. BZ2 doesn't support Armory Build VO's, let alone different types. :(
				switch (PowerupClassHash)
				{
				case CLASS_POWERUP_RELOAD:				// If it's an Ammo powerup, grab that soundfile. 
					GetODFString(TempODF1, TempODF2, "ArmoryClass", "ammoSound", MAX_ODF_LENGTH, SoundFile);
					break;
				case CLASS_POWERUP_REPAIR:				// If it's a Repair powerup, grab that soundfile. 
					GetODFString(TempODF1, TempODF2, "ArmoryClass", "repaSound", MAX_ODF_LENGTH, SoundFile);
					break;
				case CLASS_POWERUP_CAMERA:				// If it's a Camera powerup, grab that soundfile. 
				case CLASS_NAVBEACON:					// Include BZ2 style Navs too, just in case. (Though I don't think those can be thrown by an armory)
					GetODFString(TempODF1, TempODF2, "ArmoryClass", "navSound", MAX_ODF_LENGTH, SoundFile);
					break;
				case CLASS_WEAPON:						// If it's a Weapon powerup, grab that soundfile. 
					GetODFString(TempODF1, TempODF2, "ArmoryClass", "weaponSound", MAX_ODF_LENGTH, SoundFile);
					break;
				case CLASS_DAYWRECKER:					// If it's a Bomb powerup, grab that soundfile. 
					GetODFString(TempODF1, TempODF2, "ArmoryClass", "bombSound", MAX_ODF_LENGTH, SoundFile);
					break;
				default:								// If it's not a normal armory throwable thing, such as a constructor, say something funny.
					GetODFString(TempODF1, TempODF2, "ArmoryClass", "rescueMsg", MAX_ODF_LENGTH, SoundFile);
					break;
				}

				if(Logging)
					FormatLogMessage("Sound Is: %s", SoundFile); 

				if((SoundFile[0]) && (GetLocalPlayerTeamNumber() == Team))
				{
					if(DoesFileExist(SoundFile))
						 AudioMessage(SoundFile);
					else
						FormatLogMessage("ERROR: Sound file: %s doesn't exist! Fix ODF: %s", SoundFile, GetODFName(teamInfo.Slf)); 
				}

				// Open it, grab out the var.
				int AnimationCount = 0;
				GetODFInt(TempODF1, TempODF2, "GameObjectClass", "animCount", &AnimationCount);
				for (int i = 1; i < AnimationCount+1; i++) // Look through 16 animnames. If you made a unit that has more then 16 anims, 1: Congratulations, and 2: The loop and off ones better be between 1 and 16.
				{
					char DesiredValue[MAX_ODF_LENGTH] = {0};
					sprintf_s(DesiredValue, "animName%d", i);
					if(GetODFString(TempODF1, TempODF2, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, AnimationName)) // Grab the animation name.
					{
						if((_stricmp(AnimationName,"build")) == 0)
						{
							HasAnim1 = true;
							break; // We found it, abort!
						}
					}
					else
					{
						break;
					}
				}

				if(HasAnim1)
				{
					teamInfo.SLFAnimationFrames = SetAnimation(teamInfo.Slf, "build", 1); // Restart the animation. 
					StartAnimation(teamInfo.Slf);
					teamInfo.SlfPlayingAnim = true;
				}
			}
			if(teamInfo.SlfPlayingAnim)
			{
				if(GetAnimationFrame(teamInfo.Slf, "build") >= teamInfo.SLFAnimationFrames-1.0f)
				{
					//SetAnimation(teamInfo.Slf, NULL, 1); // Restart the animation to nothing, it'll automatically reverse. 
					teamInfo.SLFAnimationFrames = SetAnimation(teamInfo.Slf, "BZCloop", 0);
					StartAnimation(teamInfo.Slf);
					teamInfo.SlfPlayingAnim = false;
				}
			}
		}

	} // Loop over teams.
}
