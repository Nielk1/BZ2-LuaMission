#include "DLLBase.h"

// Globals, shared code

static int CurrentWorld;
static DLLBase *Missions[3];
DLLBase *Mission;

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

// ------------------------ Interface to C++ Mission class ------------------------ 

void DLLAPI InitialSetup(void)
{
	if (Mission == NULL)
		Mission = BuildMission();
	Mission->InitialSetup();
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
