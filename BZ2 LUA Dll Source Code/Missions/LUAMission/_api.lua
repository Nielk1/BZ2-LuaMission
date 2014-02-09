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
//__declspec( dllexport ) void __cdecl SetControls(int,struct VehicleControls const &,unsigned long) asm("?SetControls@@YAXHABUVehicleControls@@K@Z");
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
//__declspec( dllexport ) void __cdecl ConvertHandles(int *,int) asm("?ConvertHandles@@YAXPAHH@Z");
//__declspec( dllexport ) bool __cdecl Read(void *,int) asm("?Read@@YA_NPAXH@Z");
//__declspec( dllexport ) bool __cdecl Read(bool *,int) asm("?Read@@YA_NPA_NH@Z");
//__declspec( dllexport ) bool __cdecl Read(float *,int) asm("?Read@@YA_NPAMH@Z");
//__declspec( dllexport ) bool __cdecl Read(int *,int) asm("?Read@@YA_NPAHH@Z");
//__declspec( dllexport ) bool __cdecl Write(void *,int) asm("?Write@@YA_NPAXH@Z");
//__declspec( dllexport ) bool __cdecl Write(bool *,int) asm("?Write@@YA_NPA_NH@Z");
//__declspec( dllexport ) bool __cdecl Write(float *,int) asm("?Write@@YA_NPAMH@Z");
//__declspec( dllexport ) bool __cdecl Write(int *,int) asm("?Write@@YA_NPAHH@Z");
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
__declspec( dllexport ) void __cdecl SetUserTarget(int) asm("?SetUserTarget@@YAXH@Z");
//__declspec( dllexport ) int __cdecl GetUserTarget(void) asm("?GetUserTarget@@YAHXZ");// legacy
__declspec( dllexport ) void __cdecl SetPerceivedTeam(int,int) asm("?SetPerceivedTeam@@YAXHH@Z");
//__declspec( dllexport ) enum AiCommand __cdecl GetCurrentCommand(int) asm("?GetCurrentCommand@@YA?AW4AiCommand@@H@Z");
__declspec( dllexport ) int __cdecl GetCurrentWho(int) asm("?GetCurrentWho@@YAHH@Z");
__declspec( dllexport ) void __cdecl EjectPilot(int) asm("?EjectPilot@@YAXH@Z");
__declspec( dllexport ) void __cdecl HopOut(int) asm("?HopOut@@YAXH@Z");
__declspec( dllexport ) void __cdecl KillPilot(int) asm("?KillPilot@@YAXH@Z");
__declspec( dllexport ) void __cdecl RemovePilotAI(int) asm("?RemovePilotAI@@YAXH@Z");
__declspec( dllexport ) int __cdecl HoppedOutOf(int) asm("?HoppedOutOf@@YAHH@Z");
//__declspec( dllexport ) void __cdecl GetCameraPosition(struct Vector &,struct Vector &) asm("?GetCameraPosition@@YAXAAUVector@@0@Z");
//__declspec( dllexport ) void __cdecl SetCameraPosition(struct Vector const &,struct Vector const &) asm("?SetCameraPosition@@YAXABUVector@@0@Z");
//__declspec( dllexport ) void __cdecl ResetCameraPosition(void) asm("?ResetCameraPosition@@YAXXZ");
__declspec( dllexport ) unsigned long __cdecl CalcCRC(char *) asm("?CalcCRC@@YAKPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_Exec(char *) asm("?IFace_Exec@@YAXPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_Activate(char *) asm("?IFace_Activate@@YAXPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_Deactivate(char *) asm("?IFace_Deactivate@@YAXPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_CreateCommand(char *) asm("?IFace_CreateCommand@@YAXPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_CreateString(char *,char *) asm("?IFace_CreateString@@YAXPAD0@Z");
//__declspec( dllexport ) void __cdecl IFace_SetString(char *,char *) asm("?IFace_SetString@@YAXPAD0@Z");
//__declspec( dllexport ) void __cdecl IFace_GetString(char *,char *,int) asm("?IFace_GetString@@YAXPAD0H@Z");
//__declspec( dllexport ) void __cdecl IFace_CreateInteger(char *,int) asm("?IFace_CreateInteger@@YAXPADH@Z");
//__declspec( dllexport ) void __cdecl IFace_SetInteger(char *,int) asm("?IFace_SetInteger@@YAXPADH@Z");
//__declspec( dllexport ) int __cdecl IFace_GetInteger(char *) asm("?IFace_GetInteger@@YAHPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_SetIntegerRange(char *,int,int) asm("?IFace_SetIntegerRange@@YAXPADHH@Z");
//__declspec( dllexport ) void __cdecl IFace_CreateFloat(char *,float) asm("?IFace_CreateFloat@@YAXPADM@Z");
//__declspec( dllexport ) void __cdecl IFace_SetFloat(char *,float) asm("?IFace_SetFloat@@YAXPADM@Z");
//__declspec( dllexport ) float __cdecl IFace_GetFloat(char *) asm("?IFace_GetFloat@@YAMPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_ClearListBox(char *) asm("?IFace_ClearListBox@@YAXPAD@Z");
//__declspec( dllexport ) void __cdecl IFace_AddTextItem(char *,char *) asm("?IFace_AddTextItem@@YAXPAD0@Z");
//__declspec( dllexport ) void __cdecl IFace_GetSelectedItem(char *,char *,int) asm("?IFace_GetSelectedItem@@YAXPAD0H@Z");
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
//__declspec( dllexport ) bool __cdecl CameraReady(void) asm("?CameraReady@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl CameraPath(char *,int,int,int) asm("?CameraPath@@YA_NPADHHH@Z");
//__declspec( dllexport ) bool __cdecl CameraPathDir(char *,int,int) asm("?CameraPathDir@@YA_NPADHH@Z");
//__declspec( dllexport ) bool __cdecl PanDone(void) asm("?PanDone@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl CameraObject(int,float,float,float,int) asm("?CameraObject@@YA_NHMMMH@Z");
//__declspec( dllexport ) bool __cdecl CameraFinish(void) asm("?CameraFinish@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl CameraCancelled(void) asm("?CameraCancelled@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl FreeCamera(void) asm("?FreeCamera@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl FreeFinish(void) asm("?FreeFinish@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl PlayMovie(char * const) asm("?PlayMovie@@YA_NQAD@Z");
//__declspec( dllexport ) void __cdecl StopMovie(void) asm("?StopMovie@@YAXXZ");
//__declspec( dllexport ) bool __cdecl PlayMove(void) asm("?PlayMove@@YA_NXZ");
//__declspec( dllexport ) bool __cdecl PlayRecording(char * const) asm("?PlayRecording@@YA_NQAD@Z");
//__declspec( dllexport ) bool __cdecl PlayRecording(char * const,bool) asm("?PlayRecording@@YA_NQAD_N@Z");
//__declspec( dllexport ) bool __cdecl PlaybackVehicle(char * const) asm("?PlaybackVehicle@@YA_NQAD@Z");
__declspec( dllexport ) float __cdecl SetAnimation(int,char *,int) asm("?SetAnimation@@YAMHPADH@Z");
__declspec( dllexport ) float __cdecl GetAnimationFrame(int,char *) asm("?GetAnimationFrame@@YAMHPAD@Z");
__declspec( dllexport ) void __cdecl StartAnimation(int) asm("?StartAnimation@@YAXH@Z");
__declspec( dllexport ) void __cdecl MaskEmitter(int,unsigned long) asm("?MaskEmitter@@YAXHK@Z");
__declspec( dllexport ) void __cdecl StartEmitter(int,int) asm("?StartEmitter@@YAXHH@Z");
__declspec( dllexport ) void __cdecl StopEmitter(int,int) asm("?StopEmitter@@YAXHH@Z");
//__declspec( dllexport ) void __cdecl SaveObjects(char * &,unsigned long &) asm("?SaveObjects@@YAXAAPADAAK@Z");
//__declspec( dllexport ) void __cdecl LoadObjects(char *,unsigned long) asm("?LoadObjects@@YAXPADK@Z");
__declspec( dllexport ) void __cdecl IgnoreSync(bool) asm("?IgnoreSync@@YAX_N@Z");
__declspec( dllexport ) bool __cdecl IsRecording(void) asm("?IsRecording@@YA_NXZ");
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
//__declspec( dllexport ) void __cdecl SpawnBirds(int,int,char *,int,char *) asm("?SpawnBirds@@YAXHHPADH0@Z");
//__declspec( dllexport ) void __cdecl SpawnBirds(int,int,char *,int,int,int) asm("?SpawnBirds@@YAXHHPADHHH@Z");
//__declspec( dllexport ) void __cdecl RemoveBirds(int) asm("?RemoveBirds@@YAXH@Z");
__declspec( dllexport ) void __cdecl SetColorFade(float,float,unsigned long) asm("?SetColorFade@@YAXMMK@Z");
__declspec( dllexport ) void __cdecl StopCheats(void) asm("?StopCheats@@YAXXZ");
//__declspec( dllexport ) void __cdecl CalcCliffs(int,int,float) asm("?CalcCliffs@@YAXHHM@Z");
//__declspec( dllexport ) void __cdecl CalcCliffs(char *) asm("?CalcCliffs@@YAXPAD@Z");
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
//__declspec( dllexport ) char * __cdecl GetPlayerODF(int,enum RandomizeType) asm("?GetPlayerODF@@YAPADHW4RandomizeType@@@Z");
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
//__declspec( dllexport ) bool __cdecl GetObjInfo(int,enum ObjectInfoType,char * const) asm("?GetObjInfo@@YA_NHW4ObjectInfoType@@QAD@Z");
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
//__declspec( dllexport ) int __cdecl GetODFHexInt(char const *,char const *,char const *,int *,int) asm("?GetODFHexInt@@YAHPBD00PAHH@Z");
//__declspec( dllexport ) int __cdecl GetODFInt(char const *,char const *,char const *,int *,int) asm("?GetODFInt@@YAHPBD00PAHH@Z");
//__declspec( dllexport ) int __cdecl GetODFLong(char const *,char const *,char const *,long *,long) asm("?GetODFLong@@YAHPBD00PAJJ@Z");
//__declspec( dllexport ) int __cdecl GetODFFloat(char const *,char const *,char const *,float *,float) asm("?GetODFFloat@@YAHPBD00PAMM@Z");
//__declspec( dllexport ) int __cdecl GetODFDouble(char const *,char const *,char const *,double *,double) asm("?GetODFDouble@@YAHPBD00PANN@Z");
//__declspec( dllexport ) int __cdecl GetODFChar(char const *,char const *,char const *,char *,char) asm("?GetODFChar@@YAHPBD00PADD@Z");
//__declspec( dllexport ) int __cdecl GetODFBool(char const *,char const *,char const *,bool *,bool) asm("?GetODFBool@@YAHPBD00PA_N_N@Z");
//__declspec( dllexport ) int __cdecl GetODFString(char const *,char const *,char const *,unsigned int,char *,char const *) asm("?GetODFString@@YAHPBD00IPAD0@Z");
//__declspec( dllexport ) int __cdecl GetODFColor(char const *,char const *,char const *,unsigned long *,unsigned long) asm("?GetODFColor@@YAHPBD00PAKK@Z");
//__declspec( dllexport ) int __cdecl GetODFVector(char const *,char const *,char const *,struct Vector *,struct Vector) asm("?GetODFVector@@YAHPBD00PAUVector@@U1@@Z");
//__declspec( dllexport ) bool __cdecl OpenODF(char *) asm("?OpenODF@@YA_NPAD@Z");
//__declspec( dllexport ) bool __cdecl CloseODF(char *) asm("?CloseODF@@YA_NPAD@Z");
__declspec( dllexport ) void __cdecl NoteGameoverWithCustomMessage(char const *) asm("?NoteGameoverWithCustomMessage@@YAXPBD@Z");
__declspec( dllexport ) int __cdecl SetBestGroup(int) asm("?SetBestGroup@@YAHH@Z");
__declspec( dllexport ) void __cdecl GetGroup(int,int,enum ObjectInfoType,char * const) asm("?GetGroup@@YAXHHW4ObjectInfoType@@QAD@Z");
__declspec( dllexport ) int __cdecl GetGroupCount(int,int) asm("?GetGroupCount@@YAHHH@Z");
__declspec( dllexport ) void __cdecl SetLifespan(int,float) asm("?SetLifespan@@YAXHM@Z");
__declspec( dllexport ) bool __cdecl DoesFileExist(char const *) asm("?DoesFileExist@@YA_NPBD@Z");
__declspec( dllexport ) bool __cdecl LoadFile(char const *,void *,unsigned int &) asm("?LoadFile@@YA_NPBDPAXAAI@Z");
typedef int DLLAudioHandle;
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
// 	CMD_NONE, // == 0
// 	CMD_SELECT, // == 1
// 	CMD_STOP, // == 2
// 	CMD_GO, // == 3
// 	CMD_ATTACK, // == 4
// 	CMD_FOLLOW, // ... you do the math. Values increase by 1 per line
// 	CMD_FORMATION, // not used anywhere in code.
// 	CMD_PICKUP,
// 	CMD_DROPOFF,
// 	CMD_UNDEPLOY,
// 	CMD_DEPLOY,
// 	CMD_NO_DEPLOY, // Used by crigs, deploybuildings to indicate they can't do that there
// 	CMD_GET_REPAIR,
// 	CMD_GET_RELOAD,
// 	CMD_GET_WEAPON,
// 	CMD_GET_CAMERA, // Human players only.
// 	CMD_GET_BOMB,
// 	CMD_DEFEND,
// 	CMD_RESCUE,
// 	CMD_RECYCLE,
// 	CMD_SCAVENGE,
// 	CMD_HUNT,
// 	CMD_BUILD,
// 	CMD_PATROL,
// 	CMD_STAGE,
// 	CMD_SEND,
// 	CMD_GET_IN,
// 	CMD_LAY_MINES,
// 	CMD_LOOK_AT,
// 	CMD_SERVICE, // == 29
// 	CMD_UPGRADE,
// 	CMD_DEMOLISH,
// 	CMD_POWER,
// 	CMD_BACK,
// 	CMD_DONE,
// 	CMD_CANCEL,
// 	CMD_SET_GROUP,
// 	CMD_SET_TEAM,
// 	CMD_SEND_GROUP,
// 	CMD_TARGET,
// 	CMD_INSPECT,
// 	CMD_SWITCHTEAM,
// 	CMD_INTERFACE,
// 	CMD_LOGOFF,
// 	CMD_AUTOPILOT,
// 	CMD_MESSAGE,
// 	CMD_CLOSE,
// 	CMD_MORPH_SETDEPLOYED, // For morphtanks
// 	CMD_MORPH_SETUNDEPLOYED, // For morphtanks
// 	CMD_MORPH_UNLOCK, // For morphtanks
// 	CMD_BAILOUT,
// 	CMD_BUILD_ROTATE, // Update building rotations by 90 degrees.
// 	CMD_CMDPANEL_SELECT,
// 	CMD_CMDPANEL_DESELECT,
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
__declspec( dllexport ) char const * __cdecl GetLabel(int) asm("?GetName@@YAPBDH@Z");
__declspec( dllexport ) void __cdecl SetLabel(int,char * const) asm("?SetName@@YAXHQAD@Z");
__declspec( dllexport ) int __cdecl GetTap(int,int) asm("?GetTap@@YAHHH@Z");
__declspec( dllexport ) void __cdecl SetTap(int,int,int) asm("?SetTap@@YAXHHH@Z");
__declspec( dllexport ) float __cdecl GetCurLocalAmmo(int,int) asm("?GetCurLocalAmmo@@YAMHH@Z");
__declspec( dllexport ) void __cdecl AddLocalAmmo(int,float,int) asm("?AddLocalAmmo@@YAXHMH@Z");
__declspec( dllexport ) float __cdecl GetMaxLocalAmmo(int,int) asm("?GetMaxLocalAmmo@@YAMHH@Z");
__declspec( dllexport ) void __cdecl SetCurLocalAmmo(int,float,int) asm("?SetCurLocalAmmo@@YAXHMH@Z");
]]

--==============================================================================================================================================================
-- Utility Functions
--==============================================================================================================================================================

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

--- Is this oject a string?
-- @param object Object in question
function isstring(object)
    return (type(object) == "string");
end

--- Is this oject a Vector?
-- @param object Object in question
function isvector(object)
    return (type(object) == "cdata" and ffi.istype("Vector", object));
end

--- Is this oject a Matrix?
-- @param object Object in question
function ismatrix(object)
    return (type(object) == "cdata" and ffi.istype("Matrix", object));
end

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
-- GameObject
--==============================================================================================================================================================

--- GameObject
-- An object containing all functions and data related to a game object.
-- [future] GameObject will survive postload.
-- [future] GameObject will preserve extra data added by serving existing instance for given id.
local GameObject = {}; -- the table representing the class, which will double as the metatable for the instances
GameObject.__index = GameObject; -- failed table lookups on the instances should fallback to the class table, to get methods

--- Create new GameObject Intance
-- @param id Handle from BZ2.
function GameObject.new(id)
  local self = setmetatable({}, GameObject);
  self.id = id;
  return self;
end

--- Get Handle used by BZ2.
-- @param self GameObject instance.
function GameObject.GetHandle(self)
  return self.id;
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












--- Build Object
-- @param odf Object Definition File (without ".odf")
-- @param team Team number for the object, 0 to 15
-- @param pos Position as GameObject, Pathpoint Name, AiPath, Vector, or Matrix
function BuildObject(odf, team, pos)
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



function AllLookAt(team, target, priority)
  if priority == nil then priority = 1; end
  if type(target) == "table" and target.GetHandle ~= nil and type(target.GetHandle) == "function" then
    ffi.C.AllLookAt(team, target:GetHandle(), priority);
  else
    error("AllLookAt target type invalid, received " .. type(target) .. ", expected GameObject");
  end
end
function GameObject.IsOdf(self, odf)
  if type(odf) == "string" then
    local odfClean = tocstring(odf);
    return ffi.C.IsOdf(self:GetHandle(), odfClean);
  else
    error("IsOdf paramater type invalid, received " .. type(odf) .. ", expected string");
  end
end
function GameObject.GetRace(self)
  return ffi.C.GetRace(self:GetHandle());
end
function GameObject.IsAround(self)
  return ffi.C.IsAround(self:GetHandle());
end
function GameObject.InBuilding(self)
  return Handle.new(ffi.C.InBuilding(self:GetHandle()));
end
function GameObject.AtTerminal(self)
  return Handle.new(ffi.C.AtTerminal(self:GetHandle()));
end
function GetHandle(name)
  if type(name) == "string" then
    local cName = tocstring(name);
    return Handle.new(ffi.C.GetHandle(cName));
  elseif type(name) == "number" then
    return Handle.new(ffi.C.GetHandleSeq(name));
  else
    error("GetHandle paramater type invalid, received " .. type(name) .. ", expected string or int");
  end
end
function GameObject.GetPositionV(self)
  local Vector retVal = Vector();
  ffi.C.GetPositionV(self:GetHandle(), retVal);
  return retVal;
end
function GameObject.GetPosition2(self)
  local Vector retVal = Vector();
  ffi.C.GetPosition2(self:GetHandle(), retVal);
  return retVal;
end
function GameObject.GetFront(self)
  local Vector retVal = Vector();
  ffi.C.GetFront(self:GetHandle(), retVal);
  return retVal;
end
function GameObject.GetPositionM(self)
  local Matrix retVal = Matrix();
  ffi.C.GetPositionM(self:GetHandle(), retVal);
  return retVal;
end
function GameObject.SetPositionM(self, loc)
  ffi.C.GetPositionM(self:GetHandle(), loc);
end
function GameObject.Damage(self, amt)
  ffi.C.Damage(self:GetHandle(), amt);
end
function GameObject.GetHealth(self)
  return ffi.C.GetHealth(self:GetHandle());
end
function GameObject.GetCurHealth(self)
  return ffi.C.GetCurHealth(self:GetHandle());
end
function GameObject.GetMaxHealth(self)
  return ffi.C.GetMaxHealth(self:GetHandle());
end
function GameObject.SetCurHealth(self, NewHealth)
  ffi.C.SetCurHealth(self:GetHandle(), NewHealth);
end
function GameObject.SetMaxHealth(self, NewHealth)
  ffi.C.SetMaxHealth(self:GetHandle(), NewHealth);
end
function GameObject.AddHealth(self, health)
  ffi.C.AddHealth(self:GetHandle(), health)
end
function GameObject.GetAmmo(self)
  return ffi.C.GetAmmo(self:GetHandle());
end
function GameObject.AddAmmo(self, ammo)
  ffi.C.AddAmmo(self:GetHandle(), ammo);
end
function GameObject.GetCurAmmo(self)
  return ffi.C.GetCurAmmo(self:GetHandle());
end
function GameObject.GetMaxAmmo(self)
  return ffi.C.GetMaxAmmo(self:GetHandle());
end
function GameObject.SetCurAmmo(self, NewAmmo)
  ffi.C.SetCurAmmo(self:GetHandle(), NewAmmo);
end
function GameObject.SetMaxAmmo(self, NewAmmo)
  ffi.C.SetMaxAmmo(self:GetHandle(), NewAmmo);
end
function GameObject.GetTeamNum(self)
  return ffi.C.GetTeamNum(self:GetHandle());
end
function GameObject.SetTeamNum(self, t)
  ffi.C.SetTeamNum(self:GetHandle(), t);
end
function GameObject.SetPositionP(self, path)
  ffi.C.SetPositionP(self:GetHandle(), tocstring(path));
end
function GameObject.SetVectorPosition(self, pos)
  ffi.C.SetVectorPosition(self:GetHandle(), pos);
end
function GameObject.SetVelocity(self, vel)
  ffi.C.SetVelocity(self:GetHandle(), vel);
end
--SetControls
function GameObject.GetWhoShotMe(self)
  return Handle.new(ffi.C.GetWhoShotMe(self:GetHandle()));
end
--GetLastEnemyShot
--GetLastFriendShot
function DefaultAllies()
  ffi.C.DefaultAllies();
end
function TeamplayAllies()
  ffi.C.TeamplayAllies();
end
function Ally(t1, t2)
  ffi.C.Ally(t1, t2);
end
function UnAlly(t1, t2)
  ffi.C.UnAlly(t1, t2);
end
function GameObject.IsAlly(self, him)
  return ffi.C.IsAlly(self:GetHandle(), him);
end
function AudioMessage(msg, purge)
  if purge == nil then purge = false; end
  return ffi.C.AudioMessage(tocstring(msg), purge);
end
function IsAudioMessageDone(id)
  return ffi.C.IsAudioMessageDone(id);
end
function StopAudioMessage(id)
  ffi.C.StopAudioMessage(id);
end
function PreloadAudioMessage(msg)
  ffi.C.PreloadAudioMessage(tocstring(msg));
end
function PurgeAudioMessage(msg)
  ffi.C.PurgeAudioMessage(tocstring(msg));
end

function PreloadMusicMessage(msg)
  ffi.C.PreloadMusicMessage(tocstring(msg));
end
function PurgeMusicMessage(msg)
  ffi.C.PurgeMusicMessage(tocstring(msg));
end
function LoadJukeFile(msg)
  ffi.C.LoadJukeFile(tocstring(msg));
end
function SetMusicIntensity(intensity)
  ffi.C.SetMusicIntensity(intensity);
end
function GameObject.SetIndependence(independence)
  ffi.C.SetIndependence(self:GetHandle(), independence);
end
function IsInfo(odf)
  return ffi.C.IsInfo(tocstring(odf));
end
function StartCockpitTimer(time, warn, alert)
  if warn == nil then warn = LONG_MIN; end
  if alert == nil then alert = LONG_MIN; end
  ffi.C.StartCockpitTimer(time, warn, alert);
end
function StartCockpitTimerUp(time, warn, alert)
  if warn == nil then warn = LONG_MAX; end
  if alert == nil then alert = LONG_MAX; end
  ffi.C.StartCockpitTimerUp(time, warn, alert);
end
function StopCockpitTimer()
  ffi.C.StopCockpitTimer();
end
function HideCockpitTimer()
  ffi.C.HideCockpitTimer();
end
function GetCockpitTimer()
  return ffi.C.GetCockpitTimer();
end
function StartEarthQuake(magnitude)
  ffi.C.StartEarthQuake(magnitude);
end
function UpdateEarthQuake(magnitude)
  ffi.C.UpdateEarthQuake(magnitude);
end
function StopEarthQuake()
  ffi.C.StopEarthQuake();
end
--ConvertHandles
--Read
--Write
function GameObject:IsPerson(self)
  return ffi.C.IsPerson(self:GetHandle());
end
function GetCurWorld()
  return ffi.C.GetCurWorld();
end
function GetVarItemStr(VarItemName)
  return ffi.C.GetVarItemStr(tocstring(VarItemName));
end
function GetVarItemInt(VarItemName)
  return ffi.C.GetVarItemStr(GetVarItemInt(VarItemName));
end
function GetCVarItemInt(TeamNum, Idx)
  return ffi.C.GetCVarItemInt(TeamNum, Idx);
end
function GetCVarItemStr(TeamNum, Idx)
  return ffi.C.GetCVarItemStr(TeamNum, Idx);
end
function PreloadODF(cfg)
  ffi.C.PreloadODF(tocstring(cfg));
end
function TerrainFindFloor(x, z)
  return ffi.C.TerrainFindFloor(x, z);
end
function GameObject.AddPilot(self)
  ffi.C.AddPilotByHandle(self:GetHandle())
end
function PrintConsoleMessage(msg)
  ffi.C.PrintConsoleMessage(tocstring(msg));
end
function GetRandomFloat(range)
  return ffi.C.GetRandomFloat(range);
end
function GameObject.IsDeployed(self)
  return ffi.C.IsDeployed(self:GetHandle());
end
function GameObject.Deploy(self)
  ffi.C.Deploy(self:GetHandle());
end
function GameObject.IsSelected(self)
  return ffi.C.IsSelected(self:GetHandle());
end
function SetWeaponMask(self, mask)
  ffi.C.SetWeaponMask(self:GetHandle(), mask);
end
function GiveWeapon(self, weapon)
  ffi.C.GiveWeapon(self:GetHandle(), tocstring(weapon));
end
function GameObject.FireAt(self, target, doAim)
  if doAim == nil then doAim = false; end
  if type(target) == "table" and target.GetHandle ~= nil and type(target.GetHandle) == "function" then
    ffi.C.FireAt(self:GetHandle(), target:GetHandle(), doAim);
  else
    error("GameObject:FireAt target type invalid, received " .. type(target) .. ", expected GameObject");
  end
end
function GameObject.IsFollowing(self)
  return ffi.C.IsFollowing(self:GetHandle());
end
function GameObject.WhoFollowing(self)
  return GameObject.new(ffi.C.WhoFollowing(self:GetHandle()));
end
function SetUserTarget(h)
  ffi.C.SetUserTarget(h.GetHandle());
end
function GameObject.SetPerceivedTeam(self, team)
  ffi.C.SetPerceivedTeam(self:GetHandle(), team);
end
function GameObject.GetCurrentWho(self)
  return GameObject.new(ffi.C.GetCurrentWho(self:GetHandle()));
end
function GameObject.EjectPilot(self)
  ffi.C.EjectPilot(self:GetHandle());
end
function GameObject.HopOut(self)
  ffi.C.HopOut(self:GetHandle());
end
function GameObject.KillPilot(self)
  ffi.C.KillPilot(self:GetHandle());
end
function GameObject.RemovePilotAI(self)
  ffi.C.RemovePilotAI(self:GetHandle());
end
function GameObject.HoppedOutOf(self)
  return GameObject.new(ffi.C.HoppedOutOf(self:GetHandle()));
end
--
function CalcCRC(n)
  return ffi.C.CalcCRC(tocstring(n));
end
--
function GameObject.SetSkill(self, s)
  ffi.C.SetSkill(self:GetHandle(), s);
end
function SetPlan(cfg, team)
  if team == nil then team = -1; end
  ffi.C.SetSkill(tocstring(cfg), team);
end
function LogFloat(v)
  ffi.C.LogFloat(v);
end
function GetInstantMyForce()
  return ffi.C.GetInstantMyForce();
end
function GetInstantCompForce()
  return ffi.C.GetInstantCompForce();
end
function GetInstantDifficulty()
  return ffi.C.GetInstantDifficulty();
end
function GetInstantGoal()
  return ffi.C.GetInstantGoal();
end
function GetInstantType()
  return ffi.C.GetInstantType();
end
function GetInstantFlag()
  return ffi.C.GetInstantFlag();
end
function GetInstantMySide()
  return ffi.C.GetInstantMySide();
end
--StoppedPlayback
--Cam functions
function GameObject.SetAnimation(self, name, type)
  if type == nil then type = 0; end
  type = math.clamp(type,0,1);
  return ffi.C.SetAnimation(self:GetHandle(), tocstring(name), type);
end
function GameObject.GetAnimationFrame(self, name)
  return ffi.C.GetAnimationFrame(self:GetHandle(), tocstring(name));
end
function GameObject.StartAnimation(self)
  ffi.C.StartAnimation(self:GetHandle());
end
function GameObject.MaskEmitter(self, mask)
  ffi.C.MaskEmitter(self:GetHandle(), mask);
end
function GameObject.StartEmitter(self, slot)
  ffi.C.StartEmitter(self:GetHandle(), slot);
end
function GameObject.StopEmitter(self, slot)
  ffi.C.StopEmitter(self:GetHandle(), slot);
end
--SaveObjects
--LoadObjects
function IgnoreSync(on)
  ffi.C.IgnoreSync(on);
end
function IsRecording()
  return ffi.C.IsRecording();
end
function GameObject.SetObjectiveOn(self)
  ffi.C.SetObjectiveOn(self:GetHandle());
end
function GameObject.SetObjectiveOff(self)
  ffi.C.SetObjectiveOff(self:GetHandle());
end
function GameObject.SetObjectiveName(self, name)
  ffi.C.SetObjectiveName(self:GetHandle(), tocstring(name));
end
function ClearObjectives()
  ffi.C.ClearObjectives();
end
function AddObjective(name, color, showTime)
  local colorIn = color;
  if type(color) == "table" and color.ToColorLong ~= nil then
    colorIn = color:ToColorLong()
  end
  colorIn = bit.band(0xFFFFFFFF,colorIn)
  if showTime == nil then
    ffi.C.AddObjective(tocstring(name),colorIn, 8.0)
  else
    ffi.C.AddObjective(tocstring(name),colorIn,showTime)
  end
end
function GameObject.IsWithin(self, h2, distance)
  return ffi.C.IsWithin(self:GetHandle(), h2:GetHandle(), distance);
end
function GameObject.CountUnitsNearObject(self, dist, team, odf)
  return ffi.C.CountUnitsNearObject(self:GetHandle(), dist, team, tocstring(odf));
end
function GameObject.SetAvoidType(self, avoidType)
  ffi.C.SetAvoidType(self:GetHandle(), avoidType);
end
function GameObject.Annoy(self, target)
  ffi.C.Annoy(self:GetHandle(), target:GetHandle());
end
function GameObject.ClearThrust(self)
  ffi.C.ClearThrust(self:GetHandle());
end
function GameObject.SetVerbose(self, on)
  ffi.C.SetVerbose(self:GetHandle(), on);
end
function GameObject.ClearIdleAnims(self)
  ffi.C.ClearIdleAnims(self:GetHandle());
end
function GameObject.AddIdleAnim(self, name)
  ffi.C.AddIdleAnim(self:GetHandle(), tocstring(name));
end
function GameObject.IsIdle(self)
  return ffi.C.IsIdle(self:GetHandle());
end
function GameObject.CountThreats(self)
  local here = ffi.new("int[1]");
  local coming = ffi.new("int[1]");
  ffi.C.CountThreats(self:GetHandle(), here, coming);
  return tonumber(here[0]),tonumber(coming[0]);
end
--SpawnBirds
--SpawnBirds
--RemoveBirds
function SetColorFade(ratio, rate, color)
  local colorIn = color;
  if type(color) == "table" and color.ToColorLong ~= nil then
    colorIn = color:ToColorLong()
  end
  colorIn = bit.band(0xFFFFFFFF,colorIn)
  ffi.C.SetColorFade(ratio, rate, colorIn);
end
function StopCheats()
  ffi.C.StopCheats();
end
--CalcCliffs
function StartSoundEffect(file, h)
  return ffi.C.StartSoundEffect(tocstring(file), h:GetHandle());
end
function FindSoundEffect(file, h)
  return ffi.C.FindSoundEffect(tocstring(file), h:GetHandle());
end
function StopSoundEffect(sound)
  ffi.C.StopSoundEffect(sound);
end
--[[
const int DLL_TEAM_SLOT_RECYCLER = 1;
const int DLL_TEAM_SLOT_FACTORY = 2;
const int DLL_TEAM_SLOT_ARMORY = 3;
const int DLL_TEAM_SLOT_TRAINING = 4;
const int DLL_TEAM_SLOT_BOMBERBAY = 5;
const int DLL_TEAM_SLOT_SERVICE = 6;
const int DLL_TEAM_SLOT_TECHCENTER = 7;
const int DLL_TEAM_SLOT_COMMTOWER = 8;
const int DLL_TEAM_SLOT_BASE4 = 9; // Whatever's in base slot #4 (only w/ mods)
const int DLL_TEAM_SLOT_BASE5 = 10; // see above
const int DLL_TEAM_SLOT_BASE6 = 11; // see above
const int DLL_TEAM_SLOT_BASE7 = 12; // see above
const int DLL_TEAM_SLOT_BASE8 = 13; // see above
const int DLL_TEAM_SLOT_BASE9 = 14; // see above
]]
function GetObjectByTeamSlot(TeamNum, Slot)
  return GameObject.new(ffi.C.GetObjectByTeamSlot(TeamNum, Slot));
end
function portable_sin(ang)
  return ffi.C.portable_sin(ang);
end
function portable_cos(ang)
  return ffi.C.portable_cos(ang);
end
function portable_atan2(x, y)
  return ffi.C.portable_atan2(x, y);
end
function portable_asin(x)
  return ffi.C.portable_asin(x);
end
function portable_acos(x)
  return ffi.C.portable_acos(x);
end
function IsNetworkOn()
  return ffi.C.IsNetworkOn();
end
function ImServer()
  return ffi.C.ImServer();
end
function ImDedicatedServer()
  return ffi.C.ImDedicatedServer();
end
function IsTeamplayOn()
  return ffi.C.IsTeamplayOn();
end
function CountPlayers()
  return ffi.C.CountPlayers()
end
function GetPlayerHandle(team)
  return GameObject.new(ffi.C.GetPlayerHandle(team));
end
function GetRaceOfTeam(TeamNum)
  return ffi.C.GetRaceOfTeam(TeamNum);
end
function GameObject.IsPlayer(self)
  return ffi.C.IsPlayer(self:GetHandle());
end
function GameObject.GetPlayerName(self)
  return ffi.C.GetPlayerName(self:GetHandle());
end
function WhichTeamGroup(Team)
  local retVal = ffi.C.WhichTeamGroup(Team);
  if retVal == 0xA5A5A5A5 then return nil; end
  return retVal;
end
function CountAllies(Team)
  return ffi.C.CountAllies(Team);
end
function GetCommanderTeam(Team)
  return ffi.C.GetCommanderTeam(Team);
end
function GetFirstAlliedTeam(Team)
  return ffi.C.GetFirstAlliedTeam(Team);
end
function GetLastAlliedTeam(Team)
  return ffi.C.GetLastAlliedTeam(Team);
end
function GetAlliedTeamRange(Team)
  return GetFirstAlliedTeam(Team),GetLastAlliedTeam(Team);
end
function GetTeamplayRanges(Team)
  local DefenseTeamNum = ffi.new("int[0]");
  local OffenseMinTeamNum = ffi.new("int[0]");
  local OffenseMaxTeamNum = ffi.new("int[0]");
  return tonumber(DefenseTeamNum),tonumber(OffenseMinTeamNum),tonumber(OffenseMaxTeamNum);
end
function GameObject.SetRandomHeadingAngle(self)
  return ffi.C.SetRandomHeadingAngle(self:GetHandle());
end
function ClearTeamColors()
  ffi.C.ClearTeamColors();
end
function DefaultTeamColors()
  ffi.C.DefaultTeamColors();
end
function TeamplayTeamColors()
  ffi.C.TeamplayTeamColors();
end
function SetTeamColor(team, color)
  local r = bit.rshift(bit.band(0x00FF0000,color),16);
  local g = bit.rshift(bit.band(0x0000FF00,color),8);
  local b = bit.band(0x000000FF,color);
  ffi.C.SetTeamColor(team, r, g, b);
end
function ClearTeamColor(team)
  ffi.C.ClearTeamColor(team);
end
function GameObject.MakeInert(self)
  ffi.C.MakeInert(self:GetHandle());
end
function GetPositionNear(Pos, MinRadiusAway, MaxRadiusAway)
  return ffi.C.GetPositionNear(Pos, MinRadiusAway, MaxRadiusAway);
end
--GetPlayerODF
function GameObject.BuildEmptyCraftNear(self, ODF, Team, MinRadiusAway, MaxRadiusAway)
  return GameObject.new(ffi.C.BuildEmptyCraftNear(self, tocstring(ODF), Team, MinRadiusAway, MaxRadiusAway));
end
function SetCircularPos(CenterPos, Radius, Angle)
  local NewPos = Vector();
  ffi.C.SetCircularPos(CenterPos, Radius, Angle, NewPos);
  return NewPos;
end
function GetSafestSpawnpoint()
  return ffi.C.GetSafestSpawnpoint();
end
function GetSpawnpoint(TeamNum)
  return ffi.C.GetSpawnpoint(TeamNum);
end
function GetSpawnpointHandle(TeamNum)
  return GameObject.new(ffi.C.GetSpawnpointHandle(TeamNum));
end
function GetRandomSpawnpoint(TeamNum)
  if TeamNum == nil then TeamNum = -1; end
  return ffi.C.GetRandomSpawnpoint(TeamNum);
end
function SetTimerBox(message)
  ffi.C.SetTimerBox(tocstring(message));
end
function AddToMessagesBox(msg)
  ffi.C.AddToMessagesBox(tocstring(msg));
end
function GameObject.GetDeaths(self)
  return ffi.C.GetDeaths(self:GetHandle());
end
function GameObject.GetKills(self)
  return ffi.C.GetKills(self:GetHandle());
end
function GameObject.GetScore(self)
  return ffi.C.GetScore(self:GetHandle());
end
function GameObject.SetDeaths(self, NewValue)
  return ffi.C.SetDeaths(self:GetHandle(), NewValue);
end
function GameObject.SetKills(self, NewValue)
  return ffi.C.SetKills(self:GetHandle(), NewValue);
end
function GameObject.SetScore(self, NewValue)
  return ffi.C.SetScore(self:GetHandle(), NewValue);
end
function GameObject.AddDeaths(self, delta)
  ffi.C.AddDeaths(self:GetHandle(), delta)
end
function GameObject.AddKills(self, delta)
  ffi.C.AddKills(self:GetHandle(), delta)
end
function AddScore(team, delta)
  ffi.C.AddScore(team, delta)
end
function GameObject.SetAsUser(self, Team)
  ffi.C.SetAsUser(self:GetHandle(), Team);
end
function GameObject.SetNoScrapFlagByHandle(self)
  ffi.C.SetNoScrapFlagByHandle(self:GetHandle());
end
function GameObject.ClearNoScrapFlagByHandle(self)
  ffi.C.ClearNoScrapFlagByHandle(self:GetHandle());
end
function GetLocalPlayerTeamNumber()
  return ffi.C.GetLocalPlayerTeamNumber();
end
function GetLocalPlayerDPID()
  return ffi.C.GetLocalPlayerDPID();
end
function GameObject.FlagStealBy(self, holder)
  ffi.C.FlagSteal(self:GetHandle(), holder.GetHandle());
end
function GameObject.FlagRecoverBy(self, holder)
  ffi.C.FlagRecover(self:GetHandle(), holder.GetHandle());
end
function GameObject.FlagScoreBy(self, holder)
  ffi.C.FlagScore(self:GetHandle(), holder.GetHandle());
end
function GameObject.FlagSteal(self, holder)
  ffi.C.FlagSteal(holder.GetHandle(), self:GetHandle());
end
function GameObject.FlagRecover(self, holder)
  ffi.C.FlagRecover(holder.GetHandle(), self:GetHandle());
end
function GameObject.FlagScore(self, holder)
  ffi.C.FlagScore(holder.GetHandle(), self:GetHandle());
end
function GameObject.MoneyScore(self, ammount)
  ffi.C.MoneyScore(ammount, self:GetHandle());
end
function NoteGameoverByTimelimit()
  ffi.C.NoteGameoverByTimelimit()
end
function NoteGameoverByKillLimit(h)
  ffi.C.NoteGameoverByKillLimit(h.handle)
end
function NoteGameoverByScore(h)
  ffi.C.NoteGameoverByScore(h.handle)
end
function NoteGameoverByLastWithBase(h)
  ffi.C.NoteGameoverByLastWithBase(h.handle)
end
function NoteGameoverByLastTeamWithBase(Teamgroup)
  ffi.C.NoteGameoverByLastTeamWithBase(Teamgroup)
end
function NoteGameoverByNoBases()
  ffi.C.NoteGameoverByNoBases()
end
function DoGameover(SecondsFromNow)
  ffi.C.DoGameover(SecondsFromNow)
end
function SetMPTeamRace(WhichTeamGroup, RaceIdentifier)
  ffi.C.SetMPTeamRace(WhichTeamGroup, RaceIdentifier);
end
function GetUserTarget(TeamNum)
  return GameObject.new(ffi.C.GameUserTarget(TeamNum));
end
function GameObject.GetTarget(self)
  return GameObject.new(GetTarget(self:GetHandle()));
end
function IFace_ConsoleCmd(pStr, bSquelchOutput)
  if bSquelchOutput == nil then bSquelchOutput = true; end
  ffi.C.IFace_ConsoleCmd(tocstring(pStr));
end
function AddToMessagesBox2(message, color)
  local colorIn = color;
  if type(color) == "table" and color.ToColorLong ~= nil then
    colorIn = color:ToColorLong()
  end
  colorIn = bit.band(0xFFFFFFFF,colorIn)
  ffi.C.AddToMessagesBox2(tocstring(message), colorIn)
end
function Network_SetString(name, value)
  ffi.C.Network_SetString(tocstring(name), tocstring(value));
end
function Network_SetInteger(name, value)
  ffi.C.Network_SetInteger(tocstring(name), value);
end
function GetFirstEmptyGroup(t)
  return ffi.C.GetFirstEmptyGroup(t);
end
function GameObject.GetVelocity(self)
  return ffi.C.GetVelocity(self:GetHandle());
end
--GetObjInfo(Handle h, ObjectInfoType type, char pBuffer[64]);
function DoesODFExist(odf)
  return ffi.C.DoesODFExist(tocstring(odf));
end
function GameObject.IsAlive2(self)
  return ffi.C.IsAlive2(self:GetHandle());
end
function GameObject.IsFlying2(self)
  return ffi.C.IsFlying2(self:GetHandle());
end
function GameObject.IsAliveAndPilot2(self)
  return ffi.C.IsAliveAndPilot2(self:GetHandle());
end
function TranslateString2(Src, size)
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
function GameObject.GetScavengerCurScrap(self)
  local retVal = ffi.C.GetScavengerCurScrap(self:GetHandle());
  if retVal == -1 then
    return nil;
  end
  return GameObject.new(retVal);
end
function GameObject.GetScavengerMaxScrap(self)
  local retVal = ffi.C.GetScavengerMaxScrap(self:GetHandle());
  if retVal == -1 then
    return nil;
  end
  return GameObject.new(retVal);
end
function GameObject.SetScavengerCurScrap(self,aNewScrap)
  ffi.C.SetScavengerCurScrap(self:GetHandle(),aNewScrap);
end
function GameObject.SetScavengerMaxScrap(self,aNewScrap)
  ffi.C.SetScavengerMaxScrap(self:GetHandle(),aNewScrap);
end
function GameObject.DamageF(self, amt)
  ffi.C.DamageF(self:GetHandle(), amt);
end
function GameObject.SelfDamage(self, amt)
  ffi.C.SelfDamage(self:GetHandle(), amt);
end
function WantBotKillMessages()
  ffi.C.WantBotKillMessages();
end
function EnableHighTPS(newRate)
  ffi.C.EnableHighTPS(newRate);
end
function GetLocalUserInspectHandle()
  return GameObject.new(ffi.C.GetLocalUserInspectHandle());
end
function GetLocalUserSelectHandle()
  return GameObject.new(ffi.C.GetLocalUserSelectHandle());
end
function GameObject.ResetTeamSlot(self)
  ffi.C.ResetTeamSlot(self:GetHandle());
end
function GameObject.GetCategoryTypeOverride(self)
  local retVal = ffi.C.GetCategoryTypeOverride(self:GetHandle());
  if retVal == -1 then return nil; end
  return retVal;
end
function GameObject.GetCategoryType(self)
  local retVal = ffi.C.GetCategoryType(self:GetHandle());
  if retVal == -1 then return nil; end
  return retVal;
end
--[[DLLEXPORT int DLLAPI GetODFHexInt(const char* file, const char* block, const char* name, int* value = NULL, int defval = 0);
DLLEXPORT int DLLAPI GetODFInt(const char* file, const char* block, const char* name, int* value = NULL, int defval = 0);
DLLEXPORT int DLLAPI GetODFLong(const char* file, const char* block, const char* name, long* value = NULL, long defval = 0);
DLLEXPORT int DLLAPI GetODFFloat(const char* file, const char* block, const char* name, float* value = NULL, float defval = 0.0f);
DLLEXPORT int DLLAPI GetODFDouble(const char* file, const char* block, const char* name, double* value = NULL, double defval = 0.0);
DLLEXPORT int DLLAPI GetODFChar(const char* file, const char* block, const char* name, char* value = NULL, char defval = '\0');
DLLEXPORT int DLLAPI GetODFBool(const char* file, const char* block, const char* name, bool* value = NULL, bool defval = false);
DLLEXPORT int DLLAPI GetODFString(const char* file, const char* block, const char* name, size_t ValueLen = 0, char* value = NULL, const char* defval = NULL);
DLLEXPORT int DLLAPI GetODFColor(const char* file, const char* block, const char* name, DWORD* value = NULL, DWORD defval = 0);
DLLEXPORT int DLLAPI GetODFVector(const char* file, const char* block, const char* name, Vector* value = NULL, Vector defval = Vector(0.0f, 0.0f, 0.0f));
DLLEXPORT bool DLLAPI OpenODF(char *name);
DLLEXPORT bool DLLAPI CloseODF(char *name);]]
function NoteGameoverWithCustomMessage(pString)
  ffi.C.NoteGameoverWithCustomMessage(tocstring(pString));
end
function GameObject.SetBestGroup(self)
  return ffi.C.SetBestGroup(self:GetHandle());
end
function GetGroup(team, group, type)
  local pBuffer = ffi.new("char[64]");
  ffi.C.GetGroup(team, group, type, pBuffer);
  return tostring(pBuffer);
end
function GetGroupCount(team, group)
  return ffi.C.GetGroupCount(team, group);
end
function GameObject.SetLifespan(self, timout)
  if timeout == nil then timeout = -1; end
  ffi.C.SetLifespan(self:GetHandle(), timeout);
end
function DoesFileExist(filename)
  return ffi.C.DoesFileExist(tocstring(filename));
end
function LoadFile(filename)
  local filenameC = tocstring(LoadFile);
  local bufSize = ffi.new("unsigned int[1]");
  local success = ffi.C.LoadFile(filename, nil, bufSize);
  if not success then return nil; end
  local pData = ffi.new("char[?]",tonumber(bufSize) + 1);
  success = ffi.C.LoadFile(filename, pData, bufSize);
  if success then
    return tostring(pData);
  end
  return nil;
end
function StartAudio3D(name, gameobject, category, loop, stopLast)
  if category == nil then category = DLLAudioCategory.AUDIO_CAT_UNKNOWN; end
  if loop == nil then loop = false; end
  if stopLast == nil then stopLast = false; end
  return ffi.C.StartAudio3D(tocstring(name), gameobject.GetHandle(), category, loop, stopLast);
end
function StartAudio3DV(name, pos, category, loop)
  if category == nil then category = DLLAudioCategory.AUDIO_CAT_UNKNOWN; end
  if loop == nil then loop = false; end
  return ffi.C.StartAudio3DV(tocstring(name), pos.x, post.y, pos.z, category, loop);
end
function StartAudio2D(name, volume, pan, rate, loop, isMusic)
  if loop == nil then loop = false; end
  if isMusic == nil then isMusic = false; end
  return ffi.C.StartAudio2D(tocstring(name), volume, pan, rate, loop, isMusic);
end
function IsAudioPlaying(h)
  return ffi.C.IsAudioPlaying(h);
end
function StopAudio(h)
  ffi.C.StopAudio(h);
end
function PauseAudio(h)
  ffi.C.PauseAudio(h);
end
function ResumeAudio(h)
  ffi.C.ResumeAudio(h);
end
function SetVolume(h, vol, adjustByVolumes)
  if adjustByVolumes == nil then adjustByVolumes = true; end
  ffi.C.SetVolume(h, vol, adjustByVolumes);
end
function SetPan(h, pan)
  ffi.C.SetPan(h, pan);
end
function SetRate(h, rate)
  ffi.C.SetRate(h, rate);
end
function GetAudioFileDuration(h)
  return ffi.C.GetAudioFileDuration(h);
end
function IsPlayingLooped(h)
  return ffi.C.IsPlayingLooped(h);
end
function GetNearestEnemy(h, ignorePilots, ignoreScavs, maxDist)
  if maxDist == nil then maxDist = 450.0; end
  return GameObject.new(ffi.C.GetNearestEnemy(h.handle, ignorePilots, ignoreScavs, maxDist));
end
function GetNearestPowerup(h, ignoreSpawnpoints, maxDist)
  if maxDist == nil then maxDist = 450.0; end
  return GameObject.new(ffi.C.GetNearestPowerup(h.handle, ignoreSpawnpoints, maxDist));
end
function GetNearestPerson(h, skipFriendlies, maxDist)
  if maxDist == nil then maxDist = 450.0; end
  return GameObject.new(ffi.C.GetNearestPerson(h.handle, skipFriendlies, maxDist));
end
function GameObject.SetCommand(self, cmd, priority, who, loc, param)
  if param == nil then param = 0; end
  if type(pos) == "cdata" and ffi.istype("Vector", pos) then
    ffi.C.SetCommandV(self:GetHandle(), cmd, priority, who.GetHandle(), where, param);
  else
    if priority == nil then priority = 0; end
    if who == nil then who = 0; end
    if isgameobject(who) then who = who.GetHandle(); end
    if path ~= nil then path = tocstring(path); end
    ffi.C.SetCommandP(self:GetHandle(), cmd, priority, who, path, param);
  end
end
function SetGravity(gravity)
  if gravity == nil then gravity = 12.5; end
  ffi.C.SetGravity(gravity);
end
function SetAutoGroupUnits(autoGroup)
  if autoGroup == nil then autoGroup = true; end
  ffi.C.SetAutoGroupUnits(autoGroup);
end
function GameObject.KickPlayer(self, pExplanationStr, banAlso)
  if banAlso == nil then banAlso = false; end
  ffi.C.KickPlayer(self:GetHandle(), tocstring(pExplanationStr));
end
function TerrainIsWater(posORx, z)
  if z == nil then
    ffa.C.TerrainIsWaterV(posOrx);
  else
    ffa.C.TerrainIsWater(posOrx, z);
  end
end
function TerrainGetHeightAndNormal(pos, useWater)
  local outHeight = ffi.new("float[1]");
  local outNormal = Vector();--ffi.new("Vector[1]");
  local success = ffi.C.TerrainGetHeightAndNormal(pos, outHeight, outNormal, useWater);
  if success ~= nil then return nil; end
  return tonumber(outHeight), outNormal;
end
function GetOutputPath()
  local bufSize = ffi.new("unsigned int[1]");
  local success = ffi.C.GetOutputPath(bufSize, nil);
  if not success then return nil; end
  local pData = ffi.new("char[?]",tonumber(bufSize) + 1);
  success = ffi.C.GetOutputPath(bufSize, pData);
  if not success then return nil; end
  return tostring(pData);
end
function GetPathPoints(path)
  local pathC = tocstring(path);
  local bufSize = ffi.new("int[0]");
  local sucess = ffi.C.GetPathPoints(pathC, bufSize, nil);
  if not success then return nil; end
  local pData = ffi.new("float[?]", tonumber(bufSize) * 2);
  sucess = ffi.C.GetPathPoints(pathC, bufSize, pData);
  if not success then return nil; end
  local paths = {};
  for itr = 1,tonumber(bufSize) + 1,2 do
    paths.insert(VECTOR_2D(pData[itr+1],pData[iter+2]));
  end
  return paths;
end
function GameObject.GetOwner(self)
  return GameObject.new(ffi.C.GetOwner(self:GetHandle()));
end
function GameObject.SetTarget(self, target)
  return GameObject.new(ffi.C.SetTarget(self:GetHandle(), target));
end
function GameObject.SetOwner(self, owner)
  return GameObject.new(ffi.C.SetOwner(self:GetHandle(), owner));
end
function GameObject.SetPilotClass(self, odf)
  ffi.C.SetPilotClass(self:GetHandle(), tocstring(odf));
end
function AllowRandomTracks(allow)
  if allow == nil then allow = true end
  ffi.C.AllowRandomTracks(allow)
end
function SetFFATeamColors(type)
  ffi.C.SetFFATeamColors(type);
end
function SetTeamStratColors(type)
  ffi.C.SetTeamStratColors(type);
end
function GetFFATeamColor(type, team)
  local r = ffi.new("int[1]");
  local g = ffi.new("int[1]");
  local b = ffi.new("int[1]");
  ffi.C.GetFFATeamColor(type, team, r, g, b);
  return Color.new(tonumber(r),tonumber(g),tonumber(b));
end
function GetTeamStratColor(type, team)
  local r = ffi.new("int[1]");
  local g = ffi.new("int[1]");
  local b = ffi.new("int[1]");
  ffi.C.GetTeamStratColor(type, team, r, g, b);
  return Color.new(tonumber(r),tonumber(g),tonumber(b));
end
function SwapTeamStratColors()
  ffi.C.SwapTeamStratColors();
end
function GetTeamColorsAreFFA()
  return ffi.C.GetTeamColorsAreFFA();
end
function SetTeamColors(type)
  return ffi.C.SetTeamColors(type);
end
function AddPower(team, delta)
  return ffi.C.AddPower(team, delta)
end
function SetPower(team, delta)
  return ffi.C.SetPower(team, delta)
end
function GetPower(team)
  return ffi.C.GetPower(team)
end
function GetMaxPower(team)
  return ffi.C.GetMaxPower(team)
end
function AddMaxScrap(team, ammount)
  ffi.C.AddMaxScrap(team, ammount)
end
function AddMaxPower(team, ammount)
  ffi.C.AddMaxPower(team, ammount)
end
function SetMaxScrap(team, ammount)
  ffi.C.SetMaxScrap(team, ammount)
end
function SetMaxPower(team, ammount)
  ffi.C.SetMaxPower(team, ammount)
end
function GetTeamStratIndividualColor(type, team)
  local r = ffi.new("int[1]");
  local g = ffi.new("int[1]");
  local b = ffi.new("int[1]");
  ffi.C.GetTeamStratIndividualColor(type, team, r, g, b);
  return Color.new(tonumber(r),tonumber(g),tonumber(b));
end
function GetMapTRNFilename()
  return tostring(ffi.C.GetMapTRNFilename());
end
function IsTeamAllied(t1, t2)
  return ffi.C.IsTeamAllied(t1, t2);
end
function GameObject.IsNotDeadAndPilot2(self)
  return ffi.C.IsNotDeadAndPilot2(self:GetHandle());
end
function GameObject.GetLabel(self)
  return ffi.C.GetLabel(self:GetHandle());
end
function GameObject.SetLabel(self, pLabel)
  ffi.C.SetLabel(self:GetHandle(), tocstring(pLabel));
end
function GameObject.GetTap(self, index)
  return GameObject.new(ffi.C.GetTap(self:GetHandle(), index));
end
function GameObject.SetTap(self, index, tapObjectHandle)
  ffi.C.SetTap(self:GetHandle(), index, tapObjectHandle.GetHandle());
end
function GameObject.GetCurLocalAmmo(self, weaponIndex)
  return ffi.C.GetCurLocalAmmo(self:GetHandle(), weaponIndex)
end
function GameObject.AddLocalAmmo(self, ammount, weaponIndex)
  ffi.C.AddLocalAmmo(self:GetHandle(), ammount, weaponIndex)
end
function GameObject.GetMaxLocalAmmo(self, weaponIndex)
  return ffi.C.GetMaxLocalAmmo(self:GetHandle(), weaponIndex)
end
function GameObject.SetCurLocalAmmo(self, v, weaponIndex)
  ffi.C.SetCurLocalAmmo(self:GetHandle(), v, weaponIndex)
end


-- Color
local Color = {}
Color.__index = Color

function Color.new(r,g,b,a)
  local self = setmetatable({}, Color)
  if r ~= nil and g == nil and b == nil and a == nil then
    self.a = bit.rshift(bit.band(0xFF000000,r),24)
    self.r = bit.rshift(bit.band(0x00FF0000,r),16)
    self.g = bit.rshift(bit.band(0x0000FF00,r),8)
    self.b = bit.band(0x000000FF,r)
  else
    self.r = math.clamp(r,0,255)
    self.g = math.clamp(g,0,255)
    self.b = math.clamp(b,0,255)
    if a == nill then
      self.a = 255
    else
      self.a = math.clamp(a,0,255)
    end
  end
  return self
end

function Color.ToColorLong(self)
  return bit.bor(bit.lshift(self.a, 24), bit.lshift(self.r, 16), bit.lshift(self.g, 8), self.b)
end

function Color.FromRGB(r,g,b)
  return bit.bor(0xFF000000, bit.lshift(r, 16), bit.lshift(g, 8), b)
end

function Color.FromRGBA(r,g,b,a)
  return bit.bor(bit.lshift(a, 24), bit.lshift(r, 16), bit.lshift(g, 8), b)
end

Color.WHITE     = 0xFFFFFFFF
Color.RED       = 0xFFFF0000
Color.GREEN     = 0xFF00FF00
Color.BLUE      = 0xFF0000FF
Color.TURQUOISE = 0xFF00FFFF
Color.VIOLET    = 0xFFFF00FF
Color.YELLOW    = 0xFFFFFF00
-- /Color















function Ally(t1,t2)
  ffi.C.Ally(t1, t2)
end





local VECTOR_2D
VECTOR_2D = ffi.metatype("VECTOR_2D", {
  __add = function(a, b) return VECTOR_2D(a.x+b.x, a.z+b.z) end,
})

local Vector
Vector = ffi.metatype("Vector", {
  __add = function(a, b, c) return Vector(a.x+b.x, a.y+b.y, a.z+b.z) end,
})

local Matrix
Matrix = ffi.metatype("Matrix", {})




function InitialSetup()
	io.write("InitialSetup, from ",_VERSION,"!\n")
  
  --for k,v in pairs(getmetatable(bzone)) do
  --for k,v in pairs(getmetatable(ffi.C)) do
  --  ScriptUtils.PrintConsoleMessage("", k, ", ", v)
  --end
  
  --PrintConsoleMessage("InitialSetup, from " .. _VERSION .. "!\n")
  
  --local v0 = Vector()
  --local v1 = Vector(5,5,5)
  --local v2 = Vector(1,2,3)
  --local v3 = v1 + v2
  
  --PrintConsoleMessage("v0: " .. v0.x .. ", " .. v0.y .. ", " .. v0.z)
  --PrintConsoleMessage("v1: " .. v1.x .. ", " .. v1.y .. ", " .. v1.z)
  --PrintConsoleMessage("v2: " .. v2.x .. ", " .. v2.y .. ", " .. v2.z)
  --PrintConsoleMessage("v1 + v2: " .. v3.x .. ", " .. v3.y .. ", " .. v3.z)
  
  --AddObjective("Color Number",0xFF11CCFF)
  --AddObjective("Color Class",Color.new(127,255,127))
  --AddObjective("Color Class",Color.FromRGB(127,255,127))
  --AddObjective("Color Class",Color.FromRGBA(127,255,127,128))
  --AddObjective("Color.WHITE",Color.WHITE)
  --AddObjective("Color.RED",Color.RED)
  --AddObjective("Color.GREEN",Color.GREEN)
  --AddObjective("Color.BLUE",Color.BLUE)
  --AddObjective("Color.TURQUOISE",Color.TURQUOISE)
  --AddObjective("Color.VIOLET",Color.VIOLET)
  --AddObjective("Color.YELLOW",Color.YELLOW)
  
  --AddToMessagesBox("test")
  --AddToMessagesBox2("test2",Color.YELLOW)
  
  --AllowRandomTracks()
  --AllowRandomTracks(false)
  --AllowRandomTracks(true)
  
  local friendTank = BuildObject("ivtank",1,Vector());
  local enemyTank = BuildObject("ivtank",2,Vector(100,100,100));
  enemyTank:Attack(friendTank);
  local here;
  local coming;
  here,coming = friendTank:CountThreats();
  PrintConsoleMessage("Here: " .. here .. " Coming: " .. coming);
end

function Update()
	io.write("Update, from ",_VERSION,"!\n")
  --PrintConsoleMessage("Update, from " .. _VERSION .. "!\n");
end

function Save(misnSave)
	io.write("Save("..tostring(misnSave).."), from ",_VERSION,"!\n")
	return true
end

function Load(misnSave)
	io.write("Load("..tostring(misnSave).."), from ",_VERSION,"!\n")
	return true
end

function PostLoad(misnSave)
	io.write("PostLoad("..tostring(misnSave).."), from ",_VERSION,"!\n")
	return true
end

function AddObject(h)
	io.write("AddObject("..tostring(h).."), from ",_VERSION,"!\n")
  --local gameObject = GameObject.new(h)
  --gameObject:AddIdleAnim("test");
end

function DeleteObject(h)
	io.write("DeleteObject("..tostring(h).."), from ",_VERSION,"!\n")
end

function PostRun()
	io.write("PostRun, from ",_VERSION,"!\n")
end

function AddPlayer(id, team, shouldCreateThem)
	io.write("AddPlayer("..tostring(id)..", "..tostring(team)..", "..tostring(shouldCreateThem).."), from ",_VERSION,"!\n")
	return true
end

function DeletePlayer(id)
	io.write("DeletePlayer("..tostring(id).."), from ",_VERSION,"!\n")
end

function PlayerEjected(deadObjectHandle)
	io.write("PlayerEjected("..tostring(deadObjectHandle).."), from ",_VERSION,"!\n")
	return 0
end

function ObjectKilled(deadObjectHandle, killersHandle)
	io.write("ObjectKilled("..tostring(deadObjectHandle)..", "..tostring(killersHandle).."), from ",_VERSION,"!\n")
	return 1
end

function ObjectSniped(deadObjectHandle, killersHandle)
	io.write("ObjectSniped("..tostring(deadObjectHandle)..", "..tostring(killersHandle).."), from ",_VERSION,"!\n")
	return 2
end

function GetNextRandomVehicleODF(team)
	io.write("GetNextRandomVehicleODF("..tostring(team).."), from ",_VERSION,"!\n")
	return "someVehicle"
end

function SetWorld(nextWorld)
	io.write("SetWorld("..tostring(nextWorld).."), from ",_VERSION,"!\n")
end

function ProcessCommand(crc)
	io.write("ProcessCommand("..tostring(crc).."), from ",_VERSION,"!\n")
end

function SetRandomSeed(seed)
	io.write("SetRandomSeed("..tostring(seed).."), from ",_VERSION,"!\n")
end