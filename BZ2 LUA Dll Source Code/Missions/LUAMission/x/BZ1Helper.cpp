#include "BZ1Helper.h"
#include <share.h>

#include "PUPMgr.h" // PupMgr, now globally available. :) Also added _team# optional param to the format, "ODF_Timer_team#_Index", i.e. "aprepa_5_team2_1", or "aprepa_5_1". The former will put it on Team 2. The latter will default to Team 0.
//#include "TapAssignmentHelper.h" // This code handles tap upgrading, because of a bug in the BZ2 engine. (TEMPORARY! Remove when bug is fixed!!!) Also handles updating rotation/position when a host object moves. (via DLL/Editor) (Moved to BuildingHelper)
#include "ProductionClassHelper.h" // This code handles scraphold management, SLF sound/animations, and production class team switching.
#include "RecycleHelper.h" // This code creates dummy recyclers to allow units to recycle at a TEAM_SLOT_5 recycler, and at silos if recycler is not around.
#include "PilotResourceHelper.h" // This code handles the pilot resource and accessor functions. It also handles Pilot AI control, making them recycle at Recy or Barracks, or get in ships, or shoot you.
#include "BuildingHelper.h" // This code handles buildings powered by functionality, based on: BZ1 (distance within a power plant), BZ2 (Power) or Lung (TAPS) // This code handles tap upgrading, because of a bug in the BZ2 engine. (TEMPORARY! Remove when bug is fixed!!!) Also handles updating rotation/position when a host object moves. (via DLL/Editor)
#include "APCHelper.h" // This code controls the artificial APC AI, which returns to the nearest barracks/recy and refills ammo. It also prevents a player from dispensing soldiers when not in range of a valid target (BZ1.5 behavior).
#include "HQCPHelper.h" // This code controls the HQCP, a powered sensor class with filter and sound options.
#include "LPadHelper.h" // This code controls the LPAD, a landing site for periodic refilling of scrap, pilots, and/or units.
#include "PortalHelper.h" // This code is portals, buildable, configurable, because the cake is a lie.
#include "AttacherHelper.h" // This code handles attached objects, i.e. Vehicle taps. Taps can be attached to anything, just avoid an infinite loop. It also handles Cerberus Tail animations/attack.
#include "ReplaceOnDeathHelper.h" // This code replaces a dead object with another object, optionally with explosion or without. Useful for making buildings turn into debris.
#include "UtilityShipHelper.h" // This code handles Scavenger's scrap multiplier in Multiplayer. 
#include "PowerHelper.h" // This code plays the "Power Lost" wave sound when a BZ1 (fake) Power Plant is lost. It also manages DLL driven Power for Comm, HQCP, Towe, Jamm, and Shld class DLL driven codes.
#include "DeployHelper.h" // This code allows for Aircraft, APCs, and Bombers to have a deploy animation.
#include "FuryHelper.h" // This code handles the easter egg Fury class, which has a % chance every minute to turn against you.
#include "LavaDamageHelper.h" // This code creates a damage field within a path area, within x meters off the ground, with height falloff. (less damage higher up) 
#include "ScrapHelper.h" // This code is a cull function to keep scrap from overflowing the map in Single Player. Max of 2048 scrap at a time.
#include "DispatchHelper.h" // This code handles artificial AI dispatch functions including: Turrets, Minelayers, Patrols, Hunt, Service, Flee, and Cloaking. Also handles AI team launching day wreckers.
#include "SatelliteHelper.h" // This code creates the remote controlled Satellite, and handles all the switching around to use it. 
#include "LimiterHelper.h" // This code creates the limiting of 10 offensive, defensive, and utility units, and omits the player's craft from the count. 
#include "SwitchClassHelper.h" // This code creates the MAS Grenade "switchclass" weapon.
#include "AnchorClassHelper.h" // This code creates an artificial Anchor class weapon (SPEED only, doesnt effect turning).
#include "AmmoDrainHelper.h" // This code creates a Sticky (anchor? lockshell?) class weapon that drains its target's ammo and feeds it to the ship that shot it.
#include "UndeployHelper.h" // This code handles Production Unit undeploying. 
#include "QuakeHelper.h" // This code creates the "quakeblast" effect of the Day Wrecker.
#include "GeyserHelper.h" // This code creates dummy Geysers to appear under deployed production units, and creates BZ2 Pool objects if a BZ2 race is present. 
#include "AIWeaponHelper.h" // This code handles giving the AI random weapons and weaponmasks based on individual config files, taking requireNames into account.
#include "AttacherWeaponHelper.h" // This code creates an attached weapon item, similar to the AttacherHelper, but is toggled on/off via a dispenser weapon. 
#include "MorphObjectHelper.h" // This code handles the swapping of an object with another object via a dispenser weapon toggle. Used to make Cloaking Walkers, Flying Maulers, etc.
#include "ShieldHelper.h" // This code handles the Shield class, which creates a wall of shieldtower class objects between it and another Shield class. Optionally uses ODF Offsets as shield emitters, so a shield class could have up to 8 emitters linking to other shields.
#include "EnergyArmorHelper.h" // This creates an armored unit that absorbs energy from enemy weapons fire and uses it to regain health over time.
#include "JammerHelper.h" // This code handles tap swapping to make a Jammer that requires power switch on/off.
#include "CarrierHelper.h" // This code handles "Carrier" ships and their Drones. Drones target whatever the Carrier targets, has the drones defend the Carrier, and returns the Drones to the bay when there's no enemies around.
#include "TransportHelper.h" // This code handles Transports for landing and giving scrap, pilots, and/or units. Used in LPad code and Reinforcements option.
#include "DropShipHelper.h" // This code handles DropShip class objects, which can load other units into them and carry them to another location to be unloaded.
#include "RecallMineHelper.h" // This code is Neilk1's Recall Mine. It allows a ship to drop a pair of mines that teleport it, like a pre-placed Blink device.

// enable floating-point exceptions
//unsigned int prev = _controlfp_s(0, _EM_ZERODIVIDE|_EM_INVALID);

BZ1Helper *BZ1Helper::m_BZ1Helper;

// Static bullocks, These are NOT saved. -GBD
//static char TempODFName[MAX_ODF_LENGTH];
static bool GotPaths = false;

// These ARE saved. 
int BZ1Helper::m_GameTPS;
int BZ1Helper::m_TurnCounter;
int BZ1Helper::TotalMapObjects;
Vector BZ1Helper::Where;
char BZ1Helper::WorldSetting;
int BZ1Helper::m_GameType;
int BZ1Helper::m_GameVersion;
float BZ1Helper::m_ModVersion;

// These are NOT saved.
bool BZ1Helper::Logging; // Flag for printing stuffs.

// BZ1 Interface secondary Health/Ammo bars.
float BZ1Helper::HullGauge2;
float BZ1Helper::AmmoGauge2;
float BZ1Helper::AuxGauge;
bool BZ1Helper::HullGaugeSet;
bool BZ1Helper::AmmoGaugeSet;
bool BZ1Helper::AuxGaugeSet;

// Constrctor. (No, not the NSDF kind...)
BZ1Helper::BZ1Helper(void)
{
	// create helper classes
	m_PUPMgr = new PUPMgr();
	//m_TapAssignmentHelper = new TapAssignmentHelper();
	m_ProductionClassHelper = new ProductionClassHelper();
	m_RecycleHelper = new RecycleHelper();
	m_PilotResourceHelper = new PilotResourceHelper();
	m_PowerHelper = new PowerHelper();
	m_BuildingHelper = new BuildingHelper();
	m_APCHelper = new APCHelper();
	m_HQCPHelper = new HQCPHelper();
	m_LPadHelper = new LPadHelper();
	m_PortalHelper = new PortalHelper();
	m_AttacherHelper = new AttacherHelper();
	m_ReplaceOnDeathHelper = new ReplaceOnDeathHelper();
	m_UtilityShipHelper = new UtilityShipHelper();
	m_DeployHelper = new DeployHelper();
	m_FuryHelper = new FuryHelper();
	m_LavaDamageHelper = new LavaDamageHelper();
	m_ScrapHelper = new ScrapHelper();
	m_DispatchHelper = new DispatchHelper();
	m_SatelliteHelper = new SatelliteHelper();
	m_LimiterHelper = new LimiterHelper();
	m_SwitchClassHelper = new SwitchClassHelper();
	m_AnchorClassHelper = new AnchorClassHelper();
	m_AmmoDrainHelper = new AmmoDrainHelper();
	m_UndeployHelper = new UndeployHelper();
	m_QuakeHelper = new QuakeHelper();
	m_GeyserHelper = new GeyserHelper();
	m_AIWeaponHelper = new AIWeaponHelper();
	m_AttacherWeaponHelper = new AttacherWeaponHelper();
	m_MorphObjectHelper = new MorphObjectHelper();
	m_ShieldHelper = new ShieldHelper();
	m_EnergyArmorHelper = new EnergyArmorHelper();
	m_JammerHelper = new JammerHelper();
	m_CarrierHelper = new CarrierHelper();
	m_TransportHelper = new TransportHelper();
	m_DropShipHelper = new DropShipHelper();
	m_RecallMineHelper = new RecallMineHelper();

	//m_GameTPS = 10; // default
	EnableHighTPS(m_GameTPS);
	//WantBotKillMessages(); // For Object Ejected/Killed/Sniped functions. Only works for Players. :(
	b_count = &b_last - &b_first - 1;
	b_array = &b_first + 1;

	f_count = &f_last - &f_first - 1;
	f_array = &f_first + 1;

	/*
    h_count = &h_last - &h_first - 1;
	h_array = &h_first + 1;
	*/

	i_count = &i_last - &i_first - 1;
	i_array = &i_first + 1;

	// Zero things out first off.
	if(i_array)
		memset(i_array, 0, i_count*sizeof(int));
	if(f_array)
		memset(f_array, 0, f_count*sizeof(float));
//	if(h_array)
//		memset(h_array, 0, h_count*sizeof(Handle));
	if(b_array)
		memset(b_array, 0, b_count*sizeof(bool));

	// Special things, zero them out too.

	m_TurnCounter = 0;
	TotalMapObjects = 0;
	WorldSetting = 0;

	AIpathCount = 0;
	AIpathNames = NULL;

	HullGauge2 = 0;
	AmmoGauge2 = 0;
	AuxGauge = 0;
	HullGaugeSet = false;
	AmmoGaugeSet = false;
	AuxGaugeSet = false;

	memset(&Where, 0, sizeof(Where));
	Where = Vector(-4000, 0, -4000);

	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if(OpenODF2(mapTrnFile))
		GetODFChar(mapTrnFile, "World", "WorldLetter", &WorldSetting); // Grab the world letter.

	Logging = GetVarItemInt("network.session.ivar115"); // Logging Flag.

	// Memset these to 0.
	memset(&mapInfo, 0, sizeof(mapInfo));
	memset(&teamInfo, 0, sizeof(teamInfo));
	memset(&gravityInfo, 0, sizeof(gravityInfo));

	m_GameVersion = Get13Version();
	m_ModVersion = GetBZCVersion();

	OpenLogFile();

	m_BZ1Helper = this;
}

BZ1Helper::~BZ1Helper()
{
	CloseOpenODFs();
	CloseLogFile();
	CloseOpenFiles();

	FormatLogMessage("Game Ended.\n");

	// delete helper classes
	delete m_PUPMgr;
	//delete m_TapAssignmentHelper;
	delete m_ProductionClassHelper;
	delete m_RecycleHelper;
	delete m_PilotResourceHelper;
	delete m_PowerHelper;
	delete m_BuildingHelper;
	delete m_APCHelper;
	delete m_HQCPHelper;
	delete m_LPadHelper;
	delete m_PortalHelper;
	delete m_AttacherHelper;
	delete m_ReplaceOnDeathHelper;
	delete m_UtilityShipHelper;
	delete m_DeployHelper;
	delete m_FuryHelper;
	delete m_LavaDamageHelper;
	delete m_ScrapHelper;
	delete m_DispatchHelper;
	delete m_SatelliteHelper;
	delete m_LimiterHelper;
	delete m_SwitchClassHelper;
	delete m_AnchorClassHelper;
	delete m_AmmoDrainHelper;
	delete m_UndeployHelper;
	delete m_QuakeHelper;
	delete m_GeyserHelper;
	delete m_AIWeaponHelper;
	delete m_AttacherWeaponHelper;
	delete m_MorphObjectHelper;
	delete m_ShieldHelper;
	delete m_EnergyArmorHelper;
	delete m_JammerHelper;
	delete m_CarrierHelper;
	delete m_TransportHelper;
	delete m_DropShipHelper;
	delete m_RecallMineHelper;

	 m_BZ1Helper = NULL;
}

// Load data.
bool BZ1Helper::Load(bool missionSave)
{
	if (missionSave) {
		int i;
		EnableHighTPS(m_GameTPS);

		// init bools
		if((b_array) && (b_count))
			for (i = 0; i < b_count; i++)
				b_array[i] = false;

		// init floats
		if((f_array) && (f_count))
			for (i = 0; i < f_count; i++)
				f_array[i] = 0.0f;

		/*
		// init handles
		if((h_array) && (h_count))
			for (i = 0; i < h_count; i++)
				h_array[i] = 0;
		*/

		// init ints
		if((i_array) && (i_count))
			for (i = 0; i < i_count; i++)
				i_array[i] = 0;

		m_PUPMgr->Load(missionSave);
		//m_TapAssignmentHelper->Load(missionSave);
		m_ProductionClassHelper->Load(missionSave);
		//m_RecycleHelper->Load(missionSave);
		m_PilotResourceHelper->Load(missionSave);
		m_PowerHelper->Load(missionSave);
		m_BuildingHelper->Load(missionSave);
		m_APCHelper->Load(missionSave);
		m_HQCPHelper->Load(missionSave);
		m_LPadHelper->Load(missionSave);
		m_PortalHelper->Load(missionSave);
		m_AttacherHelper->Load(missionSave);
		m_ReplaceOnDeathHelper->Load(missionSave);
		m_UtilityShipHelper->Load(missionSave);
		m_DeployHelper->Load(missionSave);
		m_FuryHelper->Load(missionSave);
		//m_LavaDamageHelper->Load(missionSave);
		m_ScrapHelper->Load(missionSave);
		m_DispatchHelper->Load(missionSave);
		m_SatelliteHelper->Load(missionSave);
		//m_LimiterHelper->Load(missionSave);
		m_SwitchClassHelper->Load(missionSave);
		m_AnchorClassHelper->Load(missionSave);
		m_AmmoDrainHelper->Load(missionSave);
		m_UndeployHelper->Load(missionSave);
		m_QuakeHelper->Load(missionSave);
		m_GeyserHelper->Load(missionSave);
		//m_AIWeaponHelper->Load(missionSave);
		m_AttacherWeaponHelper->Load(missionSave);
		m_MorphObjectHelper->Load(missionSave);
		m_ShieldHelper->Load(missionSave);
		m_EnergyArmorHelper->Load(missionSave);
		m_JammerHelper->Load(missionSave);
		m_CarrierHelper->Load(missionSave);
		m_TransportHelper->Load(missionSave);
		m_DropShipHelper->Load(missionSave);
		m_RecallMineHelper->Load(missionSave);

		Setup(); // Run this to re initialize the proper positions. 
		return true;
	}

	int size;

	Read(static_cast<void *>(&mapInfo), sizeof(MapInfo));
	
	Read(&size, 1);
	HandleList.resize(size);
	if(size)
		Read(&HandleList.front(), sizeof(ObjectClass)*size);

	/*
	Read(&size, 1);
	BuildingList.resize(size);
	if(size)
		Read(&BuildingList.front(), sizeof(BuildingClass)*size);
	*/

	Read(&size, 1);
	AlignTerrainReplaceList.resize(size);
	if(size)
		Read(&AlignTerrainReplaceList.front(), sizeof(AlignTerrainReplaceClass)*size);
	
	Read(&size, 1);
	DeleteAfterDelayList.resize(size);
	if(size)
		Read(&DeleteAfterDelayList.front(), sizeof(DeleteAfterDelayClass)*size);

	for(int i = 0; i < MAX_TEAMS; i++)
		Read(&teamInfo[i], sizeof(TeamInfo));

	for(int i = 0; i < MAX_GRAVITY_TYPES; i++)
		Read(&gravityInfo[i], sizeof(GravityInfo));

	/*
	Read(&size, 1);
	MissionAudioList.resize(size);
	if(size)
		Read(&MissionAudioList.front(), sizeof(MissionAudioClass)*size);
	*/

	bool ret = true;

	// bools
	if (b_array != NULL)
		ret = ret && Read(b_array, b_count);

	// floats
	if (f_array != NULL)
		ret = ret && Read(f_array, f_count);

	/*
	// Handles
	if (h_array != NULL)
		ret = ret && Read(h_array, h_count);
	*/

	// ints
	if (i_array != NULL)
		ret = ret && Read(i_array, i_count);

	ret = ret && m_PUPMgr->Load(missionSave);
	//ret = ret && m_TapAssignmentHelper->Load(missionSave);
	ret = ret && m_ProductionClassHelper->Load(missionSave);
	//ret = ret && m_RecycleHelper->Load(missionSave);
	ret = ret && m_PilotResourceHelper->Load(missionSave);
	ret = ret && m_PowerHelper->Load(missionSave);
	ret = ret && m_BuildingHelper->Load(missionSave);
	ret = ret && m_APCHelper->Load(missionSave);
	ret = ret && m_HQCPHelper->Load(missionSave);
	ret = ret && m_LPadHelper->Load(missionSave);
	ret = ret && m_PortalHelper->Load(missionSave);
	ret = ret && m_AttacherHelper->Load(missionSave);
	ret = ret && m_ReplaceOnDeathHelper->Load(missionSave);
	ret = ret && m_UtilityShipHelper->Load(missionSave);
	ret = ret && m_DeployHelper->Load(missionSave);
	ret = ret && m_FuryHelper->Load(missionSave);
	//ret = ret && m_LavaDamageHelper->Load(missionSave);
	ret = ret && m_ScrapHelper->Load(missionSave);
	ret = ret && m_DispatchHelper->Load(missionSave);
	ret = ret && m_SatelliteHelper->Load(missionSave);
	//ret = ret && m_LimiterHelper->Load(missionSave);
	ret = ret && m_SwitchClassHelper->Load(missionSave);
	ret = ret && m_AnchorClassHelper->Load(missionSave);
	ret = ret && m_AmmoDrainHelper->Load(missionSave);
	ret = ret && m_UndeployHelper->Load(missionSave);
	ret = ret && m_QuakeHelper->Load(missionSave);
	ret = ret && m_GeyserHelper->Load(missionSave);
	//ret = ret && m_AIWeaponHelper->Load(missionSave);
	ret = ret && m_AttacherWeaponHelper->Load(missionSave);
	ret = ret && m_MorphObjectHelper->Load(missionSave);
	ret = ret && m_ShieldHelper->Load(missionSave);
	ret = ret && m_EnergyArmorHelper->Load(missionSave);
	ret = ret && m_JammerHelper->Load(missionSave);
	ret = ret && m_CarrierHelper->Load(missionSave);
	ret = ret && m_TransportHelper->Load(missionSave);
	ret = ret && m_DropShipHelper->Load(missionSave);
	ret = ret && m_RecallMineHelper->Load(missionSave);

	ret = ret && Read(&(m_TurnCounter),1);
	ret = ret && Read(&(TotalMapObjects),1);
	ret = ret && Read(&(Where),sizeof(Where));
	ret = ret && Read(&(WorldSetting),sizeof(WorldSetting));
	ret = ret && Read(&(m_GameType),1);
	ret = ret && Read(&(m_GameVersion),1);
	ret = ret && Read(&(m_ModVersion),1);

	// Set this right after reading -- we might be on a client.  Safe to call this multiple times on the server.
	SetGravity(mapInfo.Gravity);

	return ret;
}

// Post Load/Convert data. (This pretty much exists only to convert handles.)
bool BZ1Helper::PostLoad(bool missionSave)
{
	if (missionSave)
		return true;

	for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
	{
		ObjectClass &sao = *iter;
		ConvertHandles(&sao.ObjectHandle, 1);
	}

	/*
	for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
	{
		BuildingClass &sao = *iter;
		ConvertHandles(&sao.BuildingHandle, 1);
	}
	*/

	for (std::vector<AlignTerrainReplaceClass>::iterator iter = AlignTerrainReplaceList.begin(); iter != AlignTerrainReplaceList.end(); ++iter)
	{
		AlignTerrainReplaceClass &sao = *iter;
		ConvertHandles(&sao.AlignHandle, 1);
	}

	for (std::vector<DeleteAfterDelayClass>::iterator iter = DeleteAfterDelayList.begin(); iter != DeleteAfterDelayList.end(); ++iter)
	{
		DeleteAfterDelayClass &sao = *iter;
		ConvertHandles(&sao.DeleteAfterDelayHandle, 1);
	}

	bool ret = true;

	// Special note: ANY Handles not stored between h_first/h_last NEED to be specified here to convert. -GBD
	//ConvertHandles(h_array, h_count);

	for(int i = 0; i < MAX_TEAMS; i++)
		teamInfo[i].ConvertHandles();


	/*
	// Logging to check the return values of the parameters.
	std::vector<ObjectClass> TempHandleBuffer = HandleList;
	// Our current values differ from the recieved data, report what's different.
	if ((!TempHandleBuffer.empty() && !HandleList.empty()) && 
		(TempHandleBuffer.size() != HandleList.size() || memcmp(&TempHandleBuffer.front(), &HandleList.front(), TempHandleBuffer.size() * sizeof(TempHandleBuffer[0]) != 0)))
	{
		std::pair<std::vector<ObjectClass>::iterator, std::vector<ObjectClass>::iterator> result = std::make_pair(TempHandleBuffer.begin(), HandleList.begin());
		while(result.first != TempHandleBuffer.end() && result.second != HandleList.end())
		{
				result = std::mismatch(result.first, TempHandleBuffer.end(), result.second);
				FormatLogMessage("HandleList Load Mismatch Index: %d", result.second - HandleList.begin()); //result.first - TempHandleBuffer.begin();
		}
	}

	std::vector<AlignTerrainReplaceClass> TempAlignBuffer = AlignTerrainReplaceList;
	// Our current values differ from the recieved data, report what's different.
	if ((!TempAlignBuffer.empty() && !AlignTerrainReplaceList.empty()) && 
		(TempAlignBuffer.size() != AlignTerrainReplaceList.size() || memcmp(&TempAlignBuffer.front(), &AlignTerrainReplaceList.front(), TempAlignBuffer.size() * sizeof(TempAlignBuffer[0]) != 0)))
	{
		std::pair<std::vector<AlignTerrainReplaceClass>::iterator, std::vector<AlignTerrainReplaceClass>::iterator> result = std::make_pair(TempAlignBuffer.begin(), AlignTerrainReplaceList.begin());
		while(result.first != TempAlignBuffer.end() && result.second != AlignTerrainReplaceList.end())
		{
				result = std::mismatch(result.first, TempAlignBuffer.end(), result.second);
				FormatLogMessage("AlignTerrainReplace Load Mismatch Index: %d", result.second - AlignTerrainReplaceList.begin()); //result.first - TempAlignBuffer.begin();
		}
	}

	std::vector<DeleteAfterDelayClass> TempDeleteBuffer = DeleteAfterDelayList;
	// Our current values differ from the recieved data, report what's different.
	if ((!TempDeleteBuffer.empty() && !DeleteAfterDelayList.empty()) && 
		(TempDeleteBuffer.size() != DeleteAfterDelayList.size() || memcmp(&TempDeleteBuffer.front(), &DeleteAfterDelayList.front(), TempDeleteBuffer.size() * sizeof(TempDeleteBuffer[0]) != 0)))
	{
		std::pair<std::vector<DeleteAfterDelayClass>::iterator, std::vector<DeleteAfterDelayClass>::iterator> result = std::make_pair(TempDeleteBuffer.begin(), DeleteAfterDelayList.begin());
		while(result.first != TempDeleteBuffer.end() && result.second != DeleteAfterDelayList.end())
		{
				result = std::mismatch(result.first, TempDeleteBuffer.end(), result.second);
				FormatLogMessage("DeleteAfterDelay Load Mismatch Index: %d", result.second - DeleteAfterDelayList.begin()); //result.first - TempDeleteBuffer.begin();
		}
	}

	for(int i = 0; i < MAX_TEAMS; i++)
	{

	}
	*/

	ret = ret && m_PUPMgr->PostLoad(missionSave);
	//ret = ret && m_TapAssignmentHelper->PostLoad(missionSave);
	ret = ret && m_ProductionClassHelper->PostLoad(missionSave);
	//ret = ret && m_RecycleHelper->PostLoad(missionSave);
	ret = ret && m_PilotResourceHelper->PostLoad(missionSave);
	ret = ret && m_PowerHelper->PostLoad(missionSave);
	ret = ret && m_BuildingHelper->PostLoad(missionSave);
	ret = ret && m_APCHelper->PostLoad(missionSave);
	ret = ret && m_HQCPHelper->PostLoad(missionSave);
	ret = ret && m_LPadHelper->PostLoad(missionSave);
	ret = ret && m_PortalHelper->PostLoad(missionSave);
	ret = ret && m_AttacherHelper->PostLoad(missionSave);
	ret = ret && m_ReplaceOnDeathHelper->PostLoad(missionSave);
	ret = ret && m_UtilityShipHelper->PostLoad(missionSave);
	ret = ret && m_DeployHelper->PostLoad(missionSave);
	ret = ret && m_FuryHelper->PostLoad(missionSave);
	//ret = ret && m_LavaDamageHelper->PostLoad(missionSave);
	ret = ret && m_ScrapHelper->PostLoad(missionSave);
	ret = ret && m_DispatchHelper->PostLoad(missionSave);
	ret = ret && m_SatelliteHelper->PostLoad(missionSave);
	//ret = ret && m_LimiterHelper->PostLoad(missionSave);
	ret = ret && m_SwitchClassHelper->PostLoad(missionSave);
	ret = ret && m_AnchorClassHelper->PostLoad(missionSave);
	ret = ret && m_AmmoDrainHelper->PostLoad(missionSave);
	ret = ret && m_UndeployHelper->PostLoad(missionSave);
	//ret = ret && m_QuakeHelper->PostLoad(missionSave);
	ret = ret && m_GeyserHelper->PostLoad(missionSave);
	//ret = ret && m_AIWeaponHelper->PostLoad(missionSave);
	ret = ret && m_AttacherWeaponHelper->PostLoad(missionSave);
	ret = ret && m_MorphObjectHelper->PostLoad(missionSave);
	ret = ret && m_ShieldHelper->PostLoad(missionSave);
	ret = ret && m_EnergyArmorHelper->PostLoad(missionSave);
	ret = ret && m_JammerHelper->PostLoad(missionSave);
	ret = ret && m_CarrierHelper->PostLoad(missionSave);
	ret = ret && m_TransportHelper->PostLoad(missionSave);
	ret = ret && m_DropShipHelper->PostLoad(missionSave);
	ret = ret && m_RecallMineHelper->PostLoad(missionSave);

	return ret;
}

// Save data.
bool BZ1Helper::Save(bool missionSave)
{
	if (missionSave)
		return true;

	Write(static_cast<void *>(&mapInfo), sizeof(MapInfo));

	int size = int(HandleList.size());
	Write(&size, 1);
	if(size)
		Write(&HandleList.front(), sizeof(ObjectClass)*size);

	/*
	size = int(BuildingList.size());
	Write(&size, 1);
	if(size)
		Write(&BuildingList.front(), sizeof(BuildingClass)*size);
	*/

	size = int(AlignTerrainReplaceList.size());
	Write(&size, 1);
	if(size)
		Write(&AlignTerrainReplaceList.front(), sizeof(AlignTerrainReplaceClass)*size);

	size = int(DeleteAfterDelayList.size());
	Write(&size, 1);
	if(size)
		Write(&DeleteAfterDelayList.front(), sizeof(DeleteAfterDelayClass)*size);

	for(int i = 0; i < MAX_TEAMS; i++)
		Write(&teamInfo[i], sizeof(TeamInfo));

	for(int i = 0; i < MAX_GRAVITY_TYPES; i++)
		Write(&gravityInfo[i], sizeof(GravityInfo));

	/*
	size = int(MissionAudioList.size());
	Write(&size, 1);
	if(size)
		Write(&MissionAudioList.front(), sizeof(MissionAudioClass)*size);
	*/

	bool ret = true;

	// bools
	if (b_array != NULL)
		ret = ret && Write(b_array, b_count);

	// floats
	if (f_array != NULL)
		ret = ret && Write(f_array, f_count);

	// Handles
//	if (h_array != NULL)
//		ret = ret && Write(h_array, h_count);

	// ints
	if (i_array != NULL)
		ret = ret && Write(i_array, i_count);

	ret = ret && m_PUPMgr->Save(missionSave);
	//ret = ret && m_TapAssignmentHelper->Save(missionSave);
	ret = ret && m_ProductionClassHelper->Save(missionSave);
	//ret = ret && m_RecycleHelper->Save(missionSave);
	ret = ret && m_PilotResourceHelper->Save(missionSave);
	ret = ret && m_PowerHelper->Save(missionSave);
	ret = ret && m_BuildingHelper->Save(missionSave);
	ret = ret && m_APCHelper->Save(missionSave);
	ret = ret && m_HQCPHelper->Save(missionSave);
	ret = ret && m_LPadHelper->Save(missionSave);
	ret = ret && m_PortalHelper->Save(missionSave);
	ret = ret && m_AttacherHelper->Save(missionSave);
	ret = ret && m_ReplaceOnDeathHelper->Save(missionSave);
	ret = ret && m_UtilityShipHelper->Save(missionSave);
	ret = ret && m_DeployHelper->Save(missionSave);
	ret = ret && m_FuryHelper->Save(missionSave);
	//ret = ret && m_LavaDamageHelper->Save(missionSave);
	ret = ret && m_ScrapHelper->Save(missionSave);
	ret = ret && m_DispatchHelper->Save(missionSave);
	ret = ret && m_SatelliteHelper->Save(missionSave);
	//ret = ret && m_LimiterHelper->Save(missionSave);
	ret = ret && m_SwitchClassHelper->Save(missionSave);
	ret = ret && m_AnchorClassHelper->Save(missionSave);
	ret = ret && m_AmmoDrainHelper->Save(missionSave);
	ret = ret && m_UndeployHelper->Save(missionSave);
	ret = ret && m_QuakeHelper->Save(missionSave);
	ret = ret && m_GeyserHelper->Save(missionSave);
	//ret = ret && m_AIWeaponHelper->Save(missionSave);
	ret = ret && m_AttacherWeaponHelper->Save(missionSave);
	ret = ret && m_MorphObjectHelper->Save(missionSave);
	ret = ret && m_ShieldHelper->Save(missionSave);
	ret = ret && m_EnergyArmorHelper->Save(missionSave);
	ret = ret && m_JammerHelper->Save(missionSave);
	ret = ret && m_CarrierHelper->Save(missionSave);
	ret = ret && m_TransportHelper->Save(missionSave);
	ret = ret && m_DropShipHelper->Save(missionSave);
	ret = ret && m_RecallMineHelper->Save(missionSave);

	ret = ret && Write(&(m_TurnCounter),1);
	ret = ret && Write(&(TotalMapObjects),1);
	ret = ret && Write(&(Where),sizeof(Where));
	ret = ret && Write(&(WorldSetting),sizeof(WorldSetting));
	ret = ret && Write(&(m_GameType),1);
	ret = ret && Write(&(m_GameVersion),1);
	ret = ret && Write(&(m_ModVersion),1);

	return ret;
}



// When things are created. Most variables are retrieved and stored here.
void BZ1Helper::AddObject(Handle h)
{
	PetWatchdogThread();

	bool InEditor = false;
	
#ifdef EDITOR
	InEditor = GetVarItemInt("shell.dll.editor.active");
#endif

	TotalMapObjects++;

	// This Vector is now stored globally. -GBD
	//'scraphold' location is off the map, off the screen
	Where.x = -4000;
	Where.z = -4000;
	Where.y = TerrainFindFloor(Where.x, Where.z);

	// BZC Mod specific additions by Avatar, General BlackDragon, and Nielk1.
	char ODFName[MAX_ODF_LENGTH] = {0};
	// Open the ODF once here, and pass these Filenames on to sub-AddObject functions for reading.
	char TempODF1[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	GetAndOpenODFNames(h, TempODF1, TempODF2, ODFName); //GetODFName(h, ODFName);
	unsigned int ODFNameHash = Hash(ODFName);
	char ObjClass[MAX_ODF_LENGTH] = {0};
	GetObjInfo(h, Get_GOClass, ObjClass);
	unsigned int ObjClassHash = Hash(ObjClass);
	int Team = GetTeamNum(h);
	Matrix ObjectPos = GetMatrixPosition(h);; // Store it's Position.

	/*
	char EntityType[MAX_ODF_LENGTH] = {0};
	GetObjInfo(h, Get_EntityType, EntityType);
	unsigned int EntityTypeHash = Hash(EntityType);
	FormatConsoleMessage("EntityType is: %s, Hash is: %08x", EntityType, EntityTypeHash);
//	*/

	// New BZCClasslabel ODF Variable.
	char BZCClass[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
	GetODFString(TempODF1, TempODF2, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);
	unsigned int BZCClassHash = Hash(BZCClass);
	
	//bool m_AwareV13 = IFace_GetInteger("options.instant.awarev13");
	// Only in MPI. Done before Init, on first initial pass of all bzn saved game objects. Have to use direct ivar because Init hasn't run yet.
	if((!m_DidInit) && (Team > 0) && (!IsPlayer(h)) && // Before init, and Not the player.
		((IsNetworkOn()) && (GetVarItemInt("network.session.ivar53") == 2)) || // MPI.
		(IFace_GetInteger("options.instant.awarev13"))) // Instant Action.
	{
		int m_SwitchTeamA[MAX_TEAMS] = {0}; // Original Team.
		int m_SwitchTeamB[MAX_TEAMS] = {0}; // Team to switch to. 
		char mapTrnFile[2*MAX_ODF_LENGTH];
		strcpy_s(mapTrnFile, GetMapTRNFilename());
		if(OpenODF2(mapTrnFile))
		{
			for(int i = 0; i<MAX_TEAMS; i++)
			{
				char tempStr[256] = {0};
				char keyName[256] = {0};
				sprintf_s(keyName, "MPISwitchTeams%d", i+1);
				if((GetODFString(mapTrnFile, "DLL", keyName, sizeof(tempStr), tempStr)) && (tempStr[0]))
				{
					m_SwitchTeamA[i] = atoi(tempStr);
					char* pUnderscore = strchr(tempStr, '_');
					if(pUnderscore != NULL)
						m_SwitchTeamB[i] = atoi(pUnderscore + 1);
				}
				else // GetODFString couldn't find the entry. Exit the loop
				{
					break;
				}
			}
		}
		for(int i = 0; i < MAX_TEAMS; i++)
		{
			if((m_SwitchTeamA[i] == Team) && (m_SwitchTeamA[i] != m_SwitchTeamB[i])) // This team is flagged for switching, do it.
			{
				SetTeamNum(h, m_SwitchTeamB[i]); // Set it to the new team.
				Team = m_SwitchTeamB[i]; // Set team for anything that follows. 
			}
		}
	}

	// And this.
	if(GetMaxHealth(h) > 0)
		++teamInfo[Team].NumTeamObjects;

	// Custom Name setting. Override unitName, if this is set.
	char UnitName2[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
	GetODFString(TempODF1, TempODF2, "GameObjectClass", "unitName2", MAX_ODF_LENGTH, UnitName2);
	if(UnitName2[0])
		SetObjectiveName(h, UnitName2);

	// Add things to the master handle list.
	bool AddToList = true;
	int ObjectListIndex = -1;
	for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
	{
		if(iter->ObjectHandle == h)
		{
			// This object likely Deployed or was Upgraded, so it's Handle ID is the same, check to see if it's actually different.
			if(iter->ODFNameHash == ODFNameHash)
			{
		//		if(Logging) // !-- Radar / Satellite Lock called by AddObject functions in ProductionHelper and SatelliteHelper. Move these.
					FormatConsoleMessage("BZ1Helper AddObject Skipping Duplicate Object: %08x, ODF: %s, Team is: %d, Index is: %d", iter->ObjectHandle, ODFName, Team, iter - HandleList.begin());

				return; // This object already exists, completely skip it.
			}
			else
			{
				ObjectListIndex = iter - HandleList.begin();
			}
			break;
		}
	}
	if(AddToList)
	{
		// Look for new ODF Parameter HeightOffset. Done here so it only runs once, on this object's first time creation. Resyncs/saved games shouldn't re-move it.
		float HeightOffset = 0.0f;
		if((GetODFFloat(TempODF1, TempODF2, "GameObjectClass", "HeightOffset", &HeightOffset)) && (HeightOffset)) // If the value is set to move this object up/down, do it. 
		{
			SetVectorPosition(h, Add_Vectors(GetPosition(h), Vector(0, HeightOffset, 0)));
			ObjectPos = GetMatrixPosition(h);; // Update it's Position.
		}

		if(ObjectListIndex < 0) // New Object, Increase list size to compensate.
		{
			HandleList.resize(HandleList.size()+1);
			ObjectListIndex = HandleList.size()-1; // Use the new index.
		}
		ObjectClass &sao = HandleList[ObjectListIndex]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(ObjectClass)); // Zero things out at start.

		sao.ODFNameHash = ODFNameHash;
		sao.ObjClassHash = ObjClassHash;

		// Store it's Category ID for quicker identification later.
		if(IsVehicle(h))
			sao.CategoryID = Vehicle;
		else if(IsBuilding(h))
			sao.CategoryID = Building;
		else if(IsPerson(h))
			sao.CategoryID = Person;
		else if(IsPowerup(h))
			sao.CategoryID = Powerup;
		else if(IsWeapon(h))
			sao.CategoryID = Weapon;
		else if(IsScrap(h))
			sao.CategoryID = Scrap;
		else if(IsMisc(h))
			sao.CategoryID = Other;

		// Special "default" omission of Misc from HQCPClass Objectification, ODF can override. 
		bool ObjectifyDefault = (sao.CategoryID != Other);

		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanHQCPObjectify", &sao.CanObjectify, ObjectifyDefault);
		if(sao.CanObjectify)
			GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanHQCPObjectifyDeployed", &sao.CanObjectifyDeployed, true);

		bool DefaultTeleport = false;
		if(((sao.CategoryID == Vehicle) || (sao.CategoryID == Powerup) || (sao.CategoryID == Person) || //(IsWeapon(h)) || 
		(ObjClassHash == CLASS_TORPEDO) || (ObjClassHash == CLASS_SEEKER) || (ObjClassHash == CLASS_ARTIFACT) || 
		(ObjClassHash == CLASS_BOID) || (ObjClassHash == CLASS_FLAG)) && // These things can move, in theory. (Yes, one day I want to watch a boid fly through a Portal)
		(ObjClassHash != CLASS_POWERUP_CAMERA) && (ObjClassHash != CLASS_NAVBEACON) && (BZCClassHash != BZCCLASS_PORTAL) && 
		(BZCClassHash != BZCCLASS_PORTAL_SWITCH))
			DefaultTeleport = true;

		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "CanTeleport", &sao.CanTeleport, DefaultTeleport);

		bool DefaultShowSkill = (((sao.CategoryID == Vehicle) || (sao.CategoryID == Person)) && (GetMaxAmmo(h) > 0));
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ShowSkill", &sao.ShowSkill, DefaultShowSkill);
		GetODFInt(TempODF1, TempODF2, "GameObjectClass", "ShowSkillTeamFilter", &sao.ShowSkillTeamFilter, SameTeam+Allies);
		if(sao.ShowSkillTeamFilter == 0)
			sao.ShowSkillTeamFilter = -1; // 0 is All.

		// Fill in Supply Settings for DispatchHelper to use. (or anything else that cares if a Service Bay can service it)
		if(sao.ObjClassHash == CLASS_SUPPLYDEPOT)
		{
			GetODFFloat(TempODF1, TempODF2, "SupplyDepotClass", "healthRate", &sao.HealthRate);
			GetODFFloat(TempODF1, TempODF2, "SupplyDepotClass", "ammoRate", &sao.AmmoRate);
			GetODFFloat(TempODF1, TempODF2, "SupplyDepotClass", "supplyRate", &sao.ServiceRate);
			GetODFBool(TempODF1, TempODF2, "SupplyDepotClass", "serviceCraft", &sao.ServiceCraft, true);
		}

		sao.EngageRange = GetEngageRange(h);
		sao.ObjectHandle = h;
		sao.OriginalTeam = Team;
		sao.CurTeamNum = Team;
		sao.Position = GetPosition(h);
	}

	// New feature: AlignToTerrain. This aligns an object to terrain. -GBD
	if(m_TurnCounter > 0) // Game has officially started. Skip pre-placed objects on the map.
	{
		bool AlignToTerrain = false;
		if(GetODFBool(TempODF1, TempODF2, "GameObjectClass", "AlignToTerrain", &AlignToTerrain, false))
		{
			if(AlignToTerrain)
			{
				int ArrayEntry = -1;
				Vector Pos = GetPosition(h);
				Vector Front = GetFront(h);
				Vector ANormal(0, 0, 0);
				float Height = 0.0f;

				for (std::vector<AlignTerrainReplaceClass>::iterator iter = AlignTerrainReplaceList.begin(); iter != AlignTerrainReplaceList.end(); ++iter)
				{
					if(iter->AlignHandle == h)
					{
						ArrayEntry = iter-AlignTerrainReplaceList.begin();
						break;
					}
				}
				if(ArrayEntry >= 0)
				{
					ANormal = AlignTerrainReplaceList[ArrayEntry].NormalVec;
					//FormatLogMessage("Saved Object, Normal is: %f, %f, %f", ANormal.x, ANormal.y, ANormal.z);
					AlignTerrainReplaceList.erase(AlignTerrainReplaceList.begin() + ArrayEntry);
				}
				else
				{
					TerrainGetHeightAndNormal(Pos, Height, ANormal, false);
				}

			//	if((_stricmp(BZCClass,"recy") == 0) || (_stricmp(BZCClass,"muf") == 0) || (_stricmp(BZCClass,"slf") == 0))
			//		FormatLogMessage("Aligning to Terrain, Normal is: %f, %f, %f", ANormal.x, ANormal.y, ANormal.z);

				Matrix NewPos = Build_Orthogonal_Matrix(ANormal, Front, Pos);
				SetPosition(h, NewPos);
				SetVectorPosition(h, Pos);
			}
		}
	}

	// Look for new ODF Parameter PerceivedTeam.
	int TempPerceivedTeam = -1;
	if(GetODFInt(TempODF1, TempODF2, "GameObjectClass", "PerceivedTeam", &TempPerceivedTeam, -1))
		// Set its default perceived team.
		if((TempPerceivedTeam >= 0) && (GetPerceivedTeam(h) != TempPerceivedTeam))
			SetPerceivedTeam(h, TempPerceivedTeam);

	// Automatically set a group.
	bool AutoGroupUnits = false;
	if(GetODFBool(TempODF1, TempODF2, "GameObjectClass", "AutoSetGroup", &AutoGroupUnits, false))
		// Add it to a group. -GBD
		if(AutoGroupUnits)
			SetBestGroup(h);

	// DeleteObject code added for stuff. -GBD
	// Delete this item immidately.
	bool DeleteOnLoad = false;
	if(GetODFBool(TempODF1, TempODF2, "GameObjectClass", "DeleteOnLoad", &DeleteOnLoad, false))
		// Add it to the list.
		if(DeleteOnLoad)
			DeleteAfterDelay(h);

	// Setup new AI Weapon Randomization configs. Only do this if an AIP is running on this team with a specified difficulty, and they have an Armory.
	if((teamInfo[Team].m_CPUAIDifficulty) && (GetNumWeapons(h)) && (IsBuilding(teamInfo[Team].Slf)))
		m_AIWeaponHelper->AIGiveWeapons(h, Team);

	// Add things to the Building handle list.
	if(IsBuilding(h))
	{
		// Powered buildings, used for Powered Looping Animations. Used by CommTowers, HQCP, Portals, Jammers, and Gun Towers.
		m_BuildingHelper->AddBuildingObject(h, TempODF1, TempODF2, Team, ODFNameHash, ObjClassHash, BZCClassHash);

		/* // Left this off because the tap may not exist yet if its using AutoBuildTap1 ... AutoBuildTap8 = false, so just look inside.
		Handle TapH = GetTap(h, 0);
		char TapODF[MAX_ODF_LENGTH] = {0};
		GetObjInfo(TapH, Get_ODF, TapODF);
		if(TapODF[0]) // Add potential Tap Host object.
			FormatConsoleMessage("I HAZ A TAP, AND MY NAME IS: %s!", TapODF); //m_TapAssignmentHelper->AddTapHostObject(h);
		*/

		// Add potential Tap Host object.
		//m_TapAssignmentHelper->AddTapHostObject(h);
	}

	// Pilot code
	m_PilotResourceHelper->AddPilotObject(h, TempODF1, TempODF2, Team, BZCClassHash, ODFName);

	// Attacher Code
	m_AttacherHelper->AddAttacherObject(h, TempODF1, TempODF2, BZCClassHash);

	// Satellite Switches.
	m_SatelliteHelper->AddSatelliteSwitchObject(h, Team, ODFName);

	// Limiter Objects.
	m_LimiterHelper->AddLimiterObject(h, Team, ODFName);

	// Is it a Replace On Death object?
	char TestODF[MAX_ODF_LENGTH] = {0};
	if((GetODFString(TempODF1, TempODF2, "GameObjectClass", "ReplaceOnDeath", MAX_ODF_LENGTH, TestODF)) && (TestODF[0]))
		m_ReplaceOnDeathHelper->AddReplaceOnDeathObject(h, TestODF);

	// Claoker?
	bool Cloaker = false;
	GetODFBool(TempODF1, TempODF2, "MorphTankClass", "HiddenWhenMorphed", &Cloaker, false);
	bool AICloaker = true;
	GetODFBool(TempODF1, TempODF2, "CraftClass", "CanAICloak", &AICloaker, true);
	if(!AICloaker)
		Cloaker = AICloaker;

	// Patrol/Minelayers/Hunters/Cloakers.
	if((Cloaker) || (BZCClassHash == BZCCLASS_MINELAYER) || (BZCClassHash == BZCCLASS_PATROL_SHIP) || (BZCClassHash == BZCCLASS_HUNT_SHIP))
		AddToDispatch(h, 5.0f);

	// Add possible classes to Escort stuff.
	int TempEscortPriority = 0;
	GetODFInt(TempODF1, TempODF2, "CraftClass", "AIEscortPriority", &TempEscortPriority, 0);
	TempEscortPriority = clamp(TempEscortPriority, 0, 3);

	if((ObjClassHash == CLASS_SCAVENGER) || (ObjClassHash == CLASS_SCAVENGERH) || // Scavenger.
		(ObjClassHash == CLASS_MINELAYER) || (ObjClassHash == CLASS_TUG) || (ObjClassHash == CLASS_CONSTRUCTIONRIG) || // Utility Ship.
		(BZCClassHash == BZCCLASS_APC) || (ObjClassHash == CLASS_APC) || (TempEscortPriority)) // APC, or ODF override.
		m_UtilityShipHelper->AddUtilityShipObject(h, ObjClassHash, BZCClassHash, TempEscortPriority);

	// Loop over BZCClass, doing things.
	switch (BZCClassHash)
	{
	case BZCCLASS_RECYCLER:
	case BZCCLASS_FACTORY:
	case BZCCLASS_ARMORY:
	case BZCCLASS_CONSTRUCTOR:			// ProductionClassHelper.h
		m_ProductionClassHelper->AddProductionClassObject(h, TempODF1, TempODF2, Team, ObjClassHash, BZCClassHash);
		break;
	case BZCCLASS_APC:					// APCHelper.h
		m_APCHelper->AddAPCObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_ATTACHERWEAPON:		// AttacherWeaponHelper.h
		m_AttacherWeaponHelper->AddAttacherWeaponToggleObject(h, TempODF1, TempODF2, Team, ODFName);
		break;
	case BZCCLASS_CARRIER:				// CarrierHelper.h
		m_CarrierHelper->AddCarrierObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_CARRIER_DRONE:		// CarrierHelper.h
		m_CarrierHelper->AddCarrierDroneObject(h, Team);
		break;
	case BZCCLASS_DROPSHIP:				// DropShipHelper.h
		m_DropShipHelper->AddDropShipObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_FURY:					// FuryHelper.h
		m_FuryHelper->AddFuryObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_GEYSER:				// GeyserHelper.h
		if((m_TurnCounter == 0) || (InEditor))
			m_GeyserHelper->AddGeyserObject(h); // Add this to doubleup the geysers, this makes the "pretend" geyser underneath deployed production units. -GBD Only runs once on map loaded objects, or objects placed via editor.
		break;
	case BZCCLASS_HQCP:					// HQCPHelper.h
		m_HQCPHelper->AddHQCPObject(h, TempODF1, TempODF2); // CommandCenter(HQCP) Class
		break;
	case BZCCLASS_JAMM:					// JammerHelper.h
		m_JammerHelper->AddJammerObject(h); // Powered Jammer objects.
		break;
	case BZCCLASS_LAUNCH_PAD:			// LPadHelper.h
		m_LPadHelper->AddLPadObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_COMMTOWER:			// SatelliteHelper.h
		m_SatelliteHelper->AddSatelliteObject(h, TempODF1, TempODF2, Team); // If it's a commtower, based on the BZCClass, grab it. Can't select commbunker/tower in MP :(
		break;
	case BZCCLASS_MORPH_OBJECT:			// MorphObjectHelper.h
		m_MorphObjectHelper->AddMorphObjectHostObject(h, TempODF1, TempODF2); // "Morphing" Objects (Cloaking Walkers)
		break;
	case BZCCLASS_MORPH_OBJECT_TOGGLE:	// MorphObjectHelper.h
		m_MorphObjectHelper->AddMorphObjectToggleObject(h);
		break;
	case BZCCLASS_PORTAL:				// PortalHelper.h
		m_PortalHelper->AddPortalObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_PORTAL_SWITCH:		// PortalHelper.h
		m_PortalHelper->AddPortalSwitchObject(h, TempODF1, TempODF2); // Portal Channel Switches.
		break;
	case BZCCLASS_POWERPLANT:			// PowerHelper.h
		m_PowerHelper->AddPowerObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_RECALLMINE:			// RecallMineHelper.h
		m_RecallMineHelper->AddRecallMineObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_QUAKEBOMB:			// QuakeHelper.h
		m_QuakeHelper->AddQuakeObject(h, TempODF1, TempODF2);
		break;
	case BZCCLASS_SHIELD_TOWER:			// ShieldHelper.h
		m_ShieldHelper->AddShieldTowerObject(h, TempODF1, TempODF2);
		break;
	} // End BZCClass case.

	// Add Soldiers, remove them if the APC isn't targeting/within range. (BZ1.5 player driven APC behavior)
	char TempAIProcess[MAX_ODF_LENGTH] = {0};
	GetODFString(TempODF1, TempODF2, "GameObjectClass", "aiName", MAX_ODF_LENGTH, TempAIProcess);
	if(((BZCClassHash == BZCCLASS_PERSON_SOLDIER) || (_stricmp(TempAIProcess, "SoldierProcess") == 0)) && (!IsPlayer(h)))
		m_APCHelper->AddAPCSoldierObject(h, TempODF1, TempODF2, Team);
	
	// Loop over ObjClass, doing stuffs.
	switch (ObjClassHash)
	{
	case CLASS_RECYCLERVEHICLE: // For BZ2 class recy.
		if(!teamInfo[Team].TeamRace)
			teamInfo[Team].TeamRace = GetRace(h); // Save the team's race.

		if(!m_BuildPools)
			GetODFBool(TempODF1, TempODF2, "GameObjectClass", "UseBZ2Pools", &m_BuildPools, false);
		break;
	case CLASS_BARRACKS: // Count this.
		++teamInfo[Team].NumBarracks;
		break;
	case CLASS_SCRAP: // Track scrap, and store it. Remove some if we have too much. 
		m_ScrapHelper->AddScrapObject(h, TempODF1, TempODF2);
		break;
	case CLASS_POWERUP_CAMERA:			// ProductionClassHelper.h
		if(m_GameVersion <= v13PB65) // PB7+ can use GetNearestPowerup to detect CameraPods. :)
			m_ProductionClassHelper->AddCameraObject(h);
		break;
	case CLASS_POWERUP_REPAIR:
	case CLASS_POWERUP_SERVICE:			// BuildingHelper.h
		m_BuildingHelper->AddRepairPowerupObject(h, TempODF1, TempODF2, Team);
		break;
	case CLASS_AIR:
	case CLASS_APC:
	case CLASS_BOMBER:					// DeployHelper.h
		m_DeployHelper->AddDeployObject(h); // Look for any Aircraft, APC, or Bomber that has a depoy animation, and save it. -GBD
		break;
	case CLASS_SCAVENGER:
	case CLASS_SCAVENGERH: // Preload scavengers deployclass.
		{
			char DeployODF[MAX_ODF_LENGTH] = {0};
			if((GetODFString(TempODF1, TempODF2, "ScavengerClass", "deployClass", MAX_ODF_LENGTH, DeployODF)) || 
				(GetODFString(TempODF1, TempODF2, "ScavengerHClass", "deployClass", MAX_ODF_LENGTH, DeployODF)))
			{
				if((DeployODF[0]) && (DoesODFExist(DeployODF)))
				{
					PetWatchdogThread();
					PreloadODF(DeployODF);
					PetWatchdogThread();
				}
			}
		}
		break;
	}

	//-- Massive tweaks to this to prevent AV. If the object does not have a "deploy" or "undeploy" anim, then the old code would AV. 
	// BZ2 can't start an anim that doesn't exist. -GBD
	//watch for Undeploy trigger, add to undeploying objects queue.
	if(_strnicmp(ODFName,"BZCundep",8) == 0)
		m_UndeployHelper->AddUndeployObject(h, Team, ODFName);	

	// Energy Absorbing Armor
	char TempArmorClass = 0;
	GetODFChar(TempODF1, TempODF2, "GameObjectClass", "armorClass", &TempArmorClass, 'N');
	if((int(TempArmorClass) == 69) || ((int(TempArmorClass) == 101))) // E or e.
		m_EnergyArmorHelper->AddEnergyArmorObject(h, TempODF1, TempODF2);

	/*
	// Keep track of dummy recycle points, we have to reset base slot 1.
	bool RecyclePoint = (((h == GetRecy(Team)) && (IsBuilding(h))) || (IsSilo(h)));
	GetODFBool(h, "GameObjectClass", "RecyclePoint", &RecyclePoint, RecyclePoint);
	if(RecyclePoint)
		m_RecycleHelper->AddRecyclePointObject(h, Team);
	*/

	// Lives only in Network mode.
	if(IsNetworkOn())
	{
		// If this object is on a team who's Player is out of Lives, switch it's team to 15.
		if(teamInfo[Team].TeamIsDead)
		{
			if(!IsTeamplayOn())
			{
				Stop(h, 1); //Remove command, now!
				SetTeamNum(h, 15); // Throw it onto team 15.
			}
			else
			{
				SetTeamNum(h, GetCommanderTeam(Team)); // Throw it Back to Commander.
				//SetBestGroup(h); // Test me?
			}
		}
	}

	//End of BZC Additions
}

// When things are removed. End things done here.
void BZ1Helper::DeleteObject(Handle h)
{
	TotalMapObjects--;

	--m_LastObjectListCount; // Knock this down just because.

	//BZC addition - always get ODF name
	char ODFName[MAX_ODF_LENGTH] = {0};
	char TempODF1[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	GetAndOpenODFNames(h, TempODF1, TempODF2, ODFName);
	//unsigned int ODFNameHash = Hash(ODFName);

    int Team = GetTeamNum(h);

	char ObjClass[MAX_ODF_LENGTH] = {0};
	GetObjInfo(h, Get_GOClass, ObjClass);
	unsigned int ObjClassHash = Hash(ObjClass);

	// Matrix is useless for dead things, GetPosition2 only takes Vectors.
	Vector ObjectPos(0, 0, 0); // Store it's Position.
	GetPosition2(h, ObjectPos);

	char BZCClass[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
	GetODFString(TempODF1, TempODF2, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);
	unsigned int BZCClassHash = Hash(BZCClass);

	// Subtract this.
	if(GetMaxHealth(h) > 0)
		--teamInfo[Team].NumTeamObjects;

	//Pilot Code
	m_PilotResourceHelper->DeletePilotObject(h, TempODF1, TempODF2, Team, BZCClassHash, ObjClassHash);
	
	// Replace on Death Code
	m_ReplaceOnDeathHelper->DeleteReplaceOnDeathObject(h);

	// Dispatch Code
	m_DispatchHelper->DeleteDispatchObject(h, Team);

	switch (BZCClassHash)
	{
	case BZCCLASS_DROPSHIP:				// DropShipHelper.h
		m_DropShipHelper->DeleteDropShipObject(h);
		break;
	case BZCCLASS_POWERPLANT:			// PowerHelper.h
		m_PowerHelper->DeletePowerObject(h);
		break;
	case BZCCLASS_DAYWRECKER:			// QuakeHelper.h 
		m_QuakeHelper->DeleteQuakeObject(h, TempODF1, TempODF2, Team); // These ARE the droids we're looking for...
		break;
	} // Switch BZCClass

	switch (ObjClassHash)
	{
	case CLASS_BARRACKS:
		--teamInfo[Team].NumBarracks;
		break;
	// If this might be deploying into another building, lets fetch the Terrain Normal here and save it for use in AddObject. 
	case CLASS_RECYCLERVEHICLE:
	case CLASS_SCAVENGER:
	case CLASS_SCAVENGERH:
	case CLASS_DEPLOYBUILDING:
		{
			if(IsDeployed(h))
			{
				char TempODF3[MAX_ODF_LENGTH] = {0};
				char TempODF4[MAX_ODF_LENGTH] = {0};

				if((ObjClassHash == CLASS_RECYCLERVEHICLE) || (ObjClassHash == CLASS_DEPLOYBUILDING))
				{
					GetODFString(TempODF1, TempODF2, "DeployBuildingClass", "deployName", MAX_ODF_LENGTH, TempODF3);
				}
				else if((GetODFString(TempODF1, TempODF2, "ScavengerClass", "deployClass", MAX_ODF_LENGTH, TempODF3)) || 
						(GetODFString(TempODF1, TempODF2, "ScavengerHClass", "deployClass", MAX_ODF_LENGTH, TempODF3)))
				{
					//Got it
				}

				if(TempODF3[0]) // This is not a blank string "". 
				{
					strcat_s(TempODF3, ".odf"); // Add .odf to the end.
					// Open the ODF once here.
					if(OpenODF2(TempODF3))
					{
						if(GetODFString(TempODF3, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF4))
						{
							strcat_s(TempODF4, ".odf"); // Add .odf to the end.
							OpenODF2(TempODF4);
						}

						bool AlignToTerrain = false;
						if((GetODFBool(TempODF3, "GameObjectClass", "AlignToTerrain", &AlignToTerrain, false)) || 
							(GetODFBool(TempODF4, "GameObjectClass", "AlignToTerrain", &AlignToTerrain, false)))
						{
							if(AlignToTerrain)
							{
								for (std::vector<AlignTerrainReplaceClass>::iterator iter = AlignTerrainReplaceList.begin(); iter != AlignTerrainReplaceList.end(); ++iter)
								{
									if(iter->AlignHandle == h)
										return; // Done early, since this is at bottom of function. Must stay at bottom since we exit here.
								}

								AlignTerrainReplaceList.resize(AlignTerrainReplaceList.size()+1);
								AlignTerrainReplaceClass &sao = AlignTerrainReplaceList[AlignTerrainReplaceList.size()-1]; // No temporary being created, *much* faster
								memset(&sao, 0, sizeof(AlignTerrainReplaceClass)); // Zero things out at start.

								Vector ANormal(0, 0, 0);
								float Height = 0.0;
								TerrainGetHeightAndNormal(ObjectPos, Height, ANormal, false);

								//FormatLogMessage("Deploying into Building, Normal is: %f, %f, %f", ANormal.x, ANormal.y, ANormal.z);

								sao.NormalVec = ANormal;
								sao.AlignHandle = h;
							}
						}
					}
				}
			}
		}
		break;
	} // Switch ObjClass.
}

// Internal one-time setup. First added for Cloud code. -GBD
void BZ1Helper::Init(void)
{
//	bool Logging = BZ1Helper::Logging;
	//DWORD startTime = timeGetTime();

	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if(OpenODF2(mapTrnFile))
	{
		GetODFFloat(mapTrnFile, "Size", "Height", &mapInfo.Height); // Grab the map's original mapheight, used to base height off of. 
		GetODFFloat(mapTrnFile, "Size", "MetersPerGrid", &mapInfo.MPG); // Grab the map's Meters Per Grid, used to base Count off of. 
		GetODFFloat(mapTrnFile, "Size", "Width", &mapInfo.MapWidth); // Grab the map's original width
		GetODFFloat(mapTrnFile, "Size", "Depth", &mapInfo.MapDepth); // Grab the map's original depth
		GetODFFloat(mapTrnFile, "Size", "MinX", &mapInfo.MinX);
		GetODFFloat(mapTrnFile, "Size", "MinZ", &mapInfo.MinZ);
		GetODFFloat(mapTrnFile, "World", "Gravity", &mapInfo.Gravity, 12.5f);

		GetODFFloat(mapTrnFile, "DLL", "ReinforcementMinRadius", &m_ReinforcementMinRadius, 150.0f);
		GetODFFloat(mapTrnFile, "DLL", "ReinforcementMaxRadius", &m_ReinforcementMaxRadius, 250.0f);
		GetODFFloat(mapTrnFile, "DLL", "ReinforcementCliffTolerance", &m_ReinforcementMaxCliff, 100.0f);

		GetODFFloat(mapTrnFile, "DLL", "MinScrapHeight", &mapInfo.MinScrapHeight, -1000000.0f);
		GetODFFloat(mapTrnFile, "DLL", "MaxScrapHeight", &mapInfo.MaxScrapHeight, 1000000.0f);

		// read in gravity info from map trn.
		for(int i = 0; i < MAX_GRAVITY_TYPES; i++)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "Gravity%d", i+1);
			GetODFFloat(mapTrnFile, "Gravity", DesiredValue, &gravityInfo[i].Gravity, 12.5f);

			sprintf_s(DesiredValue, "OnPeriod%d", i+1);
			GetODFFloat(mapTrnFile, "Gravity", DesiredValue, &gravityInfo[i].OnPeriod, 0.0f);

			sprintf_s(DesiredValue, "TranPeriod%d", i+1);
			GetODFFloat(mapTrnFile, "Gravity", DesiredValue, &gravityInfo[i].TranPeriod, 0.0f);
		}

		size_t bufSize = 0;
		GetPathPoints("edge_path", bufSize, NULL);
		float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
		VECTOR_2D EdgePoints[2];
		memset(EdgePoints, 0, sizeof(EdgePoints));
		if(GetPathPoints("edge_path", bufSize, pData))
		{
			if(bufSize == 2) // Must be 2, or ignore it and use map size.
			{
				for(size_t i = 0; i < bufSize; ++i)
				{
					EdgePoints[i].x = pData[2*i+0];
					EdgePoints[i].z = pData[2*i+1];
				}
			}
		}
		if((EdgePoints[0].x + EdgePoints[0].z + EdgePoints[1].x + EdgePoints[1].z) != 0.0f)
		{
			mapInfo.MaxWidth = max(EdgePoints[0].x, EdgePoints[1].x);
			mapInfo.MaxDepth = max(EdgePoints[0].z, EdgePoints[1].z);
			mapInfo.MinWidth = min(EdgePoints[0].x, EdgePoints[1].x);
			mapInfo.MinDepth = min(EdgePoints[0].z, EdgePoints[1].z);
		}
		else
		{
			mapInfo.MaxWidth = (mapInfo.MapWidth / 2);
			mapInfo.MaxDepth = (mapInfo.MapDepth / 2);
			mapInfo.MinWidth = -(mapInfo.MapWidth / 2);
			mapInfo.MinDepth = -(mapInfo.MapDepth / 2);
		}

		// Retrieve Sun information if it's available. Set Defaults if not.
		SunSpriteIndex = -1; // Assume none for now.
		GetODFFloat(mapTrnFile, "Sun", "Angle", &SunAngle, 12.0f);
		GetODFFloat(mapTrnFile, "Sun", "Period", &SunPeriod, 24.0f);
		for(int i = 0; i < MAX_SPRITES; i++)
		{
			char SpriteTextureName[MAX_ODF_LENGTH] = {0};
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "TextureName%d", i+1);
			if(GetODFString(mapTrnFile, "Sprites", DesiredValue, MAX_ODF_LENGTH, SpriteTextureName))
			{
				if(_strnicmp(SpriteTextureName,"BZCSun", 6) == 0)
				{
					SunSpriteIndex = i;
					break;
				}
			}
		}
	}

	// Tag log file with map and character name for reference.
	if (char *dot = strrchr(mapTrnFile, '.'))
		*dot = '\0';

	FormatLogMessage("Game Started on map: %s by %s. Version: %d\n", mapTrnFile, GetPlayerName(GetPlayerHandle()), m_GameVersion);

	// Set Scavenger Scrap Mult for MP.
	if(IsNetworkOn())
	{
		m_IsFriendlyFireOn = (GetVarItemInt("network.session.ivar32") != 0);
		m_MaxLives = GetVarItemInt("network.session.ivar60"); // Lives Limit. -GBD
		m_ScrapMultiplier = GetVarItemInt("network.session.ivar81");
		m_GameType = GetVarItemInt("network.session.ivar5"); // All=0, DM=1, Strat=2, all other values undefined (used in filters)
		m_DamageMultiplier = (GetVarItemInt("network.session.ivar26") / 10.0f); // Game only supports DamageMult in DM (ivar5 == 1) Other versions DLL implemented.
		m_CPUUseDayWreckers = GetVarItemInt("network.session.ivar89");
		m_DisableSiteCamMP = GetVarItemInt("network.session.ivar90");
		mapInfo.Gravity = (static_cast<float>(GetVarItemInt("network.session.ivar31")) * 0.5f);
		SetGravity(GetMapInfo().Gravity); // Set this for the server now. Clients get this set from Load().
	}
	else
	{
		m_CPUUseDayWreckers = IFace_GetInteger("options.instant.bool2");
		m_DamageMultiplier = 1.0f; // Set the Default!
	}

	m_PUPMgr->Init(*this);
	//m_DispatchHelper->Init(*this);
	m_LavaDamageHelper->Init(*this);

	// Set initial value ASAP! This is used as a constant location. Always the same. 
	Where.x = -4000.0f;
	Where.z = -4000.0f;
	Where.y = TerrainFindFloor(Where.x, Where.z);
	
	bool GenerateClouds = true;

#ifdef EDITOR

	bool m_AwareV13 = IFace_GetInteger("options.instant.awarev13");
	if((m_AwareV13) && (!IsNetworkOn())) // This is likely Instant Action map.
		GenerateClouds = IFace_GetInteger("options.instant.bool2"); // Is cloud switch on? If so, build them here.

	if(GetVarItemInt("network.session.ivar119")) // If we're generating a map but not running script, don't spawn clouds.
		GenerateClouds = false;

	// Load Sky Settings. Editor mode only!.
	LoadSkySettingsFromTRN();

	if(GetVarItemInt("network.session.ivar116") == 2) // Save objects to file.
		SaveMapObjectsToFile();
	else if(GetVarItemInt("network.session.ivar116") == 1) // Load objects from file.
		LoadMapObjectsFromFile();

#endif
	
	if(GenerateClouds) // Somewhere, we said generate clouds. 
		SetupClouds();
	else
		m_CloudsDone = true;

//	/*
	// Check for mod installation. These are several hook files that indicate this DLL is being run with BZC. If somebody tries to steal our DLL, disappoint them. -GBD !! Under no circumstances is this list to be shared in any way!
	if((!DoesFileExist("bzceditor.txt")) || (!DoesFileExist("bzshell_messgallary.cfg")) || (!DoesFileExist("bzshell_credits.cfg")) || 
		(!DoesFileExist("bzshell_editornotes.cfg")) || (!DoesFileExist("avrecy.odf")) || (!DoesFileExist("bzcwelcome.inf")) || 
		(!DoesFileExist("BZCundep1.odf")) || (!DoesFileExist("BZCundep2.odf")) || (!DoesFileExist("BZCundep3.odf")) || 
		(!DoesFileExist("BZCvars.txt")) || (!DoesFileExist("BZCgysr.odf")) || (!DoesFileExist("svrecy.odf")) || 
		(!DoesFileExist("BZCMoon.des")) || (!DoesFileExist("powerbolt.odf")) || (!DoesFileExist("win.wav")) || 
		(!DoesFileExist("svtank.odf")) || (!DoesFileExist("apBZCwrck.odf")) || (!DoesFileExist("mpobjective_koth.otf")) || 
		(!DoesFileExist("Gallary.txt")) || (!DoesFileExist("cnstshold.odf")) || (!DoesFileExist("BZCStrat.dll")) || 
		(!DoesFileExist("BZCacloud.msh")) || (!DoesFileExist("hvsav.odf")) || (!DoesFileExist("bzescape_640X480.cfg")))
	{
//	*/
	// BZC Demo version:
	/*
	// Check for mod installation. These are several hook files that indicate this DLL is being run with BZC. If somebody tries to steal our DLL, disappoint them. -GBD !! Under no circumstances is this list to be shared in any way!
	if((!DoesFileExist("bzceditor.txt")) || (!DoesFileExist("missions.odf")) || (!DoesFileExist("bzshell_multi_bzcffacap.cfg")) || 
		(!DoesFileExist("bzshell_editornotes.cfg")) || (!DoesFileExist("avrecy.odf")) || (!DoesFileExist("bzcwelcome.inf")) || 
		(!DoesFileExist("BZCundep1.odf")) || (!DoesFileExist("BZCundep2.odf")) || (!DoesFileExist("BZCundep3.odf")) || 
		(!DoesFileExist("BZCgrave00.wav")) || (!DoesFileExist("BZCgysr.odf")) || (!DoesFileExist("svfigh.odf")) || 
		(!DoesFileExist("BZCMoon.des")) || (!DoesFileExist("powerbolt.odf")) || (!DoesFileExist("win.wav")) || 
		(!DoesFileExist("svtank.odf")) || (!DoesFileExist("apBZCwrck.odf")) || (!DoesFileExist("mpobjective_koth.otf")) || 
		(!DoesFileExist("BZC_stratstarting.txt")) || (!DoesFileExist("cnstshold.odf")) || (!DoesFileExist("BZCStrat.dll")) || 
		(!DoesFileExist("BZCacloud.msh")) || (!DoesFileExist("hvsatodin.odf")) || (!DoesFileExist("bzescape_640X480.cfg")))
	{
	*/
		FormatLogMessage("Battlezone Classic not installed properly!");
		AddToMessagesBox2("-- Aborting!", RED);
		if(IsNetworkOn())
		{
			NoteGameoverWithCustomMessage("Battlezone Classic not installed properly!");
			DoGameover(10.0f);
		}
		else
		{
			FailMission(GetTime() + 10.0f, "reinstall.des");
		}
	}

	// Easter egg, of sorts...
	const char *PlayerName = GetCVarItemStr(GetLocalPlayerTeamNumber(), 0);
	unsigned int PlayerNameHash = Hash(PlayerName);
	switch (PlayerNameHash)
	{
	case 0x223449c4: /* "Avatar" */
		AddToMessagesBox2("-- Hello Av, How are you? -GBD", BLUE);
		break;
	case 0xb0d6ce78: /* "General_BlackDragon" */
		AddToMessagesBox2("-- FILL ME IN? Please. -GBD", BLUE);
		break;
	case 0xbd3082dd: /* "Nielk1" */
		AddToMessagesBox2("-- Hey N1, How's the job going? -GBD", BLUE);
		break;
	case 0x4d7d3b40: /* "DarkCobra262" */
		AddToMessagesBox2("-- Thanks for all your dedicated hours of testing. -GBD", BLUE);
		break;
	case 0x937ffc03: /* "Josiah" */
		AddToMessagesBox2("-- Josiah! Thx for the Ice. -GBD", BLUE);
		break;
	case 0x1b19a8a7: /* "Kinjetica" */
		AddToMessagesBox2("-- Where are my bump maps? :) -GBD", BLUE);
		break;
	case 0xa395eed4: /* "Zax" */
	case 0x25bf4c5c: /* "BB1" */
		AddToMessagesBox2("-- Hello BB1, Hows the cat doing? :) -GBD", BLUE);
		break;
	case 0xeb1b38cd: /* "Vearie" */
	case 0x91d71f53: /* "Stroket" */
	case 0x4e8bb8ea: /* "RushLimbah" */
	case 0x6eb81089: /* "Rush_Limbah" */ // Gave up trying to keep up with his names...
		AddToMessagesBox2("-- Hi Vearie, Thanks for the scripts! :) -GBD", BLUE);
		break;
	case 0x7715e009: /* "Ken" */
	case 0xc772fbd9: /* "Ultraken" */
		AddToMessagesBox2("-- You're not real, are you? -GBD", BLUE);
		break;
	case 0x4439adc5: /* "GSH" */
		AddToMessagesBox2("-- GSH? The Real one? -GBD", BLUE);
		break;
	case 0x81ae53ba: /* "Lil_Stealth" */
		AddToMessagesBox2("-- Hello Cutie. :) How are you? -GBD", BLUE);
		break;
	case 0x22d2c757: /* "Zach" */
		AddToMessagesBox2("-- Hello Sweetie. Thanks for all your hard work testing. -GBD", BLUE);
		break;
	case 0x1453e3cf: /* "Bzone_Lord" */
		AddToMessagesBox2("-- 'Ello Lord. How are you mate? -GBD", BLUE);
		break;
	case 0xc1d6f7ca: /* "Deus_Ex" */
		AddToMessagesBox2("-- Hey Dues, thanks for all the models. :) -GBD", BLUE);
		break;
	}

	//DWORD endTime = timeGetTime();
	//char buf[MAX_ODF_LENGTH];
	//sprintf_s(buf, "BZ1Helper::Init() %d ms\n", endTime - startTime);
	//OutputDebugString(buf);

	m_DidInit = true; // Flag this here. 
}

// One time Setup, first run, AND on each load. This properly restores things from saved games/late joiners. -GBD
void BZ1Helper::Setup(void)
{
	m_DidSetup = true;

	// Set the defaults. These MUST be filled from somewhere, or the Matrix will do horrible, horrible things to our objects, including, but not limited to, 
	// stretching, turning inside out, and killing everything on the map. -GBD 
	Where.y = TerrainFindFloor(Where.x, Where.z);

	// Loop over, reset object positions to ensure they are where they're supposed to be. 
	for(int Team = 0; Team<MAX_TEAMS; Team++)
	{
		if(IsAround(teamInfo[Team].RecScrap))
			SetVectorPosition(teamInfo[Team].RecScrap, Where);
		if(IsAround(teamInfo[Team].MufScrap))
			SetVectorPosition(teamInfo[Team].MufScrap, Where);
		if(IsAround(teamInfo[Team].SlfScrap))
			SetVectorPosition(teamInfo[Team].SlfScrap, Where);
		if(IsAround(teamInfo[Team].CnstScrap))
			SetVectorPosition(teamInfo[Team].CnstScrap, Where);

		if(IsAround(teamInfo[Team].LimiterObject[0]))
			SetVectorPosition(teamInfo[Team].LimiterObject[0], Where);
		if(IsAround(teamInfo[Team].LimiterObject[1]))
			SetVectorPosition(teamInfo[Team].LimiterObject[1], Where);
		if(IsAround(teamInfo[Team].LimiterObject[2]))
			SetVectorPosition(teamInfo[Team].LimiterObject[2], Where);
		if(IsAround(teamInfo[Team].ImaPilot))
			SetVectorPosition(teamInfo[Team].ImaPilot, Where);

		if(IsAround(teamInfo[Team].SatelliteOn))
			SetVectorPosition(teamInfo[Team].SatelliteOn, Where);
		if(IsAround(teamInfo[Team].SatelliteOff))
			SetVectorPosition(teamInfo[Team].SatelliteOff, Where);
		if(IsAround(teamInfo[Team].SatelliteLock))
			SetVectorPosition(teamInfo[Team].SatelliteLock, Where);
	}
}

// This runs every game tick, master controller for everything that runs. 
void BZ1Helper::Execute()
{
	// Retrieve maps pathpoints, store for general use across all BZC DLLs. -GBD
	// NOTE: This is done LOCALLY, not saved across MP, so it MUST be called here, at the start.
	if(!GotPaths)
	{
		GetAiPaths(AIpathCount, (char **&)AIpathNames);
		m_LavaDamageHelper->LoadLavaField(GetMapTRNFilename());
		GotPaths = true;

		for (int i = 0; i < AIpathCount; ++i)
		{
			const char *label = AIpathNames[i];

			int teamNum = 0, pathNum = 0;

			if (sscanf_s(label, "%dMLay%d", &teamNum, &pathNum) == 2)
				MLayPathList[teamNum].push_back(label);
			else if (sscanf_s(label, "%dPatrol%d", &teamNum, &pathNum) == 2)
				PatrolPathList[teamNum].push_back(label);
		}
	}
	// Do one time server init.
	if(!m_DidInit)
		Init();
	// Continue to call SetupClouds untill all clouds are placed.
	if(!m_CloudsDone)
		SetupClouds();
	// Do this too? Just in case Load didn't call it for some reason.
	if(!m_DidSetup)
		Setup();

	m_TurnCounter++; // Count Stuff, just because.

	if(m_TurnCounter < ConvertToTurns(60.0f)) // Wait 60 seconds before letting the Watchdog Thread close the game. -[GBD]
		PetWatchdogThread();

	// Cleanup Structs here.
	HandleList.erase(std::remove_if(HandleList.begin(), HandleList.end(), ShouldRemoveObject), HandleList.end());
//	BuildingList.erase(std::remove_if(BuildingList.begin(), BuildingList.end(), ShouldRemoveBuilding), BuildingList.end());
	AlignTerrainReplaceList.erase(std::remove_if(AlignTerrainReplaceList.begin(), AlignTerrainReplaceList.end(), ShouldRemoveAlignTerrainReplace), AlignTerrainReplaceList.end());
	DeleteAfterDelayList.erase(std::remove_if(DeleteAfterDelayList.begin(), DeleteAfterDelayList.end(), ShouldRemoveDeleteAfterDelay), DeleteAfterDelayList.end());
//	MissionAudioList.erase(std::remove_if(MissionAudioList.begin(), MissionAudioList.end(), ShouldRemoveMissionAudio), MissionAudioList.end());

	// Run DeleteObject loading here.
	for (std::vector<DeleteAfterDelayClass>::iterator iter = DeleteAfterDelayList.begin(); iter != DeleteAfterDelayList.end(); ++iter)
	{
		--iter->TurnsRemaining;

		if(iter->TurnsRemaining < 0)
			RemoveObject(iter->DeleteAfterDelayHandle);
	}

	// Master Handle List loop. Update things for general objects here. Hand out HandleList index's to sub-functions where possible.
	for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
	{
		int HandleIndex = iter - HandleList.begin(); // The index, for passing things into helper functions.
		iter->CurTeamNum = GetTeamNum(iter->ObjectHandle); // Update this, every tick.
		iter->Position = GetPosition(iter->ObjectHandle);
		int CurrentCommand = GetCurrentCommand(iter->ObjectHandle);

		// Experiment to re-iterate percievedteam on things for newly built units.
		SetPerceivedTeam(iter->ObjectHandle, GetPerceivedTeam(iter->ObjectHandle));

		//HQCPCode();
		if((iter->CanObjectify) && ((iter->CanObjectifyDeployed) || (!IsDeployed(iter->ObjectHandle)))) // Passes Deployed setting.
			m_HQCPHelper->ExecuteHandle(HandleIndex);

		//PortalCode();
		if(iter->CanTeleport)
			m_PortalHelper->ExecuteHandle(HandleIndex);

		// Create a couple of custom Command work arounds utilizing the Hunt command.
		if(CurrentCommand == CMD_HUNT)
		{
			switch (iter->ObjClassHash)
			{
			case CLASS_RECYCLERVEHICLE:
			case CLASS_TRACKEDDEPLOYABLE:
				{
					SetCommand(iter->ObjectHandle, CMD_DEPLOY, 0); // A "Deploy Now" command, to remotely tell a recycler to deploy without using the aim reticle.
				}
				break;
			case CLASS_CONSTRUCTIONRIG:
				{
					SetCommand(iter->ObjectHandle, CMD_STOP, 0); // Yay! A "Hold" command.
				}
				break;
			case CLASS_TUG:
				{
					Handle TempH = GetNearestObject(iter->ObjectHandle, 100.0f, -1, 0, 1, true, 3, "CLASS_ARTIFACT"); // We want Artifacts. Time to us a fancy feature I made :)
					if(TempH)
					{
						SetCommand(iter->ObjectHandle, CMD_PICKUP, 0, TempH); //Pickup(iter->ObjectHandle, TempH, 0);
					}
					else
					{
						SetCommand(iter->ObjectHandle, CMD_NO_DEPLOY, 0);
						PlayVOMsg(iter->ObjectHandle, "user2Msg", iter->CurTeamNum); // Play user2Msg, if possible.
					}
				}
				break;
			}
		}

		// Special code for Minelayer VO's. //!!-- Remove once PB7 fixes it.
		if(iter->ObjClassHash == CLASS_MINELAYER)
		{
			// Vo for when Laying Mines.
			if(iter->LastCommand == CMD_LAY_MINES)
			{
				if(CurrentCommand != CMD_LAY_MINES)
					PlayVOMsg(iter->ObjectHandle, "user2Msg", iter->CurTeamNum);
			}
			else
			{
				if(CurrentCommand == CMD_LAY_MINES)
					PlayVOMsg(iter->ObjectHandle, "user1Msg", iter->CurTeamNum);
			}
		}

		// Custom Skill Function. When a unit's skill changes, update it's name.
		if(m_GameVersion >= v13PB7)
		{
			//int CurSkill = GetSkill(iter->ObjectHandle); // Save the new skill level.

			if((iter->ShowSkill) && ((m_TurnCounter % m_GameTPS) == 0) && (TeamFilterCheck(iter->ShowSkillTeamFilter, GetLocalPlayerTeamNumber(), iter->CurTeamNum))) // Only show Skill if the TeamFilter is set to for this unit.
			{
				SetSkillName(iter->ObjectHandle);

				/*
				if(CurSkill > 0) // Do we even bother?
				{
					char UnitName[MAX_ODF_LENGTH] = {0};
					strcpy_s(UnitName, GetObjectiveName(iter->ObjectHandle));

					//!!-- Move to a Sub Function to GetName without skill level, for use in Portal/DropShip Helper.
					// Step through backwards, removing any previous skill level " ****" on the end of the name. Code from Ken.
					size_t len = strlen(UnitName)-1;
					for (size_t pos = len; pos >= 0; --pos)
					{
						if (UnitName[pos] != '*')
						{
							if (pos < len && UnitName[pos] == ' ')
							{
								UnitName[pos] = '\0';
								len = pos;
							}
							break;
						}
					}
					// If our length is long enough, add the " ****" to the end. If it's not, don't do anything.
					if((len + 1 + CurSkill + 1) < MAX_ODF_LENGTH)
					{
						UnitName[++len] = ' ';
						for (int i = 0; i < CurSkill; ++i)
							UnitName[++len] = '*';
						UnitName[++len] = '\0';
					}

					SetObjectiveName(iter->ObjectHandle, UnitName); // Update the name.
				}
			//	*/
			}
		}

		// Throw empty ships that got thumped off map, back onto map. Also half their velocities. Also, look for RecyclerClass vehicles that are told to Deploy Here.
		if( (iter->CategoryID == Vehicle) && (!IsAliveAndPilot2(iter->ObjectHandle)) && (
			(iter->Position.x > mapInfo.MaxWidth) || 
			(iter->Position.x < mapInfo.MinWidth) || 
			(iter->Position.z > mapInfo.MaxDepth) || 
			(iter->Position.z < mapInfo.MinDepth)))
			SetVelocity(iter->ObjectHandle, Vector_Scale(Neg_Vector(GetVelocity(iter->ObjectHandle)), 0.5f));

		iter->LastCommand = CurrentCommand;

		// Run this near bottom, since it might remove the index. Change to DeleteAfterDelay if we need more then one of this.
		m_LavaDamageHelper->Execute(HandleIndex);
	}

	// Execute a check on all players, and make sure their arrays are in order. This handles Giving/Taking Command while in Satellite mode. Code from N1. -GBD
	for(int SavedTeam = 0; SavedTeam < MAX_TEAMS; SavedTeam++)
	{
		// This maybe? Should force update the Race of Team in SP and MP. // Failed. :(
		//if(GetRaceOfTeam(SavedTeam) != teamInfo[SavedTeam].TeamRace)
		//	SetMPTeamRace(WhichTeamGroup(SavedTeam), teamInfo[SavedTeam].TeamRace);

		if(IsPlayer(teamInfo[SavedTeam].RealPlayer))
		{
			int RealTeam = GetTeamNum(teamInfo[SavedTeam].RealPlayer);
			if((RealTeam != SavedTeam) && (RealTeam > 0))
			{
				// Lives stuff. Lives count.
				SwitchVariables(teamInfo[SavedTeam].PlayerLives, teamInfo[RealTeam].PlayerLives);
				// Lives Stuff, Is team out of lives?
				SwitchVariables(teamInfo[SavedTeam].TeamIsDead, teamInfo[RealTeam].TeamIsDead);

				// Convert Real Player handle. 
				SwitchVariables(teamInfo[SavedTeam].RealPlayer, teamInfo[RealTeam].RealPlayer);

				// Convert satellite object.
				SwitchVariables(teamInfo[SavedTeam].SatellitePlayer, teamInfo[RealTeam].SatellitePlayer);
				if(!IsPlayer(teamInfo[RealTeam].SatellitePlayer))
					SetTeamNum(teamInfo[RealTeam].SatellitePlayer, RealTeam);

				// Convert Satellite Switch. 
				SwitchVariables(teamInfo[SavedTeam].SatelliteViewActive, teamInfo[RealTeam].SatelliteViewActive);
				if((IsAround(teamInfo[SavedTeam].SatelliteOn)) && (!IsAround(teamInfo[RealTeam].SatelliteOn)))
					BuildObject("BZCsatviewoff",SavedTeam,Where);

				// Convert Satellite Dummy Ship Handle.
				SwitchVariables(teamInfo[SavedTeam].Player, teamInfo[RealTeam].Player);
				if(!IsPlayer(teamInfo[RealTeam].Player))
					SetTeamNum(teamInfo[RealTeam].Player, RealTeam);

				// Convert Satellite Ammo Counter.
				SwitchVariables(teamInfo[SavedTeam].SatelliteOutOfAmmo, teamInfo[RealTeam].SatelliteOutOfAmmo);

				// Convert Satellite Saved Pilot.
				char TempPilotODF[MAX_ODF_LENGTH] = {0};
				strcpy_s(TempPilotODF, sizeof(TempPilotODF), teamInfo[SavedTeam].PlayerPilotODF);
				strcpy_s(teamInfo[SavedTeam].PlayerPilotODF, sizeof(teamInfo[SavedTeam].PlayerPilotODF), teamInfo[RealTeam].PlayerPilotODF);
				strcpy_s(teamInfo[RealTeam].PlayerPilotODF, sizeof(teamInfo[RealTeam].PlayerPilotODF), TempPilotODF);

				// Covnert the Limiter object.
				SwitchVariables(teamInfo[SavedTeam].LimitPlayer, teamInfo[RealTeam].LimitPlayer);
				
				// Reset things for the new player.

				// swap other values you need to like lives
				// might be wise to implement a swap function that takes
				// a reference to the array and the two indexes to swap
				// Type generics would be good too but that is probably beyond you. -N1
			}
		}
	}
	// Set the real player now.
	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		if(!IsPlayer(teamInfo[Team].RealPlayer))
			teamInfo[Team].RealPlayer = GetPlayerHandle(Team);

		if(m_GameVersion >= v13PB7)
		{
			const char *TempAIPFile = GetPlan(Team);
			if((TempAIPFile) && (_stricmp(TempAIPFile, teamInfo[Team].TeamAIPFile) != 0))
				UpdateTeamAIP(TempAIPFile, Team);
		}

		// Player Nav Dropper. Tied to civar7. 
		if(teamInfo[Team].RealPlayer) // There's a Player.
		{
			int NavSwitch = 0;

			if(IsNetworkOn())
				NavSwitch = GetCVarItemInt(Team, 15);
			else
				NavSwitch = IFace_GetInteger("shell.dll.dropnav");

			if(NavSwitch)
			{
				bool CanDropNavs = IsVehicle(teamInfo[Team].RealPlayer);
				GetODFBool(teamInfo[Team].RealPlayer, "CraftClass", "CanDropNavs", &CanDropNavs, CanDropNavs);

				if(CanDropNavs)
				{
					float NavDelay = 1.0f;
					GetODFFloat(teamInfo[Team].RealPlayer, "CraftClass", "NavDropDelay", &NavDelay, 1.0f);
					
					if(m_TurnCounter > teamInfo[Team].LastNavTime + ConvertToTurns(NavDelay))
					{
						// Build us a Nav Beacon.
						char NavODF[MAX_ODF_LENGTH] = {0};
						if(!GetODFString(teamInfo[Team].RealPlayer, "CraftClass", "NavConfig", MAX_ODF_LENGTH, NavODF))
						{
							sprintf_s(NavODF, "%cbnav", GetRace(teamInfo[Team].RealPlayer));
							if(!DoesODFExist(NavODF))
								strcpy_s(NavODF, "ibnav"); // Fallback to Default. This is BZ2's Default Nav Beacon.
						}

						char NavDropSound[MAX_ODF_LENGTH] = {0};
						GetODFString(teamInfo[Team].RealPlayer, "CraftClass", "NavDropSound", MAX_ODF_LENGTH, NavDropSound, "gprox00.wav");

						// Calculate the position of the rear of their ship.
						float CollisionRadius = 0.0f;
						GetODFFloat(teamInfo[Team].RealPlayer, "GameObjectClass", "CollisionRadius", &CollisionRadius, 0.0f);

						Matrix Position = GetMatrixPosition(teamInfo[Team].RealPlayer);
						Position.posit = Position.posit - Position.front * CollisionRadius;
						Handle Nav = BuildObject(NavODF, Team, Position); // Make it pop!
						StartAudio3D(NavDropSound, teamInfo[Team].RealPlayer, AUDIO_CAT_WEAPON);
						teamInfo[Team].LastNavTime = m_TurnCounter;

						// Does this user have a custom Nav Name?
						const char *NavName = GetCVarItemStr(Team, 1);
						if((NavName) && (NavName[0])) // If it's something.
							SetObjectiveName(Nav, NavName);
					}
				}
				
				if(Team == GetLocalPlayerTeamNumber()) // This is us, reset button.
				{
					if(IsNetworkOn())
						IFace_SetInteger("network.session.civar15", 0);
					else
						IFace_SetInteger("shell.dll.dropnav", 0);
				}
			}
		}

	}

	// Run these:
	BZ1Interface();
	BZ1Lives();
	EnvironmentalSystems();
	//DayNightCycle(); // Sun sprite modulation. Only works in EDITOR! :(

	m_PUPMgr->Execute(*this);
	//m_TapAssignmentHelper->Execute(*this);
	m_PowerHelper->Execute(*this);
	m_BuildingHelper->Execute(*this); //BuildingCode();
	m_ProductionClassHelper->Execute(*this); //SLFCode(); //ScrapHoldCode();
	m_RecycleHelper->Execute(*this); //RecyclePointCode();
	m_PilotResourceHelper->Execute(*this); //PilotCode();
	m_UndeployHelper->Execute(*this); //UndeployCode();
	m_LimiterHelper->Execute(*this); //LimiterCode();
	m_APCHelper->Execute(*this); //APCCode();
	m_LPadHelper->Execute(*this); //LPadCode();
	m_HQCPHelper->Execute(*this); //HQCPCode();
	m_PortalHelper->Execute(*this); //PortalCode();
	m_AttacherHelper->Execute(*this); //AttacherCode();
	m_ReplaceOnDeathHelper->Execute(*this); //ReplaceOnDeathCode();
	m_UtilityShipHelper->Execute(*this); //ScavengerCode();
	m_DeployHelper->Execute(*this);
	m_FuryHelper->Execute(*this);
	//m_LavaDamageHelper->Execute(*this);
	m_ScrapHelper->Execute(*this);
	m_DispatchHelper->Execute(*this);
	m_SatelliteHelper->Execute(*this);
	m_SwitchClassHelper->Execute(*this);
	m_AnchorClassHelper->Execute(*this);
	m_AmmoDrainHelper->Execute(*this);
	m_QuakeHelper->Execute(*this);
	m_GeyserHelper->Execute(*this);
	m_AttacherWeaponHelper->Execute(*this);
	m_MorphObjectHelper->Execute(*this);
	m_ShieldHelper->Execute(*this);
	m_EnergyArmorHelper->Execute(*this);
	m_JammerHelper->Execute(*this);
	m_CarrierHelper->Execute(*this);
	m_TransportHelper->Execute(*this);
	m_DropShipHelper->Execute(*this);
	m_RecallMineHelper->Execute(*this);

	if(m_ReinforcementTime)
		ExecuteReinforcements();

	// Store this counter here so we can compare with the last known number instead of the current number.
	m_LastObjectListCount = HandleList.size();
}

// PreSnipe, what happens if a sniper rifle hits a thingy. :)
// Notification to the DLL: a sniper shell has hit a piloted craft. The exe passes the current world, shooters handle, victim handle, and the ODF string of the 
// ordnance involved in the snipe. Returns a code detailing what to do. If DLLs want to do any actions to the world based on this PreSnipe callback, they should 
// (1) Ensure curWorld == 0 (lockstep) -- do NOTHING if curWorld is != 0, and (2) probably queue up an action to do in the next Execute() call.
PreSnipeReturnCodes BZ1Helper::PreSnipe(const int curWorld, Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	// If this is a sniper round, read the TeamFilter from the ODF to see if we should snipe.
	PreSnipeReturnCodes KillIt = PRESNIPE_KILLPILOT; // Assume we're successful.
	int TempTeamFilter = 0;
	int Team = GetTeamNum(victimHandle);
	int ShooterTeam = ordnanceTeam;
	if(!ShooterTeam)
		ShooterTeam = GetTeamNum(shooterHandle);

	char TempODF[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	strcpy_s(TempODF, pOrdnanceODF);
	if(OpenODF2(TempODF))
	{
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf");
			OpenODF2(TempODF2);
		}
	}

	if((GetODFInt(TempODF, "SniperShellClass", "TeamFilter", &TempTeamFilter, 0)) || // Always snipe.
		(GetODFInt(TempODF2, "SniperShellClass", "TeamFilter", &TempTeamFilter, 0)))
	{
		// Got it.
	}

	// Team 0 can always snipe us.
	if((!ShooterTeam) || (TempTeamFilter == 0))
		TempTeamFilter = 7;

	// If Friendly Fire is off, then see if we should disallow the snipe
	if(IsNetworkOn())
	{
		if(!m_IsFriendlyFireOn)
		{
			const TEAMRELATIONSHIP relationship = GetTeamRelationship(shooterHandle, victimHandle);

			if((//(relationship == TEAMRELATIONSHIP_SAMETEAM) || // Allow same team snipes. -GBD
			   (relationship == TEAMRELATIONSHIP_ALLIEDTEAM)) && 
			   (IsPlayer(victimHandle) || Team != 0)) // Allow snipes of items on team 0/perceived team 0, as long as they're not a local/remote player.
			   KillIt = PRESNIPE_ONLYBULLETHIT; // Nope.
		}
	}

	// If it's still allowed to be sniped, check the TeamFilter setting.
	if(KillIt == PRESNIPE_KILLPILOT)
	{
		if(TeamFilterCheck(TempTeamFilter, Team, ShooterTeam))
			SetPerceivedTeam(victimHandle, 0); // Set it's perceived team to 0. It's about to have a very unpleasent evening.
		else
			KillIt = PRESNIPE_ONLYBULLETHIT; // Nope.
	}

	return KillIt; // Do the thing.
}

// PreOrdnanceHit. Happens anytime a bullet hits something.
void BZ1Helper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	char BZCClass[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
	strcpy_s(TempODF, pOrdnanceODF);
	if(OpenODF2(TempODF))
	{
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf");
			OpenODF2(TempODF2);
		}
	}

	GetODFString(TempODF, TempODF2, "OrdnanceClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);
	unsigned int BZCClassHash = Hash(BZCClass);

	// ST mode Damage Multiplier. Uses ivar26. Note: This only effects direct Ordnance impacts, and only on Vehicles or Buildings with canCollide = true.
	if((IsNetworkOn()) && (m_GameType == GameType_ST))
	{
		long OrdDamage = GetOrdnanceDamageDealt(victimHandle, pOrdnanceODF, true);
		long MultDamage = long((OrdDamage * m_DamageMultiplier));
		long DamageDifference = (MultDamage - OrdDamage);
		if(GetCurHealth(victimHandle) >= DamageDifference) // This would not KILL the target, we want the actual ordnance to deal the killing blow for credits.
			Damage(victimHandle, DamageDifference); // Apply the difference before the ordnance hits us.
	}

	m_EnergyArmorHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF, TempODF, TempODF2);
	m_DispatchHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);
	m_APCHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);
	m_ReplaceOnDeathHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);

	if(!victimHandle)
		return; // No shooter reported, done early.

	switch (BZCClassHash)
	{
	case BZCCLASS_ORDNANCE_SWITCH:			// SwitchClassHelper.h
		m_SwitchClassHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF, TempODF, TempODF2);
		break;
	case BZCCLASS_ORDNANCE_VAMPIRE:			// AmmoDrainHelper.h
		m_AmmoDrainHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF, TempODF, TempODF2);
		break;
	case BZCCLASS_ORDNANCE_ANCHOR:			// AnchorHelper.h
		m_AnchorClassHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF, TempODF, TempODF2);
		break;
	}

	m_CarrierHelper->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);
}

// PreGetIn. Happens when a pilot gets into a ship.
PreGetInReturnCodes BZ1Helper::PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle)
{
	m_SatelliteHelper->PreGetIn(curWorld, pilotHandle, emptyCraftHandle);

	// Update the OriginalTeam number of the object with the new Team if it changed.
	int PilotTeam = GetTeamNum(pilotHandle);

	if(GetTeamNum(emptyCraftHandle) != PilotTeam)
	{
		for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
		{
			if(iter->ObjectHandle == emptyCraftHandle)
			{
				iter->OriginalTeam = PilotTeam; // Update their original Team #.
				break;
			}
		}
	}
	
	// Bug ish? fix: Clear the last orders this unit had if a Player is on the team.
	if(GetPlayerHandle(PilotTeam))
		SetCommand(emptyCraftHandle, CMD_NONE, 0);  // Release F group control / Clear orders.

	// Always allow the entry
	return PREGETIN_ALLOW;
}

// BZ1Interface Code
void BZ1Helper::BZ1Interface(void)
{
	Handle TempPlayerH = GetPlayerHandle();
	if(!TempPlayerH)
		return;

	// This *should* set max pilots in the shell to the local player's max power number?
	int Team = GetTeamNum(TempPlayerH);

	// Find out if the Escape Menu is active. Custom CFG set Var in bzgame_enter/exit.cfgs.
	int EscapeActive = IFace_GetInteger("shell.dll.escape.active"); //GetVarItemInt("shell.dll.escape.active"); // GetVarItem looks at ivars first, then elsewhere. Should only be used for network.session.ivar*'s.

	// Do Repeat Audio Keybind:
	int RepeatAudio = IFace_GetInteger("shell.dll.repeataudio");
	if(RepeatAudio)
	{
		if((!EscapeActive) && (!IsAudioMessagePlaying()))
			RepeatAudioMessage();

		IFace_SetInteger("shell.dll.repeataudio", 0);
	}

	// Open shell if YOUR team is dead. 
	if((m_MaxLives > 0) && (GetTeamInfo(GetLocalPlayerTeamNumber()).TeamIsDead) && (EscapeActive == 0)) // Lives are on, and YOU'VE run out, HaHa! -GBD
		IFace_ConsoleCmd("mission.escape");

	for(int x = 0; x<9;x++)
	{
		int i = x+1;
		if(x > 2)
			i = x+6;

		Handle TempSlot = GetObjectByTeamSlot(Team,i);
		if(!TempSlot)
			continue;

		char DesiredValue[MAX_ODF_LENGTH] = {0};
		sprintf_s(DesiredValue, "shell.dll.base%dhull", x+1);
		IFace_SetFloat(DesiredValue, GetHealth(TempSlot));

		char BaseSlotName[MAX_ODF_LENGTH] = {0};
		GetODFString(TempSlot, "GameObjectClass", "shellName", MAX_ODF_LENGTH, BaseSlotName);
	
		if(!BaseSlotName[0])
		{
			if((x == 0) || (x == 4))
				strcpy_s(BaseSlotName, "Recycler");
			else if((x == 1) || (x == 5))
				strcpy_s(BaseSlotName, "Factory");
			else if((x == 2) || (x == 6))
				strcpy_s(BaseSlotName, "Armory");
			else if((x == 3) || (x == 7))
				strcpy_s(BaseSlotName, "Constructor");
			else if(x == 8)
				strcpy_s(BaseSlotName, "Satellite");
		}

		sprintf_s(DesiredValue, "shell.dll.basename%d", x+1);
		IFace_SetString(DesiredValue, BaseSlotName);
	}

	if(IsNetworkOn() && IsTeamplayOn())
		Team = GetCommanderTeam(Team);

	if(teamInfo[Team].m_UsePilots)
	{
		IFace_SetInteger("shell.dll.currpilots", teamInfo[Team].CurrPilots);
		IFace_SetInteger("shell.dll.maxpilots", teamInfo[Team].MaxPilots);
		IFace_SetString("shell.dll.pilotname", "Pilot");
	}
	else
	{
		IFace_SetInteger("shell.dll.currpilots", GetPower(Team));
		IFace_SetInteger("shell.dll.maxpilots", GetMaxPower(Team));
		IFace_SetString("shell.dll.pilotname", "Power");
	}

	if(HullGaugeSet)
	{
		IFace_SetFloat("shell.dll.hull2", HullGauge2);
		HullGaugeSet = false;
	}
	else
	{
		IFace_SetFloat("shell.dll.hull2", 0.0f);
	}

	if(AmmoGaugeSet)
	{
		IFace_SetFloat("shell.dll.ammo2", AmmoGauge2);
		AmmoGaugeSet = false;
	}
	else
	{
		IFace_SetFloat("shell.dll.ammo2", 0.0f);
	}

	if(AuxGaugeSet)
	{
		IFace_SetFloat("shell.dll.gauge", AuxGauge);
		AuxGaugeSet = false;
	}
	else
	{
		IFace_SetFloat("shell.dll.gauge", 0.0f);
	}
}

// Loop over all teams, and update lives, handle unit passing if this team is dead.
void BZ1Helper::BZ1Lives(void)
{
	// Track some things per team. This is mostly for updating things...
	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		// Death Camera Sequence: -GBD // BZ1 "DeathCam"
		if((teamInfo[Team].DeathCameraOn) && (IsAround(teamInfo[Team].DeathCameraTarget)))
		{
			if(CameraCircle(teamInfo[Team].DeathCameraTarget, Vector(10,10,10), teamInfo[Team].DeathCameraTarget, Vector(1,1,1)) || (m_TurnCounter > teamInfo[Team].DeathCameraTime))
			{
				CameraFinish();
				teamInfo[Team].DeathCameraOn = false;
				teamInfo[Team].DeathCameraTime = 0;
			//	if(IsAround(teamInfo[Team].DeathCameraTarget))
					RemoveObject(teamInfo[Team].DeathCameraTarget);
			}
		}

		// This team iz dead. Watch for revival?
		if(GetTeamInfo(Team).TeamIsDead)
		{
			if(GetTeamInfo(Team).PlayerLives > 0) // They have lives again? Restore their stuff. 
			{
				for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
				{
					// If it is still on the "dead" team 15, and it's original Team is this team, and it's not a player.
					if((iter->CurTeamNum == 15) && (iter->OriginalTeam == Team) && (!IsPlayer(iter->ObjectHandle)))
					{
						SetTeamNum(iter->ObjectHandle, Team);
						iter->CurTeamNum = Team;
					}

					Stop(iter->ObjectHandle, 0); // Restore command. 
				}
			}
			else // Loop over all the player's stored stuffs, and make them inert. You are dead.
			{
				for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
				{
					if((iter->CurTeamNum == Team) && (!IsPlayer(iter->ObjectHandle)))
					{
						if(!IsTeamplayOn())
						{
							SetTeamNum(iter->ObjectHandle, 15); // Throw it onto Team 15.
							iter->CurTeamNum = 15;
						}
						else
						{
							int NewTeam = GetCommanderTeam(Team);
							SetTeamNum(iter->ObjectHandle, NewTeam); // Throw it onto Commander's Team.
							iter->CurTeamNum = NewTeam;
						}

						Stop(iter->ObjectHandle, 1); 
						SetBestGroup(iter->ObjectHandle);
						//SetPerceivedTeam(iter->ObjectHandle, Team); // set percieved team to their old team.
					}
				} // i loop over all objects
			}
		} // End object tracker update code.
	}

}

// Setup clouds until they're all placed.
void BZ1Helper::SetupClouds(void)
{
	//bool Logging = BZ1Helper::Logging; //true;

	// Count per mapsize, and tilesize, and all other settings from trn.  
	//float MapHeight = 0, MapMPG = 0;
	//float MapWidth = 0, MapDepth = 0;
	float CloudMaxX = 0, CloudMaxZ = 0, CloudMinX = 0, CloudMinZ = 0;
	int Type = 0, CloudTypes = 0, CloudCount = 0, TileSize = 0; //, VisibilityRange = 300;
	int CloudTeam[MAX_CLOUD_TYPES] = {0};
	float CloudHeight[MAX_CLOUD_TYPES] = {0};
	char CloudName[MAX_CLOUD_TYPES][MAX_ODF_LENGTH] = {0};

	/*
	size_t bufSize = 0;
	GetPathPoints("edge_path", bufSize, NULL);
	float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
	VECTOR_2D EdgePoints[2];
	memset(EdgePoints, 0, sizeof(EdgePoints));
	if(GetPathPoints("edge_path", bufSize, pData))
	{
		size_t i;
		for(i = 0; i < bufSize; ++i)
		{
			if(bufSize != 2)
			{
				FormatLogMessage("Malformed Edge_Path! Aborting Cloud Generation.");
				//break;
				return;
			}
			EdgePoints[i].x = pData[2*i+0];
			EdgePoints[i].z = pData[2*i+1];
		}
	}
	*/

	// Read in map trn file values. BTW I didn't know you could just open a random { } without an if statement, cool. -GBD
//	{
	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if(OpenODF2(mapTrnFile))
	{
		//GetODFFloat(mapTrnFile, "Size", "Height", &MapHeight); // Grab the map's original mapheight, used to base height off of. 
		//GetODFFloat(mapTrnFile, "Size", "MetersPerGrid", &MapMPG); // Grab the map's Meters Per Grid, used to base Count off of. 
	//	GetODFFloat(mapTrnFile, "Size", "Width", &MapWidth); // Grab the map's original width, used to base Position off of. 
	//	GetODFFloat(mapTrnFile, "Size", "Depth", &MapDepth); // Grab the map's original depth, used to base Position off of.
		GetODFInt(mapTrnFile, "Clouds", "Type", &Type); // Grab the number of clouds per tile, defaults to 0 (none). 
		GetODFInt(mapTrnFile, "Clouds", "Count", &CloudCount); // Grab the number of clouds per tile, defaults to 0 (none). 
		GetODFInt(mapTrnFile, "Clouds", "TileSize", &TileSize); // How big each tile is.
		//GetODFInt(mapTrnFile, "Sky", "VisibilityRange", &VisibilityRange); // VisibilityRange, as defined in TRN. (may not be what it actually is)

		for (int i = 0; i < MAX_CLOUD_TYPES; i++)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};

			// Find out how many cloud variations there are for this entry, and randomly pick one.
			int CloudRandomTotal = 0;
			for(int x = 0; x < MAX_CLOUD_TYPES; x++)
			{
				char TempCloudName[MAX_ODF_LENGTH] = {0};
				sprintf_s(DesiredValue, "Name%d_%d", i+1, x+1);
				if(GetODFString(mapTrnFile, "Clouds", DesiredValue, MAX_ODF_LENGTH, TempCloudName, 0))
					++CloudRandomTotal;
				else
					break;
			}

			// If we had multiple types possible, randomly pick out of them, else use old method.
			if(CloudRandomTotal > 0)
				sprintf_s(DesiredValue, "Name%d_%d", i+1, GetRandomInt(1, CloudRandomTotal));
			else
				sprintf_s(DesiredValue, "Name%d", i+1);
			
			// Grab the ODF names.
			if(GetODFString(mapTrnFile, "Clouds", DesiredValue, MAX_ODF_LENGTH, CloudName[i], 0))
				++CloudTypes;
			else
				break; // Stop reading if there's a gap.

			// Get the Height.
			sprintf_s(DesiredValue, "Height%d", i+1);
			GetODFFloat(mapTrnFile, "Clouds", DesiredValue, &CloudHeight[i], 100.0f);

			// Get the Team.
			sprintf_s(DesiredValue, "Team%d", i+1);
			GetODFInt(mapTrnFile, "Clouds", DesiredValue, &CloudTeam[i], 0);
		}

		float EdgeMinX = mapInfo.MinWidth;
		float EdgeMaxX = mapInfo.MaxWidth;
		float EdgeMinZ = mapInfo.MinDepth;
		float EdgeMaxZ = mapInfo.MaxDepth;

		/*
		// There's an Edge Path. Use it's positions instead.
		if((EdgePoints[0].x + EdgePoints[0].z + EdgePoints[1].x + EdgePoints[1].z) != 0.0f)
		{
			EdgeMinX = min(EdgePoints[0].x, EdgePoints[1].x);
			EdgeMinZ = min(EdgePoints[0].z, EdgePoints[1].z);

			EdgeMaxX = max(EdgePoints[0].x, EdgePoints[1].x);
			EdgeMaxZ = max(EdgePoints[0].z, EdgePoints[1].z);
		}
		else
		{
			EdgeMinX = -0.5f * mapInfo.Width;
			EdgeMaxX = 0.5f * mapInfo.Width;

			EdgeMinZ = -0.5f * mapInfo.Depth;
			EdgeMaxZ = 0.5f * mapInfo.Depth;
		}
		*/

		float CloudTilesX = TileSize * floorf((EdgeMaxX - EdgeMinX) / TileSize); // + VisibilityRange * 2
		float CloudTilesZ = TileSize * floorf((EdgeMaxZ - EdgeMinZ) / TileSize);
		float CloudCenterX = 0.5f * (EdgeMaxX + EdgeMinX);
		float CloudCenterZ = 0.5f * (EdgeMaxZ + EdgeMinZ);
		CloudMinX = CloudCenterX - CloudTilesX * 0.5f;
		CloudMaxX = CloudCenterX + CloudTilesX * 0.5f;
		CloudMinZ = CloudCenterZ - CloudTilesZ * 0.5f;
		CloudMaxZ = CloudCenterZ + CloudTilesZ * 0.5f;

		/*
		CloudMinX = TileSize * (floorf(EdgeMinX / TileSize)); // - 1);
		CloudMaxX = TileSize * (floorf(EdgeMaxX / TileSize)); // + 1);
		CloudMinZ = TileSize * (floorf(EdgeMinZ / TileSize)); // - 1);
		CloudMaxZ = TileSize * (floorf(EdgeMaxZ / TileSize)); // + 1);
		*/

		//	CloseODF(mapTrnFile);
//		}

		// Initialize a NESW directional matrix array. :)
		Matrix FacingMatrix[4] = { Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)), 
			Matrix(Vector(0, 0, 1), Vector(0, 1, 0), Vector(-1, 0, 0), Vector(0, 0, 0)), 
			Matrix(Vector(-1, 0, 0), Vector(0, 1, 0), Vector(0, 0, -1), Vector(0, 0, 0)), 
			Matrix(Vector(0, 0, -1), Vector(0, 1, 0), Vector(1, 0, 0), Vector(0, 0, 0)) };

		if(CurCloudType < CloudTypes)
		{
			Matrix Position = FacingMatrix[GetRandomInt(3)];

			Position.posit.x = CurTileX + GetRandomFloat(float(TileSize)) - (TileSize * 0.5f);
			Position.posit.z = CurTileZ + GetRandomFloat(float(TileSize)) - (TileSize * 0.5f);
			if(Type)
				Position.posit.y = TerrainFindFloor(Position.posit.x, Position.posit.z) + CloudHeight[CurCloudType];
			else
				Position.posit.y = CloudHeight[CurCloudType];

			BuildObject(CloudName[CurCloudType], CloudTeam[CurCloudType], Position);

			++CurCloudCount;
			if (CurCloudCount >= CloudCount)
			{
				CurCloudCount = 0;
				CurTileX += TileSize;
				if (CurTileX > CloudMaxX)
				{
					CurTileX = CloudMinX;
					CurTileZ += TileSize;
					if (CurTileZ > CloudMaxZ)
					{
						CurTileZ = CloudMinZ;
						++CurCloudType;
						if (CurCloudType >= CloudTypes)
						{
							m_CloudsDone = true;

							if(Logging)
							{
							//	FormatLogMessage("Map Height Is: %d", mapInfo.Height);
								FormatLogMessage("Map Width Is: %f", mapInfo.MapWidth);
								FormatLogMessage("Map Depth Is: %f", mapInfo.MapDepth);
								FormatLogMessage("Clouds Specified Are: %d", CloudTypes);
								FormatLogMessage("Total Clouds are: %f", (floor(EdgeMaxZ/TileSize)-floor(EdgeMinZ/TileSize)+1)*(floor(EdgeMaxX/TileSize)-floor(EdgeMinX/TileSize)+1)*CloudCount*CloudTypes);
								FormatLogMessage("Map TRN Is: %s", mapTrnFile);
							}
						}
					}
				}
			}
		}
		/*
		// For each Type, place clouds for each Count.
		for(int i = 0; i<CloudTypes; ++ i)
		{
			for(float tileZ = CloudMinZ; tileZ <= CloudMaxZ; tileZ += TileSize)
			{
				for(float tileX = CloudMinX; tileX <= CloudMaxX; tileX += TileSize)
				{
					for(int Count = 0; Count < CloudCount; Count++)
					{
						Matrix Position = FacingMatrix[GetRandomInt(3)];

						Position.posit.x = tileX + GetRandomFloat(float(TileSize));
						Position.posit.z = tileZ + GetRandomFloat(float(TileSize));
						Position.posit.y = CloudHeight[i];

						BuildObject(CloudName[i], CloudTeam[i], Position);
						++TotalClouds;
					}
				}
			}
		}
		*/
	}
}

// Moves cloud tiles around based on Player positions.
/*
void BZ1Helper::MoveClouds(void)
{

}
*/

// Rotate the Sun Sprite. Doesn't work in bzone.exe, only in BZ2Edit.exe. :(
void BZ1Helper::DayNightCycle(void)
{
	if(SunSpriteIndex >= 0)
	{
		IFace_FormatConsoleCmd("sprites.select %d", SunSpriteIndex);
		float SpriteElev = 360.0f * fmodf((SunAngle + m_TurnCounter / ConvertToTurns(3600.0f)) / SunPeriod, 1.0f) - 90.0f;
		IFace_FormatConsoleCmd("sprites.elev %f", SpriteElev);

		if(Logging)
			FormatLogMessage("New SunSprite Elev is: %f", SpriteElev);
	}
}

// Run the Gravity, and etc systems.
void BZ1Helper::EnvironmentalSystems(void)
{
	int CurrState = mapInfo.GravityState;
	int onperiod = ConvertToTurns(gravityInfo[CurrState].OnPeriod);
	int NextState = (mapInfo.GravityState + 1) % MAX_GRAVITY_TYPES;

	if(gravityInfo[NextState].OnPeriod <= 0)
	{
		// Find our next gravity state.
		int x = NextState;
		do
		{
			if(gravityInfo[x].OnPeriod > 0)
				break;

			if (++x == MAX_GRAVITY_TYPES-1)
				x = 0;
		}
		while (x != NextState);

		NextState = x;
	}

	if(onperiod <= 0) // || ((mapInfo.GravityState + 1) % MAX_GRAVITY_TYPES == 0))
	{
		// Time to switch. 
		mapInfo.GravityState = (mapInfo.GravityState + 1) % MAX_GRAVITY_TYPES;

	//	if(Logging)
	//		FormatLogMessage("Gravity State skipped, next Gravity State: %d", mapInfo.GravityState);
	}
	else
	{
		int tranperiod = ConvertToTurns(gravityInfo[NextState].TranPeriod);

		if(m_TurnCounter == 1) // This is our first time run, set intial gravity here.
		{
			mapInfo.Gravity = gravityInfo[CurrState].Gravity;
			SetGravity(mapInfo.Gravity);
		}

		// Count the on period time.
		gravityInfo[CurrState].OnCounter++;

		// our on time is up, time to switch.
		if(gravityInfo[CurrState].OnCounter >= onperiod)
		{
			// If we haven't calculated our deltaGravity, do it now.
			if(!gravityInfo[NextState].Gravitation)
			{
				if(tranperiod > 0)
					gravityInfo[NextState].Gravitation = ((gravityInfo[NextState].Gravity - mapInfo.Gravity) / tranperiod);
				else
					gravityInfo[NextState].Gravitation = (gravityInfo[NextState].Gravity - mapInfo.Gravity);

				if(Logging)
					FormatLogMessage("Gravity: %d NextGravity: %d, CurGravity: %f, NextGravity: %f, Gravitation is: %f, TranPeriod is: %f", CurrState, NextState, mapInfo.Gravity, gravityInfo[NextState].Gravity, gravityInfo[NextState].Gravitation, gravityInfo[NextState].TranPeriod);
			}

			// If our gravity is not == to it yet, do it.
			if(fabsf(mapInfo.Gravity - gravityInfo[NextState].Gravity) > 0.001f)
			{
				mapInfo.Gravity += gravityInfo[NextState].Gravitation;
				SetGravity(mapInfo.Gravity);

			//	if(Logging)
			//		FormatLogMessage("Gravity: %d NewGravity is: %f, TargetGravity is: %f", CurrState, mapInfo.Gravity, gravityInfo[NextState].Gravity);
			}
			else
			{
				// Reset these.
				gravityInfo[CurrState].OnCounter = 0;
				gravityInfo[CurrState].Gravitation = 0;
				// Time to switch. 
				mapInfo.GravityState = (mapInfo.GravityState + 1) % MAX_GRAVITY_TYPES;

				if(Logging)
					FormatLogMessage("Gravity State over, next Gravity State: %d", mapInfo.GravityState);
			}
		}
	}

}

// Reinforcements, spawns and creates Transports that land nearby a team's production units, and deploys additional units/repair/ammo.
void BZ1Helper::ExecuteReinforcements(void)
{
	//bool Logging = BZ1Helper::Logging; //false; 

	for(int Team = 0; Team < MAX_TEAMS; Team++)
	{
		teamInfo[Team].ReinforcementCount++;

		if((teamInfo[Team].ReinforcementCount % m_ReinforcementTime == 0) && (!IsAround(teamInfo[Team].Transport)))
		{
			char TeamRace = 0;

			Vector ReinforcementSpawn = Vector(0, 0, 0);

			// Set this here. 
			Handle TempProdUnit = teamInfo[Team].Recy;
			if(!IsAround(TempProdUnit))
				TempProdUnit = teamInfo[Team].Muf;
			if(!IsAround(TempProdUnit))
				TempProdUnit = teamInfo[Team].Slf;
			if(!IsAround(TempProdUnit))
				TempProdUnit = teamInfo[Team].Cnst;

			if(IsAround(TempProdUnit)) // Only grab a new position if the recy is alive. If it's dead, continue to use last known position. 
			{
				GetPosition(TempProdUnit, teamInfo[Team].ReinforcementPos);
				TeamRace = GetRace(TempProdUnit);
			}
			else
			{
				TeamRace = GetRaceOfTeam(Team);
			}

			//if(TeamIsFunctioning[Team]) //!! This stops thug's from getting transports? maybe. Test.  
			if(IsAround(TempProdUnit))
			{
				if(Logging)
					FormatLogMessage("Timer is: %d, Building Transport", teamInfo[Team].ReinforcementCount); 

				ReinforcementSpawn = GetPositionNear(teamInfo[Team].ReinforcementPos, m_ReinforcementMinRadius, m_ReinforcementMaxRadius);
				// Try to find a more suitable position if this one looks like it might be on a cliff.
				for(int Tries = 0; Tries < 50; Tries++)
				{
					float X = FLT_MAX;

					if(teamInfo[Team].ReinforcementPos.y > ReinforcementSpawn.y)
						X = teamInfo[Team].ReinforcementPos.y - ReinforcementSpawn.y;
					else
						X = ReinforcementSpawn.y - teamInfo[Team].ReinforcementPos.y;

					if(X < m_ReinforcementMaxCliff)
						break;
					else
						ReinforcementSpawn = GetPositionNear(teamInfo[Team].ReinforcementPos, m_ReinforcementMinRadius, m_ReinforcementMaxRadius);
				}

				// Build the transport. 
				char HTransportODF[MAX_ODF_LENGTH] = {0};
				sprintf_s(HTransportODF, "%cvtrans_lnto", TeamRace);
				if(!DoesODFExist(HTransportODF))
					sprintf_s(HTransportODF, "%cvdroptank", TeamRace);

				if(DoesODFExist(HTransportODF))
				{
					char TransODF[MAX_ODF_LENGTH] = {0};
					sprintf_s(TransODF, "%s.odf", HTransportODF);
					float Altitude = 0.0f;
					GetODFFloat(TransODF, "HoverCraftClass", "setAltitude", &Altitude, 300.0f);

					ReinforcementSpawn.y = (TerrainFindFloor(ReinforcementSpawn.x, ReinforcementSpawn.z) + Altitude); // spawn the transport up in the air. 

					teamInfo[Team].Transport = BuildTransportObject(HTransportODF, Team, ReinforcementSpawn, 5.0f, 0.0f, 0, 0, m_ReinforcementAmount, true, true, false);

					if(Team == GetLocalPlayerTeamNumber())
						AddToMessagesBox2("-- Transport Inbound!", YELLOW);
				}
			}
		}
	}
} // End reinforcement code.

// Public Accessories. These allow other DLL's to do stuff. 
bool BZ1Helper::SetAIPFile(const char* Value, const int Team)
{
	SetPlan(Value, Team);

	return UpdateTeamAIP(Value, Team);
}
bool BZ1Helper::UpdateTeamAIP(const char* AIPFile, const int Team)
{
	if(OpenODF2(AIPFile))
	{
		strcpy_s(teamInfo[Team].TeamAIPFile, AIPFile);
		{
			GetODFInt(AIPFile, "Cheat", "pilotAmount", &teamInfo[Team].m_CPUPilotCheatAmount, 0); // Grab the value.
			GetODFInt(AIPFile, "Cheat", "pilotDelay", &teamInfo[Team].m_CPUPilotCheatDelay, 15); // Grab the value.
			GetODFInt(AIPFile, "Start", "Difficulty", &teamInfo[Team].m_CPUAIDifficulty, 0); // Grab the value.
			teamInfo[Team].m_CPUAIDifficulty = clamp(teamInfo[Team].m_CPUAIDifficulty, 0, 4);
			GetODFBool(AIPFile, "Start", "UnitsService", &teamInfo[Team].m_CPUUnitsCanService, false); // Grab the value.
		//	GetODFFloat(AIPFile, "Start", "PatrolChance", &teamInfo[Team].m_CPUPatrolChance, 0.0f); // Grab the value.
		//	GetODFFloat(AIPFile, "Start", "HuntChance", &teamInfo[Team].m_CPUHuntChance, 0.0f); // Grab the value.
			GetODFBool(AIPFile, "IdleDispatcher", "DoDLLDispatch", &teamInfo[Team].m_CPUDoDispatch, true); // Grab the value.
			GetODFInt(AIPFile, "IdleDispatcher", "MaxPatrolUnits", &teamInfo[Team].m_CPUMaxPatrolUnits, 0); // Grab the value.
			GetODFInt(AIPFile, "IdleDispatcher", "MaxHuntUnits", &teamInfo[Team].m_CPUMaxHuntUnits, 0); // Grab the value.
			GetODFInt(AIPFile, "IdleDispatcher", "MaxEscortUnits", &teamInfo[Team].m_CPUMaxEscortUnits, 0); // Grab the value.

			if(m_CPUUseDayWreckers)
				GetODFFloat(AIPFile, "Start", "DayWreckerChance", &teamInfo[Team].m_CPUDayWreckerChance, 0.0f); // Grab the value.

		}
	}
	return (Team >= 0 || Team < 15) && teamInfo[Team].TeamAIPFile[0];
}
// Adds a unit to BZC's custom AI Dispatch code, with options.
bool BZ1Helper::AddToDispatch(const Handle h, const float IdleTime, const bool SkipAIPCheck, const int CloakPriority, const bool CanFlee, const bool CloakNow, const bool Hunt, const bool CanGetService)
{
	return m_DispatchHelper->AddToDispatch(h, IdleTime, SkipAIPCheck, CloakPriority, CanFlee, CloakNow, Hunt, CanGetService);
}
// Cloak functions.
void BZ1Helper::Cloak(const Handle h, const int Priority) { m_DispatchHelper->Cloak(h, Priority); }
void BZ1Helper::Decloak(const Handle h, const int Priority) { m_DispatchHelper->Decloak(h, Priority); }
bool BZ1Helper::IsCloaked(const Handle h) { return m_DispatchHelper->IsCloaked(h); }
//void CloakAttack(Handle me, Handle him, int Priority) { return m_DispatchHelper->CloakAttack(me, him, Priority); }
Handle BZ1Helper::SetCloakAllowed(const Handle h, const bool IsAllowed, const bool PlaySound)
{
	bool CanCloak = false;
	if(!GetODFBool(h, "MorphTankClass", "HiddenWhenMorphed", &CanCloak, false))
		return h; // Not a cloak tank, return original Handle.

	if(((IsAllowed) && (CanCloak)) || ((!IsAllowed) && (!CanCloak)))
		return h; // Setting already matches, abort early and notify that the function didn't do anything.

	char ODFName[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "CloakSwitchODF", MAX_ODF_LENGTH, ODFName);

	char CloakFailSound[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "CloakFailSound", MAX_ODF_LENGTH, CloakFailSound, "cloakfail.wav");

	// If this isn't set, try the alternate method of using file name schemes.
	if(!DoesODFExist(ODFName))
	{
		// If this ODF does not exist, it might have been built by a factory, check for that.
		GetODFName(h, ODFName);

		// Remove the "World Letter", if it exists.
		char TestODF[MAX_ODF_LENGTH] = {0};
		int length = strlen(ODFName)-1;
		strncpy_s(TestODF, ODFName, length);

		if((TestODF[0]) && (DoesODFExist(TestODF)))
			strncpy_s(ODFName, TestODF, length); // If this had a world letter, copy in the new ODFName, otherwise leave it as is.

		strcat_s(ODFName, "NC");
	}

	if((h == GetPlayerHandle()) && (ODFName[0]) && (DoesODFExist(ODFName)) && (PlaySound) && (!IsAllowed) && (IsCloaked(h)) && (DoesFileExist(CloakFailSound)))
		AudioMessage(CloakFailSound);
	
	if((ODFName[0]) && (DoesODFExist(ODFName)))
		return ReplaceObject(h, ODFName);
	else
		return h;
}
// Other Dispatch related functions.
void BZ1Helper::DLLHunt(const Handle h, const int Priority) { m_DispatchHelper->DLLHunt(h, Priority); }
void BZ1Helper::SetUseService(const Handle h, const bool Service) { m_DispatchHelper->SetUseService(h, Service); }
// Gets an Escort Target.
Handle BZ1Helper::GetEscortTarget(const int Team, const int TeamFilter) { return m_UtilityShipHelper->GetEscortTarget(Team, TeamFilter); }

// DLL driven Quake functions. These interact with BZC's DayWrecker quake to prevent them from conflicting.
void BZ1Helper::DLLStartEarthQuake(const float Magnitude) { m_QuakeHelper->DLLStartEarthQuake(Magnitude); }
void BZ1Helper::DLLUpdateEarthQuake(const float Magnitude) { m_QuakeHelper->DLLUpdateEarthQuake(Magnitude); }
void BZ1Helper::DLLStopEarthQuake(void) { m_QuakeHelper->DLLStopEarthQuake(); }

// Portal functions. 
// Sets Channel Get/Set.
void BZ1Helper::SetPortalChannel(const Handle h, const int Channel) { m_PortalHelper->SetPortalChannel(h, Channel); }
int BZ1Helper::GetPortalChannel(const Handle h) { return m_PortalHelper->GetPortalChannel(h); }
// Portal Effects
void BZ1Helper::SetPortalEffectStart(const Handle h, const int FxNumber, const int Type) { m_PortalHelper->SetPortalEffectStart(h, FxNumber, Type); }
void BZ1Helper::SetPortalEffectOn(const Handle h, const int FxNumber, const int Type) { m_PortalHelper->SetPortalEffectOn(h, FxNumber, Type); }
void BZ1Helper::SetPortalEffectEnd(const Handle h, const int FxNumber, const int Type) { m_PortalHelper->SetPortalEffectEnd(h, FxNumber, Type); }
bool BZ1Helper::IsPortalEffectActive(const Handle h) { return m_PortalHelper->IsPortalEffectActive(h); }

// Sets if a Pilot is DLL Controlled or not.
void BZ1Helper::SetPilotIndependence(const Handle h, const bool Independence) { return m_PilotResourceHelper->SetPilotIndependence(h, Independence); }

// Is a building Powered? 
bool BZ1Helper::IsBuildingPowered(const Handle h) { return m_BuildingHelper->IsBuildingPowered(h); }
// Set a building as Powered/Unpowered/Autodetermine. 0 = Unpowered, 1 = Powered, -1 = Autodetermine.
void BZ1Helper::SetBuildingPowered(const Handle h, const int Powered) { return m_BuildingHelper->SetBuildingPowered(h, Powered); }

// Scrap Accessor Functions.
void BZ1Helper::GetRidOfSomeScrap(const int ScrapLimit) { return m_ScrapHelper->GetRidOfSomeScrap(ScrapLimit); }
void BZ1Helper::ClearScrapAround(const Dist d, const Vector &v, const float Percent) { return m_ScrapHelper->ClearScrapAround(d, v, Percent); }

// Builds a Transport Object.
Handle BZ1Helper::BuildTransportObject(const char *ODF, const int Team, const Matrix Position, const float DownTime, const float HeightOffset, const int PilotAmount, const int ScrapAmount, const int UnitsAmount, const bool SpawnPowerups, const bool Objectify, const bool AddToGroup) {
	return m_TransportHelper->BuildTransportObject(ODF, Team, Position, DownTime, HeightOffset, PilotAmount, ScrapAmount, UnitsAmount, SpawnPowerups, Objectify, AddToGroup); }
Handle BZ1Helper::BuildTransportObject(const char *ODF, const int Team, const Handle him, const float DownTime, const float HeightOffset, const int PilotAmount, const int ScrapAmount, const int UnitsAmount, const bool SpawnPowerups, const bool Objectify, const bool AddToGroup)
{
	if(!IsAround(him))
	{
		FormatLogMessage("ERROR: BuildObject Failed, Handle missing, cannot build ODF %s", ODF); 
		return 0;
	}
	Matrix BuildLoc = Identity_Matrix; //Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)); // Initialize default Matrix.
	// Special case for LPads.
	if(IsLPad(him))
		BuildLoc = GetODFPositionOffset(him, 0);
	else
		GetPosition(him, BuildLoc);

	return BuildTransportObject(ODF, Team, BuildLoc, DownTime, HeightOffset, PilotAmount, ScrapAmount, UnitsAmount, SpawnPowerups, Objectify, AddToGroup);
}
Handle BZ1Helper::BuildTransportObject(const char *ODF, const int Team, const Name APath, const float DownTime, const float HeightOffset, const int PilotAmount, const int ScrapAmount, const int UnitsAmount, const bool SpawnPowerups, const bool Objectify, const bool AddToGroup)
{
	if(!DoesPathExist(APath))
	{
		FormatLogMessage("ERROR: Pathpoint %s missing, cannot build ODF %s", APath, ODF); 
		return 0;
	}
	//Matrix BuildLoc = Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), GetVectorFromPath(APath)); // Initialize default Matrix.

	return BuildTransportObject(ODF, Team, Build_Identity_Matrix(GetVectorFromPath(APath)), DownTime, HeightOffset, PilotAmount, ScrapAmount, UnitsAmount, SpawnPowerups, Objectify, AddToGroup);
}
Handle BZ1Helper::BuildTransportObject(const char *ODF, const int Team, const AiPath *APath, const float DownTime, const float HeightOffset, const int PilotAmount, const int ScrapAmount, const int UnitsAmount, const bool SpawnPowerups, const bool Objectify, const bool AddToGroup)
{
	Handle TempH = BuildObject("abstor",0,APath);
	if(!IsAround(TempH))
		return 0;
	Matrix BuildLoc = GetMatrixPosition(TempH);
	RemoveObject(TempH);

	return BuildTransportObject(ODF, Team, BuildLoc, DownTime, HeightOffset, PilotAmount, ScrapAmount, UnitsAmount, SpawnPowerups, Objectify, AddToGroup);
}
Handle BZ1Helper::BuildTransportObject(const char *ODF, const int Team, const Vector &pos, const float DownTime, const float HeightOffset, const int PilotAmount, const int ScrapAmount, const int UnitsAmount, const bool SpawnPowerups, const bool Objectify, const bool AddToGroup)
{
	//Matrix BuildLoc = Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), pos); // Initialize default Matrix.
	return BuildTransportObject(ODF, Team, Build_Identity_Matrix(pos), DownTime, HeightOffset, PilotAmount, ScrapAmount, UnitsAmount, SpawnPowerups, Objectify, AddToGroup);
}

// Adds an object to be replaced when it dies.
void BZ1Helper::ReplaceOnDeath(const Handle h, const char *ReplaceODF, const int Team, const float HeightOffset, const bool DoExplode, const bool StaysTap, const float MinReplaceTime, const float MaxReplaceTime, const bool RandomizeTime)
{
	m_ReplaceOnDeathHelper->AddReplaceOnDeathObject(h, ReplaceODF, Team, HeightOffset, DoExplode, StaysTap, MinReplaceTime, MaxReplaceTime, RandomizeTime);
}

// Is this object blocked by a building?
Handle BZ1Helper::IsBlockedByBuilding(const Handle h1, const Handle h2, const float WidthPadding, const Vector FrontA, const Vector FrontB, const bool IgnoreInvincible)
{
	// Find out if GetPosition(h, matrix) works in DeleteObject? Should maybe attempt to use GetPosition2 somewhere here? Maybe?
	if(!IsAround(h1) || !IsAround(h2))
		return 0;

	Matrix PositionA = GetMatrixPosition(h1);
	Matrix PositionB = GetMatrixPosition(h2);
	Handle TempH = 0;

	if(!IsNullVector(FrontA))
		PositionA = Build_Directinal_Matrix(PositionA.posit, FrontA);

	if(!IsNullVector(FrontB))
		PositionB = Build_Directinal_Matrix(PositionB.posit, FrontB);

	//for (std::vector<BuildingClass>::iterator iter = BuildingList.begin(); iter != BuildingList.end(); ++iter)
	for (int i = 0; i < m_BuildingHelper->GetNumBuildingObjects(); i++)
	{
		BuildingHelper::BuildingClass buildingInfo = m_BuildingHelper->GetBuildingInfo(i);
		Handle Building = buildingInfo.BuildingObject;
		bool Proceed = true;
		// Loop over, make sure we ignore these object's Taps.
		for(int x = 0; x < buildingInfo.TapNumber; x++)
		{
			Handle tap = GetTap(Building, x);
			if((tap == h1) || (tap == h2) || (GetTap(h1, x) == Building) || (GetTap(h2, x) == Building))
			{
				Proceed = false;
				break;
			}
		}
		if(!Proceed)
			continue; // Skip to next building.

		// Make sure we don't include ourselves, or invincibles if we're ignoring them.
		if((Building != h1) && (Building != h2) && ((!IgnoreInvincible) || (GetMaxHealth(Building) > 0)))
		{
			Vector buildingLocalPositionA = Vector_TransformInv(PositionA, GetPosition(Building));
			Vector buildingLocalPositionB = Vector_TransformInv(PositionB, GetPosition(Building));
			const float buildingRadius = buildingInfo.CollisionRadius; //iter->CollisionRadius;
			const float totalLength = GetDistance2D(PositionA.posit, PositionB.posit);

			if ((fabsf(buildingLocalPositionA.x) < WidthPadding + buildingRadius) && (buildingLocalPositionA.z > -buildingRadius) && (buildingLocalPositionA.z < totalLength + buildingRadius))
			{
				TempH = Building;
				/*
				FormatLogMessage("IsBuildingBlockedByBuilding Connection from %08x -> %08x Blocked by: %08x, ODF: %s, CollisionRadius: %f, WidthPadding: %f", h1, h2, Building, GetODFName(Building), buildingRadius, WidthPadding);
				*/
				break;
			}
			/*
			else
			{
				FormatLogMessage("IsBuildingBlockedByBuilding Building %08x %s Not in the way, Allow Connecting: %08x -> %08x,  CollisionRadius: %f, WidthPadding: %f", Building, GetODFName(Building), h1, h2, buildingRadius, WidthPadding);
			}
			*/
		}
	}
	return TempH;
}

// Adds an object to the DeleteObject class list to be removed the next Execute() call.
void BZ1Helper::DeleteAfterDelay(const Handle h, const float Delay)
{
	if(!IsAround(h))
		return;

	for (std::vector<DeleteAfterDelayClass>::iterator iter = DeleteAfterDelayList.begin(); iter != DeleteAfterDelayList.end(); ++iter)
	{
		if(iter->DeleteAfterDelayHandle == h)
			return;
	}

	DeleteAfterDelayList.resize(DeleteAfterDelayList.size()+1);
	DeleteAfterDelayClass &sao = DeleteAfterDelayList[DeleteAfterDelayList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(DeleteAfterDelayClass)); // Zero things out at start.

	sao.DeleteAfterDelayHandle = h;
	sao.TurnsRemaining = ConvertToTurns(Delay);
}

// Gets a random object, with optional type filter masks, teamfilters, team, includetiemsthisturn, and ignoreinvincible options.
Handle BZ1Helper::GetRandomObject(const int Team, const int TeamFilter, const int CategoryMask, const bool IncludeItemsAddedThisTurn, const bool IgnoreInvincible, const int MatchMask, const char *ObjClass, const char *ODFName)
{
	const int NewCategoryMask = CategoryMask ? CategoryMask : -1;
	const int NewTeamFilter = TeamFilter ? TeamFilter : -1;
	const int NewMatchMask = MatchMask ? MatchMask : -1;

	unsigned int ObjClassHash = 0;
	unsigned int ODFNameHash = 0;

	if(ObjClass)
		ObjClassHash = Hash(ObjClass);

	if(ODFName)
		ODFNameHash = Hash(ODFName);

	Handle TempH = 0;

	int Size = IncludeItemsAddedThisTurn ? HandleList.size()-1 : m_LastObjectListCount-1;
	Size = clamp(Size, 0, int(HandleList.size())-1);

	int Start = GetRandomInt(Size-1);
	int x = Start;
	do
	{
		Handle Target = HandleList[x].ObjectHandle;

		if( //(IsAround(Target)) && // It's around.
			((Team < 0) || (TeamFilterCheck(NewTeamFilter, HandleList[x].CurTeamNum, Team))) && // Team Filter matches.
			((!IgnoreInvincible) || (GetMaxHealth(Target) > 0)) && // Health matches.
			(
			(((NewMatchMask & 1) == 1) && 
			((((NewCategoryMask & 1) == 1) && (HandleList[x].CategoryID == Vehicle)) || //IsVehicle(Target)) || // Vehicle
			(((NewCategoryMask & 2) == 2) && (HandleList[x].CategoryID == Building)) || //IsBuilding(Target)) || // Building
			(((NewCategoryMask & 4) == 4) && (HandleList[x].CategoryID == Person)) || //IsPerson(Target)) || // Pilot
			(((NewCategoryMask & 8) == 8) && (HandleList[x].CategoryID == Powerup)) || //IsPowerup(Target)) || // Powerup
			(((NewCategoryMask & 16) == 16) && (HandleList[x].CategoryID == Weapon)) || //IsWeapon(Target)) || // Weapon
			(((NewCategoryMask & 32) == 32) && (HandleList[x].CategoryID == Scrap)) || //IsScrap(Target)) || // Scrap
			(((NewCategoryMask & 64) == 64) && (HandleList[x].CategoryID == Other)))) || //IsMisc(Target) && (!IsScrap(Target))))) || // Misc (Not Scrap)
			(
			((NewMatchMask & 2) == 2) && // Look for a specific Classlabel.
			(ObjClass) && (HandleList[x].ObjClassHash == ObjClassHash)
			) || 
			(
			((NewMatchMask & 4) == 4) && // Look for a specific ODFName.
			(ODFName) && (HandleList[x].ODFNameHash == ODFNameHash)
			))
			)
		{
			TempH = Target;
			break;
		}
		++x;
		if (x == Size)
			x = 0;
	}
	while (x != Start);

	return TempH;
}

// Gets the number of objects with filter options.
int BZ1Helper::GetNumObjects(const int Team, const int TeamFilter, const int CategoryMask, const bool IncludeItemsAddedThisTurn, const bool IgnoreInvincible, const int MatchMask, const char *ObjClass, const char *ODFName)
{
	int Count = 0;
	const int NewCategoryMask = CategoryMask ? CategoryMask : -1;
	const int NewTeamFilter = TeamFilter ? TeamFilter : -1;
	const int NewMatchMask = MatchMask ? MatchMask : -1;

	unsigned int ObjClassHash = 0;
	unsigned int ODFNameHash = 0;

	if(ObjClass)
		ObjClassHash = Hash(ObjClass);

	if(ODFName)
		ODFNameHash = Hash(ODFName);

	int Size = IncludeItemsAddedThisTurn ? HandleList.size()-1 : m_LastObjectListCount-1;
	Size = clamp(Size, 0, int(HandleList.size())-1);

	//for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
	for(int i = 0; i < Size; i++)
	{
		Handle Target = HandleList[i].ObjectHandle;

		if( //(IsAround(Target)) && // It's around.
			((Team < 0) || (TeamFilterCheck(NewTeamFilter, HandleList[i].CurTeamNum, Team))) && // Team Filter matches.
			((!IgnoreInvincible) || (GetMaxHealth(Target) > 0)) && // Health matches.
			(
			(((NewMatchMask & 1) == 1) && 
			((((NewCategoryMask & 1) == 1) && (HandleList[i].CategoryID == Vehicle)) || //IsVehicle(Target)) || // Vehicle
			(((NewCategoryMask & 2) == 2) && (HandleList[i].CategoryID == Building)) || //IsBuilding(Target)) || // Building
			(((NewCategoryMask & 4) == 4) && (HandleList[i].CategoryID == Person)) || //IsPerson(Target)) || // Pilot
			(((NewCategoryMask & 8) == 8) && (HandleList[i].CategoryID == Powerup)) || //IsPowerup(Target)) || // Powerup
			(((NewCategoryMask & 16) == 16) && (HandleList[i].CategoryID == Weapon)) || //IsWeapon(Target)) || // Weapon
			(((NewCategoryMask & 32) == 32) && (HandleList[i].CategoryID == Scrap)) || //IsScrap(Target)) || // Scrap
			(((NewCategoryMask & 64) == 64) && (HandleList[i].CategoryID == Other)))) || //IsMisc(Target) && (!IsScrap(Target))))) || // Misc (Not Scrap)
			(
			((NewMatchMask & 2) == 2) && // Look for a specific Classlabel.
			(ObjClass) && (HandleList[i].ObjClassHash == ObjClassHash)
			) || 
			(
			((NewMatchMask & 4) == 4) && // Look for a specific ODFName.
			(ODFName) && (HandleList[i].ODFNameHash == ODFNameHash)
			))
			)
			Count++;
	}

	return Count;
}

// Gets the nearest object to a Vector, with filtering options.
Handle BZ1Helper::GetNearestObject(const Vector Position, const float MaxRange, const int Team, const int TeamFilter, const int CategoryMask, const bool IgnoreInvincible, const Handle Me, const int MatchMask, const char *ObjClass, const char *ODFName)
{
	const int NewCategoryMask = CategoryMask ? CategoryMask : -1;
	const int NewTeamFilter = TeamFilter ? TeamFilter : -1;
	const int NewMatchMask = MatchMask ? MatchMask : -1;
	Handle TempH = 0;

	unsigned int ObjClassHash = 0;
	unsigned int ODFNameHash = 0;

	if(ObjClass)
		ObjClassHash = Hash(ObjClass);

	if(ODFName)
		ODFNameHash = Hash(ODFName);

	float BestDistSquared = MaxRange <= 0 ? FLT_MAX : Squared(MaxRange); // Start out with MaxRange, or infinite.
	for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
	{
		//int TempPTeam = GetPerceivedTeam(iter->ObjectHandle);
		if( //(IsAround(iter->ObjectHandle)) && // It's around.
			((!Me) || (Me != iter->ObjectHandle)) && // It's not itself, if we passed in a handle.
			//(iter->CurTeamNum == Team) && // Team matches.
			((Team < 0) || (TeamFilterCheck(NewTeamFilter, iter->CurTeamNum, Team))) && // Team Filter matches.
			((!IgnoreInvincible) || (GetMaxHealth(iter->ObjectHandle) > 0)) && // Health matches.
			(
			(((NewMatchMask & 1) == 1) &&
			((((NewCategoryMask & 1) == 1) && (iter->CategoryID == Vehicle)) || //IsVehicle(iter->ObjectHandle)) || // Vehicle
			(((NewCategoryMask & 2) == 2) && (iter->CategoryID == Building)) || //IsBuilding(iter->ObjectHandle)) || // Building
			(((NewCategoryMask & 4) == 4) && (iter->CategoryID == Person)) || //IsPerson(iter->ObjectHandle)) || // Pilot
			(((NewCategoryMask & 8) == 8) && (iter->CategoryID == Powerup)) || //IsPowerup(iter->ObjectHandle)) || // Powerup
			(((NewCategoryMask & 16) == 16) && (iter->CategoryID == Weapon)) || //IsWeapon(iter->ObjectHandle)) || // Weapon
			(((NewCategoryMask & 32) == 32) && (iter->CategoryID == Scrap)) || //IsScrap(iter->ObjectHandle)) || // Scrap
			(((NewCategoryMask & 64) == 64) && (iter->CategoryID == Other)))) || //IsMisc(iter->ObjectHandle) && (!IsScrap(iter->ObjectHandle))))) || // Misc (Not Scrap)
			(
			((NewMatchMask & 2) == 2) && // Look for a specific Classlabel.
			(ObjClass) && (iter->ObjClassHash == ObjClassHash) //(_stricmp(TempItemClass,ObjClass) == 0)
			) || 
			(
			((NewMatchMask & 4) == 4) && // Look for a specific ODFName.
			(ODFName) && (iter->ODFNameHash == ODFNameHash) //(_stricmp(TempODFName,ODFName) == 0)
			))
			)
		{
			float DistSquared = GetDistance3DSquared(iter->Position, Position);
			if (BestDistSquared > DistSquared) // It is closer then our last found Handle.
			{
				BestDistSquared = DistSquared;
				TempH = iter->ObjectHandle;
			}
		}
	}

	return TempH;
}
// Inspired by TRO function, gets the nearest object to a path point, with filtering options.
Handle BZ1Helper::GetNearestObject(const Name Path, const int Point, const float MaxRange, int Team, const int TeamFilter, const int CategoryMask, const bool IgnoreInvincible, const int MatchMask, const char *ObjClass, const char *ODFName)
{
	if(!DoesPathExist(Path))
		return 0;

	return GetNearestObject(GetVectorFromPath(Path, Point), MaxRange, Team, TeamFilter, CategoryMask, IgnoreInvincible, 0, MatchMask, ObjClass, ODFName);
}
// Version that takes a Handle.
Handle BZ1Helper::GetNearestObject(const Handle h, const float MaxRange, const int Team, const int TeamFilter, const int CategoryMask, const bool IgnoreInvincible, const int MatchMask, const char *ObjClass, const char *ODFName)
{
	if(!IsAround(h))
		return 0;

	return GetNearestObject(GetPosition(h), MaxRange, Team, TeamFilter, CategoryMask, IgnoreInvincible, h, MatchMask, ObjClass, ODFName);
}
// Version that takes a Matrix
Handle BZ1Helper::GetNearestObject(const Matrix Position, const float MaxRange, const int Team, const int TeamFilter, const int CategoryMask, const bool IgnoreInvincible, const int MatchMask, const char *ObjClass, const char *ODFName)
{
	return GetNearestObject(Position.posit, MaxRange, Team, TeamFilter, CategoryMask, IgnoreInvincible, 0, MatchMask, ObjClass, ODFName);
}

// Builds a vehicle, with options.
Handle BZ1Helper::BuildAngleObject(const char *ODF, const int Team, const Matrix &mat, const char *Label, const float Degrees, const int Group, const bool Empty, const bool StartDeploy, const float HeightOffset, const bool SpreadOut, const int AlignToTerrain, const bool AlignUseWater)
{
	Handle h = Label ? GetHandle(Label) : 0;
	if(!IsAround(h))
	{
		if(!ODF[0])
			return 0;

		int TempPilotCost = 0; //Default of 0.
		char TempODF[MAX_ODF_LENGTH] = {0};
		strcpy_s(TempODF, ODF);
		Matrix BuildLoc = mat;
		if(Degrees > 0.0f)
		{
			Vector Front(0.0f, 0.0f, 1.0f);
			Front = HRotateFront(Front, Degrees * DEG_2_RAD);
			BuildLoc = Build_Directinal_Matrix(BuildLoc.posit, Front);
		}

		if((SpreadOut) && (GetTurnCount() > 1) && ((_strnicmp(TempODF+1,"v",1) == 0) || (_strnicmp(TempODF+1,"s",1) == 0))) // It's a vehicle, try to spread out position.
			BuildLoc.posit = GetPositionNear(BuildLoc.posit, 2.0f, 10.0f);

		if(AlignToTerrain) // If not set to 0 (False) by script. e.g. -1 (default) or 1 (True).
		{
			// Read ODF Parameter, that overrides the "Default" setting. Script can override everything.
			bool ODFOverride = false;
			bool UseAlign = true;
			char TempODF2[MAX_ODF_LENGTH] = {0};
			char TempODF3[MAX_ODF_LENGTH] = {0};
			char ClassLabel[MAX_ODF_LENGTH] = {0};
			sprintf_s(TempODF2, "%s.odf", ODF); // Add odf to the end.

			if(OpenODF2(TempODF2))
			{
				if(GetODFString(TempODF2, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, ClassLabel))
				{
					sprintf_s(TempODF3, "%s.odf", ClassLabel); // Add odf to the end.
					OpenODF2(TempODF3);
				}

				if(GetODFBool(TempODF2, TempODF3, "GameObjectClass", "AlignToTerrain", &UseAlign, false))
					ODFOverride = true;
			}

			if(AlignToTerrain < 0) // Auto Determine
			{
				if((!ODFOverride) && (DefaultAlignToTerrain(ODF)))	
					UseAlign = true;
			}
			else // Force on via script command. (If script wanted it off, none of this code section runs)
			{
				UseAlign = true;
			}

			// If the above code checks didn't disable aligntoterrain.
			if(UseAlign)
			{
				Vector ANormal(0, 0, 0);
				float Height = 0.0f;
				TerrainGetHeightAndNormal(BuildLoc.posit, Height, ANormal, AlignUseWater);
				BuildLoc = Build_Orthogonal_Matrix(ANormal, BuildLoc.front, BuildLoc.posit);
			}
		}

		BuildLoc.posit.y += HeightOffset;

		GetWorldVariantODF(TempODF);

		if(DoesODFExist(TempODF))
		{
			TempPilotCost = GetPilotCost(TempODF);
			if((!Empty) && (GetTurnCount() > 1))
				AddPilot(Team,TempPilotCost);
			h = BuildObject(TempODF, Team, BuildLoc);
		}
		else
		{
			FormatLogMessage("ERROR: ODF %s doesn't exist, Fix DLLs!", TempODF);
			return 0;
		}

		if(h != 0)
		{
			if(_strnicmp(TempODF+1,"v",1) == 0) // It's a vehicle, set it's group and rotate it.
			{
				if(Group < 0)
					SetBestGroup(h);
				if(Group > 0)
					SetGroup(h,Group-1);
			}
			else if(_strnicmp(TempODF+1, "s", 1) == 0) // This is a pilot. By Default, don't add it to PilotDispatch code.
			{
				SetPilotIndependence(h, 0);
			}

			// Special for class Turret. They snap to center of terrain square, in cases of SP missions this is not desirable.
			if(IsGunTower(h))
			{
				bool Snappy = false;
				GetODFBool(h, "TurretCraftClass", "DLLSnapToPosition", &Snappy, false);
				if(Snappy)
					SetPosition(h, BuildLoc);
			}

			if(StartDeploy)
				Deploy(h);

			if(Empty)
				RemovePilotAI(h);

			if(Label && Label[0])
				SetLabel(h, Label);
		}
	}
	return h;
}
Handle BZ1Helper::BuildAngleObject(const char *ODF, const int Team, const Handle him, const char *Label, const float Degrees, const int Group, const bool Empty, const bool StartDeploy, const float HeightOffset, const bool SreadOut, const int AlignToTerrain, const bool AlignUseWater)
{
	Handle h = Label ? GetHandle(Label) : 0;
	if(!IsAround(h))
	{
		if(!IsAround(him))
		{
			FormatLogMessage("ERROR: BuildObject Failed, Handle missing, cannot build ODF %s", ODF); 
			return 0;
		}
		Matrix BuildLoc = Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0)); // Initialize default Matrix.
		// Special case for Portals.
		if(IsPortal(him))
			BuildLoc = GetODFPositionOffset(him, 0);
		else
			GetPosition(him, BuildLoc);

		h = BuildAngleObject(ODF, Team, BuildLoc, Label, Degrees, Group, Empty, StartDeploy, HeightOffset, SreadOut, AlignToTerrain, AlignUseWater);
	}
	return h;
}
Handle BZ1Helper::BuildAngleObject(const char *ODF, const int Team, const Name APath, const char *Label, const float Degrees, const int Group, const bool Empty, const bool StartDeploy, const float HeightOffset, const bool SpreadOut, const int AlignToTerrain, const bool AlignUseWater)
{
	Handle h = Label ? GetHandle(Label) : 0;
	if(!IsAround(h))
	{
		if(!DoesPathExist(APath))
		{
			FormatLogMessage("ERROR: Pathpoint %s missing, cannot build ODF %s", APath, ODF); 
			return 0;
		}
		Matrix BuildLoc = Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), GetVectorFromPath(APath)); // Initialize default Matrix.

		h = BuildAngleObject(ODF, Team, BuildLoc, Label, Degrees, Group, Empty, StartDeploy, HeightOffset, SpreadOut, AlignToTerrain, AlignUseWater);
	}
	return h;
}
Handle BZ1Helper::BuildAngleObject(const char *ODF, const int Team, const AiPath *APath, const char *Label, const float Degrees, const int Group, const bool Empty, const bool StartDeploy, const float HeightOffset, const bool SpreadOut, const int AlignToTerrain, const bool AlignUseWater)
{
	Handle h = Label ? GetHandle(Label) : 0;
	if(!IsAround(h))
	{
		Handle TempH = BuildObject("abstor",0,APath);
		if(!IsAround(TempH))
			return 0;
		Matrix BuildLoc = GetMatrixPosition(TempH);
		RemoveObject(TempH);

		h = BuildAngleObject(ODF, Team, BuildLoc, Label, Degrees, Group, Empty, StartDeploy, HeightOffset, SpreadOut, AlignToTerrain, AlignUseWater);
	}
	return h;
}
Handle BZ1Helper::BuildAngleObject(const char *ODF, const int Team, const Vector &pos, const char *Label, const float Degrees, const int Group, const bool Empty, const bool StartDeploy, const float HeightOffset, const bool SpreadOut, const int AlignToTerrain, const bool AlignUseWater)
{
	Handle h = Label ? GetHandle(Label) : 0;
	if(!IsAround(h))
	{
		Matrix BuildLoc = Matrix(Vector(1, 0, 0),Vector(0, 1, 0), Vector(0, 0, 1), pos); // Initialize default Matrix.

		h = BuildAngleObject(ODF, Team, BuildLoc, Label, Degrees, Group, Empty, StartDeploy, HeightOffset, SpreadOut, AlignToTerrain, AlignUseWater);
	}
	return h;
}

// Sets the CPU Plan based on var options. Includes Race Team# and World Variant.
void BZ1Helper::SetCPUPlan(const char *AIPBaseName, const char Race, const int Team)
{
	if((Team < 0) || (Team > MAX_TEAMS))
		return; // ByeBye.

	char AIPBaseFile[256] = {0};
	strcpy_s(AIPBaseFile, AIPBaseName);

	// Use the default BZC naming scheme if not specified.
	if((AIPBaseFile == NULL) || (AIPBaseFile[0] == '\0'))
		strcpy_s(AIPBaseFile, "bzcnorm_");

	char AIPFile[256] = {0};

//	char WorldSetting = 0;
//	GetODFChar(GetMapTRNFilename(), "World", "WorldLetter", &WorldSetting); // Grab the world letter.

	if(WorldSetting)
		sprintf_s(AIPFile, "%s%c%d%c.aip", AIPBaseFile, Race, Team, WorldSetting); // Removed the Herobine element. -GBD

	if((!WorldSetting) || (!DoesFileExist(AIPFile)))
		sprintf_s(AIPFile, "%s%c%d.aip", AIPBaseFile, Race, Team); // Try without the World Setting.

	// Make the Team Number optional.
	if(!DoesFileExist(AIPFile))
	{
		if(WorldSetting)
			sprintf_s(AIPFile, "%s%c%c.aip", AIPBaseFile, Race, WorldSetting); // Try without the Team number.

		if((!WorldSetting) || (!DoesFileExist(AIPFile)))
			sprintf_s(AIPFile, "%s%c.aip", AIPBaseFile, Race); // Try without the World Setting.
	}

	SetAIPFile(AIPFile, Team);
}

// Attempts to play an AudioMessage, and saves it as the last one played.
int BZ1Helper::MissionAudioMessage(const char* msg)
{
	if(!msg)
		return 0;

	strcpy_s((char *)MissionAudioFile, sizeof(MissionAudioFile), msg);
	MissionAudioHandle = AudioMessage(msg);
	return MissionAudioHandle;
}

// Repeats the last message played via MissionAudioMessage.
int BZ1Helper::RepeatAudioMessage(void)
{
	if(MissionAudioFile[0])
	{ 
		MissionAudioHandle = AudioMessage((char*)(&MissionAudioFile[0]));
		return MissionAudioHandle;
	}
	else
	{ 
		return 0;
	}
}
/*
// Mission Audio Message.
int BZ1Helper::MissionAudioMessage(const char* msg)
{
	if(DoesFileExist(msg))
	{
		MissionAudioList.resize(MissionAudioList.size()+1);
		MissionAudioClass &sao = MissionAudioList[MissionAudioList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(MissionAudioClass)); // Zero things out at start.

		strcpy_s((char *)sao.MissionAudioFile, sizeof(sao.MissionAudioFile), msg);
		sao.MissionAudioHandle = AudioMessage(msg);
		return sao.MissionAudioHandle;
	}
	else
	{
		FormatConsoleMessage("ERROR: Mission Audio Message %s not found! Fix DLL's!", msg);
		return 0;
	}
}

// Repeats the last audio message.
int BZ1Helper::RepeatAudioMessage(void)
{
	if(!MissionAudioList.empty())
		return AudioMessage((char *)MissionAudioList.back().MissionAudioFile); //AudioMessage((char*)(&MissionAudioList[Size-1].MissionAudioFile[0]));
	else
		return 0;
}
*/

// Replaces an object with another object, retaining as much information from the original as desired. 
Handle BZ1Helper::ReplaceObject(const Handle H, const char *ODF, const int Team, const float HeightOffset, const int Empty, const bool RestoreWeapons, const int Group, const bool KeepCommand, const int NewCommand, const Handle NewWho)
{
	if(!IsAround(H))
		return 0;

	const char *ODFName = GetODFName(H); // Get the ODF.
	char NewODF[MAX_ODF_LENGTH] = {0};
	if(!ODF)
		strcpy_s(NewODF, ODFName);
	else
		strcpy_s(NewODF, ODF);

	int TempPilotCost = 0; //Default of 0.
	bool IsEmpty = false;
	Handle h = 0;
	Matrix BuildLoc = GetMatrixPosition(H);
	Vector Velocity = GetVelocity(H);
	float HealthP = GetHealth(H);
	float AmmoP = GetAmmo(H);
	bool WasPlayer = IsPlayer(H);
	int Who = GetCurrentWho(H);
	//Handle Target = GetTarget(H);
	AiCommand Command = GetCurrentCommand(H);
	int Scrap = GetScavengerCurScrap(H);
	const int NewTeam = Team >= 0 ? Team : GetTeamNum(H);
	bool Deployed = IsDeployed(H);
	const char* Label = GetLabel(H);
	int PercievedTeam = GetPerceivedTeam(H);
	float LifeSpan = GetRemainingLifespan(H);

	int Independence = 0;
	int Skill = 0;
	char ObjectiveName[MAX_ODF_LENGTH] = {0};
	long WeaponMask = 0;
	int NewGroup = Group;
	if(m_GameVersion >= v13PB7)
	{
		Independence = GetIndependence(H);
		Skill = GetSkill(H);
		strcpy_s(ObjectiveName, GetObjectiveName(H));
		if(NewGroup < 0)
			NewGroup = GetGroup(H)+1;
		WeaponMask = GetWeaponMask(H);
	}

	char PilotODF[MAX_ODF_LENGTH] = {0};
	//strcpy_s(PilotODF,GetPilotClass(H)); // Dangerous, GetPilotClass(H) can return NULL if the object is empty.
	//if (char *dot = strrchr(PilotODF, '.')) *dot = '\0';
	if (const char *pilotOdf = GetPilotClass(H))
	{
		strcpy_s(PilotODF, pilotOdf);
		if (char *dot = strrchr(PilotODF, '.'))
			*dot = '\0';
	}

	if(Empty == -1)
	{
		if((IsAround(H)) && (!IsNotDeadAndPilot2(H)) && (!IsPlayer(H)))
			IsEmpty = true;
	}
	else
	{
		IsEmpty = Empty;
	}
	
	char Weapon[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	float WeaponLocalAmmo[MAX_HARDPOINTS] = {0};

	for(int i = 0; i < MAX_HARDPOINTS; i++)
	{
		GetObjInfo(H, ObjectInfoType(Get_Weapon0Config+i), Weapon[i]);
		WeaponLocalAmmo[i] = GetCurLocalAmmo(H, i);
	}

	GetWorldVariantODF(NewODF);

	if(HeightOffset != 0)
		BuildLoc.posit.y += HeightOffset;

	if(DoesODFExist(NewODF))
	{
		TempPilotCost = GetPilotCost(NewODF);

		if((!IsEmpty) && (m_TurnCounter > 1))
			AddPilot(NewTeam, TempPilotCost);

		h = BuildObject(NewODF, NewTeam, BuildLoc);
		RemoveObject(H);

		if(NewGroup < 0)
			SetBestGroup(h);
		else if(NewGroup > 0)
			SetGroup(h, NewGroup-1);
	}
	else
	{
		FormatLogMessage("ERROR: ReplaceODF %s doesn't exist! Fix DLLs!", NewODF);
		return H; // Report back the same handle, didn't work.
	}

	if(WasPlayer)
	{
		SetAsUser(h,NewTeam);
	}
	else
	{
		if(NewCommand >= 0)
			SetCommand(h, NewCommand, 0, NewWho);
		else if(KeepCommand)
			SetCommand(h, Command, 0, Who);
	}

	if(Deployed)
		Deploy(h);

	SetLabel(h, Label);

	if((!WasPlayer) && (IsEmpty))
		RemovePilotAI(h);

	SetHealth(h, HealthP);
	SetAmmo(h, AmmoP);
	SetVelocity(h, Velocity);
	SetPilotClass(h, PilotODF);
	SetScavengerCurScrap(h, Scrap);
	SetPerceivedTeam(h, PercievedTeam);
	SetLifespan(h, LifeSpan);

	if(m_GameVersion >= v13PB7)
	{
		SetIndependence(h, Independence);
		SetSkill(h, Skill);
		SetObjectiveName(h, ObjectiveName);
		SetWeaponMask(h, WeaponMask);
	}

	if(RestoreWeapons)
		ReplaceWeapons(h, Weapon, WeaponLocalAmmo, ODFName);

	return h;
}

// Save info on all the map objects to a file for later reading/replacing.
void BZ1Helper::SaveMapObjectsToFile(const char *file)
{
	FormatLogMessage("Saving map objects to text file.");

	char FileName[MAX_ODF_LENGTH] = {0};
	
	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if (char *dot = strrchr(mapTrnFile, '.'))
		*dot = '\0';

	// If there is no specified file, use maptrn name base.
	if(!file)
		sprintf_s(FileName, "%s_ObjectLoad.txt", mapTrnFile);
	else
		strcpy_s(FileName, file);

	// Test writing a file...
	wchar_t* pRootOutputDir = NULL;
	size_t bufSize = 0;
	GetOutputPath(bufSize, pRootOutputDir);
	wchar_t *pData = static_cast<wchar_t *>(alloca(bufSize*sizeof(wchar_t)));
	if(GetOutputPath(bufSize, pData))
	{
		wchar_t outPath[MAX_MESSAGE_LENGTH] = {0};
		swprintf_s(outPath, L"%s/BZCaddon/%S", pData, FileName);
		FILE* pFile = _wfsopen(outPath, L"w", _SH_DENYWR);
		if(pFile)
		{
			// Loop over all objects currently on the map, and save their info.
			for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
			{
				Handle h = iter->ObjectHandle;
				const char *ODFName = GetODFName(h);
				int Team = GetTeamNum(h);
				const char *Label = GetLabel(h);
				long MaxHealth = GetMaxHealth(h);
				long CurHealth = GetCurHealth(h);
				long MaxAmmo = GetMaxAmmo(h);
				long CurAmmo = GetCurAmmo(h);
				int ScavScrap = GetScavengerCurScrap(h);
				Vector Front = GetFront(h);
				Vector Position = GetPosition(h);
				Vector Velocity = GetVelocity(h);
				bool Deployed = IsDeployed(h);
				char PilotODF[MAX_ODF_LENGTH] = {0};
				if (const char *pilotOdf = GetPilotClass(h))
				{
					strcpy_s(PilotODF, pilotOdf);
					if (char *dot = strrchr(PilotODF, '.'))
						*dot = '\0';
				}

				float LifeSpan = GetRemainingLifespan(h);
				int Independence = 0;
				int Skill = 0;
				char ObjectiveName[MAX_ODF_LENGTH] = {0};
				long WeaponMask = 0;
				int Group = 0;
				if(m_GameVersion >= v13PB7)
				{
					Independence = GetIndependence(h);
					Skill = GetSkill(h);
					strcpy_s(ObjectiveName, GetObjectiveName(h));
					Group = GetGroup(h);
					WeaponMask = GetWeaponMask(h);
				}

				fprintf(pFile, "[Object%d]\n", iter-HandleList.begin()+1);
				fprintf(pFile, "ODFName = \"%s\"\n", ODFName);
				fprintf(pFile, "Team = %d\n", Team);
				fprintf(pFile, "Label = \"%s\"\n", Label);
				fprintf(pFile, "MaxHealth = %d\n", MaxHealth);
				fprintf(pFile, "CurHealth = %d\n", CurHealth);
				fprintf(pFile, "MaxAmmo = %d\n", MaxAmmo);
				fprintf(pFile, "CurAmmo = %d\n", CurAmmo);
				fprintf(pFile, "ScavScrap = %d\n", ScavScrap);
				fprintf(pFile, "FrontVector = \"%f %f %f\"\n", Front.x, Front.y, Front.z);
				fprintf(pFile, "Position = \"%f %f %f\"\n", Position.x, Position.y, Position.z);
				fprintf(pFile, "Velocity = \"%f %f %f\"\n", Velocity.x, Velocity.y, Velocity.z);
				fprintf(pFile, "PilotODF = \"%s\"\n", PilotODF);
				fprintf(pFile, "Deployed = %d\n", Deployed);
				fprintf(pFile, "LifeSpan = %f\n", LifeSpan);
				fprintf(pFile, "Independence = %d\n", Independence);
				fprintf(pFile, "Skill = %d\n", Skill);
				fprintf(pFile, "WeaponMask = %d\n", WeaponMask);
				fprintf(pFile, "Group = %d\n", Group);
				fprintf(pFile, "ObjectiveName = %s\n", ObjectiveName);
				fprintf(pFile, "\n");
			}
			fclose(pFile);
		}
	}
}

// This function loads objects from a specially formatted text file onto the map.
void BZ1Helper::LoadMapObjectsFromFile(const char *file)
{
	FormatLogMessage("Loading map objects from text file.");

	char FileName[MAX_ODF_LENGTH] = {0};
	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if (char *dot = strrchr(mapTrnFile, '.'))
		*dot = '\0';

	// If there is no specified file, use maptrn name base.
	if(!file)
		sprintf_s(FileName, "%s_ObjectLoad.text", mapTrnFile);
	else
		strcpy_s(FileName, file);

	if(OpenODF2(FileName))
	{
		char TempVal[MAX_ODF_LENGTH] = {0};

		char ODFName[MAX_ODF_LENGTH] = {0};
		int Team = 0;
		char Label[MAX_ODF_LENGTH] = {0};
		long MaxHealth = 0;
		long CurHealth = 0;
		long MaxAmmo = 0;
		long CurAmmo = 0;
		int ScavScrap = 0;
		Vector Front(0, 0, 0);
		Vector Position(0, 0, 0);
		Vector Velocity(0, 0, 0);
		char PilotODF[MAX_ODF_LENGTH] = {0};
		bool Deployed = 0;
		float LifeSpan = -1.0f;
		int Independence = 0;
		int Skill = 0;
		char ObjectiveName[MAX_ODF_LENGTH] = {0};
		long WeaponMask = 0;
		int Group = 0;

		// Loop over all possible objects on the map. BZ2 has max "ENTITIES" of 4096.
		for(int i = 0; i < 4096; i++)
		{
			bool Continue = true;
			sprintf_s(TempVal, "[Object%d]", i+1);
			if(GetODFString(FileName, TempVal, "ODFName", MAX_ODF_LENGTH, ODFName))
			{
				GetODFInt(FileName, TempVal, "Team", &Team);
				GetODFString(FileName, TempVal, "Label", MAX_ODF_LENGTH, Label);
				GetODFLong(FileName, TempVal, "MaxHealth", &MaxHealth);
				GetODFLong(FileName, TempVal, "CurHealth", &CurHealth);
				GetODFLong(FileName, TempVal, "MaxAmmo", &MaxAmmo);
				GetODFLong(FileName, TempVal, "CurAmmo", &CurAmmo);
				GetODFInt(FileName, TempVal, "ScavScrap", &ScavScrap);
				GetODFVector(FileName, TempVal, "Front", &Front);
				GetODFVector(FileName, TempVal, "Position", &Position);
				GetODFVector(FileName, TempVal, "Velocity", &Velocity);
				GetODFString(FileName, TempVal, "PilotODF", MAX_ODF_LENGTH, PilotODF);
				GetODFBool(FileName, TempVal, "Deployed", &Deployed);

				GetODFFloat(FileName, TempVal, "LifeSpan", &LifeSpan);
				GetODFInt(FileName, TempVal, "Independence", &Independence);
				GetODFInt(FileName, TempVal, "Skill", &Skill);
				GetODFLong(FileName, TempVal, "WeaponMask", &WeaponMask);
				GetODFInt(FileName, TempVal, "Group", &Group);
				GetODFString(FileName, TempVal, "ObjectiveName", MAX_ODF_LENGTH, ObjectiveName);

				// Loop over all objects currently on the map, and make sure none of them match this.
				for (std::vector<ObjectClass>::iterator iter = HandleList.begin(); iter != HandleList.end(); ++iter)
				{
					Handle h = iter->ObjectHandle;
					Vector TestPosition = GetPosition(h);
					const char *TestLabel = GetLabel(h);
					const char *TestODFName = GetODFName(h);

					if((close_enough(TestPosition, Position, 0.01f)) && (TestODFName) && (_stricmp(TestODFName, ODFName) == 0) && (TestLabel) && (_stricmp(TestLabel, Label) == 0))
					{
						Continue = false;
						break; // This object is already around, skip it.
					}
				}
				if(!Continue)
					continue; // Skip this object, it's already on the map.

				Matrix TruePos = Build_Directinal_Matrix(Position, Front);
				Handle H = BuildObject(ODFName, Team, TruePos);
				if(IsAround(H))
				{
					FormatLogMessage("Building Object: %s, Team: %d, Position: %f %f %f, Label: %s, MaxHealth: %d, CurHealth: %d, MaxAmmo: %d, CurAmmo: %d, SavedScrap: %d, Pilot: %s, Deployed: %d, Velocity: %f %f %f, LifeSpan: %f, Independence: %d, Skill: %d, WeaponMask: %d, Group: %d, ObjectiveName: %s", ODFName, Team, Position.x, Position.y, Position.z, Label, MaxHealth, CurHealth, MaxAmmo, CurAmmo, ScavScrap, PilotODF, Deployed, Velocity.x, Velocity.y, Velocity.z, LifeSpan, Independence, Skill, WeaponMask, Group, ObjectiveName);

					SetVelocity(H, Velocity);
					SetLabel(H, Label);
					SetMaxHealth(H, MaxHealth);
					SetCurHealth(H, CurHealth);
					SetMaxAmmo(H, MaxAmmo);
					SetCurAmmo(H, CurAmmo);
					if(ScavScrap)
						SetScavengerCurScrap(H, ScavScrap);
					SetPilotClass(H, PilotODF);
					if(Deployed)
						Deploy(H);

					SetLifespan(H, LifeSpan);
					SetGroup(H, Group);

					if(m_GameVersion >= v13PB7)
					{
						SetIndependence(H, Independence);
						SetSkill(H, Skill);
						SetObjectiveName(H, ObjectiveName);
						SetWeaponMask(H, WeaponMask);
					}
				}
			}
		//	else
		//	{
		//		break; // We hit the end of our objects, skip out.
		//	}
		}
	}
}
