#include "DLLBase.h"

// Globals, shared code

static int CurrentWorld;
static DLLBase *Missions[3];
DLLBase *Mission = NULL;

// ------------------- Stub behaviors to be overwridden by virtual fcn -------------------- 

void DLLBase::InitialSetup(void)
{
}

bool DLLBase::Save(bool missionSave)
{
	return TRUE;
}

bool DLLBase::Load(bool missionSave)
{
	return TRUE;
}

bool DLLBase::PostLoad(bool missionSave)
{
	return TRUE;
}

void DLLBase::AddObject(Handle h)
{
}

void DLLBase::DeleteObject(Handle h)
{
}

void DLLBase::Execute(void)
{
}

void DLLBase::PostRun(void)
{
}

void DLLBase::DeletePlayer(DPID id)
{
}

void DLLBase::SetWorld(int nextWorld)
{
}

void DLLBase::ProcessCommand(unsigned long crc)
{
}

void DLLBase::SetRandomSeed(unsigned long seed)
{
}

bool DLLBase::AddPlayer(DPID id, int Team, bool ShouldCreateThem)
{
	_ASSERTE(FALSE);
	return false;
}

EjectKillRetCodes DLLBase::PlayerEjected(Handle DeadObjectHandle)
{
	return DoEjectPilot;
}

EjectKillRetCodes DLLBase::ObjectKilled(int DeadObjectHandle, int KillersHandle)
{
	if(!IsPlayer(DeadObjectHandle)) // J.Random AI-controlled Object is toast
		return DLLHandled;
	else // Player dead
		return DoEjectPilot;
}

EjectKillRetCodes DLLBase::ObjectSniped(int DeadObjectHandle, int KillersHandle)
{
	if(!IsPlayer(DeadObjectHandle)) // J.Random AI-controlled Object is toast
		return DLLHandled;
	else // Player dead
		return DoRespawnSafest;
}

char *DLLBase::GetNextRandomVehicleODF(int Team)
{
	_ASSERTE(FALSE);
	return "player";
}


// Notification to the DLL: a sniper shell has hit a piloted
// craft. The exe passes the current world, shooters handle, victim
// handle, and the ODF string of the ordnance involved in the
// snipe. Returns a code detailing what to do.
//
// !! Note : If DLLs want to do any actions to the world based on this
// PreSnipe callback, they should (1) Ensure curWorld == 0 (lockstep)
// -- do NOTHING if curWorld is != 0, and (2) probably queue up an
// action to do in the next Execute() call.
PreSnipeReturnCodes DLLBase::PreSnipe(const int curWorld, Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	// Default handler for this function: behave like 1.0-1.3.6.4.  If
	// a derived class overrides this function, it can implement
	// custom logic.
	return PRESNIPE_KILLPILOT;
}


// Notification to the DLL: ordnance has hit something. This passes
// the shooters handle, victim handle, ordnance's team, and the ODF
// string of the ordnance involved. Note that the shooters handle
// *might* be invalid -- consider a mortar (or other slow ordnance)
// launched by a craft just before that craft explodes. By the time
// that mortar lands, the shooter is dead.  Thus the ordnance's team
// is also passed as a possible fallback for identification.
//
// This call may not quite catch all ordnance hit events yet; anything
// derived from a Bullet will probably work well. Anything else may or
// may not work.  For sniper shells hitting targets, they should
// always generate a PreOrdnanceHitCallback (in the lockstep world),
// followed by (an optional) PreSnipeCallback if the sniper shell hit
// a snipable cockpit, etc. In visual worlds, sniper shells will only
// generate the PreSnipeCallback.
void DLLBase::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	// Do nothing. Derived classes might want to override this
	// function and do something useful.
}


// Notification to the DLL: called when a pilot tries to enter an
// empty craft, and all other checks (i.e. craft is empty, masks
// match, etc) have passed. DLLs can prevent that pilot from
// entering the craft if desired.
//
// !! Note : If DLLs want to do any actions to the world based on this
// PreSnipe callback, they should (1) Ensure curWorld == 0 (lockstep)
// -- do NOTHING if curWorld is != 0, and (2) probably queue up an
// action to do in the next Execute() call.
PreGetInReturnCodes DLLBase::PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle)
{
	// Default handler for this function: behave like 1.0-1.3.6.4.  If
	// a derived class overrides this function, it can implement
	// custom logic.
	return PREGETIN_ALLOW;
}


// Notification to the DLL: called when a pilot/craft is about to
// pickup a power up, and all other checks have passed. DLLs can
// prevent that pick up if desired.
//
// !! Note : If DLLs want to do any actions to the world based on this
// PreSnipe callback, they should (1) Ensure curWorld == 0 (lockstep)
// -- do NOTHING if curWorld is != 0, and (2) probably queue up an
// action to do in the next Execute() call.
PrePickupPowerupReturnCodes DLLBase::PrePickupPowerup(const int curWorld, Handle me, Handle powerupHandle)
{
	// Default handler for this function: behave like 1.0-1.3.6.4.  If
	// a derived class overrides this function, it can implement
	// custom logic.
	return PREPICKUPPOWERUP_ALLOW;
}

// Notification to the DLL: called when a pilot/craft has changed targets
void DLLBase::PostTargetChangedCallback(Handle craft, Handle previousTarget, Handle currentTarget)
{
	// Default handler for this function: no-operation.  If a derived
	// class overrides this function, it can implement custom logic.
}


// Stub callback for PreSnipeCallback to turn it into a virtual
// function call via the DLLBase class
PreSnipeReturnCodes DLLAPI DLLBasePreSnipeCallback(const int curWorld, Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	if(Mission)
	{
		return Mission->PreSnipe(curWorld, shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);
	}
	else
	{
		// Things are in a bad place here, we should have the Mission
		// global filled out. Don't crash here, but things will
		// probably crash soonish :(
		return PRESNIPE_KILLPILOT;
	}
}


// Stub callback for PreOrdnanceHitCallback to turn it into a virtual
// function call via the DLLBase class
void DLLAPI DLLBasePreOrdnanceHitCallback(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	if(Mission)
	{
		Mission->PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);
	}
	else
	{
		// Things are in a bad place here, we should have the Mission
		// global filled out. Don't crash here, but things will
		// probably crash soonish :(
	}
}

// Stub callback for PreGetInCallback to turn it into a virtual
// function call via the DLLBase class
PreGetInReturnCodes DLLAPI DLLBasePreGetInCallback(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle)
{
	if(Mission)
	{
		return Mission->PreGetIn(curWorld, pilotHandle, emptyCraftHandle);
	}
	else
	{
		// Things are in a bad place here, we should have the Mission
		// global filled out. Don't crash here, but things will
		// probably crash soonish :(
		return PREGETIN_ALLOW;
	}
}

// Stub callback for PrePickupPowerup to turn it into a virtual
// function call via the DLLBase class
PrePickupPowerupReturnCodes DLLAPI DLLBasePrePickupPowerupCallback(const int curWorld, Handle me, Handle powerupHandle)
{
	if(Mission)
	{
		return Mission->PrePickupPowerup(curWorld, me, powerupHandle);
	}
	else
	{
		// Things are in a bad place here, we should have the Mission
		// global filled out. Don't crash here, but things will
		// probably crash soonish :(
		return PREPICKUPPOWERUP_ALLOW;
	}
}

// Stub callback for PostTargetChangedCallback to turn it into a virtual
// function call via the DLLBase class
void DLLAPI DLLBasePostTargetChangedCallback(Handle craft, Handle previousTarget, Handle currentTarget)
{
	if(Mission)
	{
		return Mission->PostTargetChangedCallback(craft, previousTarget, currentTarget);
	}
	else
	{
		// Things are in a bad place here, we should have the Mission
		// global filled out. Don't crash here, but things will
		// probably crash soonish :(
	}
}


// ------------------------ Interface to C++ Mission class ------------------------ 

void DLLAPI InitialSetup(void)
{
	if (Mission == NULL)
		Mission = BuildMission();
	Mission->InitialSetup();

	// Subscribe to the 1.3.6.5+ callbacks so that all derived DLLs get
	// it for free.
	SetPreSnipeCallback(DLLBasePreSnipeCallback);
	SetPreOrdnanceHitCallback(DLLBasePreOrdnanceHitCallback);
	SetPreGetInCallback(DLLBasePreGetInCallback);
	SetPrePickupPowerupCallback(DLLBasePrePickupPowerupCallback);
	SetPostTargetChangedCallback(DLLBasePostTargetChangedCallback);
}

bool DLLAPI Save(bool misnSave)
{
	return Mission->Save(misnSave);
}

bool DLLAPI Load(bool misnSave)
{
	if (Mission == NULL)
		Mission = BuildMission();
	return Mission->Load(misnSave);
}

bool DLLAPI PostLoad(bool misnSave)
{
	return Mission->PostLoad(misnSave);
}

void DLLAPI AddObject(Handle h)
{
	Mission->AddObject(h);
}

void DLLAPI DeleteObject(Handle h)
{
	Mission->DeleteObject(h);
}

void DLLAPI Update(void)
{
	Mission->Execute();
}

void DLLAPI PostRun(void)
{
	Mission->PostRun();
	delete Mission;
}

bool DLLAPI AddPlayer(DPID id, int Team, bool ShouldCreateThem)
{
	return Mission->AddPlayer(id, Team, ShouldCreateThem);
}

void DLLAPI DeletePlayer(DPID id)
{
	Mission->DeletePlayer(id);
}

EjectKillRetCodes DLLAPI PlayerEjected(Handle DeadObjectHandle)
{
	return Mission->PlayerEjected(DeadObjectHandle);
}

EjectKillRetCodes DLLAPI ObjectKilled(int DeadObjectHandle,int KillersHandle)
{
	return Mission->ObjectKilled(DeadObjectHandle, KillersHandle);
}

EjectKillRetCodes DLLAPI ObjectSniped(int DeadObjectHandle,int KillersHandle)
{
	return Mission->ObjectSniped(DeadObjectHandle, KillersHandle);
}

char * DLLAPI GetNextRandomVehicleODF(int Team)
{
	return Mission->GetNextRandomVehicleODF(Team);
}

void DLLAPI SetWorld(int nextWorld)
{
	if (CurrentWorld >= 0)
		Missions[CurrentWorld] = Mission;
	Mission = Missions[nextWorld];
	CurrentWorld = nextWorld;

	// Hack? If mission for this world doesn't yet exist, make sure it
	// does now before calling its SetWorld() function
	if (Mission == NULL)
		Mission = BuildMission();

	Mission->SetWorld(nextWorld);
}

void DLLAPI ProcessCommand(unsigned long crc)
{
	Mission->ProcessCommand(crc);
}

void DLLAPI SetRandomSeed(unsigned long seed)
{
	Mission->SetRandomSeed(seed);
}

static MisnExport misnExport;
MisnImport misnImport;

MisnExport * __cdecl GetMisnAPI(MisnImport *import)
{
	misnImport = *import;
	memset(&misnExport, 0, sizeof(misnExport));
	misnExport.version = LATEST_DLL_VERSION;
	misnExport.VersionModifier = LATEST_DLL_VERSION_MODIFIER;
	misnExport.misnImport = &misnImport;
	misnExport.InitialSetup= InitialSetup;
	misnExport.Save = Save;
	misnExport.Load = Load;
	misnExport.PostLoad = PostLoad;
	misnExport.AddObject = AddObject;
	misnExport.DeleteObject = DeleteObject;
	misnExport.Update = Update;
	misnExport.PostRun = PostRun;
	misnExport.AddPlayer = AddPlayer;
	misnExport.DeletePlayer = DeletePlayer;
	misnExport.PlayerEjected = PlayerEjected;
	misnExport.ObjectKilled = ObjectKilled;
	misnExport.ObjectSniped = ObjectSniped;
	misnExport.GetNextRandomVehicleODF = GetNextRandomVehicleODF;
	misnExport.SetWorld = SetWorld;
	misnExport.ProcessCommand = ProcessCommand;
	misnExport.SetRandomSeed = SetRandomSeed;
	CurrentWorld = 0;
	return &misnExport;
}
