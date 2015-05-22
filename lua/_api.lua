--BZ2LuaMission
local ffi = require("ffi")
ffi.cdef[[
typedef struct Vector {
	float x;
	float y;
	float z;
} Vector;
typedef struct VECTOR_2D {
	float x;
	float z;
} VECTOR_2D;
typedef float F32;
typedef struct Matrix {
	Vector right;
	F32 rightw;
	Vector up;
	F32 upw;
	Vector front;
	F32 frontw;
	Vector posit;
	F32 positw;
} Matrix;

//class AiPath;

static const long LONG_MIN = (-2147483647L - 1); /* minimum (signed) long value */
static const long LONG_MAX = 2147483647L;        /* maximum (signed) long value */

__declspec( dllexport ) int __cdecl BuildObject(char *,int,int) asm("?BuildObject@@YAHPADHH@Z");
__declspec( dllexport ) int __cdecl BuildObjectP(char *,int,char *) asm("?BuildObject@@YAHPADH0@Z");
//__declspec( dllexport ) int __cdecl BuildObject(char *,int,class AiPath *) asm("?BuildObject@@YAHPADHPAVAiPath@@@Z");//no AiPath access
__declspec( dllexport ) int __cdecl BuildObjectV(char *,int,struct Vector &) asm("?BuildObject@@YAHPADHAAUVector@@@Z");
__declspec( dllexport ) int __cdecl BuildObjectM(char *,int,struct Matrix &) asm("?BuildObject@@YAHPADHAAUMatrix@@@Z");
__declspec( dllexport ) void __cdecl RemoveObject(int) asm("?RemoveObject@@YAXH@Z");
//__declspec( dllexport ) int __cdecl GetFirstEmptyGroup(void) asm("?GetFirstEmptyGroup@@YAHXZ");//legacy
__declspec( dllexport ) void __cdecl SetGroup(int,int) asm("?SetGroup@@YAXHH@Z");
__declspec( dllexport ) void __cdecl Attack(int,int,int) asm("?Attack@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl Service(int,int,int) asm("?Service@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl GotoH(int,int,int) asm("?Goto@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl GotoP(int,char *,int) asm("?Goto@@YAXHPADH@Z");
__declspec( dllexport ) void __cdecl Mine(int,char *,int) asm("?Mine@@YAXHPADH@Z");
__declspec( dllexport ) void __cdecl Follow(int,int,int) asm("?Follow@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl Defend(int,int) asm("?Defend@@YAXHH@Z");
__declspec( dllexport ) void __cdecl Defend2(int,int,int) asm("?Defend2@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl Stop(int,int) asm("?Stop@@YAXHH@Z");
__declspec( dllexport ) void __cdecl Patrol(int,char *,int) asm("?Patrol@@YAXHPADH@Z");
__declspec( dllexport ) void __cdecl RetreatP(int,char *,int) asm("?Retreat@@YAXHPADH@Z");
__declspec( dllexport ) void __cdecl RetreatH(int,int,int) asm("?Retreat@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl GetIn(int,int,int) asm("?GetIn@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl Pickup(int,int,int) asm("?Pickup@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl Dropoff(int,char *,int) asm("?Dropoff@@YAXHPADH@Z");
__declspec( dllexport ) void __cdecl Build(int,char *,int) asm("?Build@@YAXHPADH@Z");
__declspec( dllexport ) void __cdecl LookAt(int,int,int) asm("?LookAt@@YAXHHH@Z");
__declspec( dllexport ) void __cdecl AllLookAt(int,int,int) asm("?AllLookAt@@YAXHHH@Z");
__declspec( dllexport ) bool __cdecl IsOdf(int,char *) asm("?IsOdf@@YA_NHPAD@Z");
__declspec( dllexport ) char __cdecl GetRace(int) asm("?GetRace@@YADH@Z");
//__declspec( dllexport ) int __cdecl GetPlayerHandle(void) asm("?GetPlayerHandle@@YAHXZ"); // legacy
//__declspec( dllexport ) bool __cdecl IsAlive(int &) asm("?IsAlive@@YA_NAAH@Z"); // legacy
__declspec( dllexport ) bool __cdecl IsFlying(int &) asm("?IsFlying@@YA_NAAH@Z");
//__declspec( dllexport ) bool __cdecl IsAliveAndPilot(int &) asm("?IsAliveAndPilot@@YA_NAAH@Z"); // legacy
__declspec( dllexport ) bool __cdecl IsAround(int) asm("?IsAround@@YA_NH@Z");
__declspec( dllexport ) int __cdecl InBuilding(int) asm("?InBuilding@@YAHH@Z");
__declspec( dllexport ) int __cdecl AtTerminal(int) asm("?AtTerminal@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetHandle(char *) asm("?GetHandle@@YAHPAD@Z");
__declspec( dllexport ) int __cdecl GetHandleSeq(int) asm("?GetHandle@@YAHH@Z");
__declspec( dllexport ) void __cdecl GetPositionV(int,struct Vector &) asm("?GetPosition@@YAXHAAUVector@@@Z");
__declspec( dllexport ) void __cdecl GetPosition2(int,struct Vector &) asm("?GetPosition2@@YAXHAAUVector@@@Z");
__declspec( dllexport ) void __cdecl GetFront(int,struct Vector &) asm("?GetFront@@YAXHAAUVector@@@Z");
__declspec( dllexport ) void __cdecl GetPositionM(int,struct Matrix &) asm("?GetPosition@@YAXHAAUMatrix@@@Z");
__declspec( dllexport ) void __cdecl SetPositionM(int,struct Matrix &) asm("?SetPosition@@YAXHAAUMatrix@@@Z");
__declspec( dllexport ) void __cdecl Damage(int,long) asm("?Damage@@YAXHJ@Z");
__declspec( dllexport ) float __cdecl GetHealth(int) asm("?GetHealth@@YAMH@Z");
__declspec( dllexport ) long __cdecl GetCurHealth(int) asm("?GetCurHealth@@YAJH@Z");
__declspec( dllexport ) long __cdecl GetMaxHealth(int) asm("?GetMaxHealth@@YAJH@Z");
__declspec( dllexport ) void __cdecl SetCurHealth(int,long) asm("?SetCurHealth@@YAXHJ@Z");
__declspec( dllexport ) void __cdecl SetMaxHealth(int,long) asm("?SetMaxHealth@@YAXHJ@Z");
__declspec( dllexport ) void __cdecl AddHealth(int,long) asm("?AddHealth@@YAXHJ@Z");
__declspec( dllexport ) float __cdecl GetAmmo(int) asm("?GetAmmo@@YAMH@Z");
__declspec( dllexport ) void __cdecl AddAmmo(int,long) asm("?AddAmmo@@YAXHJ@Z");
__declspec( dllexport ) long __cdecl GetCurAmmo(int) asm("?GetCurAmmo@@YAJH@Z");
__declspec( dllexport ) long __cdecl GetMaxAmmo(int) asm("?GetMaxAmmo@@YAJH@Z");
__declspec( dllexport ) void __cdecl SetCurAmmo(int,long) asm("?SetCurAmmo@@YAXHJ@Z");
__declspec( dllexport ) void __cdecl SetMaxAmmo(int,long) asm("?SetMaxAmmo@@YAXHJ@Z");
__declspec( dllexport ) int __cdecl GetTeamNum(int) asm("?GetTeamNum@@YAHH@Z");
__declspec( dllexport ) void __cdecl SetTeamNum(int,int) asm("?SetTeamNum@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetPositionP(int,char *) asm("?SetPosition@@YAXHPAD@Z");
__declspec( dllexport ) void __cdecl SetVectorPosition(int,struct Vector) asm("?SetVectorPosition@@YAXHUVector@@@Z");
__declspec( dllexport ) void __cdecl SetVelocity(int,struct Vector const &) asm("?SetVelocity@@YAXHABUVector@@@Z");
typedef struct VehicleControls {
	float braccel;
	float steer;
	float pitch;
	float strafe;
	char jump;
	char deploy;
	char eject;
	char abandon;
	char fire;
} VehicleControls;
__declspec( dllexport ) void __cdecl SetControls(int,struct VehicleControls const &,unsigned long) asm("?SetControls@@YAXHABUVehicleControls@@K@Z");
__declspec( dllexport ) int __cdecl GetWhoShotMe(int) asm("?GetWhoShotMe@@YAHH@Z");
__declspec( dllexport ) float __cdecl GetLastEnemyShot(int) asm("?GetLastEnemyShot@@YAMH@Z");
__declspec( dllexport ) float __cdecl GetLastFriendShot(int) asm("?GetLastFriendShot@@YAMH@Z");
__declspec( dllexport ) void __cdecl DefaultAllies(void) asm("?DefaultAllies@@YAXXZ");
__declspec( dllexport ) void __cdecl TeamplayAllies(void) asm("?TeamplayAllies@@YAXXZ");
__declspec( dllexport ) void __cdecl Ally(int,int) asm("?Ally@@YAXHH@Z");
__declspec( dllexport ) void __cdecl UnAlly(int,int) asm("?UnAlly@@YAXHH@Z");
__declspec( dllexport ) bool __cdecl IsAlly(int,int) asm("?IsAlly@@YA_NHH@Z");
__declspec( dllexport ) int __cdecl AudioMessage(char const *,bool) asm("?AudioMessage@@YAHPBD_N@Z");
__declspec( dllexport ) bool __cdecl IsAudioMessageDone(int) asm("?IsAudioMessageDone@@YA_NH@Z");
__declspec( dllexport ) void __cdecl StopAudioMessage(int) asm("?StopAudioMessage@@YAXH@Z");
__declspec( dllexport ) void __cdecl PreloadAudioMessage(char const *) asm("?PreloadAudioMessage@@YAXPBD@Z");
__declspec( dllexport ) void __cdecl PurgeAudioMessage(char const *) asm("?PurgeAudioMessage@@YAXPBD@Z");
__declspec( dllexport ) void __cdecl PreloadMusicMessage(char const *) asm("?PreloadMusicMessage@@YAXPBD@Z");
__declspec( dllexport ) void __cdecl PurgeMusicMessage(char const *) asm("?PurgeMusicMessage@@YAXPBD@Z");
__declspec( dllexport ) void __cdecl LoadJukeFile(char *) asm("?LoadJukeFile@@YAXPAD@Z");
__declspec( dllexport ) void __cdecl SetMusicIntensity(int) asm("?SetMusicIntensity@@YAXH@Z");
//__declspec( dllexport ) class AiPath * __cdecl FindAiPath(struct Vector const &,struct Vector const &) asm("?FindAiPath@@YAPAVAiPath@@ABUVector@@0@Z");
//__declspec( dllexport ) void __cdecl FreeAiPath(class AiPath *) asm("?FreeAiPath@@YAXPAVAiPath@@@Z");
//__declspec( dllexport ) void __cdecl GetAiPaths(int &,char * * &) asm("?GetAiPaths@@YAXAAHAAPAPAD@Z");
//__declspec( dllexport ) void __cdecl SetPathType(char *,enum PathType) asm("?SetPathType@@YAXPADW4PathType@@@Z");
__declspec( dllexport ) void __cdecl SetIndependence(int,int) asm("?SetIndependence@@YAXHH@Z");
__declspec( dllexport ) bool __cdecl IsInfo(char *) asm("?IsInfo@@YA_NPAD@Z");
__declspec( dllexport ) void __cdecl StartCockpitTimer(long,long,long) asm("?StartCockpitTimer@@YAXJJJ@Z");
__declspec( dllexport ) void __cdecl StartCockpitTimerUp(long,long,long) asm("?StartCockpitTimerUp@@YAXJJJ@Z");
__declspec( dllexport ) void __cdecl StopCockpitTimer(void) asm("?StopCockpitTimer@@YAXXZ");
__declspec( dllexport ) void __cdecl HideCockpitTimer(void) asm("?HideCockpitTimer@@YAXXZ");
__declspec( dllexport ) long __cdecl GetCockpitTimer(void) asm("?GetCockpitTimer@@YAJXZ");
__declspec( dllexport ) void __cdecl StartEarthQuake(float) asm("?StartEarthQuake@@YAXM@Z");
__declspec( dllexport ) void __cdecl UpdateEarthQuake(float) asm("?UpdateEarthQuake@@YAXM@Z");
__declspec( dllexport ) void __cdecl StopEarthQuake(void) asm("?StopEarthQuake@@YAXXZ");
__declspec( dllexport ) void __cdecl ConvertHandles(int *,int) asm("?ConvertHandles@@YAXPAHH@Z");
__declspec( dllexport ) bool __cdecl ReadBytes(void *,int) asm("?Read@@YA_NPAXH@Z");
__declspec( dllexport ) bool __cdecl ReadBool(bool *,int) asm("?Read@@YA_NPA_NH@Z");
__declspec( dllexport ) bool __cdecl ReadFloat(float *,int) asm("?Read@@YA_NPAMH@Z");
__declspec( dllexport ) bool __cdecl ReadInt(int *,int) asm("?Read@@YA_NPAHH@Z");
__declspec( dllexport ) bool __cdecl WriteBytes(void *,int) asm("?Write@@YA_NPAXH@Z");
__declspec( dllexport ) bool __cdecl WriteBool(bool *,int) asm("?Write@@YA_NPA_NH@Z");
__declspec( dllexport ) bool __cdecl WriteFloat(float *,int) asm("?Write@@YA_NPAMH@Z");
__declspec( dllexport ) bool __cdecl WriteInt(int *,int) asm("?Write@@YA_NPAHH@Z");
__declspec( dllexport ) bool __cdecl IsPerson(int) asm("?IsPerson@@YA_NH@Z");
__declspec( dllexport ) int __cdecl GetCurWorld(void) asm("?GetCurWorld@@YAHXZ");
__declspec( dllexport ) char const * __cdecl GetVarItemStr(char *) asm("?GetVarItemStr@@YAPBDPAD@Z");
__declspec( dllexport ) int const __cdecl GetVarItemInt(char *) asm("?GetVarItemInt@@YA?BHPAD@Z");
__declspec( dllexport ) int const __cdecl GetCVarItemInt(int,int) asm("?GetCVarItemInt@@YA?BHHH@Z");
__declspec( dllexport ) char const * __cdecl GetCVarItemStr(int,int) asm("?GetCVarItemStr@@YAPBDHH@Z");
__declspec( dllexport ) void __cdecl PreloadODF(char *) asm("?PreloadODF@@YAXPAD@Z");
__declspec( dllexport ) float __cdecl TerrainFindFloor(float,float) asm("?TerrainFindFloor@@YAMMM@Z");
__declspec( dllexport ) void __cdecl AddPilotByHandle(int) asm("?AddPilotByHandle@@YAXH@Z");
__declspec( dllexport ) void __cdecl PrintConsoleMessage(char*) asm("?PrintConsoleMessage@@YAXPAD@Z");
__declspec( dllexport ) float __cdecl GetRandomFloat(float) asm("?GetRandomFloat@@YAMM@Z");
__declspec( dllexport ) bool __cdecl IsDeployed(int) asm("?IsDeployed@@YA_NH@Z");
__declspec( dllexport ) void __cdecl Deploy(int) asm("?Deploy@@YAXH@Z");
__declspec( dllexport ) bool __cdecl IsSelected(int) asm("?IsSelected@@YA_NH@Z");
__declspec( dllexport ) void __cdecl SetWeaponMask(int,long) asm("?SetWeaponMask@@YAXHJ@Z");
__declspec( dllexport ) void __cdecl GiveWeapon(int,char *) asm("?GiveWeapon@@YAXHPAD@Z");
__declspec( dllexport ) void __cdecl FireAt(int,int,bool) asm("?FireAt@@YAXHH_N@Z");
__declspec( dllexport ) bool __cdecl IsFollowing(int) asm("?IsFollowing@@YA_NH@Z");
__declspec( dllexport ) int __cdecl WhoFollowing(int) asm("?WhoFollowing@@YAHH@Z");
//__declspec( dllexport ) void __cdecl SetUserTarget(int) asm("?SetUserTarget@@YAXH@Z");// legacy
//__declspec( dllexport ) int __cdecl GetUserTarget(void) asm("?GetUserTarget@@YAHXZ");// legacy
__declspec( dllexport ) void __cdecl SetPerceivedTeam(int,int) asm("?SetPerceivedTeam@@YAXHH@Z");
enum AiCommand {
	CMD_NONE,
	CMD_SELECT,
	CMD_STOP,
	CMD_GO,
	CMD_ATTACK,
	CMD_FOLLOW,
	CMD_FORMATION,
	CMD_PICKUP,
	CMD_DROPOFF,
	CMD_UNDEPLOY,
	CMD_DEPLOY,
	CMD_NO_DEPLOY,
	CMD_GET_REPAIR,
	CMD_GET_RELOAD,
	CMD_GET_WEAPON,
	CMD_GET_CAMERA,
	CMD_GET_BOMB,
	CMD_DEFEND,
	CMD_RESCUE,
	CMD_RECYCLE,
	CMD_SCAVENGE,
	CMD_HUNT,
	CMD_BUILD,
	CMD_PATROL,
	CMD_STAGE,
	CMD_SEND,
	CMD_GET_IN,
	CMD_LAY_MINES,
	CMD_LOOK_AT,
	CMD_SERVICE,
	CMD_UPGRADE,
	CMD_DEMOLISH,
	CMD_POWER,
	CMD_BACK,
	CMD_DONE,
	CMD_CANCEL,
	CMD_SET_GROUP,
	CMD_SET_TEAM,
	CMD_SEND_GROUP,
	CMD_TARGET,
	CMD_INSPECT,
	CMD_SWITCHTEAM,
	CMD_INTERFACE,
	CMD_LOGOFF,
	CMD_AUTOPILOT,
	CMD_MESSAGE,
	CMD_CLOSE,
	CMD_MORPH_SETDEPLOYED, // For morphtanks
	CMD_MORPH_SETUNDEPLOYED, // For morphtanks
	CMD_MORPH_UNLOCK, // For morphtanks
	CMD_BAILOUT,
	CMD_BUILD_ROTATE, // Update building rotations by 90 degrees.
	CMD_CMDPANEL_SELECT,
	CMD_CMDPANEL_DESELECT,

	NUM_CMD // Must be last!
}; // Don't let NUM_COMMAND go past 255, as 1 byte is used for it when sending across network
__declspec( dllexport ) enum AiCommand __cdecl GetCurrentCommand(int) asm("?GetCurrentCommand@@YA?AW4AiCommand@@H@Z");
__declspec( dllexport ) int __cdecl GetCurrentWho(int) asm("?GetCurrentWho@@YAHH@Z");
__declspec( dllexport ) void __cdecl EjectPilot(int) asm("?EjectPilot@@YAXH@Z");
__declspec( dllexport ) void __cdecl HopOut(int) asm("?HopOut@@YAXH@Z");
__declspec( dllexport ) void __cdecl KillPilot(int) asm("?KillPilot@@YAXH@Z");
__declspec( dllexport ) void __cdecl RemovePilotAI(int) asm("?RemovePilotAI@@YAXH@Z");
__declspec( dllexport ) int __cdecl HoppedOutOf(int) asm("?HoppedOutOf@@YAHH@Z");
__declspec( dllexport ) void __cdecl GetCameraPosition(struct Vector &,struct Vector &) asm("?GetCameraPosition@@YAXAAUVector@@0@Z");
__declspec( dllexport ) void __cdecl SetCameraPosition(struct Vector const &,struct Vector const &) asm("?SetCameraPosition@@YAXABUVector@@0@Z");
__declspec( dllexport ) void __cdecl ResetCameraPosition(void) asm("?ResetCameraPosition@@YAXXZ");
__declspec( dllexport ) unsigned long __cdecl CalcCRC(char *) asm("?CalcCRC@@YAKPAD@Z");
__declspec( dllexport ) void __cdecl IFace_Exec(char *) asm("?IFace_Exec@@YAXPAD@Z");
__declspec( dllexport ) void __cdecl IFace_Activate(char *) asm("?IFace_Activate@@YAXPAD@Z");
__declspec( dllexport ) void __cdecl IFace_Deactivate(char *) asm("?IFace_Deactivate@@YAXPAD@Z");
__declspec( dllexport ) void __cdecl IFace_CreateCommand(char *) asm("?IFace_CreateCommand@@YAXPAD@Z");
__declspec( dllexport ) void __cdecl IFace_CreateString(char *,char *) asm("?IFace_CreateString@@YAXPAD0@Z");
__declspec( dllexport ) void __cdecl IFace_SetString(char *,char *) asm("?IFace_SetString@@YAXPAD0@Z");
__declspec( dllexport ) void __cdecl IFace_GetString(char *,char *,int) asm("?IFace_GetString@@YAXPAD0H@Z");
__declspec( dllexport ) void __cdecl IFace_CreateInteger(char *,int) asm("?IFace_CreateInteger@@YAXPADH@Z");
__declspec( dllexport ) void __cdecl IFace_SetInteger(char *,int) asm("?IFace_SetInteger@@YAXPADH@Z");
__declspec( dllexport ) int __cdecl IFace_GetInteger(char *) asm("?IFace_GetInteger@@YAHPAD@Z");
__declspec( dllexport ) void __cdecl IFace_SetIntegerRange(char *,int,int) asm("?IFace_SetIntegerRange@@YAXPADHH@Z");
__declspec( dllexport ) void __cdecl IFace_CreateFloat(char *,float) asm("?IFace_CreateFloat@@YAXPADM@Z");
__declspec( dllexport ) void __cdecl IFace_SetFloat(char *,float) asm("?IFace_SetFloat@@YAXPADM@Z");
__declspec( dllexport ) float __cdecl IFace_GetFloat(char *) asm("?IFace_GetFloat@@YAMPAD@Z");
__declspec( dllexport ) void __cdecl IFace_ClearListBox(char *) asm("?IFace_ClearListBox@@YAXPAD@Z");
__declspec( dllexport ) void __cdecl IFace_AddTextItem(char *,char *) asm("?IFace_AddTextItem@@YAXPAD0@Z");
__declspec( dllexport ) void __cdecl IFace_GetSelectedItem(char *,char *,int) asm("?IFace_GetSelectedItem@@YAXPAD0H@Z");
__declspec( dllexport ) void __cdecl SetSkill(int,int) asm("?SetSkill@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetPlan(char *,int) asm("?SetPlan@@YAXPADH@Z");
__declspec( dllexport ) void __cdecl LogFloat(float) asm("?LogFloat@@YAXM@Z");
__declspec( dllexport ) int __cdecl GetInstantMyForce(void) asm("?GetInstantMyForce@@YAHXZ");
__declspec( dllexport ) int __cdecl GetInstantCompForce(void) asm("?GetInstantCompForce@@YAHXZ");
__declspec( dllexport ) int __cdecl GetInstantDifficulty(void) asm("?GetInstantDifficulty@@YAHXZ");
__declspec( dllexport ) int __cdecl GetInstantGoal(void) asm("?GetInstantGoal@@YAHXZ");
__declspec( dllexport ) int __cdecl GetInstantType(void) asm("?GetInstantType@@YAHXZ");
__declspec( dllexport ) int __cdecl GetInstantFlag(void) asm("?GetInstantFlag@@YAHXZ");
__declspec( dllexport ) int __cdecl GetInstantMySide(void) asm("?GetInstantMySide@@YAHXZ");
//DLLEXPORT bool DLLAPI StoppedPlayback(void); // not found in exports
__declspec( dllexport ) bool __cdecl CameraReady(void) asm("?CameraReady@@YA_NXZ");
__declspec( dllexport ) bool __cdecl CameraPath(char *,int,int,int) asm("?CameraPath@@YA_NPADHHH@Z");
__declspec( dllexport ) bool __cdecl CameraPathDir(char *,int,int) asm("?CameraPathDir@@YA_NPADHH@Z");
__declspec( dllexport ) bool __cdecl PanDone(void) asm("?PanDone@@YA_NXZ");
__declspec( dllexport ) bool __cdecl CameraObject(int,float,float,float,int) asm("?CameraObject@@YA_NHMMMH@Z");
__declspec( dllexport ) bool __cdecl CameraFinish(void) asm("?CameraFinish@@YA_NXZ");
__declspec( dllexport ) bool __cdecl CameraCancelled(void) asm("?CameraCancelled@@YA_NXZ");
__declspec( dllexport ) bool __cdecl FreeCamera(void) asm("?FreeCamera@@YA_NXZ");
__declspec( dllexport ) bool __cdecl FreeFinish(void) asm("?FreeFinish@@YA_NXZ");
__declspec( dllexport ) bool __cdecl PlayMovie(char * const) asm("?PlayMovie@@YA_NQAD@Z");
__declspec( dllexport ) void __cdecl StopMovie(void) asm("?StopMovie@@YAXXZ");
__declspec( dllexport ) bool __cdecl PlayMove(void) asm("?PlayMove@@YA_NXZ");
__declspec( dllexport ) bool __cdecl PlayRecording(char * const) asm("?PlayRecording@@YA_NQAD@Z");
__declspec( dllexport ) bool __cdecl PlayRecordingU(char * const,bool) asm("?PlayRecording@@YA_NQAD_N@Z");
__declspec( dllexport ) bool __cdecl PlaybackVehicle(char * const) asm("?PlaybackVehicle@@YA_NQAD@Z");
__declspec( dllexport ) float __cdecl SetAnimation(int,char *,int) asm("?SetAnimation@@YAMHPADH@Z");
__declspec( dllexport ) float __cdecl GetAnimationFrame(int,char *) asm("?GetAnimationFrame@@YAMHPAD@Z");
__declspec( dllexport ) void __cdecl StartAnimation(int) asm("?StartAnimation@@YAXH@Z");
__declspec( dllexport ) void __cdecl MaskEmitter(int,unsigned long) asm("?MaskEmitter@@YAXHK@Z");
__declspec( dllexport ) void __cdecl StartEmitter(int,int) asm("?StartEmitter@@YAXHH@Z");
__declspec( dllexport ) void __cdecl StopEmitter(int,int) asm("?StopEmitter@@YAXHH@Z");
//__declspec( dllexport ) void __cdecl SaveObjects(char * &,unsigned long &) asm("?SaveObjects@@YAXAAPADAAK@Z");
//__declspec( dllexport ) void __cdecl LoadObjects(char *,unsigned long) asm("?LoadObjects@@YAXPADK@Z");
//__declspec( dllexport ) void __cdecl IgnoreSync(bool) asm("?IgnoreSync@@YAX_N@Z");
//__declspec( dllexport ) bool __cdecl IsRecording(void) asm("?IsRecording@@YA_NXZ");
__declspec( dllexport ) void __cdecl SetObjectiveOn(int) asm("?SetObjectiveOn@@YAXH@Z");
__declspec( dllexport ) void __cdecl SetObjectiveOff(int) asm("?SetObjectiveOff@@YAXH@Z");
__declspec( dllexport ) void __cdecl SetObjectiveName(int,char *) asm("?SetObjectiveName@@YAXHPAD@Z");
__declspec( dllexport ) void __cdecl ClearObjectives(void) asm("?ClearObjectives@@YAXXZ");
__declspec( dllexport ) void __cdecl AddObjective(char *,long,float) asm("?AddObjective@@YAXPADJM@Z");
__declspec( dllexport ) bool __cdecl IsWithin(int &,int &,float) asm("?IsWithin@@YA_NAAH0M@Z");
__declspec( dllexport ) int __cdecl CountUnitsNearObject(int,float,int,char *) asm("?CountUnitsNearObject@@YAHHMHPAD@Z");
__declspec( dllexport ) void __cdecl SetAvoidType(int,int) asm("?SetAvoidType@@YAXHH@Z");
__declspec( dllexport ) void __cdecl Annoy(int,int) asm("?Annoy@@YAXHH@Z");
__declspec( dllexport ) void __cdecl ClearThrust(int) asm("?ClearThrust@@YAXH@Z");
__declspec( dllexport ) void __cdecl SetVerbose(int,bool) asm("?SetVerbose@@YAXH_N@Z");
__declspec( dllexport ) void __cdecl ClearIdleAnims(int) asm("?ClearIdleAnims@@YAXH@Z");
__declspec( dllexport ) void __cdecl AddIdleAnim(int,char *) asm("?AddIdleAnim@@YAXHPAD@Z");
__declspec( dllexport ) bool __cdecl IsIdle(int) asm("?IsIdle@@YA_NH@Z");
__declspec( dllexport ) void __cdecl CountThreats(int,int &,int &) asm("?CountThreats@@YAXHAAH0@Z");
__declspec( dllexport ) void __cdecl SpawnBirds(int,int,char *,int,char *) asm("?SpawnBirds@@YAXHHPADH0@Z");
__declspec( dllexport ) void __cdecl SpawnBirds(int,int,char *,int,int,int) asm("?SpawnBirds@@YAXHHPADHHH@Z");
__declspec( dllexport ) void __cdecl RemoveBirds(int) asm("?RemoveBirds@@YAXH@Z");
__declspec( dllexport ) void __cdecl SetColorFade(float,float,unsigned long) asm("?SetColorFade@@YAXMMK@Z");
__declspec( dllexport ) void __cdecl StopCheats(void) asm("?StopCheats@@YAXXZ");
__declspec( dllexport ) void __cdecl CalcCliffs(int,int,float) asm("?CalcCliffs@@YAXHHM@Z");
__declspec( dllexport ) void __cdecl CalcCliffsP(char *) asm("?CalcCliffs@@YAXPAD@Z");
__declspec( dllexport ) int __cdecl StartSoundEffect(char const *,int) asm("?StartSoundEffect@@YAHPBDH@Z");
__declspec( dllexport ) int __cdecl FindSoundEffect(char const *,int) asm("?FindSoundEffect@@YAHPBDH@Z");
__declspec( dllexport ) void __cdecl StopSoundEffect(int) asm("?StopSoundEffect@@YAXH@Z");
__declspec( dllexport ) int __cdecl GetObjectByTeamSlot(int,int) asm("?GetObjectByTeamSlot@@YAHHH@Z");
//__declspec( dllexport ) void __cdecl TranslateString(char *,char *) asm("?TranslateString@@YAXPAD0@Z");//Legacy, DO NOT USE
__declspec( dllexport ) float __cdecl portable_sin(float) asm("?portable_sin@@YAMM@Z");
__declspec( dllexport ) float __cdecl portable_cos(float) asm("?portable_cos@@YAMM@Z");
__declspec( dllexport ) float __cdecl portable_atan2(float,float) asm("?portable_atan2@@YAMMM@Z");
__declspec( dllexport ) float __cdecl portable_asin(float) asm("?portable_asin@@YAMM@Z");
__declspec( dllexport ) float __cdecl portable_acos(float) asm("?portable_acos@@YAMM@Z");
__declspec( dllexport ) bool __cdecl IsNetworkOn(void) asm("?IsNetworkOn@@YA_NXZ");
__declspec( dllexport ) bool __cdecl ImServer(void) asm("?ImServer@@YA_NXZ");
__declspec( dllexport ) bool __cdecl ImDedicatedServer(void) asm("?ImDedicatedServer@@YA_NXZ");
__declspec( dllexport ) bool __cdecl IsTeamplayOn(void) asm("?IsTeamplayOn@@YA_NXZ");
__declspec( dllexport ) int __cdecl CountPlayers(void) asm("?CountPlayers@@YAHXZ");
__declspec( dllexport ) int __cdecl GetPlayerHandle(int) asm("?GetPlayerHandle@@YAHH@Z");
__declspec( dllexport ) char __cdecl GetRaceOfTeam(int) asm("?GetRaceOfTeam@@YADH@Z");
__declspec( dllexport ) bool __cdecl IsPlayer(int) asm("?IsPlayer@@YA_NH@Z");
__declspec( dllexport ) char const * __cdecl GetPlayerName(int) asm("?GetPlayerName@@YAPBDH@Z");
__declspec( dllexport ) int __cdecl WhichTeamGroup(int) asm("?WhichTeamGroup@@YAHH@Z");
__declspec( dllexport ) int __cdecl CountAllies(int) asm("?CountAllies@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetCommanderTeam(int) asm("?GetCommanderTeam@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetFirstAlliedTeam(int) asm("?GetFirstAlliedTeam@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetLastAlliedTeam(int) asm("?GetLastAlliedTeam@@YAHH@Z");
__declspec( dllexport ) void __cdecl GetTeamplayRanges(int,int &,int &,int &) asm("?GetTeamplayRanges@@YAXHAAH00@Z");
__declspec( dllexport ) void __cdecl SetRandomHeadingAngle(int) asm("?SetRandomHeadingAngle@@YAXH@Z");
__declspec( dllexport ) void __cdecl ClearTeamColors(void) asm("?ClearTeamColors@@YAXXZ");
__declspec( dllexport ) void __cdecl DefaultTeamColors(void) asm("?DefaultTeamColors@@YAXXZ");
__declspec( dllexport ) void __cdecl TeamplayTeamColors(void) asm("?TeamplayTeamColors@@YAXXZ");
__declspec( dllexport ) void __cdecl SetTeamColor(int,int,int,int) asm("?SetTeamColor@@YAXHHHH@Z");
__declspec( dllexport ) void __cdecl ClearTeamColor(int) asm("?ClearTeamColor@@YAXH@Z");
__declspec( dllexport ) void __cdecl MakeInert(int) asm("?MakeInert@@YAXH@Z");
__declspec( dllexport ) struct Vector __cdecl GetPositionNear(struct Vector,float,float) asm("?GetPositionNear@@YA?AUVector@@U1@MM@Z");
enum RandomizeType {
	Randomize_None, // Don't modify what they selected in the shell.
	Randomize_ByRace,
	Randomize_Any,
};
__declspec( dllexport ) char * __cdecl GetPlayerODF(int,enum RandomizeType) asm("?GetPlayerODF@@YAPADHW4RandomizeType@@@Z");
__declspec( dllexport ) int __cdecl BuildEmptyCraftNear(int,char *,int,float,float) asm("?BuildEmptyCraftNear@@YAHHPADHMM@Z");
__declspec( dllexport ) void __cdecl SetCircularPos(struct Vector const &,float,float,struct Vector &) asm("?SetCircularPos@@YAXABUVector@@MMAAU1@@Z");
__declspec( dllexport ) struct Vector __cdecl GetSafestSpawnpoint(void) asm("?GetSafestSpawnpoint@@YA?AUVector@@XZ");
__declspec( dllexport ) struct Vector __cdecl GetSpawnpoint(int) asm("?GetSpawnpoint@@YA?AUVector@@H@Z");
__declspec( dllexport ) int __cdecl GetSpawnpointHandle(int) asm("?GetSpawnpointHandle@@YAHH@Z");
__declspec( dllexport ) struct Vector __cdecl GetRandomSpawnpoint(int) asm("?GetRandomSpawnpoint@@YA?AUVector@@H@Z");
__declspec( dllexport ) void __cdecl SetTimerBox(char const *) asm("?SetTimerBox@@YAXPBD@Z");
__declspec( dllexport ) void __cdecl AddToMessagesBox(char const *) asm("?AddToMessagesBox@@YAXPBD@Z");
__declspec( dllexport ) int __cdecl GetDeaths(int) asm("?GetDeaths@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetKills(int) asm("?GetKills@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetScore(int) asm("?GetScore@@YAHH@Z");
__declspec( dllexport ) void __cdecl SetDeaths(int,int) asm("?SetDeaths@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetKills(int,int) asm("?SetKills@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetScore(int,int) asm("?SetScore@@YAXHH@Z");
__declspec( dllexport ) void __cdecl AddDeaths(int,int) asm("?AddDeaths@@YAXHH@Z");
__declspec( dllexport ) void __cdecl AddKills(int,int) asm("?AddKills@@YAXHH@Z");
__declspec( dllexport ) void __cdecl AddScore(int,int) asm("?AddScore@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetAsUser(int,int) asm("?SetAsUser@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetNoScrapFlagByHandle(int) asm("?SetNoScrapFlagByHandle@@YAXH@Z");
__declspec( dllexport ) void __cdecl ClearNoScrapFlagByHandle(int) asm("?ClearNoScrapFlagByHandle@@YAXH@Z");
__declspec( dllexport ) int __cdecl GetLocalPlayerTeamNumber(void) asm("?GetLocalPlayerTeamNumber@@YAHXZ");
__declspec( dllexport ) unsigned long __cdecl GetLocalPlayerDPID(void) asm("?GetLocalPlayerDPID@@YAKXZ");
__declspec( dllexport ) void __cdecl FlagSteal(int,int) asm("?FlagSteal@@YAXHH@Z");
__declspec( dllexport ) void __cdecl FlagRecover(int,int) asm("?FlagRecover@@YAXHH@Z");
__declspec( dllexport ) void __cdecl FlagScore(int,int) asm("?FlagScore@@YAXHH@Z");
__declspec( dllexport ) void __cdecl MoneyScore(int,int) asm("?MoneyScore@@YAXHH@Z");
__declspec( dllexport ) void __cdecl NoteGameoverByTimelimit(void) asm("?NoteGameoverByTimelimit@@YAXXZ");
__declspec( dllexport ) void __cdecl NoteGameoverByKillLimit(int) asm("?NoteGameoverByKillLimit@@YAXH@Z");
__declspec( dllexport ) void __cdecl NoteGameoverByScore(int) asm("?NoteGameoverByScore@@YAXH@Z");
__declspec( dllexport ) void __cdecl NoteGameoverByLastWithBase(int) asm("?NoteGameoverByLastWithBase@@YAXH@Z");
__declspec( dllexport ) void __cdecl NoteGameoverByLastTeamWithBase(int) asm("?NoteGameoverByLastTeamWithBase@@YAXH@Z");
__declspec( dllexport ) void __cdecl NoteGameoverByNoBases(void) asm("?NoteGameoverByNoBases@@YAXXZ");
__declspec( dllexport ) void __cdecl DoGameover(float) asm("?DoGameover@@YAXM@Z");
__declspec( dllexport ) void __cdecl SetMPTeamRace(int,char) asm("?SetMPTeamRace@@YAXHD@Z");
__declspec( dllexport ) int __cdecl GetUserTarget(int) asm("?GetUserTarget@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetTarget(int) asm("?GetTarget@@YAHH@Z");
__declspec( dllexport ) void __cdecl IFace_ConsoleCmd(char const *,bool) asm("?IFace_ConsoleCmd@@YAXPBD_N@Z");
__declspec( dllexport ) void __cdecl AddToMessagesBox2(char const *,unsigned long) asm("?AddToMessagesBox2@@YAXPBDK@Z");
__declspec( dllexport ) void __cdecl Network_SetString(char *,char *) asm("?Network_SetString@@YAXPAD0@Z");
__declspec( dllexport ) void __cdecl Network_SetInteger(char *,int) asm("?Network_SetInteger@@YAXPADH@Z");
__declspec( dllexport ) int __cdecl GetFirstEmptyGroup(int) asm("?GetFirstEmptyGroup@@YAHH@Z");
__declspec( dllexport ) struct Vector __cdecl GetVelocity(int) asm("?GetVelocity@@YA?AUVector@@H@Z");
enum ObjectInfoType {
	Get_CFG, // Returns the GameObjectClass's cfg string
	Get_ODF, // Returns the ODF of the object
	Get_GOClass_gCfg, // Returns the GameObjectClass's gCfg string (not 100% sure how it differs from the CFG)
	Get_EntityType, // Returns the entity type of the object, one of the follow (though
	//   not all are possible for objects the DLL knows about). Values will be
	//	a string from the following list:

	// 	"CLASS_ID_NONE",		// undefined object (default)
	// 	"CLASS_ID_CRAFT",		// hovering vehicle
	// 	"CLASS_ID_VEHICLE",	// tracked vehicle
	// 	"CLASS_ID_TORPEDO",	// miniature vehicle
	// 	"CLASS_ID_POWERUP",	// power up pod
	// 	"CLASS_ID_PERSON",	// pilot or soldier
	// 	"CLASS_ID_ANIMAL",	// animal
	// 	"CLASS_ID_STRUCT",	// generic building
	// 	"CLASS_ID_BUILDING",	// important building
	// 	"CLASS_ID_SIGN",		// mine object
	// 	"CLASS_ID_SCRAP",		// scrap piece
	// 	"CLASS_ID_DEPOSIT",	// scrap deposit
	// 	"CLASS_ID_BEACON",	// nav beacon
	// 	"CLASS_ID_PLANT",		// plant object
	// 	"CLASS_ID_TERRAIN",	// terrain object
	// 	"CLASS_ID_WEAPON",	// weapon object
	// 	"CLASS_ID_ORDNANCE",	// ordnance object
	// 	"CLASS_ID_EXPLOSION",	// explosion object
	// 	"CLASS_ID_CHUNK",		// chunk object
	// 	"CLASS_ID_CRASH",		// crash object
	// 	"CLASS_ID_COLLAPSE",	// collapsing object

	Get_GOClass, // Returns the GameObject class of the object, one of the following (though
	// not all are possible for objects the DLL knows about). Values
	// will be a string from the following list (trimmed to 63
	// characters; the filename on each line is just a hint in case
	// you're trying to figure out what it means):

	//   "CLASS_AIR" // AirCraft.h
	//   "CLASS_ANCHORROCKET" // AnchorRocketClass.h
	//   "CLASS_APC" // APC.h
	//   "CLASS_ARCCANNON" // ArcCannonClass.h
	//   "CLASS_ARMORY" // Armory.h
	//   "CLASS_ARTIFACT" // Artifact.h
	//   "CLASS_ARTILLERY" // Artillery.h
	//   "CLASS_ASSAULTTANK" // AssaultTank.h
	//   "CLASS_BARRACKS" // Barracks.h
	//   "CLASS_BEAM"
	//   "CLASS_BLINK" // BlinkDeviceClass.h
	//   "CLASS_BOID"
	//   "CLASS_BOMBER" // Bomber.h
	//   "CLASS_BOMBERBAY" // BomberBay.h
	//   "CLASS_BOUNCEBOMB"
	//   "CLASS_BUILDING" // BuildingClass.h
	//   "CLASS_BULLET" // BulletClass.h
	//   "CLASS_CANNON" // CannonClass.h
	//   "CLASS_CANNON_MACHINEGUN" // MachineGunClass.h
	//   "CLASS_CANNON_TARGETING" // TargetingGunClass.h
	//   "CLASS_CHARGEGUN" // ChargeGunClass.h
	//   "CLASS_COMMBUNKER" // CommBunker.h
	//   "CLASS_COMMTOWER" // CommTower.h
	//   "CLASS_COMMVEHICLE" // CommVehicle.h
	//   "CLASS_CONSTRUCTIONRIG" // ConstructionRig.h
	//   "CLASS_CRAFT" // CraftClass.h
	//   "CLASS_DAMAGEFIELD" // DamageFieldClass.h
	//   "CLASS_DAYWRECKER" // DayWrecker.h
	//   "CLASS_DEPLOYABLE" // Deployable.h
	//   "CLASS_DEPLOYBUILDING" // DeployBuilding.h
	//   "CLASS_DEPOSIT" // Deposit.h
	//   "CLASS_DISPENSER" // DispenserClass.h
	//   "CLASS_EXPLOSION" // ExplosionClass.h
	//   "CLASS_EXTRACTOR" // Extractor.h
	//   "CLASS_FACTORY" // Factory.h
	//   "CLASS_FLAG" // FlagObject.h
	//   "CLASS_FLAREMINE" // FlareMineClass.h
	//   "CLASS_GAMEOBJECT" // GameObjectClass.h
	//   "CLASS_GRENADE" // GrenadeClass.h
	//   "CLASS_GRENADE_LASERPOPPER" // LaserPopperClass.h
	//   "CLASS_GRENADE_POPPER" // PopperClass.h
	// 	//  "CLASS_GRENADE_RADARPOPPER" // RadarPopperClass.h [Same sig as GRENADE_POPPER, uses that]
	//   "CLASS_HOVER" // HoverCraft.h
	//   "CLASS_HOWITZER" // Howitzer.h
	//   "CLASS_I76BUILDING" // Building.h
	//   "CLASS_JAMMER" // JammerTower.h
	//   "CLASS_JETPACK" // JetPackClass.h
	//   "CLASS_KINGOFHILL" // KingOfHill.h
	//   "CLASS_LAUNCHER" // LauncherClass.h
	//   "CLASS_LAUNCHER_IMAGE" // ImageLauncherClass.h
	//   "CLASS_LAUNCHER_MULTI" // MultiLauncherClass.h
	//   "CLASS_LAUNCHER_RADAR" // RadarLauncherClass.h
	//   "CLASS_LAUNCHER_THERMAL" // ThermalLauncherClass.h
	//   "CLASS_LAUNCHER_TORPEDO" // TorpedoLauncherClass.h
	// 	//  "CLASS_LEADER_ROUND" // LeaderRoundClass.h [Same sig as CLASS_CANNON_TARGETING, returns that]
	//   "CLASS_LOCKSHELL" // LockShellClass.h
	//   "CLASS_MAGNETGUN" // MagnetGunClass.h
	//   "CLASS_MAGNETSHELL" // MagnetShellClass.h
	//   "CLASS_MINE" // MineClass.h
	//   "CLASS_MINELAYER" // Minelayer.h
	//   "CLASS_MINE_MAGNET" // MagnetMineClass.h
	//   "CLASS_MINE_PROXIMITY" // ProximityMineClass.h
	//   "CLASS_MINE_TRIP" // TripMineClass.h
	//   "CLASS_MINE_WEAPON" // WeaponMineClass.h
	//   "CLASS_MISSILE" // MissileClass.h
	//   "CLASS_MISSILE_IMAGE" // ImageMissileClass.h
	//   "CLASS_MISSILE_LASER" // LaserMissileClass.h
	//   "CLASS_MISSILE_RADAR" // RadarMissileClass.h
	//   "CLASS_MISSILE_THERMAL" // ThermalMissileClass.h
	//   "CLASS_MORPHTANK" // MorphTank.h
	//   "CLASS_MORTAR" // MortarClass.h
	//   "CLASS_MORTAR_REMOTE" // RemoteDetonatorClass.h
	//   "CLASS_MOTIONSENSOR" // MotionSensor.h
	//   "CLASS_NAVBEACON" // NavBeaconClass.h
	//   "CLASS_OBJECTSPAWN" // ObjectSpawn.h
	//   "CLASS_ORDNANCE" // OrdnanceClass.h
	//   "CLASS_PERSON" // PersonClass.h
	//   "CLASS_PLANT" // Plant.h
	//   "CLASS_POWERED" // PoweredBuilding.h
	//   "CLASS_POWERUP_CAMERA" // CameraPod.h
	//   "CLASS_POWERUP_MONEY" // MoneyPowerup.h
	//   "CLASS_POWERUP_RELOAD" // ServicePowerup.h
	//   "CLASS_POWERUP_REPAIR" // ServicePowerup.h
	//   "CLASS_POWERUP_SERVICE" // ServicePowerup.h
	// 	//  "CLASS_POWERUP_WEAPON" // WeaponPowerup.h [Same sig as CLASS_WEAPON, returns that]
	//   "CLASS_PULSESHELL" // PulseShellClass.h
	//   "CLASS_RECYCLER" // Recycler.h
	//   "CLASS_RECYCLERVEHICLE" // RecyclerVehicle.h
	//   "CLASS_SALVOLAUNCHER" // SalvoLauncherClass.h
	//   "CLASS_SATCHELCHARGE" // SatchelCharge.h
	//   "CLASS_SATCHELPACK" // SatchelPackClass.h
	//   "CLASS_SAV" // SAV.h
	//   "CLASS_SCAVENGER" // Scavenger.h
	//   "CLASS_SCAVENGERH" // ScavengerH.h
	//   "CLASS_SCRAP" // Scrap.h
	//   "CLASS_SEEKER" // SeekerClass.h
	//   "CLASS_SEISMICWAVE" // SeismicWaveClass.h
	//   "CLASS_SERVICE" // ServiceTruck.h
	//   "CLASS_SERVICEH" // ServiceTruckH.h
	//   "CLASS_SHIELDTOWER" // ShieldTower.h
	//   "CLASS_SHIELDUP" // ShieldUpgradeClass.h
	//   "CLASS_SIGN" // BuildingClass.h
	//   "CLASS_SILO" // Silo.h
	//   "CLASS_SNIPERSHELL" // SniperShellClass.h
	//   "CLASS_SPAWNBUOY" // SpawnBuoy.h
	//   "CLASS_SPECIAL" // SpecialItemClass.h
	//   "CLASS_SPECIAL_FORCEFIELD" // ForceFieldClass.h
	//   "CLASS_SPECIAL_IMAGEREFRACT" // ImageRefractClass.h
	//   "CLASS_SPECIAL_RADARDAMPER" // RadarDamperClass.h
	//   "CLASS_SPECIAL_TERRAINEXPOSE" // TerrainExposeClass.h
	//   "CLASS_SPRAYBOMB" // SprayBombClass.h
	//   "CLASS_SPRAYBUILDING" // SprayBuildingClass.h
	//   "CLASS_SUPPLYDEPOT" // SupplyDepot.h
	//   "CLASS_TELEPORTAL" // TelePortalClass.h
	//   "CLASS_TERRAIN" // DummyClass.h
	//   "CLASS_TORPEDO" // TorpedoClass.h
	//   "CLASS_TRACKEDDEPLOYABLE" // TrackedDeployable.h
	//   "CLASS_TRACKEDVEHICLE" // TrackedVehicle.h
	//   "CLASS_TUG" // Tug.h
	//   "CLASS_TURRET" // TurretCraft.h - ibgtow/fbspir (Guntower/gunspires)
	//   "CLASS_TURRETTANK" // TurretTank.h - ivturr/fvturr (vehicle turrets)
	//   "CLASS_WALKER" // Walker.h
	//   "CLASS_WEAPON" // WeaponClass.h
	//   "CLASS_WINGMAN" // Wingman.h
	// 	"CLASS_UNKNOWN" // default

	// Returns an empty string if the handle specified is of an object
	// can't carry such a weapon, or has no weapon in that slot, etc.
	Get_Weapon0Config, 
	Get_Weapon1Config,
	Get_Weapon2Config,
	Get_Weapon3Config,
	Get_Weapon4Config,

	// Gets the weapon ODF for weapons 0-4. Returns an empty string if
	// the handle specified is of an object can't carry such a weapon,
	// or has no weapon in that slot, etc.
	Get_Weapon0ODF, 
	Get_Weapon1ODF,
	Get_Weapon2ODF,
	Get_Weapon3ODF,
	Get_Weapon4ODF,

	// Gets the GameObject class for weapons 0-4. See list above in
	// point 4 for a list of possible return values.

	Get_Weapon0GOClass, 
	Get_Weapon1GOClass,
	Get_Weapon2GOClass,
	Get_Weapon3GOClass,
	Get_Weapon4GOClass,
};
__declspec( dllexport ) bool __cdecl GetObjInfo(int,enum ObjectInfoType,char * const) asm("?GetObjInfo@@YA_NHW4ObjectInfoType@@QAD@Z");
__declspec( dllexport ) bool __cdecl DoesODFExist(char *) asm("?DoesODFExist@@YA_NPAD@Z");
__declspec( dllexport ) bool __cdecl IsAlive2(int) asm("?IsAlive2@@YA_NH@Z");
__declspec( dllexport ) bool __cdecl IsFlying2(int) asm("?IsFlying2@@YA_NH@Z");
__declspec( dllexport ) bool __cdecl IsAliveAndPilot2(int) asm("?IsAliveAndPilot2@@YA_NH@Z");
__declspec( dllexport ) void __cdecl TranslateString2(char *,unsigned int,char *) asm("?TranslateString2@@YAXPADI0@Z");
__declspec( dllexport ) int __cdecl GetScavengerCurScrap(int) asm("?GetScavengerCurScrap@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetScavengerMaxScrap(int) asm("?GetScavengerMaxScrap@@YAHH@Z");
__declspec( dllexport ) void __cdecl SetScavengerCurScrap(int,int) asm("?SetScavengerCurScrap@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetScavengerMaxScrap(int,int) asm("?SetScavengerMaxScrap@@YAXHH@Z");
__declspec( dllexport ) void __cdecl DamageF(int,float) asm("?DamageF@@YAXHM@Z");
__declspec( dllexport ) void __cdecl SelfDamage(int,float) asm("?SelfDamage@@YAXHM@Z");
__declspec( dllexport ) void __cdecl WantBotKillMessages(void) asm("?WantBotKillMessages@@YAXXZ");
__declspec( dllexport ) void __cdecl EnableHighTPS(int &) asm("?EnableHighTPS@@YAXAAH@Z");
__declspec( dllexport ) int __cdecl GetLocalUserInspectHandle(void) asm("?GetLocalUserInspectHandle@@YAHXZ");
__declspec( dllexport ) int __cdecl GetLocalUserSelectHandle(void) asm("?GetLocalUserSelectHandle@@YAHXZ");
__declspec( dllexport ) void __cdecl ResetTeamSlot(int) asm("?ResetTeamSlot@@YAXH@Z");
__declspec( dllexport ) int __cdecl GetCategoryTypeOverride(int) asm("?GetCategoryTypeOverride@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetCategoryType(int) asm("?GetCategoryType@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetODFHexInt(char const *,char const *,char const *,int *,int) asm("?GetODFHexInt@@YAHPBD00PAHH@Z");
__declspec( dllexport ) int __cdecl GetODFInt(char const *,char const *,char const *,int *,int) asm("?GetODFInt@@YAHPBD00PAHH@Z");
__declspec( dllexport ) int __cdecl GetODFLong(char const *,char const *,char const *,long *,long) asm("?GetODFLong@@YAHPBD00PAJJ@Z");
__declspec( dllexport ) int __cdecl GetODFFloat(char const *,char const *,char const *,float *,float) asm("?GetODFFloat@@YAHPBD00PAMM@Z");
__declspec( dllexport ) int __cdecl GetODFDouble(char const *,char const *,char const *,double *,double) asm("?GetODFDouble@@YAHPBD00PANN@Z");
__declspec( dllexport ) int __cdecl GetODFChar(char const *,char const *,char const *,char *,char) asm("?GetODFChar@@YAHPBD00PADD@Z");
__declspec( dllexport ) int __cdecl GetODFBool(char const *,char const *,char const *,bool *,bool) asm("?GetODFBool@@YAHPBD00PA_N_N@Z");
__declspec( dllexport ) int __cdecl GetODFString(char const *,char const *,char const *,unsigned int,char *,char const *) asm("?GetODFString@@YAHPBD00IPAD0@Z");
__declspec( dllexport ) int __cdecl GetODFColor(char const *,char const *,char const *,unsigned long *,unsigned long) asm("?GetODFColor@@YAHPBD00PAKK@Z");
__declspec( dllexport ) int __cdecl GetODFVector(char const *,char const *,char const *,struct Vector *,struct Vector) asm("?GetODFVector@@YAHPBD00PAUVector@@U1@@Z");
__declspec( dllexport ) bool __cdecl OpenODF(char *) asm("?OpenODF@@YA_NPAD@Z");
__declspec( dllexport ) bool __cdecl CloseODF(char *) asm("?CloseODF@@YA_NPAD@Z");
__declspec( dllexport ) void __cdecl NoteGameoverWithCustomMessage(char const *) asm("?NoteGameoverWithCustomMessage@@YAXPBD@Z");
__declspec( dllexport ) int __cdecl SetBestGroup(int) asm("?SetBestGroup@@YAHH@Z");
__declspec( dllexport ) void __cdecl GetGroup(int,int,enum ObjectInfoType,char * const) asm("?GetGroup@@YAXHHW4ObjectInfoType@@QAD@Z");
__declspec( dllexport ) int __cdecl GetGroupCount(int,int) asm("?GetGroupCount@@YAHHH@Z");
__declspec( dllexport ) void __cdecl SetLifespan(int,float) asm("?SetLifespan@@YAXHM@Z");
__declspec( dllexport ) bool __cdecl DoesFileExist(char const *) asm("?DoesFileExist@@YA_NPBD@Z");
__declspec( dllexport ) bool __cdecl LoadFile(char const *,void *,unsigned int &) asm("?LoadFile@@YA_NPBDPAXAAI@Z");
enum DLLAudioCategory
{
	AUDIO_CAT_UNKNOWN,
	AUDIO_CAT_UNIT_DLG,
	AUDIO_CAT_MISSION_DLG,
	AUDIO_CAT_INTERFACE,
	AUDIO_CAT_WEAPON,
	AUDIO_CAT_ORDNANCE,
	AUDIO_CAT_EXPLOSION,
	AUDIO_CAT_ENGINE,
	AUDIO_CAT_AMBIENT,
	AUDIO_CAT_MUSIC,
};
__declspec( dllexport ) int __cdecl StartAudio3D(char const * const,int,enum DLLAudioCategory,bool,bool) asm("?StartAudio3D@@YAHQBDHW4DLLAudioCategory@@_N2@Z");
__declspec( dllexport ) int __cdecl StartAudio3DV(char const * const,float,float,float,enum DLLAudioCategory,bool) asm("?StartAudio3D@@YAHQBDMMMW4DLLAudioCategory@@_N@Z");
__declspec( dllexport ) int __cdecl StartAudio2D(char const * const,float,float,float,bool,bool) asm("?StartAudio2D@@YAHQBDMMM_N1@Z");
__declspec( dllexport ) bool __cdecl IsAudioPlaying(int &) asm("?IsAudioPlaying@@YA_NAAH@Z");
__declspec( dllexport ) void __cdecl StopAudio(int) asm("?StopAudio@@YAXH@Z");
__declspec( dllexport ) void __cdecl PauseAudio(int) asm("?PauseAudio@@YAXH@Z");
__declspec( dllexport ) void __cdecl ResumeAudio(int) asm("?ResumeAudio@@YAXH@Z");
__declspec( dllexport ) void __cdecl SetVolume(int,float,bool) asm("?SetVolume@@YAXHM_N@Z");
__declspec( dllexport ) void __cdecl SetPan(int,float) asm("?SetPan@@YAXHM@Z");
__declspec( dllexport ) void __cdecl SetRate(int,float) asm("?SetRate@@YAXHM@Z");
__declspec( dllexport ) float __cdecl GetAudioFileDuration(int) asm("?GetAudioFileDuration@@YAMH@Z");
__declspec( dllexport ) bool __cdecl IsPlayingLooped(int) asm("?IsPlayingLooped@@YA_NH@Z");
__declspec( dllexport ) int __cdecl GetNearestEnemy(int,bool,bool,float) asm("?GetNearestEnemy@@YAHH_N0M@Z");
__declspec( dllexport ) int __cdecl GetNearestPowerup(int,bool,float) asm("?GetNearestPowerup@@YAHH_NM@Z");
__declspec( dllexport ) int __cdecl GetNearestPerson(int,bool,float) asm("?GetNearestPerson@@YAHH_NM@Z");
__declspec( dllexport ) void __cdecl SetCommandV(int,int,int,int,struct Vector const &,int) asm("?SetCommand@@YAXHHHHABUVector@@H@Z");
__declspec( dllexport ) void __cdecl SetCommandP(int,int,int,int,char * const,int) asm("?SetCommand@@YAXHHHHQADH@Z");
__declspec( dllexport ) void __cdecl SetGravity(float) asm("?SetGravity@@YAXM@Z");
__declspec( dllexport ) void __cdecl SetAutoGroupUnits(bool) asm("?SetAutoGroupUnits@@YAX_N@Z");
__declspec( dllexport ) void __cdecl KickPlayer(int,char const *,bool) asm("?KickPlayer@@YAXHPBD_N@Z");
__declspec( dllexport ) bool __cdecl TerrainIsWaterV(struct Vector const &) asm("?TerrainIsWater@@YA_NABUVector@@@Z");
__declspec( dllexport ) bool __cdecl TerrainIsWater(float,float) asm("?TerrainIsWater@@YA_NMM@Z");
__declspec( dllexport ) bool __cdecl TerrainGetHeightAndNormal(struct Vector const &,float &,struct Vector &,bool) asm("?TerrainGetHeightAndNormal@@YA_NABUVector@@AAMAAU1@_N@Z");
__declspec( dllexport ) bool __cdecl GetOutputPath(unsigned int &,wchar_t *) asm("?GetOutputPath@@YA_NAAIPA_W@Z");
__declspec( dllexport ) bool __cdecl GetPathPoints(char *,unsigned int &,float *) asm("?GetPathPoints@@YA_NPADAAIPAM@Z");
__declspec( dllexport ) int __cdecl GetOwner(int) asm("?GetOwner@@YAHH@Z");
__declspec( dllexport ) void __cdecl SetTarget(int,int) asm("?SetTarget@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetOwner(int,int) asm("?SetOwner@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetPilotClass(int,char *) asm("?SetPilotClass@@YAXHPAD@Z");
__declspec( dllexport ) void __cdecl AllowRandomTracks(bool) asm("?AllowRandomTracks@@YAX_N@Z");
// More teamcolors functions.
enum TEAMCOLOR_TYPE
{
	TEAMCOLOR_TYPE_DEFAULT, // Defaults
	TEAMCOLOR_TYPE_GAMEPREFS, // What's set in gameprefs
	TEAMCOLOR_TYPE_SERVER, // What the server sent us.
	TEAMCOLOR_TYPE_CURRENT, // Valid only to GetFFATeamColor().
};
__declspec( dllexport ) void __cdecl SetFFATeamColors(enum TEAMCOLOR_TYPE) asm("?SetFFATeamColors@@YAXW4TEAMCOLOR_TYPE@@@Z");
__declspec( dllexport ) void __cdecl SetTeamStratColors(enum TEAMCOLOR_TYPE) asm("?SetTeamStratColors@@YAXW4TEAMCOLOR_TYPE@@@Z");
__declspec( dllexport ) void __cdecl GetFFATeamColor(enum TEAMCOLOR_TYPE,int,int &,int &,int &) asm("?GetFFATeamColor@@YAXW4TEAMCOLOR_TYPE@@HAAH11@Z");
__declspec( dllexport ) void __cdecl GetTeamStratColor(enum TEAMCOLOR_TYPE,int,int &,int &,int &) asm("?GetTeamStratColor@@YAXW4TEAMCOLOR_TYPE@@HAAH11@Z");
__declspec( dllexport ) void __cdecl SwapTeamStratColors(void) asm("?SwapTeamStratColors@@YAXXZ");
__declspec( dllexport ) bool __cdecl GetTeamColorsAreFFA(void) asm("?GetTeamColorsAreFFA@@YA_NXZ");
__declspec( dllexport ) void __cdecl SetTeamColors(enum TEAMCOLOR_TYPE) asm("?SetTeamColors@@YAXW4TEAMCOLOR_TYPE@@@Z");
__declspec( dllexport ) int __cdecl AddPower(int,int) asm("?AddPower@@YAHHH@Z");
__declspec( dllexport ) int __cdecl SetPower(int,int) asm("?SetPower@@YAHHH@Z");
__declspec( dllexport ) int __cdecl GetPower(int) asm("?GetPower@@YAHH@Z");
__declspec( dllexport ) int __cdecl GetMaxPower(int) asm("?GetMaxPower@@YAHH@Z");
__declspec( dllexport ) void __cdecl AddMaxScrap(int,int) asm("?AddMaxScrap@@YAXHH@Z");
__declspec( dllexport ) void __cdecl AddMaxPower(int,int) asm("?AddMaxPower@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetMaxScrap(int,int) asm("?SetMaxScrap@@YAXHH@Z");
__declspec( dllexport ) void __cdecl SetMaxPower(int,int) asm("?SetMaxPower@@YAXHH@Z");
__declspec( dllexport ) void __cdecl GetTeamStratIndividualColor(enum TEAMCOLOR_TYPE,int,int &,int &,int &) asm("?GetTeamStratIndividualColor@@YAXW4TEAMCOLOR_TYPE@@HAAH11@Z");
__declspec( dllexport ) char const * __cdecl GetMapTRNFilename(void) asm("?GetMapTRNFilename@@YAPBDXZ");
__declspec( dllexport ) bool __cdecl IsTeamAllied(int,int) asm("?IsTeamAllied@@YA_NHH@Z");
__declspec( dllexport ) bool __cdecl IsNotDeadAndPilot2(int) asm("?IsNotDeadAndPilot2@@YA_NH@Z");
//__declspec( dllexport ) char const * __cdecl GetLabel(int) asm("?GetName@@YAPBDH@Z");
__declspec( dllexport ) char const * __cdecl GetLabel(int) asm("?GetLabel@@YAPBDH@Z");
//__declspec( dllexport ) void __cdecl SetLabel(int,char * const) asm("?SetName@@YAXHQAD@Z");
__declspec( dllexport ) void __cdecl SetLabel(int,char * const) asm("?SetLabel@@YAXHQAD@Z");
__declspec( dllexport ) int __cdecl GetTap(int,int) asm("?GetTap@@YAHHH@Z");
__declspec( dllexport ) void __cdecl SetTap(int,int,int) asm("?SetTap@@YAXHHH@Z");
__declspec( dllexport ) float __cdecl GetCurLocalAmmo(int,int) asm("?GetCurLocalAmmo@@YAMHH@Z");
__declspec( dllexport ) void __cdecl AddLocalAmmo(int,float,int) asm("?AddLocalAmmo@@YAXHMH@Z");
__declspec( dllexport ) float __cdecl GetMaxLocalAmmo(int,int) asm("?GetMaxLocalAmmo@@YAMHH@Z");
__declspec( dllexport ) void __cdecl SetCurLocalAmmo(int,float,int) asm("?SetCurLocalAmmo@@YAXHMH@Z");

// NEW ITEMS
/*
asm("?GetActualScrapCost@@YAHH@Z");
asm("?GetAllSpawnpoints@@YAIAAPAUSpawnpointInfo@@H@Z");
asm("?GetBaseScrapCost@@YAHH@Z");
asm("?GetNetworkListCount@@YAIW4NETWORK_LIST_TYPE@@@Z");
asm("?GetNetworkListItem@@YAPBDW4NETWORK_LIST_TYPE@@I@Z");
asm("?GetPerceivedTeam@@YAHH@Z");
asm("?GetPilotClass@@YAPBDH@Z");
asm("?GetRemainingLifespan@@YAMH@Z");
asm("?GetTeamRelationship@@YA?AW4TEAMRELATIONSHIP@@HH@Z");
asm("?Goto@@YAXHABUVector@@H@Z");
asm("?HasPilot@@YA_NH@Z");
asm("?PetWatchdogThread@@YAXXZ");
asm("?SetLastCurrentPosition@@YAXHABUMatrix@@0@Z");
asm("?SetPostTargetChangedCallback@@YAXP6AXHHH@Z@Z");
asm("?SetPreGetInCallback@@YAXP6A?AW4PreGetInReturnCodes@@HHH@Z@Z");
asm("?SetPreOrdnanceHitCallback@@YAXP6AXHHHPAD@Z@Z");
asm("?SetPrePickupPowerupCallback@@YAXP6A?AW4PrePickupPowerupReturnCodes@@HHH@Z@Z");
asm("?SetPreSnipeCallback@@YAXP6A?AW4PreSnipeReturnCodes@@HHHHPAD@Z@Z");
*/

/*typedef struct MisnImport {
	float time;
	void (__cdecl *FailMission)(float t, char* fileName);
	void (__cdecl *SucceedMission)(float t, char* fileName);
	void (__cdecl *ChangeSide)(void);
	int (__cdecl *AddScrap)(int t, int v);
	int (__cdecl *SetScrap)(int t, int v);
	int (__cdecl *GetScrap)(int t);
	int (__cdecl *GetMaxScrap)(int t);
	int (__cdecl *GetTug)(int h);
	bool (__cdecl *HasCargo)(int h);
	float (__cdecl *GetDistanceObject)(int &h1, int &h2);
	float (__cdecl *GetDistancePath)(int &h1, char* path, int point);
//	float (__cdecl *GetDistancePathPtr)(int &h1, AiPath *path, int point);
	int (__cdecl *GetNearestObject)(int h);
	int (__cdecl *GetNearestVehicleObject)(int h);
	int (__cdecl *GetNearestVehiclePath)(char* path, int point);
	int (__cdecl *GetNearestBuilding)(int h);
	int (__cdecl *GetNearestEnemy)(int h);
} MisnImport;*/
]]

--==============================================================================================================================================================
-- Utility Functions
--==============================================================================================================================================================

--- Is this oject a function?
-- Checks that the object is a function
-- @param object Object in question
function isfunction(object)
    return (type(object) == "function");
end



--- Message Buffer
-- Used to avoid extra allocation time when possible.
-- This API will avoid using this variable twice without first converting to lua strings.
local msgBuffer = ffi.new("char[1025]");

--- Convert input to CString char[]
-- This utility function is used in bindings to handle cstring reference passes.
-- This function will use the preallocated msgBuffer if possible
-- @param input The string for conversion.
-- @param doNotUseBuffer Do not use existing buffer, even if possible, defaults to false. (You may need this if an exported function has 2 cstring paramaters.)
function tocstring(input, doNotUseBuffer)
    if doNotUseBuffer or string.len(input) > 1024 then
        local longString = ffi.new("char[?]",string.len(input) + 1);
        ffi.copy(longString,input);
        return longString;
    end
    ffi.fill(msgBuffer,1025);
    ffi.copy(msgBuffer,input);
    return msgBuffer;
end

--- Clamps a number to within a certain range.
-- @param n Input number.
-- @param low Minimum of range.
-- @param high Maximum of range.
function math.clamp(n, low, high) return math.min(math.max(n, low), high) end

--- Is this oject an instance of GameObject?
-- Checks that the object is a GameObject or similar enough to one
-- @param object Object in question
function isgameobject(object)
    return (type(object) == "table" and object.GetHandle ~= nil and type(object.GetHandle) == "function");
end

function isobjectdefinition(object)
    return (type(object) == "table" 
      and object.Open ~= nil and type(object.Open) == "function"
      and object.Close ~= nil and type(object.Close) == "function"
      and object.GetHexInt ~= nil and type(object.GetHexInt) == "function"
      and object.GetInt ~= nil and type(object.GetInt) == "function"
      and object.GetLong ~= nil and type(object.GetLong) == "function"
      and object.GetFloat ~= nil and type(object.GetFloat) == "function"
      and object.GetDouble ~= nil and type(object.GetDouble) == "function"
      and object.GetChar ~= nil and type(object.GetChar) == "function"
      and object.GetBool ~= nil and type(object.GetBool) == "function"
      and object.GetString ~= nil and type(object.GetString) == "function"
      and object.GetColor ~= nil and type(object.GetColor) == "function"
      and object.GetVector ~= nil and type(object.GetVector) == "function");
end

--- Is this oject a string?
-- @param object Object in question
function isstring(object)
    return (type(object) == "string");
end

--- Is this oject a number?
-- @param object Object in question
function isnumber(object)
    return (type(object) == "number");
end

--- Is this oject a Vector?
-- @param object Object in question
function isvector(object)
    return (type(object) == "cdata" and ffi.istype("Vector", object));
end

--- Is this oject a VECTOR_2D?
-- @param object Object in question
function isvector_2d(object)
    return (type(object) == "cdata" and ffi.istype("VECTOR_2D", object));
end

--- Is this oject a Matrix?
-- @param object Object in question
function ismatrix(object)
    return (type(object) == "cdata" and ffi.istype("Matrix", object));
end

--- Is this object a Color?
-- @param object Object in question
function iscolor(object)
    return (type(object) == "table" and object.ToColorLong ~= nil and type(object.ToColorLong) == "function" and object.ToRBGA ~= nil and type(object.ToRBGA) == "function" and object.ToRBG ~= nil and type(object.ToRBG) == "function");
end

--- Is this object a TEAMCOLOR_TYPE?
-- @param object Object in question
function isteamcolortype(object)
    return (type(object) == "cdata" and ffi.istype("TEAMCOLOR_TYPE", object));
end

--- Is this object a ObjectInfoType?
-- @param object Object in question
function isobjectinfotype(object)
    return (type(object) == "cdata" and ffi.istype("ObjectInfoType", object));
end

--- Is this object a DLLAudioCategory?
-- @param object Object in question
function isaudiocategory(object)
    return (type(object) == "cdata" and ffi.istype("DLLAudioCategory", object));
end

--- Is this oject an instance of AudioHandle?
-- Checks that the object is a AudioHandle or similar enough to one
-- @param object Object in question
function isaudiohandle(object)
    return (type(object) == "table" and object.GetAudioHandle ~= nil and type(object.GetAudioHandle) == "function");
end

--- Is this oject an instance of RandomizeType?
-- @param object Object in question
function israndomizetype(object)
    return (type(object) == "cdata" and ffi.istype("RandomizeType", object));
end


--==============================================================================================================================================================
-- Common Enums, Structs, Types
--==============================================================================================================================================================

--local VECTOR_2D
VECTOR_2D = ffi.metatype("VECTOR_2D", {
  __add = function(a, b) return VECTOR_2D(a.x+b.x, a.z+b.z) end,
})

--local Vector
Vector = ffi.metatype("Vector", {
  __add = function(a, b) return Vector(a.x+b.x, a.y+b.y, a.z+b.z) end,
})

--local Matrix
Matrix = ffi.metatype("Matrix", {})

--local AnimationType = {};
AnimationType = {};
AnimationType.loop = 0;
AnimationType.twoway = 1;

--local AvoidType = {};
AvoidType = {};
AvoidType.AVD_NONE = 0; -- don't avoid collisions
AvoidType.AVD_FORCE = 1; -- use force avoidance
AvoidType.AVD_PLAN = 2; -- plan collision avoidance

--local Command = {};
Command = {};
Command.CMD_NONE = 0;
Command.CMD_SELECT = 1;
Command.CMD_STOP = 2;
Command.CMD_GO = 3;
Command.CMD_ATTACK = 4;
Command.CMD_FOLLOW = 5;
Command.CMD_FORMATION = 6; -- not used anywhere in code.
Command.CMD_PICKUP = 7;
Command.CMD_DROPOFF = 8;
Command.CMD_UNDEPLOY = 9;
Command.CMD_DEPLOY = 10;
Command.CMD_NO_DEPLOY = 11; -- Used by crigs, deploybuildings to indicate they can't do that there
Command.CMD_GET_REPAIR = 12;
Command.CMD_GET_RELOAD = 13;
Command.CMD_GET_WEAPON = 14;
Command.CMD_GET_CAMERA = 15; -- Human players only.
Command.CMD_GET_BOMB = 16;
Command.CMD_DEFEND = 17;
Command.CMD_RESCUE = 18;
Command.CMD_RECYCLE = 19;
Command.CMD_SCAVENGE = 20;
Command.CMD_HUNT = 21;
Command.CMD_BUILD = 22;
Command.CMD_PATROL = 23;
Command.CMD_STAGE = 24;
Command.CMD_SEND = 25;
Command.CMD_GET_IN = 26;
Command.CMD_LAY_MINES = 27;
Command.CMD_LOOK_AT = 28;
Command.CMD_SERVICE = 29;
Command.CMD_UPGRADE = 30;
Command.CMD_DEMOLISH = 31;
Command.CMD_POWER = 32;
Command.CMD_BACK = 33;
Command.CMD_DONE = 34;
Command.CMD_CANCEL = 35;
Command.CMD_SET_GROUP = 36;
Command.CMD_SET_TEAM = 37;
Command.CMD_SEND_GROUP = 38;
Command.CMD_TARGET = 39;
Command.CMD_INSPECT = 40;
Command.CMD_SWITCHTEAM = 41;
Command.CMD_INTERFACE = 42;
Command.CMD_LOGOFF = 43;
Command.CMD_AUTOPILOT = 44;
Command.CMD_MESSAGE = 45;
Command.CMD_CLOSE = 46;
Command.CMD_MORPH_SETDEPLOYED = 47; -- For morphtanks
Command.CMD_MORPH_SETUNDEPLOYED = 48; -- For morphtanks
Command.CMD_MORPH_UNLOCK = 49; -- For morphtanks
Command.CMD_BAILOUT = 50;
Command.CMD_BUILD_ROTATE = 51; -- Update building rotations by 90 degrees.
Command.CMD_CMDPANEL_SELECT = 52;
Command.CMD_CMDPANEL_DESELECT = 53;

--local Slots = {};
Slots = {};
Slots.DLL_TEAM_SLOT_RECYCLER = 1;
Slots.DLL_TEAM_SLOT_FACTORY = 2;
Slots.DLL_TEAM_SLOT_ARMORY = 3;
Slots.DLL_TEAM_SLOT_TRAINING = 4;
Slots.DLL_TEAM_SLOT_BOMBERBAY = 5;
Slots.DLL_TEAM_SLOT_SERVICE = 6;
Slots.DLL_TEAM_SLOT_TECHCENTER = 7;
Slots.DLL_TEAM_SLOT_COMMTOWER = 8;
Slots.DLL_TEAM_SLOT_BASE4 = 9; -- Whatever's in base slot #4 (only w/ mods)
Slots.DLL_TEAM_SLOT_BASE5 = 10; -- see above
Slots.DLL_TEAM_SLOT_BASE6 = 11; -- see above
Slots.DLL_TEAM_SLOT_BASE7 = 12; -- see above
Slots.DLL_TEAM_SLOT_BASE8 = 13; -- see above
Slots.DLL_TEAM_SLOT_BASE9 = 14; -- see above


--==============================================================================================================================================================
-- Color
--==============================================================================================================================================================

--local Color = {};
Color = {};
Color.__index = Color;
Color.__add = function(a,b)
    return Color.new((a.r + b.r) / 2, (a.g + b.g) / 2, (a.b + b.b) / 2, (a.a + b.a) / 2);
end;
Color.__newindex = function(table, key, value)
    if key == "a" or key == "r"  or key == "g"  or key == "b" then
        rawset(table, key, value);
    else
        error("Attempt to modify read-only table")
    end
end


--- Create new Color Intance
-- @param r Red or long describing color.
-- @param g Green or nil if r is long.
-- @param b Blue or nil if r is long.
-- @param a Alpha, defaults to 255, nil if r is long.
function Color.new(r,g,b,a)
  local self = setmetatable({}, Color)
  if r ~= nil and g == nil and b == nil and a == nil then
    self.a = bit.rshift(bit.band(0xFF000000,r),24);
    self.r = bit.rshift(bit.band(0x00FF0000,r),16);
    self.g = bit.rshift(bit.band(0x0000FF00,r),8);
    self.b = bit.band(0x000000FF,r);
  else
    self.r = math.clamp(r,0,255);
    self.g = math.clamp(g,0,255);
    self.b = math.clamp(b,0,255);
    if a == nill then
      self.a = 255;
    else
      self.a = math.clamp(a,0,255);
    end
  end
  return self;
end

--- Return long form color for most BZ2 functions
-- @param self Color instance
-- @return long form color
function Color.ToColorLong(self)
  return bit.bor(bit.lshift(self.a, 24), bit.lshift(self.r, 16), bit.lshift(self.g, 8), self.b);
end

--- Return RGB form color for some BZ2 functions for normal use
-- @param self Color instance
-- @return Red
-- @return Green
-- @return Blue
function Color.ToRGB(self)
    return self.r,self.g,self.b;
end

--- Return RGBA form color for normal use
-- @param self Color instance
-- @return Red
-- @return Green
-- @return Blue
-- @return Alpha
function Color.ToRGBA(self)
    return self.r,self.g,self.b,self.a;
end

--- Color long for White
Color.WHITE     = 0xFFFFFFFF;
--- Color long for Red
Color.RED       = 0xFFFF0000;
--- Color long for Green
Color.GREEN     = 0xFF00FF00;
--- Color long for Blue
Color.BLUE      = 0xFF0000FF;
--- Color long for Turquoise
Color.TURQUOISE = 0xFF00FFFF;
--- Color long for Violet
Color.VIOLET    = 0xFFFF00FF;
--- Color long for Yellow
Color.YELLOW    = 0xFFFFFF00;

--==============================================================================================================================================================
-- AiPathObject
--==============================================================================================================================================================

--FindAiPath
--FreeAiPath
--GetAiPaths
--SetPathType
--[[
__declspec( dllexport ) class AiPath * __cdecl FindAiPath(struct Vector const &,struct Vector const &) asm("?FindAiPath@@YAPAVAiPath@@ABUVector@@0@Z");
__declspec( dllexport ) void __cdecl FreeAiPath(class AiPath *) asm("?FreeAiPath@@YAXPAVAiPath@@@Z");
__declspec( dllexport ) void __cdecl GetAiPaths(int &,char * * &) asm("?GetAiPaths@@YAXAAHAAPAPAD@Z"); ??
__declspec( dllexport ) void __cdecl SetPathType(char *,enum PathType) asm("?SetPathType@@YAXPADW4PathType@@@Z"); ??

--- AiPathObject
-- An object containing all functions and data related to an AiPath
local AiPathObject = {}; -- the table representing the class, which will double as the metatable for the instances
AiPathObject.__index = AiPathObject; -- failed table lookups on the instances should fallback to the class table, to get methods
AiPathObject.__gc = function(self) ffi.C.FreeAiPath(self.GetAiPath()); end;

--- Create new GameObject Intance
-- @param id Handle from BZ2.
function AiPathObject.new(object)
  local self = setmetatable({}, AiPathObject);
  self.object = object;
  return self;
end

--- Get AiPath used by BZ2.
-- @param self AiPathObject instance.
function AiPathObject.GetAiPath(self)
  return self.object;
end
]]


--==============================================================================================================================================================
-- ObjectDefinition
--==============================================================================================================================================================

local ObjectDefinition_ = {};
ObjectDefinition_.__index = ObjectDefinition_;
ObjectDefinition_.__newindex = function(table, key, value)
    if key == "open" or key == "name" then
        rawset(table, key, value);
    else
        error("Attempt to modify read-only table")
    end
end

function ObjectDefinition(name)
    if not isstring(name) then error("Paramater name must be string."); end
    local self = setmetatable({}, ObjectDefinition_)
    self.name = name;
    return self;
end

function ObjectDefinition_.Save(self)
    WriteMarker("ObjectDefinition");
    local length = string.len(self.name);
    ffi.C.WriteInt(ffi.new("int[1]", length));
    ffi.C.WriteBytes(tocstring(self.name), length);
end

function ObjectDefinition_.Open(self)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not self.open then
       ffi.C.OpenODF(tocstring(self.name)); 
       self.open = true;
    end
end

function ObjectDefinition_.Close(self)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if self.open then
       ffi.C.CloseODF(tocstring(self.name)); 
       self.open = false;
    end
end

function ObjectDefinition_.GetHexInt(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0; end
    local value = ffi.new("int[1]");
    ffi.C.GetODFHexInt(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetInt(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0; end
    local value = ffi.new("int[1]");
    ffi.C.GetODFInt(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetLong(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0; end
    local value = ffi.new("long[1]");
    ffi.C.GetODFLong(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetFloat(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0.0; end
    local value = ffi.new("float[1]");
    ffi.C.GetODFFloat(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetDouble(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0.0; end
    local value = ffi.new("double[1]");
    ffi.C.GetODFDouble(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetChar(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = '\0'; end
    local value = ffi.new("char[1]");
    ffi.C.GetODFChar(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tostring(value);
end

function ObjectDefinition_.GetBool(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = false; end
    local value = ffi.new("bool[1]");
    ffi.C.GetODFBool(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tobool(value);
end

function ObjectDefinition_.GetString(self, block, name, size, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = nil; end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    if default ~= nil then
        ffi.C.GetODFString(tocstring(self.name), tocstring(block), tocstring(name), size, passIn, tocstring(default));
    else
        ffi.C.GetODFString(tocstring(self.name), tocstring(block), tocstring(name), size, passIn, nil);
    end
    return ffi.string(passIn);
end

function ObjectDefinition_.GetColor(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then
        default = 0;
    elseif iscolor(default) then
        default = default:ToColorLong();
    end
    local value = ffi.new("unsigned long[1]");
    ffi.C.GetODFColor(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return Color.new(value);
end

function ObjectDefinition_.GetVector(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = Vector(); end
    local value = ffi.new("Vector[1]");
    ffi.C.GetODFVector(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return value;
end



--==============================================================================================================================================================
-- AudioHandle
--==============================================================================================================================================================

local AudioHandle = {};
AudioHandle.__index = AudioHandle;
AudioHandle.__newindex = function(table, key, value)
    error("Attempt to modify read-only table")
end

function AudioHandle.new(id)
    local self = setmetatable({}, AudioHandle)
    self.handle = id;
    return self;
end

function AudioHandle.GetAudioHandle(self)
    return self.handle;
end

function AudioHandle.Save(self)
    WriteMarker("AudioHandle");
    ffi.C.WriteInt(ffi.new("int[1]", self.handle), 1);
end

--- Start audio in 3D on object
-- @param name Soundfile name
-- @param gameobject GameObject to play sound around
-- @param category DLLAudioCategory
-- @param loop Loop audio, defaults to false
-- @param stopLast Stop a prior sound on this object with the same name
-- @return AudioHandle object
function StartAudio3D(name, gameobject, category, loop, stopLast)
    if not isstring(block) then error("Paramater block must be string."); end
    if not isgameobject(gameobject) then error("Paramater gameobject must be GameObject instance."); end
    if category == nil then category = DLLAudioCategory.AUDIO_CAT_UNKNOWN; end
    if not isaudiocategory(category) then error("Paramater block must be string."); end
    --if loop == nil then loop = false; end
    --if stopLast == nil then stopLast = false; end
    return AudioHandle.new(ffi.C.StartAudio3D(tocstring(name), gameobject:GetHandle(), category, loop, stopLast));
end

--- Start audio in 3D at a location
-- @param name Soundfile name
-- @param pos Vector location for sound
-- @param category DLLAudioCategory
-- @param loop Loop audio, defaults to false
-- @return AudioHandle object
function StartAudio3DV(name, pos, category, loop)
    if not isstring(block) then error("Paramater block must be string."); end
    if not isvector(pos) then error("Paramater pos must be a Vector."); end
    if category == nil then category = DLLAudioCategory.AUDIO_CAT_UNKNOWN; end
    if not isaudiocategory(category) then error("Paramater block must be string."); end
    --if loop == nil then loop = false; end
    return ffi.C.StartAudio3DV(tocstring(name), pos.x, post.y, pos.z, category, loop);
end

--- Start audio in 2D
-- @param name Soundfile name
-- @param volume Volume, -1 uses ShellSFX volume
-- @param pan Pan
-- @param rate Rate
-- @param loop Loop audio, defaults to false, default false
-- @param isMusic Is this muic? Defaults false
-- @return AudioHandle object
function StartAudio2D(name, volume, pan, rate, loop, isMusic)
    if not isstring(block) then error("Paramater block must be string."); end
    if not isnumber(volume) then error("Paramater volume must be number."); end
    if not isnumber(pan) then error("Paramater pan must be number."); end
    if not isnumber(rate) then error("Paramater rate must be number."); end
    --if loop == nil then loop = false; end
    --if isMusic == nil then isMusic = false; end
    return AudioHandle.new(ffi.C.StartAudio2D(tocstring(name), volume, pan, rate, loop, isMusic));
end

--- Is audio playing?
-- Looped audio will be true unless a reload has occured
-- @param self AudioHandle instance.
-- @return Boolean
function AudioHandle.IsAudioPlaying(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    return ffi.C.IsAudioPlaying(self:GetAudioHandle());
end

--- Stop
-- @param self AudioHandle instance.
-- @return Boolean
function AudioHandle.StopAudio(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    ffi.C.StopAudio(self:GetAudioHandle());
end

--- Pause
-- @param self AudioHandle instance.
function AudioHandle.PauseAudio(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    ffi.C.PauseAudio(self:GetAudioHandle());
end

--- Resume
-- @param self AudioHandle instance.
function AudioHandle.ResumeAudio(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    ffi.C.ResumeAudio(self:GetAudioHandle());
end

--- Set Volume
-- @param self AudioHandle instance.
-- @param vol Volume ratio
-- @param ignoreUserVolume Do not use the user's preference as 100%
function SetVolume(self, vol, ignoreUserVolume)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    if not isnumber(vol) then error("Paramater vol must be number."); end
    ffi.C.SetVolume(self:GetAudioHandle(), vol, not ignoreUserVolume);
end

--- Set Pan
-- @param self AudioHandle instance.
-- @param pan Pan Only valid for 2D sounds, -1.0 to +1.0
function SetPan(self, pan)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    if not isnumber(vol) then error("Paramater vol must be number."); end
    ffi.C.SetPan(self:GetAudioHandle(), pan);
end

--- Set the rate
-- @param self AudioHandle instance.
-- @param rate Valid values are 0.0f .. 44100.0f. 
function SetRate(self, rate)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    if not isnumber(vol) then error("Paramater vol must be number."); end
    ffi.C.SetRate(self:GetAudioHandle(), rate);
end

--- Get duration
-- @param self AudioHandle instance.
-- @return Length in seconds
function GetAudioFileDuration(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    return ffi.C.GetAudioFileDuration(self:GetAudioHandle());
end

--- Is the playing looped
-- @param self AudioHandle instance.
-- @return Boolean
function IsPlayingLooped(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    return ffi.C.IsPlayingLooped(self:GetAudioHandle());
end


--==============================================================================================================================================================
-- GameObject
--==============================================================================================================================================================

local GameObjectMetatable = {};
GameObjectMetatable.__mode = "k";
local GameObjectWeakList = setmetatable({}, GameObjectMetatable);
local GameObjectAltered = {};

--- GameObject
-- An object containing all functions and data related to a game object.
-- [future] GameObject will survive postload.
-- [future] GameObject will preserve extra data added by serving existing instance for given id.
local GameObject = {}; -- the table representing the class, which will double as the metatable for the instances
--GameObject.__index = GameObject; -- failed table lookups on the instances should fallback to the class table, to get methods
GameObject.__index = function(table, key)
    local retVal = rawget(table, key);
    if retVal ~= nil then return retVal; end
    if rawget(table, "addonData") ~= nil and rawget(rawget(table, "addonData"), key) ~= nil then return rawget(rawget(table, "addonData"), key); end
    return rawget(GameObject, key); -- if you fail to get it from the subdata, move on to base (looking for functions)
end
GameObject.__newindex = function(table, key, value)
    if key ~= "id" and key ~= "addonData" then
        local addonData = rawget(table, "addonData");
        if addonData == nil then
            rawset(table, "addonData", {});
            addonData = rawget(table, "addonData");
        end
        rawset(addonData, key, value);
        GameObjectAltered[table:GetHandle()] = table;
    else
        rawset(table, key, value);
    end
end

--- Create new GameObject Intance
-- @param id Handle from BZ2.
function GameObject.new(id)
    if GameObjectWeakList[id] ~= nil then return GameObjectWeakList[id]; end
    local self = setmetatable({}, GameObject);
    self.id = id;
    GameObjectWeakList[id] = self;
    return self;
end

--- Get Handle used by BZ2.
-- @param self GameObject instance.
function GameObject.GetHandle(self)
    return self.id;
end

-- Save data
-- If this function is present in a table, it will save the return data instead of table elements
--function GameObject.Save(self)
--    return self.id;
--end

function GameObject.PostLoad(self)
    local id = ffi.new("int[1]", self.id);
    ffi.C.ConvertHandles(id, 1);
    GameObjectWeakList[self.id] = nil;
    local AdjustAlteredCache = GameObjectAltered[self.id] ~= nil;
    if AdjustedAlteredCache then GameObjectAltered[self.id] = nil; end
    self.id = tonumber(id[0]);
    GameObjectWeakList[self.id] = self;
    if AdjustedAlteredCache then GameObjectAltered[self.id] = self; end
end

function GameObject.Save(self)
    WriteMarker("GameObject");
    ffi.C.WriteInt(ffi.new("int[1]", self.id), 1);
end

--- Remove GameObject from world
-- @param self GameObject instance.
function GameObject.RemoveObject(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.RemoveObject(self:GetHandle());
end

--- Set group of GameObject in interface
-- @param self GameObject instance.
-- @param group Group number.
function GameObject.SetGroup(self, group)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetGroup(self:GetHandle(), group);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Orders
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Order GameObject to Attack target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.Attack(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Attack(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Service target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.Service(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Service(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Goto target GameObject / Path
-- @param self GameObject instance.
-- @param target Target GameObject or Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Goto(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if priority == nil then priority = 1; end
    if isgameobject(target) then
        ffi.C.GotoH(self:GetHandle(), target:GetHandle(), priority);
    elseif isstring(target) then
        ffi.C.GotoP(self:GetHandle(), tocstring(target), priority)
    else
        error("Paramater self must be GameObject instance or string");
    end
end

--- Order GameObject to Mine target Path
-- @param self GameObject instance.
-- @param target Target Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Mine(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    if priority == nil then priority = 1; end
    ffi.C.Mine(self:GetHandle(), tocstring(target), priority)
end

--- Order GameObject to Follow target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Follow(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Follow(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Defend area
-- @param self GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Defend(self, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Defend(self:GetHandle(), priority);
end

--- Order GameObject to Defend2 target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Defend2(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Defend2(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Stop
-- @param self GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Stop(self, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Stop(self:GetHandle(), priority);
end

--- Order GameObject to Patrol target path
-- @param self GameObject instance.
-- @param target Target Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Patrol(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    if priority == nil then priority = 1; end
    ffi.C.Patrol(self:GetHandle(), tocstring(target), priority);
end

--- Order GameObject to Retreat
-- @param self GameObject instance.
-- @param target Target GameObject or Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Retreat(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if priority == nil then priority = 1; end
    if isgameobject(target) then
        ffi.C.RetreatH(self:GetHandle(), target:GetHandle(), priority);
    elseif isstring(target) then
        ffi.C.RetreatP(self:GetHandle(), tocstring(target), priority)
    else
        error("Paramater self must be GameObject instance or string");
    end
end

--- Order GameObject to GetIn target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.GetIn(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.GetIn(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Pickup target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.Pickup(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Pickup(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Pickup target path name
-- @param self GameObject instance.
-- @param target Target path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Dropoff(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    if priority == nil then priority = 1; end
    ffi.C.Dropoff(self:GetHandle(), tocstring(target), priority)
end

--- Order GameObject to Build target config
-- Oddly this function does not include a location for the action, might want to use the far more powerful orders system.
-- @param self GameObject instance.
-- @param target Target path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Build(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    if priority == nil then priority = 1; end
    ffi.C.Build(self:GetHandle(), tocstring(target), priority);
end

--- Order GameObject to LookAt target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.LookAt(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.LookAt(self:GetHandle(), target:GetHandle(), priority);
end

--- Order entire team to look at GameObject
-- @param self GameObject instance.
-- @param target Target team number.
-- @param priority Order priority, >0 removes user control.
function GameObject.AllLookAtMe(self, team, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(team) then error("Paramater target must be number."); end
    if priority == nil then priority = 1; end
    ffi.C.AllLookAt(team, self:GetHandle(), priority);
end

--- Order GameObject to FireAt target GameObject
-- @param self GameObject instance.
-- @param target Gameobject instance.
-- @param doAim Aimfirst, defaults to false.
function GameObject.FireAt(self, target, doAim)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    ffi.C.FireAt(self:GetHandle(), target:GetHandle(), doAim);
end

--- Set command of GameObject directly
-- @param self GameObject instance.
-- @param cmd Command, see Command enum.
-- @param priority Command priority, default is 0 but only if loc is Vector
-- @param who GameObject target, defaults to 0/invalid/none but only if loc is Vector
-- @param pos Vector position or path name string
-- @param param Optional paramater interger
function GameObject.SetCommand(self, cmd, priority, who, loc, param)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(cmd) then error("Paramater target must be number."); end
    if param == nil then param = 0; end
    if isvector(pos) then
        ffi.C.SetCommandV(self:GetHandle(), cmd, priority, who.GetHandle(), where, param);
    else
        if priority == nil then priority = 0; end
        if who == nil then who = 0; end
        if isgameobject(who) then who = who.GetHandle(); end
        if path ~= nil then path = tocstring(path); end
        ffi.C.SetCommandP(self:GetHandle(), cmd, priority, who, path, param);
    end
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Position and Velocity
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get the position vector of a GameObject
-- @param self GameObject instance.
-- @return Position Vector
function GameObject.GetPositionV(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Vector();
    ffi.C.GetPositionV(self:GetHandle(), retVal);
    return retVal;
end

--- Get the position vector of a GameObject using deeper search
-- Slower search that covers recently killed or removed objects.
-- @param self GameObject instance.
-- @return Position Vector
function GameObject.GetPosition2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Vector();
    ffi.C.GetPosition2(self:GetHandle(), retVal);
    return retVal;
end

--- Get the forward facing vector of the GameObject
-- @param self GameObject instance.
-- @return Facing Vector
function GameObject.GetFront(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Vector();
    ffi.C.GetFront(self:GetHandle(), retVal);
    return retVal;
end

--- Get the position matrix of the GameObject
-- @param self GameObject instance.
-- @return Position Matrix
function GameObject.GetPositionM(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Matrix();
    ffi.C.GetPositionM(self:GetHandle(), retVal);
    return retVal;
end

--- Set the position of the GameObject
-- @param self GameObject instance.
-- @param loc Vector or Matrix position, or path point string
function GameObject.SetPosition(self, pos)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if ismatrix(loc) then
        ffi.C.GetPositionM(self:GetHandle(), pos);
    elseif isvector(loc) then
        ffi.C.SetVectorPosition(self:GetHandle(), pos);
    elseif isstring(loc) then
        ffi.C.SetPositionP(self:GetHandle(), tocstring(pos));
    else
        error("Paramater loc must be Vector, Matrix, or path point name (string).");
    end
end

--- Get the velocity vector of the GameObject
-- @param self GameObject instance.
-- @return Velocity vector.
function GameObject.GetVelocity(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetVelocity(self:GetHandle());
end

--- Set the velocity vector of the GameObject
-- @param self GameObject instance.
-- @param vel Velocity vector.
function GameObject.SetVelocity(self, vel)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isvector(vel) then error("Paramater vel must be Vector instance."); end
    ffi.C.SetVelocity(self:GetHandle(), vel);
end

--- Set a random heading angle for the GameObject
-- @param self GameObject instance.
function GameObject.SetRandomHeadingAngle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.SetRandomHeadingAngle(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Health / Damage
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Applys damage to the GameObject crediting last damager
-- @param self GameObject instance.
-- @param amt Ammount of Damage, support whole numbers.
function GameObject.Damage(self, amt)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    ffi.C.Damage(self:GetHandle(), amt);
end

--- Applys damage to the GameObject crediting last damager
-- @param self GameObject instance.
-- @param amt Ammount of Damage, supports decimal numbers.
function GameObject.DamageF(self, amt)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    ffi.C.DamageF(self:GetHandle(), amt);
end

--- Applys damage to the GameObject with the doer as self
-- @param self GameObject instance.
-- @param amt Ammount of Damage, supports decimal numbers.
function GameObject.SelfDamage(self, amt)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    ffi.C.SelfDamage(self:GetHandle(), amt);
end

--- Get the health of the GameObject
-- @param self GameObject instance.
-- @return Health
function GameObject.GetHealth(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    return ffi.C.GetHealth(self:GetHandle());
end

--- Get the current health of the GameObject
-- @param self GameObject instance.
-- @return Health
function GameObject.GetCurHealth(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetCurHealth(self:GetHandle());
end

--- Get the max health of the GameObject
-- @param self GameObject instance.
-- @return Health
function GameObject.GetMaxHealth(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetMaxHealth(self:GetHandle());
end

--- Sets the current health of the GameObject
-- @param self GameObject instance.
-- @param NewHealth New health value.
function GameObject.SetCurHealth(self, NewHealth)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewHealth) then error("Paramater NewHealth must be a number."); end
    ffi.C.SetCurHealth(self:GetHandle(), NewHealth);
end

--- Sets the max health of the GameObject
-- @param self GameObject instance.
-- @param NewHealth New health value.
function GameObject.SetMaxHealth(self, NewHealth)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewHealth) then error("Paramater NewHealth must be a number."); end
    ffi.C.SetMaxHealth(self:GetHandle(), NewHealth);
end

--- Adds health to the GameObject
-- @param self GameObject instance.
-- @param health New health value.
function GameObject.AddHealth(self, health)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(health) then error("Paramater health must be a number."); end
    ffi.C.AddHealth(self:GetHandle(), health)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Ammo / Local Ammo
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get the ammo of the GameObject
-- @param self GameObject instance.
-- @return Ammo
function GameObject.GetAmmo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetAmmo(self:GetHandle());
end

--- Adds ammo to the GameObject
-- @param self GameObject instance.
-- @param ammo Ammo to add.
function GameObject.AddAmmo(self, ammo)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammo) then error("Paramater health must be a number."); end
    ffi.C.AddAmmo(self:GetHandle(), ammo);
end

--- Get the current ammo of the GameObject
-- @param self GameObject instance.
-- @return Current Ammo.
function GameObject.GetCurAmmo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetCurAmmo(self:GetHandle());
end

--- Get the max ammo of the GameObject
-- @param self GameObject instance.
-- @return Max Ammo.
function GameObject.GetMaxAmmo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetMaxAmmo(self:GetHandle());
end

--- Set current ammo of the GameObject
-- @param self GameObject instance.
-- @param NewAmmo Ammo to set.
function GameObject.SetCurAmmo(self, NewAmmo)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammo) then error("Paramater ammo must be a number."); end
    ffi.C.SetCurAmmo(self:GetHandle(), NewAmmo);
end

--- Set max ammo of the GameObject
-- @param self GameObject instance.
-- @param NewAmmo Max ammo to set.
function GameObject.SetMaxAmmo(self, NewAmmo)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammo) then error("Paramater ammo must be a number."); end
    ffi.C.SetMaxAmmo(self:GetHandle(), NewAmmo);
end

--- Get the current local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param weaponIndex Weapon index.
-- @return Current local ammo.
function GameObject.GetCurLocalAmmo(self, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    return ffi.C.GetCurLocalAmmo(self:GetHandle(), weaponIndex)
end

--- Add to the current local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param ammount Ammo to add.
-- @param weaponIndex Weapon index.
function GameObject.AddLocalAmmo(self, ammount, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a number."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    ffi.C.AddLocalAmmo(self:GetHandle(), ammount, weaponIndex)
end

--- Get the max local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param weaponIndex Weapon index.
-- @return Max local ammo.
function GameObject.GetMaxLocalAmmo(self, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    return ffi.C.GetMaxLocalAmmo(self:GetHandle(), weaponIndex)
end

--- Set the current local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param ammount Ammo to set.
-- @param weaponIndex Weapon index.
function GameObject.SetCurLocalAmmo(self, ammount, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a number."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    ffi.C.SetCurLocalAmmo(self:GetHandle(), ammount, weaponIndex)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Animation and Effects
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Set the aimation of the GameObject
-- @param self GameObject instance.
-- @param name name of the animation
-- @param type Animation type, referance AnimationType
-- @return max frames
function GameObject.SetAnimation(self, name, type)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    if not isnumber(type) then error("Paramater type must be a number."); end
    if type == nil then type = 0; end
    type = math.clamp(type,0,1);
    return ffi.C.SetAnimation(self:GetHandle(), tocstring(name), type);
end

--- Get the aimation of the GameObject
-- @param self GameObject instance.
-- @param name name of the animation
-- @return current frame
function GameObject.GetAnimationFrame(self, name)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    return ffi.C.GetAnimationFrame(self:GetHandle(), tocstring(name));
end

--- Start current aimation of the GameObject
-- @param self GameObject instance.
function GameObject.StartAnimation(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.StartAnimation(self:GetHandle());
end

--- Start current emmiter mask of the GameObject
-- [todo] check if mask is properly handled, might need to do some bitwise stuff
-- @param self GameObject instance.
function GameObject.MaskEmitter(self, mask)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(mask) then error("Paramater mask must be a number."); end
    ffi.C.MaskEmitter(self:GetHandle(), mask);
end

--- Start the emmiter in the selected slot of the GameObject
-- @param self GameObject instance.
-- @param slot Emitter slot.
function GameObject.StartEmitter(self, slot)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(slot) then error("Paramater slot must be a number."); end
    ffi.C.StartEmitter(self:GetHandle(), slot);
end

--- Start the emmiter in the selected slot of the GameObject
-- @param self GameObject instance.
-- @param slot Emitter slot.
function GameObject.StopEmitter(self, slot)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(slot) then error("Paramater slot must be a number."); end
    ffi.C.StopEmitter(self:GetHandle(), slot);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Idel State and Animations
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Clear idel animations on this GameObject.
-- @param self GameObject instance.
function GameObject.ClearIdleAnims(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ClearIdleAnims(self:GetHandle());
end

--- Add an idel animation to this GameObject.
-- @param self GameObject instance.
-- @param name Animation name.
function GameObject.AddIdleAnim(self, name)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    ffi.C.AddIdleAnim(self:GetHandle(), tocstring(name));
end

--- Is this GameObject idel.
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsIdle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsIdle(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Objectification
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Objectify the GameObject
-- @param self GameObject instance.
function GameObject.SetObjectiveOn(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetObjectiveOn(self:GetHandle());
end

--- Objectify the GameObject
-- @param self GameObject instance.
function GameObject.SetObjectiveOff(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetObjectiveOff(self:GetHandle());
end

--- Set the objective name of the GameObject
-- @param self GameObject instance.
-- @param name Name for the objective.
function GameObject.SetObjectiveName(self, name)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    ffi.C.SetObjectiveName(self:GetHandle(), tocstring(name));
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - CTF Flag and Loot Money
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Trigger a flag steal of this object by target GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagStealBy(self, holder)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(holder) then error("Paramater holder must be GameObject instance."); end
    ffi.C.FlagSteal(self:GetHandle(), holder.GetHandle());
end

--- Trigger a flag recovery of this object by target GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagRecoverBy(self, holder)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(holder) then error("Paramater holder must be GameObject instance."); end
    ffi.C.FlagRecover(self:GetHandle(), holder.GetHandle());
end

--- Trigger a flag score of this object by target GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagScoreBy(self, holder)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(holder) then error("Paramater holder must be GameObject instance."); end
    ffi.C.FlagScore(self:GetHandle(), holder.GetHandle());
end

--- Trigger a flag steal of the target by this GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagSteal(self, flag)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(flag) then error("Paramater flag must be GameObject instance."); end
    ffi.C.FlagSteal(flag.GetHandle(), self:GetHandle());
end

--- Trigger a flag recover of the target by this GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagRecover(self, flag)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(flag) then error("Paramater flag must be GameObject instance."); end
    ffi.C.FlagRecover(flag.GetHandle(), self:GetHandle());
end

--- Trigger a flag score of the target by this GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagScore(self, flag)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(flag) then error("Paramater flag must be GameObject instance."); end
    ffi.C.FlagScore(flag.GetHandle(), self:GetHandle());
end

--- Trigger a money score by this GameObject
-- @param self GameObject instance.
-- @param ammount Score to award.
function GameObject.MoneyScore(self, ammount)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a string."); end
    ffi.C.MoneyScore(ammount, self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Team
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get team of GameObject
-- @param self GameObject instance.
-- @return Team number.
function GameObject.GetTeamNum(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetTeamNum(self:GetHandle());
end

--- Set team of GameObject
-- @param self GameObject instance.
-- @param t Team number.
function GameObject.SetTeamNum(self, t)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a string."); end
    ffi.C.SetTeamNum(self:GetHandle(), t);
end

--- Set team of GameObject
-- Note aliances are 1 directional, be sure to check both ways
-- @param self GameObject instance.
-- @param him GameObject instance.
-- @return Is ally bool.
function GameObject.IsAlly(self, him)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(him) then error("Paramater him must be GameObject instance."); end
    return ffi.C.IsAlly(self:GetHandle(), him);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Deaths
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get deaths of GameObject
-- @param self GameObject instance.
-- @return Death count
function GameObject.GetDeaths(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetDeaths(self:GetHandle());
end

--- Set deaths of GameObject
-- @param self GameObject instance.
-- @param NewValue GameObject instance.
function GameObject.SetDeaths(self, NewValue)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewValue) then error("Paramater NewValue must be GameObject instance."); end
    ffi.C.SetDeaths(self:GetHandle(), NewValue);
end

--- Add to deaths of GameObject
-- @param delta GameObject instance.
-- @param him GameObject instance.
function GameObject.AddDeaths(self, delta)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(delta) then error("Paramater delta must be GameObject instance."); end
    ffi.C.AddDeaths(self:GetHandle(), delta)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Kills
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get kills of GameObject
-- @param self GameObject instance.
-- @return Kill count
function GameObject.GetKills(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetKills(self:GetHandle());
end

--- Set kills of GameObject
-- @param self GameObject instance.
-- @param NewValue GameObject instance.
function GameObject.SetKills(self, NewValue)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewValue) then error("Paramater NewValue must be GameObject instance."); end
    return ffi.C.SetKills(self:GetHandle(), NewValue);
end

--- Add to kills of GameObject
-- @param delta GameObject instance.
-- @param him GameObject instance.
function GameObject.AddKills(self, delta)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(delta) then error("Paramater delta must be GameObject instance."); end
    ffi.C.AddKills(self:GetHandle(), delta)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Score
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get score of GameObject
-- @param self GameObject instance.
-- @return Score count
function GameObject.GetScore(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetScore(self:GetHandle());
end

--- Set score of GameObject
-- @param self GameObject instance.
-- @param NewValue GameObject instance.
function GameObject.SetScore(self, NewValue)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewValue) then error("Paramater NewValue must be GameObject instance."); end
    return ffi.C.SetScore(self:GetHandle(), NewValue);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Scavenger
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get current scrap in hold of GameObject
-- @param self GameObject instance.
function GameObject.GetScavengerCurScrap(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetScavengerCurScrap(self:GetHandle());
    if retVal == -1 then return nil; end
    return GameObject.new(retVal);
end

--- Get max scrap in hold of GameObject
-- @param self GameObject instance.
function GameObject.GetScavengerMaxScrap(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetScavengerMaxScrap(self:GetHandle());
    if retVal == -1 then return nil; end
    return GameObject.new(retVal);
end

--- Set current scrap in hold of GameObject
-- @param self GameObject instance.
-- @param aNewScrap new scrap ammount.
function GameObject.SetScavengerCurScrap(self,aNewScrap)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetScavengerCurScrap(self:GetHandle(),aNewScrap);
end

--- Set max scrap in hold of GameObject
-- @param self GameObject instance.
-- @param aNewScrap new scrap ammount.
function GameObject.SetScavengerMaxScrap(self,aNewScrap)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetScavengerMaxScrap(self:GetHandle(),aNewScrap);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Scavenger
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get nearest enemy to this GameObject
-- @param self GameObject instance.
-- @param ignorePilots Ignore pilots?
-- @param ignoreScavs Ignore scavengers?
-- @param maxDist Max distance to search, hard ceiling of 450.0, which is the max
-- @return Gameobject found
function GameObject.GetNearestEnemy(self, ignorePilots, ignoreScavs, maxDist)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    -- can't check for bools since nil = false and !nil = true
    if maxDist == nil then maxDist = 450.0; end
    return GameObject.new(ffi.C.GetNearestEnemy(self.handle, ignorePilots, ignoreScavs, maxDist));
end

--- Get nearest powerup to this GameObject
-- @param self GameObject instance.
-- @param ignoreSpawnpoints Ignore spawn points?
-- @param maxDist Max distance to search, hard ceiling of 450.0, which is the max
-- @return Gameobject found
function GameObject.GetNearestPowerup(self, ignoreSpawnpoints, maxDist)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    -- can't check for bools since nil = false and !nil = true
    if maxDist == nil then maxDist = 450.0; end
    return GameObject.new(ffi.C.GetNearestPowerup(self.handle, ignoreSpawnpoints, maxDist));
end

--- Get nearest person to this GameObject
-- @param self GameObject instance.
-- @param skipFriendlies Ignore friendly objects?
-- @param maxDist Max distance to search, hard ceiling of 450.0, which is the max
-- @return GameObject found.
function GameObject.GetNearestPerson(self, skipFriendlies, maxDist)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    -- can't check for bools since nil = false and !nil = true
    if maxDist == nil then maxDist = 450.0; end
    return GameObject.new(ffi.C.GetNearestPerson(self.handle, skipFriendlies, maxDist));
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Actions
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Deploy this GameObject
-- @param self GameObject instance.
function GameObject.Deploy(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.Deploy(self:GetHandle());
end

--- Annoy the target GameObject with this GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
function GameObject.Annoy(self, target)
  ffi.C.Annoy(self:GetHandle(), target:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Order States and General States
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Is this GameObject following something?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsFollowing(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsFollowing(self:GetHandle());
end

--- Get what this GameObject is following
-- @param self GameObject instance.
-- @return GameObject we are following.
function GameObject.WhoFollowing(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.WhoFollowing(self:GetHandle()));
end

--- Get current command
-- @param self GameObject instance.
-- @return AiCommand enum value
function GetCurrentCommand(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetCurrentCommand(self:GetHandle());
end

--- Get the subject of this GameObject's current order
-- @param self GameObject instance.
-- @return GameObject subject of our orders.
function GameObject.GetCurrentWho(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.GetCurrentWho(self:GetHandle()));
end

--- Is this GameObject deployed?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsDeployed(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsDeployed(self:GetHandle());
end

--- Is this GameObject selected?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsSelected(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsSelected(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Pilot Operations
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Eject the pilot from the GameObject
-- @param self GameObject instance.
function GameObject.EjectPilot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.EjectPilot(self:GetHandle());
end

--- Hop the pilot out from the GameObject
-- @param self GameObject instance.
function GameObject.HopOut(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.HopOut(self:GetHandle());
end

--- Kill the pilot of the GameObject
-- @param self GameObject instance.
function GameObject.KillPilot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.KillPilot(self:GetHandle());
end

--- Remove the AI of the GameObject without killing or removing the pilot
-- @param self GameObject instance.
function GameObject.RemovePilotAI(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.RemovePilotAI(self:GetHandle());
end

--- The GameObject which this GameObject hopped out of
-- @param self GameObject instance.
-- @return GameObject that this hopped out of.
function GameObject.HoppedOutOf(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.HoppedOutOf(self:GetHandle()));
end

--- Adds a pilot if needed to the GameObject
-- @param self GameObject instance.
function GameObject.AddPilot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.AddPilotByHandle(self:GetHandle())
end

--- Sets the pilot class of he GameObject
-- @param self GameObject instance.
-- @param odf Object Definition of pilot.
function GameObject.SetPilotClass(self, odf)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(odf) then error("Paramater odf must be a string."); end
    ffi.C.SetPilotClass(self:GetHandle(), tocstring(odf));
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Status
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Is the GameObject alive?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsAlive2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsAlive2(self:GetHandle());
end

--- Is the GameObject flying?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsFlying2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsFlying2(self:GetHandle());
end

--- Is the GameObject alive and pilot?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsAliveAndPilot2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsAliveAndPilot2(self:GetHandle());
end

--- Is the GameObject not dead and pilot?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsNotDeadAndPilot2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsNotDeadAndPilot2(self:GetHandle());
end

--- The GameObject is around
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsAround(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsAround(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Shooting
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Who shot this GameObject?
-- @param self GameObject instance.
-- @return Shooter GameObject.
function GameObject.GetWhoShotMe(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return Handle.new(ffi.C.GetWhoShotMe(self:GetHandle()));
end

--- Get last time this GameObject shot an enemy
-- @param self GameObject instance.
-- @return time of last event.
function GameObject.GetLastEnemyShot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetLastEnemyShot();
end

--- Get last time this GameObject shot an friend
-- @param self GameObject instance.
-- @return time of last event.
function GameObject.GetLastFriendShot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetLastFriendShot();
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Other (Too Many...)
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Owner of this GameObject
-- @param self GameObject instance.
-- @return owner GameObject.
function GameObject.GetOwner(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.GetOwner(self:GetHandle()));
end

--- Set the target of this GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
function GameObject.SetTarget(self, target)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    GameObject.new(ffi.C.SetTarget(self:GetHandle(), target));
end

--- Set the owner of this GameObject
-- @param self GameObject instance.
-- @param owner Owner GameObject.
function GameObject.SetOwner(self, owner)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(owner) then error("Paramater owner must be GameObject instance."); end
    GameObject.new(ffi.C.SetOwner(self:GetHandle(), owner));
end

--- Is this GameObject an instance of given odf?
-- @param self GameObject instance.
-- @param odf ODF to check.
-- @return boolean, this GameObject is an instance of given odf.
function GameObject.IsOdf(self, odf)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(odf) then error("Paramater odf must be a string."); end
    local odfClean = tocstring(odf);
    return ffi.C.IsOdf(self:GetHandle(), odfClean);
end

--- Get the race character of the GameObject
-- @param self GameObject instance.
-- @return race character.
function GameObject.GetRace(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetRace(self:GetHandle());
end

--- Get the building the GameObject is in
-- @param self GameObject instance.
-- @return Building the GameObject is in.
function GameObject.InBuilding(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return Handle.new(ffi.C.InBuilding(self:GetHandle()));
end

--- Get the terminal the GameObject is at
-- @param self GameObject instance.
-- @return Terminal the GameObject is at.
function GameObject.AtTerminal(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return Handle.new(ffi.C.AtTerminal(self:GetHandle()));
end

--- Set the controls of the GameObject
-- @param self GameObject instance.
-- @param controls Controls object, nil results in clear control set.
-- @param CTRL_BRACCEL Apply braccel control? Default false.
-- @param CTRL_STEER Apply steer control? Default false.
-- @param CTRL_PITCH Apply pitch control? Default false.
-- @param CTRL_STRAFE Apply strafe control? Default false.
-- @param CTRL_JUMP Apply jump control? Default false.
-- @param CTRL_DEPLOY Apply deploy control? Default false.
-- @param CTRL_EJECT Apply eject control? Default false.
-- @param CTRL_ABANDON Apply abandon control? Default false.
-- @param CTRL_FIRE Apply fire control? Default false.
function GameObject.SetControls(self, controls, CTRL_BRACCEL, CTRL_STEER, CTRL_PITCH, CTRL_STRAFE, CTRL_JUMP, CTRL_DEPLOY, CTRL_EJECT, CTRL_ABANDON, CTRL_FIRE)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local whichControls = 0;
    if CTRL_BRACCEL then whichControls = whichControls +   1; end
    if CTRL_STEER   then whichControls = whichControls +   2; end
    if CTRL_PITCH   then whichControls = whichControls +   4; end
    if CTRL_STRAFE  then whichControls = whichControls +   8; end
    if CTRL_JUMP    then whichControls = whichControls +  16; end
    if CTRL_DEPLOY  then whichControls = whichControls +  32; end
    if CTRL_EJECT   then whichControls = whichControls +  64; end
    if CTRL_ABANDON then whichControls = whichControls + 128; end
    if CTRL_FIRE    then whichControls = whichControls + 256; end
    if controls == nil then controls = VehicleControls(); end
    ffi.C.SetControls(self.GetHandle(), controls, whichControls);
end

--- Set the independence of the GameObject
-- @param self GameObject instance.
-- @param independence 1 = high, 0 = low.
function GameObject.SetIndependence(self, independence)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetIndependence(self:GetHandle(), independence);
end

--- Set the weapon mask of the GameObject
-- @param self GameObject instance.
-- @param mask A long of bits, similar to the mask in ODFs but converted to a decimal number.
function GameObject.SetWeaponMask(self, mask)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(mask) then error("Paramater mask must be a number"); end
    ffi.C.SetWeaponMask(self:GetHandle(), mask);
end

--- Give a weapon to the GameObject
-- This function runs through normal powerup pickup logic.
-- For more control you might need to get creative with checking weapons and giving weapons.
-- @param self GameObject instance.
-- @param weapon Weapon Name.
function GameObject.GiveWeapon(self, weapon)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(weapon) then error("Paramater weapon must be a string"); end
    ffi.C.GiveWeapon(self:GetHandle(), tocstring(weapon));
end

--- Set the perceived team of the GameObject
-- @param self GameObject instance.
-- @param team
function GameObject.SetPerceivedTeam(self, team)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(team) then error("Paramater team must be a number"); end
    ffi.C.SetPerceivedTeam(self:GetHandle(), team);
end

--- Set the skill level of the GameObject
-- @param self GameObject instance.
-- @param s Skill level.
function GameObject.SetSkill(self, s)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(s) then error("Paramater s must be a number"); end
    ffi.C.SetSkill(self:GetHandle(), s);
end

--- Check if the GameObject is within given distance of another object.
-- @param self GameObject instance.
-- @param h2 Other Object.
-- @param distance Distance to check within.
function GameObject.IsWithin(self, h2, distance)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(h2) then error("Paramater h2 must be GameObject instance."); end
    if not isnumber(distance) then error("Paramater distance must be a number"); end
    return ffi.C.IsWithin(self:GetHandle(), h2:GetHandle(), distance);
end

--- Count untits near GameObject.
-- [future] test to see what the deal is with this function, can it have some things left out?
-- @param self GameObject instance.
-- @param dist Distance to check within.
-- @param team Team filter.
-- @param odf ODF
-- @return count of objects
function GameObject.CountUnitsNearObject(self, dist, team, odf)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(dist) then error("Paramater dist must be a number"); end
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isstring(odf) then error("Paramater odf must be a string"); end
    return ffi.C.CountUnitsNearObject(self:GetHandle(), dist, team, tocstring(odf));
end

--- Set avoid type of GameObject.
-- @param self GameObject instance.
-- @param avoidType See AvoidType table.
function GameObject.SetAvoidType(self, avoidType)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(avoidType) then error("Paramater avoidType must be a number"); end
    ffi.C.SetAvoidType(self:GetHandle(), avoidType);
end

--- Clear the thrust of the GameObject.
-- @param self GameObject instance.
function GameObject.ClearThrust(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ClearThrust(self:GetHandle());
end

--- Clear the thrust of the GameObject.
-- @param self GameObject instance.
-- @param on Boolean
function GameObject.SetVerbose(self, on)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetVerbose(self:GetHandle(), on);
end

--- Clear the thrust of the GameObject.
-- @param self GameObject instance.
-- @return Here
-- @return Coming
function GameObject.CountThreats(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local here = ffi.new("int[1]");
    local coming = ffi.new("int[1]");
    ffi.C.CountThreats(self:GetHandle(), here, coming);
    return tonumber(here[0]),tonumber(coming[0]);
end

--- Is this GameObject the player?
-- @param self GameObject instance.
-- @return Boolean
function GameObject.IsPlayer(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsPlayer(self:GetHandle());
end

--- Get this player's name.
-- @param self GameObject instance.
-- @return Player name
function GameObject.GetPlayerName(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetPlayerName(self:GetHandle());
end

--- Make the GameObject inert.
-- This function was used on the dedicated server host back when that was attempted.
-- @param self GameObject instance.
function GameObject.MakeInert(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.MakeInert(self:GetHandle());
end

--- Get the GameObject's label (set in editor)
-- @param self GameObject instance.
-- @return Object label
function GameObject.GetLabel(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetLabel(self:GetHandle());
end

--- Set the GameObject's label
-- @param self GameObject instance.
-- @param pLabel New object label
function GameObject.SetLabel(self, pLabel)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(pLabel) then error("Paramater pLabel must be a string."); end
    ffi.C.SetLabel(self:GetHandle(), tocstring(pLabel));
end

--- Get the GameObject's Tap by index
-- @param self GameObject instance.
-- @param index Tap index.
-- @return Tap object
function GameObject.GetTap(self, index)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(index) then error("Paramater index must be a number."); end
    return GameObject.new(ffi.C.GetTap(self:GetHandle(), index));
end

--- Get the GameObject's Tap by index
-- @param self GameObject instance.
-- @param index Tap index.
-- @param tapObjectHandle Object to set as tap.
function GameObject.SetTap(self, index, tapObjectHandle)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(index) then error("Paramater index must be a number."); end
    if not isgameobject(tapObjectHandle) then error("Paramater tapObjectHandle must be GameObject instance."); end
    ffi.C.SetTap(self:GetHandle(), index, tapObjectHandle.GetHandle());
end

--- Create an empty craft within the two radius defined of GameObject
-- @param self GameObject instance.
-- @param ODF ODF file.
-- @param Team Team number.
-- @param MinRadiusAway Minimum radius for spawn.
-- @return New GameObject.
function GameObject.BuildEmptyCraftNear(self, ODF, Team, MinRadiusAway, MaxRadiusAway)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(ODF) then error("Paramater  ODF must be a string."); end
    if not isnumber(Team) then error("Paramater Team must be a number."); end
    if not isnumber(MinRadiusAway) then error("Paramater MinRadiusAway must be a number."); end
    if not isnumber(MaxRadiusAway) then error("Paramater MaxRadiusAway must be a number."); end
    return GameObject.new(ffi.C.BuildEmptyCraftNear(self, tocstring(ODF), Team, MinRadiusAway, MaxRadiusAway));
end

--- Set this GameObject as User for given team
-- @param self GameObject instance.
-- @param Team Team number.
function GameObject.SetAsUser(self, Team)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(Team) then error("Paramater Team must be a number."); end
    ffi.C.SetAsUser(self:GetHandle(), Team);
end

--- Set the No Scrap Flag for this GameObject
-- @param self GameObject instance.
function GameObject.SetNoScrapFlagByHandle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetNoScrapFlagByHandle(self:GetHandle());
end

--- Clear the No Scrap Flag for this GameObject
-- @param self GameObject instance.
function GameObject.ClearNoScrapFlagByHandle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ClearNoScrapFlagByHandle(self:GetHandle());
end

--- Get the GameObject's Target.
-- @param self GameObject instance.
-- @return Targeted GameObject.
function GameObject.GetTarget(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(GetTarget(self:GetHandle()));
end

--- Reset the GameObject's team slot.
-- @param self GameObject instance.
function GameObject.ResetTeamSlot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ResetTeamSlot(self:GetHandle());
end

--- Get the GameObject's category type override.
-- @param self GameObject instance.
-- @return Category Type Override, nil if invalid handle.
function GameObject.GetCategoryTypeOverride(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetCategoryTypeOverride(self:GetHandle());
    if retVal == -2 then return nil; end
    return retVal;
end

--- Get the GameObject's category type.
-- @param self GameObject instance.
-- @return Category Type, nil if invalid handle.
function GameObject.GetCategoryType(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetCategoryType(self:GetHandle());
    if retVal == -2 then return nil; end
    return retVal;
end

--- Set the GameObject group to the first non-full group matching its type.
-- @param self GameObject instance.
-- @return Group the unit was moved too, nil if any problems.
function GameObject.SetBestGroup(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.SetBestGroup(self:GetHandle());
end

--- Set the GameObject's lifespan.
-- @param self GameObject instance.
-- @param timeout Timeout time, -1 or nil will clear the timeout.
function GameObject.SetLifespan(self, timout)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if timeout == nil then timeout = -1; end
    ffi.C.SetLifespan(self:GetHandle(), timeout);
end

--- The the player that is this GameObject
-- @param self GameObject instance.
-- @param pExplanationStr Reason for the kick.
-- @param banAlso Ban the player from the session.
function GameObject.KickPlayer(self, pExplanationStr, banAlso)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(pExplanationStr) then error("Paramater pExplanationStr must be a string."); end
    -- banAlso as nil would be treated as false
    ffi.C.KickPlayer(self:GetHandle(), tocstring(pExplanationStr));
end

--- Is this GameObject a person?
-- @param self GameObject instance.
-- @return Boolean
function GameObject:IsPerson(self)
    return ffi.C.IsPerson(self:GetHandle());
end


--==============================================================================================================================================================
-- ObjectInfo
--==============================================================================================================================================================

--- Get Object Info
-- @param h GameObject
-- @param type ObjectInfoType
-- @return String value
function GetObjInfo(h, type)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    if not isobjectinfotype(type) then error("Paramater type must be an ObjectInfoType."); end
    local pBuffer = ffi.new("char[65]");
    ffi.fill(pBuffer, 65);
    ffi.C.GetObjInfo(h:GetHandle(),type, pBuffer);
    return tostring(pBuffer);
end

--- Get Group Info
-- @param team Team number
-- @param group Group number
-- @param type ObjectInfoType, only accepts Get_CFG, Get_ODF, or Get_GOClass_gCfg
-- @return String value
function GetGroup(team, group, type)
    if not isnumber(team) then error("Paramater team must be a number."); end
    if not isnumber(group) then error("Paramater group must be a number."); end
    if not isobjectinfotype(type) then error("Paramater type must be an ObjectInfoType."); end
    local pBuffer = ffi.new("char[65]");
    ffi.fill(pBuffer, 65);
    ffi.C.GetGroup(team, group, type, pBuffer);
    return tostring(pBuffer);
end


--==============================================================================================================================================================
-- Cockpit Timer
--==============================================================================================================================================================

--- Start the Cockpit Timer countdown
-- @param time Target time.
-- @param warn Time for warning color.
-- @param alert Time for alert color.
function StartCockpitTimer(time, warn, alert)
    if not isnumber(time) then error("Paramater time must be a number."); end
    if not isnumber(warn) then error("Paramater warn must be a number."); end
    if not isnumber(alert) then error("Paramater alert must be a number."); end
    if warn == nil then warn = LONG_MIN; end
    if alert == nil then alert = LONG_MIN; end
    ffi.C.StartCockpitTimer(time, warn, alert);
end

--- Start the Cockpit Timer countup
-- @param time Starting time.
-- @param warn Time for warning color.
-- @param alert Time for alert color.
function StartCockpitTimerUp(time, warn, alert)
    if not isnumber(time) then error("Paramater time must be a number."); end
    if not isnumber(warn) then error("Paramater warn must be a number."); end
    if not isnumber(alert) then error("Paramater alert must be a number."); end
    if warn == nil then warn = LONG_MAX; end
    if alert == nil then alert = LONG_MAX; end
    ffi.C.StartCockpitTimerUp(time, warn, alert);
end

--- Stop the Cockpit Timer
function StopCockpitTimer()
  ffi.C.StopCockpitTimer();
end

--- Hide the Cockpit Timer
function HideCockpitTimer()
  ffi.C.HideCockpitTimer();
end

--- Get the Cockpit Timer time
-- @return time of timer
function GetCockpitTimer()
  return ffi.C.GetCockpitTimer();
end


--==============================================================================================================================================================
-- End of nicely sorted stuff











--- Build Object
-- @param odf Object Definition File (without ".odf")
-- @param team Team number for the object, 0 to 15
-- @param pos Position as GameObject, Pathpoint Name, AiPath, Vector, or Matrix
-- @return Newly built GameObject
function BuildObject(odf, team, pos)
    if not isstring(odf) then error("Paramater odf must be a string."); end
    if not isnumber(team) then error("Paramater team must be a number."); end
    local msg = tocstring(odf); -- convert lua string to cstring
    local handle = 0;
    if isgameobject(pos) then
        handle = ffi.C.BuildObject(msg, team, pos:GetHandle());
    elseif isstring(pos) then
        handle = ffi.C.BuildObjectP(msg, team, tocstring(pos));
    --elseif type(pos) == "AiPath" then
    --  handle = ffi.C.BuildObject(msg, team, pos)
    elseif isvector(pos) then
        handle = ffi.C.BuildObjectV(msg, team, pos);
    elseif ismatrix(pos) then
        handle = ffi.C.BuildObjectM(msg, team, pos);
    else
        error("BuildObject pos paramater is invalid, received " .. type(pos) .. ", expected GameObject, Path Name (string), AiPath, Vector, or Matrix");
    end
      
    if handle == 0 then return nil end;
    return GameObject.new(handle);
end

--- Get object by label or seq no.
-- @param name Label on GameObject.
-- @return GameObject with label
function GetHandle(name)
    local handle = 0;
    if isstring(name) then
        local cName = tocstring(name);
        handle = ffi.C.GetHandle(cName);
    elseif isstring(name) then
        handle = ffi.C.GetHandleSeq(name);
    else
        error("GetHandle paramater type invalid, received " .. type(name) .. ", expected string or int");
    end
    if handle == 0 then return nil; end
    return Handle.new(handle);
end


--- Set DefaultAllies
function DefaultAllies()
    ffi.C.DefaultAllies();
end

--- Set TeamplayAllies
function TeamplayAllies()
    ffi.C.TeamplayAllies();
end

--- Ally two teams
-- This operation is one way, be sure to call again with the team paramaters swapped for a proper alliance.
-- @param t1 Team 1
-- @param t2 Team 2
function Ally(t1, t2)
    if not isnumber(t1) then error("Paramater t1 must be a number."); end
    if not isnumber(t2) then error("Paramater t2 must be a number."); end
    ffi.C.Ally(t1, t2);
end

--- UnAlly two teams
-- This operation is one way, be sure to call again with the team paramaters swapped for a proper un-alliance.
-- @param t1 Team 1
-- @param t2 Team 2
function UnAlly(t1, t2)
    if not isnumber(t1) then error("Paramater t1 must be a number."); end
    if not isnumber(t2) then error("Paramater t2 must be a number."); end
    ffi.C.UnAlly(t1, t2);
end


--- Play audio message
-- @param msg Filename of audio message.
-- @param purge Optional bool to purge other sounds.
-- @return AudioMessage ID.
function AudioMessage(msg, purge)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    return ffi.C.AudioMessage(tocstring(msg), purge);
end

--- Is the audio message done?
-- @param id AudioMessage ID.
-- @return Boolean
function IsAudioMessageDone(id)
    if not isnumber(id) then error("Paramater msg must be a isnumber."); end
  return ffi.C.IsAudioMessageDone(id);
end

--- Is the audio message done?
-- @param id AudioMessage ID.
function StopAudioMessage(id)
    if not isnumber(id) then error("Paramater msg must be a isnumber."); end
    ffi.C.StopAudioMessage(id);
end

--- Preload the audio message file
-- @param msg Filename of audio message.
function PreloadAudioMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PreloadAudioMessage(tocstring(msg));
end

--- Purge the audio message file
-- @param msg Filename of audio message.
function PurgeAudioMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PurgeAudioMessage(tocstring(msg));
end

--- Preload the music message file
-- @param msg Filename of music message.
function PreloadMusicMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PreloadMusicMessage(tocstring(msg));
end

--- Purge the music message file
-- @param msg Filename of music message.
function PurgeMusicMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PurgeMusicMessage(tocstring(msg));
end

--- Play music message
-- @param msg Filename of music message.
function LoadJukeFile(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.LoadJukeFile(tocstring(msg));
end

--- Set music message intensity
-- @param intensity Music message intensity.
function SetMusicIntensity(intensity)
    if not isnumber(intensity) then error("Paramater intensity must be a number."); end
    ffi.C.SetMusicIntensity(intensity);
end

--- Is the object the local user is inspecting defined by this object definition?
-- @param odf The definition file of the object being inspected.
-- @return Boolean
function IsInfo(odf)
    if not isstring(intensity) then error("Paramater intensity must be a string."); end
    return ffi.C.IsInfo(tocstring(odf));
end

--- Start a quake
-- @param magnitude Magnitude of the quake.
function StartEarthQuake(magnitude)
    if not isnumber(magnitude) then error("Paramater magnitude must be a number."); end
    ffi.C.StartEarthQuake(magnitude);
end
--- Change the quake magnitude
-- @param magnitude Magnitude of the quake.
function UpdateEarthQuake(magnitude)
    if not isnumber(magnitude) then error("Paramater magnitude must be a number."); end
    ffi.C.UpdateEarthQuake(magnitude);
end

--- Stop the quake
function StopEarthQuake()
  ffi.C.StopEarthQuake();
end

--- Get the world the DLL is currently executing in
-- [future] document what world number is what.
-- @return World number
function GetCurWorld()
    return ffi.C.GetCurWorld();
end

--- Get string variable by name
-- @param VarItemName Variable name.
-- @return Variable value.
function GetVarItemStr(VarItemName)
    if not isstring(VarItemName) then error("Paramater VarItemName must be a string."); end
    return tostring(ffi.C.GetVarItemStr(tocstring(VarItemName)));
end

--- Get integer variable by name
-- @param VarItemName Variable name.
-- @return Variable value.
function GetVarItemInt(VarItemName)
    if not isstring(VarItemName) then error("Paramater VarItemName must be a string."); end
    return ffi.C.GetVarItemStr(GetVarItemInt(VarItemName));
end

--- Get client integer variable by index and team
-- @param TeamNum Team number.
-- @param Idx civar index.
-- @return Variable value.
function GetCVarItemInt(TeamNum, Idx)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if not isnumber(Idx) then error("Paramater Idx must be a number."); end
    return ffi.C.GetCVarItemInt(TeamNum, Idx);
end

--- Get client string variable by index and team
-- @param TeamNum Team number.
-- @param Idx civar index.
-- @return Variable value.
function GetCVarItemStr(TeamNum, Idx)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if not isnumber(Idx) then error("Paramater Idx must be a number."); end
    return tostring(ffi.C.GetCVarItemStr(TeamNum, Idx));
end

--- Preload an ODF by filename
-- @param cfg Object Definition filename.
function PreloadODF(cfg)
    if not isstring(cfg) then error("Paramater cfg must be a string."); end
    ffi.C.PreloadODF(tocstring(cfg));
end

--- Terrain floor at X/Z
-- @param x X coordinate.
-- @param z Z coordinate.
-- @return Y coordinate of terrain at that X/Z.
function TerrainFindFloor(x, z)
    if not isnumber(x) then error("Paramater x must be a number."); end
    if not isnumber(z) then error("Paramater z must be a number."); end
    return ffi.C.TerrainFindFloor(x, z);
end

--- PrintConsoleMessage
-- @param msg Message to print.
function PrintConsoleMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PrintConsoleMessage(tocstring(msg));
end

--- Get Random Float
-- Safe for MP, portable function.
-- @param range upper bound of random range.
-- @return Random float within range.
function GetRandomFloat(range)
    if not isnumber(range) then error("Paramater range must be a number."); end
    return ffi.C.GetRandomFloat(range);
end

--- Calculate the CRC of a string
-- @param n Input string.
-- @return long CRC value.
function CalcCRC(n)
    if not isstring(n) then error("Paramater n must be a string."); end
    return ffi.C.CalcCRC(tocstring(n));
end

--- Set AI Plan for given team
-- @param cfg AIP filename.
-- @param team Team number, defaults to -1.
function SetPlan(cfg, team)
    if not isstring(cfg) then error("Paramater (cfg must be a string."); end
    if team == nil then team = -1; end
    ffi.C.SetSkill(tocstring(cfg), team);
end

--- Log a float
-- @param v A decimal number.
function LogFloat(v)
    ffi.C.LogFloat(v);
end

--- Get Instation Option 'My Force'
-- @param Integer value
function GetInstantMyForce()
    return ffi.C.GetInstantMyForce();
end

--- Get Instation Option 'Computer Force'
-- @param Integer value
function GetInstantCompForce()
    return ffi.C.GetInstantCompForce();
end

--- Get Instation Option 'Difficulty'
-- @param Integer value
function GetInstantDifficulty()
    return ffi.C.GetInstantDifficulty();
end

--- Get Instation Option 'Goal'
-- @param Integer value
function GetInstantGoal()
    return ffi.C.GetInstantGoal();
end

--- Get Instation Option 'My Force'
-- @param Integer value
function GetInstantType()
    return ffi.C.GetInstantType();
end

--- Get Instation Option 'Flag'
-- @param Integer value
function GetInstantFlag()
    return ffi.C.GetInstantFlag();
end

--- Get Instation Option 'My Side'
-- @param Integer value
function GetInstantMySide()
  return ffi.C.GetInstantMySide();
end

--- Ignore Sync
-- @param on Boolean, turn on?
--function IgnoreSync(on)
--    ffi.C.IgnoreSync(on);
--end

--- Is recording?
-- @return Boolean
--function IsRecording()
--    return ffi.C.IsRecording();
--end

--- Clear Text Ojectives
function ClearObjectives()
    ffi.C.ClearObjectives();
end

--- Add Objective Text
-- @param name Text to display
-- @param color Color for the text
-- @param showTime Time to show the objective box, defaults to 8.0
function AddObjective(name, color, showTime)
    if not isstring(name) then error("Paramater name must be a string."); end
    if not iscolor(color) then error("Paramater color must be an instance of color."); end
    if showTime == nil then showTime = 8.0; end
    ffi.C.AddObjective(tocstring(name),color:ToColorLong(),showTime);
end

--- Set color fade
-- @param ratio Ratio
-- @param rate Rate
-- @param color Color to fade
function SetColorFade(ratio, rate, color)
    if not isnumber(ratio) then error("Paramater ratio must be a number."); end
    if not isnumber(rate) then error("Paramater rate must be a number."); end
    if not iscolor(color) then error("Paramater color must be an instance of color."); end
    ffi.C.SetColorFade(ratio, rate, color:ToColorLong());
end

--- Stop cheats
function StopCheats()
    ffi.C.StopCheats();
end

--- Recalculate Cliffs between two objects
-- @param h1 First Object
-- @param h2 Last Object
-- @param radius Radius
function CalcCliffs(h1, h2, radius)
    if not isgameobject(h1) then error("Paramater h1 must be GameObject instance."); end
    if not isgameobject(h2) then error("Paramater h2 must be GameObject instance."); end
    if not isnumber(radius) then error("Paramater radius must be a number."); end
    ffi.C.CalcCliffs(tocstring(path));
end

--- Recalculate Cliffs along path
-- @param path Path
function CalcCliffsP(path)
    if not isstring(path) then error("Paramater path must be a string."); end
    ffi.C.CalcCliffsP(tocstring(path));
end

--- Start sound effect
-- @param file Filename
-- @param h Optional handle
-- @return Sound effect ID
function StartSoundEffect(file, h)
    if not isstring(path) then error("Paramater path must be a string."); end
    local handle = 0;
    if isgameobject(h) then handle = h:GetHandle(); end
    return ffi.C.StartSoundEffect(tocstring(file), h);
end

--- Start sound effect
-- @param file Filename
-- @param h Optional handle
-- @return Sound effect ID
function FindSoundEffect(file, h)
    if not isstring(path) then error("Paramater path must be a string."); end
    local handle = 0;
    if isgameobject(h) then handle = h:GetHandle(); end
    return ffi.C.FindSoundEffect(tocstring(file), h);
end

-- Stop sound effect by ID
-- @param sound Sound Effect ID
function StopSoundEffect(sound)
    if not isnumber(sound) then error("Paramater sound must be a number."); end
    ffi.C.StopSoundEffect(sound);
end

--- Get object by team slot and team
-- @param TeamNum Team number
-- @param Slot Slot, see Slots table
-- @return Found GameObject
function GetObjectByTeamSlot(TeamNum, Slot)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if not isnumber(Slot) then error("Paramater Slot must be a number."); end
    return GameObject.new(ffi.C.GetObjectByTeamSlot(TeamNum, Slot));
end

--- sin (Network Safe)
-- @param ang Angle
-- @return Number, math operation result
function portable_sin(ang)
    if not isnumber(ang) then error("Paramater ang must be a number."); end
    return ffi.C.portable_sin(ang);
end

--- cosin (Network Safe)
-- @param ang Angle
-- @return Number, math operation result
function portable_cos(ang)
    if not isnumber(ang) then error("Paramater ang must be a number."); end
    return ffi.C.portable_cos(ang);
end

--- arctangent (Network Safe)
-- @param x X
-- @param y Y
-- @return Number, math operation result
function portable_atan2(x, y)
    if not isnumber(x) then error("Paramater x must be a number."); end
    if not isnumber(y) then error("Paramater y must be a number."); end
    return ffi.C.portable_atan2(x, y);
end

--- arcsin (Network Safe)
-- @param x X
-- @return Number, math operation result
function portable_asin(x)
    if not isnumber(x) then error("Paramater x must be a number."); end
    return ffi.C.portable_asin(x);
end

--- arccosin (Network Safe)
-- @param x X
-- @return Number, math operation result
function portable_acos(x)
    if not isnumber(x) then error("Paramater x must be a number."); end
    return ffi.C.portable_acos(x);
end

--- Is the network on?
-- @return Boolean
function IsNetworkOn()
    return ffi.C.IsNetworkOn();
end

--- Is this the server?
-- @return Boolean
function ImServer()
    return ffi.C.ImServer();
end

--- Is this a dedicated server?
-- @return Boolean
function ImDedicatedServer()
    return ffi.C.ImDedicatedServer();
end

--- Is teamplay on?
-- @return Boolean
function IsTeamplayOn()
    return ffi.C.IsTeamplayOn();
end

--- Get count of active players
-- @return Count of players active
function CountPlayers()
    return ffi.C.CountPlayers()
end

--- Get player handle by team
-- @param team Team number
-- @return GameObject of player
function GetPlayerHandle(team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return GameObject.new(ffi.C.GetPlayerHandle(team));
end

--- Get team race
-- @param TeamNum Team number
-- @return Race letter of team
function GetRaceOfTeam(TeamNum)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetRaceOfTeam(TeamNum);
end

--- Get teamgroup of given team
-- @param Team Team number
-- @return Teamgroup number, nil if the team doesn't fit into any normal group
function WhichTeamGroup(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    local retVal = ffi.C.WhichTeamGroup(Team);
    if retVal == 0xA5A5A5A5 then return nil; end
    return retVal;
end

--- Get count of ally teams (teamgroup based)
-- @param Team Team number
-- @return Count of ally teams (or 0 if teamgroups are off)
function CountAllies(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.CountAllies(Team);
end

--- Get commander team
-- @para Team
-- @return Commander's Team
function GetCommanderTeam(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetCommanderTeam(Team);
end

--- Get first allied team
-- @para Team
-- @return First Allied Team
function GetFirstAlliedTeam(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetFirstAlliedTeam(Team);
end

--- Get last allied team
-- @para Team
-- @return Last Allied Team
function GetLastAlliedTeam(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetLastAlliedTeam(Team);
end

--- Get allied team range
-- @para Team
-- @return First Allied Team
-- @return Last Allied Team
function GetAlliedTeamRange(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return GetFirstAlliedTeam(Team),GetLastAlliedTeam(Team);
end

--- Get Teamplay Ranges
-- @para Team
-- @return DefenseTeamNum
-- @return OffenseMinTeamNum
-- @return OffenseMaxTeamNum
function GetTeamplayRanges(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    local DefenseTeamNum = ffi.new("int[0]");
    local OffenseMinTeamNum = ffi.new("int[0]");
    local OffenseMaxTeamNum = ffi.new("int[0]");
    ffi.C.GetTeamplayRanges(Team,DefenseTeamNum,OffenseMinTeamNum,OffenseMaxTeamNum);
    return tonumber(DefenseTeamNum),tonumber(OffenseMinTeamNum),tonumber(OffenseMaxTeamNum);
end

--- Clear team colors
function ClearTeamColors()
    ffi.C.ClearTeamColors();
end

--- Set default team colors
function DefaultTeamColors()
    ffi.C.DefaultTeamColors();
end

--- Set teamplay team colors
function TeamplayTeamColors()
    ffi.C.TeamplayTeamColors();
end

--- Set team color
-- @param team Team Number
-- @param color Color
function SetTeamColor(team, color)
    if not isnumber(team) then error("Paramater team must be a number."); end
    if not iscolor(color) then error("Paramater color must be a Color instance."); end
    local r;
    local g;
    local b;
    r,g,b = color.ToRGB();
    ffi.C.SetTeamColor(team, r, g, b);
end

--- Clear team color on given team
-- @param team Team Number
function ClearTeamColor(team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    ffi.C.ClearTeamColor(team);
end

--- Get a position near
-- @param Pos Vector location
-- @param MinRadiusAway Minimum radius
-- @param MaxRadiusAway Maximum radius
-- @return Vector location
function GetPositionNear(Pos, MinRadiusAway, MaxRadiusAway)
    if not isvector(Pos) then error("Paramater Pos must be a Vector."); end
    if not isnumber(MinRadiusAway) then error("Paramater MinRadiusAway must be a number."); end
    if not isnumber(MaxRadiusAway) then error("Paramater MaxRadiusAway must be a number."); end
    return ffi.C.GetPositionNear(Pos, MinRadiusAway, MaxRadiusAway);
end

--- Get the player ODF
-- @param TeamNum Team number
-- @param RType RandomizeType
-- @return Player ODF
function GetPlayerODF(TeamNum, RType)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if RType == null then RType = RandomizeType.Randomize_None; end
    if not israndomizetype(RType) then error("Paramater RType must be a RandomizeType."); end
    return tostring(ffi.C.GetPlayerODF(TeamNum, RType));
end

--- Get a position at a radius from the given location along an angle
-- @param CenterPos Vector location
-- @param Radius Radius
-- @param Angle Angle
-- @return Vector location
function SetCircularPos(CenterPos, Radius, Angle)
    if not isvector(CenterPos) then error("Paramater CenterPos must be a Vector."); end
    if not isnumber(Radius) then error("Paramater Radius must be a number."); end
    if not isnumber(Angle) then error("Paramater Angle must be a number."); end
    local NewPos = Vector();
    ffi.C.SetCircularPos(CenterPos, Radius, Angle, NewPos);
    return NewPos;
end

--- Get safest spawnpoint vector
-- @return Vector location
function GetSafestSpawnpoint()
    return ffi.C.GetSafestSpawnpoint();
end

--- Get spawnpoint vector for team
-- @param TeamNum Team number
-- @return Vector location
function GetSpawnpoint(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    return ffi.C.GetSpawnpoint(TeamNum);
end

--- Get spawnpoint handle for team
-- @param TeamNum Team number
-- @return Handle spawn point
function GetSpawnpointHandle(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    return GameObject.new(ffi.C.GetSpawnpointHandle(TeamNum));
end

--- Returns a random spawnpoint that 'looks' safe. [Nobody within 100 meters]
-- @param TeamNum Team number, -1 default
-- @return Vector location
function GetRandomSpawnpoint(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if TeamNum == nil then TeamNum = -1; end
    return ffi.C.GetRandomSpawnpoint(TeamNum);
end

--- Set the message in the timer box
-- @param message
function SetTimerBox(message)
    if not isstring(message) then error("Paramater message must be a string."); end
    ffi.C.SetTimerBox(tocstring(message));
end

--- Add message to the message box
-- @param msg
function AddToMessagesBox(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.AddToMessagesBox(tocstring(msg));
end

--- Add to score of GameObject
-- @param team GameObject instance.
-- @param delta Score to add.
function AddScore(team, delta)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(delta) then error("Paramater delta must be GameObject instance."); end
    ffi.C.AddScore(team, delta)
end

--- Get local player team number
-- @return Team number
function GetLocalPlayerTeamNumber()
    return ffi.C.GetLocalPlayerTeamNumber();
end

--- Get local player's DPID
-- @return DPID / DWORD / unsigned long
function GetLocalPlayerDPID()
    return ffi.C.GetLocalPlayerDPID();
end

--- Note game over by timelimit
function NoteGameoverByTimelimit()
    ffi.C.NoteGameoverByTimelimit()
end

--- Note game over by kill limit
-- @param h GameObject
function NoteGameoverByKillLimit(h)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    ffi.C.NoteGameoverByKillLimit(h.handle)
end

--- Note game over by score
-- @param h GameObject
function NoteGameoverByScore(h)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    ffi.C.NoteGameoverByScore(h.handle)
end

--- Note game over by last with base
-- @param h GameObject
function NoteGameoverByLastWithBase(h)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    ffi.C.NoteGameoverByLastWithBase(h.handle)
end

--- Note game over by last team with base
-- @param Teamgroup Team number
function NoteGameoverByLastTeamWithBase(Teamgroup)
    if not isnumber(Teamgroup) then error("Paramater Teamgroup must be a number."); end
    ffi.C.NoteGameoverByLastTeamWithBase(Teamgroup)
end

--- Note game over by no bases
function NoteGameoverByNoBases()
    ffi.C.NoteGameoverByNoBases()
end

--- Do gameover after X time
-- @param SecondsFromNow Seconds from now
function DoGameover(SecondsFromNow)
    if not isnumber(SecondsFromNow) then error("Paramater SecondsFromNow must be a number."); end
    ffi.C.DoGameover(SecondsFromNow)
end

--- Lock in MP race because someone started that team group
-- In Teamplay, this is a reporting by the DLL that a teamgroup (0,1) has built a recycler (or otherwise locked in a race) of the given race ('i' or 'f')
-- @param WhichTeamGroup Teamgroup
-- @param RaceIdentifier Race character
function SetMPTeamRace(WhichTeamGroup, RaceIdentifier)
    if not isnumber(WhichTeamGroup) then error("Paramater WhichTeamGroup must be a number."); end
    if not isstring(RaceIdentifier) then error("Paramater RaceIdentifier must be a string / character."); end
    ffi.C.SetMPTeamRace(WhichTeamGroup, RaceIdentifier);
end

--- Get user target
-- @param TeamNum Team number
-- @return User on team's target GameObject
function GetUserTarget(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    return GameObject.new(ffi.C.GameUserTarget(TeamNum));
end

--- Execute console command
-- @param pStr Command
-- @param bShowOutput Show output, optional, default false
function IFace_ConsoleCmd(pStr, bShowOutput)
    if not isstring(pStr) then error("Paramater pStr must be a string."); end
    ffi.C.IFace_ConsoleCmd(tocstring(pStr), not bShowOutput);
end

--- Print text in chat box with color
-- @param message Text
-- @param color Color for text
function AddToMessagesBox2(message, color)
    if not isstring(message) then error("Paramater message must be a string."); end
    if not iscolor(color) then error("Paramater color must be an instance of Color."); end
    ffi.C.AddToMessagesBox2(tocstring(message), color:ToColorLong())
end

--- Set string across network
-- Only has effect on host, could take as long as 150 turns to send data
-- Do not use this unless you are damn sure you know what you are doing
-- This might not even work anymore in 1.3
-- @param name Name of the variable
-- @param value Value for the variable
function Network_SetString(name, value)
    if not isstring(name) then error("Paramater name must be a string."); end
    if not isstring(value) then error("Paramater value must be a string."); end
    ffi.C.Network_SetString(tocstring(name), tocstring(value));
end

--- Set integer across network
-- Only has effect on host, could take as long as 150 turns to send data
-- Do not use this unless you are damn sure you know what you are doing
-- This might not even work anymore in 1.3
-- @param name Name of the variable
-- @param value Value for the variable
function Network_SetInteger(name, value)
    if not isstring(name) then error("Paramater name must be a string."); end
    if not isnumber(value) then error("Paramater value must be a number."); end
    ffi.C.Network_SetInteger(tocstring(name), value);
end

--- Get the first empty group
-- @param t Team number
-- @return Group number
function GetFirstEmptyGroup(t)
    if not isnumber(t) then error("Paramater t must be a number."); end
    return ffi.C.GetFirstEmptyGroup(t);
end


--- Does this odf exist?
-- @param odf Filename, omit the file extension
-- @return Boolean
function DoesODFExist(odf)
    if not isstring(odf) then error("Paramater odf must be a string."); end
    return ffi.C.DoesODFExist(tocstring(odf));
end

--- Look up string in localize table
-- @param Src Localize key
-- @param size Max size of string, defaults to 1024
-- @return Lookup result string
function TranslateString2(Src, size)
    if not isstring(Src) then error("Paramater Src must be a string."); end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    ffi.C.TranslateString2(passIn, size, tocstring(Src));
    return tostring(passIn);
end

--- We want PlayerEjected messages for all units and not just players
-- This is for 1.3 Bot DMs
function WantBotKillMessages()
    ffi.C.WantBotKillMessages();
end

--- Set the TPS of the mission/game
-- @param newRate New TPS value, accepts 10, 15, 20, or 30
function EnableHighTPS(newRate)
    if not isnumber(newRate) then error("Paramater newRate must be a number."); end
    if newRate ~= 10 and newRate ~= 15 and newRate ~= 20 and newRate ~= 30 then error("Paramater newRate must be 10, 15, 20, or 30."); end
    ffi.C.EnableHighTPS(newRate);
end

--- Get object being inspected by local user
-- This is not MP safe unless you are very carful with what you are doing
-- @return GameObject being inspected
function GetLocalUserInspectHandle()
    return GameObject.new(ffi.C.GetLocalUserInspectHandle());
end

--- Get object being selected by local user
-- This is not MP safe unless you are very carful with what you are doing
-- @return GameObject being selected
function GetLocalUserSelectHandle()
    return GameObject.new(ffi.C.GetLocalUserSelectHandle());
end

--- Note gameover with custom message
-- @param pString Message
function NoteGameoverWithCustomMessage(pString)
    if not isstring(pString) then error("Paramater pString must be a string."); end
    ffi.C.NoteGameoverWithCustomMessage(tocstring(pString));
end

--- Get group count
-- @param team Team number
-- @param group Group number
-- @return Count of items in group
function GetGroupCount(team, group)
    if not isnumber(team) then error("Paramater team must be a number."); end
    if not isnumber(group) then error("Paramater group must be a number."); end
    return ffi.C.GetGroupCount(team, group);
end

--- Does this file exist in loadable assets?
-- @param filename Filename
-- @return Boolean does the file exist?
function DoesFileExist(filename)
    if not isstring(filename) then error("Paramater filename must be a string."); end
    return ffi.C.DoesFileExist(tocstring(filename));
end

--- Load a file and return itx text
-- @param filename Filename
-- @return File contents as string, nil if not found
function LoadFile(filename)
    local filenameC = tocstring(LoadFile);
    local bufSize = ffi.new("unsigned int[1]");
    local success = ffi.C.LoadFile(filename, nil, bufSize);
    if not success then return nil; end
    local pData = ffi.new("char[?]",tonumber(bufSize) + 1);
    ffi.fill(pData, tonumber(bufSize) + 1);
    success = ffi.C.LoadFile(filename, pData, bufSize);
    if success then
        return tostring(pData);
    end
    return nil;
end

--- Set the map gravity
-- @param gravity Gravity to set, defaults to 12.5 if nil
function SetGravity(gravity)
    if gravity == nil then gravity = 12.5; end
    ffi.C.SetGravity(gravity);
end

--- Set if objects build from BuildObject are automaticly grouped
-- @param stopAutoGroup
function SetAutoGroupUnits(stopAutoGroup)
  ffi.C.SetAutoGroupUnits(not stopAutoGroup);
end

--- Is this terrain cell water?
-- @param posORx Position vector, or X coordinate
-- @param z Z coordinate if posORx is not a vector
-- @return Boolean Is this terrain cell water?
function TerrainIsWater(posORx, z)
    if isvector(posORx) then
        return ffa.C.TerrainIsWaterV(posOrx);
    elseif isnumber(posORx) and isnumber(z) then
        return ffa.C.TerrainIsWater(posOrx, z);
    else
        error("posORx must be vector or posORx and z must be numbers");
    end
end

--- Get various terrain data
-- @param pos Position vector
-- @param useWater Use water? defaults to false
-- @return Terrain (or water if selected) height
-- @return Normal vector
function TerrainGetHeightAndNormal(pos, useWater)
    if not isvector(pos) then error("Paramater pos must be a Vector."); end
    local outHeight = ffi.new("float[1]");
    local outNormal = Vector();--ffi.new("Vector[1]");
    local success = ffi.C.TerrainGetHeightAndNormal(pos, outHeight, outNormal, useWater);
    if success ~= nil then return nil; end
    return tonumber(outHeight), outNormal;
end

--- Get a usable directory for writing files, should be in mydocs
-- @return Path
function GetOutputPath()
    local bufSize = ffi.new("unsigned int[1]");
    local success = ffi.C.GetOutputPath(bufSize, nil);
    if not success then return nil; end
    local pData = ffi.new("char[?]",tonumber(bufSize) + 1);
    success = ffi.C.GetOutputPath(bufSize, pData);
    if not success then return nil; end
    return tostring(pData);
end

--- Get pathpoints
-- @param path Path name
-- @return Array of VECTOR_2Ds
function GetPathPoints(path)
    if not isstring(path) then error("Paramater path must be a string."); end
    local pathC = tocstring(path);
    local bufSize = ffi.new("int[0]");
    local sucess = ffi.C.GetPathPoints(pathC, bufSize, nil);
    if not success then return nil; end
    local pData = ffi.new("float[?]", tonumber(bufSize) * 2);
    sucess = ffi.C.GetPathPoints(pathC, bufSize, pData);
    if not success then return nil; end
    local paths = {};
    for itr = 1,tonumber(bufSize) + 1,2 do
        table.insert(paths, VECTOR_2D(pData[itr+1],pData[iter+2]));
    end
    return paths;
end

--- AllowRandomTracks if music, rather than those listed in the TRN
-- Must be called immediatly
-- @param allow Boolean to allow or not, defaults to true in the C++ DLL but here it defaults false
function AllowRandomTracks(allow)
    ffi.C.AllowRandomTracks(allow)
end

--- Set FFA team colors
-- @param type TEAMCOLOR_TYPE
function SetFFATeamColors(type)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    ffi.C.SetFFATeamColors(type);
end

--- Set TeamStrat team colors
-- @param type TEAMCOLOR_TYPE
function SetTeamStratColors(type)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    ffi.C.SetTeamStratColors(type);
end

--- Get FFA team colors
-- @param type TEAMCOLOR_TYPE
-- @param team Team number
-- @return Color
function GetFFATeamColor(type, team)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    local r = ffi.new("int[1]");
    local g = ffi.new("int[1]");
    local b = ffi.new("int[1]");
    ffi.C.GetFFATeamColor(type, team, r, g, b);
    return Color.new(tonumber(r),tonumber(g),tonumber(b));
end

--- Get TeamStrat team colors
-- @param type TEAMCOLOR_TYPE
-- @param team Team number
-- @return Color
function GetTeamStratColor(type, team)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    if not isnumber(team) then error("Paramater team must be a number"); end
    local r = ffi.new("int[1]");
    local g = ffi.new("int[1]");
    local b = ffi.new("int[1]");
    ffi.C.GetTeamStratColor(type, team, r, g, b);
    return Color.new(tonumber(r),tonumber(g),tonumber(b));
end

--- Swap TeamStrat colors
function SwapTeamStratColors()
    ffi.C.SwapTeamStratColors();
end

--- Are team colors FFA?
-- @return Boolean
function GetTeamColorsAreFFA()
    return ffi.C.GetTeamColorsAreFFA();
end

--- Set team colors
-- @param type TEAMCOLOR_TYPE
function SetTeamColors(type)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    return ffi.C.SetTeamColors(type);
end

--- Add power
-- @param team Team
-- @param v Add power
-- @return Power level
function AddPower(team, v)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return ffi.C.AddPower(team, v)
end

--- Set power
-- @param team Team
-- @param v New power
-- @return Power level
function SetPower(team, v)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return ffi.C.SetPower(team, v)
end

--- Get power
-- @param team Team
-- @return Power level
function GetPower(team)
    if not isnumber(team) then error("Paramater team must be a number"); end
    return ffi.C.GetPower(team)
end

--- Get max power
-- @param team Team
-- @return Power level
function GetMaxPower(team)
    if not isnumber(team) then error("Paramater team must be a number"); end
    return ffi.C.GetMaxPower(team)
end

--- Add max scrap
-- @param team Team
-- @param ammount Ammount
function AddMaxScrap(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.AddMaxScrap(team, ammount)
end

--- Add max power
-- @param team Team
-- @param ammount Ammount
function AddMaxPower(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.AddMaxPower(team, ammount)
end

--- Set max scrap
-- @param team Team
-- @param ammount Ammount
function SetMaxScrap(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.SetMaxScrap(team, ammount)
end

--- Set max power
-- @param team Team
-- @param ammount Ammount
function SetMaxPower(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.SetMaxPower(team, ammount)
end

--- Get TeamStrat Individual Color
-- @param type TEAMCOLOR_TYPE
-- @param team Team number
-- @return Color
function GetTeamStratIndividualColor(type, team)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    local r = ffi.new("int[1]");
    local g = ffi.new("int[1]");
    local b = ffi.new("int[1]");
    ffi.C.GetTeamStratIndividualColor(type, team, r, g, b);
    return Color.new(tonumber(r),tonumber(g),tonumber(b));
end

--- Get the map's TRN filename
-- Useful for reading the ODF format file for custom data
-- @return TRN Filename
function GetMapTRNFilename()
    return ffi.string(ffi.C.GetMapTRNFilename());
end

--- Is this team allied?
-- @param t1 Team 1
-- @param t2 Team 2
-- @return Boolean
function IsTeamAllied(t1, t2)
    if not isnumber(t1) then error("Paramater t1 must be a number"); end
    if not isnumber(t2) then error("Paramater t2 must be a number"); end
    return ffi.C.IsTeamAllied(t1, t2);
end

--- Spawn birds
-- @param group Group number
-- @param count Count of birds
-- @param odf ODF Filename
-- @param t TeamNum
-- @param path Path name
function SpawnBirds(group, count, odf, t, path)
    if not isnumber(group) then error("Paramater group must be a number"); end
    if not isnumber(count) then error("Paramater count must be a number"); end
    if not isstring(odf) then error("Paramater odf must be a string"); end
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isstring(path) then error("Paramater path must be a string"); end
    ffi.C.SpawnBirds(group, count, tocstring(odf), t, tocstring(path));
end

--- Spawn birds
-- @param group Group number
-- @param count Count of birds
-- @param odf ODF Filename
-- @param t TeamNum
-- @param startObj Starting GameObject
-- @param destObj Destination GameObject
function SpawnBirds(group, count, odf, t, startObj, destObj)
    if not isnumber(group) then error("Paramater group must be a number"); end
    if not isnumber(count) then error("Paramater count must be a number"); end
    if not isstring(odf) then error("Paramater odf must be a string"); end
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isgameobject(startObj) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(destObj) then error("Paramater self must be GameObject instance."); end
    ffi.C.SpawnBirds(group, count, tocstring(odf), t, startObj, destObj);
end

--- Remove birds
-- @param group Group number
function RemoveBirds(group)
    if not isnumber(group) then error("Paramater group must be a number"); end
    ffi.C.RemoveBirds(group);
end

--- Get Camera Position and Heading
-- @return Camera Position
-- @return Camera Direction
function GetCameraPosition()
    local pos = Vector();
    local dir = Vector();
    ffi.C.GetCameraPosition(pos, dir);
    return pos,dir;
end

--- Set Camera Position and Heading
-- @param pos Camera Position
-- @param dir Camera Direction
function SetCameraPosition(pos, dir)
    if not isvector(pos) then error("Paramater pos must be Vector instance."); end
    if not isvector(dir) then error("Paramater dir must be Vector instance."); end
    ffi.C.SetCameraPosition(pos, dir);
end
 
--- Reset camera position
function ResetCameraPosition()
    ffi.C.ResetCameraPosition();
end
 
 --- Is the Camera Ready?
function CameraReady()
    return ffi.C.CameraReady();
end

--- Camera Path
-- @param path_name Path Name
-- @param height Height
-- @param speed Speed
-- @param target_handle Target GameObject
-- @return True when done
function CameraPath(path_name, height, speed, target_handle)
    if not isstring(path_name) then error("Paramater path_name must be a string"); end
    if not isnumber(height) then error("Paramater height must be a number"); end
    if not isnumber(speed) then error("Paramater speed must be a number"); end
    if not isgameobject(target_handle) then error("Paramater target_handle must be GameObject instance."); end
    return ffi.C.CameraPath(tocstring(path_name), height, speed, target_handle:GetHandle());
end

--- Camera Path Dir
-- @param path_name Path Name
-- @param height Height
-- @param speed Speed
-- @return True when done
function CameraPathDir(path_name, height, speed, target_handle)
    if not isstring(path_name) then error("Paramater path_name must be a string"); end
    if not isnumber(height) then error("Paramater height must be a number"); end
    if not isnumber(speed) then error("Paramater speed must be a number"); end
    return ffi.C.CameraPathDir(tocstring(path_name), height, speed);
end

--- PanDone
-- @return Boolean
function PanDone()
    return ffi.C.PanDone();
end

--- CameraObject
-- @param object_handle Object to Camera
-- @param i offset in centimeters
-- @param j offset in centimeters
-- @param k offset in centimeters
-- @param target_handle Target Object
-- @return Boolean
function CameraObject(object_handle, i, j, k, target_handle)
    if not isgameobject(object_handle) then error("Paramater object_handle must be GameObject instance."); end
    if not isnumber(i) then error("Paramater i must be a number"); end
    if not isnumber(j) then error("Paramater j must be a number"); end
    if not isnumber(k) then error("Paramater k must be a number"); end
    if not isgameobject(target_handle) then error("Paramater target_handle must be GameObject instance."); end
    return ffi.C.CameraObject(object_handle, i, j, k, target_handle);
end

--- CameraFinish
-- @return Boolean
function CameraFinish()
    return ffi.C.CameraFinish();
end

--- CameraCancelled
-- @return Boolean
function CameraCancelled()
    return ffi.C.CameraCancelled();
end

--- FreeCamera
-- @return Boolean
function FreeCamera()
    return ffi.C.FreeCamera();
end

--- FreeFinish
-- @return Boolean
function FreeFinish()
    return ffi.C.FreeFinish();
end

--- PlayMovie
-- @param name Name of max 20 characters
-- @return Boolean
function PlayMovie(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlayMovie(nameIn);
end

--- StopMovie
function StopMovie()
    ffi.C.StopMovie();
end

--- PlayMove
-- @return Boolean
function PlayMove()
    return ffi.C.PlayMove();
end

--- PlayRecording
-- @param name Name of max 20 characters
-- @return Boolean
function PlayRecording(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlayRecording(nameIn);
end

--- PlayRecordingUpdateCam
-- @param name Name of max 20 characters
-- @param updateCam Update Camera
-- @return Boolean
function PlayRecordingUpdateCam(name, updateCam)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlayRecordingU(nameIn, updateCam);
end

--- PlaybackVehicle
-- @param name Name of max 20 characters
-- @return Boolean
function PlaybackVehicle(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlaybackVehicle(nameIn);
end


--- load an interface definition
-- @param n String name
function IFace_Exec(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_Exec(tocstring(n));
end

--- activate a control
-- @param n String name
function IFace_Activate(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_Activate(tocstring(n));
end

--- deactivate a control
-- @param n String name
function IFace_Deactivate(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_Deactivate(tocstring(n));
end

--- create a mission command
-- @param n String name
function IFace_CreateCommand(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_CreateCommand(tocstring(n));
end

--- create a mission string variable
-- @param name Var name
-- @param value Value
function IFace_CreateString(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isstring(value) then error("Paramater value must be a string"); end
    ffi.C.IFace_CreateString(tocstring(name), tocstring(value));
end

--- set a mission string variable
-- @param name Var name
-- @param value Value
function IFace_SetString(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isstring(value) then error("Paramater value must be a string"); end
    ffi.C.IFace_SetString(tocstring(name), tocstring(value));
end

--- get a mission string variable
-- @param name Var name
-- @param maxSize Maximum length of string
-- @return Value
function IFace_GetString(name, maxSize)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(maxSize) then error("Paramater maxSize must be a number"); end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    ffi.C.IFace_GetString(tocstring(name), passIn, maxSize);
    return tostring(passIn);
end

--- create a mission integer variable
-- @param name Var name
-- @param value Value
function IFace_CreateInteger(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_CreateInteger(tocstring(name), tocstring(value));
end

--- set a mission integer variable
-- @param name Var name
-- @param value Value
function IFace_SetInteger(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_SetInteger(tocstring(name), tocstring(value));
end

--- get a mission integer variable
-- @param name Var name
-- @return Value
function IFace_SetInteger(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    ffi.C.IFace_GetInteger(tocstring(name));
end

-- set the range of a mission integer variable
-- @param name Var name
-- @param low Low
-- @param high High
function IFace_SetIntegerRange(name, low, high)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(low) then error("Paramater low must be a number"); end
    if not isnumber(high) then error("Paramater high must be a number"); end
    ffi.C.IFace_SetIntegerRange(tocstring(name), low, high);
end


--- create a mission float variable
-- @param name Var name
-- @param value Value
function IFace_CreateFloat(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_CreateFloat(tocstring(name), tocstring(value));
end

--- set a mission float variable
-- @param name Var name
-- @param value Value
function IFace_SetFloat(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_SetFloat(tocstring(name), tocstring(value));
end

--- get a mission float variable
-- @param name Var name
-- @return Value
function IFace_GetFloat(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    ffi.C.IFace_GetFloat(tocstring(name));
end

--- Clear listbox
-- @param name Listbox Name
function IFace_ClearListBox(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    ffi.C.IFace_ClearListBox(tocstring(name));
end

--- Add listbox item
-- @param name Listbox Name
-- @param value Listbox Item
function IFace_AddTextItem(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isstring(value) then error("Paramater name must be a string"); end
    ffi.C.IFace_AddTextItem(tocstring(name));
end

--- Get selected listbox item
-- @param name Listbox Name
-- @param maxSize Maximum length of string
-- @return Value
function IFace_GetSelectedItem(name, maxSize)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(maxSize) then error("Paramater maxSize must be a number"); end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    ffi.C.IFace_GetSelectedItem(tocstring(name), passIn, maxSize);
    return tostring(passIn);
end



--- FailMission
-- @param t Time of failure
-- @param fileName Debriefing name (optional)
function FailMission(t, fileName)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if fileName ~= nil and not isstring(fileName) then error("Paramater fileName must be a string or nil"); end
    MisnImport.FailMission(t, fileName);
end

--- SucceedMission
-- @param t Time of success
-- @param fileName Debriefing name (optional)
function SucceedMission(t, fileName)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if fileName ~= nil and not isstring(fileName) then error("Paramater fileName must be a string or nil"); end
    MisnImport.SucceedMission(t, fileName);
end

--- ChangeSide
function SucceedMission()
    MisnImport.ChangeSide();
end

--- SucceedMission
-- @param t Time of success
-- @param fileName Debriefing name (optional)
function SucceedMission(t, fileName)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if fileName ~= nil and not isstring(fileName) then error("Paramater fileName must be a string or nil"); end
    MisnImport.SucceedMission(t, fileName);
end

--- AddScrap
-- @param t Team
-- @param v Ammount
-- @return Current
function AddScrap(t, v)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return MisnImport.AddScrap(t, v);
end

--- SetScrap
-- @param t Team
-- @param v Ammount
-- @return Current
function SetScrap(t, v)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return MisnImport.SetScrap(t, v);
end

--- GetScrap
-- @param t Team
-- @return Current
function GetScrap(t)
    if not isnumber(t) then error("Paramater t must be a number"); end
    return MisnImport.GetScrap(t);
end

--- GetMaxScrap
-- @param t Team
-- @return Current
function GetMaxScrap(t)
    if not isnumber(t) then error("Paramater t must be a number"); end
    return MisnImport.GetMaxScrap(t);
end

--- GetTug
-- @param self GameObject instance.
-- @return Tug of GameObject, or nil
function GameObject.GetTug(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
	local retVal = MisnImport.GetTug(self:GetHandle());
	if retVal == 0 then return nil; end
	return GameObject.new(retVal);
end

--- GetTug
-- @param self GameObject instance.
-- @return Tug of GameObject, or nil
function GameObject.HasCargo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
	return MisnImport.HasCargo(self:GetHandle());
end

--- GetDistanceObject
-- @param self GameObject instance.
-- @param h2 GameObject instance.
-- @return distance
function GameObject.GetDistanceObject(self, h2)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(h2) then error("Paramater h2 must be GameObject instance."); end
	return MisnImport.GetDistanceObject(self:GetHandle(), h2:GetHandle());
end

--- GetDistancePath
-- @param self GameObject instance.
-- @param path Path name.
-- @param point Path point number.
-- @return distance
function GameObject.GetDistancePath(self, path, point)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(path) then error("Paramater path must be a string"); end
    if not isnumber(point) then error("Paramater point must be a number"); end
	return MisnImport.GetDistancePath(self:GetHandle(), path, point);
end

--	float MisnImport.GetDistancePathPtr(int &h1, AiPath *path, int point);

--- GetDistanceObject
-- @param self GameObject instance.
-- @return Near Object
function GameObject.GetNearestObject(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = MisnImport.GetNearestObject(self:GetHandle());
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end

--- GetNearestVehicleObject
-- @param self GameObject instance.
-- @return Near Object
function GameObject.GetNearestVehicleObject(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = MisnImport.GetNearestVehicleObject(self:GetHandle());
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end

--- GetNearestVehiclePath
-- @param path Path name.
-- @param point Path point number.
-- @return Near Object
function GameObject.GetNearestVehiclePath(path, point)
    if not isstring(path) then error("Paramater path must be a string"); end
    if not isnumber(point) then error("Paramater point must be a number"); end
    local retVal = MisnImport.GetNearestVehiclePath(path, point);
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end

--- GetNearestBuilding
-- @param self GameObject instance.
-- @return Near Object
function GameObject.GetNearestBuilding(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = MisnImport.GetNearestBuilding(self:GetHandle());
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end
	
function GameObject.GetNearestEnemyH(self)
	return MisnImport.GetNearestEnemy(self:GetHandle());
end




--ConvertHandles
--Read
--Write
--SaveObjects
--LoadObjects

--local MissionData = {};
MissionData = {};

local SavableTypes = {};
local SavableTypeNameToIndex = {};

local LuaTypes = {};
LuaTypes.Nil = 0;
LuaTypes.Table = 1;
LuaTypes.Number = 2;
LuaTypes.Number_Int = 3;
LuaTypes.Number_UByte = 4;
LuaTypes.Boolean = 5;
LuaTypes.String = 6;
LuaTypes.COUNT = 7;

function RegisterSavableType(name, save, load, postload)
    local type = {};
    type.name = name;
    type.save = save;
    type.load = load;
    type.postload = postload;
    table.insert(SavableTypes, type);
    SavableTypeNameToIndex[name] = #SavableTypes;
end

--- Writes a table to save buffer
-- Writes the table marker
-- Writes the count of entries
-- writes the loop below for each
-- Writes the length of the key string
-- Writes the key string
-- Writes the type marker of the value
-- writes the value (using its writer function)
function WriteTable(var)
    WriteMarker("Table");
    --Write #var
    local size = 0;
    for k, v in pairs(var) do
        size = size + 1;
    end
    
    PrintDebugMessage("Start Table of " .. tostring(size) .. " items");
    ffi.C.WriteInt(ffi.new("int[1]", size), 1);
    for k, v in pairs(var) do
        local SavedThisVariable = false;

        local length = string.len(k);
        ffi.C.WriteBytes(ffi.new("uint8_t[1]", length), 1);
        ffi.C.WriteBytes(tocstring(k), length);
        PrintDebugMessage("Key: " .. k .. " (" .. tostring(length) .. ")");
        
        if type(v) == "number" then
            if v == math.floor(v) then
                if v >= 0 and v <= 255 then
                    WriteMarker("Number_UByte");
                    ffi.C.WriteBytes(ffi.new("uint8_t[1]", v), 1);
                    SavedThisVariable = true;
                else
                    WriteMarker("Number_Int");
                    ffi.C.WriteInt(ffi.new("int[1]", v), 1);
                    SavedThisVariable = true;
                end
            else
                -- lua numbers are doubles but BZ2 never seems to use those
                -- I am debating inventing a new numeric type for ints, since this also inflates size and lower accuracy
                -- Maybe I could save it as an INT if it has no decimal?
                WriteMarker("Number");
                ffi.C.WriteFloat(ffi.new("float[1]", v), 1);
                SavedThisVariable = true;
            end
        elseif type(v) == "boolean" then
          WriteMarker("Boolean");
          ffi.C.WriteBool(ffi.new("bool[1]", v), 1);
          SavedThisVariable = true;
        elseif type(v) == "string" then
          WriteMarker("String");
          ffi.C.WriteInt(ffi.new("int[1]", string.len(v)), 1);
          ffi.C.WriteBytes(tocstring(v), string.len(v));
          SavedThisVariable = true;
        end
        
        --do save attempts on all known custom types
        if not SavedThisVariable then
            for SaveTypeIndex = #SavableTypes,1,-1 do
                local SaveTypeData = SavableTypes[SaveTypeIndex];
                if SaveTypeData.save ~= nil then
                    if SaveTypeData.save(v) then
                        SavedThisVariable = true;
                        break;
                    end
                end
            end
        end
        
        if not SavedThisVariable then
            if type(v) == "table" then
                WriteTable(v);
                SavedThisVariable = true;
            end
        end
        
        if not SavedThisVariable then
            PrintConsoleMessage("Failed to save variable \"" .. k .. "\" of LUA type " .. type(v));
            PrintConsoleMessage("The save is likely unstable");
            WriteNil();
        end
    end
    PrintDebugMessage("End Table of " .. tostring(size) .. " items");
end

function ReadTable(var)
    local size = ffi.new("int[1]");
    ffi.C.ReadInt(size, 1);
    size = tonumber(size[0]);
    
    PrintDebugMessage("Start Table of " .. tostring(size) .. " items");
    local keyLength = ffi.new("uint8_t[1]");
    local keyValue = ffi.new("char[256]");
    
    local FloatValue = ffi.new("float[1]");
    local IntValue = ffi.new("int[1]");
    local UByteValue = ffi.new("uint8_t[1]");
    local BoolValue = ffi.new("bool[1]");
    
    for k = 1,size,1 do
        --local LoadedThisVariable = false;
        
        ffi.fill(keyValue, 256);
        ffi.C.ReadBytes(keyLength, 1);
        ffi.C.ReadBytes(keyValue, tonumber(keyLength[0]));
        
        PrintDebugMessage("Key: " .. ffi.string(keyValue) .. " (" .. tostring(tonumber(keyLength[0])) .. ")");
        
        ffi.C.ReadBytes(keyLength, 1); -- keyLength now holds the type byte,double duty
        local valueType = tonumber(keyLength[0]);
        
        local value = nil;
        if valueType >= LuaTypes.COUNT then
            -- type is a custom type
            local SaveTypeData = SavableTypes[valueType + 1 - LuaTypes.COUNT];
            PrintDebugMessage("Key is of type " .. SaveTypeData.name .. " (" .. tostring(valueType + 1 - LuaTypes.COUNT) .. ")");
            if SaveTypeData.load ~= nil then
                value = SaveTypeData.load();
            end
        elseif valueType == LuaTypes.Nil then
            -- do nothing, nil has no data after the marker
            PrintDebugMessage("Key is of type is Nil (" .. tostring(LuaTypes.Nil) .. ")");
        elseif valueType == LuaTypes.Table then
            PrintDebugMessage("Key is of type Table (" .. tostring(LuaTypes.Table) .. ")");
            value = {};
            ReadTable(value);
        elseif valueType == LuaTypes.Number then
            PrintDebugMessage("Key is of type Number (" .. tostring(LuaTypes.Number) .. ")");
            ffi.C.ReadFloat(FloatValue, 1);
            value = tonumber(FloatValue[0]);
        elseif valueType == LuaTypes.Number_Int then
            PrintDebugMessage("Key is of type Number_Int (" .. tostring(LuaTypes.Number_Int) .. ")");
            ffi.C.ReadInt(IntValue, 1);
            value = tonumber(IntValue[0]);
        elseif valueType == LuaTypes.Number_UByte then
            PrintDebugMessage("Key is of type Number_UByte (" .. tostring(LuaTypes.Number_UByte) .. ")");
            ffi.C.ReadBytes(UByteValue, 1);
            value = tonumber(UByteValue[0]);
        elseif valueType == LuaTypes.Boolean then
            PrintDebugMessage("Key is of type Boolean (" .. tostring(LuaTypes.Boolean) .. ")");
            ffi.C.ReadBool(BoolValue, 1);
            value = tonumber(BoolValue[0]) ~= 0; -- maybe ~= nil?
        elseif valueType == LuaTypes.String then
            ffi.C.ReadInt(IntValue, 1);
            local stringLength = tonumber(IntValue[0]);
            local stringVal = ffi.new("char[?]", stringLength + 1);
            ffi.fill(stringVal, stringLength + 1);
            PrintDebugMessage("Key is of type String (" .. tostring(LuaTypes.String) .. ")");
            ffi.C.ReadBytes(stringVal, stringLength);
            value = ffi.string(stringVal);
        end
        
        var[ffi.string(keyValue)] = value;
        PrintDebugMessage(ffi.string(keyValue) .. " = " .. value);
    end
    PrintDebugMessage("End Table of " .. tostring(size) .. " items");
end

function WriteNil()
    WriteMarker("Nil"); -- nill is just a marker, that's all
end

function WriteMarker(id)
    if LuaTypes[id] ~= nil then
        ffi.C.WriteBytes(ffi.new("uint8_t[1]", LuaTypes[id]), 1);
        PrintDebugMessage("Write marker \"" .. id .. "\" ID: " .. LuaTypes[id]);
    elseif SavableTypeNameToIndex[id] ~= nil then
        ffi.C.WriteBytes(ffi.new("uint8_t[1]", LuaTypes.COUNT - 1 + SavableTypeNameToIndex[id]), 1);
        PrintDebugMessage("Write marker \"" .. id .. "\" ID: " .. LuaTypes.COUNT - 1 + SavableTypeNameToIndex[id]);
    end
end





function PrintDebugMessage(message)
    PrintConsoleMessage(message);
end


function SaveGameObject(val)
    if not isgameobject(val) or val.Save == nil or type(val.Save) ~= "function" then return false; end
    val:Save();
    return true;
end

function LoadGameObject()
    local id = ffi.new("int[1]");
    ffi.C.ReadInt(id, 1);
    return GameObject.new(tonumber(id[0]));
end

function PostLoadGameObject(val)
    val:PostLoad();
end

function SaveColor(val)
    if not iscolor(val) then return false; end
    WriteMarker("Color");
    ffi.C.WriteInt(ffi.new("int[1]", val.ToColorLong()), 1);
    return true;
end

function LoadColor()
    local id = ffi.new("int[1]");
    ffi.C.ReadInt(id, 1);
    return Color.new(tonumber(id[0]));
end

function SaveAudioHandle(val)
    if not isaudiohandle(val) or val.Save == nil or type(val.Save) ~= "function" then return false; end
    val:Save();
    return true;
end

function LoadAudioHandle()
    local id = ffi.new("int[1]");
    ffi.C.ReadInt(id, 1);
    return AudioHandle.new(tonumber(id[0]));
end

function SaveVECTOR_2D(val)
    if not isvector_2d(val) then return false; end
    WriteMarker("VECTOR_2D");
    ffi.C.WriteBytes(val, ffi.sizeof(val));
    return true;
end

function LoadVECTOR_2D()
    local retVal = VECTOR_2D();
    ffi.C.ReadBytes(retVal, ffi.sizeof(retVal));
    return retVal;
end

function SaveVector(val)
    if not isvector(val) then return false; end
    WriteMarker("Vector");
    ffi.C.WriteBytes(val, ffi.sizeof(val));
    return true;
end

function LoadVector()
    local retVal = Vector();
    ffi.C.ReadBytes(retVal, ffi.sizeof(retVal));
    return retVal;
end

function SaveMatrix(val)
    if not ismatrix(val) then return false; end
    WriteMarker("Matrix");
    ffi.C.WriteBytes(val, ffi.sizeof(val));
    return true;
end

function LoadMatrix()
    local retVal = Matrix();
    ffi.C.ReadBytes(retVal, ffi.sizeof(retVal));
    return retVal;
end

function SaveObjectDefinition(val)
    if not isobjectdefinition(val) or val.Save == nil or type(val.Save) ~= "function" then return false; end
    val:Save();
    return true;
end

function LoadObjectDefinition()
    local length = ffi.new("int[1]");
    length = tonumber(length[0]);
    local name = ffi.new("char[?]", length + 1);
    ffi.fill(name, length + 1)
    ffi.C.ReadBytes(name, length);
    return ObjectDefinition(ffi.string(name));
end

function InitialSetup()
    --local here;
    --local coming;
    --here,coming = friendTank:CountThreats();
    --PrintConsoleMessage("Here: " .. here .. " Coming: " .. coming);
    
    --MissionData.TestData = {};
    --MissionData.TestData.Spawned = false;
    --MissionData.TestData.FreshLoad = false;
    
    hook.Call( "InitialSetup" );
end

function Update()
	--io.write("Update, from ",_VERSION,"!\n")
  --PrintConsoleMessage("Update, from " .. _VERSION .. "!\n");
  
    --PrintConsoleMessage(tostring(MisnImport.GetNearestBuilding(GetPlayerHandle(1):GetHandle())));
    
    --if not MissionData.TestData.Spawned then
    --    MissionData.TestData.friendTank = BuildObject("ivtank",1,Vector());
    --    MissionData.TestData.enemyTank = BuildObject("ivtank",2,Vector(100,100,100));
    --    MissionData.TestData.enemyTank:Attack(MissionData.TestData.friendTank);
    --    MissionData.TestData.Spawned = true;
    --end
    
    --if MissionData.TestData.FreshLoad then
    --    MissionData.TestData.friendTank:EjectPilot();
    --    MissionData.TestData.enemyTank:EjectPilot();
    --end
        
    hook.Call( "Update", MissionData.GameTurn );
    MissionData.GameTurn = MissionData.GameTurn + 1;
end

function Save(misnSave)

    if misnSave then
        -- clean var arrays
        return true;
    end

    local returnValue = true;
    
    PrintDebugMessage(tostring(#SavableTypes) .. " types to save");
    returnValue = returnValue and ffi.C.WriteBytes(ffi.new("uint8_t[1]", #SavableTypes), 1);
    for k = 1,#SavableTypes,1 do
        local length = string.len(SavableTypes[k].name);
        returnValue = returnValue and ffi.C.WriteBytes(ffi.new("uint8_t[1]", length), 1); -- type names can't be longer than 255
        returnValue = returnValue and ffi.C.WriteBytes(tocstring(SavableTypes[k].name), length);
        PrintDebugMessage(length .. " " .. SavableTypes[k].name .. " = " .. LuaTypes.COUNT - 1 + k);
    end
    
    -- GameObjectAltered data dump
    -- this remakes the gameobjects before they are needed with their addon data
    -- The way GameObject.new works will allow these to be caught
    local countCustomGameObjects = 0;
    for k,v in pairs(GameObjectAltered) do
        countCustomGameObjects = countCustomGameObjects + 1;
    end
    returnValue = returnValue and ffi.C.WriteInt(ffi.new("int[1]", countCustomGameObjects),1);
    PrintDebugMessage("Saving " .. countCustomGameObjects .. " Custom Game Objects Handles");
    for k,v in pairs(GameObjectAltered) do
        SaveGameObject(v);
    end
    PrintDebugMessage("Saving " .. countCustomGameObjects .. " Custom Game Objects Data Extensions");
    for k,v in pairs(GameObjectAltered) do
        WriteTable(v.addonData);
    end
    
    WriteTable(MissionData);

    local hookResults = { hook.CallAll( "Save", misnSave ) };
    if hookResults ~= nil then
      for k,v in pairs(hookResults) do
        returnValue = returnValue and v;
      end
    end
    
    return returnValue
end

function Load(misnSave)
	--io.write("Load("..tostring(misnSave).."), from ",_VERSION,"!\n")
  
    if misnSave then
        return true;
    end
    
    local returnValue = true;
    
    -- dump the weak reference table
    for k,v in pairs(GameObjectWeakList) do
        GameObjectWeakList[k] = nil;
    end
    
    local countTypes = ffi.new("uint8_t[1]");
    returnValue = returnValue and ffi.C.ReadBytes(countTypes, 1);
    countTypes = tonumber(countTypes[0]);
    PrintDebugMessage(tostring(countTypes) .. " types to load");
    local length = ffi.new("uint8_t[1]");
    local name = ffi.new("char[?]",256);
    for k = 1,countTypes,1 do
        returnValue = returnValue and ffi.C.ReadBytes(length, 1);
        ffi.fill(name,256); -- memset
        returnValue = returnValue and ffi.C.ReadBytes(name, tonumber(length[0]));
        PrintDebugMessage(tonumber(length[0]) .. " " .. ffi.string(name) .. " = " .. LuaTypes.COUNT - 1 + k);
    end
    
    -- remap types here
    
    -- GameObjectAltered data
    local countCustomGameObjects = ffi.new("int[1]");
    ffi.C.ReadInt(countCustomGameObjects, 1);
    countCustomGameObjects = tonumber(countCustomGameObjects[0]);
    PrintDebugMessage("Reading " .. countCustomGameObjects .. " Custom Game Objects Handles");
    local TmpGameObjects = {};
    for k = 1,countCustomGameObjects,1 do
        ffi.C.ReadBytes(length, 1); -- useless byte :/
        table.insert(TmpGameObjects, LoadGameObject());
    end
    PrintDebugMessage("Reading " .. countCustomGameObjects .. " Custom Game Objects Data Extensions");
    for k = 1,countCustomGameObjects,1 do
        ffi.C.ReadBytes(length, 1); -- useless byte :/
        ReadTable(TmpGameObjects[k].addonData);
    end
    
    -- use the length byte we don't need anymore to pull off the table marker
    -- We know that this first item must be a table
    ffi.C.ReadBytes(length, 1);
    ReadTable(MissionData);
    
    PrintConsoleMessage(type(MissionData) .. " " .. table.getn(MissionData));
    
    hook.Call( "Load", misnSave );
    
    return returnValue;
end

function PostLoad(misnSave)
    if misnSave then
      return true;
    end
    
    for k,v in pairs(GameObjectWeakList) do
        v:PostLoad();
    end
    
    hook.Call( "PostLoad", misnSave );
    
    return true;
end

function AddObject(h)
    hook.Call( "AddObject", GameObject.new(h) );
end

function DeleteObject(h)
    local object = GameObject.new(h);
    hook.Call( "DeleteObject", object );
    if GameObjectAltered[object:GetHandle()] ~= nil then GameObjectAltered[object:GetHandle()] = nil; end
end

function PostRun()
    hook.Call( "PostRun", GameObject.new(h) );
end

function AddPlayer(id, team, shouldCreateThem)
    local returnValue = true;
    local hookResults = { hook.CallAll( "AddPlayer", id, team, shouldCreateThem ) };
    if hookResults ~= nil then
      for k,v in pairs(hookResults) do
        returnValue = returnValue and v;
      end
    end
    
    return returnValue;
end

function DeletePlayer(id)
    hook.Call( "DeletePlayer", id );
end

function PlayerEjected(deadObjectHandle)
    local retVal = hook.Call( "PlayerEjected", GameObject.new(deadObjectHandle));
    if retVal ~= nil then return retVal; else return 0; end
end

function ObjectKilled(deadObjectHandle, killersHandle)
    local retVal = hook.Call( "ObjectKilled", GameObject.new(deadObjectHandle), GameObject.new(killersHandle));
    if retVal ~= nil then return retVal; else return 1; end
end

function ObjectSniped(deadObjectHandle, killersHandle)
    local retVal = hook.Call( "ObjectSniped", GameObject.new(deadObjectHandle), GameObject.new(killersHandle));
    if retVal ~= nil then return retVal; else return 2; end
end

function GetNextRandomVehicleODF(team)
    local retVal = hook.Call( "GetNextRandomVehicleODF", team );
    if retVal ~= nil then return retVal; end
    return "someVehicle"
end

function SetWorld(nextWorld)
    hook.Call( "SetWorld", nextWorld );
end

function ProcessCommand(crc)
    hook.Call( "ProcessCommand", crc );
end

function SetRandomSeed(seed)
    hook.Call( "SetRandomSeed", seed );
end
    MissionData.GameTurn = 0;
    
    RegisterSavableType("Color", SaveColor, LoadColor);
    RegisterSavableType("AudioHandle", SaveAudioHandle, LoadAudioHandle);
    RegisterSavableType("ObjectDefinition", SaveObjectDefinition, LoadObjectDefinition);
    RegisterSavableType("GameObject", SaveGameObject, LoadGameObject, PostLoadGameObject);
    RegisterSavableType("VECTOR_2D", SaveVECTOR_2D, LoadVECTOR_2D);
    RegisterSavableType("Vector", SaveVector, LoadVector);
    RegisterSavableType("Matrix", SaveMatrix, LoadMatrix);
    PrintDebugMessage(tostring(#SavableTypes) .. " Custom DataTypes");
    for k = #SavableTypes,1,-1 do
        local v = SavableTypes[k];
        PrintDebugMessage(tostring(SavableTypeNameToIndex[v.name]) .. " " .. tostring(v.name));
    end
    --PrintConsoleMessage("Loading TRN Data");
    --local MapDataFile = GetMapTRNFilename();
    --PrintConsoleMessage("TRN File: " .. MapDataFile);
    --local MapData = ObjectDefinition(MapDataFile);
    --MapData:Open();
    --local TRNScriptName = MapData:GetString("DLL","LuaMission");
    --MapData:Close();
    --PrintConsoleMessage("Script File: " .. TRNScriptName);
    --if TRNScriptName ~= nil and string.len(TRNScriptName) > 0 then
    --    Core.LoadFromBZ2(TRNScriptName);
    --else
    --    PrintConsoleMessage("TRN Script Not Set");
    --    Core.LoadFromBZ2("Test.lua");
    --    PrintConsoleMessage("Loaded Test.lua");
    --end
    
    PrintConsoleMessage("Loading Modules");
    require("hook");
    
    local MapLuaFile = string.gsub(GetMapTRNFilename(),"%.trn$",".lua");
    PrintConsoleMessage("Loading Lua: " .. MapLuaFile);
    if Core.LoadFromBZ2(MapLuaFile) then
      PrintConsoleMessage("Map lua loaded");
    else
      PrintConsoleMessage("Failed to load map lua");
    end