#ifndef _BZCScriptUtils_
#define _BZCScriptUtils_

#include "..\..\source\fun3d\ScriptUtils.h"
#include "ScriptUtilsExtension.h"
#include "MathUtils.h"
#include <math.h>
#include <vector>
#include <hash_map>
#include <algorithm>
#include <limits.h>
#include <float.h>
#include <ctype.h>

#define MAX_TAPS 8 // Max number of vehicle taps.
#define MAX_LAVA_FIELDS 64 // Max number of lava fields.
#define MAX_CLOUD_TYPES 16 // Max Clouds to possibly look at. Double the BZ1 value, yay! -GBD
#define MAX_CLOUD_OBJECTS 64 // Max Clouds per Tile, Double the BZ1 value, yay? -GBD
#define MAX_ODF_LENGTH 64 // Max ODF Length.
#define GLOBAL_MULT 0.8f // Scale for things. Put here to quickly change it.
#define MAX_SHIELDS 16 // Max number of Shield Emitters.
#define MAX_LOCAL_FOGVOLUMES 16 // Max Local Fog volumes.
#define MAX_SPRITES 64 // Max Sprites.
#define PI 3.141592654f // Because Pie are Square.
#define DEG_2_RAD 0.0174532925222222f // Degrees to Radians.
#define RAD_2_DEG 57.2957877f // Radians to Degrees.
#define MAX_MINES 64 // number of mines to track. SP only.
#define MAX_DROPSHIP_CARGO 10 // Number of ships that can be carried in a dropship.
#define MAX_MESSAGE_LENGTH 1024 // Max Length of a message string.
#define MAX_HARDPOINTS 5 // This is the max number of hardpoints a ship can have. Maybe it'll go up some day? :)
#define MAX_GRAVITY_TYPES 16 // Max number of Gravity types to Cycle through.
#define INVALID_VALUE -1234 // The code returned by the game on Invalid Values, such as GetMaxHealth() of a killed thing in DeleteObject.

typedef int PilotValue;


// Difficulty Types.
enum Difficulty 
{
	Easy,
	Normal,
	Hard,
	Extreme,
};

// Game Types. //ivar5, All=0, DM=1, Strat=2, all other values undefined (used in filters)
enum GameTypes 
{
	GameType_ALL,
	GameType_DM,
	GameType_ST,
};

// Portal Color FXNumber Enum.
enum PortalEffect
{
	PortalBlue,
	PortalRed,
	PortalGreen,
	PortalWhite,
};

// Portal Type Enum.
enum PortalType
{
	PortalNone,
	PortalOn,
	PortalBuild,
};

// TeamFilter Enum
enum TeamFilter_Type
{
	AllTeamFilters = 0,
	SameTeam = 1,
	Allies = 2,
	Enemies = 4,
};

// CategoryType Enum
enum Category_Type
{
	AllCategories = 0,
	Vehicle = 1,
	Building = 2,
	Person = 4,
	Powerup = 8,
	Weapon = 16,
	Scrap = 32,
	Other = 64,
};

// Power Types.
enum PowerTypes
{
	PType_None = 0,
	PType_BZ1 = 1,
	PType_BZ2 = 2,
	PType_Lung = 4,
};

// 1.3 Patch Versions.
enum Patch_Names
{
	vStock = 0, // Assume 0 means Default, base game 1.0.
	v1012 = 1012, // 1.01b2.
	v1200 = 1200, // 1.2
	v13PB1 = 1310, // 1.3 pb1.
	v13PB2 = 1320, // 1.3 pb2.
	v13PB3 = 1330, // 1.3 pb3.
	v13PB4 = 1340, // 1.3 pb4a.
	v13PB5 = 1350, // 1.3 ta5.
	v13PB51 = 1351, // 1.3 pb5.1.
	v13PB6 = 1360, // 1.3 pb6.0.
	v13PB61 = 1361, // 1.3 pb6.1.
	v13PB62 = 1362, // 1.3 pb6.2.
	v13PB63 = 1363, // 1.3 pb6.3.
	v13PB64 = 1364, // 1.3 pb6.4.
	v13PB65 = 1365, // 1.3 pb6.5.
	v13PB7 = 1370, // 1.3 pb7.
};

// FNV-1a Hash's of Get_EntityType strings, for faster comparisons.
enum Hash_EntityType
{
	CLASS_ID_NONE = 0x08ade38c,			//	"CLASS_ID_NONE",		// undefined object (default)
	CLASS_ID_CRAFT = 0xf2070008,		// 	"CLASS_ID_CRAFT",		// hovering vehicle
	CLASS_ID_VEHICLE = 0x27d612aa,		// 	"CLASS_ID_VEHICLE",		// tracked vehicle
	CLASS_ID_TORPEDO = 0xfcbb9395,		// 	"CLASS_ID_TORPEDO",		// miniature vehicle
	CLASS_ID_POWERUP = 0x8f2b1662,		// 	"CLASS_ID_POWERUP",		// power up pod
	CLASS_ID_PERSON = 0xa7a2704b,		// 	"CLASS_ID_PERSON",		// pilot or soldier
	CLASS_ID_ANIMAL = 0x2988eee2,		// 	"CLASS_ID_ANIMAL",		// animal
	CLASS_ID_STRUCT = 0xfea184f3,		// 	"CLASS_ID_STRUCT",		// generic building
	CLASS_ID_BUILDING = 0xc07bf472,		// 	"CLASS_ID_BUILDING",	// important building
	CLASS_ID_SIGN = 0x1f71f513,			// 	"CLASS_ID_SIGN",		// mine object
	CLASS_ID_SCRAP = 0xaff3780b,		// 	"CLASS_ID_SCRAP",		// scrap piece
	CLASS_ID_DEPOSIT = 0xe120e5e8,		// 	"CLASS_ID_DEPOSIT",		// scrap deposit
	CLASS_ID_BEACON = 0x4c47d522,		// 	"CLASS_ID_BEACON",		// nav beacon
	CLASS_ID_PLANT = 0x6c6effbb,		// 	"CLASS_ID_PLANT",		// plant object
	CLASS_ID_TERRAIN = 0xb6789db7,		// 	"CLASS_ID_TERRAIN",		// terrain object
	CLASS_ID_WEAPON = 0xefa88fd6,		// 	"CLASS_ID_WEAPON",		// weapon object
	CLASS_ID_ORDNANCE = 0x5ac64c32,		// 	"CLASS_ID_ORDNANCE",	// ordnance object
	CLASS_ID_EXPLOSION = 0x4366a7c9,	// 	"CLASS_ID_EXPLOSION",	// explosion object
	CLASS_ID_CHUNK = 0x28e135b3,		// 	"CLASS_ID_CHUNK",		// chunk object
	CLASS_ID_CRASH = 0xdc273111,		// 	"CLASS_ID_CRASH",		// crash object
	CLASS_ID_COLLAPSE = 0x9592e289,		// 	"CLASS_ID_COLLAPSE",	// collapsing object
};

// FNV-1a Hash of Get_GOClass strings for faster comparison.
enum Hash_GOClass
{
	CLASS_AIR = 0x04de8a34,						//   "CLASS_AIR" // AirCraft.h
	CLASS_ANCHORROCKET = 0x2503fbf9,			//   "CLASS_ANCHORROCKET" // AnchorRocketClass.h
	CLASS_APC = 0x0da4acce,						//   "CLASS_APC" // APC.h
	CLASS_ARCCANNON = 0xe91cb08d,				//   "CLASS_ARCCANNON" // ArcCannonClass.h
	CLASS_ARMORY = 0x63ae241a,					//   "CLASS_ARMORY" // Armory.h
	CLASS_ARTIFACT = 0x3eb9e3b2,				//   "CLASS_ARTIFACT" // Artifact.h
	CLASS_ARTILLERY = 0xf9df4e76,				//   "CLASS_ARTILLERY" // Artillery.h
	CLASS_ASSAULTTANK = 0x1aefd513,				//   "CLASS_ASSAULTTANK" // AssaultTank.h
	CLASS_ASSAULTHOVER = 0xc864a6ef,			//   "CLASS_ASSAULTHOVER" // AssaultHover.h
	CLASS_BARRACKS = 0xb18ef1c3,				//   "CLASS_BARRACKS" // Barracks.h
	CLASS_BEAM = 0x20cec93f,					//   "CLASS_BEAM"
	CLASS_BLINK = 0x5c09179e,					//   "CLASS_BLINK" // BlinkDeviceClass.h
	CLASS_BOID = 0xa9238ce6,					//   "CLASS_BOID"
	CLASS_BOMBER = 0x4606b887,					//   "CLASS_BOMBER" // Bomber.h
	CLASS_BOMBERBAY = 0x6d0df679,				//   "CLASS_BOMBERBAY" // BomberBay.h
	CLASS_BOUNCEBOMB = 0xfce344e8,				//   "CLASS_BOUNCEBOMB"
	CLASS_BUILDING = 0x98264ce8,				//   "CLASS_BUILDING" // BuildingClass.h
	CLASS_BULLET = 0xd97930d0,					//   "CLASS_BULLET" // BulletClass.h
	CLASS_CANNON = 0xa54f6ffd,					//   "CLASS_CANNON" // CannonClass.h
	CLASS_CANNON_MACHINEGUN = 0x9203c82b,		//   "CLASS_CANNON_MACHINEGUN" // MachineGunClass.h
	CLASS_CANNON_TARGETING = 0x071c0ca1,		//   "CLASS_CANNON_TARGETING" // TargetingGunClass.h
	CLASS_CHARGEGUN = 0xfa593446,				//   "CLASS_CHARGEGUN" // ChargeGunClass.h
	CLASS_COMMBUNKER = 0xabf8e42d,				//   "CLASS_COMMBUNKER" // CommBunker.h
	CLASS_COMMTOWER = 0xfcc7b3dd,				//   "CLASS_COMMTOWER" // CommTower.h
	CLASS_COMMVEHICLE = 0xb6f61ce0,				//   "CLASS_COMMVEHICLE" // CommVehicle.h
	CLASS_CONSTRUCTIONRIG = 0x77f02e21,			//   "CLASS_CONSTRUCTIONRIG" // ConstructionRig.h
	CLASS_CLASS_CRAFT = 0xcf98abe1,				//   "CLASS_CRAFT" // CraftClass.h
	CLASS_CLASS_DAMAGEFIELD = 0x8ac14f6c,		//   "CLASS_DAMAGEFIELD" // DamageFieldClass.h
	CLASS_DAYWRECKER = 0x998a9793,				//   "CLASS_DAYWRECKER" // DayWrecker.h
	CLASS_DEPLOYABLE = 0x22b50b1f,				//   "CLASS_DEPLOYABLE" // Deployable.h
	CLASS_DEPLOYBUILDING = 0xf1656077,			//   "CLASS_DEPLOYBUILDING" // DeployBuilding.h
	CLASS_DEPOSIT = 0xdc5f98c6,					//   "CLASS_DEPOSIT" // Deposit.h
	CLASS_DISPENSER = 0xd566b803,				//   "CLASS_DISPENSER" // DispenserClass.h
	CLASS_EXPLOSION = 0x79dfd297,				//   "CLASS_EXPLOSION" // ExplosionClass.h
	CLASS_EXTRACTOR = 0x490dd218,				//   "CLASS_EXTRACTOR" // Extractor.h
	CLASS_FACTORY = 0xe39ee1ae,					//   "CLASS_FACTORY" // Factory.h
	CLASS_FLAG = 0xf2ecd752,					//   "CLASS_FLAG" // FlagObject.h
	CLASS_FLAREMINE = 0x88799827,				//   "CLASS_FLAREMINE" // FlareMineClass.h
	CLASS_GAMEOBJECT = 0xd840f319,				//   "CLASS_GAMEOBJECT" // GameObjectClass.h
	CLASS_GRENADE = 0x9e8cc54e,					//   "CLASS_GRENADE" // GrenadeClass.h
	CLASS_GRENADE_LASERPOPPER = 0x6255d3b4,		//   "CLASS_GRENADE_LASERPOPPER" // LaserPopperClass.h
	CLASS_GRENADE_POPPER = 0xab9d3e35,			//   "CLASS_GRENADE_POPPER" // PopperClass.h
	CLASS_GRENADE_RADARPOPPER = 0xdd93bbe7,		// //  "CLASS_GRENADE_RADARPOPPER" // RadarPopperClass.h [Same sig as GRENADE_POPPER, uses that]
	CLASS_HOVER = 0xbb81d932,					//   "CLASS_HOVER" // HoverCraft.h
	CLASS_HOWITZER = 0x25fd9ee0,				//   "CLASS_HOWITZER" // Howitzer.h
	CLASS_I76BUILDING = 0x3a9268d2,				//   "CLASS_I76BUILDING" // Building.h
	CLASS_JAMMER = 0x41fb32ec,					//   "CLASS_JAMMER" // JammerTower.h
	CLASS_JETPACK = 0x834f248a,					//   "CLASS_JETPACK" // JetPackClass.h
	CLASS_KINGOFHILL = 0xa9a32ae5,				//   "CLASS_KINGOFHILL" // KingOfHill.h
	CLASS_LAUNCHER = 0xc3cf9e70,				//   "CLASS_LAUNCHER" // LauncherClass.h
	CLASS_LAUNCHER_IMAGE = 0x9d350762,			//   "CLASS_LAUNCHER_IMAGE" // ImageLauncherClass.h
	CLASS_LAUNCHER_MULTI = 0xd264f7ea,			//   "CLASS_LAUNCHER_MULTI" // MultiLauncherClass.h
	CLASS_LAUNCHER_RADAR = 0xa905d923,			//   "CLASS_LAUNCHER_RADAR" // RadarLauncherClass.h
	CLASS_LAUNCHER_THERMAL = 0x46577898,		//   "CLASS_LAUNCHER_THERMAL" // ThermalLauncherClass.h
	CLASS_LAUNCHER_TORPEDO = 0x2b57e4d0,		//   "CLASS_LAUNCHER_TORPEDO" // TorpedoLauncherClass.h
	CLASS_LEADER_ROUND = 0xcdc72a4a,			// //  "CLASS_LEADER_ROUND " // LeaderRoundClass.h [Same sig as CLASS_CANNON_TARGETING, returns that]
	CLASS_LOCKSHELL = 0xc878487b,				//   "CLASS_LOCKSHELL" // LockShellClass.h
	CLASS_MAGNETGUN = 0x7eb95e20,				//   "CLASS_MAGNETGUN" // MagnetGunClass.h
	CLASS_MAGNETSHELL = 0x700207a4,				//   "CLASS_MAGNETSHELL" // MagnetShellClass.h
	CLASS_MINE = 0x17900f9b,					//   "CLASS_MINE" // MineClass.h
	CLASS_MINELAYER = 0x32493554,				//   "CLASS_MINELAYER" // Minelayer.h
	CLASS_MINE_MAGNET = 0x6e0a7e06,				//   "CLASS_MINE_MAGNET" // MagnetMineClass.h
	CLASS_MINE_PROXIMITY = 0x3a1b5a15,			//   "CLASS_MINE_PROXIMITY" // ProximityMineClass.h
	CLASS_MINE_TRIP = 0x9558c8af,				//   "CLASS_MINE_TRIP" // TripMineClass.h
	CLASS_MINE_WEAPON = 0x175a2ab0,				//   "CLASS_MINE_WEAPON" // WeaponMineClass.h
	CLASS_MISSILE = 0xe1de9178,					//   "CLASS_MISSILE" // MissileClass.h
	CLASS_MISSILE_IMAGE = 0x778daf1a,			//   "CLASS_MISSILE_IMAGE" // ImageMissileClass.h
	CLASS_MISSILE_LASER = 0x61e8eaac,			//   "CLASS_MISSILE_LASER" // LaserMissileClass.h
	CLASS_MISSILE_RADAR = 0x6a0a773b,			//   "CLASS_MISSILE_RADAR" // RadarMissileClass.h
	CLASS_MISSILE_THERMAL = 0x2bc381e0,			//   "CLASS_MISSILE_THERMAL" // ThermalMissileClass.h
	CLASS_MORPHTANK = 0x1c628d2e,				//   "CLASS_MORPHTANK" // MorphTank.h
	CLASS_MORTAR = 0x3170ea1d,					//   "CLASS_MORTAR" // MortarClass.h
	CLASS_MORTAR_REMOTE = 0xf75934f4,			//   "CLASS_MORTAR_REMOTE" // RemoteDetonatorClass.h
	CLASS_MOTIONSENSOR = 0x6dd673e4,			//   "CLASS_MOTIONSENSOR" // MotionSensor.h
	CLASS_NAVBEACON = 0x33e33cd9,				//   "CLASS_NAVBEACON" // NavBeaconClass.h
	CLASS_OBJECTSPAWN = 0xccf5a072,				//   "CLASS_OBJECTSPAWN" // ObjectSpawn.h
	CLASS_ORDNANCE = 0x0dcf99a8,				//   "CLASS_ORDNANCE" // OrdnanceClass.h
	CLASS_PERSON = 0x5768dca5,					//   "CLASS_PERSON" // PersonClass.h
	CLASS_PLANT = 0xdd054da5,					//   "CLASS_PLANT" // Plant.h
	CLASS_POWERED = 0xd68d4890,					//   "CLASS_POWERED" // PoweredBuilding.h
	CLASS_POWERUP_CAMERA = 0x8d7c8912,			//   "CLASS_POWERUP_CAMERA" // CameraPod.h
	CLASS_POWERUP_MONEY = 0xb163057b,			//   "CLASS_POWERUP_MONEY" // MoneyPowerup.h
	CLASS_POWERUP_RELOAD = 0xa662aa60,			//   "CLASS_POWERUP_RELOAD" // ServicePowerup.h
	CLASS_POWERUP_REPAIR = 0x4b970310,			//   "CLASS_POWERUP_REPAIR" // ServicePowerup.h
	CLASS_POWERUP_SERVICE = 0x33fe0a68,			//   "CLASS_POWERUP_SERVICE" // ServicePowerup.h
	CLASS_POWERUP_WEAPON = 0xfa990e15,			// //  "CLASS_POWERUP_WEAPON" // WeaponPowerup.h [Same sig as CLASS_WEAPON, returns that]
	CLASS_PULSESHELL = 0x27cd147d,				//   "CLASS_PULSESHELL" // PulseShellClass.h
	CLASS_RECYCLER = 0x8a0a1ead,				//   "CLASS_RECYCLER" // Recycler.h
	CLASS_RECYCLERVEHICLE = 0xc9cd7077,			//   "CLASS_RECYCLERVEHICLE" // RecyclerVehicle.h
	CLASS_SALVOLAUNCHER = 0xe8ce53bb,			//   "CLASS_SALVOLAUNCHER" // SalvoLauncherClass.h
	CLASS_SATCHELCHARGE = 0xc7788a0e,			//   "CLASS_SATCHELCHARGE" // SatchelCharge.h
	CLASS_SATCHELPACK = 0x2695d937,				//   "CLASS_SATCHELPACK" // SatchelPackClass.h
	CLASS_SAV = 0xa7275daa,						//   "CLASS_SAV" // SAV.h
	CLASS_SCAVENGER = 0x33a01170,				//   "CLASS_SCAVENGER" // Scavenger.h
	CLASS_SCAVENGERH = 0x5cfae8c8,				//   "CLASS_SCAVENGERH" // ScavengerH.h
	CLASS_SCRAP = 0x6ee4c53d,					//   "CLASS_SCRAP" // Scrap.h
	CLASS_SEEKER = 0xd888605f,					//   "CLASS_SEEKER" // SeekerClass.h
	CLASS_SEISMICWAVE = 0xa6dfc78e,				//   "CLASS_SEISMICWAVE" // SeismicWaveClass.h
	CLASS_SERVICE = 0x28e4d573,					//   "CLASS_SERVICE" // ServiceTruck.h
	CLASS_SERVICEH = 0x7b3b7981,				//   "CLASS_SERVICEH" // ServiceTruckH.h
	CLASS_SHIELDTOWER = 0x9a4a949e,				//   "CLASS_SHIELDTOWER" // ShieldTower.h
	CLASS_SHIELDUP = 0xe5a9cafc,				//   "CLASS_SHIELDUP" // ShieldUpgradeClass.h
	CLASS_SIGN = 0x81ed3511,					//   "CLASS_SIGN" // BuildingClass.h
	CLASS_SILO = 0x80f9ff71,					//   "CLASS_SILO" // Silo.h
	CLASS_SNIPERSHELL = 0xaaa6237f,				//   "CLASS_SNIPERSHELL" // SniperShellClass.h
	CLASS_SPAWNBUOY = 0x4a25dbb6,				//   "CLASS_SPAWNBUOY" // SpawnBuoy.h
	CLASS_SPECIAL = 0xc4f1f17b,					//   "CLASS_SPECIAL" // SpecialItemClass.h
	CLASS_SPECIAL_FORCEFIELD = 0x81e62d61,		//   "CLASS_SPECIAL_FORCEFIELD" // ForceFieldClass.h
	CLASS_SPECIAL_IMAGEREFRACT = 0xbefe3de0,	//   "CLASS_SPECIAL_IMAGEREFRACT" // ImageRefractClass.h
	CLASS_SPECIAL_RADARDAMPER = 0x364ce6f5,		//   "CLASS_SPECIAL_RADARDAMPER" // RadarDamperClass.h
	CLASS_SPECIAL_TERRAINEXPOSE = 0x02c0cbed,	//   "CLASS_SPECIAL_TERRAINEXPOSE" // TerrainExposeClass.h
	CLASS_SPRAYBOMB = 0xbe8a7c31,				//   "CLASS_SPRAYBOMB" // SprayBombClass.h
	CLASS_SPRAYBUILDING = 0xf4c70715,			//   "CLASS_SPRAYBUILDING" // SprayBuildingClass.h
	CLASS_SUPPLYDEPOT = 0xdadcb21b,				//   "CLASS_SUPPLYDEPOT" // SupplyDepot.h
	CLASS_TELEPORTAL = 0x7d9b0716,				//   "CLASS_TELEPORTAL" // TelePortalClass.h
	CLASS_TERRAIN = 0x6e47e5f1,					//   "CLASS_TERRAIN" // DummyClass.h
	CLASS_TORPEDO = 0xcd066db7,					//   "CLASS_TORPEDO" // TorpedoClass.h
	CLASS_TRACKEDDEPLOYABLE = 0x07257e15,		//   "CLASS_TRACKEDDEPLOYABLE" // TrackedDeployable.h
	CLASS_TRACKEDVEHICLE = 0x57b8478a,			//   "CLASS_TRACKEDVEHICLE" // TrackedVehicle.h
	CLASS_TUG = 0x04e12224,						//   "CLASS_TUG" // Tug.h
	CLASS_TURRET = 0xdf6dabba,					//   "CLASS_TURRET" // TurretCraft.h - ibgtow/fbspir (Guntower/gunspires)
	CLASS_TURRETTANK = 0x4a7aef92,				//   "CLASS_TURRETTANK" // TurretTank.h - ivturr/fvturr (vehicle turrets)
	CLASS_WALKER = 0xedb121ba,					//   "CLASS_WALKER" // Walker.h
	CLASS_WEAPON = 0x16164674,					//   "CLASS_WEAPON" // WeaponClass.h
	CLASS_WINGMAN = 0x83416d25,					//   "CLASS_WINGMAN" // Wingman.h
	CLASS_UNKNOWN = 0x1418355a,					//   "CLASS_UNKNOWN" // default
};

enum Hash_BZCClass
{
	BZCCLASS_ARMORY = 0xc6385024,				// "slf"				// ProductionHelper.h
	BZCCLASS_APC = 0x326aa115,					// "apc" 				// APCHelper.h
	BZCCLASS_ATTACHERWEAPON = 0xdc72cd43,		// "attacherweapon" 	// AttacherWeaponHelper.h
	BZCCLASS_CARRIER = 0xd7241cf7,				// "carrier"			// CarrierHelper.h
	BZCCLASS_CARRIER_DRONE = 0xfcd48611,		// "drone"				// CarrierHelper.h
	BZCCLASS_CERBERUS = 0xfba971de,				// "cerberus"			// AttacherHelper.h
	BZCCLASS_CERBERUS_TAIL = 0xc17ac6e3,		// "cerbtail"			// AttacherHelper.h
	BZCCLASS_COMMTOWER = 0xf542afc9,			// "comm"				// SatelliteHelper.h
	BZCCLASS_CONSTRUCTOR = 0x6b5974e3,			// "cnst"				// ProductionClassHelper.h
	BZCCLASS_DAYWRECKER = 0x94e092e2,			// "wrck"				// QuakeHelper.h
	BZCCLASS_DROPSHIP = 0xe5a66df6,				// "dropship"			// DropShipHelper.h
	BZCCLASS_FACTORY = 0xe184ddc3,				// "muf"				// ProductionHelper.h
	BZCCLASS_FURY = 0xf09c2de1,					// "fury"				// FuryHelper.h
	BZCCLASS_GEYSER = 0x63d063d8,				// "gysr"				// GeyserHelper.h
	BZCCLASS_HQCP = 0x06f63e29,					// "hqcp"				// HQCPHelper.h
	BZCCLASS_HUNT_SHIP = 0x44ad0c90,			// "DLLHuntUnit"		// DispatchHelper.h
	BZCCLASS_JAMM = 0x2c5846a2,					// "jamm"				// JammerHelper.h
	BZCCLASS_LAUNCH_PAD = 0x89fa26ac,			// "lpad"				// LPadHelper.h
	BZCCLASS_MINELAYER = 0x53179125,			// "DLLMinelayer",		// DispatchHelper.h
	BZCCLASS_MORPH_OBJECT = 0x2411d97a,			// "morphobject"		// MorphObjectHelper.h
	BZCCLASS_MORPH_OBJECT_TOGGLE = 0xb8846ed2,	// "morphobjecttoggle"	// MorphObjectHelper.h
	BZCCLASS_ORDNANCE_ANCHOR = 0x5ea3580a,		// "DLLAnchor"			// AnchorHelper.h
	BZCCLASS_ORDNANCE_SWITCH = 0x93e05f71,		// "switch"				// SwitchClassHelper.h
	BZCCLASS_ORDNANCE_VAMPIRE = 0x123ea3cf,		// "DLLVampire"			// AmmoDrainHelper.h
	BZCCLASS_PATROL_SHIP = 0x6f5ff9ab,			// "DLLPatrolship"		// DispatchHelper.h
	BZCCLASS_PERSON_PILOT = 0x1e627a4f,			// "pilo"				// PilotResourceHelper.h
	BZCCLASS_PERSON_SOLDIER = 0xe0633d1f,		// "sold"				// PilotResourceHelper.h
	BZCCLASS_PORTAL = 0x150c5d8b,				// "portal"				// PortalHelper.h
	BZCCLASS_PORTAL_SWITCH = 0xb472c1ee,		// "portswitch"			// PortalHelper.h
	BZCCLASS_POWERPLANT = 0xf54f2346,			// "power"				// PowerHelper.h
	BZCCLASS_QUAKEBOMB = 0x5e68e172,			// "quakebomb"			// QuakeHelper.h
	BZCCLASS_RECALLMINE = 0xbdfd16ab,			// "recallmine"				// RecallMineHelper.h
	BZCCLASS_RECYCLER = 0xdde43890,				// "recy"				// ProductionHelper.h
	BZCCLASS_SCAVENGER = 0xd4e81320,			// "scav"				// UtilityShipHelper.h
	BZCCLASS_SHIELD_TOWER = 0xba4a3b96,			// "shld"				// ShieldHelper.h
//	BZCCLASS_
};

// Static Const Char lists.
extern const char *CommandList[NUM_CMD];
extern const Hash_GOClass BuildingClass[];
extern const Hash_GOClass VehicleClass[];
extern const Hash_GOClass PowerupClass[];
extern const Hash_GOClass WeaponClass[];
extern const Hash_GOClass MiscClass[];
extern const char *DefaultAlignToTerrainList[];

// Common Unit ODF Lists.
extern const char *OffensiveShipList[];
extern const char *DefensiveShipList[];
extern const char *UtilityShipList[];

// Preload ODF List.
extern const char *PreloadRaceList[];

// Saved open ODF Files on this machine.
struct ODFName { char name[MAX_ODF_LENGTH]; }; // The ODF.
extern stdext::hash_map<unsigned long, ODFName> ODFNameMap;
// Saved open Files on this machine.
extern stdext::hash_map<const char*, FILE*> FileNameMap;
typedef stdext::hash_map<const char*, FILE*>::iterator FileMapIt;

// The Matrix. Free Your Mind...
static Matrix Identity_Matrix = Matrix(Vector(1, 0, 0), Vector(0, 1, 0), Vector(0, 0, 1), Vector(0, 0, 0));

// Things moved from DLLUtils.h

// Returns true if h is a recycler or recycler vehicle, false if h is invalid, or not a recycler.
extern bool IsRecycler(const Handle h);

// Given a team #, counts the number of allied human players currently playing. (i.e. not the number of possible allies, but the number of actual allies) 
// Note: should return at least 1, because team is an ally of itself. Does not count the neutral team (0), which is an ally of all teams by default.
extern int CountAlliedPlayers(const int Team);

// Sanity wrapper for GetVarItemStr. Reads the specified svar, and verifies it's present in the specified list. If not found in there, returns NULL.
extern const char* GetCheckedNetworkSvar(const size_t svar, const NETWORK_LIST_TYPE listType);

// Things moved from MP DLLs, shared here.

// Gets the initial player vehicle as selected in the shell.
//extern char *GetInitialPlayerODF(int Team);

// Given a race identifier, get the pilot back (used during a respawn).
extern const char *GetInitialPlayerPilotODF(const char Race);

// Given a race identifier, get the recycler ODF back.
extern const char *GetInitialRecyclerODF(const char Race);

// Gets the next vehicle ODF for the player on a given team.
extern const char *GetNextVehicleODF(const int Team, const bool Randomize = false, const RandomizeType DefaultRType = Randomize_None);

// Helper function for SetupTeam(), returns an appropriate spawnpoint.
extern Vector GetSpawnpointForTeam(const int Team);

// Camera functions from Bob "BS-er" Stewart's DLL Scriptor.

// Sets the angle of an object, in centigrees. 
extern void SetAngle(const Handle h, const float Degrees);
// Internal math functions used in setting the Angle of an object in the BuildAngleObject functions, and the SetAngle function.
extern Vector HRotateFront(const Vector Front, const float HAngleDifference);

// Camera Pos function from Bob "BS-er" Stewert, moves the camera between two positions at the incrument speed. Tweaked by GBD.
extern bool CameraPos(const Handle me, const Handle him, const Vector &PosA, const Vector &PosB, const float Increment);

// New things :)

// Code from WhosDr, Camera function that rotates around an object, at Offset position, at the speed of Speed.
extern bool CameraCircle(const Handle me, const Vector Offset, const Handle him, const Vector Speed);

// Formats a string with optional parameters, and prints it to the console. 
// Use with %d, %s, %f, %08x, etc, like with sprintf_s().
extern void FormatConsoleMessage(const char *format, ...);
// Formats a string with optional parameters, writes it to a log file and prints it to the console if Logging is enabled.
extern bool FormatLogMessage(const char *format, ...);
extern bool SimpleLogMessage(const char *format, ...);
// Version that takes in a wide char.
//extern void FormatLogMessage(wchar_t *format, ...);
// Writes a message to a file. Returns true if it succeeded, false if it failed. If Append is true, it appends to the existing file, if False then it whipes the existing file before writing. 
// This auotmatically opens the file name and stores it's name. Must call CloseOpenFiles() in destructor or postload.
extern bool FormatFileMessage(const char *Filename, const bool Append, const char *format, ...);

// Inputs an IFace_ConsoleCmd with variable argument list.
// For example: IFace_FormatConsoleCmd("sky.fogColor %d %d %d", colorR, colorG, colorB);
extern void IFace_FormatConsoleCmd(const char *format, ...);

// Attempts to determine what version the game is running, based on the text files installed in the /logs/ folder.
// Currently Returns PB1 = 131.0, PB2 = 132.0, PB3 = 133.0, PB4a = 134.0, TA5 = 135.0, PB5.1 = 135.1, PB6 = 136.0, PB6.1 = 136.1, PB6.2 = 136.2, PB6.3 = 136.3, PB6.4 = 136.4, PB6.5 = 136.5, and PB7+ = 137.0.
// Default returns 0 if it couldn't find it. It also searches from the newest to oldest.
extern int Get13Version();
// Gets the mod's version number from it's PatchData.txt file.
extern float GetBZCVersion();

// Attempts to save a game of the specified name.
extern void SaveGame(const char* filename);
// Attempts to load a Saved Game of the specified name.
extern void LoadGame(const char* filename);
// Attempts to delete a Saved Game of the specified name.
extern void DeleteGame(const char* filename);



// Gets a Matrix position based on ODF Position and Rotation Offset relative to the handle.
extern Matrix GetODFPositionOffset(const Handle h, const int Index = 0, const Vector AddPosition = Vector(0, 0, 0), const Vector AddRotation = Vector(0, 0, 0));
// Gets a Vector position based on ODF Position Offset relative to the handle.
inline Vector GetODFPositionOffset2(const Handle h, const int Index = 0, const Vector AddPosition = Vector(0, 0, 0)) { return GetODFPositionOffset(h, Index, AddPosition).posit; }

// Returns the current TPS rate in use. (and sets it for the clients)
extern int GetTPS(void);

// Useful Conversion functions. Multiplies a Float of Seconds by Game_TPS. // Enable if m_GameVersion >= v13PB7
//inline int ConvertToTurns(const float Seconds) { return int(Seconds * GetTPS()); } // SecondsToTurns(Seconds); }
//inline int ConvertToTurns(const int Seconds) { return Seconds * GetTPS(); }
//inline float ConvertToSeconds(const int Turns)  { return float(Turns / GetTPS()); } // TurnsToSeconds(Turns); }
//inline float ConvertToSeconds(const float Turns) { return Turns / GetTPS(); }

// Overload functions that take const parameters.
inline void SetPilotClass(const Handle obj, const char *odf) { SetPilotClass(obj, const_cast<char *>(odf)); }
inline Handle BuildEmptyCraftNear(const Handle h, const char* ODF, const int Team, const float MinRadiusAway, const float MaxRadiusAway) { return BuildEmptyCraftNear(h, const_cast<char *>(ODF), Team, MinRadiusAway, MaxRadiusAway); }
inline void PrintConsoleMessage(const char* msg) { PrintConsoleMessage(const_cast<char *>(msg)); }
inline bool OpenODF(const char *name) { return OpenODF(const_cast<char *>(name)); }
inline bool DoesODFExist(const char* odf) { return DoesODFExist(const_cast<char *>(odf)); }
inline Handle GetHandle(const char *name) { return GetHandle(const_cast<char *>(name)); }
inline unsigned long CalcCRC(const char *name) { return CalcCRC(const_cast<char *>(name)); }
inline void SetPlan(const char* cfg, const int team = -1) { SetPlan(const_cast<char *>(cfg), team); }
//inline Dist GetDistance(const Handle h1, const Handle h2) { return misnImport.GetDistanceObject(const_cast<Handle &>(h1), const_cast<Handle &>(h2)); }
inline Dist GetDistance(const Handle h1, const char* path, const int point = 0) { return misnImport.GetDistancePath(const_cast<Handle &>(h1), const_cast<Name>(path), point); }
inline Dist GetDistance(const Handle h1, const AiPath *path, const int point = 0) { return misnImport.GetDistancePathPtr(const_cast<Handle &>(h1), const_cast<AiPath *>(path), point); }
inline Handle GetNearestVehicle(const char* path, const int point) { return GetNearestVehicle(const_cast<Name>(path), point); }
inline void SetPosition(const Handle h, const char* path) { SetPosition(h, const_cast<Name>(path)); }
inline void SetPathType(const char* path, const PathType pathType) { SetPathType(const_cast<Name>(path), pathType); }
inline bool IsInfo(const char* odf) { return IsInfo(const_cast<Name>(odf)); }
inline void GiveWeapon(const Handle me, const char* weapon) { GiveWeapon(me, const_cast<Name>(weapon)); }
inline void IFace_Exec(const char* n) { IFace_Exec(const_cast<Name>(n)); }
inline void IFace_Activate(const char* n) { IFace_Activate(const_cast<Name>(n)); }
inline void IFace_Deactivate(const char* n) { IFace_Deactivate(const_cast<Name>(n)); }
inline void IFace_CreateCommand(const char* n) { IFace_CreateCommand(const_cast<Name>(n)); }
inline void IFace_CreateString(const char* name, const char* value) { IFace_CreateString(const_cast<Name>(name), const_cast<Name>(value)); }
inline void IFace_SetString(const char* name, const char* value) { IFace_SetString(const_cast<Name>(name), const_cast<Name>(value)); }
inline void IFace_GetString(const char* name, char* value, int maxSize) { IFace_GetString(const_cast<Name>(name), value, maxSize); }
inline void IFace_CreateInteger(const char* name, const int value) { IFace_CreateInteger(const_cast<Name>(name), value); }
inline void IFace_SetInteger(const char* name, const int value) { IFace_SetInteger(const_cast<Name>(name), value); }
inline int IFace_GetInteger(const char* name) { return IFace_GetInteger(const_cast<Name>(name)); }
inline void IFace_SetIntegerRange(const char* name, const int low, const int high) { IFace_SetIntegerRange(const_cast<Name>(name), low, high); }
inline void IFace_CreateFloat(const char* name, const float value) { IFace_CreateFloat(const_cast<Name>(name), value); }
inline void IFace_SetFloat(const char* name, const float value) { IFace_SetFloat(const_cast<Name>(name), value); }
inline float IFace_GetFloat(const char* name) { return IFace_GetFloat(const_cast<Name>(name)); }
inline void IFace_ClearListBox(const char* name) { IFace_ClearListBox(const_cast<Name>(name)); }
inline void IFace_AddTextItem(const char* name, const char* value) { IFace_AddTextItem(const_cast<Name>(name), const_cast<Name>(value)); }
inline void IFace_GetSelectedItem(const char* name, char* value, const int maxSize) { IFace_GetSelectedItem(const_cast<Name>(name), value, maxSize); }
inline bool CameraPath(const char* path_name, const int height, const int speed, const Handle target_handle) { return CameraPath(const_cast<Name>(path_name), height, speed, target_handle); }
inline bool CameraPathDir(const char* path_name, const int height, const int speed) { return CameraPathDir(const_cast<Name>(path_name), height, speed); }
inline float SetAnimation(const Handle h, const char* n, const int animType = 0) { return SetAnimation(h, const_cast<Name>(n), animType); }
inline float GetAnimationFrame(const Handle h, const char* n) { return GetAnimationFrame(h, const_cast<Name>(n)); }
inline void SetObjectiveName(const Handle h, const char* n) { SetObjectiveName(h, const_cast<Name>(n)); }
inline void AddObjective(const char* name, const long color, const float showTime = 8.0f) { AddObjective(const_cast<Name>(name), color, showTime); }
inline void AddIdleAnim(const Handle h, const char* anim) { AddIdleAnim(h, const_cast<Name>(anim)); }
inline void SpawnBirds(const int group, const int count, const char* odf, const TeamNum t, const char* path) { SpawnBirds(group, count, const_cast<Name>(odf), t, const_cast<Name>(path)); }
inline void SpawnBirds(const int group, const int count, const char* odf, const TeamNum t, const Handle startObj, const Handle destObj) { SpawnBirds(group, count, const_cast<Name>(odf), t, startObj, destObj); }
inline void CalcCliffs(const char* path) { CalcCliffs(const_cast<Name>(path)); }
inline void TranslateString2(char* Dst, const size_t bufsize, const char* Src) { TranslateString2(Dst, bufsize, const_cast<Name>(Src)); }
inline void TranslateString(char* Dst, const char* Src) { TranslateString2(Dst, sizeof(Dst), const_cast<Name>(Src)); } // Updated to re-direct to TranslateString2.
inline void Network_SetString(const char* name, const char* value) { Network_SetString(const_cast<Name>(name), const_cast<Name>(value)); }
inline void Network_SetInteger(const char* name, const int value) { Network_SetInteger(const_cast<Name>(name), value); }



// Function Overloads, these are various name overloads for stock functions that take in different types of arguments by default. Does back end fiddly work for you.
// Sets a handle to the position of another Handle.
inline void SetPosition(const Handle h, const Vector v) { SetVectorPosition(h, v); } // Name Overload for SetVectorPosition.
inline void SetPosition(const Handle h1, const Handle h2) { SetVectorPosition(h1, GetPosition(h2)); }
inline void SetPositionPath(const Handle h1, const char* Path, const int Point = 0) { SetVectorPosition(h1, GetVectorFromPath(Path, Point)); }
// Version that uses a Matrix to take rotation into account.
extern void SetPositionM(const Handle h1, const Handle h2);
// Gets the distance between a Handle and a Vector.
inline float GetDistance(const Handle h, const Vector v) { return GetDistance2D(GetPosition(h), v); }
inline float GetDistance(const Handle h, const Matrix m) { return GetDistance2D(GetPosition(h), m.posit); }
// GiveWeapon that takes a slot. (Uses ReplaceWeapon command)
extern void GiveWeapon(const Handle h, const char* Weapon, const int Slot);
// GetCircularPos.
inline Vector GetCircularPos(const Vector &CenterPos, const float Radius, const float Angle) { Vector Pos(0,0,0);	SetCircularPos(CenterPos, Radius, Angle, Pos); return Pos; }
// Version of StartAudio3D that takes a Vector and a Matrix.
inline DLLAudioHandle StartAudio3D(const char *const name, const Vector Pos, const DLLAudioCategory cat = AUDIO_CAT_UNKNOWN, const bool loop = false) { return StartAudio3D(name, Pos.x, Pos.y, Pos.z, cat, loop); }
inline DLLAudioHandle StartAudio3D(const char *const name, const Matrix Mat, const DLLAudioCategory cat = AUDIO_CAT_UNKNOWN, const bool loop = false) { return StartAudio3D(name, Mat.posit, cat, loop); }
// Version of Setcommand that takes a string for the param. e.g. "SetCommand2(me, CMD_BUILD, 0, "make_me_a_silo", "absilo");
//inline void SetCommand2(Handle me, int cmd, int priority = 0, Handle who = 0, const char* path = NULL, char *param = NULL) { SetCommand(me, cmd, priority, who, path, CalcCRC(param)); }
// Get Position that takes a path point.
inline Vector GetPosition(const char* Path, const int Point = 0) { return GetVectorFromPath(Path, Point); }
inline Vector GetPosition2(const Handle h) { Vector v(0, 0, 0); GetPosition2(h, v); return v; }
// Version that takes a Handle, Matrix, and Path name/point.
inline Vector GetPositionNear(const Handle h, const float MinRadiusAway, const float MaxRadiusAway) { return GetPositionNear(GetPosition(h), MinRadiusAway, MaxRadiusAway); }
inline Vector GetPositionNear(const Matrix m, const float MinRadiusAway, const float MaxRadiusAway) { return GetPositionNear(m.posit, MinRadiusAway, MaxRadiusAway); }
inline Vector GetPositionNear(const char* Path, const int Point, const float MinRadiusAway, const float MaxRadiusAway) { return GetPositionNear(GetVectorFromPath(Path, Point), MinRadiusAway, MaxRadiusAway); }
// Version of CameraObject that takes a Vector for position.
inline bool CameraObject(const Handle object_handle, const Vector pos, const Handle target_handle) { return CameraObject(object_handle, pos.x, pos.y, pos.z, target_handle); }
// Version that takes a Vector, a Matrix, and a Path/Point.
inline float TerrainFindFloor(const Vector pos) { return TerrainFindFloor(pos.x, pos.z); }
inline float TerrainFindFloor(const Matrix mat) { return TerrainFindFloor(mat.posit.x, mat.posit.z); }
inline float TerrainFindFloor(const char* Path, const int Point = 0) { Vector Test = GetVectorFromPath(Path, Point); return TerrainFindFloor(Test.x, Test.z); }
inline float TerrainFindFloor(const Handle h) { return TerrainFindFloor(GetPosition(h)); }
// Version that takes a Matrix and Path/Point.
inline bool TerrainIsWater(const Matrix mat) { return TerrainIsWater(mat.posit); }
inline bool TerrainIsWater(const char* Path, const int Point = 0) { return TerrainIsWater(GetVectorFromPath(Path, Point)); }
inline bool TerrainIsWater(const Handle h) { return TerrainIsWater(GetPosition(h)); }
// Version that takes a Matrix and Path/Point.
inline bool TerrainGetHeightAndNormal(const Matrix& mat, float& outHeight, Vector& outNormal, const bool useWater) { return TerrainGetHeightAndNormal(mat.posit, outHeight, outNormal, useWater); }
inline bool TerrainGetHeightAndNormal(const char* Path, const int Point, float& outHeight, Vector& outNormal, const bool useWater) { return TerrainGetHeightAndNormal(GetVectorFromPath(Path, Point), outHeight, outNormal, useWater); }
inline bool TerrainGetHeightAndNormal(const Handle h, float& outHeight, Vector& outNormal, const bool useWater) { return TerrainGetHeightAndNormal(GetPosition(h), outHeight, outNormal, useWater); }
// Version of SetTeamColor that takes a DWORD Color and Vector Color.
inline void SetTeamColor(const int Team, const DWORD Color) { SetTeamColor(Team, RGBA_GETRED(Color), RGBA_GETGREEN(Color), RGBA_GETBLUE(Color)); }
inline void SetTeamColor(const int Team, const Vector Color) { SetTeamColor(Team, int(Color.x), int(Color.y), int(Color.z)); }
// Version that returns a Vector Color
inline Vector GetFFATeamColor(const TEAMCOLOR_TYPE Type, const int Team) { int r = 0, g = 0, b = 0; GetFFATeamColor(Type, Team, r, g, b); return Vector(float(r),float(g),float(b)); }
inline Vector GetTeamStratColor(const TEAMCOLOR_TYPE Type, const int Team) { int r = 0, g = 0, b = 0; GetTeamStratColor(Type, Team, r, g, b); return Vector(float(r),float(g),float(b)); }
inline Vector GetTeamStratIndividualColor(const TEAMCOLOR_TYPE Type, const int Team) { int r = 0, g = 0, b = 0; GetTeamStratIndividualColor(Type, Team, r, g, b); return Vector(float(r),float(g),float(b)); }
// Version that takes a float for the Height.
inline bool CameraPath(const char* path_name, const float height, const int speed, const Handle target_handle) { return CameraPath(const_cast<Name>(path_name), int(height), speed, target_handle); }

// Custom GetODF functions that Handle opening/closing the ODF, as well as one level of ODF Inheritence. Use Spairingly or by themselves.
extern int GetODFHexInt(const Handle h, const char* block, const char* name, int* value = NULL, const int defval = 0);
extern int GetODFInt(const Handle h, const char* block, const char* name, int* value = NULL, const int defval = 0);
extern int GetODFLong(const Handle h, const char* block, const char* name, long* value = NULL, const long defval = 0);
extern int GetODFFloat(const Handle h, const char* block, const char* name, float* value = NULL, const float defval = 0.0f);
extern int GetODFDouble(const Handle h, const char* block, const char* name, double* value = NULL, const double defval = 0.0);
extern int GetODFChar(const Handle h, const char* block, const char* name, char* value = NULL, const char defval = '\0');
extern int GetODFBool(const Handle h, const char* block, const char* name, bool* value = NULL, const bool defval = false);
extern int GetODFString(const Handle h, const char* block, const char* name, size_t ValueLen = 0, char* value = NULL, const char* defval = NULL);
extern int GetODFColor(const Handle h, const char* block, const char* name, DWORD* value = NULL, const DWORD defval = 0);
extern int GetODFVector(const Handle h, const char* block, const char* name, Vector* value = NULL, const Vector defval = Vector(0.0f, 0.0f, 0.0f));

// Versions that use built-in single level of ODF Inheritence, pass in ODFName1, and the classlabel string with .odf appended as ODFName2, and it returns the value. NOTE: These assume the ODF is already Open. 
inline int GetODFHexInt(const char* file1, const char* file2, const char* block, const char* name, int* value = NULL, const int defval = 0) { return GetODFHexInt(file1, block, name, value, defval) || GetODFHexInt(file2, block, name, value, defval); }
inline int GetODFInt(const char* file1, const char* file2, const char* block, const char* name, int* value = NULL, const int defval = 0) { return GetODFInt(file1, block, name, value, defval) || GetODFInt(file2, block, name, value, defval); }
inline int GetODFLong(const char* file1, const char* file2, const char* block, const char* name, long* value = NULL, const long defval = 0) { return GetODFLong(file1, block, name, value, defval) || GetODFLong(file2, block, name, value, defval); }
inline int GetODFFloat(const char* file1, const char* file2, const char* block, const char* name, float* value = NULL, const float defval = 0.0f) { return GetODFFloat(file1, block, name, value, defval) || GetODFFloat(file2, block, name, value, defval); }
inline int GetODFDouble(const char* file1, const char* file2, const char* block, const char* name, double* value = NULL, const double defval = 0.0) { return GetODFDouble(file1, block, name, value, defval) || GetODFDouble(file2, block, name, value, defval); }
inline int GetODFChar(const char* file1, const char* file2, const char* block, const char* name, char* value = NULL, const char defval = '\0') { return GetODFChar(file1, block, name, value, defval) || GetODFChar(file2, block, name, value, defval); }
inline int GetODFBool(const char* file1, const char* file2, const char* block, const char* name, bool* value = NULL, const bool defval = false) { return GetODFBool(file1, block, name, value, defval) || GetODFBool(file2, block, name, value, defval); }
inline int GetODFString(const char* file1, const char* file2, const char* block, const char* name, size_t ValueLen = 0, char* value = NULL, const char* defval = NULL) { return GetODFString(file1, block, name, ValueLen, value, defval) || GetODFString(file2, block, name, ValueLen, value, defval); }
inline int GetODFColor(const char* file1, const char* file2, const char* block, const char* name, DWORD* value = NULL, const DWORD defval = 0) { return GetODFColor(file1, block, name, value, defval) || GetODFColor(file2, block, name, value, defval); }
inline int GetODFVector(const char* file1, const char* file2, const char* block, const char* name, Vector* value = NULL, const Vector defval = Vector(0.0f, 0.0f, 0.0f)) { return GetODFVector(file1, block, name, value, defval) || GetODFVector(file2, block, name, value, defval); }

// Determins if Handle 1 is facing Handle 2, within the coneAngle margin of error. Higher cone angle (1.0) means facing exactly, lower cone angle means less.
extern bool IsObjectFacingObject(const Handle h1, const Handle h2, const float ConeAngle);
// Version that takes Vectors and a Position.
extern bool IsObjectFacingObject(const Vector FrontA, const Vector PositionA, const Vector PositionB, const float ConeAngle);
// Version that takes Matrix's and a coneAngle.
inline bool IsObjectFacingObject(const Matrix PositionA, const Matrix PositionB, const float ConeAngle) { return IsObjectFacingObject(PositionA.front, PositionA.posit, PositionB.posit, ConeAngle); }

// Determins if Handle 2 is to the right of Handle 1, or to the Left. True = Right, False = Left. Rare case of either = Left.
extern bool IsObjectFacingObjectRight(const Handle me, const Handle him);

// Checks the Team Filter settings, given a Team filter and two Team numbers. Returns true if the teams match the TeamFilter. 1 = Same Team, 2 = Allies, 4 = Enemies.
extern bool TeamFilterCheck(const int TeamFilter, const int Team1, const int Team2);

// Scales camera speeds for Pre 1.3.6.7 cameras. //!!-- POST 1.3.6.7 make it only speed * GLOBAL_MULT
inline int ScaleCameraSpeed(const int speed) { if(Get13Version() >= v13PB7) return int(speed * GLOBAL_MULT); else return int(speed * GLOBAL_MULT * 60 / GetTPS()); }
// Scales for BZ1 > BZ2 distances. 
inline float ScaleDistance(const float dist) { return float(dist * GLOBAL_MULT); }
inline int ScaleDistance(const int dist) { return int(dist * GLOBAL_MULT); }

// Allies two teams together. (paired ally/unally)
inline void AllyTeams(const int t1, const int t2) { Ally(t1, t2); Ally(t2, t1); }
inline void UnAllyTeams(const int t1, const int t2) { UnAlly(t1, t2); UnAlly(t2, t1); }

// Gets a position as a Matrix.
inline Matrix GetMatrixPosition(const Handle h) { Matrix m; GetPosition(h, m); return m; }

// Polypoly. Takes in a saved std:Vector of a path. Code from Nielk1/Ken Miller.
extern bool PointInPoly(const std::vector<VECTOR_2D> &areaPath, const Vector &vector);
// Version that takes in a path name, e.g. "area_path", but it may be slightly more expensive to call this each tick. 
extern bool PointInPoly(const char* path, const Vector vector);
// Version that returns the Winding Number test count. Now utilized by above functions.
extern int GetWindCount(const std::vector<VECTOR_2D> &areaPath, const Vector &vector);
extern int GetWindCount(const char* path, const Vector vector);

// Things inspired from TRO functions:
// Gets the size of the path, and returns true when the Handle has reached < 12.0f of the final point.
extern bool IsFinishedWithPath(const Handle h, const char* path);
// IsInfo that takes in a handle.
extern bool IsInfo(const Handle h);
// Returns true if Handle me is following Handle him.
extern bool IsFollowing(const Handle me, const Handle him);
// Recycle command.
extern void Recycle(const Handle h, const int priority);
// Returns true if Handle h does not have full Health. // Same as BZ1.5's IsDamaged(Handle h)
extern bool NeedsHealth(const Handle h);
// Returns true if Handle h does not have full Ammo.
extern bool NeedsAmmo(const Handle h);

// Function to make a BZC Production unit Undeploy.
extern void PackUp(const Handle h);

// Command for Hunt/Bailout/Stage.
inline void Hunt(const Handle h, const int Priority = 1) { SetCommand(h, CMD_HUNT, Priority); }
inline void Bailout(const Handle h, const int Priority = 1) { SetCommand(h, CMD_BAILOUT, Priority); }
inline void Stage(const Handle h, const int Priority = 1) { SetCommand(h, CMD_STAGE, Priority); }

// Counts the number of taps on an object.
extern int GetTapCount(const Handle h, const bool IgnoreInvincible = false);

// Version that adds to the current amounts.
extern void AddScavengerCurScrap(const Handle h, const int aNewScrap);
extern void AddScavengerMaxScrap(const Handle h, const int aNewScrap);

// Spawns a scrap field of specified amount at a position.
extern void SpawnScrapField(const Vector Pos, const int Amount = 1, const float Radius = 0.0f, const char Race = 'n');
inline void SpawnScrapField(const char* Path, const int Point = 0, const int Amount = 1, const float Radius = 0.0f, const char Race = 'n') { SpawnScrapField(GetVectorFromPath(Path, Point), Amount, Radius, Race); }
inline void SpawnScrapField(const Handle h, const int Amount = 1, const float Radius = 0.0f, const char Race = 'n') { SpawnScrapField(GetPosition(h), Amount, Radius, Race); }
inline void SpawnScrapField(const Matrix Mat, const int Amount = 1, const float Radius = 0.0f, const char Race = 'n') { SpawnScrapField(Mat.posit, Amount, Radius, Race); }

// Returns how much damage an ordnance odf does to a particular Target. (NOTE: Does not include Explosion damage.)
extern long GetOrdnanceDamageDealt(const Handle h, const char *OrdnanceODF, const bool UseExplosion = true);

// Gets the EngageRange of a target, based on EngageRange, Rangescan, or Weapon Range, in that order.
extern float GetEngageRange(const Handle h);

// Gets the minimum amount of ammo needed to fire the selected weapons of an AI ship.
extern long GetMinFireAmmo(const Handle h);

// Returns true if a path exists.
extern bool DoesPathExist(const char* path);

// IsDamaged that takes a float.
extern bool IsDamaged(const Handle h, const float Threshold = 0.95f);

// Parellels to GetHealth / GetAmmo, sets them to a % of max.
extern void SetHealth(const Handle h, const float Percent);
extern void SetAmmo(const Handle h, const float Percent);

// Gets a random Team, based on filters. TeamFilter is 0 = All, 1 = SameTeam, 2 = Allies, 4 = Enemies. 
// ActiveTeamsOnly will only pass if this team has a BaseSlot item, or a Player. If SkipTeamZero is true, it skips Team 0.
extern int GetRandomTeam(const int TeamFilter = 0, const int Team = -1, const bool ActiveTeamsOnly = false, const bool SkipTeamZero = true);

// This function checks the validity of a CFG name retrieved by GetObjInfo Get_CFG/Get_ODF, and cuts off the :## and/or .odf from the end. 
// Useful for checking things built by factories.
extern void CheckODFName(char *ODFName);

// Gets the Current UnitName, without the Skill Modifier in it.
extern const char *GetUnitName(const Handle h);

// Applies the Skill modifier to the unit's Name.
extern void SetSkillName(const Handle h, const int Skill = -1);

// Attempts to emulate the VO register by playing a VO from a unit's ODF. If a team is specified, it plays for that Team, otherwise uses Team from unit.
extern void PlayVOMsg(const Handle h, const char *MsgString, const int Team = -1);

// Adds a world letter to the ODF, if the ODF exists.
extern bool GetWorldVariantODF(char *ODFName);
extern const char *GetWorldVariantODF(const Handle h);

// This function checks the validity of an ODF name retrieved by GetObjInfo Get_ODF, and cuts off the .odf from the end.
extern void GetODFName(const Handle h, char *ODFName);
extern const char *GetODFName(const Handle h);

// Function that gets the ODF, Attempts to Open it, and fills TempODF1 and 2 with the .ODF names of the object and it's classlabel.
extern void GetAndOpenODFNames(const Handle h, char *TempODF1, char *TempODF2, char *ODFName = NULL);

// From SPMission: //#define IsOdf2(h,name) (_strnicmp(ODFName,name,strlen(name)) == 0)
inline bool IsOdf2(const Handle h, const char *name) { return (_strnicmp(GetODFName(h), name, strlen(name)) == 0); }

// Gets a random ODF of the specified race, if it exists.
extern const char *GetRandomOffensiveODF(const char Race);
extern const char *GetRandomDefensiveODF(const char Race);
extern const char *GetRandomUtilityODF(const char Race);
// Returns a random ship ODF name, out of the above functions. (Randomly picks one of the 3 above to use, so an even chance of Offense, Defense, or Utility.)
extern const char *GetRandomShipODF(const char Race);

// Preloads a Race given a race char.
extern void PreloadRace(const char Race);

// Generalized Class Filters.
// Returns true if it is a Building. (Includes CLASS_TURRET)
extern bool IsBuilding(const Handle h);
// Returns true if it is a Vehicle. (Excludes CLASS_TURRET)
extern bool IsVehicle(const Handle h);
// Returns true if it is a Powerup. (Includes CLASS_POWERUP_CAMERA)
extern bool IsPowerup(const Handle h);
// Returns true if it is a Weapon. (Mine/Torpedo)
extern bool IsWeapon(const Handle h);
// Catchall for the rest, Scrap, KoTH, Flags, Spawnbouys, etc...
extern bool IsMisc(const Handle h);

// Tests for more specific types out of the above.
// Is it scrap? // CLASS_SCRAP
extern bool IsScrap(const Handle h);
// Is it a turret? (Tank kind, not GunTower kind) CLASS_TURRETTANK
extern bool IsTurret(const Handle h);
// Is it a scavenger? // CLASS_SCAVENGER or CLASS_SCAVENGERH
extern bool IsScavenger(const Handle h);
// Is it a barracks? // CLASS_BARRACKS
extern bool IsBarracks(const Handle h);
// Is it a gun tower? // CLASS_TURRET
extern bool IsGunTower(const Handle h);
// Is it a Geyser/Pool? // CLASS_DEPOSIT
extern bool IsGeyser(const Handle h);
// Is it a Spawnpoint? // CLASS_SPAWNBUOY
extern bool IsSpawnPoint(const Handle h);
// Is it a Silo? // CLASS_SILO or CLASS_EXTRACTOR
extern bool IsSilo(const Handle h);
// Is this a Factory? // BZCclasslabel "muf" or CLASS_FACTORY
extern bool IsFactory(const Handle h);
// Is this an Armory? // BZCClasslabel "slf" or CLASS_ARMORY
extern bool IsArmory(const Handle h);
// Is it a Constructor? // CLASS_CONSTRUCTIONRIG specifically.
extern bool IsConstructor(const Handle h);
// Is this object a producer? (IsRecycler/IsFactory/IsArmory/IsConstructor?)
inline bool IsProducer(const Handle h) { return IsRecycler(h) || IsFactory(h) || IsArmory(h) || IsConstructor(h); }
// Is this object a power plant? //BZCclasslabel "power", or CLASS_POWERPLANT with -powerCost.
extern bool IsPower(const Handle h);
// Is this object a Tug? // CLASS_TUG
extern bool IsTug(const Handle h);
// Is this object an APC? // BZCclasslabel "apc" or CLASS_APC
extern bool IsAPC(const Handle h);
// Is this a Walker? // CLASS_WALKER
extern bool IsWalker(const Handle h);
// Is it Artillary? // CLASS_HOWITZER or CLASS_ARTILLARY
extern bool IsArtillary(const Handle h);
// Is it a Comm Tower? // BZCClasslabel "comm", or CLASS_COMMBUNKER or CLASS_COMMTOWER
extern bool IsCommTower(const Handle h);
// Is it a Portal? // BZCClasslabel "portal" or CLASS_TELEPORTAL
extern bool IsPortal(const Handle h);
// Is it a fury? // BZCClasslabel "fury" or CLASS_SAV
extern bool IsFury(const Handle h);
// Is this a Carrier? // BZCClasslabel "carrier"
extern bool IsCarrier(const Handle h);
// Is this a Drone? // BZCClasslabel "drone"
extern bool IsDrone(const Handle h);
// Is this a DropShip? // BZCClasslabel "dropship"
extern bool IsDropShip(const Handle h);
// Is this an HQCP? // BZCClasslabel "hqcp" or CLASS_MOTIONSENSOR
extern bool IsHQCP(const Handle h);
// Is this a Jammer? // CLASS_JAMMER
extern bool IsJammer(const Handle h);
// Is this a MorphObject? // BZCClasslabel "morphobject"
extern bool IsMorphObject(const Handle h);
// Is this an LPad? // BZCClasslabel "lpad"
extern bool IsLPad(const Handle h);
// Is this a Shield? // BZCClasslabel "shld" or CLASS_SHIELDTOWER
extern bool IsShield(const Handle h);
// Is this a Minelayer?
extern bool IsMinelayer(const Handle h);

// Does it default to align to terrain?
extern bool DefaultAlignToTerrain(const char *ODFName);
inline bool DefaultAlignToTerrain(const Handle h) { return DefaultAlignToTerrain(GetODFName(h)); }

// Replace an object's weapons, with optional 2nd object ODF. Takes a Handle and an array of PreviousWeapons[MAX_HARDPOINTS]. Returns false if the handle passed in doesn't exist.
// NOTE: The array passed into these MUST have a size of 5. NewWeapons[MAX_HARDPOINTS] is optional, if not present it will restore old weapons from PreviousODF, if present.
extern bool ReplaceWeapons(const Handle h, const char NewWeapons[][MAX_ODF_LENGTH], const float PreviousLocalAmmo[] = NULL, const char *PreviousODF = NULL);

// Similar to GetPositionNear, only tries to find one that is further away then the current position. If Near is true, it picks a position <= distance closer, if false, it picks a position further.
extern Vector GetPositionNearOrFar(const Vector v1, const Vector v2, const float MinRadius, const float MaxRadius, const bool Near = true);
// Handles.
inline Vector GetPositionNearOrFar(const Handle me, const Handle him, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(GetPosition(me), GetPosition(him), MinRadius, MaxRadius, Near); }
inline Vector GetPositionNearOrFar(const Handle me, const Name Path, const int Point, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(GetPosition(me), GetVectorFromPath(Path, Point), MinRadius, MaxRadius, Near); }
inline Vector GetPositionNearOrFar(const Handle me, const Vector v2, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(GetPosition(me), v2, MinRadius, MaxRadius, Near); }
inline Vector GetPositionNearOrFar(const Handle me, const Matrix m2, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(GetPosition(me), m2.posit, MinRadius, MaxRadius, Near); }
// Rediculous type variations. :)
inline Vector GetPositionNearOrFar(const Matrix m1, const Matrix m2, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(m1.posit, m2.posit, MinRadius, MaxRadius, Near); }
inline Vector GetPositionNearOrFar(const Vector v1, const Matrix m2, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(v1, m2.posit, MinRadius, MaxRadius, Near); }
inline Vector GetPositionNearOrFar(const Matrix m1, const Vector v2, const float MinRadius, const float MaxRadius, const bool Near = true) { return GetPositionNearOrFar(m1.posit, v2, MinRadius, MaxRadius, Near); }

// Gets the pilot cost of an odf.
extern int GetPilotCost(const char* ODF);
// Version that takes a Handle.
inline int GetPilotCost(const Handle h) { return GetPilotCost(GetODFName(h)); }

// Returns the number of weapon hardpoints an object has. Returns -1 if the handle is invalid.
extern int GetNumWeapons(const Handle h);

// Gets a random player, out of all current players.
extern Handle GetRandomPlayer(void);

// Gets a Handle from a postion. (! WRITE ME) // Has to be in BZ1Helper to use HandleList for searching.
//extern Handle GetHandle(Matrix Position); // get nearest, fabsf 0.001
//extern Handle GetHandle(Vector Position); // get nearest, fabsf 0.001
//extern Handle GetHandle(Name Path); // get nearest 2d fabsf 0.001

// Gets the _config.odf of a handle.
//extern void GetConfigODF(Handle h, char *Value);
extern const char *GetConfigODF(const Handle h);

// This function gets an item from an objects's BuildMenu list. It returns true if successful, and fills the ScrapCost and ReturnODF with the first found items info.
// Type is: 0 = Any. 1 = Repair, 2 = Reload, 4 = Match Classlabel, 8 = Match ODFName, 16 = Match WeaponName.
extern bool GetBuildItem(const Handle h, char *ReturnODF, int *ScrapCost, const int Type = 0, const char *Classlabel = NULL, const char *ODFName = NULL, const char *WeaponName = NULL);

// Special OpenODF that only opens it if it isn't in the below ODFNames list. 
// Note: If you use this, or the below GetODF* functions, please do NOT use the traditional OpenODF(), and do NOT use the CloseODF() functions.
extern bool OpenODF2(const char *name);

// Closes all ODFs in memory opened via OpenODF2. 
// Note: If you use OpenODF2, or the below GetODF* functions, this MUST be called before the mission closes! Either in the ~Destructor, or in PostRun(). 
extern void CloseOpenODFs(void);

// Opens/Closes the BZClassic.Log file for writing. Automatically appends the current Date and Time to the end of the file name.
extern bool OpenLogFile();
extern void CloseLogFile();

// Opens/Closes and writes a message to a specified file.
extern bool FileOpen(const char *Filename, const bool Append = true);
extern void FileClose(const char *Filename);
extern void CloseOpenFiles();

// This function reads values from the maps TRN and sets the sky settings to those values. 
// Intended for use in Editor only, once at start, in Init() or InitislSetup(). Will not function in Multiplayer, and does NOT save the .Sky file. 
extern void LoadSkySettingsFromTRN(void);

// fnv-1a hash
inline unsigned int Hash(const unsigned char byte, unsigned int hash = 2166136261u)
{
	hash ^= byte;
	hash *= 16777619u;
	return hash;
}
inline unsigned int Hash(const void *data, size_t len, unsigned int hash = 2166136261u)
{
	const unsigned char * d = static_cast<const unsigned char *>(data);
	const unsigned char * const e = d + len;
	while (d < e)
		hash = Hash(*d++, hash);
	return hash;
}
inline unsigned int Hash(const char *string, unsigned int hash = 2166136261u)
{
	if (string == 0)
		return 0;
	for (const char *s = string; *s != 0; ++s)
		hash = Hash(unsigned char(tolower(*s)), hash);
	return hash;
}

// Function that switches two variables.
template <class T>
inline void SwitchVariables(T &v1, T &v2) { T tempvar = v1; v2 = v1; v1 = tempvar; }

/*
// Converts a char to a wchar string. // %S can transform this.
inline static wchar_t* CharToWChar(const char* text)
{
	size_t newsize = strlen(text) + 1;
	wchar_t *wcstring = static_cast<wchar_t *>(alloca(newsize*sizeof(wchar_t)));
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, text, newsize);

	wchar_t *Value;
	wcscpy_s(Value, sizeof(wcstring), wcstring);
	return Value;
}

// Converts a wchar to a char string. // %ls can transform this.
inline static char* WCharToChar(const wchar_t* text)
{
	size_t newsize = ((wcslen(text) + 1) * 2);
	char *string = static_cast<char *>(alloca(newsize*sizeof(char)));
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, string, newsize, text, newsize);

	char *Value;
	strcpy_s(Value, sizeof(string), string);
	return Value;
}

// Version that takes a wchar_t. 
inline void PrintConsoleMessage(wchar_t *msg)
{
	size_t newsize = ((wcslen(text) + 1) * 2);
	char *string = static_cast<char *>(alloca(newsize*sizeof(char)));
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, string, newsize, text, newsize);

	PrintConsoleMessage(string);
}
*/

/*
// This function Saves all objects on a map to a special text file to load from. 
extern void SaveMapObjectsToFile(char *FileName);

// This function Loads/places all objects on a map from a special text file made from SaveMapObjectsToFile(FileName).
extern void LoadMapObjectsFromFile(char *FileName);
*/

#endif
