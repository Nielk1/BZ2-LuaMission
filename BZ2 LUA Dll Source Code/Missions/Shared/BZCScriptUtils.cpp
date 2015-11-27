#include "BZCScriptUtils.h"
#include <math.h>
#include <memory.h>
#include <malloc.h>
#include <stdarg.h>
#include <share.h>
#include <time.h>
#include <ctype.h>
#include <shlobj.h>

//#include <windows.h>
//#include <iostream>
//#include <shlobj.h>

// Static variable for things. NOT saved. //!!-- Remove this and use char pass-in buffers, i.e. change const char * Blah() to Blah(char *Value, ...). -GBD
//char TempODFName[MAX_ODF_LENGTH];

// Temporary strings for blasting stuff into for output. NOT to be used to store anything you care about.
//char TempMsgString[MAX_MESSAGE_LENGTH];

// This stores a list of all currently open ODFs for this session.
stdext::hash_map<unsigned long, ODFName> ODFNameMap;

stdext::hash_map<const char*, FILE*> FileNameMap;
// This stores the filename of the Log file for this game.
FILE* LogFile;

// List all AI Commands. !!! Update based on aiCommands.h should it ever change! NOTE: # of entries must == NUM_CMD.
const char *CommandList[NUM_CMD] = { "CMD_NONE", "CMD_SELECT", "CMD_STOP", "CMD_GO", "CMD_ATTACK", "CMD_FOLLOW", "CMD_FORMATION", "CMD_PICKUP",
		"CMD_DROPOFF", "CMD_UNDEPLOY", "CMD_DEPLOY", "CMD_NO_DEPLOY", "CMD_GET_REPAIR", "CMD_GET_RELOAD", "CMD_GET_WEAPON", "CMD_GET_CAMERA", "CMD_GET_BOMB", 
		"CMD_DEFEND", "CMD_RESCUE", "CMD_RECYCLE", "CMD_SCAVENGE", "CMD_HUNT", "CMD_BUILD", "CMD_PATROL", "CMD_STAGE", "CMD_SEND", "CMD_GET_IN", 
		"CMD_LAY_MINES", "CMD_LOOK_AT", "CMD_SERVICE", "CMD_UPGRADE", "CMD_DEMOLISH", "CMD_POWER", "CMD_BACK", "CMD_DONE", "CMD_CANCEL", "CMD_SET_GROUP", 
		"CMD_SET_TEAM", "CMD_SEND_GROUP", "CMD_TARGET", "CMD_INSPECT", "CMD_SWITCHTEAM", "CMD_INTERFACE", "CMD_LOGOFF", "CMD_AUTOPILOT", "CMD_MESSAGE", 
		"CMD_CLOSE", "CMD_MORPH_SETDEPLOYED", "CMD_MORPH_SETUNDEPLOYED", "CMD_MORPH_UNLOCK", "CMD_BAILOUT", "CMD_BUILD_ROTATE", "CMD_CMDPANEL_SELECT", 
		"CMD_CMDPANEL_DESELECT" };

// List all Bilding Classes, based on ScriptUtils return values.
const Hash_GOClass BuildingClass[] = { CLASS_POWERED, CLASS_TURRET, CLASS_SILO, CLASS_SUPPLYDEPOT, CLASS_BARRACKS, CLASS_BUILDING, 
		CLASS_FACTORY, CLASS_ARMORY, CLASS_RECYCLER, CLASS_SHIELDTOWER, CLASS_EXTRACTOR, CLASS_COMMBUNKER, CLASS_COMMTOWER, CLASS_JAMMER, 
		CLASS_MOTIONSENSOR, CLASS_BOMBERBAY, CLASS_TELEPORTAL, CLASS_TERRAIN, CLASS_I76BUILDING };

// List all Vehicle Classes, based on ScriptUtils return values.
const Hash_GOClass VehicleClass[] = { CLASS_WINGMAN, CLASS_SCAVENGERH, CLASS_TURRETTANK, CLASS_MORPHTANK, CLASS_MINELAYER, CLASS_HOWITZER, CLASS_WALKER, 
		CLASS_SAV, CLASS_TUG, CLASS_CONSTRUCTIONRIG, CLASS_ASSAULTHOVER, CLASS_SCAVENGER, CLASS_APC, CLASS_ASSAULTTANK, CLASS_ARTILLERY, 
		CLASS_AIR, CLASS_SERVICEH, CLASS_SERVICE, CLASS_RECYCLERVEHICLE, CLASS_BOMBER, CLASS_COMMVEHICLE, CLASS_DEPLOYABLE, CLASS_TRACKEDVEHICLE,
		CLASS_DEPLOYBUILDING, CLASS_TRACKEDDEPLOYABLE, CLASS_HOVER };

// Powerups...
const Hash_GOClass PowerupClass[] = { CLASS_POWERUP_WEAPON, CLASS_POWERUP_REPAIR, CLASS_POWERUP_RELOAD, CLASS_POWERUP_CAMERA, CLASS_DAYWRECKER, 
		CLASS_POWERUP_SERVICE, CLASS_POWERUP_MONEY };

// Weapons (Only physical objects, DLL can't see ordnance yet. :( )
const Hash_GOClass WeaponClass[] = { CLASS_MINE_PROXIMITY, CLASS_MINE_MAGNET, CLASS_MINE_WEAPON, CLASS_MINE_TRIP, CLASS_FLAREMINE, 
		CLASS_SPRAYBOMB, CLASS_SPRAYBUILDING, CLASS_TORPEDO, CLASS_SEEKER, CLASS_SATCHELCHARGE }; //, CLASS_MINE };

// Misc items, usually the unimportant stuff, on team 0.
const Hash_GOClass MiscClass[] = { CLASS_SCRAP, CLASS_DEPOSIT, CLASS_NAVBEACON, CLASS_OBJECTSPAWN, CLASS_SIGN, CLASS_SPAWNBUOY, CLASS_ARTIFACT, 
		CLASS_FLAG, CLASS_KINGOFHILL, CLASS_PLANT, CLASS_BOID };

// List of Deafult Classlabel = "" strings that align to terrain by "default".
const char *DefaultAlignToTerrainList[] = { "scrap", "powered", "turret", "silo", "supplydepot", "barracks", "deposit", "factory", "armory", "recycler", 
		"shieldtower", "extractor", "commbunker", "commtower", "powerplant", "powerlung", "jammer", "sensor", "bomberbay", "teleportal", "terrain", 
		"i76building", "i76sign", "proximity", "magnet", "weaponmine", "tripmine", "flare", "spraybomb", "spraybuilding", "torpedo", "seeker", 
		"satchel" };

// List of ODF names, in reverse load order, for smooth loading. Race Letter is added on to the front.
const char *PreloadRaceList[] = { "spilo", "ssold", "suser", "vapc", "vartl", "vfigh", "vhaul", "vhraz", "vhtnk", "vltnk", "vmine", "vrckt", "vrdev", 
		"vsav", "vscav", "vtank", "vturr", "vwalk", "vatank", "vmbike", "vmisl", "vsent", "varch", // Units.
		"bmuf", "vmuf", // Factory.
		"bbarr", "bcafe", "bcomm", "bhang", "bhqcp", "blpad", "blpow", "bmbld", "bshld", "bsilo", "bspow", "bsupp", "btowe", "bwpow", 
		"barmo", "bsbay", "bbomb", "btrain", "bgtow", "bspir", // Buildings.
		"vcnst", // Constructor
		"bslf", "vslf", "brecy", "vrecy", }; // SLF/Recy.

// Common ODF Lists.
const char *OffensiveShipList[] = { "avfigh", "avltnk", "avtank", "avrckt", "avhraz", "avwalk", "avhtnk", "avapc" };
const char *DefensiveShipList[] = { "avartl", "avmine", "avturr" };
const char *UtilityShipList[] = { "avscav", "avhaul", "avserv" };

// Things moved from DLLUtils.

// Returns true if h is a recycler or recycler vehicle, false if h is invalid, or not a recycler
bool IsRecycler(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_RECYCLERVEHICLE") == 0) || (_stricmp(ObjClass, "CLASS_RECYCLER") == 0) || (_stricmp(BZCClass, "recy") == 0); // Look for ScavH's too. -GBD
}

// Given a team #, counts the number of allied human players currently playing. (i.e. not the number of possible allies, but the number of
// actual allies) Note: should return at least 1, because each team is an ally of itself. Does not count the neutral team (0), which is an
// ally of all teams by default.
int CountAlliedPlayers(const int Team)
{
	int count = 0;

	for(int i = 1; i < MAX_TEAMS; ++i)
	{
		if((IsTeamAllied(i, Team)) && (GetPlayerHandle(i)))
			++count; // team is allied
	}

	return count;
}

// Sanity wrapper for GetVarItemStr. Reads the specified svar, and verifies it's present in the specified list. If not found in there, returns NULL.
const char* GetCheckedNetworkSvar(const size_t svar, const NETWORK_LIST_TYPE listType)
{
	char svarStr[128] = {0};
	sprintf_s(svarStr, "network.session.svar%d", svar);

	const char* pContents = GetVarItemStr(svarStr);
	// Error finding? Bail, asap
	if(pContents == NULL)
		return NULL;

	size_t count = GetNetworkListCount(listType);
	for(size_t i = 0; i < count; ++i)
	{
		const char* pItem = GetNetworkListItem(listType, i);
		if(pItem == NULL)
			continue;

		if(_stricmp(pContents, pItem) == 0) // Got a good match. Return it
			return pContents;
	}

	return NULL; // No match found. Report error
}

/*
// Gets the initial player vehicle as selected in the shell
char *GetInitialPlayerODF(int Team)
{
	strcpy_s(TempODFName, GetPlayerODF(Team));
	GetWorldVariantODF(TempODFName);

	return TempODFName;
}
*/

// Gets the next vehicle ODF for the player on a given team.
char TempNextVehicleODF[MAX_ODF_LENGTH];
const char *GetNextVehicleODF(const int Team, const bool Randomize, const RandomizeType DefaultRType)
{
	memset(TempNextVehicleODF, 0, sizeof(TempNextVehicleODF)); // Clear this before use.

	int MissionTypePrefs = GetVarItemInt("network.session.ivar7");
	int m_RespawnType = (MissionTypePrefs>>8) &0xFF;

	RandomizeType RType = DefaultRType; // Default
	if(Randomize)
	{
		if(m_RespawnType == 1)
			RType = Randomize_ByRace;
		else if(m_RespawnType == 2)
			RType = Randomize_Any;
	}

	// World Letter now applied via DLL. -GBD
	strcpy_s(TempNextVehicleODF, GetPlayerODF(Team, RType));

	// May not keep this, BZC Specific bit. BZC's ISDF Scout is xvfigh.odf, so it should be safe for now.
	/* // I've never seen this fail, so disabling for moddability sake.
	// If the GetPlayerODF Failed and returned default, override the Default.
	if(_stricmp(TempNextVehicleODF, "ivscout") == 0)
	{
		FormatLogMessage("ERROR: GetPlayerODF Failed to return a valid thing! Overriding Default of ivscout!");
		strcpy_s(TempNextVehicleODF, "avfigh");
	}
	*/

	GetWorldVariantODF(TempNextVehicleODF);

	return TempNextVehicleODF;
}

// Given a race identifier, get the pilot back (used during a respawn)
char TempInitialPilotODFName[MAX_ODF_LENGTH];
const char *GetInitialPlayerPilotODF(const char Race)
{
	memset(TempInitialPilotODFName, 0, sizeof(TempInitialPilotODFName)); // Clear this before use.

	bool m_RespawnWithSniper = (GetVarItemInt("network.session.ivar16") != 0);
	bool m_RespawnWithPack = (GetVarItemInt("network.session.ivar62") != 0);

	if((m_RespawnWithSniper) && (!m_RespawnWithPack))
		strcpy_s(TempInitialPilotODFName, "asuser"); // With sniper, without pack.
	else if((m_RespawnWithSniper) && (m_RespawnWithPack))
		strcpy_s(TempInitialPilotODFName, "asuser_p"); // With sniper, with pack.
	else if((!m_RespawnWithSniper) && (m_RespawnWithPack))
		strcpy_s(TempInitialPilotODFName, "asuser_mp"); // Without sniper, with pack.
	else
		strcpy_s(TempInitialPilotODFName, "asuser_m"); // Without sniper, without pack.

	TempInitialPilotODFName[0] = Race;
	if(!DoesODFExist(TempInitialPilotODFName)) // IF their pilot doesn't exist, use race a. -GBD
		TempInitialPilotODFName[0] = 'a';
	if(!DoesODFExist(TempInitialPilotODFName)) // Still no %csuser_whatever? Use default.
		strcpy_s(TempInitialPilotODFName, "asuser");

	return TempInitialPilotODFName;
}

// Given a race identifier, get the recycler ODF back
char TempInitialRecyclerODFName[MAX_ODF_LENGTH];
const char *GetInitialRecyclerODF(const char Race)
{
	memset(TempInitialRecyclerODFName, 0, sizeof(TempInitialRecyclerODFName)); // Clear this before use.

	char *Contents = 0;
	if(IsNetworkOn())
		Contents = (char *)GetCheckedNetworkSvar(5, NETLIST_Recyclers); //GetVarItemStr("network.session.svar5");
	else
		Contents = (char *)GetVarItemStr("options.instant.string1");

	if((Contents != NULL) && (Contents[0] != '\0'))
		strcpy_s(TempInitialRecyclerODFName, Contents);
	else
		strcpy_s(TempInitialRecyclerODFName, "avrecy_m");

	TempInitialRecyclerODFName[0] = Race;

	// This race doesn't have this Recycler Variant, fall back to default!
	if(!DoesODFExist(TempInitialRecyclerODFName))
		sprintf_s(TempInitialRecyclerODFName, "%cvrecy_m", Race);

	GetWorldVariantODF(TempInitialRecyclerODFName); // Apply world letter. -GBD

	// This race doesn't have this Recycler Variant, Look for *b* building version.
	if(DoesODFExist(TempInitialRecyclerODFName))
		return TempInitialRecyclerODFName; // Exit early, we found it.
	else
		TempInitialRecyclerODFName[1] = 'b';

	if(!DoesODFExist(TempInitialRecyclerODFName))
		sprintf_s(TempInitialRecyclerODFName, "%cbrecy_m", Race);

	return TempInitialRecyclerODFName;
}

// Helper function for SetupTeam(), returns an appropriate spawnpoint.
Vector GetSpawnpointForTeam(const int Team)
{
	Vector spawnpointPosition(0, 0, 0);
	// Friendly spawnpoint: Max distance away for ally
	const float FRIENDLY_SPAWNPOINT_MAX_ALLY = 100.0f;
	// Friendly spawnpoint: Min distance away for enemy
	const float FRIENDLY_SPAWNPOINT_MIN_ENEMY = 400.0f;
	// Random spawnpoint: min distance away for enemy
	const float RANDOM_SPAWNPOINT_MIN_ENEMY = 450.0f;

	// Pick a random, ideally safe spawnpoint.
	SpawnpointInfo* pSpawnPointInfo;
	size_t count = GetAllSpawnpoints(pSpawnPointInfo, Team);

	// Designer didn't seem to put any spawnpoints on the map :(
	if(count == 0)
		return spawnpointPosition;

	// First pass: see if a spawnpoint exists with this team #
	// Note: using a temporary array allocated on stack to keep track of indices.
	size_t* pIndices = reinterpret_cast<size_t*>(_alloca(count * sizeof(size_t)));
	memset(pIndices, 0, count * sizeof(size_t));

	size_t indexCount = 0;
	for(size_t i = 0; i < count; ++i)
	{
		if(pSpawnPointInfo[i].m_Team == Team)
			pIndices[indexCount++] = i;
	}
	// Did we find any spawnpoints in the above search? If so, randomize out of that list and return that
	if(indexCount > 0)
	{
		size_t index = 0;
		// Might be unnecessary, but make sure we return a valid index in [0,indexCount)
		do
			index = GetRandomInt(indexCount); //index = static_cast<size_t>(GetRandomFloat(static_cast<float>(indexCount)));
		while(index >= indexCount);

		return pSpawnPointInfo[pIndices[index]].m_Position;
	}

	// Second pass: build up a list of spawnpoints that appear to have allies close, randomly pick one of those.
	indexCount = 0;
	for(size_t i = 0; i < count; ++i)
	{
		if(((pSpawnPointInfo[i].m_DistanceToClosestSameTeam < FRIENDLY_SPAWNPOINT_MAX_ALLY) ||
			(pSpawnPointInfo[i].m_DistanceToClosestAlly < FRIENDLY_SPAWNPOINT_MAX_ALLY)) &&
		   (pSpawnPointInfo[i].m_DistanceToClosestEnemy >= FRIENDLY_SPAWNPOINT_MIN_ENEMY))
			pIndices[indexCount++] = i;
	}
	// Did we find any spawnpoints in the above search? If so, randomize out of that list and return that
	if(indexCount > 0)
	{
		size_t index = 0;
		// Might be unnecessary, but make sure we return a valid index in [0,indexCount)
		do
			index = GetRandomInt(indexCount); //index = static_cast<size_t>(GetRandomFloat(static_cast<float>(indexCount)));
		while(index >= indexCount);

		return pSpawnPointInfo[pIndices[index]].m_Position;
	}

	// Third pass: Make up a list of spawnpoints that appear to have no enemies close.
	indexCount = 0;
	for(size_t i = 0; i < count; ++i)
	{
		if(pSpawnPointInfo[i].m_DistanceToClosestEnemy >= RANDOM_SPAWNPOINT_MIN_ENEMY)
			pIndices[indexCount++] = i;
	}
	// Did we find any spawnpoints in the above search? If so, randomize out of that list and return that
	if(indexCount > 0)
	{
		size_t index = 0;
		// Might be unnecessary, but make sure we return a valid index in [0,indexCount)
		do
			index = GetRandomInt(indexCount); //index = static_cast<size_t>(GetRandomFloat(static_cast<float>(indexCount)));
		while(index >= indexCount);

		return pSpawnPointInfo[pIndices[index]].m_Position;
	}

	// If here, all spawnpoints have an enemy within RANDOM_SPAWNPOINT_MIN_ENEMY.  Fallback to old code.
	return GetRandomSpawnpoint(Team);
}

// Things from Bob "BS-ser" Stewert's DLL Scriptor.

// Code from Bob "BS-er" Stewart. Slight tweaks by General BlackDragon. (portable_sin/atan2)
Vector HRotateFront(const Vector Front, const float HAngleDifference)
{
	Vector FinalVector(0, 0, 0);
	float Angle = 0.0f;
	float Scale = sqrt(1.0f - Front.y * Front.y);

	// Get the horizontal angle of the initial directional vector.
	Angle = portable_atan2(Front.x, Front.z);

	// Add the rotation amount to the current angle, and convert back into a directional vector.
	Angle += HAngleDifference;
	FinalVector.x = portable_sin(Angle) * Scale;
	FinalVector.y = Front.y;
	FinalVector.z = portable_cos(Angle) * Scale;

	return FinalVector;
}
// Code from Bob "BS-er" Stewart.
void SetAngle(const Handle h, const float Degrees)
{
	if(!IsAround(h))
		return;

	Vector Front = HRotateFront(Vector(0.0f, 0.0f, 1.0f), Degrees * DEG_2_RAD);
	Matrix Position = Build_Directinal_Matrix(GetPosition(h), Front); // GetPosition2?
	SetPosition(h, Position);
	SetVectorPosition(h, Position.posit);
}

// Code originally from Bob "BS-er" Stewart. Re-write by Ken Miller and General BlackDragon.
float CameraTime = -1.0f;
Vector CameraPosition(0, 0, 0);
bool CameraPos(const Handle me, const Handle him, const Vector &PosA, const Vector &PosB, const float Increment)
{
	// Compute the difference vector between the starting and ending positions
	Vector CameraDiffVector = Sub_Vectors(PosB, PosA);
	// Advance the camera position
	float Distance = GetLength3D(CameraDiffVector);

	// Compute the time when the camera shot must end
	if(CameraTime < 0.0f)
	{
		float Time = Distance / (Increment * 0.01f);
		CameraPosition = PosA;
		CameraTime = (GetTime() + Time);
	}
	else
	{
		float Scale = Increment / (Distance * 100 * GetTPS() * 2); // Why is the 2 there? -GBD
		CameraPosition = Add_Mult_Vectors(CameraPosition, CameraDiffVector, Scale); // Increment the Position.
	}

	if((CameraObject(me, CameraPosition.x, CameraPosition.y, CameraPosition.z, him)) || (GetTime() > CameraTime) || (!IsAround(him)))
	{
		CameraTime = -1.0f;
		return true; // Done here.
	}

	return false;
}

Vector CircleCalculation(0, 0, 0);
// Camera Circle function created by General BlackDragon and WhosDr. Circles around an object.
bool CameraCircle(const Handle me, const Vector Offset, const Handle him, const Vector Speed)
{
	// Calculate the radius, the origin is always 0, 0, 0. 
	float Radius = GetLength3D(Offset);
	Vector speed = Speed;

	if(!CameraTime)
	{
		CameraPosition = Offset;
		CameraTime = 1; // Set this here to flag this off.
	}
	else
	{
		speed = Vector(speed.x + speed.y, speed.x + speed.z, speed.y + speed.z);

		CircleCalculation = Add_Mult_Vectors(CircleCalculation, speed, 1.0f / GetTPS());

		CameraPosition.x = (portable_sin(CircleCalculation.x * DEG_2_RAD) * Radius);
		CameraPosition.y = (portable_cos(CircleCalculation.y * DEG_2_RAD) * Radius);
		CameraPosition.z = (portable_cos(CircleCalculation.z * DEG_2_RAD) * Radius);
	}

	if(CameraObject(me, CameraPosition.x, CameraPosition.y, CameraPosition.z, him) || (!IsAround(him)))
	{
		CameraTime = 0;
		return true;
	}

	return false;
}

// New things :)

// Formatted console message.
void FormatConsoleMessage(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char tempstr[MAX_MESSAGE_LENGTH] = {0};
	vsnprintf_s(tempstr, sizeof(tempstr), sizeof(tempstr), format, ap);
	PrintConsoleMessage(tempstr);
	va_end(ap);
}

// Opens a file and remembers the file name for later closure
bool FileOpen(const char *Filename, const bool Append)
{
	// already open
	if(FileNameMap.find(Filename) != FileNameMap.end())
	{
		errno_t Error = _wfreopen_s(&FileNameMap[Filename], NULL, Append ? L"a+" : L"w+", FileNameMap[Filename]);
		if(Error)
		{
			char ErrorMessage[MAX_ODF_LENGTH] = {0};
			strerror_s(ErrorMessage, Error);
			FormatLogMessage(ErrorMessage);
			return false;
		}
		return true;
	}

	// get the output path
	wchar_t* pRootOutputDir = NULL;
	size_t bufSize = 0;
	GetOutputPath(bufSize, pRootOutputDir);
	wchar_t *pData = static_cast<wchar_t *>(alloca(bufSize*sizeof(wchar_t)));
	if(GetOutputPath(bufSize, pData))
	{
		// generate a full path name
		wchar_t outPath[MAX_MESSAGE_LENGTH] = {0};
		wchar_t MyDocumentsPath[MAX_MESSAGE_LENGTH] = {0};
		SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, MyDocumentsPath);

		/*
		if(_wcsnicmp(pData, MyDocumentsPath, wcslen(MyDocumentsPath)) == 0)
			swprintf_s(outPath, L"%slogs/%S", pData, Filename);
		else
			swprintf_s(outPath, L"%sBZCaddon/%S", pData, Filename);
		*/

		//swprintf_s(outPath, (_wcsnicmp(pData, MyDocumentsPath, wcslen(MyDocumentsPath)) == 0) ? L"%slogs/%S" : L"%sBZCaddon/%S", pData, Filename);
		swprintf_s(outPath, L"%s%S", pData, Filename);
		FILE *file = _wfsopen(outPath, Append ? L"a+" : L"w+", _SH_DENYWR); //NULL;

		/*
		if(Append)
			file = _wfsopen(outPath, L"a+", _SH_DENYWR);
		else
			file = _wfsopen(outPath, L"w+", _SH_DENYWR);
		*/

		// try to open the file for append
		if (file)
		{
			// opened successfully
			FileNameMap[Filename] = file;
			return true;
		}
		else
		{
			FormatLogMessage("ERROR: Could not open File: %S", outPath);
		}
	}
	return false;
}

// Formatted Logfile message. Writes a char string to a log file.
bool FormatFileMessage(const char *Filename, const bool Append, const char *format, ...)
{
	if (FileOpen(Filename, Append))
	{
		va_list ap;
		va_start(ap, format);
		FILE *file = FileNameMap[Filename];
		fprintf(file, format, ap);
		fputs("\n", file);
		va_end(ap);
		return true;
	}
	return false;
}

// Closes a specific file.
void FileClose(const char *Filename)
{
	FileMapIt it = FileNameMap.find(Filename);
	if(it != FileNameMap.end())
	{
		fclose(it->second);
		FileNameMap.erase(it);
	}
}

// Close all open files.
void CloseOpenFiles()
{
	FileMapIt it = FileNameMap.begin();
	while(it != FileNameMap.end())
	{
		if (it->second)
			fclose(it->second);
		it = FileNameMap.erase(it);
	}
}

// Opens the BZClassic.Log file for writing.
bool OpenLogFile()
{
	time_t Time = 0;
	time(&Time);
	struct tm LocalTimestamp;
	localtime_s(&LocalTimestamp, &Time);
	char FileName[MAX_ODF_LENGTH] = {0};
	strftime(FileName, sizeof(FileName), "BZClassic_%Y-%m-%d %H.%M.%S %p.log", &LocalTimestamp);

	wchar_t* pRootOutputDir = NULL;
	size_t bufSize = 0;
	GetOutputPath(bufSize, pRootOutputDir);
	wchar_t *pData = static_cast<wchar_t *>(alloca(bufSize*sizeof(wchar_t)));
	if(GetOutputPath(bufSize, pData))
	{
		wchar_t outPath[MAX_MESSAGE_LENGTH] = {0};
		swprintf_s(outPath, L"%slogs/%S", pData, FileName);
		LogFile = _wfsopen(outPath, L"a+", _SH_DENYWR);
		return true;
	}
	return false;
}
// Formatted Logfile message. Writes a char string to a log file.
bool FormatLogMessage(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char tempstr[MAX_MESSAGE_LENGTH] = {0};
	vsnprintf_s(tempstr, sizeof(tempstr), sizeof(tempstr), format, ap);
	va_end(ap);

	time_t Time = 0;
	time(&Time);
	struct tm LocalTimestamp;
	localtime_s(&LocalTimestamp, &Time);

	char TimeStamp[MAX_ODF_LENGTH] = {0};
	strftime(TimeStamp, sizeof(TimeStamp), "%H:%M:%S", &LocalTimestamp);

	if(!LogFile)
		OpenLogFile();

	if(LogFile)
	{
		fprintf(LogFile, "%s | %f \t| %s\n", TimeStamp, GetTime(), tempstr);
		
		//// nielk1
		//fflush(LogFile);

		if(GetVarItemInt("network.session.ivar115")) // If logging is on, print to console.
			PrintConsoleMessage(tempstr);

		return true;
	}
	else
	{
		char Message[MAX_MESSAGE_LENGTH] = {0};
		sprintf_s(Message, "ERROR: Unable to open Log File! Reporting: %s", tempstr);
		PrintConsoleMessage(Message);
		return false;
	}
}
bool SimpleLogMessage(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char tempstr[MAX_MESSAGE_LENGTH] = { 0 };
	vsnprintf_s(tempstr, sizeof(tempstr), sizeof(tempstr), format, ap);
	va_end(ap);

	if (!LogFile)
		OpenLogFile();

	if (LogFile)
	{
		fprintf(LogFile, "%s", tempstr);

		//// nielk1
		//fflush(LogFile);

		if (GetVarItemInt("network.session.ivar115")) // If logging is on, print to console.
			PrintConsoleMessage(tempstr);

		return true;
	}
	else
	{
		char Message[MAX_MESSAGE_LENGTH] = { 0 };
		sprintf_s(Message, "ERROR: Unable to open Log File! Reporting: %s", tempstr);
		PrintConsoleMessage(Message);
		return false;
	}
}

/*
// Formatted Logfile message. Writes a wchar_t string to the log file.
void FormatLogMessage(wchar_t *format, ...)
{
	va_list ap;
	va_start(ap, format);
	wchar_t tempstr[MAX_MESSAGE_LENGTH] = {0};
	_vsnwprintf_s(tempstr, sizeof(tempstr), sizeof(tempstr), format, ap);

	time_t Time = 0;
	time(&Time);
	struct tm LocalTimestamp;
	localtime_s(&LocalTimestamp, &Time);

	wchar_t TimeStamp[MAX_ODF_LENGTH] = {0};
	wcsftime(TimeStamp, sizeof(TimeStamp), L"%H:%M:%S", &LocalTimestamp);

	if(!LogFile)
		OpenLogFile();

	if(LogFile)
		fwprintf(LogFile, L"%s | %f \t| %s\n", TimeStamp, GetTime(), tempstr);

	PrintConsoleMessage(tempstr); // WCharToChar(tempstr)
}
*/

// Closes the Log file.
void CloseLogFile()
{
	if(LogFile)
		fclose(LogFile);
}

// Prints an IFace_ConsoleCmd like FormatConsoleMessage.
void IFace_FormatConsoleCmd(const char *format, ...)
{
	va_list ap;
	va_start(ap, format);
	char tempstr[MAX_MESSAGE_LENGTH] = {0};
	vsnprintf_s(tempstr, sizeof(tempstr), sizeof(tempstr), format, ap);
	IFace_ConsoleCmd(tempstr);
	va_end(ap);

	if(GetVarItemInt("network.session.ivar115")) // If logging is on, log any console commands inputted.
		FormatLogMessage("IFace Console Command: %s", tempstr);
}

// Returns the current TPS (And sets it for entire game).
int GetTPS(void)
{
	int TPS = 10; // Default.
	EnableHighTPS(TPS);
	return TPS;
}

// Is this object facing another object?
bool IsObjectFacingObject(const Handle h1, const Handle h2, const float ConeAngle)
{
	if((!IsAround(h1)) || (!IsAround(h2)))
		return false;

	Matrix PositionA = GetMatrixPosition(h1);
	Matrix PositionB = GetMatrixPosition(h2);

	return (PositionA.front.x * (PositionB.posit.x - PositionA.posit.x) + PositionA.front.z * (PositionB.posit.z - PositionA.posit.z)) > (portable_cos(ConeAngle) * GetDistance2D(PositionB.posit, PositionA.posit));
}

// Is this Vector facing an object based on two Positions and a ConeAngle?
bool IsObjectFacingObject(const Vector FrontA, const Vector PositionA, const Vector PositionB, const float ConeAngle)
{
	return (FrontA.x * (PositionB.x - PositionA.x) + FrontA.z * (PositionB.z - PositionA.z)) > (portable_cos(ConeAngle) * GetDistance2D(PositionB, PositionA));
}

// Is the object to the Right, or to the Left?
bool IsObjectFacingObjectRight(const Handle me, const Handle him)
{
	if((!IsAround(me)) || (!IsAround(him)))
		return false;

	Matrix PositionA = GetMatrixPosition(me);
	Matrix PositionB = GetMatrixPosition(him);

	return (PositionA.right.x * (PositionB.posit.x - PositionA.posit.x) + PositionA.right.z * (PositionB.posit.z - PositionA.posit.z)) > 0.0f;
}

// Attempts to read the text string from one of the BZ2 Paths read in by the launch config file.
int Get13Version()
{
	if(DoesFileExist("BZ2_v131.3.7-a130.txt")) // Private patch version.
		return v13PB7;
	else if(DoesFileExist("BZ2_v131.3.7.0 Public Beta.txt")) // 1.3 Public Beta 7.0. Release Date: November 14, 2015. Features: - Wrote a RakNet master server replacement in Google Go with a SQL backend. This is hosted on my own server. - Implemented light-weight multithreading where it seemed safe to do so; this and SSE2 optimizations should now provide a smoother framerate ingame. - FOV is adjustable in the menus (about time!). - Work to make 1-shot weapons (e.g. laser) in MP more visible to remote players. - Fonts should look cleaner at larger resolutions (>= 960x720), and up to a 5x resolution scale is possible. - Various other fixes and optimizations.
		return v13PB7;
	else if(DoesFileExist("BZ2_v131.3.6.5 Public Beta.txt")) // 1.3 Public Beta 6.5. Release Date: June 08, 2014. Features: - Replacement matchmaking provided by RakNet. - Various other fixes and optimizations.
		return v13PB65;
	else if(DoesFileExist("BZ2_v131.3.6.4 Public Beta.txt")) // 1.3 Public Beta 6.4. Release Date: June 01, 2013. Features: - Replacement matchmaking provided by RakNet. - Various other fixes and optimizations.
		return v13PB64;
	else if(DoesFileExist("BZ2_v131.3.6.3 Public Beta.txt")) // 1.3 Public Beta 6.3. Release Date: February 12, 2013. Features: -Replacement matchmaking provided by RakNet. - Various other fixes and optimizations.
		return v13PB63;
	else if(DoesFileExist("BZ2_v131.3.6.2 Public Beta.txt")) // 1.3 Public Beta 6.2. Release Date: July 30, 2011. Features: -Reduced warp/lag for MP, -Tweaked network code for better performance with lots of players in game. - Various other fixes and optimizations.
		return v13PB62;
	else if(DoesFileExist("BZ2_v131.3.6.1 Public Beta.txt")) // 1.3 Public Beta 6.1. Release Date: June 09, 2011. Features: -Reduced warp/lag for MP, -Tweaked network code for better performance with lots of players in game. - Various other fixes and optimizations.
		return v13PB61;
	else if(DoesFileExist("BZ2_v13PublicBeta6.txt")) // 1.3 Public Beta 6.0. Release Date: February 18, 2011. Features: -Upgraded to latest GameSpy SDK. BZ2 now uses the GameSpy QR2 communication protocol, the GameSpy transport SDK, GameSpy NatNeg technology for easier hosting w/o having to open ports in many routers, GameSpy Voice communication for ingame voice communication. -Tweaked network code for better performance with lots of players in game.
		return v13PB6;
	else if(DoesFileExist("BZ2_v13PublicBeta5.1.txt")) // 1.3 Public Beta 5.1. Release Date: September 06, 2009. Features: -Performance optimizations to DirectX 9 graphics, most noticeably in the terrain rendering. Satellite view should be far faster, especially on slower machines. - EMBM bumpmaps work once again. High res shadows are re-enabled, but still just as buggy as it's always been (since Battlezone II v1.0). - Local fog should look better. - Various crash bugs fixed. - Max vertex count for models has been upped to 20,000 verts/model; old limit was 2730 verts/model. No stock assets have higher poly counts, but mods may take advantage of that.
		return v13PB51;
	else if(DoesFileExist("BZ2_v13TechAlpha5.txt")) // 1.3 Tech Alpha 5.0. Release Date: April 24, 2009. Features: -Updated graphics engine that uses (and requires) DirectX 9.0c for the best balance of compatibility and speed. Hardware transform & lighting is now supported, allowing a lot of work to be offloaded from your CPU and onto your graphics card. New DirectX features like fullscreen antialiasing (or multisample antialiasing) are supported to improve visual quality. - Various lagout bugs in network games should be drastically reduced. - Further support for Windows Vista/7 -- should fully support being installed to paths with unicode, has high-resolution icon for Windows Explorer. - Music from the BZ2 CD is now distributed with this patch, in ogg vorbis format, allowing for better performance. You can also set up a random playlist of music to be played ingame, if you copy in .ogg files to the right place and the right name. (Note: other music needs to be user-supplied.)
		return v13PB5;
	else if(DoesFileExist("BZ2_v13pb4a.txt")) // 1.3 Public Beta 4a. Release Date: August 04, 2008. Features: - More features, including weapon linking, thumpers, and variable gravity DM maps. - Increased Windows Vista compatability, audio works in WINE. - Many more modder features, such as 60-char ODF names, inheritance, tons of new tuning values, etc. - Performance optimizations. - Fewer resyncs, units (human & AI) should warp less online. - Ability to replace CD music w/ OGG files. - More crash fixes. - Easier to host games w/ automatic UPNP, Windows Firewall support(*). *-*: requires router w/ UPNP support, and Windows Firewall as your primary firewall; other firewall software/hardware will still need some manual attention. *-Note: pb4a now requires a CPU with Streaming SIMD Extensions (SSE) support, which helps give a framerate boost.
		return v13PB4;
	else if(DoesFileExist("BZ2_v13pb3.txt")) // 1.3 Public Beta 3. Release Date: August 10, 2006.
		return v13PB3;
	else if(DoesFileExist("BZ2_v13PublicBeta2.txt")) // 1.3 Public Beta 2. Release Date: February 28, 2006.
		return v13PB2;
	else if(DoesFileExist("BZ2_v13 PublicBeta1.txt")) // 1.3 Public Beta 1. Release Date: February 28, 2004.
		return v13PB1;
	else if(DoesFileExist("nettips.txt")) // 1.2, last official patch. Only valid hook to determine it.
		return v1200;
	else if(DoesFileExist("Keynames.cfg")) // 1.01b, first patch. Only valid hook to determine it.
		return v1012;
	else // Couldn't find anytihng, return NULL.
		return vStock;
}

// Tries to get the BZC Version number stored in PatchData.txt.
float GetBZCVersion(void)
{
	float Version = 0.0f;

	if(OpenODF2("PatchData.txt"))
		GetODFFloat("PatchData.txt", "VersionInfo", "Version", &Version);

	return Version;
}

// Save/Load/Delete functions to interact with Saved Games via DLL.
extern void SaveGame(const char* filename)
{
	FormatConsoleMessage("mission.fileedit %s", filename);
	IFace_ConsoleCmd("mission.save");
}
extern void LoadGame(const char* filename)
{
	FormatConsoleMessage("mission.fileedit %s", filename);
	IFace_ConsoleCmd("mission.load");
}
extern void DeleteGame(const char* filename)
{
	FormatConsoleMessage("mission.fileedit %s", filename);
	IFace_ConsoleCmd("mission.delete");
}

// Get Vector from a Path point: By Nielk1.
Vector GetVectorFromPath(const char* path, const int point)
{
	// New code from Ken Miller.
	Vector retVal(0, 0, 0);
	size_t bufSize = 0;
	GetPathPoints(path, bufSize, NULL);

	// Bad Path, abort!
	if(!bufSize)
		return retVal;

	if(point < UnsignedToSigned(bufSize))
	{
		float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
		if(GetPathPoints(path, bufSize, pData)) 
			retVal = Vector(pData[2*point+0], TerrainFindFloor(pData[2*point+0], pData[2*point+1]), pData[2*point+1]); 
	}

	return retVal;
}

// Gets a random Team, with teamfilter options.
int GetRandomTeam(const int TeamFilter, const int Team, const bool ActiveTeamsOnly, const bool SkipTeamZero)
{
	const int TempTeamFilter = TeamFilter ? TeamFilter : -1;
	//int TempTeam = -1;

	int Start = GetRandomInt(MAX_TEAMS-1);
	int x = Start;
	do
	{
		if(((Team < 0) || (TeamFilterCheck(TempTeamFilter, x, Team))) && // Team matches.
			(((ActiveTeamsOnly) && ( // ActiveTeamOnly, If they have any BaseSlot 1 - 9 items, or a Player.
			(GetPlayerHandle(x)) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_RECYCLER))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_FACTORY))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_ARMORY))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_BASE4))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_BASE5))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_BASE6))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_BASE7))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_BASE8))) || 
			(IsAround(GetObjectByTeamSlot(x, DLL_TEAM_SLOT_BASE9))))) || (!ActiveTeamsOnly)) || // OR not ActiveTeamOnly.
			(((SkipTeamZero) && (x > 0)) || (!SkipTeamZero))) // SkipTeamZero matches.
		{
			//TempTeam = x;
			return x;
		//	break;
		}
		++x;
		if (x == MAX_TEAMS-1)
			x = 0;
	}
	while (x != Start);

	return -1; //TempTeam; // Didn't find anything.
}

// Determines if a vector is within a path area. This version takes a vector_2D. Code from N1.
bool PointInPoly(const std::vector<VECTOR_2D> &areaPath, const Vector &vector)
{
	// New code from Ken, using Winding Cross test.
	return GetWindCount(areaPath, vector) != 0;

	// Old code by N1, Ray Test.
#if 0
	/* The points creating the polygon. */
	float x1 = 0.0f, x2 = 0.0f;
	
	/* The coordinates of the point */
	float px = 0.0f, py = 0.0f;
	px = vector.x;
	py = vector.z;
	
	/* How many times the ray crosses a line-segment */
//	int crossings = 0;
	bool inside = false;
	
	int numOfSides = areaPath.size();

	/* Iterate through each line */
	//int i = 0, j = 0;
	for (int i = 0, j = 1; i < numOfSides; i++, j++)
	{	
		if(j >= numOfSides)
			j = 0;

		/* This is done to ensure that we get the same result when
		the line goes from left to right and right to left */
		if ( areaPath[i].x < areaPath[j].x ){
			x1 = areaPath[i].x;
			x2 = areaPath[j].x;
		} else {
			x1 = areaPath[j].x;
			x2 = areaPath[i].x;
		}
	
		// New, simpler version by Ken. Untested, Changed to Winding Number code instead. -GBD
		/* Calculate the equation of the line */
		float dx = areaPath[j].x - areaPath[i].x;
		float dy = areaPath[j].z - areaPath[i].z;

		if ( dx * py - dy * px <= dx * areaPath[i].z - dy * areaPath[i].x )
			inside = !inside;
//#if 0 // Old code by N1.
		/* First check if the ray is possible to cross the line */
		if ( px > x1 && px <= x2 && ( py < areaPath[i].z || py <= areaPath[j].z ) )
		{
			static const float eps = 0.000001f;
	
			/* Calculate the equation of the line */
			float dx = areaPath[j].x - areaPath[i].x;
			float dy = areaPath[j].z - areaPath[i].z;
			float k = 0.0f;
	
			if ( fabs(dx) < eps ){
				k = FLT_MAX; 	// math.h //float('inf');
			} else {
				k = dy/dx;
			}
	
			float m = areaPath[i].z - k * areaPath[i].x;
			
				/* Find if the ray crosses the line */
			float y2 = k * px + m;
			if ( py <= y2 )
			{
//				crossings++;
				inside = !inside;
			}
//#endif
		}
	}

//	if ( crossings % 2 == 1 ){
//		return true;
//	}
//	return false;
	return inside;
#endif
}

// Determines if vector is in a path area. This version takes a pathpoint name.
bool PointInPoly(const char* path, const Vector vector)
{
	std::vector<VECTOR_2D> areaPath;
	areaPath.clear();
	size_t bufSize = 0;
	GetPathPoints(path, bufSize, NULL);

	// Bad Path, abort!
	if(!bufSize)
		return false;

	float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
	if(GetPathPoints(path, bufSize, pData))
	{
		for(size_t i = 0; i < bufSize; ++i)
		{
			VECTOR_2D newPoint; // does this need to be a pointer?
			newPoint.x = pData[2*i+0];
			newPoint.z = pData[2*i+1];
			areaPath.push_back(newPoint);
		}
		//printf("Pathpoint %d at (%.2f, %.2f)\n", i, pData[2*i+0], pData[2*i+1]);
	}

	return PointInPoly(areaPath, vector);
}

// Alternate PointInPoly code Ken Miller found online: http://geomalgorithms.com/a03-_inclusion.html
// Ken's LUA Implementation: http://pastebin.com/nhtkYEpN

int GetWindCount(const std::vector<VECTOR_2D> &areaPath, const Vector &vector)
{
	int wn = 0; // the  winding number counter.
	int numOfSides = areaPath.size();

	// Iterate through each line.
	for (int i = 0, j = 1; i < numOfSides; i++, j++)
	{	
		if(j >= numOfSides)
			j = 0;

		if(areaPath[i].z <= vector.z) // start z <= Vector.z
		{
            if((areaPath[j].z > vector.z) && (IsLeftOnRight(areaPath[i], areaPath[j], vector) > 0))  // An upward crossing and Vector left of  edge.
				++wn; // have  a valid up intersect
        }
        else // start z > Vector.z (no test needed)
		{
            if((areaPath[j].z <= vector.z) && (IsLeftOnRight(areaPath[i], areaPath[j], vector) < 0)) // A downward crossing and Vector right of  edge.
				--wn; // have  a valid down intersect
        }
	}

	return wn;
}

// Version that takes a Path Name.
int GetWindCount(const char* path, const Vector vector)
{
	std::vector<VECTOR_2D> areaPath;
	areaPath.clear();
	size_t bufSize = 0;
	GetPathPoints(path, bufSize, NULL);

	// Bad Path, abort!
	if(!bufSize)
		return 0;

	float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
	if(GetPathPoints(path, bufSize, pData))
	{
		for(size_t i = 0; i < bufSize; ++i)
		{
			VECTOR_2D newPoint; // does this need to be a pointer?
			newPoint.x = pData[2*i+0];
			newPoint.z = pData[2*i+1];
			areaPath.push_back(newPoint);
		}
		//printf("Pathpoint %d at (%.2f, %.2f)\n", i, pData[2*i+0], pData[2*i+1]);
	}

	return GetWindCount(areaPath, vector);
}



/*
// wn_PnPoly(): winding number test for a point in a polygon
//      Input:   P = a point,
//               V[] = vertex points of a polygon V[n+1] with V[n]=V[0]
//      Return:  wn = the winding number (=0 only when P is outside)
int
wn_PnPoly( Point P, Point* V, int n )
{
    int    wn = 0;    // the  winding number counter

    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {   // edge from V[i] to  V[i+1]
        if (V[i].y <= P.y) {          // start y <= P.y
            if (V[i+1].y  > P.y)      // an upward crossing
                 if (isLeft( V[i], V[i+1], P) > 0)  // P left of  edge
                     ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (V[i+1].y  <= P.y)     // a downward crossing
                 if (isLeft( V[i], V[i+1], P) < 0)  // P right of  edge
                     --wn;            // have  a valid down intersect
        }
    }
    return wn;
}
//===================================================================
*/

// Checks if a pathpoint exists.
bool DoesPathExist(const char* path)
{
	size_t bufSize = 0;
	GetPathPoints(path, bufSize, NULL);

	return bufSize > 0;
}

// Determines if the Handle is damaged beyond the threshold.
bool IsDamaged(const Handle h, const float Threshold)
{
	if(!IsAround(h))
		return false; // Invalid Handle.

	return GetHealth(h) < Threshold;
}

// Checks if a handle is done going down a path.
bool IsFinishedWithPath(const Handle h, const char* path)
{
	if(!IsAround(h))
		return false;

	size_t bufSize = 0;
	GetPathPoints(path, bufSize, NULL);

	return bufSize > 0 && GetDistance(h, path, bufSize) < 12.0f; //5.0f;
}

// IsInfo that uses a handle. No need to know exact ODF name :)
bool IsInfo(const Handle h)
{
	if(!IsAround(h))
		return false;

	char ODFName[MAX_ODF_LENGTH] = {0};
	GetODFName(h, ODFName);

	return IsInfo(ODFName); //GetLocalUserInspectHandle() == h
}

// Is something following something?
bool IsFollowing(const Handle me, const Handle him)
{
	if(!IsAround(him)) // IsAround Me checked by WhoFollowing. :)
		return false;

	return WhoFollowing(me) == him;
}

// Command a unit to Recycle. 
void Recycle(const Handle h, const int priority)
{
	SetCommand(h, CMD_RECYCLE, 1, priority);
}

// Finds out if a handle needs Health.
bool NeedsHealth(const Handle h)
{
	if(!IsAround(h))
		return false;

	return GetCurHealth(h) < GetMaxHealth(h);
}

// Finds out if a handle needs Ammo.
bool NeedsAmmo(const Handle h)
{
	if(!IsAround(h))
		return false;

	return GetMaxAmmo(h) < GetCurAmmo(h);
}

// Parellel to GetHealth, Sets the health as a % of MaxHealth.
void SetHealth(const Handle h, const float Percent)
{
	if(!IsAround(h))
		return;

	float TempP = clamp(Percent, 0.0f, 1.0f);
	SetCurHealth(h, long(GetMaxHealth(h) * TempP));
}

// Parellel to GetAmmo, Sets the ammo as a % of MaxAmmo.
void SetAmmo(const Handle h, const float Percent)
{
	if(!IsAround(h))
		return;

	float TempP = clamp(Percent, 0.0f, 1.0f);
	SetCurAmmo(h, long(GetMaxAmmo(h) * TempP));
}

// Version of GiveWeapon that utilizes ReplaceWeapon to only switch out one slot.
void GiveWeapon(const Handle h, const char* Weapon, const int Slot)
{
	if(!IsAround(h))
		return;

	if(!DoesODFExist(Weapon))
	{
		FormatLogMessage("ERROR: Weapon ODF %s doesn't exist! Fix DLLs GiveWeapon call!", Weapon);
		return;
	}

	float LocalAmmo[MAX_HARDPOINTS] = {0};
	char Weapons[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	for(int i = 0; i < MAX_HARDPOINTS; i++)
	{
		GetObjInfo(h, ObjectInfoType(Get_Weapon0Config+i), Weapons[i]);
		LocalAmmo[i] = GetCurLocalAmmo(h, i);
	}

	LocalAmmo[Slot] = 0;
	strcpy_s(Weapons[Slot], sizeof(Weapon), Weapon);

	ReplaceWeapons(h, Weapons, LocalAmmo);
}

// Triggers a BZC Production Unit to Undeploy, if possible.
void PackUp(const Handle h)
{
	if(!IsAround(h))
		return;

	int Team = GetTeamNum(h);

	// Loop through all 9 Base Slots, find the one that matches this undeploy item.
	for(int x = 0; x < 9; x++)
	{
		int i = x+1;
		if(x>2)
			i = x+6;

		if(GetObjectByTeamSlot(Team, i) == h)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "BZCundep%d", x+1);
			BuildObject(DesiredValue, Team, h);
			return; //break; // We found it.
		}
	}
}

// Gets the number of Taps a handle has, optionally excluding invulnerable ones.
int GetTapCount(const Handle h, const bool IgnoreInvincible)
{
	int Taps = 0;
	for(int x = 0; x < MAX_TAPS; x++)
	{
		Handle TempTap = GetTap(h, x);
		if((TempTap) && ((!IgnoreInvincible) || (GetMaxHealth(TempTap) > 0)))
			++Taps;
	}
	return Taps;
}

// Sets the position of an object to the position of another object.
void SetPositionM(const Handle h1, const Handle h2)
{
	Matrix Position = GetMatrixPosition(h2);
	SetPosition(h1, Position);
}

// Determine TeamFilter passing.
bool TeamFilterCheck(const int TeamFilter, const int Team1, const int Team2)
{
	if(Team1 < 0 || Team1 > 15 || Team2 < 0 || Team2 > 15)
		return false;

	return ((((TeamFilter & 1) == 1) && Team1 == Team2) ||  // Same Team Only
			((Team1 != Team2) && // Not same Team.
			((((TeamFilter & 2) == 2) && (IsTeamAllied(Team1, Team2))) ||  // Allies Only
			(((TeamFilter & 4) == 4) && (!IsTeamAllied(Team1, Team2))))));  // Enemies Only
}

// Adds to the total of the Scavenger's current scrap.
void AddScavengerCurScrap(const Handle h, const int aNewScrap)
{
	if((!IsAround(h)) || (!IsScavenger(h)))
		return;

	int ScavScrap = GetScavengerCurScrap(h);
	ScavScrap += aNewScrap;
	SetScavengerCurScrap(h, ScavScrap);
}

// Adds to the total of the Scavengers max's scrap.
void AddScavengerMaxScrap(const Handle h, const int aNewScrap)
{
	if((!IsAround(h)) || (!IsScavenger(h)))
		return;

	int ScavScrap = GetScavengerMaxScrap(h);
	ScavScrap += aNewScrap;
	SetScavengerMaxScrap(h, ScavScrap);
}

// Generates an artifical scrap field. :)
void SpawnScrapField(const Vector Pos, const int Amouunt, const float Radius, const char Race)
{
	if(IsNullVector(Pos))
		return;

	// Randomize the scrap.
	char ScrapODF[MAX_ODF_LENGTH] = {0};
	float MaxDist = 10.0f * sqrtf(float(Amouunt));

	if(Radius > 0) // If there is a specified radius, use that.
		MaxDist = Radius;

	for(int i = 0; i < Amouunt; i++)
	{
		sprintf_s(ScrapODF, "%cpscr%d", Race, GetRandomInt(2)+1);
		BuildObject(ScrapODF, 0, Build_Directinal_Matrix(GetPositionNear(Pos, 0, MaxDist), HRotateFront(Vector(0.0f, 0.0f, 1.0f), GetRandomFloat(360.0f) * DEG_2_RAD)));
	}
}

// Gets the EngageRange of a handle.
float GetEngageRange(const Handle h)
{
	if(!IsAround(h))
		return 0;

	float TempVal = 0.0f;

	if((GetODFFloat(h, "CraftClass", "EngageRange", &TempVal, 0.0f)) || 
		(GetODFFloat(h, "CraftClass", "RangeScan", &TempVal, 200.0f))) // || (GetODFFloat(h, "PoweredBuildingClass", "detectRange", &TempVal, 100.0f)))
	{
		// Got it.
	}
	else // Not specified, go into Weapons and compare ranges. Choose the highest one.
	{
		char WeaponODF[MAX_ODF_LENGTH] = {0};
		char OrdODF[MAX_ODF_LENGTH] = {0};
		char DesiredValue[MAX_ODF_LENGTH] = {0};
		for(int i = 0; i < MAX_HARDPOINTS; i++)
		{
			float TestVal = 0.0f;
			sprintf_s(DesiredValue, "weaponName%d", i+1);
			if((GetODFString(h, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, WeaponODF)) && (WeaponODF[0]))
			{
				strcat_s(WeaponODF, ".odf"); // Add .odf to the end.
				if(OpenODF2(WeaponODF))
				{
					if(GetODFFloat(WeaponODF, "WeaponClass", "aiRange", &TestVal))
					{
						// Got it.
					}
					else if(GetODFString(WeaponODF, "WeaponClass", "ordName", MAX_ODF_LENGTH, OrdODF))
					{
						strcat_s(OrdODF, ".odf"); // Add .odf to the end.
						if(OpenODF2(OrdODF))
						{
							float ShotSpeed = 0.0f;
							float LifeSpan = 0.0f;
							GetODFFloat(OrdODF, "OrdnanceClass", "shotSpeed", &ShotSpeed);
							GetODFFloat(OrdODF, "OrdnanceClass", "lifeSpan", &LifeSpan);

							TestVal = ShotSpeed * LifeSpan;
						}
					}
				}
			}
			if(TestVal > TempVal)
				TempVal = TestVal;
		}
	}
	return TempVal;
}

// Gets the minimum amount of ammo needed to fire an AI's Weapons.
long GetMinFireAmmo(const Handle h)
{
	if(!IsAround(h))
		return 0;

	long MinFireAmmo = 0;

	char WeaponMask[MAX_HARDPOINTS+1] = {0};
	GetODFString(h, "GameObjectClass", "weaponMask", MAX_HARDPOINTS+1, WeaponMask);

	// Loop over all weapons, if they're set in weaponMask, add their required ammo together.
	for(int i = 0; i < MAX_HARDPOINTS; i++)
	{
		bool WeaponMasks = (WeaponMask[4-i] != '0'); // A function to GetWeaponMask(h) would be nice. 
		if(WeaponMasks)
		{
			char WeaponODF[MAX_ODF_LENGTH] = {0};
			GetObjInfo(h, ObjectInfoType(Get_Weapon0ODF+i), WeaponODF);

			//sprintf_s(TempODF, "%s.odf", WeaponODF); // Add .odf to the end.
			if((WeaponODF[0]) && (OpenODF2(WeaponODF)))
			{
				char TempODF2[MAX_ODF_LENGTH] = {0};
				if(GetODFString(WeaponODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
				{
					strcat_s(TempODF2, ".odf"); // Add .odf to the end.
					OpenODF2(TempODF2);
				}

				char TempODF3[MAX_ODF_LENGTH] = {0};
				if((GetODFString(WeaponODF, TempODF2, "WeaponClass", "ordName", MAX_ODF_LENGTH, TempODF3)) || 
					(GetODFString(WeaponODF, TempODF2, "DispenserClass", "objectClass", MAX_ODF_LENGTH, TempODF3)) || 
					(GetODFString(WeaponODF, TempODF2, "TorpedoLauncherClass", "objectClass", MAX_ODF_LENGTH, TempODF3)))
				{
					// Got it.
				}

				if(TempODF3[0])
				{
					strcat_s(TempODF3, ".odf"); // Add .odf to the end.
					if(OpenODF2(TempODF3))
					{
						char TempODF4[MAX_ODF_LENGTH] = {0};
						long TempAmmoCost = 0;
						if(GetODFString(TempODF3, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF4))
						{
							strcat_s(TempODF4, ".odf"); // Add .odf to the end.
							OpenODF2(TempODF4);
						}

						if(GetODFLong(TempODF3, TempODF4, "OrdnanceClass", "ammoCost", &TempAmmoCost))
							MinFireAmmo += TempAmmoCost;
					}
				}
			}
		}
	}

	return MinFireAmmo;
}

// Gets the damage an ordnance should do to the specified handle.
long GetOrdnanceDamageDealt(const Handle h, const char *OrdnanceODF, const bool UseExplosion)
{
	if(!IsAround(h))
		return -1; // Return invalid.

	long Damage = 0;

	char TempODF[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};

	int ArmorClass = 78; // N = 78, n = 110, L = 76, l = 108, H = 72, h = 104
	int ShieldClass = 0; // A = 65, a = 97, D = 68, d = 100, S = 83, s = 115
	// Get it's Armor class for later use.
	char TempArmorClass = 0;
	GetODFChar(h, "GameObjectClass", "armorClass", &TempArmorClass, 'N');
	ArmorClass = int(TempArmorClass);

	for(int i = 0; i < MAX_HARDPOINTS; i++)
	{
		char HostWeaponClass[MAX_ODF_LENGTH] = {0};
		GetObjInfo(h, ObjectInfoType(Get_Weapon0GOClass+i), HostWeaponClass);

		if(_stricmp(HostWeaponClass,"CLASS_SHIELDUP") == 0)
		{
			char HostWeapon[MAX_ODF_LENGTH] = {0};
			GetObjInfo(h, ObjectInfoType(Get_Weapon0ODF+i), HostWeapon);

			char TempShieldClass = 0;
			if(OpenODF2(HostWeapon))
			{
				if(GetODFString(HostWeapon, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2)) // Grab the classlabel.
				{
					strcat_s(TempODF2, ".odf"); // Add .odf to the end.
					OpenODF2(TempODF2);
				}

				GetODFChar(HostWeapon, TempODF2, "ShieldUpgradeClass", "shieldClass", &TempShieldClass, 'S'); // Grab the value.
			}
			ShieldClass = int(TempShieldClass);

			break;
		}
	} // Shield Check.


	char ExplosionODF[MAX_ODF_LENGTH] = {0}; // A place to store our desired variable. 
	char ExplosionODF2[MAX_ODF_LENGTH] = {0};
	strcpy_s(TempODF, OrdnanceODF);
	if(OpenODF2(TempODF))
	{
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf");
			OpenODF2(TempODF2);
		}

		int RealDamage = 0;
		int xplRealDamage = 0;

		char DesiredValue[MAX_ODF_LENGTH] = {0};
		char ExplosionType[MAX_ODF_LENGTH] = {0};

		// N = 78, n = 110, L = 76, l = 108, H = 72, h = 104
		// A = 65, a = 97, D = 68, d = 100, S = 83, s = 115
		if((ShieldClass == 65) || (ShieldClass == 97))
			strcpy_s(DesiredValue, "damageValue(A)");
		else if((ShieldClass == 68) || (ShieldClass == 100))
			strcpy_s(DesiredValue, "damageValue(D)");
		else if((ShieldClass == 83) || (ShieldClass == 115))
			strcpy_s(DesiredValue, "damageValue(S)");
		else if((ArmorClass == 72) || (ArmorClass == 104))
			strcpy_s(DesiredValue, "damageValue(H)");
		else if((ArmorClass == 76) || (ArmorClass == 108))
			strcpy_s(DesiredValue, "damageValue(L)");
		else
			strcpy_s(DesiredValue, "damageValue(N)");

		// Retrieve Damage Values here.
		GetODFInt(TempODF, TempODF2, "OrdnanceClass", DesiredValue, &RealDamage);

		if(UseExplosion)
		{
			// Pull Explosion ODF Damage, and include it.
			if(IsBuilding(h))
				strcpy_s(ExplosionType, "xplBuilding");
			else
				strcpy_s(ExplosionType, "xplVehicle");

			GetODFString(TempODF, TempODF2, "OrdnanceClass", ExplosionType, MAX_ODF_LENGTH, ExplosionODF);

			if(OpenODF2(ExplosionODF))
			{
				if(GetODFString(ExplosionODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, ExplosionODF2))
				{
					strcat_s(ExplosionODF2, ".odf");
					OpenODF2(ExplosionODF2);
				}
				GetODFInt(TempODF, TempODF2, "OrdnanceClass", DesiredValue, &xplRealDamage);
			}

			// Combine the damage types.
			Damage = RealDamage + xplRealDamage;
		}
		else
		{
			Damage = RealDamage;
		}
	}
	return Damage;
}

/*
// Gets a Vector based off an ODF Position Offset.
void GetODFPositionOffset(Handle h, Vector *Position, int Index, float AddOffsetX, float AddOffsetY, float AddOffsetZ)
{
	Matrix TempPos;
	memset(&TempPos, 0, sizeof(TempPos));
	GetPosition(h, TempPos);

	Vector Offset(0, 0, 0);
	char DesiredValue[MAX_ODF_LENGTH] = {0};
	sprintf_s(DesiredValue, "PositionOffset%d", Index+1);
	GetODFVector(h, "GameObjectClass", DesiredValue, &Offset);
	TempPos.posit.x += AddOffsetX;
	TempPos.posit.y += AddOffsetY;
	TempPos.posit.z += AddOffsetZ;

	Position = &Matrix_Multiply(Build_Position_Rotation_Matrix(0, 0, 0, Offset), TempPos).posit;

	//return Position.posit;
}

// Gets a Matrix based off an ODF Position Offset, and ODF Rotation Offset.
void GetODFPositionOffset(Handle h, Matrix *Position, int Index, float AddOffsetX, float AddOffsetY, float AddOffsetZ, float RotationOffsetX, float RotationOffsetY, float RotationOffsetZ)
{
	Matrix TempPos;
	memset(&TempPos, 0, sizeof(TempPos));
	GetPosition(h, TempPos);

	Vector Offset(0, 0, 0);
	Vector Rotation(0, 0, 0);
	char DesiredValue[MAX_ODF_LENGTH] = {0};
	sprintf_s(DesiredValue, "PositionOffset%d", Index+1);
	GetODFVector(h, "GameObjectClass", DesiredValue, &Offset);
	TempPos.posit.x += AddOffsetX;
	TempPos.posit.y += AddOffsetY;
	TempPos.posit.z += AddOffsetZ;
	sprintf_s(DesiredValue, "RotationOffset%d", Index+1);
	GetODFVector(h, "GameObjectClass", DesiredValue, &Rotation);
	Rotation.x += RotationOffsetX;
	Rotation.y += RotationOffsetY;
	Rotation.z += RotationOffsetZ;
	Rotation.x = Rotation.x*DEG_2_RAD;
	Rotation.y = Rotation.y*DEG_2_RAD;
	Rotation.z = Rotation.z*DEG_2_RAD;

	Position = &Matrix_Multiply(Build_Position_Rotation_Matrix(Rotation.x, Rotation.y, Rotation.z, Offset), TempPos);

	//return Position;
}
*/

// Gets a Matrix based off an ODF Position Offset, and ODF Rotation Offset.
Matrix GetODFPositionOffset(const Handle h, const int Index, const Vector AddPosition, const Vector AddRotation)
{
	if(!IsAround(h))
		return Identity_Matrix;

	Vector Position(0, 0, 0);
	char DesiredValue[MAX_ODF_LENGTH] = {0};
	sprintf_s(DesiredValue, "PositionOffset%d", Index+1);
	GetODFVector(h, "GameObjectClass", DesiredValue, &Position);
	Position = Add_Vectors(Position, AddPosition);

	Vector Rotation(0, 0, 0);
	sprintf_s(DesiredValue, "AddRotation%d", Index+1);
	GetODFVector(h, "GameObjectClass", DesiredValue, &Rotation);
	Rotation = Add_Vectors(Rotation, AddRotation);
	Rotation = Vector_Scale(Rotation, DEG_2_RAD);

	const Matrix BaseMat = GetMatrixPosition(h);
	const Matrix LocalMat = Build_Position_Rotation_Matrix(Rotation.x, Rotation.y, Rotation.z, Position);
	const Matrix WorldMat = Matrix_Multiply(LocalMat, BaseMat);
	return WorldMat;
}

// Gets a Vector further away from v1 then v2, with min/max radius's.
Vector GetPositionNearOrFar(const Vector v1, const Vector v2, const float MinRadius, const float MaxRadius, const bool Near)
{
	Vector v3 = GetPositionNear(v2, MinRadius, MaxRadius);

	if(v1 != v2)
	{
		Vector dir = Normalize_Vector(v1 - v2);
		float alongDir = Dot_Product(v3 - v2, dir);

		if((alongDir > 0) ^ Near)
		{
			v3 = Add_Mult_Vectors(v3, dir, -2.0f * alongDir);
			v3.y = TerrainFindFloor(v3.x, v3.z); // Recompute terrain height.
		}
	}
	return v3; //Go directly to Jail, Do not pass Go, Do not collect $200.
}

// Conforms an ODFName retrieved with Get_CFG to cutoff the :## appended to it if it was built by a factory. Also removes the . if there's a .odf attached.
void CheckODFName(char *ODFName)
{
	if(!ODFName)
		return; // Null pointer, bad. return! return!

	if(!DoesODFExist(ODFName))
	{
		if (char *dot = strchr(ODFName, ':'))
			*dot = '\0';

		if(!DoesODFExist(ODFName))
		{
			if (char *dot = strrchr(ODFName, '.'))
				*dot = '\0';
		}
	}
}

// Gets the unit's Name, without the Skill modifier " ****" applied.
char TempGetUnitName[MAX_ODF_LENGTH];
const char *GetUnitName(const Handle h)
{
	 strcpy_s(TempGetUnitName, GetObjectiveName(h));
	// Step through backwards, removing any previous skill level " ****" on the end of the name. Code from Ken.
	size_t len = strlen(TempGetUnitName)-1;
	for (size_t pos = len; pos >= 0; --pos)
	{
		if (TempGetUnitName[pos] != '*')
		{
			if (pos < len && TempGetUnitName[pos] == ' ')
			{
				TempGetUnitName[pos] = '\0';
				len = pos;
			}
			break;
		}
	}

	return TempGetUnitName;
}
// Applies the Skill Name to the unit's name.
void SetSkillName(const Handle h, const int Skill)
{
	int CurSkill = Skill;

	if(Skill < 0)
		CurSkill = GetSkill(h);

	if(CurSkill <= 0)
		return; // Don't bother

	char UnitName[MAX_ODF_LENGTH] = {0};
	strcpy_s(UnitName, GetUnitName(h));
	// If our length is long enough, add the " ****" to the end. If it's not, don't do anything. Code from Ken.
	if(!IsPlayer(h))
	{
		size_t len = strlen(UnitName)-1;
		if((len + 1 + CurSkill + 1) < MAX_ODF_LENGTH)
		{
			UnitName[++len] = ' ';
			for (int i = 0; i < CurSkill; ++i)
				UnitName[++len] = '*';
			UnitName[++len] = '\0';
		}
	}

	SetObjectiveName(h, UnitName); // Update the name.
}

// Attempts to play a VO msg from a unit's [CraftClass] section.
void PlayVOMsg(const Handle h, const char *MsgString, const int Team)
{
	if((!IsAround(h)) || (!MsgString))
		return;

	const int TempTeam = Team == -1 ? GetTeamNum(h) : Team;

	// Play user2Msg, if possible.
	if(TempTeam == GetLocalPlayerTeamNumber())
	{
		char MsgSound[MAX_ODF_LENGTH] = {0};
		if(GetODFString(h, "CraftClass", MsgString, MAX_ODF_LENGTH, MsgSound))
		{
			if(MsgSound[0])
				AudioMessage(MsgSound);
		}
	}

}

// Conforms an ODFName retrieved with Get_ODF to cutoff the .odf.
void GetODFName(const Handle h, char *ODFName)
{
	if(!ODFName)
		return; // Null pointer, bad. return! return!

	if(!GetObjInfo(h, Get_ODF, ODFName))
		return; // Invalid Handle.

	if (char *dot = strrchr(ODFName, '.'))
		*dot = '\0';
}
// Version that returns a char *.
char TempGetODFName[MAX_ODF_LENGTH];
const char *GetODFName(const Handle h)
{
	memset(TempGetODFName, 0, sizeof(TempGetODFName)); // Clear this before use.

	if(!GetObjInfo(h, Get_ODF, TempGetODFName))
		return TempGetODFName; // Don't return NULL, since we don't want a char * null pointer, instead return an empty string, "".

	if (char *dot = strrchr(TempGetODFName, '.'))
		*dot = '\0';

	return TempGetODFName;
}

// Function that gets the ODFName, as well as a TempODF of the name with .odf and the classlabel string with .odf extention.
void GetAndOpenODFNames(const Handle h, char *TempODF1, char *TempODF2, char *ODFName)
{
	if((!TempODF1) || (!TempODF2))
		return; // Null pointers, bad. return! return!

	if(!GetObjInfo(h, Get_ODF, TempODF1))
		return; // Invalid Handle.

	if((OpenODF2(TempODF1)) && (GetODFString(TempODF1, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2)))
	{
		if(DoesODFExist(TempODF2))
		{
			strcat_s(TempODF2, MAX_ODF_LENGTH, ".odf"); // Add .odf to the end.
			OpenODF2(TempODF2); // Open 
		}
	}

	if(ODFName) // If ODFName is != NULL.
	{
		strcpy_s(ODFName, MAX_ODF_LENGTH, TempODF1); // Copy the retrieved ODF to ODFName too, and cutoff the .odf.
		if (char *dot = strrchr(ODFName, '.'))
			*dot = '\0';
	}
}

// Sets the world variant ODF, if it exists.
bool GetWorldVariantODF(char *ODFName)
{
	if(!ODFName)
		return false;

	if (char *dot = strrchr(ODFName, '.'))
		*dot = '\0';

	if(tolower(ODFName[1]) == 'v') // If it's a *v* odf, vehicle, apply world letter. -GBD
	{
		char WorldSetting = 0;
		if(GetODFChar(GetMapTRNFilename(), "World", "WorldLetter", &WorldSetting)) // Grab the world letter.
		{
			char TempWorldODF[MAX_ODF_LENGTH] = {0};
			sprintf_s(TempWorldODF, "%s%c", ODFName, WorldSetting);

			if(DoesODFExist(TempWorldODF))
			{
				strcpy_s(ODFName, sizeof(TempWorldODF), TempWorldODF);
				return true;
			}
		}
	}
	return false;
}
char TempWorldVariantODFName[MAX_ODF_LENGTH];
const char *GetWorldVariantODF(const Handle h)
{
	memset(TempWorldVariantODFName, 0, sizeof(TempWorldVariantODFName)); // Clear this before use.

	if(!GetObjInfo(h, Get_ODF, TempWorldVariantODFName))
		return TempWorldVariantODFName; // Don't return NULL, since we don't want a char * null pointer, instead return an empty string, "".

	GetWorldVariantODF(TempWorldVariantODFName);

	return TempWorldVariantODFName;
}


// Gets a random ODF of the specified race, if it exists.
char TempRandomOffensiveODFName[MAX_ODF_LENGTH];
const char *GetRandomOffensiveODF(const char Race)
{
	memset(TempRandomOffensiveODFName, 0, sizeof(TempRandomOffensiveODFName)); // Clear this before use.

	for(int i = 0; i < 256; i++) // Try 256 times to get a valid ODF.
	{
		strcpy_s(TempRandomOffensiveODFName, OffensiveShipList[GetRandomInt(sizeof(OffensiveShipList)/sizeof(OffensiveShipList[0])-1)]);
		TempRandomOffensiveODFName[0] = Race; // Assign Race lettr.
		if(DoesODFExist(TempRandomOffensiveODFName))
			return TempRandomOffensiveODFName;
	}
	return NULL; // Failed. :(
}
char TempRandomDefensiveODFName[MAX_ODF_LENGTH];
const char *GetRandomDefensiveODF(const char Race)
{
	memset(TempRandomDefensiveODFName, 0, sizeof(TempRandomDefensiveODFName)); // Clear this before use.

	for(int i = 0; i < 256; i++) // Try 256 times to get a valid ODF.
	{
		strcpy_s(TempRandomDefensiveODFName, DefensiveShipList[GetRandomInt(sizeof(DefensiveShipList)/sizeof(DefensiveShipList[0])-1)]);
		TempRandomDefensiveODFName[0] = Race; // Assign Race lettr.
		if(DoesODFExist(TempRandomDefensiveODFName))
			return TempRandomDefensiveODFName;
	}
	return NULL; // Failed. :(
}
char TempRandomUtilityODFName[MAX_ODF_LENGTH];
const char *GetRandomUtilityODF(const char Race)
{
	memset(TempRandomUtilityODFName, 0, sizeof(TempRandomUtilityODFName)); // Clear this before use.

	for(int i = 0; i < 256; i++) // Try 256 times to get a valid ODF.
	{
		strcpy_s(TempRandomUtilityODFName, UtilityShipList[GetRandomInt(sizeof(UtilityShipList)/sizeof(UtilityShipList[0])-1)]);
		TempRandomUtilityODFName[0] = Race; // Assign Race lettr.
		if(DoesODFExist(TempRandomUtilityODFName))
			return TempRandomUtilityODFName;
	}
	return NULL; // Failed. :(
}
// Gets any random, out of a random choice of the above.
const char *GetRandomShipODF(const char Race)
{
	// This is a switch case. :)
	switch(GetRandomInt(2)) // 1 of 3...
	{
	case 0:
		{
			return GetRandomOffensiveODF(Race);
		}
	case 1:
		{
			return GetRandomDefensiveODF(Race);
		}
	case 2:
		{
			return GetRandomUtilityODF(Race);
		}
	}
	return NULL; // Failed. :(
}

// Preloads a race given a race char.
void PreloadRace(const char Race)
{
	char TestODF[MAX_ODF_LENGTH] = {0};
	for(int i = 0; i < sizeof(PreloadRaceList)/sizeof(PreloadRaceList[0]); i++)
	{
		sprintf_s(TestODF, "%c%s", Race, PreloadRaceList[i]);
		if(DoesODFExist(TestODF))
			PreloadODF(TestODF);
	}
}

// Generalized Class Filters.
// Is it a Building?
bool IsBuilding(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;
	/*
	char EntityType[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_EntityType, EntityType))
		return false;

	return (_stricmp(EntityType, "CLASS_ID_BUILDING") == 0) || (_stricmp(ObjClass, "CLASS_TURRET") == 0) || (_stricmp(ObjClass, "CLASS_I76BUILDING") == 0);
	*/

//	/*
	int ObjClassHash = Hash(ObjClass);
	for(int i = 0; i < sizeof(BuildingClass)/sizeof(BuildingClass[0]); i++)
		if(ObjClassHash == BuildingClass[i])
			return true;

	return false;
//	*/
}

// Is it a Vehicle?
bool IsVehicle(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	/*
	char EntityType[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_EntityType, EntityType))
		return false;

	return ((_stricmp(EntityType, "CLASS_ID_CRAFT") == 0) || (_stricmp(EntityType, "CLASS_ID_VEHICLE") == 0)) && (_stricmp(ObjClass, "CLASS_TURRET") != 0);
	*/

//	/*
	int ObjClassHash = Hash(ObjClass);
	for(int i = 0; i < sizeof(VehicleClass)/sizeof(VehicleClass[0]); i++)
		if(ObjClassHash == VehicleClass[i])
			return true;

	return false;
//	*/
}

// Is it a Powerup?
bool IsPowerup(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	/*
	char EntityType[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_EntityType, EntityType))
		return false;

	return (_stricmp(EntityType, "CLASS_ID_POWERUP") == 0) || (_stricmp(ObjClass, "CLASS_POWERUP_CAMERA") == 0);
	*/

//	/*
	int ObjClassHash = Hash(ObjClass);
	for(int i = 0; i < sizeof(PowerupClass)/sizeof(PowerupClass[0]); i++)
		if(ObjClassHash == PowerupClass[i])
			return true;

	return false;
//	*/
}

// Is it a Weapon?
bool IsWeapon(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	int ObjClassHash = Hash(ObjClass);
	for(int i = 0; i < sizeof(WeaponClass)/sizeof(WeaponClass[0]); i++)
		if(ObjClassHash == WeaponClass[i])
			return true;

	return false;
}

// Is it something else?
bool IsMisc(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	int ObjClassHash = Hash(ObjClass);
	for(int i = 0; i < sizeof(MiscClass)/sizeof(MiscClass[0]); i++)
		if(ObjClassHash == MiscClass[i])
			return true;

	return false;
}

// Test for common types.
// Is it a scrap?
bool IsScrap(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_SCRAP") == 0);
}
// Is it a turret? (Tank kind, not GunTower kind)
bool IsTurret(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_TURRETTANK") == 0);
}
// Is it a scavenger?
bool IsScavenger(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);
	unsigned int BZCClassHash = Hash(BZCClass);

	return ((_stricmp(ObjClass, "CLASS_SCAVENGER") == 0) || (_stricmp(ObjClass, "CLASS_SCAVENGERH") == 0)) && // Is a Scav
		(BZCClassHash != BZCCLASS_RECYCLER) && (BZCClassHash != BZCCLASS_FACTORY) && (BZCClassHash != BZCCLASS_ARMORY); // Is NOT a BZC Production unit.
}
// Is it a barracks?
bool IsBarracks(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_BARRACKS") == 0);
}
// Is it a gun tower? (The Turret kind)
bool IsGunTower(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_TURRET") == 0);
}
// Is it a Geyser/Pool?
bool IsGeyser(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_DEPOSIT") == 0);
}
// Is it a Spawnpoint?
bool IsSpawnPoint(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_SPAWNBUOY") == 0);
}
// Is it a Silo/Extractor?
bool IsSilo(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_SILO") == 0) || (_stricmp(ObjClass, "CLASS_EXTRACTOR") == 0);
}
// Is it a Factory?
bool IsFactory(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_FACTORY") == 0) || (_stricmp(BZCClass, "muf") == 0); // Look for Factory
}
// Is it an Armory?
bool IsArmory(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_ARMORY") == 0) || (_stricmp(BZCClass, "slf") == 0); // Look for Armory.
}
// Is it a Constructor?
bool IsConstructor(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_CONSTRUCTIONRIG") == 0);
}
/* // Split up into inline functions of the above.
// Is it a Production vehicle/building?
bool IsProducer(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_RECYCLERVEHICLE") == 0) || 
		(_stricmp(ObjClass, "CLASS_RECYCLER") == 0) || 
		(_stricmp(BZCClass, "recy") == 0) || // Look for Recycler too. -GBD
		(_stricmp(ObjClass, "CLASS_FACTORY") == 0) || 
		(_stricmp(BZCClass, "muf") == 0) || // Look for Factory
		(_stricmp(ObjClass, "CLASS_ARMORY") == 0) || 
		(_stricmp(BZCClass, "slf") == 0) || // Look for Armory.
		(_stricmp(ObjClass, "CLASS_CONSTRUCTIONRIG") == 0) || 
		(_stricmp(BZCClass, "cnst") == 0); // Look for Constructor.
}
*/
// Is it a Power Plant?
bool IsPower(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	int PowerCost = 0;
	if(_stricmp(ObjClass, "CLASS_POWERPLANT") == 0)
		GetODFInt(h, "GameObjectClass", "powerCost", &PowerCost, -3);

	return ((_stricmp(BZCClass, "power") == 0) || ((_stricmp(ObjClass, "CLASS_POWERPLANT") == 0) && (PowerCost < 0)));
}
// Is it a Tug?
bool IsTug(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_TUG") == 0);
}
// Is it an APC?
bool IsAPC(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(BZCClass,"apc") == 0) || (_stricmp(ObjClass, "CLASS_APC") == 0);
}
// Is it a Walker?
bool IsWalker(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass, "CLASS_WALKER") == 0);
}
// Is it an Artillary? 
bool IsArtillary(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass,"CLASS_HOWITZER") == 0) || (_stricmp(ObjClass,"CLASS_ARTILLERY") == 0);
}
// Is it a CommTower? BZC, BZ2, or BZ2 other Class?
bool IsCommTower(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(BZCClass,"comm") == 0) || (_stricmp(ObjClass, "CLASS_COMMBUNKER") == 0) || (_stricmp(ObjClass, "CLASS_COMMTOWER") == 0);
}
// Is it a portal? (BZCClasslabel = "portal" or CLASS_TELEPORTAL
bool IsPortal(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(BZCClass,"portal") == 0) || (_stricmp(ObjClass, "CLASS_TELEPORTAL") == 0);
}
// Is it a Fury?
bool IsFury(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_SAV") == 0) || (_stricmp(BZCClass,"fury") == 0);
}
// Is it a Carrier?
bool IsCarrier(const Handle h)
{
	char BZCClass[MAX_ODF_LENGTH] = {0};
	if(!GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass))
		return false;

	return (_stricmp(BZCClass,"carrier") == 0);
}
// Is it a Drone?
bool IsDrone(const Handle h)
{
	char BZCClass[MAX_ODF_LENGTH] = {0};
	if(!GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass))
		return false;

	return (_stricmp(BZCClass,"drone") == 0);
}
// Is it a DropShip? 
bool IsDropShip(const Handle h)
{
	char BZCClass[MAX_ODF_LENGTH] = {0};
	if(!GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass))
		return false;

	return (_stricmp(BZCClass,"dropship") == 0);
}
// Is it an HQCP? 
bool IsHQCP(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_MOTIONSENSOR") == 0) || (_stricmp(BZCClass,"hqcp") == 0);
}
// Is it a Jammer?
bool IsJammer(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_JAMMER") == 0) || (_stricmp(BZCClass,"jamm") == 0);
}
// Is it a Morph Object?
bool IsMorphObject(const Handle h)
{
	char BZCClass[MAX_ODF_LENGTH] = {0};
	if(!GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass))
		return false;

	return (_stricmp(BZCClass,"morphobject") == 0);
}
// Is it a LaunchPad?
bool IsLPad(const Handle h)
{
	char BZCClass[MAX_ODF_LENGTH] = {0};
	if(!GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass))
		return false;

	return (_stricmp(BZCClass,"lpad") == 0);
}
// Is it a Shield?
bool IsShield(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	char BZCClass[MAX_ODF_LENGTH] = {0};
	GetODFString(h, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, BZCClass);

	return (_stricmp(ObjClass, "CLASS_SHIELDTOWER") == 0) || (_stricmp(BZCClass,"shld") == 0);
}
// Is it a Minelayer?
bool IsMinelayer(const Handle h)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;

	return (_stricmp(ObjClass,"CLASS_MINELAYER") == 0);
}

// Is it something that can AlignToTerrain? (Note: We pass in it's Classlabel string here, due to it's first usage.)
bool DefaultAlignToTerrain(const char *ODFName)
{
	if(!ODFName)
		return false;

	char TempODF[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	char Classlabel[MAX_ODF_LENGTH] = {0};

	const char *ext = strrchr(ODFName, '.');
	if(!ext)
		sprintf_s(TempODF, "%s.odf", ODFName); // Add odf to the end.

	if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
	{
		if(DoesODFExist(TempODF2)) // This classlabel points to an odf, it's inherited, go deeper.
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if(!GetODFString(TempODF2, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, Classlabel))
				return false; // No classlabel string in this odf, your ODF is invalid.
		}
	}

	for(int i = 0; i < sizeof(DefaultAlignToTerrainList)/sizeof(DefaultAlignToTerrainList[0]); i++)
		if(_stricmp(Classlabel, DefaultAlignToTerrainList[i]) == 0)
			return true;

	return false;
}

// Gets the Pilot Cost out of an ODF.
int GetPilotCost(const char* ODF)
{
	if(!ODF)
		return 0;

	int TempPilotCost = 0; //Default of 0.
	char PilotODF[MAX_ODF_LENGTH] = {0};
	char PilotODF2[MAX_ODF_LENGTH] = {0};
	strcpy_s(PilotODF, ODF);
	// If this ODF does not exist, it might have been built by a factory, check for that.
	CheckODFName(PilotODF); // strips any .odf off if it already has it.
	const char *ext = strrchr(PilotODF, '.');
	if(!ext)
		strcat_s(PilotODF, ".odf"); // Add .odf to the end.
	// Look for new ODF Parameter pilotCost. 
	if(OpenODF2(PilotODF))
	{
		if(GetODFInt(PilotODF, "GameObjectClass", "pilotCost", &TempPilotCost)) // Grab the value.
		{
			//Got it.
		}
		else // If this ODF is inherited, then try that ODF. If still nothing, then it's nothing.
		{
			if(GetODFString(PilotODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, PilotODF2)) // Grab the classlabel.
			{
				strcat_s(PilotODF2, ".odf"); // Add .odf to the end.
				if((OpenODF2(PilotODF2)) && (GetODFInt(PilotODF2, "GameObjectClass", "pilotCost", &TempPilotCost))) // Grab the value.
				{
					// Got it.
				}
			}
		}
	}
	
	return TempPilotCost;
}

// Gets the number of Weapon Hardpoints an object has.
int GetNumWeapons(const Handle h)
{
	if(!IsAround(h))
		return -1;

	int count = 0;

	for(int i = 0; i < MAX_HARDPOINTS; i++)
	{
		char WeaponHard[MAX_ODF_LENGTH] = {0};
		char DesiredName[MAX_ODF_LENGTH] = {0};
		sprintf_s(DesiredName, "weaponHard%d", i+1);
		if((GetODFString(h, "GameObjectClass", DesiredName, MAX_ODF_LENGTH, WeaponHard)) && 
			((_strnicmp(WeaponHard,"HP_CANN",6) == 0) || // Verify valid Hardpoint types.
			(_strnicmp(WeaponHard,"HP_ROCK",6) == 0) || 
			(_strnicmp(WeaponHard,"HP_MORT",6) == 0) || 
			(_strnicmp(WeaponHard,"HP_SPEC",6) == 0) || 
			(_strnicmp(WeaponHard,"HP_PACK",6) == 0) || 
			(_strnicmp(WeaponHard,"HP_HAND",6) == 0) || 
			(_strnicmp(WeaponHard,"HP_GUN",5) == 0)))
			count++;
	}

	return count;
}

// Gets back the _Config.odf of an object.
char TempGetConfigODFName[MAX_ODF_LENGTH];
const char *GetConfigODF(const Handle h)
{
	memset(TempGetConfigODFName, 0, sizeof(TempGetConfigODFName)); // Clear this before use.

	if(!IsAround(h))
		return NULL;

	const char *ODFName = GetODFName(h);
	sprintf_s(TempGetConfigODFName, "%s_config.odf", ODFName); // Try for ODF_config.odf

	if(!DoesFileExist(TempGetConfigODFName))
	{
		GetODFString(h, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempGetConfigODFName); // Look for it's Classlabel_config.odf first.
		strcat_s(TempGetConfigODFName, "_config.odf");

		if(!DoesFileExist(TempGetConfigODFName)) // If no ODF exists, then it must not be inherited, look for BaseName_config.odf.
		{
			GetODFString(h, "GameObjectClass", "baseName", MAX_ODF_LENGTH, TempGetConfigODFName); // Look for basename_config.odf next.
			strcat_s(TempGetConfigODFName, "_config.odf");

			if(!DoesFileExist(TempGetConfigODFName)) // If no ODF exists, then return NULL.
				return NULL;
				//sprintf_s(TempODFName, "%s_config.odf", ODFName);
		}
	}
	return TempGetConfigODFName;
}

// Gets a random player handle, out of all possible players.
Handle GetRandomPlayer(void) //bool UseCountPlayers)
{
	Handle playerArray[MAX_TEAMS - 1];
	int playerCount = 0;
	for (int team = 1; team < MAX_TEAMS; ++team)
	{
		Handle player = GetPlayerHandle(team);
		if (IsAround(player))
			playerArray[playerCount++] = player;
	}
	if (playerCount > 0)
		return playerArray[GetRandomInt(0, playerCount-1)];

	return 0;
}

// Checks to see if this ODF is already open, then opens it and adds it to the list of currently open odfs. Note: You MUST call CloseOpenODFs() at the end of the mission, in the ~Destructor or PostRun.
bool OpenODF2(const char *name)
{
//	if(!name || !name[0] || name[0] == '.')
//		FormatLogMessage("Found ya!");

	if((!name[0]) || (name[0] == '.'))
		return false;

	// get the CRC hash of the name.
	unsigned long nameHash = CalcCRC(name);

	// check if the name is already open.
	if(ODFNameMap.find(nameHash) != ODFNameMap.end())
		return true; // already open.

	PetWatchdogThread();
	// open the file.
	if(OpenODF(name))
	{
		PetWatchdogThread();
		// add it to the open files.
		strcpy_s(ODFNameMap[nameHash].name, name);
		return true;
	}

	return false; // file not found.
}

// Closes all ODFs opened by OpenODF. Note: This MUST be called at the end of the mission, in the ~Destructor or PostRun().
void CloseOpenODFs(void)
{
	// Close all ODFs we've opened this game.
	for (stdext::hash_map<unsigned long, ODFName>::iterator iter = ODFNameMap.begin(); iter != ODFNameMap.end(); ++iter)
		CloseODF(iter->second.name);

	ODFNameMap.clear();
}

// Custom GetODF functions that OpenODF2 the ODF, and include ODF Inhertience.
int GetODFHexInt(const Handle h, const char* block, const char* name, int* value, const int defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFHexInt(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFHexInt(TempODF2, block, name, value, defval)))
				return true;
		}

	}
	return false;
}
int GetODFInt(const Handle h, const char* block, const char* name, int* value, const int defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFInt(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFInt(TempODF2, block, name, value, defval)))
				return true;
		}

	}
	return false;
}
int GetODFLong(const Handle h, const char* block, const char* name, long* value, const long defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFLong(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && 	(GetODFLong(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFFloat(const Handle h, const char* block, const char* name, float* value, const float defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFFloat(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFFloat(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFDouble(const Handle h, const char* block, const char* name, double* value, const double defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFDouble(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFDouble(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFChar(const Handle h, const char* block, const char* name, char* value, const char defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFChar(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFChar(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFBool(const Handle h, const char* block, const char* name, bool* value, const bool defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFBool(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFBool(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFString(const Handle h, const char* block, const char* name, size_t ValueLen, char* value, const char* defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFString(TempODF, block, name, ValueLen, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFString(TempODF2, block, name, ValueLen, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFColor(const Handle h, const char* block, const char* name, DWORD* value, const DWORD defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFColor(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFColor(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}
int GetODFVector(const Handle h, const char* block, const char* name, Vector* value, const Vector defval)
{
	char TempODF[MAX_ODF_LENGTH] = {0};
	if((!GetObjInfo(h, Get_ODF, TempODF)) || (!block) || (!name))
		return false;

	if(OpenODF2(TempODF))
	{
		if(GetODFVector(TempODF, block, name, value, defval))
			return true;

		char TempODF2[MAX_ODF_LENGTH] = {0};
		if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			if((OpenODF2(TempODF2)) && (GetODFVector(TempODF2, block, name, value, defval)))
				return true;
		}
	}
	return false;
}

// This function gets info from an object's buildItem# list, and returns it's Scrap Cost and ODFName.
bool GetBuildItem(const Handle h, char *ReturnODF, int *ScrapCost, const int Type, const char *Classlabel, const char *ODFName, const char *WeaponName)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
		return false;
	unsigned int ObjClassHash = Hash(ObjClass);

	unsigned int ClasslabelHash = 0;
	unsigned int ODFNameHash = 0;
	unsigned int WeaponNameHash = 0;

	if(Classlabel)
		ClasslabelHash = Hash(Classlabel);

	if(ODFName)
		ODFNameHash = Hash(ODFName);

	if(WeaponName)
		WeaponNameHash = Hash(WeaponName);

	// int Type, bitmask return type. 0 = Any. 1 = Repair, 2 = Reload, 4 = Match Classlabel, 8 = Match ODFName, 16 = Match WeaponName.
	 const int NewType = Type ? Type : -1;

	if((ObjClassHash == CLASS_ARMORY)  || (ObjClassHash == CLASS_FACTORY) || (ObjClassHash == CLASS_RECYCLER) || (ObjClassHash == CLASS_CONSTRUCTIONRIG))
	{
		// Loops through all 10 Item#'s, looking for a the specified type of thing.
		for(int i = 0; i<11;i++)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};

			if(ObjClassHash == CLASS_ARMORY)
			{
				if(i == 0)
					sprintf_s(DesiredValue, "ArmoryClass");
				else
					sprintf_s(DesiredValue, "ArmoryGroup%d", i);
			}
			else
			{
				if(i == 0)
				{
					if(ObjClassHash == CLASS_CONSTRUCTIONRIG)
						sprintf_s(DesiredValue, "ConstructionRigClass");
					else
						sprintf_s(DesiredValue, "FactoryClass");
				}
				else
				{
					sprintf_s(DesiredValue, "BuildGroup%d", i);
				}
			}
			// Loop over all BuildItems.
			for(int x = 0; x<10;x++)
			{
				char TempBuildODF[MAX_ODF_LENGTH] = {0};
				char ItemODF[MAX_ODF_LENGTH] = {0};
				char ItemODF2[MAX_ODF_LENGTH] = {0};
				// Filter info.
				char TempItemClass[MAX_ODF_LENGTH] = {0};
				char TempWeaponName[MAX_ODF_LENGTH] = {0};
			//	int ServiceHealth = 0;
			//	int ServiceAmmo = 0;

				char DesiredValue2[MAX_ODF_LENGTH] = {0};
				sprintf_s(DesiredValue2, "buildItem%d", x+1);
				if((GetODFString(h, DesiredValue, DesiredValue2, MAX_ODF_LENGTH, TempBuildODF)) && (TempBuildODF[0]))
				{
					sprintf_s(ItemODF, "%s.odf", TempBuildODF); // Add .odf to the end.
					if((OpenODF2(ItemODF)) && (GetODFString(ItemODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempItemClass)) && (DoesODFExist(TempItemClass)))
					{
						sprintf_s(ItemODF2, "%s.odf", TempItemClass); // Add .odf to the end.
						if(OpenODF2(ItemODF2)) // If this classlabel ODF does exist, then it's not a true game classlabel.
							GetODFString(ItemODF2, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempItemClass);
					}

					// Pull info used in filters here.
					GetODFString(ItemODF, ItemODF2, "WeaponPowerupClass", "weaponName", MAX_ODF_LENGTH, TempWeaponName);

					/* // Just assume that if it's a service, it gives some bloody service.
					// Get Ammo Amount.
					if((GetODFInt(ItemODF, ItemODF2, "AmmoPowerupClass", "ammoUp", &ServiceAmmo)) || 
						(GetODFInt(ItemODF, ItemODF2, "ServicePowerupClass", "serviceUp", &ServiceAmmo)))
					{
						// Got it.
					}
					// Get Repair Amount.
					if((GetODFInt(ItemODF, ItemODF2, "HealthPowerupClass", "healthUp", &ServiceHealth)) || 
						(GetODFInt(ItemODF, ItemODF2, "ServicePowerupClass", "serviceUp", &ServiceHealth)))
					{
						// Got it.
					}
					*/

					// Filter conditions here.
					if(
						(
						((NewType & 1) == 1) && // Look for a Repair pod.
						((ObjClassHash == CLASS_POWERUP_REPAIR) || (ObjClassHash == CLASS_POWERUP_SERVICE))
						) || 
						(
						((NewType & 2) == 2) && // Look for an Ammo Pod.
						((ObjClassHash == CLASS_POWERUP_RELOAD) || (ObjClassHash == CLASS_POWERUP_SERVICE))
						) || 
						(
						((NewType & 4) == 4) && // Look for a specific Classlabel.
						(Classlabel) && (Hash(TempItemClass) == ClasslabelHash)
						) || 
						(
						((NewType & 8) == 8) && // Look for a specific ODFName.
						(ODFName) && (Hash(TempBuildODF) == ODFNameHash)
						) || 
						(
						((NewType & 16) == 16) && // Look for a specific WeaponName.
						(WeaponName) && (Hash(TempWeaponName) == WeaponNameHash)
						) )
					{
						// Get Scrap Cost.
						GetODFInt(ItemODF, ItemODF2, "GameObjectClass", "scrapCost", ScrapCost);

						// Copy the variable into the string and return true.
						strcpy_s(ReturnODF, sizeof(TempBuildODF), TempBuildODF);
						return true;
					}
				}
			}
		}
	}

	// Returns false if we got here.
	return false;
}

// Replace the weapons in an object. 
bool ReplaceWeapons(const Handle h, const char NewWeapons[][MAX_ODF_LENGTH], const float PreviousLocalAmmo[], const char *PreviousODF)
{
	char ODFName[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(h, Get_ODF, ODFName)) // Get the ODF.
		return false;

	bool Logging = GetVarItemInt("network.session.ivar115");

	char OldODFName[MAX_ODF_LENGTH] = {0};
	strcpy_s(OldODFName, PreviousODF ? PreviousODF : ODFName);

	CheckODFName(OldODFName);

	if(!DoesFileExist(OldODFName))
		strcat_s(OldODFName, ".odf"); // Add .odf to the end.

	char ReplaceWeapons[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	float ReplacementLocalAmmo[MAX_HARDPOINTS] = {0};
	for(int x = 0; x < MAX_HARDPOINTS; x++)
	{
		strcpy_s(ReplaceWeapons[x], NewWeapons[x]);
		if(!PreviousLocalAmmo)
			ReplacementLocalAmmo[x] = 1e6; //1.0f;
		else
			ReplacementLocalAmmo[x] = PreviousLocalAmmo[x];
	}

	bool ContinueWithReplace = true;
	int NumHardPointTypes[MAX_HARDPOINTS] = {0};
	bool BuildWeapons[MAX_HARDPOINTS] = {0};
	char StockWeapon[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	char WeaponClass[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	int WeaponAssault[MAX_HARDPOINTS] = {0};
	int WeaponGroup[MAX_HARDPOINTS] = {0};
	bool UseGroups[MAX_HARDPOINTS] = {0};
	int NewWeaponAssault[MAX_HARDPOINTS] = {0};
	char NewWeaponAltName[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	char StockWeaponClass[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0}; // Second temp odf, for grabbing out classlabel.

	char TempWeaponODF[MAX_ODF_LENGTH] = {0}; // Second temp odf, for grabbing out classlabel.
	char TempWeaponODF2[MAX_ODF_LENGTH] = {0}; // Second temp odf, for grabbing out classlabel.

	if(OpenODF2(OldODFName))
	{
		if(GetODFString(OldODFName, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
		{
			strcat_s(TempODF2, ".odf"); // Add .odf to the end.
			OpenODF2(TempODF2);
		}

		for(int w = 0; w < MAX_HARDPOINTS; w++)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			// Get Weapon Class.
			sprintf_s(DesiredValue, "weaponHard%d", w+1);
			GetODFString(OldODFName, TempODF2, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, WeaponClass[w]);

			// Get Weapon Assault.
			sprintf_s(DesiredValue, "weaponAssault%d", w+1);
			GetODFInt(OldODFName, TempODF2, "GameObjectClass", DesiredValue, &WeaponAssault[w]);

			// Get Weapon Groups.
			sprintf_s(DesiredValue, "weaponGroup%d", w+1);
			if(GetODFInt(OldODFName, TempODF2, "GameObjectClass", DesiredValue, &WeaponGroup[w]))
				UseGroups[w] = true;
		}
	}

	// Look up NewWeapon's isAssault flag.
	for(int w = 0; w < MAX_HARDPOINTS; w++)
	{
		// Read Each Weapon's Assault Flag.
		sprintf_s(TempWeaponODF, "%s.odf", ReplaceWeapons[w]); // Add .odf to the end.
		if(OpenODF2(TempWeaponODF))
		{
			if(GetODFString(TempWeaponODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempWeaponODF2)) // Grab the classlabel.
			{
				strcat_s(TempWeaponODF2, ".odf"); // Add .odf to the end.
				OpenODF2(TempWeaponODF2);
			}

			GetODFInt(TempWeaponODF, TempWeaponODF2, "WeaponClass", "isAssault", &NewWeaponAssault[w]);
			GetODFString(TempWeaponODF, TempWeaponODF2, "WeaponClass", "altName", MAX_ODF_LENGTH, NewWeaponAltName[w]);
		}
	}

	// Look up the STOCK weapons of the ODF we want to use.
	if(OpenODF2(ODFName))
	{
		for(int w = 0; w < MAX_HARDPOINTS; w++)
		{
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "weaponHard%d", w+1);
			GetODFString(h, "GameObjectClass", DesiredValue, MAX_ODF_LENGTH, StockWeaponClass[w]);
		}
	}

	for(int i = 0; i < MAX_HARDPOINTS; i++)
		GetObjInfo(h, ObjectInfoType(Get_Weapon0Config+i), StockWeapon[i]);

//	/* // This code is stupid. What was I thinking? :P -GBD
	for(int c = 0; c < MAX_HARDPOINTS; c++)
	{
		if(_strnicmp(WeaponClass[c]+3,StockWeaponClass[c]+3,4) != 0)
		{
			ContinueWithReplace = false;
			break;
		}
	}
//	*/

	for(int c = 0; c < MAX_HARDPOINTS; c++)
	{
		//for(int s = c; s < MAX_HARDPOINTS; s++)
		for(int s = 0; s < MAX_HARDPOINTS; s++)
		{
			if((_strnicmp(WeaponClass[c]+3,WeaponClass[s]+3,4) == 0) && // Weapon Classes Match.
				(WeaponClass[c][0]) && (WeaponClass[s][0])) // Both Weapons Classes exist.
			{
				bool Skip = false;
				for(int t = 0; t < MAX_HARDPOINTS; t++)
				{
					/*
					SplitWeapons[2][MAX_HARDPOINTS][MAX_ODF_LENGTH] maybe
					[ARM] Ultraken: So, for each entry in NewWeapons, open that ODF, get its altClass, and open that ODF.  Take the altClass ODF, get its isAssault flag, and assign altClass to the corresponding slot in SplitWeapons.  Then take the original ODF, get its isAssault flag, and assign the original to the corresponding slot in SplitWeapons.
					[ARM] Ultraken: You can then use those SplitWeapons indexed by the isAssault of that hardpoint
					[ARM] Ultraken: *shrug*
					[ARM] Ultraken: well, that's what WeaponPowerup does internally
					*/

					if(((!UseGroups[c]) && (!UseGroups[t]) && (_strnicmp(WeaponClass[s]+3,WeaponClass[t]+3,4) == 0)) && // Both are not using groups, and classes match.
						(((WeaponAssault[c] == WeaponAssault[t]) && (NumHardPointTypes[t] > 0) && (t<c)) || // Assault matches, number of hardpoints > 0, and t<c.
						((NewWeaponAssault[t]) && (!NewWeaponAltName[t][0]) && (t>s) && (!WeaponAssault[s]) && (_stricmp(ReplaceWeapons[s], StockWeapon[s]) == 0)))) // Make this skip a previous non assault hardpoint if this weapon is Assault Only.
					{
						if(Logging)
							FormatLogMessage("Assault Matchs, Weapon:%d, CompareWeapon:%d, NumHardTypesS: %d, T<C Skipping S. WeaponClassT: %s", c, t, NumHardPointTypes[t], WeaponClass[t]);
						Skip = true;
						break;
					}
				}
				if(Skip)
					continue;

				if(Logging)
				{
					if(((WeaponGroup[c] != WeaponGroup[s]) && (s<=c)) || ((WeaponGroup[c] == WeaponGroup[s]) && ((UseGroups[c]) || (UseGroups[s])) && (s==c)))
						FormatLogMessage("Using Groups, Groups Dont Match, S<C, or We are Ourselves, Weapon:%d, CompareWeapon:%d", c, s);
					else if((WeaponAssault[c] != WeaponAssault[s]) && (!UseGroups[c]) && (!UseGroups[s]) && (NumHardPointTypes[c] < NumHardPointTypes[s]))
						FormatLogMessage("Assault NOT Matches,Weapon:%d, CompareWeapon:%d, NumHardTypesC: %d < NumHardTypesS: %d", c, s, NumHardPointTypes[c], NumHardPointTypes[s]);
					else if((WeaponAssault[c] == WeaponAssault[s]) && (!UseGroups[c]) && (!UseGroups[s]) && (s>=c)) // && (NumHardPointTypes[s] < 1) && (NumHardPointTypes[c] < 1))
						FormatLogMessage("Assault Matchs, Weapon:%d, CompareWeapon:%d, NumHardTypesS: %d, NumHardTypesC: %d", c, s, NumHardPointTypes[s], NumHardPointTypes[c]);
				}

			}

			if((_strnicmp(WeaponClass[c]+3,WeaponClass[s]+3,4) == 0) && // Weapon Classes Match.
				(WeaponClass[c][0]) && (WeaponClass[s][0]) && // Both Weapons Classes exist.
				// Restrict to, If assault matches, and Group Matches, then restrict to 1, else don't !numhardpointtypes.
				// Works for BZ1 stuff. (Groups)
				((((WeaponGroup[c] != WeaponGroup[s]) && (s<=c)) || ((WeaponGroup[c] == WeaponGroup[s]) && ((UseGroups[c]) || (UseGroups[s])) && (s==c))) || // Both dont match, or we our ourselves. count this as a new group.
				// BZ2 Assault combinations.
				((WeaponAssault[c] != WeaponAssault[s]) && (!UseGroups[c]) && (!UseGroups[s]) && (NumHardPointTypes[c] < NumHardPointTypes[s])) || // Weapon assaults dont match, but group overrides.
				// BZ2 Grouped Combat Weapons. (works for isdf rocket tank).
				((WeaponAssault[c] == WeaponAssault[s]) && (!UseGroups[c]) && (!UseGroups[s]) && (s>=c) && (NumHardPointTypes[c] <= NumHardPointTypes[s]))))//&& (NumHardPointTypes[s] < 1) && (NumHardPointTypes[c] < 1)) // Weapon assaults match and groups match, treat as 1 group.
				++NumHardPointTypes[c];
		}
	}

	/* // Ken's suggestion for optimizing the loop, Didn't work when 1st hp_cann == same 2nd hp_cann == different.
	for (int c = 1; c < MAX_HARDPOINTS; c++)
	{
		 for (int s = 0; s < c; s++)
		 {
	*/
	for(int c = 0; c < MAX_HARDPOINTS; c++)
	{
		for(int s = 0; s < MAX_HARDPOINTS; s++)
		{
			if(Logging)
				FormatLogMessage("Weapon Class C: %d is: %s, Weapon Class S: %d Is: %s", c, WeaponClass[c], s, WeaponClass[s]);

			if((WeaponClass[c] != 0) &&  // Weapon Class is not null.
				(WeaponClass[s] != 0) &&  // Weapon Compare class is not null.
				((c != s) || (_stricmp(ReplaceWeapons[c], StockWeapon[c]) != 0)) && // This is not the same as itself.
				(_strnicmp(WeaponClass[c]+3,WeaponClass[s]+3,4) == 0) && // Weapon Classes Match.
				((_stricmp(ReplaceWeapons[c], StockWeapon[c]) != 0) || // New Weapon does not match Original Weapon.
				((_stricmp(ReplaceWeapons[s], StockWeapon[s]) != 0) && (s > 0)))) // 2nd+ New Weapon doesn't match Original Weapon.
			{
				if(Logging)
					FormatLogMessage("Weapon Class C: %d is: %s, Weapon Class S: %d Is: %s", c, WeaponClass[c], s, WeaponClass[s]);
				BuildWeapons[c] = true;
			}
		}
	}

	if(ContinueWithReplace)
	{
		// Handle the weapon replacement. This must go in reverse order. -GBD
		for(int x = 4; x > -1;x--)
		{
			if(Logging)
				FormatLogMessage("Index: %d Original Weapon: %s, New Weapon: %s, Number of Hardpoints: %d", x, StockWeapon[x], ReplaceWeapons[x], NumHardPointTypes[x]);

			if((ReplaceWeapons[x][0]) && (BuildWeapons[x])) // || (_stricmp(ReplaceWeapons[x], StockWeapon[x]) != 0))
			{
				for(int t = 0; t < NumHardPointTypes[x]; t++)
				{
					GiveWeapon(h, ReplaceWeapons[x]);
					if(Logging)
						FormatLogMessage("Giving Weapon: %s, index: %d", ReplaceWeapons[x], x);
				}
			}
		}
		for(int x = 0; x < MAX_HARDPOINTS; x++)
			SetCurLocalAmmo(h, ReplacementLocalAmmo[x], x);
	}
	return true; // Assume it worked...
}

// Loads several SKY related settings from the map's TRN file.
void LoadSkySettingsFromTRN(void)
{
	// Load Generic Sky Settings.
	float VisibilityMult = 1.0f;
	char DefaultSkyFile[MAX_ODF_LENGTH] = {0};
	strcpy_s(DefaultSkyFile, "BZCDefaultSky.odf");

	// Load any Default settings from here.
	if(OpenODF2(DefaultSkyFile))
	{
		GetODFFloat(DefaultSkyFile, "Sky", "VisibilityMult", &VisibilityMult, 1.0f);
	}

	// Create SKY Settings from map TRN.
	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if(OpenODF2(mapTrnFile))
	{
		// Do Stars.
		////////////////////////////////////////////////////////////////////////////
		char StarTexture[MAX_ODF_LENGTH] = {0};
		int StarCount = 0, StarModulate = 0;
		float StarDistance = 0, StarSpeedX = 0, StarSpeedY = 0, StarSize = 0, StarHeight = 0;
		DWORD StarColor = 0;

		if(GetODFInt(mapTrnFile, "Stars", "Count", &StarCount))
			IFace_FormatConsoleCmd("stars.count %d", StarCount);

		// Depreciated values. Only valid if SkyPointsArePoints = false in map TRN.
		if((GetODFFloat(mapTrnFile, "Stars", "SpeedX", &StarSpeedX)) && (GetODFFloat(mapTrnFile, "Stars", "SpeedY", &StarSpeedY)))
			IFace_FormatConsoleCmd("stars.speed %f %f", StarSpeedX, StarSpeedY);

		if(GetODFFloat(mapTrnFile, "Stars", "Size", &StarSize))
			IFace_FormatConsoleCmd("stars.size %f", StarSize);

		if(GetODFString(mapTrnFile, "Stars", "Texture", MAX_ODF_LENGTH, StarTexture))
			IFace_FormatConsoleCmd("stars.texture \"%s\"", StarTexture);

		if(GetODFFloat(mapTrnFile, "Stars", "Height", &StarHeight))
			IFace_FormatConsoleCmd("stars.height %f", StarHeight);

		if(GetODFColor(mapTrnFile, "Stars", "Color", &StarColor))
			IFace_FormatConsoleCmd("stars.color %d %d %d %d", RGBA_GETRED(StarColor), RGBA_GETGREEN(StarColor), RGBA_GETBLUE(StarColor), RGBA_GETALPHA(StarColor));

		if(GetODFInt(mapTrnFile, "Stars", "Modulate", &StarModulate))
			IFace_FormatConsoleCmd("stars.modulate %d", StarModulate);
		// End Depreciated values.
		
		if(GetODFFloat(mapTrnFile, "Stars", "Distance", &StarDistance))
			IFace_FormatConsoleCmd("stars.distance %f", StarDistance);

		// Do Sun.
		////////////////////////////////////////////////////////////////////////////
		float SunAngle = 123456789.0f, SunPeriod = 24.0f;
		char SunTexture[MAX_ODF_LENGTH] = {0}; 
		Vector SunColor(255, 255, 255);

		if(GetODFString(mapTrnFile, "Sun", "Texture", MAX_ODF_LENGTH, SunTexture))
			IFace_FormatConsoleCmd("sun.texturename \"%s\"", SunTexture);

		if(GetODFVector(mapTrnFile, "Sun", "Color", &SunColor))
			IFace_FormatConsoleCmd("sun.color %f %f %f", SunColor.x, SunColor.y, SunColor.z);

		if(GetODFFloat(mapTrnFile, "Sun", "Angle", &SunAngle, 123456789.0f))
			IFace_FormatConsoleCmd("sun.angle %f", SunAngle);

		if(GetODFFloat(mapTrnFile, "Sun", "Period", &SunPeriod, 24.0f))
			IFace_FormatConsoleCmd("sun.period %f", SunPeriod);

		// Do Dome.
		/////////////////////////////////////////////////////////////////////////////
		int DomeType = 0, DomeAzim = 0;
		float DomeUVSpeedX = 0, DomeUVSpeedY = 0, DomeHeight = 0, DomeRadius = 0, DomeDistance = 0;
		char DomeName[MAX_ODF_LENGTH] = {0};
		Vector DomeAmbient(0, 0, 0);

		if(GetODFInt(mapTrnFile, "Dome", "Type", &DomeType))
			IFace_FormatConsoleCmd("dome.type %d", DomeType);

		if(GetODFInt(mapTrnFile, "Dome", "Azim", &DomeAzim))
			IFace_FormatConsoleCmd("dome.azim %d", DomeAzim);

		if(GetODFVector(mapTrnFile, "Dome", "AmbientColor", &DomeAmbient))
			IFace_FormatConsoleCmd("dome.ambient %f %f %f", DomeAmbient.x, DomeAmbient.y, DomeAmbient.z);

		if((GetODFFloat(mapTrnFile, "Dome", "UVSpeedX", &DomeUVSpeedX)) && (GetODFFloat(mapTrnFile, "Dome", "UVSpeedY", &DomeUVSpeedY)))
			IFace_FormatConsoleCmd("dome.uvspeed %f %f", DomeUVSpeedX, DomeUVSpeedY);

		if(GetODFFloat(mapTrnFile, "Dome", "Height", &DomeHeight))
			IFace_FormatConsoleCmd("dome.height %f", DomeHeight);

		if(GetODFString(mapTrnFile, "Dome", "Name", MAX_ODF_LENGTH, DomeName))
			IFace_FormatConsoleCmd("dome.name \"%s\"", DomeName);

		if(GetODFFloat(mapTrnFile, "Dome", "Radius", &DomeRadius))
			IFace_FormatConsoleCmd("dome.radius %f", DomeRadius);

		if(GetODFFloat(mapTrnFile, "Dome", "Distance", &DomeDistance))
			IFace_FormatConsoleCmd("dome.distance %f", DomeDistance);

		// Do Sky.
		/////////////////////////////////////////////////////////////////////////////
		int SkyModulate = 0, SkyType = 0, SkyFogStart = 100, SkyFogEnd = 250, SkyVisibilityRange = 250;
		float FogBreak = 0, SkyUVSpeedX = 0, SkyUVSpeedY = 0, SkyHeight = 0, SkyTileSize = 0;
		char SkyTextureName[MAX_ODF_LENGTH] = {0};
		Vector SkyAmbientColor(127, 127, 127), SkyFogColor(0, 0, 0);
		DWORD SkyColor = 0;

		// Depreciated.
		if(GetODFFloat(mapTrnFile, "Sky", "FogBreak", &FogBreak))
			IFace_FormatConsoleCmd("sky.fogbreak %f", FogBreak*VisibilityMult);

		if(GetODFInt(mapTrnFile, "Sky", "Type", &SkyType))
			IFace_FormatConsoleCmd("sky.type %d", SkyType);

		if(GetODFString(mapTrnFile, "Sky", "TextureName", MAX_ODF_LENGTH, SkyTextureName))
		{
			IFace_FormatConsoleCmd("sky.texturename \"%s\"", SkyTextureName);

			// If there's a sky texture set, do this, otherwise set color to invisible.
			if((SkyTextureName[0]) && (GetODFColor(mapTrnFile, "Sky", "Color", &SkyColor)))
				IFace_FormatConsoleCmd("sky.color %d %d %d %d", RGBA_GETRED(SkyColor), RGBA_GETGREEN(SkyColor), RGBA_GETBLUE(SkyColor), RGBA_GETALPHA(SkyColor));
			else
				IFace_FormatConsoleCmd("sky.color 0 0 0 0");
		}

		if(GetODFVector(mapTrnFile, "Sky", "AmbientColor", &SkyAmbientColor))
			IFace_FormatConsoleCmd("sky.ambientcolor %f %f %f", SkyAmbientColor.x, SkyAmbientColor.y, SkyAmbientColor.z);

		if((GetODFFloat(mapTrnFile, "Sky", "UVSpeedX", &SkyUVSpeedX)) && (GetODFFloat(mapTrnFile, "Sky", "UVSpeedY", &SkyUVSpeedY)))
			IFace_FormatConsoleCmd("sky.uvspeed %f %f", SkyUVSpeedX, SkyUVSpeedY);

		if(GetODFFloat(mapTrnFile, "Sky", "Height", &SkyHeight))
			IFace_FormatConsoleCmd("sky.height %f", SkyHeight);

		if(GetODFInt(mapTrnFile, "Sky", "VisibilityRange", &SkyVisibilityRange))
			IFace_FormatConsoleCmd("sky.visibilityrange %f", SkyVisibilityRange*VisibilityMult);

		if(GetODFFloat(mapTrnFile, "Sky", "TileSize", &SkyTileSize))
			IFace_FormatConsoleCmd("sky.tilesize %f", SkyTileSize);

		if((GetODFInt(mapTrnFile, "Sky", "FogStart", &SkyFogStart)) && (GetODFInt(mapTrnFile, "Sky", "FogEnd", &SkyFogEnd)))
			IFace_FormatConsoleCmd("sky.fogrange %f %f", SkyFogStart*VisibilityMult, SkyFogEnd*VisibilityMult);

		if(GetODFVector(mapTrnFile, "Sky", "FogColor", &SkyFogColor))
			IFace_FormatConsoleCmd("sky.fogcolor %f %f %f", SkyFogColor.x, SkyFogColor.y, SkyFogColor.z);

		if(GetODFInt(mapTrnFile, "Sky", "Modulate", &SkyModulate))
			IFace_FormatConsoleCmd("sky.modulate %d", SkyModulate);

		// Nope.avi. sky.toggle you must set manually because it's a toggle.
	//	if(GetODFInt(mapTrnFile, "Sky", "SkyToggleMask", &SkyToggle))
	//	{
	//		// Toggle, reads in a Bitmask and Set matching values.
	//		if((SkyToggle & 1) == 1) // Dome
	//		if((SkyToggle & 2) == 2) // Stars
	//		if((SkyToggle & 4) == 4) // Flat
	//		if((SkyToggle & 8) == 8) // Clouds
	//		if((SkyToggle & 16) == 16) // Sprites
	//		if((SkyToggle & 32) == 32) // Sun
	//	}

		// Do Sky Bolt.
		/////////////////////////////////////////////////////////////////////////////
		int BoltEnable = 0, BoltBolts = 0;
		float BoltDuration = 0, BoltMinDelay = 0, BoltMaxDelay = 0, BoltStartWidth = 0, BoltDistance = 0;
		char BoltTextureName[MAX_ODF_LENGTH] = {0}, BoltSound1[MAX_ODF_LENGTH] = {0}, BoltSound2[MAX_ODF_LENGTH] = {0};

		if(GetODFInt(mapTrnFile, "Bolt", "Enable", &BoltEnable))
			IFace_FormatConsoleCmd("sky.bolt.enable %d", BoltEnable);

		// Cannot seem to be > 1? :(
		if(GetODFInt(mapTrnFile, "Bolt", "Bolts", &BoltBolts))
			IFace_FormatConsoleCmd("sky.bolt.bolts %d", BoltBolts);

		if(GetODFFloat(mapTrnFile, "Bolt", "Duration", &BoltDuration))
			IFace_FormatConsoleCmd("sky.bolt.duration %f", BoltDuration);

		if(GetODFFloat(mapTrnFile, "Bolt", "MinDelay", &BoltMinDelay))
			IFace_FormatConsoleCmd("sky.bolt.mindelay %f", BoltMinDelay);

		if(GetODFFloat(mapTrnFile, "Bolt", "MaxDelay", &BoltMaxDelay))
			IFace_FormatConsoleCmd("sky.bolt.maxdelay %f", BoltMaxDelay);

		if(GetODFFloat(mapTrnFile, "Bolt", "StartWidth", &BoltStartWidth))
			IFace_FormatConsoleCmd("sky.bolt.startwidth %f", BoltStartWidth);

		if(GetODFFloat(mapTrnFile, "Bolt", "Distance", &BoltDistance))
			IFace_FormatConsoleCmd("sky.bolt.distance %f", BoltDistance);

		if(GetODFString(mapTrnFile, "Bolt", "Texture", MAX_ODF_LENGTH, BoltTextureName))
			IFace_FormatConsoleCmd("sky.bolt.texture \"%s\"", BoltTextureName);

		if(GetODFString(mapTrnFile, "Bolt", "Sound1", MAX_ODF_LENGTH, BoltSound1))
			IFace_FormatConsoleCmd("sky.bolt.sound1 \"%s\"", BoltSound1);

		if(GetODFString(mapTrnFile, "Bolt", "Sound2", MAX_ODF_LENGTH, BoltSound2))
			IFace_FormatConsoleCmd("sky.bolt.sound2 \"%s\"", BoltSound2);

		// Do Fog.
		/////////////////////////////////////////////////////////////////////////////
		Vector FogHeight(0, 0, 0);

		// Depreciated value.
		if(GetODFVector(mapTrnFile, "Fog", "FogHeight", &FogHeight))
			IFace_FormatConsoleCmd("fog.height %f %f %f", FogHeight.x, FogHeight.y, FogHeight.z);

		// Nope, this method is not very possible because there is no command to Delete a fog once you type fog.new, and no fog selection index way to make a new one.
		/*
		for(int i = 0; i < MAX_LOCAL_FOGVOLUMES; i++)
		{
			float FogDensity = 0;
			Vector FogPosition(0, 0, 0), FogRadius(0, 0, 0);

			IFace_ConsoleCmd("fog.new 0 0 0 0 0 0 -1"); // Initialize the fog. This doesn't actually make it visible, just initializes it for setting below.
			IFace_FormatConsoleCmd("fog.select %d", i); // Select the Fog we just made.

			char DesiredValue[MAX_ODF_LENGTH] = {0};

			sprintf_s(DesiredValue, "LocalFogDensity%d", i+1);
			if(GetODFFloat(mapTrnFile, "Fog", DesiredValue, &FogDensity, -1.0f))
				IFace_FormatConsoleCmd("fog.density %f", FogDensity);
			else
				IFace_FormatConsoleCmd("fog.density %f", -1.0f);

			sprintf_s(DesiredValue, "LocalFogPosition%d", i+1);
			if(GetODFVector(mapTrnFile, "Fog", DesiredValue, &FogPosition))
				IFace_FormatConsoleCmd("fog.position %f %f %f", FogPosition.x, FogPosition.y, FogPosition.z);
			else
				IFace_FormatConsoleCmd("fog.position %f %f %f", 0.0f, -1000.0f, 0.0f);

			sprintf_s(DesiredValue, "LocalFogRadius%d", i+1);
			if(GetODFVector(mapTrnFile, "Fog", DesiredValue, &FogRadius))
				IFace_FormatConsoleCmd("fog.radius %f %f %f", FogRadius.x, FogRadius.y, FogRadius.z);
			else
				IFace_FormatConsoleCmd("fog.radius %f %f %f", 0.0f, 0.0f, 0.0f);
		}
		*/

		// Do Water Fog.
		/////////////////////////////////////////////////////////////////////////////
		int WaterEnable = 0;
		float WaterDepth = 0;
		Vector WaterFogTopColor(0, 0, 0), WaterFogBottomColor(0, 0, 0), WaterFogVisibility(0, 0, 0);

		if(GetODFInt(mapTrnFile, "WaterFog", "Enable", &WaterEnable))
			IFace_FormatConsoleCmd("fog.water.enable %d", WaterEnable);

		if((GetODFVector(mapTrnFile, "WaterFog", "TopColor", &WaterFogTopColor)) && (GetODFVector(mapTrnFile, "WaterFog", "BottomColor", &WaterFogBottomColor)))
			IFace_FormatConsoleCmd("fog.water.color %f %f %f %f %f %f", SkyAmbientColor.x, SkyAmbientColor.y, SkyAmbientColor.z);

		//if(GetODFVector(mapTrnFile, "WaterFog", "Visibility", &WaterFogVisibility))
		//	IFace_ConsoleCmd("fog.water.visibility %f %f %f", WaterFogVisibility.x, WaterFogVisibility.y, WaterFogVisibility.z);
		if((GetODFFloat(mapTrnFile, "WaterFog", "VisibilityTop", &WaterFogVisibility.x)) && (GetODFFloat(mapTrnFile, "WaterFog", "VisibilityBottom", &WaterFogVisibility.y)) && (GetODFFloat(mapTrnFile, "WaterFog", "FogBreak", &WaterFogVisibility.z)))
			IFace_FormatConsoleCmd("fog.water.visibility %f %f %f", WaterFogVisibility.x, WaterFogVisibility.y, WaterFogVisibility.z);

		if(GetODFFloat(mapTrnFile, "WaterFog", "Depth", &WaterDepth))
			IFace_FormatConsoleCmd("fog.water.depth %f", WaterDepth);

		// Do Mirror.
		/////////////////////////////////////////////////////////////////////////////
		int MirrorEnable = 0, MirrorTexture = 0;
		float MirrorOffset = 0;

		if(GetODFInt(mapTrnFile, "Mirror", "Enable", &MirrorEnable))
			IFace_FormatConsoleCmd("mirror.enable %d", MirrorEnable);

		if(GetODFInt(mapTrnFile, "Mirror", "TextureMode", &MirrorTexture))
			IFace_FormatConsoleCmd("mirror.texture %d", MirrorTexture);

		if(GetODFFloat(mapTrnFile, "Mirror", "Offset", &MirrorOffset))
			IFace_FormatConsoleCmd("mirror.offset %f", MirrorOffset);

		// Do Sprites. Loop over all 64 potential ones.
		/////////////////////////////////////////////////////////////////////////////
		float SpriteHeight = 0, SpriteDistance = 0;

		if(GetODFFloat(mapTrnFile, "Sprites", "Distance", &SpriteDistance))
			IFace_FormatConsoleCmd("sprites.distance %f", SpriteDistance);

		if(GetODFFloat(mapTrnFile, "Sprites", "Height", &SpriteHeight))
			IFace_FormatConsoleCmd("sprites.height %f", SpriteHeight);

		for(int i = 0; i < MAX_SPRITES; i++)
		{
			int SpriteModulate = 0;
			float SpriteAzim = 0, SpriteSize = 0, SpriteRoll = 0, SpriteElev = 0;
			DWORD SpriteColor = 0;
			char SpriteTextureName[MAX_ODF_LENGTH] = {0};

			IFace_FormatConsoleCmd("sprites.select %d", i);

			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "TextureName%d", i+1);
			if(GetODFString(mapTrnFile, "Sprites", DesiredValue, MAX_ODF_LENGTH, SpriteTextureName))
			{
				IFace_FormatConsoleCmd("sprites.texturename \"%s\"", SpriteTextureName);

				sprintf_s(DesiredValue, "Size%d", i+1);
				// If there's no specified sprite name, set size to 0.
				if((SpriteTextureName[0]) && (GetODFFloat(mapTrnFile, "Sprites", DesiredValue, &SpriteSize)))
					IFace_FormatConsoleCmd("sprites.size %f", SpriteSize);
				else
					IFace_FormatConsoleCmd("sprites.size 0");
			}

			sprintf_s(DesiredValue, "Azim%d", i+1);
			if(GetODFFloat(mapTrnFile, "Sprites", DesiredValue, &SpriteAzim))
				IFace_FormatConsoleCmd("sprites.azim %f", SpriteAzim);

			sprintf_s(DesiredValue, "Roll%d", i+1);
			if(GetODFFloat(mapTrnFile, "Sprites", DesiredValue, &SpriteRoll))
				IFace_FormatConsoleCmd("sprites.roll %f", SpriteRoll);

			sprintf_s(DesiredValue, "Color%d", i+1);
			if(GetODFColor(mapTrnFile, "Sprites", DesiredValue, &SpriteColor))
				IFace_FormatConsoleCmd("sprites.color %d %d %d %d", RGBA_GETRED(SpriteColor), RGBA_GETGREEN(SpriteColor), RGBA_GETBLUE(SpriteColor), RGBA_GETALPHA(SpriteColor));

			sprintf_s(DesiredValue, "Elev%d", i+1);
			if(GetODFFloat(mapTrnFile, "Sprites", DesiredValue, &SpriteElev))
			{
				IFace_FormatConsoleCmd("sprites.elev %f", SpriteElev);
			} // Attempt to Auto compute it based on Sun Angle if it is BZCSun.tga.
			else if((SunAngle != 123456789.0f) && (_strnicmp(SpriteTextureName,"BZCSun", 6) == 0))
			{
				SpriteElev = fmodf(360.0f * (SunAngle / SunPeriod), 360.0f) - 90.0f;
				//SpriteElev = 360.0f * fmodf((SunAngle + m_TurnCounter / (3600 * m_gameTPS)) / SunPeriod, 1.0f) - 90.0f;
				//SpriteElev = (SunAngle - 6.0f) * 15.0f;
				//FormatLogMessage("Auto-Calculated SunSprite Elevation is: %f", SpriteElev);
				IFace_FormatConsoleCmd("sprites.elev %f", SpriteElev);
			}

			sprintf_s(DesiredValue, "Modulate%d", i+1);
			if(GetODFInt(mapTrnFile, "Sprites", DesiredValue, &SpriteModulate))
				IFace_FormatConsoleCmd("sprites.modulate %d", SpriteModulate);
		}
		// Reset Sprite selection.
		IFace_FormatConsoleCmd("sprites.select 0");

		// Do Rain.
		/////////////////////////////////////////////////////////////////////////////
		int RainEnable = 0, RainModulate = 0, RainCountX = 0, RainCountY = 0, RainCountEach = 0;
		float RainSizeX = 0, RainSizeY = 0, RainTranPeriod = 0, RainOnPeriod = 0, RainOffPeriod = 0, RainDirection = 0, RainSpeed = 0, RainBlockSize = 0, RainMinDist = 0, RainScaleFactor = 0, RainCutDist = 0;
		char RainTexture[MAX_ODF_LENGTH] = {0};
		DWORD RainColor = 0, RainDomeColor = 0;

		if(GetODFInt(mapTrnFile, "Rain", "Enable", &RainEnable))
			IFace_FormatConsoleCmd("rain.enable %d", RainEnable);

		if(GetODFString(mapTrnFile, "Rain", "Texture", MAX_ODF_LENGTH, RainTexture))
		{
			IFace_FormatConsoleCmd("rain.texture \"%s\"", RainTexture);

			// If no texture is set, set size to 0.
			if((RainTexture[0]) && (GetODFFloat(mapTrnFile, "Rain", "SizeX", &RainSizeX)) && (GetODFFloat(mapTrnFile, "Rain", "SizeY", &RainSizeY)))
				IFace_FormatConsoleCmd("rain.size %f %f", RainSizeX, RainSizeY);
			else
				IFace_FormatConsoleCmd("rain.size 0 0");
		}

		if((GetODFInt(mapTrnFile, "Rain", "CountX", &RainCountX)) && (GetODFInt(mapTrnFile, "Rain", "CountY", &RainCountY)) && (GetODFInt(mapTrnFile, "Rain", "CountEach", &RainCountEach)))
			IFace_FormatConsoleCmd("rain.count %d %d %d", RainCountX, RainCountY, RainCountEach);

		if(GetODFFloat(mapTrnFile, "Rain", "OnPeriod", &RainOnPeriod))
			IFace_FormatConsoleCmd("rain.onperiod %f", RainOnPeriod);

		if(GetODFFloat(mapTrnFile, "Rain", "OffPeriod", &RainOffPeriod))
			IFace_FormatConsoleCmd("rain.offperiod %f", RainOffPeriod);

		if(GetODFFloat(mapTrnFile, "Rain", "TranPeriod", &RainTranPeriod))
			IFace_FormatConsoleCmd("rain.tranperiod %f", RainTranPeriod);

		if(GetODFFloat(mapTrnFile, "Rain", "Direction", &RainDirection))
			IFace_FormatConsoleCmd("rain.direction %f", RainDirection);

		if(GetODFFloat(mapTrnFile, "Rain", "Speed", &RainSpeed))
			IFace_FormatConsoleCmd("rain.speed %f", RainSpeed);

		if(GetODFFloat(mapTrnFile, "Rain", "BlockSize", &RainBlockSize))
			IFace_FormatConsoleCmd("rain.blocksize %f", RainBlockSize);

		if(GetODFFloat(mapTrnFile, "Rain", "MinDist", &RainMinDist))
			IFace_FormatConsoleCmd("rain.mindist %f", RainMinDist);

		if(GetODFFloat(mapTrnFile, "Rain", "ScaleFactor", &RainScaleFactor))
			IFace_FormatConsoleCmd("rain.scalefactor %f", RainScaleFactor);

		if(GetODFFloat(mapTrnFile, "Rain", "CutDist", &RainCutDist))
			IFace_FormatConsoleCmd("rain.cutdist %f", RainCutDist);

		if(GetODFColor(mapTrnFile, "Rain", "Color", &RainColor))
			IFace_FormatConsoleCmd("rain.color %d %d %d %d", RGBA_GETRED(RainColor), RGBA_GETGREEN(RainColor), RGBA_GETBLUE(RainColor), RGBA_GETALPHA(RainColor));

		if(GetODFColor(mapTrnFile, "Rain", "DomeColor", &RainDomeColor))
			IFace_FormatConsoleCmd("rain.domecolor %d %d %d %d", RGBA_GETRED(RainDomeColor), RGBA_GETGREEN(RainDomeColor), RGBA_GETBLUE(RainDomeColor), RGBA_GETALPHA(RainDomeColor));

		if(GetODFInt(mapTrnFile, "Rain", "Modulate", &RainModulate))
			IFace_FormatConsoleCmd("rain.modulate %d", RainModulate);

		// Do Rain Splat.
		/////////////////////////////////////////////////////////////////////////////
		int SplatBlocksize = 0;

		if(GetODFInt(mapTrnFile, "Splat", "Blocksize", &SplatBlocksize))
			IFace_FormatConsoleCmd("splat.blocksize %d", SplatBlocksize);

		// Do Ground Splat
		/////////////////////////////////////////////////////////////////////////////
		int SplatGroundModulate = 0, SplatGroundCount = 0;
		float SlatGroundLifeTime = 0, SplatGroundStartWidth = 0, SplatGroundStartHeight = 0, SplatGroundEndWidth = 0, SplatGroundEndHeight = 0;
		char SplatGroundTexture[MAX_ODF_LENGTH] = {0};
		DWORD SplatGroundColor = 0;

		if(GetODFInt(mapTrnFile, "Splat", "GroundCount", &SplatGroundCount))
			IFace_FormatConsoleCmd("splat.ground.count %d", SplatGroundCount);

		if(GetODFFloat(mapTrnFile, "Splat", "GroundLifeTime", &SlatGroundLifeTime))
			IFace_FormatConsoleCmd("splat.ground.lifetime %f", SlatGroundLifeTime);

		if((GetODFFloat(mapTrnFile, "Splat", "GroundStartWidth", &SplatGroundStartWidth)) && (GetODFFloat(mapTrnFile, "Splat", "GroundStartHeight", &SplatGroundStartHeight)) && (GetODFFloat(mapTrnFile, "Splat", "GroundEndWidth", &SplatGroundEndWidth)) && (GetODFFloat(mapTrnFile, "Splat", "GroundEndHeight", &SplatGroundEndHeight)))
			IFace_FormatConsoleCmd("splat.ground.size %f %f %f %f", SplatGroundStartWidth, SplatGroundStartHeight, SplatGroundEndWidth, SplatGroundEndHeight);

		if(GetODFString(mapTrnFile, "Splat", "GroundTexture", MAX_ODF_LENGTH, SplatGroundTexture))
			IFace_FormatConsoleCmd("splat.ground.texture \"%s\"", SplatGroundTexture);

		if(GetODFColor(mapTrnFile, "Splat", "GroundColor", &SplatGroundColor))
			IFace_FormatConsoleCmd("splat.ground.color %d %d %d %d", RGBA_GETRED(SplatGroundColor), RGBA_GETGREEN(SplatGroundColor), RGBA_GETBLUE(SplatGroundColor), RGBA_GETALPHA(SplatGroundColor));

		if(GetODFInt(mapTrnFile, "Splat", "GroundModulate", &SplatGroundModulate))
			IFace_FormatConsoleCmd("splat.ground.modulate %d", SplatGroundModulate);

		// Do Water Splat
		/////////////////////////////////////////////////////////////////////////////
		int SplatWaterModulate = 0, SplatWaterCount = 0;
		float SlatWaterLifeTime = 0, SplatWaterStartWidth = 0, SplatWaterStartHeight = 0, SplatWaterEndWidth = 0, SplatWaterEndHeight = 0;
		char SplatWaterTexture[MAX_ODF_LENGTH] = {0};
		DWORD SplatWaterColor = 0;

		if(GetODFInt(mapTrnFile, "Splat", "WaterCount", &SplatWaterCount))
			IFace_FormatConsoleCmd("splat.water.count %d", SplatWaterCount);

		if(GetODFFloat(mapTrnFile, "Splat", "WaterLifeTime", &SlatWaterLifeTime))
			IFace_FormatConsoleCmd("splat.water.lifetime %f", SlatWaterLifeTime);

		if((GetODFFloat(mapTrnFile, "Splat", "WaterStartWidth", &SplatWaterStartWidth)) && (GetODFFloat(mapTrnFile, "Splat", "WaterStartHeight", &SplatWaterStartHeight)) && (GetODFFloat(mapTrnFile, "Splat", "WaterEndWidth", &SplatWaterEndWidth)) && (GetODFFloat(mapTrnFile, "Splat", "WaterEndHeight", &SplatWaterEndHeight)))
			IFace_FormatConsoleCmd("splat.water.size %f %f %f %f", SplatWaterStartWidth, SplatWaterStartHeight, SplatWaterEndWidth, SplatWaterEndHeight);

		if(GetODFString(mapTrnFile, "Splat", "WaterTexture", MAX_ODF_LENGTH, SplatWaterTexture))
			IFace_FormatConsoleCmd("splat.water.texture \"%s\"", SplatWaterTexture);

		if(GetODFColor(mapTrnFile, "Splat", "WaterColor", &SplatWaterColor))
			IFace_FormatConsoleCmd("splat.water.color %d %d %d %d", RGBA_GETRED(SplatWaterColor), RGBA_GETGREEN(SplatWaterColor), RGBA_GETBLUE(SplatWaterColor), RGBA_GETALPHA(SplatWaterColor));

		if(GetODFInt(mapTrnFile, "Splat", "WaterModulate", &SplatWaterModulate))
			IFace_FormatConsoleCmd("splat.water.modulate %d", SplatWaterModulate);

		// Do Windshield Splat
		/////////////////////////////////////////////////////////////////////////////
		int SplatWindshieldModulate = 0, SplatWindshieldCount = 0;
		float SlatWindshieldLifeTime = 0, SplatWindshieldStartWidth = 0, SplatWindshieldStartHeight = 0, SplatWindshieldEndWidth = 0, SplatWindshieldEndHeight = 0;
		char SplatWindshieldTexture[MAX_ODF_LENGTH] = {0};
		DWORD SplatWindshieldColor = 0;

		if(GetODFInt(mapTrnFile, "Splat", "WindshieldCount", &SplatWindshieldCount))
			IFace_FormatConsoleCmd("splat.windshield.count %d", SplatWindshieldCount);

		if(GetODFFloat(mapTrnFile, "Splat", "WindshieldLifeTime", &SlatWindshieldLifeTime))
			IFace_FormatConsoleCmd("splat.windshield.lifetime %f", SlatWindshieldLifeTime);

		if((GetODFFloat(mapTrnFile, "Splat", "WindshieldStartWidth", &SplatWindshieldStartWidth)) && (GetODFFloat(mapTrnFile, "Splat", "WindshieldStartHeight", &SplatWindshieldStartHeight)) && (GetODFFloat(mapTrnFile, "Splat", "WindshieldEndWidth", &SplatWindshieldEndWidth)) && (GetODFFloat(mapTrnFile, "Splat", "WindshieldEndHeight", &SplatWindshieldEndHeight)))
			IFace_FormatConsoleCmd("splat.windshield.size %f %f %f %f", SplatWindshieldStartWidth, SplatWindshieldStartHeight, SplatWindshieldEndWidth, SplatWindshieldEndHeight);

		if(GetODFString(mapTrnFile, "Splat", "WindshieldTexture", MAX_ODF_LENGTH, SplatWindshieldTexture))
			IFace_FormatConsoleCmd("splat.windshield.texture \"%s\"", SplatWindshieldTexture);

		if(GetODFColor(mapTrnFile, "Splat", "WindshieldColor", &SplatWindshieldColor))
			IFace_FormatConsoleCmd("splat.windshield.color %d %d %d %d", RGBA_GETRED(SplatWindshieldColor), RGBA_GETGREEN(SplatWindshieldColor), RGBA_GETBLUE(SplatWindshieldColor), RGBA_GETALPHA(SplatWindshieldColor));

		if(GetODFInt(mapTrnFile, "Splat", "WindshieldModulate", &SplatWindshieldModulate))
			IFace_FormatConsoleCmd("splat.windshield.modulate %d", SplatWindshieldModulate);
	}
}